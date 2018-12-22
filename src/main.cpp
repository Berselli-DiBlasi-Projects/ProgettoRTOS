#include <gtkmm.h>
#include <iostream>
#include "../lib/FrmMain.h"

using namespace std;
using namespace Gtk;

/*--------------------------------------------------------------*/
/*			MAIN process				*/
/*--------------------------------------------------------------*/

int main(int argc, char** argv) {
    //gtk code
    cout << "Started" << endl;

	Main kit(argc,argv);
	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("../glade/frmMain.glade");

	FrmMain *frm = 0;
	builder->get_widget_derived("frmMain", frm);
	frm->set_title("Image Processing");
	frm->set_icon_from_file("../media/image_processing.png");
	frm->property_resizable() = false;
	kit.run(*frm);

	cout << "End" << endl;

    return 0;
}

/*--------------------------------------------------------------*/
