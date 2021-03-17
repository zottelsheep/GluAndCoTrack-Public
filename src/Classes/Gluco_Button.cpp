#include <Classes/Gluco_Button.h>

namespace Gluco{

Gluco_Button::Gluco_Button(int x,int y,int width, int height,const char* title)
	: Fl_Button(x,y,width,height,title)

	{
			this->color(39);
			this->down_color(255);
			this->labelsize(18);
			this->labelcolor(255);
	}

Gluco_Button::~Gluco_Button(){

}

} /* namespace Gluco */

