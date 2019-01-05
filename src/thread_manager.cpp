#include "../lib/thread_manager.h"

#define STATE_IDLE      0
#define STATE_ANALISYS  1

#define PER 20   /* task period in ms		*/
#define DREL 20  /* realtive deadline in ms	*/
#define PRIO 80  /* task priority		*/

using namespace std;
using namespace cv;

int fps_value;

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
    int state;  //stato del sistema

} gestore;

void initGestore(struct gestore_t *g) {

    pthread_mutex_init(&g->mutex, NULL);
    pthread_mutex_init(&g->sem_camera, NULL);
    pthread_mutex_init(&g->sem_filter, NULL);
    pthread_mutex_init(&g->sem_histo, NULL);
    pthread_mutex_init(&g->sem_threshold, NULL);
    pthread_mutex_init(&g->sem_difference, NULL);
    /*pmux_create_pi(&g->mutex);
    pmux_create_pi(&g->sem_camera);
    pmux_create_pi(&g->sem_filter);
    pmux_create_pi(&g->sem_histo);
    pmux_create_pi(&g->sem_threshold);
    pmux_create_pi(&g->sem_difference);*/

    g->ncam = 0;

    g->nhist = g->ndiff = g->nthres = g->nfilter = 0;
    g->bhist = g->bdiff = g->bthres = g->bfilter = 0;

    pthread_mutex_lock(&g->sem_camera);
    pthread_mutex_lock(&g->sem_filter);
    pthread_mutex_lock(&g->sem_histo);
    pthread_mutex_lock(&g->sem_threshold);
    pthread_mutex_lock(&g->sem_difference);

    g->state=STATE_IDLE;

    ptask_init(SCHED_RR, GLOBAL, NO_PROTOCOL);
}

/**
 * Ritorna il valore di fps scelti dall'utente.
 * @param   : none
 * @return  : int
*/
int getFpsValue()
{
    return fps_value;
}

/**
 * Setta il valore di fps scelto dall'utente.
 * @param   : int; choice
 * @return  : void
*/
void setFpsValue(int fps)
{
    fps_value = fps;
}

void startCamera(struct gestore_t *g){
    pthread_mutex_lock(&g->mutex);

    g->ncam++;
    if(g->state == STATE_IDLE){
        pthread_mutex_unlock(&g->sem_camera);
        /*se c'è lo stato di attesa può partire l'analisi*/
    }
    pthread_mutex_unlock(&g->mutex);

    pthread_mutex_lock(&g->sem_camera);
    //può effettuare l'analisi
    //g->captured_frame = takeAPicture(g->dim_frame);
}

void endCamera(struct gestore_t *g){
    pthread_mutex_lock(&g->mutex);

    g->ncam--;
    g->state = STATE_ANALISYS;
    g->nhist++;
    g->nthres++;
    g->ndiff++;
    g->nfilter++;

    pthread_mutex_unlock(&g->sem_histo);
    pthread_mutex_unlock(&g->sem_difference);
    pthread_mutex_unlock(&g->sem_filter);
    pthread_mutex_unlock(&g->sem_threshold);
    /*quando la telecamera finisce sveglia i thread di analisi
    */

    pthread_mutex_unlock(&g->mutex);
}

void startHistogram(struct gestore_t *g){
    /*
    per prima cosa si blocca sul suo semaforo
    in questo modo si ha la certezza che, se la camera,
    non ha finito di prelevare il fotogramma esso non potrà
    continuare l'analisi
    successivamente blocca il mutex ed aggiorna il contatore
    */
    pthread_mutex_lock(&g->sem_histo);
    
    //RICHIAMO FUNZIONE HISTOGRAMMA
}

void endHistogram(struct gestore_t *g){
    
    pthread_mutex_lock(&g->mutex);

    g->nhist--;
    if(!g->ndiff && !g->nthres && !g->nfilter){
        //non c'è nessun thread di analisi attivo   
        g->state=STATE_IDLE;
        pthread_mutex_unlock(&g->sem_camera);
    }
    pthread_mutex_unlock(&g->mutex);
}

void startDifference(struct gestore_t *g){
    /*
    per prima cosa si blocca sul suo semaforo
    in questo modo si ha la certezza che, se la camera,
    non ha finito di prelevare il fotogramma esso non potrà
    continuare l'analisi
    successivamente blocca il mutex ed aggiorna il contatore
    */
    pthread_mutex_lock(&g->sem_difference);
    
    //RICHIAMO FUNZIONE DIFFERENCE
}

void endDifference(struct gestore_t *g){
    
    pthread_mutex_lock(&g->mutex);
    g->ndiff--;
    if(!g->nhist && !g->nthres && !g->nfilter){
        //non c'è nessun thread di analisi attivo   
        g->state=STATE_IDLE;
        pthread_mutex_unlock(&g->sem_camera);
    }
    pthread_mutex_unlock(&g->mutex);
}

void startThreshold(struct gestore_t *g){
    /*
    per prima cosa si blocca sul suo semaforo
    in questo modo si ha la certezza che, se la camera,
    non ha finito di prelevare il fotogramma esso non potrà
    continuare l'analisi
    successivamente blocca il mutex ed aggiorna il contatore
    */
    pthread_mutex_lock(&g->sem_threshold);
    
    //RICHIAMO FUNZIONE THRESHOLD
}

