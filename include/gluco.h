#ifndef SRC_GLUCO_H_
#define SRC_GLUCO_H_

//INCLUDES

//Standard-Libraries
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <regex>
#include <filesystem>

//FLTK
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_RGB_Image.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Native_File_Chooser.H>

#include <Classes/Gluco_Table_bloodpressure.h>
#include <Classes/Gluco_Table_food.h>
#include <Classes//Gluco_Table_glucose.h>
#include <Classes/Gluco_Table_insulin.h>
#include <Classes/Gluco_Table_config.h>
#include <Classes/Gluco_Window.h>
#include <Classes/Gluco_Button.h>
#include <Classes/Gluco_Database.h>
#include <Classes/Gluco_T_Button.h>
#include <Classes/Gluco_R_Button.h>
#include <Classes/Gluco_Group.h>
#include <Classes/Gluco_Tab.h>

// NAMESPACE USAGE

//NAMESPACES
using std::shared_ptr;
using std::make_shared;
using std::string;
using std::vector;
using std::cout;
using std::endl;

using Gluco::Gluco_Button;
using Gluco::Gluco_T_Button;
using Gluco::Gluco_R_Button;
using Gluco::Gluco_Window;
using Gluco::Gluco_Table_insulin;
using Gluco::Gluco_Table_bloodpressure;
using Gluco::Gluco_Table_food;
using Gluco::Gluco_Table_glucose;
using Gluco::Gluco_Group;
using Gluco::Gluco_Tab;
using Gluco::Gluco_Database;

namespace fs = std::filesystem;

// Variables Declaration

namespace Gluco{

struct Gluco_obj{

//HOME-SCREEN
shared_ptr<Gluco_Database> database;
shared_ptr<Gluco_config> config;

shared_ptr<Gluco_Window> window;

shared_ptr<Fl_Box> sidebar;

shared_ptr<Fl_Output> welcome_greeting;

shared_ptr<Gluco_Button>
button_import_menu,
button_read_mode,
db_change;

shared_ptr<Fl_Group>
mainwindow_Group,
welcome_group;

shared_ptr<Gluco_T_Button>
tbutton_tables,
tbutton_entries,
tbutton_export_menu,
tbutton_settings;

//INIT-SCREEN
shared_ptr<Fl_Box>
init_boxwindow,
init_textbox_welcome1,
init_textbox_welcome2,
init_textbox_welcome3,
init_textbox_name,
init_textbox_1,
init_textbox_2,
init_textbox_3,
init_textbox_4,
init_textbox_hypergly,
init_textbox_hypogly,
init_f_morning,
init_t_morning,
init_f_noon,
init_t_noon,
init_f_eve,
init_t_eve,
init_f_night,
init_t_night;

shared_ptr<Fl_Input>
init_input_name,
init_input_t_morning_from,
init_input_t_morning_to,
init_input_t_noon_from,
init_input_t_noon_to,
init_input_t_eve_from,
init_input_t_eve_to,
init_input_t_night_from,
init_input_t_night_to,
init_input_save_location;

shared_ptr<Fl_Float_Input>
init_input_hypergly,
init_input_hypogly,
init_input_f_morning,
init_input_f_noon,
init_input_f_eve,
init_input_f_night;

shared_ptr<Fl_Button>
create_database_button,
use_existing_database_button,
threedots;

shared_ptr<Fl_Group> init_Group;

//EXPORT
shared_ptr<Fl_Box> exportwindowbox;
shared_ptr<Fl_Group> export_Group;

shared_ptr<Gluco_R_Button>
export_rbutton_tsv,
export_rbutton_db;

shared_ptr<Fl_Check_Button>
export_checkbutton1,
export_checkbutton2,
export_checkbutton3,
export_checkbutton4,
export_checkbutton5;

shared_ptr<Fl_Button> export_button;

shared_ptr<Fl_Box>
export_textbox1,
export_textbox2,
export_textbox3;

shared_ptr<Fl_Input>
export_start_date,
export_end_date;


//Tables
shared_ptr<Gluco_Tab> table_tabs;

shared_ptr<Gluco_Group>
insulin_group,
bloodpressure_group,
food_group,
glucose_group;

shared_ptr<Fl_Group>
table_group,
date_select_group;

shared_ptr<Fl_Box>
date_select_box_1,
date_select_box_2;

shared_ptr<Fl_Input>
date_select_input_1,
date_select_input_2;

shared_ptr<Gluco_Button>
date_select_button;

shared_ptr<Gluco_Table_>
insulin_table,
bloodpressure_table,
food_table,
glucose_table;

//INPUT
shared_ptr<Fl_Group>
data_input_buttons_Group;
//data_input_insulin_Group,
//data_input_boodpressure_Group,
//data_input_food_Group,
//data_input_glucose_Group;

shared_ptr<Fl_Box>
data_input_window_box,
data_input_textbox_1;

shared_ptr<Gluco_Button>
data_input_insulin,
data_input_bloodpressure,
data_input_food,
data_input_glucose;

