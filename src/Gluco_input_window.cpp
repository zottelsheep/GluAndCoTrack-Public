#include <Gluco_input_window.h>

namespace Gluco{

void input_insulin_window_callback(Fl_Widget *w, void* gluco_objects_ptr){
	Gluco_obj* gluco_objects = (Gluco_obj*) gluco_objects_ptr;
	gluco_objects->insulin_window->show();
}

void input_bloodpressure_window_callback(Fl_Widget *w, void* gluco_objects_ptr){
	Gluco_obj* gluco_objects = (Gluco_obj*) gluco_objects_ptr;
	gluco_objects->bloodpressure_window->show();
}

void input_food_window_callback(Fl_Widget *w, void* gluco_objects_ptr){
	Gluco_obj* gluco_objects = (Gluco_obj*) gluco_objects_ptr;
	gluco_objects->food_window->show();
}

void input_glucose_window_callback(Fl_Widget *w, void* gluco_objects_ptr){
	Gluco_obj* gluco_objects = (Gluco_obj*) gluco_objects_ptr;
	gluco_objects->glucose_window->show();
}

void input_exit_insulin_window_callback(Fl_Widget *w, void* gluco_objects_ptr) {
	Gluco_obj* gluco_objects = (Gluco_obj*) gluco_objects_ptr;
	gluco_objects->insulin_window->hide();
	gluco_objects->insulin_input_insulin->value(0);
	gluco_objects->insulin_input_time->value(0);
	gluco_objects->insulin_input_notes->value(0);
}

void input_exit_bloodpressure_window_callback(Fl_Widget *w, void* gluco_objects_ptr) {
	Gluco_obj* gluco_objects = (Gluco_obj*) gluco_objects_ptr;
	gluco_objects->bloodpressure_window->hide();
	gluco_objects->bloodpressure_input_systol->value(0);
	gluco_objects->bloodpressure_input_diastol->value(0);
	gluco_objects->bloodpressure_input_time->value(0);
	gluco_objects->bloodpressure_input_notes->value(0);
	gluco_objects->bloodpressure_choice_tags->value(0);
}

void input_exit_food_window_callback(Fl_Widget *w, void* gluco_objects_ptr) {
	Gluco_obj* gluco_objects = (Gluco_obj*) gluco_objects_ptr;
	gluco_objects->food_window->hide();
	gluco_objects->food_input_bread_unit->value(0);
	gluco_objects->food_input_time->value(0);
	gluco_objects->food_input_meal->value(0);
}

void input_exit_glucose_window_callback(Fl_Widget *w, void* gluco_objects_ptr) {
	Gluco_obj* gluco_objects = (Gluco_obj*) gluco_objects_ptr;
	gluco_objects->glucose_window->hide();
	gluco_objects->glucose_input_glucose->value(0);
	gluco_objects->glucose_input_time->value(0);
	gluco_objects->glucose_input_notes->value(0);
	gluco_objects->glucose_choice_tags->value(0);
}

void input_save_glucose_callback(Fl_Widget*w, void* gluco_objects_ptr){
	Gluco_obj* gluco_objects = (Gluco_obj*) gluco_objects_ptr;

	// Gets variables
	string timestring = gluco_objects->glucose_input_time->value();
	string notes = gluco_objects->glucose_input_notes->value();
	double glucose_value = strtof(gluco_objects->glucose_input_glucose->value(),NULL);
	string selected_tag = gluco_objects->glucose_choice_tags->value();
	vector<string> tags{};

	//Check if timestring is in correct format
	if(!checktimestring(timestring)){
		fl_alert("Bitte geben Sie das Datum im richtigen Format an");
		return;
	}
	//Check for glucose value
	if(!glucose_value){
		fl_alert("Bitte geben Sie einen Blutzuckerwert ein");
		return;
	}
	//Checks for tag
	if(selected_tag != "Kein Tag"){
		tags.push_back(selected_tag);
	}
	//Checks for hyperglycemia and hypoglycemia and if assigns tags
	if(gluco_objects->config->limit_hypogly!=0 && glucose_value <= gluco_objects->config->limit_hypogly){
		fl_alert("Hinweis: Unterzucker");
		tags.push_back("Unterzucker");
	}
	else if (gluco_objects->config->limit_hypergly!=0 && glucose_value >= gluco_objects->config->limit_hypergly){
		fl_alert("Hinweis: Überzucker");
		tags.push_back("Überzucker");
	}

	gluco_objects->database->into_glucose(timestring,glucose_value,notes,tags);

	input_exit_glucose_window_callback(w,gluco_objects_ptr);

}

void input_save_insulin_callback(Fl_Widget*w, void* gluco_objects_ptr){
	Gluco_obj* gluco_objects = (Gluco_obj*) gluco_objects_ptr;

	// Gets variables
	string timestring = gluco_objects->insulin_input_time->value();
	string notes = gluco_objects->insulin_input_notes->value();
	double insulin_value = strtof(gluco_objects->insulin_input_insulin->value(),NULL);

	//Check if timestring is in correct format
	if(!checktimestring(timestring)){
		fl_alert("Bitte geben Sie das Datum im richtigen Format an");
		return;
	}
	//Check for insulin value
	if(!insulin_value){
		fl_alert("Bitte geben Sie einen Blutzuckerwert ein");
		return;
	}


	gluco_objects->database->into_insulin(timestring,insulin_value,notes);

	input_exit_insulin_window_callback(w,gluco_objects_ptr);

}

void input_save_food_callback(Fl_Widget*w, void* gluco_objects_ptr){
	Gluco_obj* gluco_objects = (Gluco_obj*) gluco_objects_ptr;

	// Gets variables
	string timestring = gluco_objects->food_input_time->value();
	string notes = gluco_objects->food_input_notes->value();
	double bu = strtof(gluco_objects->food_input_bread_unit->value(),NULL);
	double cal = strtof(gluco_objects->food_input_kcal->value(),NULL);
	string meal = gluco_objects->food_input_meal->value();

	//Check if timestring is in correct format
	if(!checktimestring(timestring)){
		fl_alert("Bitte geben Sie das Datum im richtigen Format an");
		return;
	}
	//Check for meal
	if(meal.empty()){
		fl_alert("Bitte geben Sie eine Mahlzeit an");
		return;
	}


	gluco_objects->database->into_food(timestring,meal,bu,cal,notes);

	input_exit_food_window_callback(w,gluco_objects_ptr);

}

void input_save_bloodpressure_callback(Fl_Widget*w, void* gluco_objects_ptr){
	Gluco_obj* gluco_objects = (Gluco_obj*) gluco_objects_ptr;

	// Gets variables
	string timestring = gluco_objects->bloodpressure_input_time->value();
	string notes = gluco_objects->bloodpressure_input_notes->value();
	double systol = strtof(gluco_objects->bloodpressure_input_systol->value(),NULL);
	double diastol = strtof(gluco_objects->bloodpressure_input_diastol->value(),NULL);
	string selected_tag = gluco_objects->bloodpressure_choice_tags->value();
	vector<string> tags{};

	//Check for systole & diastole value
	if(!systol || !diastol){
		fl_alert("Bitte geben Sie beide Blutdruck-Werte an");
		return;
	}
	//Check if timestring is in correct format
	if(!checktimestring(timestring)){
		fl_alert("Bitte geben Sie das Datum im richtigen Format an");
		return;
	}
	//Checks for tag
	if(selected_tag != "Kein Tag"){
		tags.push_back(selected_tag);
	}
//	//Checks for high / low bloodpressure
//	if(gluco_objects->config->limit_hypogly && gluco_objects->config->limit_hypergly){
//
//		if(glucose_value <= gluco_objects->config->limit_hypogly){
//			tags.push_back("Unterzucker");
//		}
//
//		else if (glucose_value >= gluco_objects->config->limit_hypergly){
//			tags.push_back("Überzucker");
//		}
//	}

	gluco_objects->database->into_bloodpressure(timestring,systol,diastol,notes,tags);

	input_exit_bloodpressure_window_callback(w,gluco_objects_ptr);

}

void load_input_window(Gluco_obj* gluco_objects){
	//Groesse der Buttons
	int width{250};
	int height{100};
	//Button Gruppe
	gluco_objects->data_input_buttons_Group = make_shared<Fl_Group>(250,0,750,750);
	gluco_objects->data_input_buttons_Group->begin();
		gluco_objects->data_input_window_box = make_shared<Fl_Box>(250,0,750,750);
		gluco_objects->data_input_textbox_1 = make_shared<Fl_Box>(252,62,0,0, "Mit diesen Schaltflächen können Sie einen neuen Eintrag \neiner Messung anlegen.");

		gluco_objects->data_input_insulin = make_shared<Gluco_Button>(650, 225, width, height, "Insulin");{
			gluco_objects->data_input_insulin->color(14);
			//data_input_insulin->selection_color(14);
		}

		gluco_objects->data_input_bloodpressure = make_shared<Gluco_Button>(650, 375, width, height, "Blutdruck");{
			gluco_objects->data_input_bloodpressure->color(94);
			//data_input_blutdruck->selection_color(94);
		}

		gluco_objects->data_input_food = make_shared<Gluco_Button>(350, 375, width, height, "Nahrung");{
			gluco_objects->data_input_food->color(FL_DARK_GREEN);
			//data_input_nahrung->selection_color(FL_DARK_GREEN);
		}

		gluco_objects->data_input_glucose = make_shared<Gluco_Button>(350, 225, width, height, "Blutzucker");{
			gluco_objects->data_input_glucose->color(88);
			//data_input_blutzucker->selection_color(88);
		}

		gluco_objects->data_input_textbox_1->labelsize(24);
		gluco_objects->data_input_textbox_1->align(FL_ALIGN_RIGHT);

	gluco_objects->data_input_buttons_Group->end();
	gluco_objects->data_input_buttons_Group->hide();
}

void load_input_button_windows(Gluco_obj* gluco_objects){
	//Insulin Window
	gluco_objects->insulin_window = make_shared<Gluco_Window>(500,60,600,700, "Insulin");

		gluco_objects->insulin_save_button = make_shared<Fl_Button> (300,635,120,40,"Speichern");{
			gluco_objects->insulin_save_button->color(FL_DARK_GREEN);
			gluco_objects->insulin_save_button->labelcolor(FL_WHITE);
			gluco_objects->insulin_save_button->labelsize(18);
			gluco_objects->insulin_save_button->callback(input_save_insulin_callback,gluco_objects);
		};
		gluco_objects->insulin_cancel_button = make_shared<Fl_Button> (450,635,120,40,"Abbrechen");{
			gluco_objects->insulin_cancel_button->color(88);
			gluco_objects->insulin_cancel_button->labelcolor(FL_WHITE);
			gluco_objects->insulin_cancel_button->labelsize(18);
			gluco_objects->insulin_cancel_button->callback(input_exit_insulin_window_callback,gluco_objects);
		}
		gluco_objects->insulin_textbox1 = make_shared<Fl_Box>(225,50,0,0, "Neuer Eintrag einer Insulinmessung:");{
			gluco_objects->insulin_textbox1->labelsize(24);
		}
		gluco_objects->insulin_textbox2 = make_shared<Fl_Box>(185,150,0,0, "Insulinmenge in Insulineinheiten [IE]:");{
			gluco_objects->insulin_textbox2->labelsize(20);
		}
		gluco_objects->insulin_textbox_insulin_units = make_shared<Fl_Box>(225,225,0,0, "IE");{
			gluco_objects->insulin_textbox_insulin_units->labelsize(20);
		}
		gluco_objects->insulin_textbox_time = make_shared<Fl_Box>(360,295,0,0, "Datum und Uhrzeit der Messung \n(YYYY-MM-DD HH:MM) | \"jetzt\"");{
			gluco_objects->insulin_textbox_time->labelsize(20);
		}
		gluco_objects->insulin_textbox3 = make_shared<Fl_Box>(115,360,0,0, "Zusätzliche Notizen:");{
			gluco_objects->insulin_textbox3->labelsize(20);
		}

		gluco_objects->insulin_input_insulin = make_shared<Fl_Float_Input>(50,200,160,50);
			gluco_objects->insulin_input_insulin->textsize(24);
		gluco_objects->insulin_input_time = make_shared<Fl_Input>(50,270,160,50);
			gluco_objects->insulin_input_time->textsize(24);
		gluco_objects->insulin_input_notes = make_shared<Fl_Multiline_Input>(50,385,500,160);
			gluco_objects->insulin_input_notes->textsize(18);

	gluco_objects->insulin_window->end();
	gluco_objects->insulin_window->hide();

	//Blutdruck Window
	gluco_objects->bloodpressure_window = make_shared<Gluco_Window> (500,60,600,700, "Blutdruck");

		gluco_objects->bloodpressure_cancel_button = make_shared<Fl_Button>(450,635,120,40,"Abbrechen");{
			gluco_objects->bloodpressure_cancel_button->color(88);
			gluco_objects->bloodpressure_cancel_button->labelcolor(FL_WHITE);
			gluco_objects->bloodpressure_cancel_button->labelsize(18);
			gluco_objects->bloodpressure_cancel_button->callback(input_exit_bloodpressure_window_callback,gluco_objects);
		}
		gluco_objects->bloodpressure_save_button = make_shared<Fl_Button>(300,635,120,40,"Speichern");{
			gluco_objects->bloodpressure_save_button->color(FL_DARK_GREEN);
			gluco_objects->bloodpressure_save_button->labelcolor(FL_WHITE);
			gluco_objects->bloodpressure_save_button->labelsize(18);
			gluco_objects->bloodpressure_save_button->callback(input_save_bloodpressure_callback,gluco_objects);
		}
		gluco_objects->bloodpressure_textbox1 = make_shared<Fl_Box>(240,50,0,0, "Neuer Eintrag einer Blutdruckmessung:");{
			gluco_objects->bloodpressure_textbox1->labelsize(24);
		}
		gluco_objects->bloodpressure_textbox_systol = make_shared<Fl_Box>(295,150,0,0, "mmHg systolisch");{
			gluco_objects->bloodpressure_textbox_systol->labelsize(20);
		}
		gluco_objects->bloodpressure_textbox_diastol = make_shared<Fl_Box>(298,225,0,0, "mmHg diastolisch");{
			gluco_objects->bloodpressure_textbox_diastol->labelsize(20);
		}
		gluco_objects->bloodpressure_textbox_time = make_shared<Fl_Box>(361,295,0,0, "Datum und Uhrzeit der Messung \n(YYYY-MM-DD HH:MM) | \"jetzt\"");{
			gluco_objects->bloodpressure_textbox_time->labelsize(20);
		}
		gluco_objects->bloodpressure_textbox_tags = make_shared<Fl_Box>(240,365,0,0, "Tags");{
			gluco_objects->bloodpressure_textbox_tags->labelsize(20);
		}
		gluco_objects->bloodpressure_textbox2 = make_shared<Fl_Box>(115,430,0,0, "Zusätzliche Notizen:");{
			gluco_objects->bloodpressure_textbox2->labelsize(20);
		}

		gluco_objects->bloodpressure_input_systol = make_shared<Fl_Float_Input>(50,125,160,50);
			gluco_objects->bloodpressure_input_systol->textsize(24);
		gluco_objects->bloodpressure_input_diastol = make_shared<Fl_Float_Input>(50,195,160,50);
			gluco_objects->bloodpressure_input_diastol->textsize(24);
		gluco_objects->bloodpressure_input_time = make_shared<Fl_Input>(50,270,160,50);
			gluco_objects->bloodpressure_input_time->textsize(24);
		gluco_objects->bloodpressure_input_notes = make_shared<Fl_Multiline_Input>(50,455,500,160);
			gluco_objects->bloodpressure_input_notes->textsize(18);

		gluco_objects->bloodpressure_choice_tags = make_shared<Fl_Input_Choice>(50,340,160,50);
		gluco_objects->bloodpressure_choice_tags->color(FL_WHITE);
		gluco_objects->bloodpressure_choice_tags->textsize(24);
		gluco_objects->bloodpressure_choice_tags->add("Kein Tag");
		gluco_objects->bloodpressure_choice_tags->add("Sport");
		gluco_objects->bloodpressure_choice_tags->add("Aufregung");
		gluco_objects->bloodpressure_choice_tags->add("Ruhe");
		gluco_objects->bloodpressure_choice_tags->add("Süßigkeiten");
		gluco_objects->bloodpressure_choice_tags->value("Kein Tag");

	gluco_objects->bloodpressure_window->end();
	gluco_objects->bloodpressure_window->hide();

	//Nahrung Window
	gluco_objects->food_window = make_shared<Gluco_Window>(500,60,600,700, "Nahrung");

		gluco_objects->food_save_button = make_shared<Fl_Button>(300,635,120,40,"Speichern");{
			gluco_objects->food_save_button->color(FL_DARK_GREEN);
			gluco_objects->food_save_button->labelcolor(FL_WHITE);
			gluco_objects->food_save_button->labelsize(18);
			gluco_objects->food_save_button->callback(input_save_food_callback,gluco_objects);
		}
		gluco_objects->food_cancel_button = make_shared<Fl_Button>(450,635,120,40,"Abbrechen");{
			gluco_objects->food_cancel_button->color(88);
			gluco_objects->food_cancel_button->labelcolor(FL_WHITE);
			gluco_objects->food_cancel_button->labelsize(18);
			gluco_objects->food_cancel_button->callback(input_exit_food_window_callback,gluco_objects);
		}
		gluco_objects->food_textbox1 = make_shared<Fl_Box>(240,30,0,0, "Neuer Eintrag einer Nahrungsmessung:");{
			gluco_objects->food_textbox1->labelsize(24);
		}
		gluco_objects->food_textbox2 = make_shared<Fl_Box>(190,80,0,0, "Nahrungsmenge in Broteinheiten [BE]:");{
			gluco_objects->food_textbox2->labelsize(20);
		}
		gluco_objects->food_textbox_bread_unit = make_shared<Fl_Box>(230,140,0,0, "BE");{
			gluco_objects->food_textbox_bread_unit->labelsize(20);
		}
		gluco_objects->food_textbox_time = make_shared<Fl_Box>(360,210,0,0, "Datum und Uhrzeit der Messung \n(YYYY-MM-DD HH:MM) | \"jetzt\"");{
			gluco_objects->food_textbox_time->labelsize(20);
		}
		gluco_objects->food_textbox3 = make_shared<Fl_Box>(150,260,0,0, "Nahrungsart/Nahrungsname:");{
			gluco_objects->food_textbox3->labelsize(20);
		}
		gluco_objects->food_textbox_kcal = make_shared<Fl_Box>(292,380,0,0, "Kcal der Mahlzeit");{
			gluco_objects->food_textbox_kcal->labelsize(20);
		}
		gluco_objects->food_textbox4 = make_shared<Fl_Box>(113,430,0,0, "Zusätzliche Notizen:");{
			gluco_objects->food_textbox4->labelsize(20);
		}

		gluco_objects->food_input_bread_unit = make_shared<Fl_Float_Input>(50,115,160,50);
			gluco_objects->food_input_bread_unit->textsize(24);
		gluco_objects->food_input_time = make_shared<Fl_Input>(50,185,160,50);
			gluco_objects->food_input_time->textsize(24);
		gluco_objects->food_input_meal = make_shared<Fl_Input>(50,285,400,50);
			gluco_objects->food_input_meal->textsize(24);
		gluco_objects->food_input_kcal = make_shared<Fl_Float_Input>(50,355,160,50);
			gluco_objects->food_input_kcal->textsize(24);
		gluco_objects->food_input_notes = make_shared<Fl_Multiline_Input>(50,455,500,160);
			gluco_objects->food_input_notes->textsize(18);
	gluco_objects->food_window->end();
	gluco_objects->food_window->hide();

	//Blutzucker Window
	gluco_objects->glucose_window = make_shared<Gluco_Window> (500,60,600,700, "Blutzucker");

		gluco_objects->glucose_cancel_button = make_shared<Fl_Button>(450,635,120,40,"Abbrechen");{
			gluco_objects->glucose_cancel_button->color(88);
			gluco_objects->glucose_cancel_button->labelcolor(FL_WHITE);
			gluco_objects->glucose_cancel_button->labelsize(18);
			gluco_objects->glucose_cancel_button->callback(input_exit_glucose_window_callback,gluco_objects);
		}
		gluco_objects->glucose_save_button = make_shared<Fl_Button>(300,635,120,40,"Speichern");{
			gluco_objects->glucose_save_button->color(FL_DARK_GREEN);
			gluco_objects->glucose_save_button->labelcolor(FL_WHITE);
			gluco_objects->glucose_save_button->labelsize(18);
			gluco_objects->glucose_save_button->callback(input_save_glucose_callback,gluco_objects);
		}
		gluco_objects->glucose_textbox1 = make_shared<Fl_Box>(250,50,0,0, "Neuer Eintrag einer Blutzuckermessung:");{
			gluco_objects->glucose_textbox1->labelsize(24);
		}
		gluco_objects->glucose_textbox2 = make_shared<Fl_Box>(245,150,0,0, "mg/dl");{
			gluco_objects->glucose_textbox2->labelsize(20);
		}
		gluco_objects->glucose_textbox_time = make_shared<Fl_Box>(361,225,0,0, "Datum und Uhrzeit der Messung \n(YYYY-MM-DD HH:MM) | \"jetzt\"");{
			gluco_objects->glucose_textbox_time->labelsize(20);
		}
		gluco_objects->glucose_textbox_tags = make_shared<Fl_Box>(240,295,0,0, "Tags");{
			gluco_objects->glucose_textbox_tags->labelsize(20);
		}
		gluco_objects->glucose_textbox3 = make_shared<Fl_Box>(120,360,0,0, "Zusätzliche Notizen:");{
			gluco_objects->glucose_textbox3->labelsize(20);
		}

		gluco_objects->glucose_input_glucose = make_shared<Fl_Float_Input>(50,125,160,50);
			gluco_objects->glucose_input_glucose->textsize(24);
		gluco_objects->glucose_input_time = make_shared<Fl_Input>(50,200,160,50);
			gluco_objects->glucose_input_time->textsize(24);
		gluco_objects->glucose_input_notes = make_shared<Fl_Multiline_Input>(50,385,500,160);
			gluco_objects->glucose_input_notes->textsize(18);

		gluco_objects->glucose_choice_tags = make_shared<Fl_Input_Choice>(50,270,160,50);
		gluco_objects->glucose_choice_tags->color(FL_WHITE);
		gluco_objects->glucose_choice_tags->textsize(24);
		gluco_objects->glucose_choice_tags->add("Kein Tag");
		gluco_objects->glucose_choice_tags->add("Sport");
		gluco_objects->glucose_choice_tags->add("Aufregung");
		gluco_objects->glucose_choice_tags->add("Ruhe");
		gluco_objects->glucose_choice_tags->add("Süßigkeiten");
		gluco_objects->glucose_choice_tags->value("Kein Tag");


	gluco_objects->glucose_window->end();
	gluco_objects->glucose_window->hide();

	gluco_objects->data_input_insulin->callback(input_insulin_window_callback,gluco_objects);
	gluco_objects->data_input_bloodpressure->callback(input_bloodpressure_window_callback,gluco_objects);
	gluco_objects->data_input_food->callback(input_food_window_callback,gluco_objects);
	gluco_objects->data_input_glucose->callback(input_glucose_window_callback,gluco_objects);
}

}


