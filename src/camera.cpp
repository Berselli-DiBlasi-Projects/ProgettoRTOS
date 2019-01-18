#include "../lib/image_processing.h"

/**
 * Cattura dello schermo (deve prendere in ingresso l'area
 * tracciata dall'utente).
 * @param   : Rect2d rect; Definisce l'area di registrazione della camera.
 * @return  : void
*/

Mat takeAPicture(Rect2d rect){
    ScreenShot screen(rect.x, rect.y, rect.width, rect.height);
    Mat imgCamera, ret;
    int nbit = getBitForChannelValue();
    screen(imgCamera);
    if (nbit != 0){
        ret = quantizeImage(imgCamera, nbit).clone();
    }else{
        ret = imgCamera.clone();
    }
	return ret;
}

Mat quantizeImage(Mat inImage, int numBits){
    cv::Mat retImage = inImage.clone();

    uchar maskBit = 0xFF;

    maskBit = maskBit << (8 - numBits);

    for(int j = 0; j < retImage.rows; j++)
        for(int i = 0; i < retImage.cols; i++)
        {
            cv::Vec3b valVec = retImage.at<cv::Vec3b>(j, i);
            valVec[0] = valVec[0] & maskBit;
            valVec[1] = valVec[1] & maskBit;
            valVec[2] = valVec[2] & maskBit;
            retImage.at<cv::Vec3b>(j, i) = valVec;
        }

    return retImage;
}