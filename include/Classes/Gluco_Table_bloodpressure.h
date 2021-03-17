#ifndef SRC_CLASSES_GLUCO_TABLE_BLOODPRESSURE_H_
#define SRC_CLASSES_GLUCO_TABLE_BLOODPRESSURE_H_

#include "Gluco_Table_.h"

namespace Gluco {

class Gluco_Table_bloodpressure : public Gluco_Table_ {

	void select_data() override;

public:
	Gluco_Table_bloodpressure(int X, int Y, int W, int H, std::shared_ptr<Gluco_Database> db_ptr, const char *L=0);
	virtual ~Gluco_Table_bloodpressure();

};

}



#endif /* SRC_CLASSES_GLUCO_TABLE_BLOODPRESSURE_H_ */
