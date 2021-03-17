#include <Classes/Gluco_Group.h>

namespace Gluco {

Gluco_Group::Gluco_Group(int x,int y,int width, int height,const char* title)
			: Fl_Group(x,y,width,height,title)
{

	this->color(255);
	this->selection_color(255);

}

Gluco_Group::~Gluco_Group() {
	// TODO Auto-generated destructor stub
}

} /* namespace Gluco */
