#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"

using namespace cv;

void outputInit();
Mat getOutCamera();
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