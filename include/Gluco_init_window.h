#ifndef SRC_GLUCO_INIT_WINDOW_H_
#define SRC_GLUCO_INIT_WINDOW_H_

#include "gluco.h"
#include "modules.h"

namespace Gluco{

//GUI-FUNCTIONS FOR INIT WINDOW

void init_load_windows(Gluco_obj* gluco_objects);
void init_save_location_callback(Fl_Widget *w, void* gluco_objects_ptr);
void init_db_open_callback(Fl_Widget *w, void* gluco_objects_ptr);
void init_window_callback(Fl_Widget *w, void* gluco_objects_ptr);
void load_init_window(Gluco_obj* gluco_objects);

}



#endif /* SRC_GLUCO_INIT_WINDOW_H_ */
