#include "../lib/image_processing.h"


/*
void displayVideoStream(Rect2d rect)
{
    ScreenShot screen(rect.x, rect.y, rect.width, rect.height);
    Mat imgCamera;
	string titleCamera = "Camera";
    screen(imgCamera);
	while(true){
        screen(imgCamera);
    	imshow(titleCamera, imgCamera);
        if(!frm->is_visible())
            break;

        waitKey(1);
    }
}
*/
/**
 * Cattura dello schermo (deve prendere in ingresso l'area
 * tracciata dall'utente).
 * @param   : Rect2d rect; Definisce l'area di registrazione della camera.
 * @return  : void
*/
Mat takeAPicture(Rect2d rect){
    ScreenShot screen(rect.x, rect.y, rect.width, rect.height);
    Mat imgCamera;
	string titleCamera = "Camera";
    screen(imgCamera);
	return imgCamera;
}

