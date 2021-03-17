#ifndef SRC_CLASSES_GLUCO_TABLE_GLUCOSE_H_
#define SRC_CLASSES_GLUCO_TABLE_GLUCOSE_H_

#include "Gluco_Table_.h"

namespace Gluco {

class Gluco_Table_glucose : public Gluco_Table_ {

	void select_data() override;

public:
	Gluco_Table_glucose(int X, int Y, int W, int H, std::shared_ptr<Gluco_Database> db_ptr, const char *L=0);
	virtual ~Gluco_Table_glucose();

};

}





#endif /* SRC_CLASSES_GLUCO_TABLE_GLUCOSE_H_ */
