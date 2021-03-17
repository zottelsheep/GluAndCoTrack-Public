#include <Classes/Gluco_T_Button.h>

namespace Gluco{

Gluco_T_Button::Gluco_T_Button(int x,int y,int width, int height,const char* title)
	: Fl_Toggle_Button(x,y,width,height,title)

	{
			this->color(39);
			this->down_color(255);
			this->labelsize(18);
			this->labelcolor(255);
	}

Gluco_T_Button::~Gluco_T_Button(){

}

} /* namespace Gluco */

