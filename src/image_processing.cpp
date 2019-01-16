#include "../lib/image_processing.h"

using namespace std;
using namespace cv;

bool cancel_signal = false;
bool execution_started = false;
int filter_choice = 0;
int frame_difference_value = 0;
int frame_scaling_value = 100;
int threshold_type;
int threshold_value;

/**
 * Ritorna il bool execution_started, di supporto all'esecuzione.
 * @param   : none
 * @return  : bool
*/
bool getExecutionStarted()
{
    return execution_started;
}

/**
 * Ritorna il valore di filter_choice per il filtering.
 * @param   : none
 * @return  : int
*/
int getFilterChoice()
{
    return filter_choice;
}

/**
 * Ritorna valore di frame_difference_value, di supporto all'esecuzione.
 * @param   : none
 * @return  : int
*/
int getFrameDifferenceValue()
{
    return frame_difference_value;
}


/**
 * Ritorna valore di frame_scaling_value, di supporto all'esecuzione.
 * @param   : none
 * @return  : int
*/
int getFrameScalingValue()
{
    return frame_scaling_value;
}


/**
 * Ritorna valore di threshold type per il threshold.
 * @param   : none
 * @return  : int
*/
int getThresholdType()
{
    return threshold_type;
}

/**
 * Ritorna valore di threshold per il thread di threshold.
 * @param   : none
 * @return  : int
*/
int getThresholdValue()
{
    return threshold_value;
}

/**
 * Riduce le dimensioni dell'immagine prendendo come 
 * come parametro di riduzione quello impostato dall'utente
 * in frame settings
 * @param : matrice da ridimensionare
 * @return : resized Mat 
*/
Mat imageScale(Mat src){
    float reduction = (float ) frame_scaling_value;
    Mat dst;

    if(reduction != 100)
    {
        reduction = reduction / 100;
        cv::Size s = src.size();
        s.height = s.height * reduction;
        s.width = s.width * reduction;
        cv::resize(src, dst, s);
    }
	else
        dst = src.clone();
	
    return dst;

}

/**
 * Crea le finestre di preview dell'esecuzione.
 * @param   : frmMain; riferimento al frmMain chiamante.
 * @return  : void
*/
void preview(FrmMain *frmMain)
{
    Rect2d rect = getRect();
    ScreenShot screen(rect.x, rect.y, rect.width, rect.height);
    Mat imgCamera;
	string titleCamera = "Camera";
    screen(imgCamera);
	setCancelSignal(false);

    Mat placeholder_image, placeholder_image_resized;
    placeholder_image = imread("../media/placeholder.png",
                                IMREAD_COLOR);

    resize(placeholder_image, placeholder_image_resized,
           rect.size(), 0, 0, cv::INTER_CUBIC);

    string placeholder_titles[4];
    placeholder_titles[0] = "Histogram";
    placeholder_titles[1] = "Filter";
    placeholder_titles[2] = "Frame difference";
    placeholder_titles[3] = "Threshold";

    time_t start = time(0);
    time_t time_taken = 0;
    int count = 0;
    
    while(true)
	{
        time_t end = time(0);

        if(time_taken != end - start) {
            cout << "FPS: " << count << endl;
            count = 0;
        }
        else
            count++;

        time_taken = end - start;

		screen(imgCamera);
    	imshow(titleCamera, imgCamera);

        //Mostra le finestre dei placeholders
        for(int i = 0; i < 
            sizeof(placeholder_titles) / sizeof(placeholder_titles[0]); i++)
            imshow(placeholder_titles[i], placeholder_image_resized);
        
        if(!frmMain->is_visible() || cancel_signal)
        {
            destroyAllWindows();
            break;
        }
        waitKey(16); //16 ms -> limit to 60 FPS.
    }
}

/**
 * Permette all'utente di selezionare l'area di schermo da registrare.
 * @param   : Gtk::Label *lblState; label di stato del frame FrmMain.
 * @return  : Rect2d; Il rettangolo che definisce l'area scelta.
*/
void selectRegion(Gtk::Label *lblState)
{
    Display* disp = XOpenDisplay(NULL);
    Screen*  scrn = DefaultScreenOfDisplay(disp);
    int width  = scrn->width;
    int height = scrn->height;
    ScreenShot screen(0, 0, scrn->width, scrn->height);
    Mat img;
    Rect2d rect;
	string title = "Select area (SPACE or ENTER to confirm)";
    screen(img);
    
    do {
        rect = selectROI(title, img);

        if(rect.width > MAX_WIDTH || rect.height > MAX_HEIGHT)
            cout << "Error: maximum ROI dimensions exceeded. (MAX_WIDTH: "<<
                MAX_WIDTH << ", MAX_HEIGHT: " << MAX_HEIGHT << ")" << endl;

    } while(rect.width > MAX_WIDTH || rect.height > MAX_HEIGHT);
    destroyWindow(title);
    lblState->set_text("Confirm to display a\npreview of the execution.");

    setRect(rect);
}

/**
 * Setta il bool cancel_signal, di supporto alla funzione preview.
 * @param   : bool signal; Il booleano da utilizzare.
 * @return  : void
*/
void setCancelSignal(bool signal)
{
    cancel_signal = signal;
}

/**
 * Setta il bool execution_started, di supporto all'esecuzione.
 * @param   : bool started; Il booleano da utilizzare.
 * @return  : void
*/
void setExecutionStarted(bool started)
{
    execution_started = started;
}

/**
 * Setta il valore di filter_choice per il filtering.
 * @param   : int; choice
 * @return  : void
*/
void setFilterChoice(int choice)
{
    filter_choice = choice;
}

/**
 * Setta il valore di frame_difference, di supporto all'esecuzione.
 * @param   : int difference; Il valore da utilizzare.
 * @return  : void
*/
void setFrameDifferenceValue(int difference)
{
    frame_difference_value = difference;
}



/**
 * Setta il valore di frame_scaling, di supporto all'esecuzione.
 * @param   : int scaling; Il valore da utilizzare.
 * @return  : void
*/
void setFrameScalingValue(int difference)
{
    frame_scaling_value = difference;
}



/**
 * Setta il valore di threshold_type, di supporto all'esecuzione.
 * @param   : int value; Il valore da utilizzare.
 * @return  : void
*/
void setThresholdType(int value)
{
    threshold_type = value;
}

/**
 * Setta il valore di threshold, di supporto all'esecuzione.
 * @param   : int value; Il valore da utilizzare.
 * @return  : void
*/
void setThresholdValue(int value)
{
    threshold_value = value;
}