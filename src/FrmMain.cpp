#include "../lib/image_processing.h"

using namespace std;
using namespace Gtk;

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


void FrmMain::on_btnSelezionaArea_clicked(){
	btnSelezionaArea->set_sensitive(false);
	selectRegion(lblState);
	btnSelezionaArea->set_sensitive(true);
	btnConferma->set_sensitive(true);
}

void FrmMain::on_btnConferma_clicked(){
	lblState->set_text("Press Cancel to select a\nnew area otherwise Start");
	btnConferma->set_sensitive(false);
	btnSelezionaArea->set_sensitive(false);
	btnAnnulla->set_sensitive(true);
	btnAvvia->set_sensitive(true);
	preview(this);
}

void FrmMain::on_btnAnnulla_clicked(){
	lblState->set_text("Configure the application\nand then Select the area.");
	btnAnnulla->set_sensitive(false);
	btnAvvia->set_sensitive(false);
	btnSelezionaArea->set_sensitive(true);
	setCancelSignal(true);
}

void FrmMain::on_btnAvvia_clicked(){
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