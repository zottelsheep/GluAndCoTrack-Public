#include <Classes/Gluco_Table_preview.h>

namespace Gluco{

//CONSTRUCTORS
Gluco_Table_preview::Gluco_Table_preview(int X, int Y, int W, int H, const vector<vector<string>> & data_vec, const char *L)
	: Gluco_Table_(X,Y,W,H,NULL,data_vec[0],L) {

	// Constructor
	std::cout << "Creating table preview" << '\n' << std::endl;

	data = data_vec;

	update_data();
}

Gluco_Table_preview::~Gluco_Table_preview(){

}


void Gluco_Table_preview::select_data(){

}

}


