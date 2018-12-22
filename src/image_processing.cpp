#include "../lib/image_processing.h"
#include <gtkmm.h>
#include "../lib/FrmMain.h"
#include <string>
#include <iostream>

using namespace std;
using namespace Gtk;

/* Funzione per selezionare la regione in cui definire la camera. */
/* Ritorna le coordinate della regione tracciata dall'utente     */
void select_region()
{
    cout << "ciao";
}

/* Funzione che visualizza un dato frame */
void show_frame(string glade_file, string frame_id, string title,
     string icon_file, int argc, char** argv)
{
    cout << "Started" << endl;

	Main kit(argc, argv);
	Glib::RefPtr<Gtk::Builder> builder = 
        Gtk::Builder::create_from_file(glade_file);

	FrmMain *frm = 0;
	builder->get_widget_derived(frame_id, frm);
	frm->set_title(title);
	frm->set_icon_from_file(icon_file);
	frm->property_resizable() = false;
	kit.run(*frm);

	cout << "End" << endl;
}