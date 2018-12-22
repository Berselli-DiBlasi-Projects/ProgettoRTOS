/*
 * FrmMain.h
 *
 *  Created on: May 15, 2012
 *      Author: milinda
 */

#include <gtkmm.h>

class FrmMain : public Gtk::Window{
protected:
	Glib::RefPtr<Gtk::Builder> builder;
	Gtk::Button *btnOk;
	Gtk::Button *btnCancel;
	Gtk::Label *lblNotice;

public:
	FrmMain(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);//constructor

protected:
	//signal handlers
	void on_ok_button_clicked();
	void on_cancel_button_clicked();

};
