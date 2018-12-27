#include "../lib/image_processing.h"
extern "C" {
#include "pmutex.h"
#include "ptask.h"
#include "tstat.h"
}

using namespace std;

#define STATE_IDLE      0
#define STATE_ANALISYS  1
#define STATE_CAMERA    2

/*--------------------------------------------------------------*/
/*		            	MAIN process				            */
/*--------------------------------------------------------------*/
struct gestore_t{
    //mutua esclusione
    pthread_mutex_t mutex;
    
    //private semaphore
    pthread_mutex_t sem_camera, 
                    sem_histo, 
                    sem_filter, 
                    sem_threshold,
                    sem_difference;
    int nhist, ndiff, nthres, nfilter;  //numero thread di analisi attivi
    int bhist, bdiff, bthres, bfilter;  //numero thread di analisi bloccati
    int ncam;   //numero camere attive
    int bcam;   //numero camere bloccate
    int state;  //stato del sistema

    //memoria condivisa
    Mat     captured_frame; // immagine da analizzare
    Rect2d  dim_frame;      // dimensione frame da analizzare

} gestore;

void initGestore(struct gestore_t *g) {
    
    pmux_create_pi(&g->mutex);
    pmux_create_pi(&g->sem_camera);
    pmux_create_pi(&g->sem_filter);
    pmux_create_pi(&g->sem_histo);
    pmux_create_pi(&g->sem_threshold);
    pmux_create_pi(&g->sem_difference);

    g->ncam = g->bcam = 0;

    g->nhist = g->ndiff = g->nthres = g->nfilter = 0;
    g->bhist = g->bdiff = g->bthres = g->bfilter = 0;

    g->state=STATE_IDLE;

    ptask_init(SCHED_FIFO, GLOBAL, PRIO_INHERITANCE);
}


void startCamera(gestore_t *g){
    
    pthread_mutex_lock(&g->mutex);
    g->ncam++;
    if(g->state == STATE_IDLE){
        pthread_mutex_unlock(&g->sem_camera);
        g->state = STATE_CAMERA;
        /*se c'è lo stato di attesa può partire l'analisi*/
    }else{
        //i thread stanno analizzando il vecchio frame
        g->bcam++;
        pthread_mutex_unlock(&g->mutex);
    }
    pthread_mutex_unlock(&g->mutex);

    pthread_mutex_lock(&g->sem_camera);
    //può effettuare l'analisi
    //g->captured_frame = takeAPicture(g->dim_frame);
}

void endCamera(gestore_t *g){
    /*
    
    */
    pthread_mutex_lock(&g->mutex);
    g->ncam--;
    g->state = STATE_ANALISYS;
    pthread_mutex_unlock(&g->mutex);
    
    pthread_mutex_unlock(&g->sem_histo);
    pthread_mutex_unlock(&g->sem_difference);
    pthread_mutex_unlock(&g->sem_filter);
    pthread_mutex_unlock(&g->sem_threshold);
    /*quando la telecamera finisce sveglia i thread di analisi
    */
    pthread_mutex_unlock(&g->mutex);

 
}

void startHistogram(gestore_t *g){
    /*
    per prima cosa si blocca sul suo semaforo
    in questo modo si ha la certezza che, se la camera,
    non ha finito di prelevare il fotogramma esso non potrà
    continuare l'analisi
    successivamente blocca il mutex ed aggiorna il contatore
    */
    pthread_mutex_lock(&g->sem_histo);
    pthread_mutex_lock(&g->mutex);
    g->nhist++;
    pthread_mutex_unlock(&g->mutex);
    
    //RICHIAMO FUNZIONE HISTOGRAMMA
}



void endHistogram(gestore_t *g){
    
    pthread_mutex_lock(&g->mutex);
    g->nhist--;
    if(!g->ndiff && !g->nthres && !g->nfilter){
        //non c'è nessun thread di analisi attivo   
        g->state=STATE_IDLE;
    }
    pthread_mutex_unlock(&g->mutex);
}

void startDifference(gestore_t *g){
    /*
    per prima cosa si blocca sul suo semaforo
    in questo modo si ha la certezza che, se la camera,
    non ha finito di prelevare il fotogramma esso non potrà
    continuare l'analisi
    successivamente blocca il mutex ed aggiorna il contatore
    */
    pthread_mutex_lock(&g->sem_difference);
    pthread_mutex_lock(&g->mutex);
    g->ndiff++;
    pthread_mutex_unlock(&g->mutex);
    
    //RICHIAMO FUNZIONE DIFFERENCE
}



