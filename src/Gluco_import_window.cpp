#include <Gluco_import_window.h>

//GUI-FUNCTIONS FOR DATA IMPORT

namespace Gluco{

fs::path import_file_chooser(string title){
	Fl_Native_File_Chooser explorer;
	explorer.title(title.c_str());
	explorer.type(Fl_Native_File_Chooser::BROWSE_FILE);
	explorer.filter("*.tsv");
	if(!explorer.show()){
		return explorer.filename();
	}
	return "";
}

void import_tsv_preview_cancel_callback(Fl_Widget* w, void*){

	w->parent()->do_callback();


}

void import_tsv_preview_save_callback(Fl_Widget* w, void* gluco_objects_ptr){

	Gluco_obj* gluco_objects = (Gluco_obj*) gluco_objects_ptr;

	int rc = gluco_objects->database->import_vector_data(*(gluco_objects->import_temp_table_data));
	if(rc & 0x10){
		fl_alert("FALSCHES FORMAT");
	}
	else if(rc & 0x01){
		fl_alert("TSV-Import fehlgeschlagen");
	}
	w->parent()->do_callback();
	return;

}

void import_tsv_preview_exit_callback(Fl_Widget*w, void* gluco_objects_ptr){
	Gluco_obj* gluco_objects = (Gluco_obj*) gluco_objects_ptr;

	gluco_objects->import_temp_table_data.reset();
	((Gluco_Window*)w)->~Fl_Window();
}

void import_tsv_preview(Gluco_obj* gluco_objects){

	//Create Preview window

	cout << "Create Preview Window" << endl;

	Gluco_Window* preview_win = new Gluco_Window(300,300,600,700,"PREVIEW");
		preview_win->callback(import_tsv_preview_exit_callback,gluco_objects);

	Gluco_Table_* preview_table = new Gluco_Table_preview(10,10,580,600,*(gluco_objects->import_temp_table_data));

	preview_win->resizable(preview_table);

	Gluco_Button* cancel_button = new Gluco_Button(20,640,200,40,"Abbrechen");
		cancel_button->color(88);
		cancel_button->callback(import_tsv_preview_cancel_callback);

	Gluco_Button* save_button = new Gluco_Button(380,640,200,40,"Speichern");
		save_button->color(FL_DARK_GREEN);
		save_button->callback(import_tsv_preview_save_callback,gluco_objects);


	preview_win->show();



}

void import_tsv_callback(Fl_Widget* w, void* gluco_objects_ptr){

	Gluco_obj* gluco_objects = (Gluco_obj*) gluco_objects_ptr;

	fs::path filepath = import_file_chooser("Wählen Sie eine TSV-Datei zum importieren aus");

	if(filepath.empty()){
		return;
	}

	cout << "Get import table data" << endl;

	gluco_objects->import_temp_table_data = make_shared<vector<vector<string>>>(get_data_from_tsv(filepath));

	cout << "Preview import data" << endl;

	import_tsv_preview(gluco_objects);

}

} /* Namespace Gluco */
