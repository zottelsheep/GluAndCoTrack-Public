#include <Classes/Gluco_Window.h>

namespace Gluco{

Gluco_Window::Gluco_Window(int x, int y,int width, int height,const char* title) : Fl_Window(x,y,width,height,title)
{
	this->color(FL_WHITE);
	this->begin();

	const Fl_RGB_Image* icon_array[2] = {&Gluco_win_icon,&Gluco_win_icon2};

	icons(icon_array,2);
//	icon(&Gluco_win_icon2);
}

Gluco_Window::~Gluco_Window(){

}

const Fl_PNG_Image Gluco_Window::Gluco_win_icon = {NULL,gluco_icon_png,gluco_icon_png_length};
const Fl_PNG_Image Gluco_Window::Gluco_win_icon2 = {NULL,gluco_icon2_png,gluco_icon2_png_length};

}/* namespace Gluco */

