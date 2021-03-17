#ifndef SRC_CLASSES_GLUCO_TABLE_INSULIN_H_
#define SRC_CLASSES_GLUCO_TABLE_INSULIN_H_

#include "Gluco_Table_.h"

namespace Gluco{

class Gluco_Table_insulin : public Gluco_Table_ {

	void select_data() override;

public:
	Gluco_Table_insulin(int X, int Y, int W, int H, std::shared_ptr<Gluco_Database> db_ptr, const char *L=0);
	virtual ~Gluco_Table_insulin();

};

}

#endif /* SRC_CLASSES_GLUCO_TABLE_INSULIN_H_ */
