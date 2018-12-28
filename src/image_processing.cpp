#include "../lib/image_processing.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace std;
using namespace cv;

bool cancel_signal = false;
bool execution_started = false;

int filter_choice = 0;

int frame_difference_value;

int r_value;
int g_value;
int b_value;

struct ScreenShot
{
    ScreenShot(int x, int y, int width, int height):
        x(x),
        y(y),
        width(width),
        height(height)
    {
        display = XOpenDisplay(nullptr);
        root = DefaultRootWindow(display);

        init = true;
    }

    void operator() (Mat& cvImg)
    {
        if(init == true)
            init = false;
        else
            XDestroyImage(img);

        img = XGetImage(display, root, x, y, width, height, AllPlanes, ZPixmap);

        cvImg = Mat(height, width, CV_8UC4, img->data);
    }

    ~ScreenShot()
    {
        if(init == false)
            XDestroyImage(img);

        XCloseDisplay(display);
    }

    Display* display;
    Window root;
    int x,y,width,height;
    XImage* img;

    bool init;
};

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
 * Ritorna valore di frame_difference_value, di supporto all'esecuzione.
 * @param   : none
 * @return  : int
*/
int getFrameDifferenceValue()
{
    return frame_difference_value;
}

/**
 * Ritorna valore di R per il threshold.
 * @param   : none
 * @return  : int
*/
int getR()
{
    return r_value;
}

/**
 * Ritorna valore di G per il threshold.
 * @param   : none
 * @return  : int
*/
int getG()
{
    return g_value;
}

/**
 * Ritorna valore di B per il threshold.
 * @param   : none
 * @return  : int
*/
int getB()
{
    return b_value;
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
 * Crea le finestre di preview dell'esecuzione.
 * @param   : Rect2d rect; Definisce l'area di registrazione della camera.
 * @return  : void
*/
void preview(Rect2d rect, FrmMain *frmMain)
{
    ScreenShot screen(rect.x, rect.y, rect.width, rect.height);
    Mat imgCamera;
	string titleCamera = "Camera";
    screen(imgCamera);
	setCancelSignal(false);

    Mat placeholder_image;
    placeholder_image = imread("../media/placeholder.png",
                                IMREAD_COLOR);
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
            imshow(placeholder_titles[i], placeholder_image);
        
        if(!frmMain->is_visible() || cancel_signal)
        {
            destroyAllWindows();
            break;
        }
        waitKey(15); //17 ms -> limit to 60 FPS.
    }
}

/**
 * Permette all'utente di selezionare l'area di schermo da registrare.
 * @param   : Gtk::Label *lblState; label di stato del frame FrmMain.
 * @return  : Rect2d; Il rettangolo che definisce l'area scelta.
*/
Rect2d select_region(Gtk::Label *lblState)
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

    // Select ROI: rect.x, rect.y, rect.x + rect.width,
    // rect.y + regt.height
    do {
        rect = selectROI(title, img);

        if(rect.width > MAX_WIDTH || rect.height > MAX_HEIGHT)
            cout << "Error: maximum ROI dimensions exceeded. (MAX_WIDTH: "<< 
                MAX_WIDTH << ", MAX_HEIGHT: " << MAX_HEIGHT << ")" << endl;

    } while(rect.width > MAX_WIDTH || rect.height > MAX_HEIGHT);
    destroyWindow(title);
    lblState->set_text("Confirm to display a\npreview of the execution.");

    return rect;

    /* Codice valido per il thread camera */
    /* bool area_selected = false;
    char exit_key_press = 0;
	do
	{
		screen(img);
    	imshow(title, img);
        if(!frm->is_visible())
            break;
        exit_key_press = cvWaitKey(27);
        waitKey(1);
	}while(cvGetWindowHandle(title.c_str()) && exit_key_press != 27);
    if(exit_key_press == 27)
        destroyWindow(title);*/
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
 * Setta il valore di R, di supporto all'esecuzione.
 * @param   : int value; Il valore da utilizzare.
 * @return  : void
*/
void setR(int value)
{
    r_value = value;
}

/**
 * Setta il valore di G, di supporto all'esecuzione.
 * @param   : int value; Il valore da utilizzare.
 * @return  : void
*/
void setG(int value)
{
    g_value = value;
}

/**
 * Setta il valore di B, di supporto all'esecuzione.
 * @param   : int value; Il valore da utilizzare.
 * @return  : void
*/
void setB(int value)
{
    b_value = value;
}