#ifndef SRC_CLASSES_GLUCO_TABLE_PREVIEW_H_
#define SRC_CLASSES_GLUCO_TABLE_PREVIEW_H_

#include "Gluco_Table_.h"

namespace Gluco {

class Gluco_Table_preview : public Gluco_Table_ {

	void select_data() override;

public:
	Gluco_Table_preview(int X, int Y, int W, int H, const vector<vector<string>> & data_vec, const char *L=0);
	virtual ~Gluco_Table_preview();

};

}



#endif /* SRC_CLASSES_GLUCO_TABLE_BLOODPRESSURE_H_ */
