#include <gtkmm.h>

class FrmSettings : public Gtk::Window{
protected:
	Glib::RefPtr<Gtk::Builder> builder;
    Gtk::RadioButton *rdoDefault;
    Gtk::RadioButton *rdoSeppia;
    Gtk::RadioButton *rdoGrayScale;
    Gtk::RadioButton *rdoBN;

    Gtk::Scale *sclFrameDifference;

    Gtk::Scale *sclR;
    Gtk::Scale *sclG;
    Gtk::Scale *sclB;

	Gtk::Button *btnQuit;
public:
	FrmSettings(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade); //constructor

protected:
	//signal handlers
    void on_rdoDefault_clicked();
    void on_rdoSeppia_clicked();
    void on_rdoGrayScale_clicked();
    void on_rdoBN_clicked();

    void on_sclFrameDifference_value_changed();
    void on_sclR_value_changed();
    void on_sclG_value_changed();
    void on_sclB_value_changed();

    void on_btnQuit_clicked();
};
