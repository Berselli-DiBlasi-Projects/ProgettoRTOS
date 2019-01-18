#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include <string>
#include <stdio.h>
#include <iostream>
#include <gtkmm.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
extern "C" {
    #include "pmutex.h"
    #include "ptask.h"
    #include "tstat.h"
}

#include "../lib/image_processing_gui.h"

#define MAX_WIDTH 1920
#define MAX_HEIGHT 1080

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

        img = XGetImage(display, root, x, y, width, height, 
                        AllPlanes, ZPixmap);

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

//int getFrameDifferenceValue();
//int getFrameScalingValue();
//int getThresholdType();
//int getThresholdValue();
Mat imageScale(Mat src);
void preview(FrmMain *frmMain);
void selectRegion(Gtk::Label *lblState);
void setCancelSignal(bool signal);
void setExecutionStarted(bool started);
void setFilterChoice(int choice);
void setFrameDifferenceValue(int difference);
void setFrameScalingValue(int scaling);
void setThresholdType(int value);
void setThresholdValue(int value);




Mat filterFrame(Mat img);
Mat frameDifference(Mat img);
//int getBitForChannelValue();
bool getFilteringActive();
//int getFpsValue();
bool getFrameDifferenceActive();
bool getHistogramActive();
bool getThresholdActive();
Mat plotHistogram(Mat src);
Mat quantizeImage(Mat inImage, int numBits);
void setBitForChannelValue(int value);
void setFilteringActive(bool value);
void setFpsValue(int fps);
void setFrameDifferenceActive(bool value);
void setHistogramActive(bool value);
void setThresholdActive(bool value);
Mat takeAPicture(Rect2d rect);
Mat threshold(Mat img);
void runExecutionThreads(FrmSettings *frmSettings);


void initOutput();
Mat getOutCamera();
Mat getOutCameraScaled();
Mat getOutDifference();
Mat getOutFilter();
Mat getOutPlotHistogram();
Mat getOutThreshold();
Rect2d getRect();
void setOutCamera(Mat img);
void setOutDifference(Mat img);
void setOutFilter(Mat img);
void setOutPlotHistogram(Mat img);
void setOutThreshold(Mat img);
void setRect(Rect2d r);







