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

void takeAPicture(Rect2d rect);
void histogram(Mat src);
void preview(Rect2d rect, FrmMain *frmMain);
Rect2d select_region(Gtk::Label *lblState);
void setCancelSignal(bool signal);