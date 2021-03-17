#include <Gluco_read_window.h>

namespace Gluco{

shared_ptr<Gluco_Database> read_getDatabase(){

	fs::path filepath{};

	Fl_Native_File_Chooser explorer;
	explorer.title("Gluco Datenbank Datei auswählen");
	explorer.type(Fl_Native_File_Chooser::BROWSE_FILE);
	explorer.filter("*.db");
	if(!explorer.show()){
		filepath = explorer.filename();
		cout << "ReadMode Path: " << filepath << endl;
	}

	if(filepath.empty()){
		return NULL;
	}

	if(!is_init(filepath)){
		fl_alert("Datenbank ist nicht initialisiert oder eine Export-Datei");
		return NULL;
	}
	else{
		return make_shared<Gluco_Database>(filepath,true);
	}
}

void table_filter_callback(Fl_Widget* w, vector<Gluco_Table_*>* t_cont){

	string date1 = ((Fl_Input*)w->parent()->child(0))->value();
	string date2 = ((Fl_Input*)w->parent()->child(1))->value();

	if(!date1.empty() && !checktimestring(date1,true)){
			fl_alert("Bitte benutzen sie ein korrektes Zeitformat!\nYYYY-MM-DD HH:MM\nYYYY-MM-DD\njetzt");
			return;
	}
	if(!date2.empty() && !checktimestring(date2,true)){
		fl_alert("Bitte benutzen sie ein korrektes Zeitformat!\nYYYY-MM-DD HH:MM\nYYYY-MM-DD\njetzt");
		return;
	}

	for(auto table : *t_cont){

		table->applyfilter(date1,date2);

	}


}

void read_window_close_callback(Fl_Widget* w,void* t_cont){

	delete (vector<Gluco_Table_*>*)t_cont;
	((Fl_Window*)w)->~Fl_Window();

}

void load_read_window(int x, int y, int w, int h){

	auto database = read_getDatabase();

	if(database.use_count()==0){
		return;
	}

	//Create Table_Container
	vector<Gluco_Table_*>* t_cont = new vector<Gluco_Table_*>;

	Gluco_Window* read_window = new Gluco_Window(x,y,w,h,"Glu&Co Track Reader");
		read_window->callback(read_window_close_callback,t_cont);

	x = 0;
	y = 0;
	int h_t=h-100;

	Gluco_Tab* table_group = new Gluco_Tab(x,y,w,h_t+30);



	y = 30;

	Gluco_Group* glucose_group = new Gluco_Group(x,y,w,h_t,"Blutzucker");
		glucose_group->selection_color(88);	//rot

		//Funktion fuer Tabelle "Blutzucker"
		Gluco_Table_glucose* glucose_table = new Gluco_Table_glucose(x,y,w,h_t,database);
		glucose_table->color(88);

		t_cont->push_back(glucose_table);

			glucose_group->end();

		Gluco_Group* insulin_group=new Gluco_Group(x,y,w,h_t,"Insulin");
			insulin_group->selection_color(14);	//türkis

			//Funktion fuer Tabelle "Insulin"
			Gluco_Table_insulin* insulin_table = new Gluco_Table_insulin(x,y,w,h_t,database);
			insulin_table->color(14);

			t_cont->push_back(insulin_table);

			insulin_group->end();

		Gluco_Group* food_group = new Gluco_Group(x,y,w,h_t,"Nahrung");
			food_group->selection_color(FL_DARK_GREEN);	//gruen

			//Funktion fuer Tabelle "Nahrung"
			Gluco_Table_food* food_table = new Gluco_Table_food(x,y,w,h_t,database);
			food_table->color(FL_DARK_GREEN);

			t_cont->push_back(food_table);

			food_group->end();

		Gluco_Group* bloodpressure_group = new Gluco_Group(x,y,w,h_t,"Blutdruck");
			bloodpressure_group->selection_color(94);	//gelb

			//Funktion fuer Tabelle "Blutdruck"
			Gluco_Table_bloodpressure* bloodpressure_table = new Gluco_Table_bloodpressure(x,y,w,h_t,database);
			bloodpressure_table->color(94);

			t_cont->push_back(bloodpressure_table);

			bloodpressure_group->end();

		Gluco_Group* config_group = new Gluco_Group(x,y,w,h_t,"Persönliche Daten");
			config_group->selection_color(12);

			//Funktion fuer Tabelle "Persönliche Daten"
			Gluco_Table_config* config_table = new Gluco_Table_config(x,y,w,h_t,database);
			config_table->color(12);

			config_group->end();

	table_group->do_callback(); //sets selection color of starting tab

	table_group->end();

	Fl_Group* date_group = new Fl_Group(x,h-100,w,100);

	new Fl_Input(w*0.25-50,h-50,100,30);
	new Fl_Input(w*0.25+130,h-50,100,30);

	//Datumsauswahl
	Fl_Box* date_select_box_1 = new Fl_Box(w*0.25-250,h-50,200,30, "Daten anzeigen von:");
		date_select_box_1->labelsize(20);
	Fl_Box* date_select_box_2 = new Fl_Box(w*0.25+70,h-50,50,30, "bis:");
		date_select_box_2->labelsize(20);

	Gluco_Button* date_select_button = new Gluco_Button(w*0.75+100,h-50,120,30, "Anwenden");
		date_select_button->color(FL_DARK_GREEN);
		date_select_button->labelcolor(FL_WHITE);
		date_select_button->labelsize(20);
		date_select_button->callback((Fl_Callback*)table_filter_callback,t_cont);

	date_group->end();

	read_window->show();

}



}

