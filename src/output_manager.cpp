#include "../lib/image_processing.h"


Mat histogram;
Mat filter;
Mat diff;
Mat thr;
Mat cam;
pthread_mutex_t sem_camera, 
                sem_histo, 
                sem_filter, 
                sem_threshold,
                sem_difference;


void outputInit(){
    pmux_create_pi(&sem_camera);
    pmux_create_pi(&sem_filter);
    pmux_create_pi(&sem_histo);
    pmux_create_pi(&sem_threshold);
    pmux_create_pi(&sem_difference);
}

/**
 * Preleva il frame camera da stampare a video.
 * @param   : None
 * @return  : Mat r; frame camera ritornato.
*/
Mat getOutCamera(){
    Mat r;
    pthread_mutex_lock(&sem_camera);
    r=cam.clone();
    pthread_mutex_unlock(&sem_camera);
    return r;
}

/**
 * Preleva il frame difference da stampare a video.
 * @param   : None
 * @return  : Mat r; frame Difference ritornato.
*/
Mat getOutDifference(){
    Mat r;
    pthread_mutex_lock(&sem_difference);
    r=diff.clone();
    pthread_mutex_unlock(&sem_difference);
    return r;
}

/**
 * Preleva il frame filtro da stampare a video.
 * @param   : None
 * @return  : Mat r; frame filtro ritornato.
*/
Mat getOutFilter(){
    Mat r;
    pthread_mutex_lock(&sem_filter);
    r=filter.clone();
    pthread_mutex_unlock(&sem_filter);
    return r;
}

/**
 * Preleva il frame istogramma da stampare a video.
 * @param   : None
 * @return  : Mat r; frame istogramma ritornato.
*/
Mat getOutPlotHistogram(){
    Mat r;
    pthread_mutex_lock(&sem_histo);
    r=histogram.clone();
    pthread_mutex_unlock(&sem_histo);
    return r;
}

/**
 * Preleva il frame threshold da stampare a video.
 * @param   : None
 * @return  : Mat r; frame threshold ritornato.
*/
Mat getOutThreshold(){
    Mat r;
    pthread_mutex_lock(&sem_threshold);
    r=thr.clone();
    pthread_mutex_unlock(&sem_threshold);
    return r;
}

/**
 * Setta il frame camera da stampare a video.
 * @param   : Mat img; frame Camera da settare.
 * @return  : None
*/
void setOutCamera(Mat img){
    pthread_mutex_lock(&sem_camera);
    cam=img.clone();
    pthread_mutex_unlock(&sem_camera);
}

/**
 * Setta il frame difference da stampare a video.
 * @param   : Mat img; frame Difference da settare.
 * @return  : None
*/
void setOutDifference(Mat img){
    pthread_mutex_lock(&sem_difference);
    diff=img.clone();
    pthread_mutex_unlock(&sem_difference);
}

/**
 * Setta il frame filtro da stampare a video.
 * @param   : Mat img; frame filtro da settare.
 * @return  : None
*/
void setOutFilter(Mat img){
    pthread_mutex_lock(&sem_filter);
    filter=img.clone();
    pthread_mutex_unlock(&sem_filter);
}

/**
 * Setta il frame istogramma da stampare a video.
 * @param   : Mat img; frame istogramma da settare.
 * @return  : None
*/
void setOutPlotHistogram(Mat img){
    pthread_mutex_lock(&sem_histo);
    histogram=img.clone();
    pthread_mutex_unlock(&sem_histo);
}

/**
 * Setta il frame threshold da stampare a video.
 * @param   : Mat img; frame Threshold da settare.
 * @return  : None
*/
void setOutThreshold(Mat img){
    pthread_mutex_lock(&sem_threshold);
    thr=img.clone();
    pthread_mutex_unlock(&sem_threshold);
}