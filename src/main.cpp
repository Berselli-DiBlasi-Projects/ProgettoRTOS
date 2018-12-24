#include "../lib/image_processing.h"
#include "pmutex.h"
#include "ptask.h"
#include "tstat.h"
using namespace std;

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

    //memoria condivisa
    Mat captured_frame; 

} gestore;

void initGestore(struct gestore_t g) {
    
    pmux_create_pi(&g->mutex);
    pmux_create_pi(&g->sem_camera);
    pmux_create_pi(&g->sem_filter);
    pmux_create_pi(&g->sem_histo);
    pmux_create_pi(&g->sem_threshold);
    pmux_create_pi(&g->sem_difference);

    ptask_init(SCHED_FIFO, GLOBAL, PRIO_INHERITANCE);
}
/*
body del thread camera
*/
void bodyCamera(){

}

int main(int argc, char** argv) {

    //gtk code
	  show_main_frame("../glade/frmMain.glade", "frmMain",
		  "Image Processing", "../media/image_processing.png", argc, argv);

  

    return 0;
}

/*--------------------------------------------------------------*/
