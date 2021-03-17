#ifndef SRC_GLUCO_FILEMANIPULATION_H_
#define SRC_GLUCO_FILEMANIPULATION_H_

#include "gluco.h"

namespace Gluco{

fs::path getlastDatabase();
void save_lastdb(fs::path filepath);
vector<vector<string>> get_data_from_tsv(fs::path filepath);

}



#endif /* SRC_GLUCO_FILEMANIPULATION_H_ */
