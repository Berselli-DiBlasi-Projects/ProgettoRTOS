#include <string>
#include <iostream>
#include <gtkmm.h>

#define MAX_WIDTH 800
#define MAX_HEIGHT 600

using namespace std;

/*Dichiarazione delle funzioni per l'image processing*/
void select_region();
void show_frame(string glade_file, string frame_id, string title,
     string icon_file, int argc, char** argv);
