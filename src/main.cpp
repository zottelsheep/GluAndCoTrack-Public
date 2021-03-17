//INCLUDES
#include <gluco.h>
#include <modules.h>

using namespace Gluco;

//void fillDatabase(Gluco_obj &gluco_objects){
//
//	std::cout << "Attempting Data-Input" << std::endl;
//	std::vector<string> tags = {"tag1","tag2","tag3"};
//	std::vector<string> tags2 = {"erhöht"};
//	gluco_objects.database->into_glucose("24-12-2020 00:50",53.4,"This is a note",tags);
//	gluco_objects.database->print_table("glucose");
//
//	gluco_objects.database->into_insulin("24-12-2020 12:04",28.2,"Weihnachten!");
//	gluco_objects.database->print_table("insulin");
//
//	gluco_objects.database->into_food("24-12-2020 12:04","disgusang",40,10000,"lets not talk about it");
//	gluco_objects.database->print_table("food");
//
//	gluco_objects.database->into_bloodpressure("2020-12-24 12:04",141,85,"erhöht",tags2);
//	gluco_objects.database->print_table("bloodpressure");
//
//}


// Main

int main(){
	Fl::scheme("gtk+");

//	string filepath="Debug/test.db";
//	std::remove(filepath.c_str());


	Gluco_obj gluco_objects;
	create_main_window(275,40,1000,750,"Glu&Co Track", &gluco_objects);

	fs::path database_path = getlastDatabase();

	if(!database_path.empty()){

		cout << "Found Database at: " << database_path << endl;

		if(is_init(database_path) & 0b01){
			cout << "Database exists, is init" << endl;
			gluco_objects.database = make_shared<Gluco_Database>(database_path);
			gluco_objects.config = make_shared<Gluco_config>();
			gluco_objects.database->load_config(*(gluco_objects.config));

			//DEBUG FILLING DB
//			fillDatabase(gluco_objects);

			load_main_window(&gluco_objects);
			load_table_window(300,50,645,635, &gluco_objects);
			load_input_window(&gluco_objects);
			load_export_window(&gluco_objects);
			load_settings_window(&gluco_objects);
			gluco_objects.window->show();
			load_input_button_windows(&gluco_objects);
			load_BE_window(&gluco_objects);
		}
		else{
			cout << "Database exists, not init" << endl;
			load_init_window(&gluco_objects);
			gluco_objects.init_input_save_location->value(database_path.string().c_str());
		}

	}
	else{
		cout << "No Database, starting init" << endl;
		load_init_window(&gluco_objects);
	}


	return Fl::run();
}


