#include <Gluco_main_window.h>

namespace Gluco{

void main_tbutton_callback(Fl_Widget *w, void* gluco_objects_ptr){

	Gluco_obj* gluco_objects = (Gluco_obj*) gluco_objects_ptr;

	int last_value = ((Fl_Toggle_Button*) w)->value();
	string tb_label = w->label();


	//Set all buttons to off
	gluco_objects->tbutton_tables->value(0);
	gluco_objects->tbutton_entries->value(0);
	gluco_objects->tbutton_export_menu->value(0);
	gluco_objects->tbutton_settings->value(0);

	//Hide all groups
	gluco_objects->table_group->hide();
	gluco_objects->export_Group->hide();;
	gluco_objects->data_input_buttons_Group->hide();
	gluco_objects->settings_Group->hide();
	gluco_objects->welcome_group->hide();

	if(last_value==1){
			((Gluco_T_Button*)w)->set();
			gluco_objects->window->label(tb_label.c_str());

			if(tb_label=="Tabellen"){
				gluco_objects->insulin_table->update_data();
				gluco_objects->bloodpressure_table->update_data();
				gluco_objects->food_table->update_data();
				gluco_objects->glucose_table->update_data();
				gluco_objects->table_group->show();
			}
			else if (tb_label=="Export"){
				gluco_objects->export_Group->show();
			}
			else if (tb_label=="Eintrag anlegen"){
				gluco_objects->data_input_buttons_Group->show();
			}
			else if(tb_label =="Einstellungen"){
				settings_variables_from_database(gluco_objects);
				gluco_objects->settings_Group->show();
			}
		}
		else{
			gluco_objects->window->label("Glu&Co Track");

			string greeting = "Guten Tag ";
			greeting.append(gluco_objects->config->name);
			greeting.append("!");
			gluco_objects->welcome_greeting->value(greeting.c_str());

			gluco_objects->welcome_group->show();

		}
}

void main_db_change_callback(Fl_Widget *w, void* gluco_objects_ptr){

	Gluco_obj* gluco_objects = (Gluco_obj*) gluco_objects_ptr;

	Fl_Native_File_Chooser explorer;
	explorer.title("Wählen sie eine neue Datenbank:");
	explorer.type(Fl_Native_File_Chooser::BROWSE_FILE);
	explorer.options(explorer.Option::SAVEAS_CONFIRM);
	explorer.filter("*.db");
	fs::path filepath;

	if(!explorer.show()){
		filepath = explorer.filename();
	}
	else{
		return;
	}
	int init_state = is_init(filepath);
	if(init_state & 0b10){
		fl_alert("Datenbank kann nicht geöffnet werden, da ein Export ist.\n Export-Dateien können nur im Lesemodus geöffnet werden.");
		return;
	}
	else{
		fl_alert("Datenbank ist nicht initialisiert");
		return;
	}

	save_lastdb(filepath);
	gluco_objects->database->filepath(filepath);
	gluco_objects->database->load_config(*(gluco_objects->config));
	main_tbutton_callback(w,gluco_objects);


}

void main_read_mode_callback(Fl_Widget* w, void*){

	load_read_window(275,40,1000,750);

}

void load_main_window(Gluco_obj* gluco_objects){
	int width{200};
	int height{80};

	string greeting = "Guten Tag ";
	greeting.append(gluco_objects->config->name);
	greeting.append("!");

	//Loading window plus Togglebuttons

	//OPTIONAL:
	//sidebar=make_shared<Fl_Box>(0,0,250,750);
	//sidebar->box(FL_PLASTIC_UP_BOX);
	gluco_objects->mainwindow_Group=make_shared<Fl_Group>(0,0,1000,750);
	gluco_objects->mainwindow_Group->begin();

		gluco_objects->welcome_group=make_shared<Fl_Group>(225,0,800,850);
		gluco_objects->welcome_group->begin();

		gluco_objects->welcome_greeting=make_shared<Fl_Output>(260,40,750,50);
		gluco_objects->db_change=make_shared<Gluco_Button>(650,650,300,50,"Datenbankspeicherort wechseln");
		gluco_objects->db_change->callback(main_db_change_callback,gluco_objects);

		gluco_objects->welcome_group->end();

	gluco_objects->sidebar=make_shared<Fl_Box>(-50,-50,300,850);

	gluco_objects->tbutton_tables=make_shared<Gluco_T_Button>(25,25,width,height,"Tabellen");
	gluco_objects->tbutton_entries=make_shared<Gluco_T_Button>(25,height+50,width,height,"Eintrag anlegen");
	gluco_objects->tbutton_export_menu=make_shared<Gluco_T_Button>(25,height+155,width,height,"Export");
	gluco_objects->button_import_menu=make_shared<Gluco_Button>(25,height+260,width,height,"TSV-Import");
	gluco_objects->button_read_mode=make_shared<Gluco_Button>(25,height+365,width,height,"Lesemodus");
	gluco_objects->tbutton_settings=make_shared<Gluco_T_Button>(25,height+565,width,height,"Einstellungen");

	gluco_objects->mainwindow_Group->end();

	gluco_objects->tbutton_tables->callback(main_tbutton_callback,gluco_objects);
	gluco_objects->tbutton_entries->callback(main_tbutton_callback,gluco_objects);
	gluco_objects->tbutton_export_menu->callback(main_tbutton_callback,gluco_objects);
	gluco_objects->button_import_menu->callback(import_tsv_callback,gluco_objects);
	gluco_objects->button_read_mode->callback(main_read_mode_callback);
	gluco_objects->tbutton_settings->callback(main_tbutton_callback,gluco_objects);

	gluco_objects->sidebar->box(FL_BORDER_BOX);
	gluco_objects->sidebar->color(12);//4,6,9,11,12,14
	gluco_objects->sidebar->align(FL_ALIGN_CLIP);

	gluco_objects->welcome_greeting->value(greeting.c_str());
	//changes cursor-color, thus it is invisible
	gluco_objects->welcome_greeting->cursor_color(FL_WHITE);
	gluco_objects->welcome_greeting->box(FL_NO_BOX);
	gluco_objects->welcome_greeting->textsize(30);
	//by changing its "marking-color" the output seems to be not markable:
	gluco_objects->welcome_greeting->color2(FL_WHITE);


}

void main_window_atclose_callback(Fl_Widget* w, void* gluco_objects_ptr){
	Gluco_obj* gluco_objects = (Gluco_obj*) gluco_objects_ptr;

	gluco_objects->database->~Gluco_Database();
	gluco_objects->window->hide();

}

void create_main_window(const int &x, const int &y, const int &w, const int &h, const char* text, Gluco_obj* gluco_objects){
	gluco_objects->window=make_shared<Gluco_Window>(x,y,w,h,text);

	//Callback when closing window
	gluco_objects->window->callback(main_window_atclose_callback,gluco_objects);
	//Color-mixed icon of the program
}
}



