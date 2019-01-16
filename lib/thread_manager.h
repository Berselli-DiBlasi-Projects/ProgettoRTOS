#include <string>
#include <iostream>
#include <stdio.h>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
extern "C" {
#include "pmutex.h"
#include "ptask.h"
#include "tstat.h"
}
#include "../lib/output_manager.h"
#include "../lib/FrmSettings.h"

Mat filterFrame(Mat img);
Mat frameDifference(Mat img);
bool getFilteringActive();
int getFpsValue();
bool getFrameDifferenceActive();
bool getHistogramActive();
bool getThresholdActive();
Mat plotHistogram(Mat src);
void setFilteringActive(bool value);
void setFpsValue(int fps);
void setFrameDifferenceActive(bool value);
void setHistogramActive(bool value);
void setThresholdActive(bool value);
Mat takeAPicture(Rect2d rect);
Mat threshold(Mat img);
void runExecutionThreads(FrmSettings *frmSettings);