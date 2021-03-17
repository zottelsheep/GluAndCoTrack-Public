#ifndef SRC_GLUCO_GROUP_H_
#define SRC_GLUCO_GROUP_H_


#include <FL/Fl_Group.H>

namespace Gluco{

class Gluco_Group : public Fl_Group{
public:
	Gluco_Group(int x,int y,int width, int height,const char* title);

	virtual ~Gluco_Group();
};

} /* namespace Gluco */

#endif /* SRC_GLUCO_GROUP_H_ */
