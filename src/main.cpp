#include "../lib/image_processing.h"
#include <iostream>

using namespace std;

/*--------------------------------------------------------------*/
/*		            	MAIN process				                        */
/*--------------------------------------------------------------*/

int main(int argc, char** argv) {

    //gtk code
	  show_main_frame("../glade/frmMain.glade", "frmMain",
		  "Image Processing", "../media/image_processing.png", argc, argv);

  

    return 0;
}

/*--------------------------------------------------------------*/
