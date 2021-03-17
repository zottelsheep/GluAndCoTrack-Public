#include <Classes/Gluco_Tab.h>

namespace Gluco {

Gluco_Tab::Gluco_Tab(int x,int y,int width, int height) : Fl_Tabs(x,y,width,height)
{
	color();
	callback(tab_select_color);

}

Gluco_Tab::~Gluco_Tab() {
	// TODO Auto-generated destructor stub
}

void tab_select_color(Fl_Widget *w, void*) {
  Fl_Tabs *tabs = (Fl_Tabs*)w;
  // When tab changed, make sure it has same color as its group
  tabs->selection_color( (tabs->value())->selection_color() );
}

} /* namespace Gluco */
