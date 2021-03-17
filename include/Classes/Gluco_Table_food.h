#ifndef SRC_CLASSES_GLUCO_TABLE_FOOD_H_
#define SRC_CLASSES_GLUCO_TABLE_FOOD_H_


#include "Gluco_Table_.h"

namespace Gluco {

class Gluco_Table_food : public Gluco_Table_ {

	void select_data() override;

public:
	Gluco_Table_food(int X, int Y, int W, int H, std::shared_ptr<Gluco_Database> db_ptr, const char *L=0);
	virtual ~Gluco_Table_food();

};

}




#endif /* SRC_CLASSES_GLUCO_TABLE_FOOD_H_ */
