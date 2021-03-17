#ifndef SRC_GLUCO_MAIN_WINDOW_H_
#define SRC_GLUCO_MAIN_WINDOW_H_

#include "gluco.h"
#include "Gluco_import_window.h"
#include "Gluco_settings_window.h"
#include "Gluco_FileManipulation.h"
#include "Gluco_read_window.h"

namespace Gluco{

//GUI-FUNCTIONS FOR MAIN WINDOW
void main_tbutton_callback(Fl_Widget *w, void* gluco_objects_ptr);
void main_db_change_callback(Fl_Widget *w, void* gluco_objects_ptr);
void load_main_window(Gluco_obj* gluco_objects);
void create_main_window(const int &x, const int &y, const int &w,const int &h,const char* text, Gluco_obj* gluco_objects);

}



#endif /* SRC_GLUCO_MAIN_WINDOW_H_ */
