#include "../lib/image_processing.h"

using namespace std;

/**
 * Visualizza il frame FrmMain.
 * @param   : string glade_file; Il percorso del file glade.
 *            string frame_id; Id della finestra.
 *            string title; Titolo della finestra.
 *            string icon_file; Icona della finestra.
 *            int argc; argc del main.
 *            char** argv; argv del main.
 * @return  : void.
*/
void show_main_frame(string glade_file, string frame_id, string title,
     string icon_file, int argc, char** argv)
{
    FrmMain *frm;
    frm = 0;
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
    cout << "Frame main Started" << endl;
    Glib::RefPtr<Gtk::Builder> builder = 
        Gtk::Builder::create_from_file(glade_file);

	builder->get_widget_derived(frame_id, frm);
	frm->set_title(title);
	frm->set_icon_from_file(icon_file);
	frm->property_resizable() = false;
	app->run(*frm);

	cout << "Frame main End" << endl;
}

/**
 * Visualizza il frame FrmSettings.
 * @param   : string glade_file; Il percorso del file glade.
 *            string frame_id; Id della finestra.
 *            string title; Titolo della finestra.
 *            string icon_file; Icona della finestra.
 *            int argc; argc del main.
 *            char** argv; argv del main.
 * @return  : void.
*/
void show_settings_frame(string glade_file, string frame_id, string title,
     string icon_file, int argc, char** argv)
{
    FrmSettings *frm;
    frm = 0;
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
    cout << "Frame settings Started" << endl;
	Glib::RefPtr<Gtk::Builder> builder = 
        Gtk::Builder::create_from_file(glade_file);

	builder->get_widget_derived(frame_id, frm);
	frm->set_title(title);
	frm->set_icon_from_file(icon_file);
	frm->property_resizable() = false;
	app->run(*frm);

	cout << "Frame settings End" << endl;
}

/*--------------------------------------------------------------*/
/*		            	MAIN process				            */
/*--------------------------------------------------------------*/

int main(int argc, char** argv) {

	show_main_frame("../glade/frmMain.glade", "frmMain", "Image Processing",
                    "../media/image_processing.png", argc, argv);

    if(getExecutionStarted())
        show_settings_frame("../glade/frmSettings.glade", "frmSettings",
                            "Settings", "../media/image_processing.png", 
                            argc, argv);


    return 0;
}

/*--------------------------------------------------------------*/