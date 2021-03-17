#ifndef SRC_GLUCO_DATABASE_H_
#define SRC_GLUCO_DATABASE_H_

// Includes

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <filesystem>
#include <fstream>
#include <sqlite3.h>

using std::string;
using std::vector;
using std::cout;
using std::endl;

namespace fs = std::filesystem;


namespace Gluco{


struct Gluco_config{
	string name="";
	double limit_hypogly=0;
	double limit_hypergly=0;
	double BE_day=0;
	double BE_noon=0;
	double BE_evening=0;
	double BE_night=0;
	string day_begin="";
	string day_end="";
	string noon_begin="";
	string noon_end="";
	string evening_begin="";
	string evening_end="";
	string night_begin="";
	string night_end="";
};

bool checktimestring(string &timestring,const bool &datestring = false);

//Returns state of database at given path. Can be either 0 (not init), 1 (init) or 2 (ro / export)
int is_init(const fs::path & filepath);


class Gluco_Database {
public:

	// Constructor: Opens Database of a given path and if desired in Readonly-Mode (Default is false)
	Gluco_Database(const fs::path &filepath_s, const bool &readonly = false);

	// Deconstructor
	virtual ~Gluco_Database();

	// Gets filepath of sqlite-database
	fs::path filepath();

	// Sets filepath of sqlite-database and reopens Database
	void filepath(const fs::path &filepath_s);

	// Returns if Database is in ReadOnly-Mode
	bool readonly();

	// Change ReadOnly-Mode
	void readonly(const bool &readonly);

	// resets database
	void reset();

	// restores database form backup
	int restore(const fs::path & restore_path);

	// Insert Functions:

	// Inserts Data into row of table glucose
	int into_glucose(const string &timestamp, const double &glucose, const string &notes, const vector<string> &tags={});
	int into_glucose(const string &timestamp, const double &glucose, const string &notes, const string & tags);

	// Inserts Data into row of table insulin
	int into_insulin(const string &timestamp,const double &insulin, const string &notes);

	// Inserts Data into row of table food
	int into_food(const string &timestamp, const string &name, const double &bread_unit, const double &calories, const string &notes);

	// Inserts Data into row of table bloodpressure
	int into_bloodpressure(const string &timestamp, const double &systole, const double &diastole, const string &notes, const vector<string> &tags={});
	int into_bloodpressure(const string &timestamp, const double &systole, const double &diastole, const string &notes, const string &tags);



	// Functions for FL_Table use

	// Gets Data from table "glucose" for use in FL_Table
	vector<vector<string>> get_table_data_glucose(	const string &sortcol="timestamp",
													const string &order="desc",
													const string &date1="",
													const string &date2="",
													const vector<string> tags={}
													);

	// Gets Data from table "insulin" for use in FL_Table
	vector<vector<string>> get_table_data_insulin(	const string &sortcol="timestamp",
													const string &order="desc",
													const string &date1="",
													const string &date2=""
													);

	// Gets Data from table "food" for use in FL_Table
	vector<vector<string>> get_table_data_food(	const string &sortcol = "timestamp",
												const string &order = "desc",
												const string &date1 = "",
												const string &date2 = ""
												);

	// Gets Data from table "bloodpressure" for use in FL_Table
	vector<vector<string>> get_table_data_bloodpressure(const string &sortcol="timestamp",
														const string &order="desc",
														const string &date1="",
														const string &date2="",
														const vector<string> tags={}
														);

	// Gets Data from table "config" for use in FL_Table
	vector<vector<string>> get_table_data_config();

	// Config

	void save_config(const Gluco_config &config);
	void load_config(Gluco_config &config);

	// Export, Import Funktions


	// Export tsv
	string export_tsv(const string &table, const string &date1 ="", const string &date2 ="");

	// Export Database as .dbback file
	void export_db(const fs::path &filepath, const vector<string> & tables,const string &date1,const string &date2);

	// Import complete vector as atomic insert. Checks for validity of insertable Data via the first row of vector.
	// Returns 0 on success, 1 on fail, and 2 on false format of Data
	int import_vector_data(vector<vector<string>> & insertData);


	// Debug Functions

	// Prints Data to stout for Debug
	void print_table(const string &table);

	// Evaluates a given SQL-Statement and prints Output
	void eval_sql(const string &sql);

	// Interactive SQLite-Interface with output to stout
	void interactive();

private:
	// Internal Copy of filepath
	fs::path filepath_v;

	//SQL-Strings

	static const string sql_create_tables;

	// Database Object Pointer
	sqlite3 *db;

	// Prepared Statement Objects
	sqlite3_stmt *glucose_insert_stmt;
	sqlite3_stmt *insulin_insert_stmt;
	sqlite3_stmt *food_insert_stmt;
	sqlite3_stmt *bloodpressure_insert_stmt;
	sqlite3_stmt *config_save_stmt;
	sqlite3_stmt *config_load_stmt;

	//Readonly?
	bool ro;

	// Initialize SQLite-Database
	int init();

	// Evaluates a given SQL-Statement and outputs data for use in tables of FLTK
	vector<vector<string>> get_table_data(const string &sql);

	string create_filter_sql(string &sql_string, const string &sortcol, const string &order, const string &date1 ="", const string &date2 ="", const vector<string> tags ={});

	//Converts tag vector to single tag-string
	string ctag(const vector<string> &tag);

	//Begins Transaction
	int beginTrans();

	//Ends Transaction
	int endTrans();

	//Rollsback Transaction;
	int rollback();

	//Prints error code if not SQL_OK
	int printiferr(const int &rc);

	//Closing Database
	void close();

};
} /* namespace Gluco */
#endif /* SRC_GLUCO_DATABASE_H_ */
