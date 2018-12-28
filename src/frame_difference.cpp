#include "../lib/image_processing.h"

int count = 0;
Mat img_old = NULL;
Mat img_out = NULL;

/**
 * Fa la differenza assoluta fra due frame tenendo conto
 * della lunghezza di passo.
 * @param   : Mat img; Immagine in ingresso, l'ultima catturata dalla camera.
 * @return  : Mat; Immagine risultante dopo aver applicato la sottrazione.
*/
Mat frameDifference(Mat img){
    //Frame difference value -> da 0 a 30
    int frame_difference_value = getFrameDifferenceValue();

    if(img_old == NULL || count == 0)
        img_old = img;

    if(img_out == NULL)
        img_out = img;

    if(frame_difference_value == count)
    {
        absdiff(img_old, img, img_out);
        count = 0;
    }
    else
    {
        count++;
    }

    if(frame_difference_value < count)
        count = 0;
    
    return img_out;
}