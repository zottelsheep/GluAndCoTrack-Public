#include <Classes/Gluco_Table_glucose.h>

namespace Gluco{

//CONSTRUCTORS
Gluco_Table_glucose::Gluco_Table_glucose(int X, int Y, int W, int H, std::shared_ptr<Gluco_Database> db_ptr, const char *L)
	: Gluco_Table_(X,Y,W,H,db_ptr,{"Zeitpunkt", "Blutzucker", "Tags", "Notizen"},L) {

	// Constructor
	std::cout << "Creating table glucose" << '\n' << std::endl;

	update_data();
}

Gluco_Table_glucose::~Gluco_Table_glucose(){

}


void Gluco_Table_glucose::select_data(){

	data = db->get_table_data_glucose(sortcol, order, date1, date2, tags);

}

}



