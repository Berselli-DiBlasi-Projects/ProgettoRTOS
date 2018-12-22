#include "../lib/FrmMain.h"

using namespace std;
using namespace Gtk;


FrmMain::FrmMain(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
	Gtk::Window(cobject), builder(refGlade){

	builder->get_widget("cmbFps", cmbFps);
	builder->get_widget("cmbScalaColori", cmbScalaColori);
	builder->get_widget("cmbScaling", cmbScaling);
	
	builder->get_widget("btnSelezionaArea", btnSelezionaArea);
	builder->get_widget("btnConferma", btnConferma);
	builder->get_widget("btnAnnulla", btnAnnulla);
	builder->get_widget("btnAvvia", btnAvvia);

	btnSelezionaArea->signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_btnSelezionaArea_clicked));
	btnConferma->signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_btnConferma_clicked));
	btnAnnulla->signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_btnAnnulla_clicked));
	btnAvvia->signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_btnAvvia_clicked));
}


void FrmMain::on_btnSelezionaArea_clicked(){
	btnSelezionaArea->set_label("Seleziona area");
}

void FrmMain::on_btnConferma_clicked(){
	btnSelezionaArea->set_label("Conferma");
}

void FrmMain::on_btnAnnulla_clicked(){
	btnSelezionaArea->set_label("Annulla");
}

void FrmMain::on_btnAvvia_clicked(){
	btnSelezionaArea->set_label("Avvia");
}
