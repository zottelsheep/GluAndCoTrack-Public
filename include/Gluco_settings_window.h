#ifndef SRC_GLUCO_SETTINGS_WINDOW_H_
#define SRC_GLUCO_SETTINGS_WINDOW_H_

#include "gluco.h"

namespace Gluco{

//GUI-FUNCTIONS FOR SETTINGS WINDOW
void settings_BE_callback(Fl_Widget *b, void* gluco_objects_ptr);
void info_callback_BE(Fl_Widget* w, long int);
void load_settings_window(Gluco_obj* gluco_objects);
void load_BE_window(Gluco_obj* gluco_objects);
void settings_variables_from_database(Gluco_obj* gluco_objects);
void settings_variables_to_database_callback(Fl_Widget* w, Gluco_obj* gluco_objects);
void settings_variables_BE_to_database_callback(Fl_Widget* w, Gluco_obj* gluco_objects);
void database_backup_callback(Fl_Widget* w, Gluco_obj* gluco_objects);
void database_restore_callback(Fl_Widget* w, Gluco_obj* gluco_objects);


}
#endif /* SRC_GLUCO_SETTINGS_WINDOW_H_ */
