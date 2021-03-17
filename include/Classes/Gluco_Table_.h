#ifndef SRC_CLASSES_GLUCO_TABLE__H_
#define SRC_CLASSES_GLUCO_TABLE__H_

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Table.H>
#include <FL/fl_draw.H>
#include <iostream>
#include <memory>
#include "Gluco_Database.h"

namespace Gluco {
class Gluco_Table_ : public Fl_Table {
private:

	void DrawHeader(const char *s, int X, int Y, int W, int H);
	void DrawData(const char *s, int X, int Y, int W, int H);
	void draw_cell(TableContext context, int ROW=0, int COL=0, int X=0, int Y=0, int W=0, int H=0);
	virtual void select_data() = 0;

protected:

	std::vector<std::vector<std::string>> data;
	vector<string> colnames{};
	std::shared_ptr<Gluco_Database> db;
	string sortcol = "timestamp";
	string order = "desc";
	string date1 = "", date2 = "";
	vector<string> tags{};



public:
	Gluco_Table_(int X, int Y, int W, int H, std::shared_ptr<Gluco_Database> db_ptr, vector<string> columnames, const char *L=0);
	void applyfilter(const string &date1, const string &date2, const vector<string> &tags={});
	void update_data();
	virtual ~Gluco_Table_() {};
	};
}
#endif /* SRC_CLASSES_GLUCO_TABLE__H_ */
