#include "../lib/image_processing.h"

int frame_count = 0;
Mat img_old;
Mat img_out;
bool begin_difference = true;

/**
 * Fa la differenza assoluta fra due frame tenendo conto
 * della lunghezza di passo.
 * @param   : Mat img; Immagine in ingresso, l'ultima catturata dalla camera.
 * @return  : Mat; Immagine risultante dopo aver applicato la sottrazione.
*/
Mat frameDifference(Mat img){
    //Frame difference value -> da 0 a 30
    int frame_difference_value = getFrameDifferenceValue();

    if(frame_count == 0)
        img_old = img;

    if(begin_difference)
        img_out = img;

    if(frame_difference_value == frame_count)
    {
        absdiff(img_old, img, img_out);
        frame_count = 0;
    }
    else
    {
        frame_count++;
    }

    if(frame_difference_value < frame_count)
        frame_count = 0;

    begin_difference = false;
    
    return img_out;
}