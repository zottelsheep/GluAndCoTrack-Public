#ifndef SRC_GLUCO_TAB_H_
#define SRC_GLUCO_TAB_H_

#include <FL/Fl_Tabs.H>

namespace Gluco {

void tab_select_color(Fl_Widget *w, void*);

class Gluco_Tab: public Fl_Tabs {
public:
	Gluco_Tab(int x,int y,int width, int height);
	virtual ~Gluco_Tab();
	Gluco_Tab(const Gluco_Tab &other);
};

} /* namespace Gluco */

#endif /* SRC_GLUCO_TAB_H_ */
