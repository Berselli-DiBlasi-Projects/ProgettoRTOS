#include "../lib/image_processing.h"


/**
 * Cattura dello schermo (deve prendere in ingresso l'area tracciata dall'utente).
 * @param   : Rect2d rect; Definisce l'area di registrazione della camera.
 * @return  : void
*/
void takeAPicture(Rect2d rect)
{
    ScreenShot screen(rect.x, rect.y, rect.width, rect.height);
    Mat imgCamera;
	string titleCamera = "Camera";
    screen(imgCamera);
	/*
    la camera cattura frame alla massima velocità
    inserisce lo screenshot nell'oggetto del gestore della
    memoria condivisa.
    
    */
    while(true)
	{

		screen(imgCamera);
    	//--- questo pezzo andrà cancellato ----
        imshow(titleCamera, imgCamera);
        //--- fine codice da cancellare     ----

        if(!frm->is_visible())
            break;

        waitKey(1);
    }
}

