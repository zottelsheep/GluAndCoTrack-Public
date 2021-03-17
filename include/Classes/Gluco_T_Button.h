#ifndef SRC_GLUCO_TBUTTON_H_
#define SRC_GLUCO_TBUTTON_H_


#include <FL/Fl_Toggle_Button.H>


namespace Gluco {

class Gluco_T_Button: public Fl_Toggle_Button {
public:
	Gluco_T_Button(int x,int y,int width, int height,const char* title);

	virtual ~Gluco_T_Button();

	Gluco_T_Button(const Gluco_T_Button &other);
};

} /* namespace Gluco */

#endif /* SRC_GLUCO_TBUTTON_H_ */
