#include <Gluco_settings_window.h>

namespace Gluco{

void settings_BE_callback(Fl_Widget *b, void* gluco_objects_ptr){

	Gluco_obj* gluco_objects = (Gluco_obj*) gluco_objects_ptr;

	std::stringstream ss;
	ss << std::setprecision(5);
	ss << gluco_objects->config->BE_day;
	gluco_objects->BE_input_f_morning->value(ss.str().c_str());

	ss.str("");
	ss << gluco_objects->config->BE_noon;
	gluco_objects->BE_input_f_noon->value(ss.str().c_str());

	ss.str("");
	ss << gluco_objects->config->BE_evening;
	gluco_objects->BE_input_f_eve->value(ss.str().c_str());

	ss.str("");
	ss << gluco_objects->config->BE_night;
	gluco_objects->BE_input_f_night->value(ss.str().c_str());

	gluco_objects->BE_input_t_morning_from->value(gluco_objects->config->day_begin.c_str());
	gluco_objects->BE_input_t_morning_to->value(gluco_objects->config->day_end.c_str());
	gluco_objects->BE_input_t_noon_from->value(gluco_objects->config->noon_begin.c_str());
	gluco_objects->BE_input_t_noon_to->value(gluco_objects->config->noon_end.c_str());
	gluco_objects->BE_input_t_eve_from->value(gluco_objects->config->evening_begin.c_str());
	gluco_objects->BE_input_t_eve_to->value(gluco_objects->config->evening_end.c_str());
	gluco_objects->BE_input_t_night_from->value(gluco_objects->config->night_begin.c_str());
	gluco_objects->BE_input_t_night_to->value(gluco_objects->config->noon_end.c_str());

	gluco_objects->BE_window->show();

}

void settings_BE_close_callback(Fl_Widget *w, void* gluco_objects_ptr){
	w->parent()->hide();
}

void info_callback_BE(Fl_Widget*, long int){


	fl_message("Entwickler: 			  Verwendete Libraries:\n"
			"\n"
			"Elias Mende 			FLTK 1.3.5	\n"
			"Felician Richter 		SQLite 3.34.0 \n"
			"Maxim Rayanov \n"
			"Constantin Michler \n"
			"Niklas Deines");
}

void settings_variables_from_database(Gluco_obj* gluco_objects){

	std::stringstream ss;
	ss << std::setprecision(5);


	gluco_objects->settings_input_name->value(gluco_objects->config->name.c_str());

	ss << gluco_objects->config->limit_hypergly;
	gluco_objects->settings_input_hypergly->value(ss.str().c_str());

	ss.str("");
	ss << gluco_objects->config->limit_hypogly;
	gluco_objects->settings_input_hypogly->value(ss.str().c_str());



}

void settings_variables_to_database_callback(Fl_Widget* w, Gluco_obj* gluco_objects){

	gluco_objects->config->name = gluco_objects->settings_input_name->value();
	gluco_objects->config->limit_hypogly = std::strtod(gluco_objects->settings_input_hypogly->value(),NULL);
	gluco_objects->config->limit_hypergly = std::strtod(gluco_objects->settings_input_hypergly->value(),NULL);

	gluco_objects->database->save_config(*(gluco_objects->config));

}

void settings_variables_BE_to_database_callback(Fl_Widget* w, Gluco_obj* gluco_objects){

	gluco_objects->config->BE_day = std::strtod(gluco_objects->BE_input_f_morning->value(),NULL);
	gluco_objects->config->BE_noon = std::strtod(gluco_objects->BE_input_f_noon->value(),NULL);
	gluco_objects->config->BE_evening = std::strtod(gluco_objects->BE_input_f_eve->value(),NULL);
	gluco_objects->config->BE_night = std::strtod(gluco_objects->BE_input_f_night->value(),NULL);
	gluco_objects->config->day_begin = gluco_objects->BE_input_t_morning_from->value();
	gluco_objects->config->day_end = gluco_objects->BE_input_t_morning_to->value();
	gluco_objects->config->noon_begin = gluco_objects->BE_input_t_noon_from->value();
	gluco_objects->config->noon_end = gluco_objects->BE_input_t_noon_to->value();
	gluco_objects->config->evening_begin = gluco_objects->BE_input_t_eve_from->value();
	gluco_objects->config->evening_end = gluco_objects->BE_input_t_eve_to->value();
	gluco_objects->config->night_begin= gluco_objects->BE_input_t_night_from->value();
	gluco_objects->config->noon_end = gluco_objects->BE_input_t_night_to->value();

	gluco_objects->database->save_config(*(gluco_objects->config));
	w->parent()->hide();

}

void info_reset_callback(Fl_Widget* w, void* gluco_objects_ptr){
	Gluco_obj* gluco_objects = (Gluco_obj*) gluco_objects_ptr;

	if(!fl_choice("Wollen Sie die Datenbank wirklich zurücksetzen?\nAlle Daten werden irreversibel gelöscht!","Ja","Nein",0)){
		if(!fl_choice("Sind Sie sicher?","Zurücksetzen","Abbruch",0)){
			gluco_objects->database->reset();
			*(gluco_objects->config) = {};
			gluco_objects->database->save_config(*(gluco_objects->config));
			settings_variables_from_database(gluco_objects); //Resets all input values
		}
	}
}

fs::path backup_path_explorer(const string &title,string type){
	Fl_Native_File_Chooser explorer;
	explorer.title(title.c_str());
	explorer.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
	explorer.options(explorer.Option::SAVEAS_CONFIRM);
	string type_dot = "*." + type;
	explorer.filter(type_dot.c_str());

	if(!explorer.show()){
		string filename = explorer.filename();
		const std::regex p {R"(^.*\.)"+type+"$"};
		if(!std::regex_match(filename,p)){
			filename.append("." + type);
		}
		return filename;
	}

	return "";

}

fs::path restore_db_file_chooser(string title){
	Fl_Native_File_Chooser explorer;
	explorer.title(title.c_str());
	explorer.type(Fl_Native_File_Chooser::BROWSE_FILE);
	explorer.filter("*.db");
	if(!explorer.show()){
		return explorer.filename();
	}
	return "";
}

void database_backup_callback(Fl_Widget* w, Gluco_obj* gluco_objects){

	fs::path current_database_path = gluco_objects->database->filepath();
	fs::path backup_path = backup_path_explorer("Datenbank Backup speichern unter","db");

	fs::copy(current_database_path,backup_path);
}

void database_restore_callback(Fl_Widget* w, Gluco_obj* gluco_objects){

	// Get Database Paths
	fs::path database_to_restore = restore_db_file_chooser("Bitte wählen Sie die Datenbank, die wiederhergestell werden soll");

	switch(gluco_objects->database->restore(database_to_restore)){
	case 0:
		// Load config and redraw settings window with new config
		gluco_objects->database->load_config(*(gluco_objects->config));
		settings_variables_from_database(gluco_objects);
		gluco_objects->settings_Group->redraw();

		fl_message("Datenbank wurde erfolreich wiederhergestellt.");

		break;
	case 1:
		fl_alert("Datenbackup konnte nicht geladen werden. Alte Datenbank wird weiter benutzt!");
		break;
	case 2:
		fl_alert("Ausgewählte Datenbank ist kein gültiges Backup!");
		break;
	}


}

void load_settings_window(Gluco_obj* gluco_objects){



	gluco_objects->settings_Group = make_shared<Fl_Group>(250,0,750,750);
	gluco_objects->settings_Group->begin();

	gluco_objects->settingswindowbox  = make_shared<Fl_Box>(250,0,750,750);

		gluco_objects->settings_textbox_intro  = make_shared<Fl_Box>(75,25,900,50,"Hier können Sie alle persönliche Daten verwalten");
		gluco_objects->settings_textbox_name = make_shared<Fl_Box>(190,100, 200,50, "Name:");
		gluco_objects->settings_textbox_hypergly = make_shared<Fl_Box>(405,150,200,50, "Grenzwert Überzucker:                            mg/dl");
		gluco_objects->settings_textbox_hypogly = make_shared<Fl_Box>(405,200,200,50, "Grenzwert Unterzucker:                           mg/dl");

		gluco_objects->settings_input_name = make_shared<Fl_Input>(330,100,345,50);
		gluco_objects->settings_input_hypergly = make_shared<Fl_Input>(525,150,150,50);
		gluco_objects->settings_input_hypogly = make_shared<Fl_Input>(525,200,150,50);

		gluco_objects->settings_input_name->textsize(24);
		gluco_objects->settings_input_hypergly->textsize(24);
		gluco_objects->settings_input_hypogly->textsize(24);

		gluco_objects->settings_button_carbo_exchanges = make_shared<Gluco_Button>(275,300,200,50, "BE - Faktoren");
		gluco_objects->settings_button_restore = make_shared<Gluco_Button>(275,500,200,50, "Daten wiederherstellen");
		gluco_objects->settings_button_backup = make_shared<Gluco_Button>(275,575,200,50, "Datenbackup anlegen");
		gluco_objects->settings_button_reset = make_shared<Gluco_Button>(275,650,200,50, "Daten zurücksetzen");
		gluco_objects->settings_button_info = make_shared<Gluco_Button>(775,575,200,50, "Über Glu&&Co Track");
		gluco_objects->settings_button_save = make_shared<Gluco_Button>(775,650,200,50, "Speichern");

		gluco_objects->settings_textbox_hypogly->labelsize(24);
		gluco_objects->settings_textbox_hypergly->labelsize(24);
		gluco_objects->settings_textbox_name->labelsize(24);
		gluco_objects->settings_textbox_intro->labelsize(24);
		gluco_objects->settings_button_save->color(FL_DARK_GREEN);
		gluco_objects->settings_button_save->labelcolor(FL_WHITE);

		gluco_objects->settings_Group->end();
		gluco_objects->settings_Group->hide();

		gluco_objects->settings_button_carbo_exchanges->callback(settings_BE_callback,gluco_objects);
		gluco_objects->settings_button_restore->callback((Fl_Callback*)database_restore_callback,gluco_objects);
		gluco_objects->settings_button_backup->callback((Fl_Callback*)database_backup_callback,gluco_objects);
		gluco_objects->settings_button_reset->callback(info_reset_callback,gluco_objects);
		gluco_objects->settings_button_info->callback(info_callback_BE);
		gluco_objects->settings_button_save->callback((Fl_Callback*)settings_variables_to_database_callback,gluco_objects);


	//settingswindowbox->hide();
		gluco_objects->settingswindowbox->color(FL_WHITE);
		gluco_objects->settingswindowbox->box(FL_NO_BOX);

}

void load_BE_window(Gluco_obj* gluco_objects){


	gluco_objects->BE_window = make_shared<Gluco_Window>(400,200,700,700, "BE - Faktoren");

	gluco_objects->BE_windowbox = make_shared<Fl_Box>(0,0,700,700);
	gluco_objects->BE_windowbox->box(FL_BORDER_BOX);
	gluco_objects->BE_windowbox->color(FL_WHITE);
	gluco_objects->BE_f_morning = make_shared<Fl_Box>(20,25,200,50, "BE - Faktor morgens:");
	gluco_objects->BE_t_morning = make_shared<Fl_Box>(70,100,200,50, "Zeitraumdefinition \"morgens\": ");
	gluco_objects->BE_f_noon = make_shared<Fl_Box>(12,175,200,50, "BE - Faktor mittags:");
	gluco_objects->BE_t_noon = make_shared<Fl_Box>(63,250,200,50, "Zeitraumdefinition \"mittags\": ");
	gluco_objects->BE_f_eve = make_shared<Fl_Box>(15,325,200,50, "BE - Faktor abends:");
	gluco_objects->BE_t_eve = make_shared<Fl_Box>(63,400,200,50, "Zeitraumdefinition \"abends\": ");
	gluco_objects->BE_f_night = make_shared<Fl_Box>(12,475,200,50, "BE - Faktor nachts:");
	gluco_objects->BE_t_night = make_shared<Fl_Box>(60,550,200,50, "Zeitraumdefinition \"nachts\": ");

	gluco_objects->BE_text_1 = make_shared<Fl_Box>(475,100,50,50, "bis");
	gluco_objects->BE_text_2 = make_shared<Fl_Box>(475,250,50,50, "bis");
	gluco_objects->BE_text_3 = make_shared<Fl_Box>(475,400,50,50, "bis");
	gluco_objects->BE_text_4 = make_shared<Fl_Box>(475,550,50,50, "bis");

	gluco_objects->BE_input_f_morning = make_shared<Fl_Input>(330,25,150,50);
	gluco_objects->BE_input_t_morning_from = make_shared<Fl_Input>(330,100,150,50);
	gluco_objects->BE_input_t_morning_to = make_shared<Fl_Input>(520,100,150,50);

	gluco_objects->BE_input_f_noon = make_shared<Fl_Input>(330,175,150,50);
	gluco_objects->BE_input_t_noon_from = make_shared<Fl_Input>(330,250,150,50);
	gluco_objects->BE_input_t_noon_to = make_shared<Fl_Input>(520,250,150,50);

	gluco_objects->BE_input_f_eve = make_shared<Fl_Input>(330,325,150,50);
	gluco_objects->BE_input_t_eve_from = make_shared<Fl_Input>(330,400,150,50);
	gluco_objects->BE_input_t_eve_to = make_shared<Fl_Input>(520,400,150,50);

	gluco_objects->BE_input_f_night = make_shared<Fl_Input>(330,475,150,50);
	gluco_objects->BE_input_t_night_from = make_shared<Fl_Input>(330,550,150,50);
	gluco_objects->BE_input_t_night_to = make_shared<Fl_Input>(520,550,150,50);

	gluco_objects->BE_button_safe = make_shared<Gluco_Button>(400,635,120,40, "Speichern");
	gluco_objects->BE_button_cancel = make_shared<Gluco_Button>(550,635,120,40, "Abbrechen");

	gluco_objects->BE_button_safe->color(FL_DARK_GREEN);
	gluco_objects->BE_button_safe->labelcolor(FL_WHITE);
	gluco_objects->BE_button_cancel->color(FL_RED);
	gluco_objects->BE_button_cancel->labelcolor(FL_WHITE);

	gluco_objects->BE_window->end();
	gluco_objects->BE_window->hide();

	gluco_objects->BE_f_morning->labelsize(24);
	gluco_objects->BE_t_morning->labelsize(24);
	gluco_objects->BE_f_noon->labelsize(24);
	gluco_objects->BE_t_noon->labelsize(24);
	gluco_objects->BE_f_eve->labelsize(24);
	gluco_objects->BE_t_eve->labelsize(24);
	gluco_objects->BE_f_night->labelsize(24);
	gluco_objects->BE_t_night->labelsize(24);
	gluco_objects->BE_text_1->labelsize(24);
	gluco_objects->BE_text_2->labelsize(24);
	gluco_objects->BE_text_3->labelsize(24);
	gluco_objects->BE_text_4->labelsize(24);
	gluco_objects->BE_input_f_morning->textsize(24);
	gluco_objects->BE_input_t_morning_from->textsize(24);
	gluco_objects->BE_input_t_morning_to->textsize(24);
	gluco_objects->BE_input_f_noon->textsize(24);
	gluco_objects->BE_input_t_noon_from->textsize(24);
	gluco_objects->BE_input_t_noon_to->textsize(24);
	gluco_objects->BE_input_f_eve->textsize(24);
	gluco_objects->BE_input_t_eve_from->textsize(24);
	gluco_objects->BE_input_t_eve_to->textsize(24);
	gluco_objects->BE_input_f_night->textsize(24);
	gluco_objects->BE_input_t_night_from->textsize(24);
	gluco_objects->BE_input_t_night_to->textsize(24);

	gluco_objects->BE_button_safe->callback((Fl_Callback*)settings_variables_BE_to_database_callback,gluco_objects);
	gluco_objects->BE_button_cancel->callback(settings_BE_close_callback);

}


}

