#include "../lib/image_processing.h"

using namespace std;
using namespace Gtk;

//_____________________________________________________________________________
//_____________________________________________________________________________
//                                GUI MAIN FRAME
//_____________________________________________________________________________
//_____________________________________________________________________________

FrmMain::FrmMain(BaseObjectType* cobject,
				 const Glib::RefPtr<Gtk::Builder>& refGlade):
	Window(cobject), builder(refGlade){

	builder->get_widget("cmbFps", cmbFps);
	builder->get_widget("cmbScalaColori", cmbScalaColori);

	builder->get_widget("lblState", lblState);

	builder->get_widget("btnSelezionaArea", btnSelezionaArea);
	builder->get_widget("btnConferma", btnConferma);
	builder->get_widget("btnAnnulla", btnAnnulla);
	builder->get_widget("btnAvvia", btnAvvia);

	btnSelezionaArea->signal_clicked().connect(
		sigc::mem_fun(*this, &FrmMain::on_btnSelezionaArea_clicked));
	btnConferma->signal_clicked().connect(
		sigc::mem_fun(*this, &FrmMain::on_btnConferma_clicked));
	btnAnnulla->signal_clicked().connect(
		sigc::mem_fun(*this, &FrmMain::on_btnAnnulla_clicked));
	btnAvvia->signal_clicked().connect(
		sigc::mem_fun(*this, &FrmMain::on_btnAvvia_clicked));
	
}


void FrmMain::on_btnSelezionaArea_clicked()
{
	btnSelezionaArea->set_sensitive(false);
	selectRegion(lblState);
	btnSelezionaArea->set_sensitive(true);
	btnConferma->set_sensitive(true);
}

void FrmMain::on_btnConferma_clicked()
{
	lblState->set_text("Press Cancel to select a\nnew area otherwise Start");
	btnConferma->set_sensitive(false);
	btnSelezionaArea->set_sensitive(false);
	btnAnnulla->set_sensitive(true);
	btnAvvia->set_sensitive(true);
	preview(this);
}

void FrmMain::on_btnAnnulla_clicked()
{
	lblState->set_text("Configure the application\nand then Select the area.");
	btnAnnulla->set_sensitive(false);
	btnAvvia->set_sensitive(false);
	btnSelezionaArea->set_sensitive(true);
	setCancelSignal(true);
}

void FrmMain::on_btnAvvia_clicked()
{
	lblState->set_text("Running...");
	btnAvvia->set_sensitive(false);
	btnAnnulla->set_sensitive(false);
	setCancelSignal(true);
	//Fai partire l'elaborazione
	setExecutionStarted(true);
	setFpsValue(stoi(cmbFps->get_active_text()));
	setBitForChannelValue(atoi(cmbScalaColori->get_active_text().c_str()));
	hide();
}

//_____________________________________________________________________________
//_____________________________________________________________________________
//                                GUI FRAME SETTINGS
//_____________________________________________________________________________
//_____________________________________________________________________________
FrmSettings::FrmSettings(BaseObjectType* cobject, 
						 const Glib::RefPtr<Gtk::Builder>& refGlade):
	Window(cobject), builder(refGlade) {
	builder->get_widget("rdoDefault", rdoDefault);
	builder->get_widget("rdoSeppia", rdoSeppia);
	builder->get_widget("rdoGrayScale", rdoGrayScale);
	builder->get_widget("rdoBN", rdoBN);
	builder->get_widget("sclFrameDifference", sclFrameDifference);
	builder->get_widget("sclFrameScaling", sclFrameScaling);
	builder->get_widget("sclThresholdType", sclThresholdType);
	builder->get_widget("sclThresholdValue", sclThresholdValue);
	builder->get_widget("lblThresholdType", lblThresholdType);
	builder->get_widget("btnFilteringActive", btnFilteringActive);
	builder->get_widget("btnFrameDifferenceActive", btnFrameDifferenceActive);
	builder->get_widget("btnThresholdActive", btnThresholdActive);
	builder->get_widget("btnHistogramActive", btnHistogramActive);
	builder->get_widget("btnQuit", btnQuit);
	builder->get_widget("btnRun", btnRun);

	rdoDefault->signal_clicked().connect(
		sigc::mem_fun(*this, &FrmSettings::on_rdoDefault_clicked));
	rdoSeppia->signal_clicked().connect(
		sigc::mem_fun(*this, &FrmSettings::on_rdoSeppia_clicked));
	rdoGrayScale->signal_clicked().connect(
		sigc::mem_fun(*this, &FrmSettings::on_rdoGrayScale_clicked));
	rdoBN->signal_clicked().connect(
		sigc::mem_fun(*this, &FrmSettings::on_rdoBN_clicked));

	sclFrameDifference->signal_value_changed().connect(
		sigc::mem_fun(*this, 
			&FrmSettings::on_sclFrameDifference_value_changed));
	sclFrameScaling->signal_value_changed().connect(
		sigc::mem_fun(*this, 
			&FrmSettings::on_sclFrameScaling_value_changed));
			
	sclThresholdType->signal_value_changed().connect(
		sigc::mem_fun(*this, &FrmSettings::on_sclThresholdType_value_changed));
	sclThresholdValue->signal_value_changed().connect(
		sigc::mem_fun(*this, &FrmSettings::on_sclThresholdValue_value_changed));

	btnFilteringActive->signal_clicked().connect(
		sigc::mem_fun(*this, &FrmSettings::on_btnFilteringActive_clicked));
	btnFrameDifferenceActive->signal_clicked().connect(
		sigc::mem_fun(*this, &FrmSettings::on_btnFrameDifferenceActive_clicked));
	btnThresholdActive->signal_clicked().connect(
		sigc::mem_fun(*this, &FrmSettings::on_btnThresholdActive_clicked));
	btnHistogramActive->signal_clicked().connect(
		sigc::mem_fun(*this, &FrmSettings::on_btnHistogramActive_clicked));
	
	btnQuit->signal_clicked().connect(
		sigc::mem_fun(*this, &FrmSettings::on_btnQuit_clicked));
	btnRun->signal_clicked().connect(
		sigc::mem_fun(*this, &FrmSettings::on_btnRun_clicked));
}

