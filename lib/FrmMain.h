#include <gtkmm.h>

class FrmMain : public Gtk::Window{
protected:
	Glib::RefPtr<Gtk::Builder> builder;
	Gtk::ComboBoxText *cmbFps;
	Gtk::ComboBoxText *cmbScalaColori;
	Gtk::ComboBoxText *cmbScaling;
	Gtk::Button *btnSelezionaArea;
	Gtk::Button *btnConferma;
	Gtk::Button *btnAnnulla;
	Gtk::Button *btnAvvia;
public:
	FrmMain(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade); //constructor

protected:
	//signal handlers
	void on_cmbFps_changed();
	void on_cmbScalaColori_changed();
	void on_cmbScaling_changed();
	void on_btnSelezionaArea_clicked();
	void on_btnConferma_clicked();
	void on_btnAnnulla_clicked();
	void on_btnAvvia_clicked();

};
