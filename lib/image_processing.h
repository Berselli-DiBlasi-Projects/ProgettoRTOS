#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include <string>
#include <iostream>
#include <gtkmm.h>
#include "../lib/FrmMain.h"
#include "../lib/FrmSettings.h"

#define MAX_WIDTH 800
#define MAX_HEIGHT 600

using namespace std;
using namespace cv;

bool getExecutionStarted();
int getFrameDifferenceValue();
int getR();
int getG();
int getB();
void preview(Rect2d rect, FrmMain *frmMain);
Rect2d select_region(Gtk::Label *lblState);
void setCancelSignal(bool signal);
void setExecutionStarted(bool started);
void setFrameDifferenceValue(int difference);
void setR(int value);
void setG(int value);
void setB(int value);
void filterFrame(Mat img);
Mat frameDifference(Mat img);
void histogram(Mat img);
void displayVideoStream(Rect2d rect);
Mat takeAPicture(Rect2d rect);
Mat threshold(Mat img);