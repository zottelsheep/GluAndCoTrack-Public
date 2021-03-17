#ifndef SRC_GLUCO_WINDOW_H_
#define SRC_GLUCO_WINDOW_H_

#include <FL/Fl_Window.H>
#include <FL/Fl_PNG_Image.H>
#include <icon.h>
#include <icon2.h>

namespace Gluco{

class Gluco_Window : public Fl_Window {

	static const Fl_PNG_Image Gluco_win_icon;
	static const Fl_PNG_Image Gluco_win_icon2;

public:
	Gluco_Window(int x, int y,int width, int height,const char* title=0);

	virtual ~Gluco_Window();
	Gluco_Window(const Gluco_Window &other);
};

}/* namespace Gluco */

#endif /* SRC_GLUCO_WINDOW_H_ */
