#include <Gluco_export_window.h>

namespace Gluco{

fs::path export_p_explorer(const string &title,string type){
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

void export_tsv(Gluco_obj* gluco_objects,const string &date1, const string &date2){

	fs::path save_dir;

	std::ofstream tsv_file;

	if(gluco_objects->export_checkbutton1->value()){
		save_dir = export_p_explorer("Persönliche Daten Export speichern unter","tsv");
		if(save_dir.empty()){
				return;
			}
		tsv_file.open(save_dir.string());
		if(tsv_file.good()){
			tsv_file << gluco_objects->database->export_tsv("config");
			tsv_file.close();
		}
	}
	if(gluco_objects->export_checkbutton2->value()){
		save_dir = export_p_explorer("Insulin Export speichern unter:","tsv");
		if(save_dir.empty()){
				return;
			}
		tsv_file.open(save_dir.string());
		if(tsv_file.good()){
			tsv_file << gluco_objects->database->export_tsv("insulin",date1,date2);
			tsv_file.close();
		}
	}
	if(gluco_objects->export_checkbutton3->value()){
		save_dir = export_p_explorer("Blutzucker Export speichern unter:","tsv");
		if(save_dir.empty()){
				return;
			}
		tsv_file.open(save_dir.string());
		if(tsv_file.good()){
			tsv_file << gluco_objects->database->export_tsv("glucose",date1,date2);
			tsv_file.close();
		}
	}
	if(gluco_objects->export_checkbutton4->value()){
		save_dir = export_p_explorer("Blutdruck Export speichern unter:","tsv");
		if(save_dir.empty()){
				return;
			}
		tsv_file.open(save_dir.string());
		if(tsv_file.good()){
			tsv_file << gluco_objects->database->export_tsv("bloodpressure",date1,date2);
			tsv_file.close();
		}
	}
	if(gluco_objects->export_checkbutton5->value()){
		save_dir = export_p_explorer("Nahrungs Export speichern unter:","tsv");
		if(save_dir.empty()){
				return;
			}
		tsv_file.open(save_dir.string());
		if(tsv_file.good()){
			tsv_file << gluco_objects->database->export_tsv("food",date1,date2);
			tsv_file.close();
		}
	}




}

void export_db(Gluco_obj* gluco_objects,const string &date1, const string &date2){

	fs::path filepath = export_p_explorer("Datenbank Export speichern unter","db");

	if(filepath.empty()){
		return;
	}

	//Get which tables are selected
	vector<string> tables;

	if(gluco_objects->export_checkbutton1->value()){
		tables.push_back("config");
	}
	if(gluco_objects->export_checkbutton2->value()){
		tables.push_back("insulin");
	}
	if(gluco_objects->export_checkbutton3->value()){
		tables.push_back("glucose");
	}
	if(gluco_objects->export_checkbutton4->value()){
		tables.push_back("bloodpressure");
	}
	if(gluco_objects->export_checkbutton5->value()){
		tables.push_back("food");
	}

	gluco_objects->database->export_db(filepath,tables,date1,date2);

}
void export_button_callback(Fl_Widget* w, void* gluco_objects_ptr){
	Gluco_obj* gluco_objects = (Gluco_obj*) gluco_objects_ptr;


	string date1 = gluco_objects->export_start_date->value();
	string date2 = gluco_objects->export_end_date->value();

	if(!date1.empty() && !checktimestring(date1,true)){
		fl_alert("Bitte benutzen sie ein korrektes Zeitformat!\nYYYY-MM-DD HH:MM\nYYYY-MM-DD\njetzt");
		return;
	}
	if(!date2.empty() && !checktimestring(date2,true)){
		fl_alert("Bitte benutzen sie ein korrektes Zeitformat!\nYYYY-MM-DD HH:MM\nYYYY-MM-DD\njetzt");
		return;
	}

	if(gluco_objects->export_rbutton_tsv->value()){
		export_tsv(gluco_objects,date1,date2);
	}
	else if (gluco_objects->export_rbutton_db->value()){
		export_db(gluco_objects,date1,date2);
	}

}

void load_export_window(Gluco_obj* gluco_objects){
	gluco_objects->export_Group = make_shared<Fl_Group>(250,0,750,750); //Used as a group for all items of the exportwindow
	gluco_objects->export_Group->begin(); //(everything following ->begin() is automatically added to this group)
	gluco_objects->exportwindowbox = make_shared<Fl_Box>(250,0,750,750);
	gluco_objects->export_textbox1 = make_shared<Fl_Box>(0,25,900,50,"Bitte wählen Sie die Art des Exports:");
	gluco_objects->export_textbox2 = make_shared<Fl_Box>(20,25,900,350,"Welche Daten möchten Sie exportieren?");
	gluco_objects->export_textbox3 = make_shared<Fl_Box>(210,485,400,100,"Zeitraum:                        bis");
	gluco_objects->export_button = make_shared<Fl_Button>(475,650,300,50,"EXPORTIEREN");

	gluco_objects->export_rbutton_tsv = make_shared<Gluco_R_Button>(365,100,150,50,"TSV-DATEI");
	gluco_objects->export_rbutton_db = make_shared<Gluco_R_Button>(575,100,150,50,"Datenbankexport \n für Arztbesuche\n");

	gluco_objects->export_checkbutton1 = make_shared<Fl_Check_Button>(325,250,25,25,"Persönliche Daten");
	gluco_objects->export_checkbutton2 = make_shared<Fl_Check_Button>(325,300,25,25,"Insulinmessungen");
	gluco_objects->export_checkbutton3 = make_shared<Fl_Check_Button>(325,350,25,25,"Blutzuckermessungen");
	gluco_objects->export_checkbutton4 = make_shared<Fl_Check_Button>(325,400,25,25,"Blutdruckmessungen");
	gluco_objects->export_checkbutton5 = make_shared<Fl_Check_Button>(325,450,25,25,"Nahrungsdaten");
	gluco_objects->export_start_date = make_shared<Fl_Input>(365,510,150,50);
	gluco_objects->export_end_date = make_shared<Fl_Input>(575,510,150,50);
	gluco_objects->export_start_date->textsize(24);
	gluco_objects->export_end_date->textsize(24);
	gluco_objects->export_checkbutton1->labelsize(24);
	gluco_objects->export_checkbutton2->labelsize(24);
	gluco_objects->export_checkbutton3->labelsize(24);
	gluco_objects->export_checkbutton4->labelsize(24);
	gluco_objects->export_checkbutton5->labelsize(24);
	gluco_objects->export_Group->end(); // ends the auto adding to this group
	gluco_objects->export_Group->hide();

	//Callbacks
	gluco_objects->export_button->callback(export_button_callback,gluco_objects);

	//Additional fixings like textsize and color
	gluco_objects->export_button->labelcolor(FL_WHITE);
	gluco_objects->export_textbox1->labelsize(24);
	gluco_objects->export_textbox2->labelsize(24);
	gluco_objects->export_textbox3->labelsize(24);
	gluco_objects->export_button->labelsize(24);
	gluco_objects->export_button->color(FL_DARK_GREEN);
	gluco_objects->export_button->down_color(FL_WHITE);

	gluco_objects->exportwindowbox->color(FL_WHITE);
	gluco_objects->exportwindowbox->box(FL_NO_BOX);
}


}


