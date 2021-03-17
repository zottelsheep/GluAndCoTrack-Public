#include <Classes/Gluco_R_Button.h>

namespace Gluco {

Gluco_R_Button::Gluco_R_Button(int x,int y,int width, int height,const char* title)
	: Fl_Radio_Button(x,y,width,height,title)
{
	this->color(39);
	this->down_color(255);
	this->labelsize(18);
	this->labelcolor(255);
}

Gluco_R_Button::~Gluco_R_Button() {
	// TODO Auto-generated destructor stub
}

} /* namespace Gluco */
