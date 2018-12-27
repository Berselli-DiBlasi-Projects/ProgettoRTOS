#include "../lib/image_processing.h"
extern "C" {
#include "pmutex.h"
#include "ptask.h"
#include "tstat.h"
}
using namespace std;

#define STATE_IDLE      0
#define STATE_ANALISYS  1;


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
    int nth;    //numero thread di analisi attivi
    int bth;    //numero thread di analisi bloccati
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

    g->nth = g->bth = g->ncam = 0 = g->bcam = 0;

    g->state=STATE_IDLE;

    ptask_init(SCHED_FIFO, GLOBAL, PRIO_INHERITANCE);
}


void startCamera(gestore_t g){
    
    pthread_mutex_lock(&g->mutex);
    g->ncam++;
    if(g->state == STATE_IDLE){
        /*se c'è lo stato di attesa può partire l'analisi*/
        g->captured_frame = takeAPicture

    }
}

int main(int argc, char** argv) {

    //gtk code
	  show_main_frame("../glade/frmMain.glade", "frmMain",
		  "Image Processing", "../media/image_processing.png", argc, argv);

  

    return 0;
}

/*--------------------------------------------------------------*/
