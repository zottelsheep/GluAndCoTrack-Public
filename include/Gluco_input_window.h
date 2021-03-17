#ifndef SRC_GLUCO_INPUT_WINDOW_H_
#define SRC_GLUCO_INPUT_WINDOW_H_

#include "gluco.h"
#include "Classes/Gluco_Database.h"

namespace Gluco{


//GUI-FUNCTIONS FOR INPUT WINDOW
void input_insulin_window_callback(Fl_Widget *w, void* gluco_objects_ptr);
void input_food_window_callback(Fl_Widget *w, void* gluco_objects_ptr);
void input_glucose_window_callback(Fl_Widget *w, void* gluco_objects_ptr);
void input_exit_insulin_window_callback(Fl_Widget *w, void* gluco_objects_ptr);
void input_exit_bloodpressure_window_callback(Fl_Widget *w, void* gluco_objects_ptr);
void input_exit_food_window_callback(Fl_Widget *w, void* gluco_objects_ptr);
void input_exit_glucose_window_callback(Fl_Widget *w, void* gluco_objects_ptr);
void input_save_glucose_callback(Fl_Widget*w, void* gluco_objects_ptr);
void load_input_window(Gluco_obj* gluco_objects);
void load_input_button_windows(Gluco_obj* gluco_objects);

} /* namespace Gluco */
#endif /* SRC_GLUCO_INPUT_WINDOW_H_ */
