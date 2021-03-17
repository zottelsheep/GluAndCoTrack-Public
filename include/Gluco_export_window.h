#ifndef SRC_GLUCO_EXPORT_WINDOW_H_
#define SRC_GLUCO_EXPORT_WINDOW_H_

#include "gluco.h"

namespace Gluco{

//GUI-FUNCTIONS FOR EXPORT WINDOW
fs::path export_p_explorer(const string &title,string type);
void export_tsv(Gluco_obj* gluco_objects);
void export_button_callback(Fl_Widget* w, void* gluco_objects_ptr);
void load_export_window(Gluco_obj* gluco_objects);

}



#endif /* SRC_GLUCO_EXPORT_WINDOW_H_ */
