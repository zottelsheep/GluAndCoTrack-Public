#include <Classes/Gluco_Table_food.h>

namespace Gluco{

//CONSTRUCTORS
Gluco_Table_food::Gluco_Table_food(int X, int Y, int W, int H, std::shared_ptr<Gluco_Database> db_ptr, const char *L)
	: Gluco_Table_(X,Y,W,H,db_ptr,{"Zeitpunkt", "Name", "Broteinheiten", "Kalorien", "Notizen"},L) {

	// Constructor
	std::cout << "Creating table food" << '\n' << std::endl;

	update_data();
}

Gluco_Table_food::~Gluco_Table_food(){

}


void Gluco_Table_food::select_data(){

	data = db->get_table_data_food(sortcol, order, date1, date2);

}

}


