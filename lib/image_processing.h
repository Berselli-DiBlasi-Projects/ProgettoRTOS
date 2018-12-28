#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include <string>
#include <iostream>
#include <gtkmm.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "../lib/FrmMain.h"
#include "../lib/FrmSettings.h"
#include "../lib/output_manager.h"

extern "C" {
#include "pmutex.h"
#include "ptask.h"
#include "tstat.h"
}

#define MAX_WIDTH 800
#define MAX_HEIGHT 600

using namespace std;
using namespace cv;

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

bool getExecutionStarted();
int getFilterChoice();
int getFrameDifferenceValue();
int getR();
int getG();
int getB();
void preview(Rect2d rect, FrmMain *frmMain);
Rect2d selectRegion(Gtk::Label *lblState);
void setCancelSignal(bool signal);
void setExecutionStarted(bool started);
void setFilterChoice(int choice);
void setFrameDifferenceValue(int difference);
void setR(int value);
void setG(int value);
void setB(int value);

Mat filterFrame(Mat img);
Mat frameDifference(Mat img);
Mat plotHistogram(Mat src);
Mat takeAPicture(Rect2d rect);
Mat threshold(Mat img);