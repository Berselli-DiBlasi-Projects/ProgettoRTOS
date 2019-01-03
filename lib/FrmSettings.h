#include <gtkmm.h>

class FrmSettings : public Gtk::Window{
protected:
	Glib::RefPtr<Gtk::Builder> builder;
    Gtk::RadioButton *rdoDefault;
    Gtk::RadioButton *rdoSeppia;
    Gtk::RadioButton *rdoGrayScale;
    Gtk::RadioButton *rdoBN;

    Gtk::Scale *sclFrameDifference;

    Gtk::Scale *sclFrameScaling;

    Gtk::Scale *sclThresholdType;
    Gtk::Scale *sclThresholdValue;
    Gtk::Label *lblThresholdType;

	Gtk::Button *btnQuit;
    Gtk::Button *btnRun;
public:
	FrmSettings(BaseObjectType* cobject, 
                const Glib::RefPtr<Gtk::Builder>& refGlade);

protected:
	//signal handlers
    void on_rdoDefault_clicked();
    void on_rdoSeppia_clicked();
    void on_rdoGrayScale_clicked();
    void on_rdoBN_clicked();

    void on_sclFrameDifference_value_changed();

    void on_sclFrameScaling_value_changed();

    void on_sclThresholdType_value_changed();
    void on_sclThresholdValue_value_changed();

    void on_btnQuit_clicked();
    void on_btnRun_clicked();
};
