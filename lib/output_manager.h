#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
extern "C" {
#include "pmutex.h"
#include "ptask.h"
#include "tstat.h"
}

using namespace cv;

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
