#include "../lib/image_processing.h"

/**
 * Applica un threshold all'immagine in ingresso e restituisce il risultato.
 * @param   : Mat img; Immagine in ingresso, l'ultima catturata dalla camera.
 * @return  : Mat; Immagine risultante dopo aver applicato il threshold.
*/
Mat threshold(Mat img){
    Mat img_out, img_gray;
    int threshold_type = getThresholdType();
    int threshold_value = getThresholdValue();

    //cvtColor(img, img_gray, CV_BGR2GRAY);
    threshold(img, img_out, threshold_value, 255, threshold_type);
    
    return img_out;
}