#ifndef SRC_CLASSES_GLUCO_TABLE_CONFIG_H_
#define SRC_CLASSES_GLUCO_TABLE_CONFIG_H_

#include "Gluco_Table_.h"

namespace Gluco{

class Gluco_Table_config : public Gluco_Table_ {

	void select_data() override;
	vector<string> rownames = {"Name","Überzucker-Grenzwert","Unterzucker-Grenzwert","BE-Faktor Tag","BE-Faktor Mittag","BE-Faktor Abend","BE-Faktor Nacht","Tag Begin","Tag Ende","Mittag Begin","Mittag Ende","Abend Begin","Abend Ende", "Nacht Begin", "Nacht Ende"};

public:
	Gluco_Table_config(int X, int Y, int W, int H, std::shared_ptr<Gluco_Database> db_ptr, const char *L=0);
	virtual ~Gluco_Table_config();

};

}

#endif /* SRC_CLASSES_GLUCO_TABLE_CONFIG_H_ */
