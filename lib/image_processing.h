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

using namespace std;
using namespace cv;

bool getExecutionStarted();
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

/* Gestione dei Thread di analisi*/
void initGestore(struct gestore_t *g);

bool getFilteringActive();
bool getFrameDifferenceActive();
bool getHistogramActive();
bool getThresholdActive();
void setBitForChannelValue(int value);
void setFilteringActive(bool value);
void setFpsValue(int fps);
void setFrameDifferenceActive(bool value);
void setHistogramActive(bool value);
void setThresholdActive(bool value);
void startCamera(struct gestore_t *g);
void endCamera(struct gestore_t *g);
void startDifference(struct gestore_t *g);
void endDifference(struct gestore_t *g);
void startFilter(struct gestore_t *g);
void endFilter(struct gestore_t *g);
void startHistogram(struct gestore_t *g);
void endHistogram(struct gestore_t *g);
void startThreshold(struct gestore_t *g);
void endThreshold(struct gestore_t *g);

void bodyCamera();
void bodyDifference();
void bodyFilter();
void bodyHistogram();
void bodyThreshold();

void createTasks(tpars params[], int *last_proc, int *max_proc);
int computeWaitTime();
int showResults(FrmSettings *frmSettings, int wait_time);

void runExecutionThreads(FrmSettings *frmSettings);

/* Elaborazione dei fotogrammi catturati */
Mat filterFrame(Mat img);
Mat frameDifference(Mat img);
Mat plotHistogram(Mat src);
Mat quantizeImage(Mat inImage, int numBits);
Mat takeAPicture(Rect2d rect);
Mat threshold(Mat img);

/* Output */
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