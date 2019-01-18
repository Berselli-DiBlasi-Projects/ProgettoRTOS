#include <gtkmm.h>

class FrmMain : public Gtk::Window{
protected:
	Glib::RefPtr<Gtk::Builder> builder;
	Gtk::ComboBoxText *cmbFps;
	Gtk::ComboBoxText *cmbScalaColori;;
	Gtk::Label *lblState;
	Gtk::Button *btnSelezionaArea;
	Gtk::Button *btnConferma;
	Gtk::Button *btnAnnulla;
	Gtk::Button *btnAvvia;
public:
	FrmMain(BaseObjectType* cobject, 
			const Glib::RefPtr<Gtk::Builder>& refGlade);

protected:
	//signal handlers
	void on_cmbFps_changed();
	void on_cmbScalaColori_changed();
	void on_btnSelezionaArea_clicked();
	void on_btnConferma_clicked();
	void on_btnAnnulla_clicked();
	void on_btnAvvia_clicked();

};

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

    Gtk::Button *btnFilteringActive;
    Gtk::Button *btnFrameDifferenceActive;
    Gtk::Button *btnThresholdActive;
    Gtk::Button *btnHistogramActive;

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

    void on_btnFilteringActive_clicked();
    void on_btnFrameDifferenceActive_clicked();
    void on_btnThresholdActive_clicked();
    void on_btnHistogramActive_clicked();

    void on_btnQuit_clicked();
    void on_btnRun_clicked();
};
