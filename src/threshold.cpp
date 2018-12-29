#include "../lib/image_processing.h"

/**
 * Applica un threshold all'immagine in ingresso e restituisce il risultato.
 * @param   : Mat img; Immagine in ingresso, l'ultima catturata dalla camera.
 * @return  : Mat; Immagine risultante dopo aver applicato il threshold.
*/
Mat threshold(Mat img){
    Mat img_out, img_gray;
    int r = getR();
    int g = getG();
    int b = getB();
    cout << r << endl;
    inRange(img, Scalar(r, g, b), Scalar(255, 255, 255), img_out);
    cvtColor(img, img_gray, CV_BGR2GRAY);
    threshold(img_gray, img_out, 0, 255, 3);
    
    return img_out;
}