	//INPUT INSULIN
	shared_ptr<Gluco_Window> insulin_window;

	shared_ptr<Fl_Button> insulin_save_button, insulin_cancel_button;

	shared_ptr<Fl_Box>
	insulin_textbox1,
	insulin_textbox2,
	insulin_textbox_insulin_units,
	insulin_textbox_time,
	insulin_textbox3;

	shared_ptr<Fl_Float_Input> insulin_input_insulin;

	shared_ptr<Fl_Input> insulin_input_time;

	shared_ptr<Fl_Multiline_Input> insulin_input_notes;


	//INPUT BLUTDRUCK
	shared_ptr<Gluco_Window> bloodpressure_window;
	shared_ptr<Fl_Button> bloodpressure_save_button, bloodpressure_cancel_button;

	shared_ptr<Fl_Box>
	bloodpressure_textbox1,
	bloodpressure_textbox_systol,
	bloodpressure_textbox_diastol,
	bloodpressure_textbox_time,
	bloodpressure_textbox_tags,
	bloodpressure_textbox2;

	shared_ptr<Fl_Float_Input> bloodpressure_input_systol, bloodpressure_input_diastol;

	shared_ptr<Fl_Input> bloodpressure_input_time;
	shared_ptr<Fl_Input_Choice> bloodpressure_choice_tags;
	shared_ptr<Fl_Multiline_Input> bloodpressure_input_notes;

	//INPUT NAHRUNG
	shared_ptr<Gluco_Window> food_window;
	shared_ptr<Fl_Button> food_save_button, food_cancel_button;

	shared_ptr<Fl_Box>
	food_textbox1,
	food_textbox2,
	food_textbox_bread_unit,
	food_textbox_time,
	food_textbox3,
	food_textbox4,
	food_textbox_kcal;

	shared_ptr<Fl_Float_Input>
	food_input_bread_unit,
	food_input_kcal;
	shared_ptr<Fl_Input> food_input_time;
	shared_ptr<Fl_Input> food_input_meal;
	shared_ptr<Fl_Multiline_Input> food_input_notes;

	//INPUT BLUTZUCKER
	shared_ptr<Gluco_Window> glucose_window;
	shared_ptr<Fl_Button> glucose_save_button, glucose_cancel_button;
	shared_ptr<Fl_Box>
	glucose_textbox1,
	glucose_textbox2,
	glucose_textbox_time,
	glucose_textbox_tags,
	glucose_textbox3;


	shared_ptr<Fl_Float_Input> glucose_input_glucose;
	shared_ptr<Fl_Input> glucose_input_time;
	shared_ptr<Fl_Input_Choice> glucose_choice_tags;
	shared_ptr<Fl_Multiline_Input> glucose_input_notes;

// IMPORT

shared_ptr<vector<vector<string>>> import_temp_table_data;

//SETTINGS

shared_ptr<Fl_Group> settings_Group;


shared_ptr<Fl_Box>
settingswindowbox,
settings_textbox_intro,
settings_textbox_name,
settings_textbox_hypergly,
settings_textbox_hypogly;

shared_ptr<Fl_Input>
settings_input_name,
settings_input_hypergly,
settings_input_hypogly;

shared_ptr<Fl_Button>
settings_button_carbo_exchanges,
settings_button_restore,
settings_button_backup,
settings_button_reset,
settings_button_info,
settings_button_save;



	//SETTING BE_WINDOW

	shared_ptr<Gluco_Window> BE_window;

	shared_ptr<Fl_Box>
	BE_windowbox,
	BE_f_morning,
	BE_t_morning,
	BE_f_noon,
	BE_t_noon,
	BE_f_eve,
	BE_t_eve,
	BE_f_night,
	BE_t_night,
	BE_text_1,
	BE_text_2,
	BE_text_3,
	BE_text_4;

	shared_ptr<Fl_Input>
	BE_input_f_morning,
	BE_input_t_morning_from,
	BE_input_t_morning_to,
	BE_input_f_noon,
	BE_input_t_noon_from,
	BE_input_t_noon_to,
	BE_input_f_eve,
	BE_input_t_eve_from,
	BE_input_t_eve_to,
	BE_input_f_night,
	BE_input_t_night_from,
	BE_input_t_night_to;

	shared_ptr<Fl_Button> BE_button_safe, BE_button_cancel;


};

}

#endif /* SRC_GLUCO_H_ */
