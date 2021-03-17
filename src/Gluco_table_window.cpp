#include <Gluco_table_window.h>

namespace Gluco{

void table_filter_callback(Fl_Widget* w, void* gluco_objects_ptr){
	Gluco_obj* gluco_objects = (Gluco_obj*) gluco_objects_ptr;

	string date1 = gluco_objects->date_select_input_1->value();
	string date2 = gluco_objects->date_select_input_2->value();

	if(!date1.empty() && !checktimestring(date1,true)){
			fl_alert("Bitte benutzen sie ein korrektes Zeitformat!\nYYYY-MM-DD HH:MM\nYYYY-MM-DD\njetzt");
			return;
	}
	if(!date2.empty() && !checktimestring(date2,true)){
		fl_alert("Bitte benutzen sie ein korrektes Zeitformat!\nYYYY-MM-DD HH:MM\nYYYY-MM-DD\njetzt");
		return;
	}

	gluco_objects->glucose_table->applyfilter(date1,date2);
	gluco_objects->insulin_table->applyfilter(date1,date2);
	gluco_objects->food_table->applyfilter(date1,date2);
	gluco_objects->bloodpressure_table->applyfilter(date1,date2);

}

//GUI-FUNCTIONS FOR TABLES AND TABS

void load_table_window(int x, int y, int w,int h, Gluco_obj* gluco_objects){

gluco_objects->table_group = make_shared<Fl_Group>(x,y-25,w,h+200);
gluco_objects->table_group->begin();

gluco_objects->date_select_group = make_shared<Fl_Group>(x,y+h-60,w,h);
gluco_objects->date_select_group->begin();

//Datumsauswahl
gluco_objects->date_select_box_1 = make_shared<Fl_Box>(x,y+h+10,200,30, "Daten anzeigen von:");
gluco_objects->date_select_box_2 = make_shared<Fl_Box>(x+300,y+h+10,50,30, "bis");
gluco_objects->date_select_box_1->labelsize(20);
gluco_objects->date_select_box_2->labelsize(20);

gluco_objects->date_select_input_1 = make_shared<Fl_Input>(x+200,y+h+10,100,30);
gluco_objects->date_select_input_2 = make_shared<Fl_Input>(x+350,y+h+10,100,30);

gluco_objects->date_select_button = make_shared<Gluco_Button>(x+500,y+h+10,120,30, "Anwenden");
gluco_objects->date_select_button->color(FL_DARK_GREEN);
gluco_objects->date_select_button->labelcolor(FL_WHITE);
gluco_objects->date_select_button->labelsize(20);
gluco_objects->date_select_button->callback(table_filter_callback,gluco_objects);

gluco_objects->date_select_group->end();


//Zeichnen des Tab-Fensters
gluco_objects->table_tabs=make_shared<Gluco_Tab>(x,y-25,w,h);{

//Group ist ein child von Tab und enthaelt den Namen des Tabs sowie die Elemente, die gezeigt werden sollen, wenn der Tab ausgewaehlt wird
	//BLUTZUCKER
	gluco_objects->glucose_group = make_shared<Gluco_Group>(x,y,w,h,"Blutzucker");{

		gluco_objects->glucose_group->selection_color(88);	//rot


		//Funktion fuer Tabelle "Blutzucker"
		gluco_objects->glucose_table = make_shared<Gluco_Table_glucose>(x,y,w,h,gluco_objects->database);
		gluco_objects->glucose_table->color(88);

	}gluco_objects->glucose_group->end();
	//INSULIN
	gluco_objects->insulin_group=make_shared<Gluco_Group>(x,y,w,h,"Insulin");{

		gluco_objects->insulin_group->selection_color(14);	//türkis

		//Funktion fuer Tabelle "Insulin"
		gluco_objects->insulin_table = make_shared<Gluco_Table_insulin>(x,y,w,h,gluco_objects->database);
		gluco_objects->insulin_table->color(14);

	}gluco_objects->insulin_group->end();
	//NAHRUNG
	gluco_objects->food_group = make_shared<Gluco_Group>(x,y,w,h,"Nahrung");{

		gluco_objects->food_group->selection_color(FL_DARK_GREEN);	//grün

		//Funktion fuer Tabelle "Nahrung"
		gluco_objects->food_table = make_shared<Gluco_Table_food>(x,y,w,h,gluco_objects->database);
		gluco_objects->food_table->color(FL_DARK_GREEN);

	}gluco_objects->food_group->end();
	//BLUTDRUCK
	gluco_objects->bloodpressure_group = make_shared<Gluco_Group>(x,y,w,h,"Blutdruck");{

		gluco_objects->bloodpressure_group->selection_color(94);	//gelb

		//Funktion fuer Tabelle "Blutdruck"
		gluco_objects->bloodpressure_table = make_shared<Gluco_Table_bloodpressure>(x,y,w,h,gluco_objects->database);
		gluco_objects->bloodpressure_table->color(94);


	}gluco_objects->bloodpressure_group->end();

	//Colouring
	gluco_objects->table_tabs->do_callback(); //sets selection color of starting tab

}gluco_objects->table_tabs->end();
//gluco_objects->table_tabs->hide();

gluco_objects->table_group->end();
gluco_objects->table_group->hide();

}//End of Function

}//End of Namespace


