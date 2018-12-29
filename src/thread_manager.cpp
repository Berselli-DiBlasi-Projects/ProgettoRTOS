#include "../lib/thread_manager.h"

#define STATE_IDLE      0
#define STATE_ANALISYS  1

#define PER 20   /* task period in ms		*/
#define DREL 20  /* realtive deadline in ms	*/
#define PRIO 80  /* task priority		*/

using namespace std;
using namespace cv;

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

    pmux_create_pi(&g->mutex);
    pmux_create_pi(&g->sem_camera);
    pmux_create_pi(&g->sem_filter);
    pmux_create_pi(&g->sem_histo);
    pmux_create_pi(&g->sem_threshold);
    pmux_create_pi(&g->sem_difference);

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

void startCamera(struct gestore_t *g){
    pthread_mutex_lock(&g->mutex);

    g->ncam++;
    if(g->state == STATE_IDLE){
        pthread_mutex_unlock(&g->sem_camera);
        /*se c'è lo stato di attesa può partire l'analisi*/
    }
    pthread_mutex_unlock(&g->mutex);

    cout << "camera blocks" << endl;
    pthread_mutex_lock(&g->sem_camera);
    cout << "START CAMERA" << endl;
    //può effettuare l'analisi
    //g->captured_frame = takeAPicture(g->dim_frame);
}

void endCamera(struct gestore_t *g){
    pthread_mutex_lock(&g->mutex);
    cout << "END CAMERA" << endl;

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
    cout << "CAMERA USCITA" << endl;
}

void startHistogram(struct gestore_t *g){
    /*
    per prima cosa si blocca sul suo semaforo
    in questo modo si ha la certezza che, se la camera,
    non ha finito di prelevare il fotogramma esso non potrà
    continuare l'analisi
    successivamente blocca il mutex ed aggiorna il contatore
    */
    cout << "histogram blocks" << endl;
    pthread_mutex_lock(&g->sem_histo);
    cout << "START HISTOGRAM" << endl;
    
    //RICHIAMO FUNZIONE HISTOGRAMMA
}

void endHistogram(struct gestore_t *g){
    
    pthread_mutex_lock(&g->mutex);
    cout << "END HISTOGRAM" << endl;

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
    cout << "difference blocks" << endl;
    pthread_mutex_lock(&g->sem_difference);
    cout << "START DIFFERENCE" << endl;
    
    //RICHIAMO FUNZIONE DIFFERENCE
}

void endDifference(struct gestore_t *g){
    
    pthread_mutex_lock(&g->mutex);
    cout << "END DIFFERENCE" << endl;
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
    cout << "threshold blocks" << endl;
    pthread_mutex_lock(&g->sem_threshold);
    cout << "START THRESHOLD" << endl;
    
    //RICHIAMO FUNZIONE THRESHOLD
}

void endThreshold(struct gestore_t *g){
    
    pthread_mutex_lock(&g->mutex);
    cout << "END THRESHOLD" << endl;
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
    cout << "filter blocks" << endl;
    pthread_mutex_lock(&g->sem_filter);
    cout << "START FILTER" << endl;
    
    //RICHIAMO FUNZIONE FILTER
}

void endFilter(struct gestore_t *g){
    
    pthread_mutex_lock(&g->mutex);
    cout << "END FILTER" << endl;

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
        //ptask_wait_for_period();
    }
}

void bodyDifference(){
    while(1)
    {
        startDifference(&gestore);
        setOutDifference(frameDifference(getOutCamera()));
        endDifference(&gestore);
        //ptask_wait_for_period();
    }
}

void bodyFilter(){
    while(1)
    {
        startFilter(&gestore);
        setOutFilter(filterFrame(getOutCamera()));
        endFilter(&gestore);
        //ptask_wait_for_period();
    }
}

void bodyThreshold(){
    while(1)
    {
        startThreshold(&gestore);
        setOutThreshold(threshold(getOutCamera()));
        endThreshold(&gestore);
        //ptask_wait_for_period();
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
        /* a round robin assignment */
        params[i].processor = last_proc++;
        if (last_proc >= max_proc)
            last_proc = 0;
    }
    
    int ret;

    ret = ptask_create_param(bodyFilter, &params[0]);
    if (ret != -1) {
        printf("Task %d created and activated\n", ret);
    } else {
        printf("Error in creating task!\n");
    }
    ret = ptask_create_param(bodyDifference, &params[0]);
    if (ret != -1) {
        printf("Task %d created and activated\n", ret);
    } else {
        printf("Error in creating task!\n");
    }
    ret = ptask_create_param(bodyThreshold, &params[0]);
    if (ret != -1) {
        printf("Task %d created and activated\n", ret);
    } else {
        printf("Error in creating task!\n");
    }
    ret = ptask_create_param(bodyHistogram, &params[0]);
    if (ret != -1) {
        printf("Task %d created and activated\n", ret);
    } else {
        printf("Error in creating task!\n");
    }

    while(true)
    {

        bodyCamera();
        
        imshow("Camera", getOutCamera());

        if(!frmSettings->is_visible())
        {
            destroyAllWindows();
            break;
        }

        waitKey(17);
    }
}