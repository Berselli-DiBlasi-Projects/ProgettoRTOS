#include "../lib/image_processing.h"
#include <gtkmm.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <string>
#include <iostream>
#include "../lib/FrmMain.h"

using namespace std;
using namespace cv;

FrmMain *frm = 0;

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

/* Funzione per selezionare la regione in cui definire la camera. */
/* Ritorna le coordinate della regione tracciata dall'utente     */
void select_region()
{
    Display* disp = XOpenDisplay(NULL);
    Screen*  scrn = DefaultScreenOfDisplay(disp);
    int width  = scrn->width;
    int height = scrn->height;
    ScreenShot screen(0, 0, scrn->width, scrn->height);

    Mat img;
	bool area_selected = false;
	string title = "Seleziona area";

    // Select ROI
    // Rect2d r = selectROI(img);

    screen(img);
    imshow(title, img);

	while(!area_selected && cvGetWindowHandle(title.c_str()))
	{
		screen(img);
    	imshow(title, img);
		waitKey(1);
        if(!frm->is_visible())
            break;
	}
    
}

/* Funzione che visualizza un dato frame */
void show_main_frame(string glade_file, string frame_id, string title,
     string icon_file, int argc, char** argv)
{
    cout << "Started" << endl;

	Gtk::Main kit(argc, argv);
	Glib::RefPtr<Gtk::Builder> builder = 
        Gtk::Builder::create_from_file(glade_file);
    
	builder->get_widget_derived(frame_id, frm);
	frm->set_title(title);
	frm->set_icon_from_file(icon_file);
	frm->property_resizable() = false;
	kit.run(*frm);

	cout << "End" << endl;
}