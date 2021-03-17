#include <Gluco_init_window.h>

namespace Gluco{

void init_save_config(Gluco_obj* gluco_objects){

	//Create internal config
	gluco_objects->config = make_shared<Gluco_config>();

	//save config to internal config
	gluco_objects->config->name = gluco_objects->init_input_name->value();
	gluco_objects->config->limit_hypogly = std::strtod(gluco_objects->init_input_hypogly->value(),NULL);
	gluco_objects->config->limit_hypergly = std::strtod(gluco_objects->init_input_hypergly->value(),NULL);
	gluco_objects->config->BE_day = std::strtod(gluco_objects->init_input_f_morning->value(),NULL);
	gluco_objects->config->BE_noon = std::strtod(gluco_objects->init_input_f_noon->value(),NULL);
	gluco_objects->config->BE_evening = std::strtod(gluco_objects->init_input_f_eve->value(),NULL);
	gluco_objects->config->BE_night = std::strtod(gluco_objects->init_input_f_night->value(),NULL);
	gluco_objects->config->day_begin = gluco_objects->init_input_t_morning_from->value();
	gluco_objects->config->day_end = gluco_objects->init_input_t_morning_to->value();
	gluco_objects->config->noon_begin = gluco_objects->init_input_t_noon_from->value();
	gluco_objects->config->noon_end = gluco_objects->init_input_t_noon_to->value();
	gluco_objects->config->evening_begin = gluco_objects->init_input_t_eve_from->value();
	gluco_objects->config->evening_end = gluco_objects->init_input_t_eve_to->value();
	gluco_objects->config->night_begin= gluco_objects->init_input_t_night_from->value();
	gluco_objects->config->noon_end = gluco_objects->init_input_t_night_to->value();

	//save config into database
	gluco_objects->database->save_config(*(gluco_objects->config));
}

void init_load_windows(Gluco_obj* gluco_objects){

	//Hides init-Group
	gluco_objects->init_Group->hide();

	load_main_window(gluco_objects);
	load_table_window(300,50,645,635, gluco_objects);
	load_input_window(gluco_objects);
	load_export_window(gluco_objects);
	load_settings_window(gluco_objects);
	load_input_button_windows(gluco_objects);
	load_BE_window(gluco_objects);

	//Add Groups to main window
	gluco_objects->window->add(*(gluco_objects->mainwindow_Group));
	gluco_objects->window->add(*(gluco_objects->table_group));
	gluco_objects->window->add(*(gluco_objects->data_input_buttons_Group));
	gluco_objects->window->add(*(gluco_objects->export_Group));
	gluco_objects->window->add(*(gluco_objects->settings_Group));

	//Show Main window
	gluco_objects->mainwindow_Group->show();
}

void init_save_location_callback(Fl_Widget *w, void* gluco_objects_ptr){
	Gluco_obj* gluco_objects = (Gluco_obj*) gluco_objects_ptr;
	fs::path cwd = fs::current_path();
	Fl_Native_File_Chooser init_file_chooser;
	init_file_chooser.title("Bitte Datenbank-Speicherort wählen");
	init_file_chooser.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
	init_file_chooser.options(init_file_chooser.Option::SAVEAS_CONFIRM);
	init_file_chooser.directory(cwd.string().c_str());
	init_file_chooser.filter("*.db");
	if(!init_file_chooser.show()){
		gluco_objects->init_input_save_location->value(init_file_chooser.filename());
	}
}

void init_db_open_callback(Fl_Widget *w, void* gluco_objects_ptr){

	Gluco_obj* gluco_objects = (Gluco_obj*) gluco_objects_ptr;

	//Get filepath from window
	fs::path filepath (gluco_objects->init_input_save_location->value());


	//Check if filepath empty or no correct type
	if(filepath.empty() || !fl_filename_match(filepath.string().c_str(),"*.db")){

		fl_alert("Bitte geben sie einen gülitigen Pfad für die Datenbank ein!");
		return;
	}
	else{
		filepath = fs::absolute(filepath);
	}

	if(fs::exists(filepath) && is_init(filepath)){

		cout << "Open Database: '" << filepath << "'" << endl;

		gluco_objects->database = make_shared<Gluco_Database>(filepath);

		gluco_objects->config = make_shared<Gluco_config>();
		gluco_objects->database->load_config(*(gluco_objects->config));

		save_lastdb(filepath);
		init_load_windows(gluco_objects);
	}




}


void init_window_callback(Fl_Widget *w, void* gluco_objects_ptr){
	Gluco_obj* gluco_objects = (Gluco_obj*) gluco_objects_ptr;

	//Get filepath from window
	fs::path filepath (gluco_objects->init_input_save_location->value());


	//Check if filepath empty or no correct type
	if(filepath.empty() || !fl_filename_match(filepath.string().c_str(),"*.db")){

		fl_alert("Bitte geben sie einen gülitigen Pfad für die Datenbank ein!");
		return;
	}
	else{
		filepath = fs::absolute(filepath);
	}

	//Check if Database Database that is selected, is already init and if, what do do next
	if(fs::exists(filepath) && is_init(filepath)){
		switch ( fl_choice("Wollen Sie die Datebank wirklich überschreiben?",
				"Abbrechen","Alles überschreiben", "Nur Einstellungen überschreiben") ) {
		case 0:
			return;
		case 1:

			cout << "Override Database: '" << filepath << "'" << endl;
			std::remove(filepath.string().c_str());

			gluco_objects->database = make_shared<Gluco_Database>(filepath);

			init_save_config(gluco_objects);
			save_lastdb(filepath);
			break;
		case 2:
			cout << "Open Database, override config: '" << filepath << "'" << endl;

			gluco_objects->database = make_shared<Gluco_Database>(filepath);

			init_save_config(gluco_objects);
			save_lastdb(filepath);

			break;
		}
	}
	else{
		cout << "Create Database: '" << filepath << "'" << endl;
		gluco_objects->database = make_shared<Gluco_Database>(filepath);
		init_save_config(gluco_objects);
		save_lastdb(filepath);
	}

	// Load Main Window

	init_load_windows(gluco_objects);

}

void load_init_window(Gluco_obj* gluco_objects){
	gluco_objects->init_Group=make_shared<Fl_Group>(0,0,1000,750);
	gluco_objects->init_Group->begin();

	gluco_objects->init_boxwindow=make_shared<Fl_Box>(0,0,1000,750);

	gluco_objects->init_textbox_welcome1=make_shared<Fl_Box>(70,25,900,50,"Herzlich Willkommen bei Glu&Co Track! \nZur erstmaligen Einrichtung des Programms geben Sie nachfolgend bitte Ihre benötigten Daten an. \nFalls Sie Ihren BE-Faktor nicht kennen, können Sie diesen bei Ihrem Diabetologen erfahren.");

	gluco_objects->init_textbox_name=make_shared<Fl_Box>(200,120,200,50, "Name:");
	gluco_objects->init_textbox_hypergly=make_shared<Fl_Box>(370,170,200,50, "Grenzwert Überzucker:                                 mg/dl");
	gluco_objects->init_textbox_hypogly=make_shared<Fl_Box>(370,220,200,50, "Grenzwert Unterzucker:                                mg/dl");

	gluco_objects->init_input_name = make_shared<Fl_Input>(338,120,275,40);
	gluco_objects->init_input_hypergly = make_shared<Fl_Float_Input>(463,170,150,40);
	gluco_objects->init_input_hypogly = make_shared<Fl_Float_Input>(463,220,150,40);

	gluco_objects->init_textbox_welcome2=make_shared<Fl_Box>(70,280,900,50,"Nachfolgend können Sie die BE-Faktoren und deren Zeiträume festlegen. \nAlle Daten können auch später noch in den Einstellungen bearbeitet werden.");

	gluco_objects->init_textbox_welcome3=make_shared<Fl_Box>(180,565,200,50,"Speicherort der Datenbank festlegen:");

	gluco_objects->init_textbox_1=make_shared<Fl_Box>(750,350,40,50,"bis");
	gluco_objects->init_textbox_2=make_shared<Fl_Box>(750,400,40,50,"bis");
	gluco_objects->init_textbox_3=make_shared<Fl_Box>(750,450,40,50,"bis");
	gluco_objects->init_textbox_4=make_shared<Fl_Box>(750,500,40,50,"bis");

	gluco_objects->init_f_morning = make_shared<Fl_Box>(100,350,200,50, " BE - Faktor morgens:");
	gluco_objects->init_t_morning = make_shared<Fl_Box>(420,350,200,50, "  Zeitraumdefinition \"morgens\": ");
	gluco_objects->init_f_noon = make_shared<Fl_Box>(99,400,200,50, "BE - Faktor mittags: ");
	gluco_objects->init_t_noon = make_shared<Fl_Box>(420,400,200,50, "Zeitraumdefinition \"mittags\": ");
	gluco_objects->init_f_eve = make_shared<Fl_Box>(100,450,200,50, "BE - Faktor abends: ");
	gluco_objects->init_t_eve = make_shared<Fl_Box>(420,450,200,50, "Zeitraumdefinition \"abends\": ");
	gluco_objects->init_f_night = make_shared<Fl_Box>(97,500,200,50, "BE - Faktor nachts: ");
	gluco_objects->init_t_night = make_shared<Fl_Box>(419,500,200,50, "Zeitraumdefinition \"nachts\": ");

	gluco_objects->init_input_f_morning = make_shared<Fl_Float_Input>(290,350,100,40);
	gluco_objects->init_input_t_morning_from = make_shared<Fl_Input>(640,350,100,40);
	gluco_objects->init_input_t_morning_to = make_shared<Fl_Input>(800,350,100,40);
	gluco_objects->init_input_f_noon = make_shared<Fl_Float_Input>(290,400,100,40);
	gluco_objects->init_input_t_noon_from = make_shared<Fl_Input>(640,400,100,40);
	gluco_objects->init_input_t_noon_to = make_shared<Fl_Input>(800,400,100,40);
	gluco_objects->init_input_f_eve = make_shared<Fl_Float_Input>(290,450,100,40);
	gluco_objects->init_input_t_eve_from = make_shared<Fl_Input>(640,450,100,40);
	gluco_objects->init_input_t_eve_to = make_shared<Fl_Input>(800,450,100,40);
	gluco_objects->init_input_f_night = make_shared<Fl_Float_Input>(290,500,100,40);
	gluco_objects->init_input_t_night_from = make_shared<Fl_Input>(640,500,100,40);
	gluco_objects->init_input_t_night_to = make_shared<Fl_Input>(800,500,100,40);

	gluco_objects->init_input_save_location = make_shared<Fl_Input>(450,565,410,40);

	gluco_objects->threedots=make_shared<Fl_Button>(860,565,40,40,"...");

	gluco_objects->use_existing_database_button=make_shared<Fl_Button>(150,650,300,50,"Vorhandene Datenbank verwenden");

	gluco_objects->create_database_button=make_shared<Fl_Button>(550,650,300,50,"DATENBANK ANLEGEN");

	gluco_objects->init_Group->end();
	gluco_objects->init_Group->show();

	gluco_objects->init_textbox_welcome3->labelsize(20);
	gluco_objects->init_textbox_welcome2->labelsize(20);
	gluco_objects->init_textbox_welcome1->labelsize(20);
	gluco_objects->init_textbox_welcome1->labelcolor(12);
	gluco_objects->init_textbox_welcome1->labelfont(FL_BOLD);

	gluco_objects->init_textbox_1->labelsize(20);
	gluco_objects->init_textbox_2->labelsize(20);
	gluco_objects->init_textbox_3->labelsize(20);
	gluco_objects->init_textbox_4->labelsize(20);

	gluco_objects->init_f_morning->labelsize(18);
	gluco_objects->init_t_morning->labelsize(18);
	gluco_objects->init_f_noon->labelsize(18);
	gluco_objects->init_t_noon->labelsize(18);
	gluco_objects->init_f_eve->labelsize(18);
	gluco_objects->init_t_eve->labelsize(18);
	gluco_objects->init_f_night->labelsize(18);
	gluco_objects->init_t_night->labelsize(18);

	gluco_objects->init_textbox_name->labelsize(18);
	gluco_objects->init_textbox_hypergly->labelsize(18);
	gluco_objects->init_textbox_hypogly->labelsize(18);

	gluco_objects->create_database_button->color(FL_DARK_GREEN);
	gluco_objects->create_database_button->down_color(FL_WHITE);
	gluco_objects->create_database_button->labelcolor(FL_WHITE);
	gluco_objects->create_database_button->labelsize(18);
	gluco_objects->use_existing_database_button->color(12);
	gluco_objects->use_existing_database_button->down_color(FL_WHITE);
	gluco_objects->use_existing_database_button->labelcolor(FL_WHITE);
	gluco_objects->use_existing_database_button->labelsize(18);
	gluco_objects->threedots->labelsize(18);
	gluco_objects->threedots->down_color(52);
	gluco_objects->threedots->color(FL_WHITE);
	gluco_objects->threedots->callback(init_save_location_callback, gluco_objects);
	gluco_objects->create_database_button->callback(init_window_callback, gluco_objects);
	gluco_objects->use_existing_database_button->callback(init_db_open_callback, gluco_objects);

	gluco_objects->window->show();

}

}
