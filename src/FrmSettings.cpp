#include "../lib/image_processing.h"

using namespace std;
using namespace Gtk;

FrmSettings::FrmSettings(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
	Window(cobject), builder(refGlade){

	builder->get_widget("rdoDefault", rdoDefault);
	builder->get_widget("rdoSeppia", rdoSeppia);
	builder->get_widget("rdoGrayScale", rdoGrayScale);
	builder->get_widget("rdoBN", rdoBN);
	builder->get_widget("sclFrameDifference", sclFrameDifference);
	builder->get_widget("sclR", sclR);
	builder->get_widget("sclG", sclG);
	builder->get_widget("sclB", sclB);
	builder->get_widget("btnQuit", btnQuit);

	rdoDefault->signal_clicked().connect(sigc::mem_fun(*this, &FrmSettings::on_rdoDefault_clicked) );
	rdoSeppia->signal_clicked().connect(sigc::mem_fun(*this, &FrmSettings::on_rdoSeppia_clicked) );
	rdoGrayScale->signal_clicked().connect(sigc::mem_fun(*this, &FrmSettings::on_rdoGrayScale_clicked));
	rdoBN->signal_clicked().connect(sigc::mem_fun(*this, &FrmSettings::on_rdoBN_clicked) );

	sclFrameDifference->signal_value_changed().connect(sigc::mem_fun(*this, &FrmSettings::on_sclFrameDifference_value_changed));
	sclR->signal_value_changed().connect(sigc::mem_fun(*this, &FrmSettings::on_sclR_value_changed));
	sclG->signal_value_changed().connect(sigc::mem_fun(*this, &FrmSettings::on_sclG_value_changed));
	sclB->signal_value_changed().connect(sigc::mem_fun(*this, &FrmSettings::on_sclB_value_changed));

	btnQuit->signal_clicked().connect(sigc::mem_fun(*this, &FrmSettings::on_btnQuit_clicked));
}

void FrmSettings::on_rdoDefault_clicked(){
	cout << "ciao" << endl;
}

void FrmSettings::on_rdoSeppia_clicked(){
	cout << "mondo" << endl;
}

void FrmSettings::on_rdoGrayScale_clicked(){
	cout << "ciao" << endl;
}

void FrmSettings::on_rdoBN_clicked(){
	cout << "mondo" << endl;
}

void FrmSettings::on_sclFrameDifference_value_changed(){
	setFrameDifferenceValue(sclFrameDifference->get_value());
}

void FrmSettings::on_sclR_value_changed(){
	cout << sclR->get_value() << endl;
}

void FrmSettings::on_sclG_value_changed(){
	cout << sclG->get_value() << endl;
}

void FrmSettings::on_sclB_value_changed(){
	cout << sclB->get_value() << endl;
}

void FrmSettings::on_btnQuit_clicked(){
	hide();
}