void endThreshold(struct gestore_t *g){
    
    pthread_mutex_lock(&g->mutex);
    g->nthres--;
    if(!g->nhist && !g->ndiff && !g->nfilter){
        //non c'è nessun thread di analisi attivo   
        g->state=STATE_IDLE;
        pthread_mutex_unlock(&g->sem_camera);
    }
    pthread_mutex_unlock(&g->mutex);
}

void startFilter(struct gestore_t *g){
    /*
    per prima cosa si blocca sul suo semaforo
    in questo modo si ha la certezza che, se la camera,
    non ha finito di prelevare il fotogramma esso non potrà
    continuare l'analisi
    successivamente blocca il mutex ed aggiorna il contatore
    */
    pthread_mutex_lock(&g->sem_filter);
    
    //RICHIAMO FUNZIONE FILTER
}

void endFilter(struct gestore_t *g){
    
    pthread_mutex_lock(&g->mutex);

    g->nfilter--;
    if(!g->nhist && !g->nthres && !g->ndiff){
        //non c'è nessun thread di analisi attivo   
        g->state=STATE_IDLE;
        pthread_mutex_unlock(&g->sem_camera);
    }
    pthread_mutex_unlock(&g->mutex);
}

void bodyCamera(){
    startCamera(&gestore);
    setOutCamera(takeAPicture(getRect()));
    endCamera(&gestore);
}

void bodyHistogram(){
    while(1)
    {
        startHistogram(&gestore);
        setOutPlotHistogram(plotHistogram(getOutCamera()));
        endHistogram(&gestore);
    }
}

void bodyDifference(){
    while(1)
    {
        startDifference(&gestore);
        setOutDifference(frameDifference(getOutCamera()));
        endDifference(&gestore);
    }
}

void bodyFilter(){
    while(1)
    {
        startFilter(&gestore);
        setOutFilter(filterFrame(getOutCamera()));
        endFilter(&gestore);
    }
}

void bodyThreshold(){
    while(1)
    {
        startThreshold(&gestore);
        setOutThreshold(threshold(getOutCamera()));
        endThreshold(&gestore);
    }
}

void runExecutionThreads(FrmSettings *frmSettings)
{
    tpars params[4]; //Parametri dei thread
    int last_proc = 0;                  /* last assigned processor      */
    int max_proc = ptask_getnumcores(); /* max number of procs  */

    //inizializzazione della memoria condivisa
    initGestore(&gestore);
    initOutput();

    for(int i = 0; i < 4; i++)
    {
        params[i] = TASK_SPEC_DFL;
        params[i].period = tspec_from(PER, MILLI);
        params[i].rdline = tspec_from(DREL, MILLI);
        params[i].priority = PRIO;
        params[i].measure_flag = 1;
        params[i].act_flag = NOW;
        params[i].processor = last_proc++;
        if (last_proc >= max_proc)
            last_proc = 0;
    }
    
    int ret;

    ret = ptask_create_param(bodyFilter, &params[0]);
    if (ret == -1) {
        printf("Error in creating task!\n");
    }
    ret = ptask_create_param(bodyDifference, &params[0]);
    if (ret == -1) {
        printf("Error in creating task!\n");
    }
    ret = ptask_create_param(bodyThreshold, &params[0]);
    if (ret == -1) {
        printf("Error in creating task!\n");
    }
    ret = ptask_create_param(bodyHistogram, &params[0]);
    if (ret == -1) {
        printf("Error in creating task!\n");
    }

    time_t start = time(0);
    time_t time_taken = 0;
    int count = 0;
    //Fa in modo che gli fps si avvicinino a quelli scelti dall'utente.
    int bias = 0;
    int base_wait;

    if(fps_value == 15)
        base_wait = 65;
    if(fps_value == 30)
        base_wait = 31;
    if(fps_value == 40)
        base_wait = 23;
    if(fps_value == 50)
        base_wait = 18;
    if(fps_value == 55)
        base_wait = 16;
    if(fps_value == 60)
        base_wait = 15;

    int wait_time = base_wait;

    

    while(true)
    {

       


        time_t end = time(0);

        if(time_taken != end - start) {
            cout << "FPS: " << count << endl;
            if(count < fps_value)
                bias--;
            else
                bias++;
            if(base_wait + bias > 5)
                wait_time = base_wait + bias;
            else
                wait_time = base_wait;
            count = 0;
        }
        else
            count++;

        time_taken = end - start;

        bodyCamera();
        
        
        imshow("Camera", getOutCameraScaled());
        imshow("Filter", getOutFilter());
        imshow("Frame difference", getOutDifference());
        imshow("Threshold", getOutThreshold());
        imshow("Histogram", getOutPlotHistogram());
        
        //cv::resize(img, im_out, cv::Size(), reduction , reduction);

        if(!frmSettings->is_visible())
        {
            destroyAllWindows();
            break;
        }

        waitKey(wait_time);
    }
}