void FrmSettings::on_rdoDefault_clicked()
{
	setFilterChoice(0);
}

void FrmSettings::on_rdoSeppia_clicked()
{
	setFilterChoice(1);
}

void FrmSettings::on_rdoGrayScale_clicked()
{
	setFilterChoice(2);
}

void FrmSettings::on_rdoBN_clicked()
{
	setFilterChoice(3);
}

void FrmSettings::on_sclFrameDifference_value_changed()
{
	setFrameDifferenceValue(sclFrameDifference->get_value());
}

void FrmSettings::on_sclFrameScaling_value_changed()
{
	setFrameScalingValue(sclFrameScaling->get_value());
}

void FrmSettings::on_sclThresholdType_value_changed()
{
	int threshold_type = sclThresholdType->get_value();
	setThresholdType(threshold_type);

	if(threshold_type == 1)
		lblThresholdType->set_text("Binary");
	if(threshold_type == 2)
		lblThresholdType->set_text("Binary Inverted");
	if(threshold_type == 3)
		lblThresholdType->set_text("Threshold truncated");
	if(threshold_type == 4)
		lblThresholdType->set_text("Threshold to zero");
	if(threshold_type == 5)
		lblThresholdType->set_text("Threshold to zero inverted");
}

void FrmSettings::on_sclThresholdValue_value_changed()
{
	setThresholdValue(sclThresholdValue->get_value());
}

void FrmSettings::on_btnFilteringActive_clicked()
{
	if(getFilteringActive())	//disattiva
	{
		Gtk::Image *eyegray;
		setFilteringActive(false);
		eyegray = Gtk::manage(new Gtk::Image());
    	eyegray->set("../media/eyegray.png");
		btnFilteringActive->set_image(*eyegray);
	}
	else	//riattiva
	{
		Gtk::Image *eye;
		setFilteringActive(true);
		eye = Gtk::manage(new Gtk::Image());
		eye->set("../media/eye.png");
		btnFilteringActive->set_image(*eye);
	}
}

void FrmSettings::on_btnFrameDifferenceActive_clicked()
{
	if(getFrameDifferenceActive())	//disattiva
	{
		Gtk::Image *eyegray;
		setFrameDifferenceActive(false);
		eyegray = Gtk::manage(new Gtk::Image());
    	eyegray->set("../media/eyegray.png");
		btnFrameDifferenceActive->set_image(*eyegray);
	}
	else	//riattiva
	{
		Gtk::Image *eye;
		setFrameDifferenceActive(true);
		eye = Gtk::manage(new Gtk::Image());
		eye->set("../media/eye.png");
		btnFrameDifferenceActive->set_image(*eye);
	}
}

void FrmSettings::on_btnThresholdActive_clicked()
{
	if(getThresholdActive())	//disattiva
	{
		Gtk::Image *eyegray;
		setThresholdActive(false);
		eyegray = Gtk::manage(new Gtk::Image());
    	eyegray->set("../media/eyegray.png");
		btnThresholdActive->set_image(*eyegray);
	}
	else	//riattiva
	{
		Gtk::Image *eye;
		setThresholdActive(true);
		eye = Gtk::manage(new Gtk::Image());
		eye->set("../media/eye.png");
		btnThresholdActive->set_image(*eye);
	}
}

void FrmSettings::on_btnHistogramActive_clicked()
{
	if(getHistogramActive())	//disattiva
	{
		Gtk::Image *eyegray;
		setHistogramActive(false);
		eyegray = Gtk::manage(new Gtk::Image());
    	eyegray->set("../media/eyegray.png");
		btnHistogramActive->set_image(*eyegray);
	}
	else	//riattiva
	{
		Gtk::Image *eye;
		setHistogramActive(true);
		eye = Gtk::manage(new Gtk::Image());
		eye->set("../media/eye.png");
		btnHistogramActive->set_image(*eye);
	}
}

void FrmSettings::on_btnQuit_clicked()
{
	hide();
}

void FrmSettings::on_btnRun_clicked()
{
	btnRun->set_sensitive(false);
	runExecutionThreads(this);
}