#include <Classes/Gluco_Table_bloodpressure.h>

namespace Gluco{

//CONSTRUCTORS
Gluco_Table_bloodpressure::Gluco_Table_bloodpressure(int X, int Y, int W, int H, std::shared_ptr<Gluco_Database> db_ptr, const char *L)
	: Gluco_Table_(X,Y,W,H,db_ptr,{"Zeitpunkt", "Systole", "Diastole", "Tags", "Notizen"},L) {

	// Constructor
	std::cout << "Creating table bloodpressure" << '\n' << std::endl;

	update_data();
}

Gluco_Table_bloodpressure::~Gluco_Table_bloodpressure(){

}


void Gluco_Table_bloodpressure::select_data(){

	data = db->get_table_data_bloodpressure(sortcol, order, date1, date2, tags);

}

}