void endDifference(gestore_t *g){
    
    pthread_mutex_lock(&g->mutex);
    g->ndiff--;
    if(!g->nhist && !g->nthres && !g->nfilter){
        //non c'è nessun thread di analisi attivo   
        g->state=STATE_IDLE;
    }
    pthread_mutex_unlock(&g->mutex);
}

void startThreshold(gestore_t *g){
    /*
    per prima cosa si blocca sul suo semaforo
    in questo modo si ha la certezza che, se la camera,
    non ha finito di prelevare il fotogramma esso non potrà
    continuare l'analisi
    successivamente blocca il mutex ed aggiorna il contatore
    */
    pthread_mutex_lock(&g->sem_threshold);
    pthread_mutex_lock(&g->mutex);
    g->nthres++;
    pthread_mutex_unlock(&g->mutex);
    
    //RICHIAMO FUNZIONE THRESHOLD
}



void endThreshold(gestore_t *g){
    
    pthread_mutex_lock(&g->mutex);
    g->nthres--;
    if(!g->nhist && !g->ndiff && !g->nfilter){
        //non c'è nessun thread di analisi attivo   
        g->state=STATE_IDLE;
    }
    pthread_mutex_unlock(&g->mutex);
}


void startFilter(gestore_t *g){
    /*
    per prima cosa si blocca sul suo semaforo
    in questo modo si ha la certezza che, se la camera,
    non ha finito di prelevare il fotogramma esso non potrà
    continuare l'analisi
    successivamente blocca il mutex ed aggiorna il contatore
    */
    pthread_mutex_lock(&g->sem_filter);
    pthread_mutex_lock(&g->mutex);
    g->nfilter++;
    pthread_mutex_unlock(&g->mutex);
    
    //RICHIAMO FUNZIONE FILTER
}



void endFilter(gestore_t *g){
    
    pthread_mutex_lock(&g->mutex);
    g->nfilter--;
    if(!g->nhist && !g->nthres && !g->ndiff){
        //non c'è nessun thread di analisi attivo   
        g->state=STATE_IDLE;
    }
    pthread_mutex_unlock(&g->mutex);
}


/**
 * Visualizza il frame FrmMain.
 * @param   : string glade_file; Il percorso del file glade.
 *            string frame_id; Id della finestra.
 *            string title; Titolo della finestra.
 *            string icon_file; Icona della finestra.
 *            int argc; argc del main
 *            char** argv; argv del main
 * @return  : void
*/
void show_main_frame(string glade_file, string frame_id, string title,
     string icon_file, int argc, char** argv)
{
    cout << "Frame main Started" << endl;
    FrmMain *frm = 0;
	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
    Glib::RefPtr<Gtk::Builder> builder = 
        Gtk::Builder::create_from_file(glade_file);

	builder->get_widget_derived(frame_id, frm);
	frm->set_title(title);
	frm->set_icon_from_file(icon_file);
	frm->property_resizable() = false;
	app->run(*frm);

	cout << "Frame main End" << endl;
}

/**
 * Visualizza il frame FrmSettings.
 * @param   : string glade_file; Il percorso del file glade.
 *            string frame_id; Id della finestra.
 *            string title; Titolo della finestra.
 *            string icon_file; Icona della finestra.
 *            int argc; argc del main
 *            char** argv; argv del main
 * @return  : void
*/
void show_settings_frame(string glade_file, string frame_id, string title,
     string icon_file, int argc, char** argv)
{
    cout << "Frame settings Started" << endl;
    FrmSettings *frm = 0;
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
	Glib::RefPtr<Gtk::Builder> builder = 
        Gtk::Builder::create_from_file(glade_file);

	builder->get_widget_derived(frame_id, frm);
	frm->set_title(title);
	frm->set_icon_from_file(icon_file);
	frm->property_resizable() = false;
	app->run(*frm);

	cout << "Frame settings End" << endl;
}

int main(int argc, char** argv) {

    //gtk code
	show_main_frame("../glade/frmMain.glade", "frmMain",
                    "Image Processing", "../media/image_processing.png", argc, argv);

    if(getExecutionStarted())
        show_settings_frame("../glade/frmSettings.glade", "frmSettings",
                            "Settings", "../media/image_processing.png", argc, argv);

    return 0;
}

/*--------------------------------------------------------------*/
