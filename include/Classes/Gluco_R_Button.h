#ifndef SRC_GLUCO_R_BUTTON_H_
#define SRC_GLUCO_R_BUTTON_H_

#include <FL/Fl_Radio_Button.H>

namespace Gluco {

class Gluco_R_Button: public Fl_Radio_Button {
public:
	Gluco_R_Button(int x,int y,int width, int height,const char* title);
	virtual ~Gluco_R_Button();
};

} /* namespace Gluco */

#endif /* SRC_GLUCO_R_BUTTON_H_ */
