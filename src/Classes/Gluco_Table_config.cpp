#include <Classes/Gluco_Table_config.h>

namespace Gluco{

//CONSTRUCTORS
Gluco_Table_config::Gluco_Table_config(int X, int Y, int W, int H, std::shared_ptr<Gluco_Database> db_ptr, const char *L) : Gluco_Table_(X,Y,W,H,db_ptr,{"Parameter","Inhalt"},L) {
	// Constructor
	std::cout << "Creating table config" << '\n' << std::endl;

	update_data();
}

Gluco_Table_config::~Gluco_Table_config(){

}



void Gluco_Table_config::select_data(){


	data.clear();

	auto temp_data = db->get_table_data_config();

	vector<string> row{""};
	data.push_back(row);

	for(auto i=0;i<temp_data[1].size()-1;i++){
		row.clear();
		row.push_back(rownames[i]);
		row.push_back(temp_data[1][i]);
		data.push_back(row);
	}

}

}
