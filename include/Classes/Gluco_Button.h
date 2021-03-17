#ifndef SRC_GLUCO_BUTTON_H_
#define SRC_GLUCO_BUTTON_H_


#include <FL/Fl_Button.H>


namespace Gluco {

class Gluco_Button: public Fl_Button {
public:
	Gluco_Button(int x,int y,int width = 250, int height =100,const char* title=0);

	virtual ~Gluco_Button();

	Gluco_Button(const Gluco_Button &other);
};

} /* namespace Gluco */

#endif /* SRC_GLUCO_BUTTON_H_ */
