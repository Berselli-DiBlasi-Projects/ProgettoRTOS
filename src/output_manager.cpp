#include "../lib/image_processing.h"


Mat istogramma;
Mat filtro;
Mat diff;
Mat thr;
Mat cam;
pthread_mutex_t sem_camera, 
                sem_histo, 
                sem_filter, 
                sem_threshold,
                sem_difference;


void outputInit(){
    //pmux_create_pi(&out->mutex);
    pmux_create_pi(&sem_camera);
    pmux_create_pi(&sem_filter);
    pmux_create_pi(&sem_histo);
    pmux_create_pi(&sem_threshold);
    pmux_create_pi(&sem_difference);
}
/**
 * prende in input l'istogramma da mandare a video
*/
void setOutPlotIstogram(Mat img){
    pthread_mutex_lock(&sem_histo);
    istogramma=img.clone();
    pthread_mutex_unlock(&sem_histo);
}

/**
 * preleva l'output da visualizzare a video
*/
Mat getOutPlotIstogram(Mat img){
    Mat r;
    pthread_mutex_lock(&sem_histo);
    r=istogramma.clone();
    pthread_mutex_unlock(&sem_histo);
    return r;
}

void setOutFilter(Mat img){
    pthread_mutex_lock(&sem_filter);
    filtro=img.clone();
    pthread_mutex_unlock(&sem_filter);
}

Mat getOutFilter(Mat img){
    Mat r;
    pthread_mutex_lock(&sem_filter);
    r=filter.clone();
    pthread_mutex_unlock(&sem_filter);
    return r;
}


void setOutDifference(Mat img){
    pthread_mutex_lock(&sem_difference);
    diff=img.clone();
    pthread_mutex_unlock(&sem_difference);
}

Mat getOutDifference(Mat img){
    Mat r;
    pthread_mutex_lock(&sem_difference);
    r=diff.clone();
    pthread_mutex_unlock(&sem_difference);
    return r;
}



void setOutThreshold(Mat img){
    pthread_mutex_lock(&sem_threshold);
    thr=img.clone();
    pthread_mutex_unlock(&sem_threshold);
}

Mat getOutThreshold(Mat img){
    Mat r;
    pthread_mutex_lock(&sem_difference);
    r=thr.clone();
    pthread_mutex_unlock(&sem_difference);
    return r;
}


void setOutCamera(Mat img){
    pthread_mutex_lock(&sem_camera);
    cam=img.clone();
    pthread_mutex_unlock(&sem_camera);
}

Mat getOutDifference(Mat img){
    Mat r;
    pthread_mutex_lock(&sem_camera);
    r=cam.clone();
    pthread_mutex_unlock(&sem_camera);
    return r;
}