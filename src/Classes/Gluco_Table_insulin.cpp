#include <Classes/Gluco_Table_insulin.h>

namespace Gluco{

//CONSTRUCTORS
Gluco_Table_insulin::Gluco_Table_insulin(int X, int Y, int W, int H, std::shared_ptr<Gluco_Database> db_ptr, const char *L) : Gluco_Table_(X,Y,W,H,db_ptr,{"Zeitpunkt", "Insulinmenge", "Notizen"},L) {
	// Constructor
	std::cout << "Creating table insulin" << '\n' << std::endl;

	update_data();
}

Gluco_Table_insulin::~Gluco_Table_insulin(){

}



void Gluco_Table_insulin::select_data(){

	data = db->get_table_data_insulin(sortcol, order, date1, date2);

}

}


