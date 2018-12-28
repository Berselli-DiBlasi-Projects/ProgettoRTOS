#include "../lib/image_processing.h"

/**
 * Applica un threshold all'immagine in ingresso e restituisce il risultato.
 * @param   : Mat img; Immagine in ingresso, l'ultima catturata dalla camera.
 * @return  : Mat; Immagine risultante dopo aver applicato il threshold.
*/
Mat threshold(Mat img){
    Mat img_out;
    int r = getR();
    int g = getG();
    int b = getB();
    
    inRange(img, Scalar(0, 0, 0), Scalar(r, g, b), img_out);
    
    return img_out;
}