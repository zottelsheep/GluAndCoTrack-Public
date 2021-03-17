#include <Classes/Gluco_Table_.h>

namespace Gluco{

Gluco_Table_::Gluco_Table_(int X, int Y, int W, int H, std::shared_ptr<Gluco_Database> db_ptr, vector<string> columnames, const char *L) : Fl_Table(X,Y,W,H,L){
	colnames = columnames;

	db = db_ptr;


	// Cols
	cols(colnames.size());
	col_header(1);              // enable column headers (along top)
	col_width_all(W/cols()-1);   // default width of columns
	col_resize(1);              // enable column resizing


	// Rows
	rows(0);
	row_header(0);              // disable row headers (along left)
	row_height_all(20);         // default height of rows
	row_resize(1);              // enable row resizing

	color(FL_WHITE);			// sets default colour to white

	end();						// end the Fl_Table group
};

//Gluco_Table_::~Gluco_Table_(){};

void Gluco_Table_::DrawHeader(const char *s, int X, int Y, int W, int H) {
	fl_push_clip(X,Y,W,H);
	  fl_draw_box(FL_THIN_UP_BOX, X,Y,W,H, color());
	  fl_color(FL_BLACK);
	  fl_draw(s, X,Y,W,H, FL_ALIGN_CENTER);
	fl_pop_clip();
};

void Gluco_Table_::DrawData(const char *s, int X, int Y, int W, int H) {
    fl_push_clip(X,Y,W,H);
      // Draw cell bg
      fl_color(FL_WHITE); fl_rectf(X,Y,W,H);
      // Draw cell data
      fl_color(FL_GRAY0); fl_draw(s, X,Y,W,H, FL_ALIGN_CENTER);
      // Draw box border
      fl_color(color()); fl_rect(X,Y,W,H);
    fl_pop_clip();
}

void Gluco_Table_::draw_cell(TableContext context, int ROW, int COL, int X, int Y, int W, int H) {
    switch ( context ) {
      case CONTEXT_STARTPAGE:                   // before page is drawn..
        fl_font(FL_HELVETICA, 16);              // set the font for our drawing operations
        return;
      case CONTEXT_COL_HEADER:                  // Draw column headers
        DrawHeader(colnames[COL].c_str(),X,Y,W,H);
        return;
      case CONTEXT_ROW_HEADER:                  // Draw row headers
        DrawHeader(data[ROW+1][0].c_str(),X,Y,W,H);
        return;

      case CONTEXT_CELL:                        // Draw data in cells
        DrawData(data[ROW+1][COL].c_str(),X,Y,W,H);
        return;
      default:
        return;
    }
}

void Gluco_Table_::applyfilter(const string &date1, const string &date2, const vector<string> &tags){
	Gluco_Table_::date1 = date1;
	Gluco_Table_::date2 = date2;
	Gluco_Table_::tags = tags;

	update_data();
}

void Gluco_Table_::update_data(){

	select_data();

	// Set how many rows
	if(!data.empty()){
		std::cout << "Updating table" << '\n' << std::endl;
		rows(data.size()-1); //-1 because of column names
		std::cout << "Updating table rows. N= " << data.size() << '\n' << std::endl;
	}
	else{
		rows(0);
	}
};

}


