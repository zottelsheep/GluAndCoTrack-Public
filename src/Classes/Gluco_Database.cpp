#include <Classes/Gluco_Database.h>

namespace Gluco{

//CLASS DECLARTION

Gluco_Database::Gluco_Database(const fs::path &filepath_s, const bool &readonly)
	: filepath_v(filepath_s) , ro(readonly)
{
	init();
}

Gluco_Database::~Gluco_Database() {
	close();
}

int Gluco_Database::init(){
	// Flags for opening database
	int openFlags;

	if(ro==true){
		openFlags = SQLITE_OPEN_READONLY;

		//Debug
		cout << "Setting readonly flag" << endl;
	}
	else{
		openFlags = SQLITE_OPEN_READWRITE + SQLITE_OPEN_CREATE;

		//Debug
		cout << "Setting Read/Write Flags" << endl;
	}

	try{

			char* messageError;
			int rc;

			// Debug
			cout << "Try to open Database" << endl;

			// Opens the Database
			rc = sqlite3_open_v2(filepath_v.string().c_str(), &db, openFlags, NULL);

			// Create tables if not existing;
			rc = sqlite3_exec(db, sql_create_tables.c_str(), NULL, 0, &messageError);

			// Check for errors
			if ( rc != SQLITE_OK ){
				std::cerr << sqlite3_errmsg(db) << endl;
				sqlite3_free(messageError);
			}
			else{
				cout << "Opened Database at " << filepath_v << endl;
			}

			// Prepare statements

			if(ro==false){

			//Debug
			cout << "Preparing Statements" << endl;

			//glucose
			{
				static const string sql_input_string = "INSERT INTO glucose(timestamp, glucose, notes, tags) "
							"values"
							"(strftime('%Y-%m-%d %H:%M', @timestamp, @modifier), @glucose, @notes, @tags);";

				// Generate Statement from sql-string
				rc = sqlite3_prepare_v2(db, sql_input_string.c_str(), -1, &glucose_insert_stmt, NULL);

				//Print if error
				printiferr(rc);
			}
			//insulin
			{
				static const string sql_input_string = "INSERT INTO insulin(timestamp, insulin, notes) "
										"values"
										"(strftime('%Y-%m-%d %H:%M', @timestamp, @modifier), @insulin, @notes);";

				rc = sqlite3_prepare_v2(db, sql_input_string.c_str(), -1, &insulin_insert_stmt, NULL);
				printiferr(rc);
			}
			//food
			{
				static const string sql_input_string = "INSERT INTO food(timestamp, name, bread_unit, calories, notes) "
										"values"
										"(strftime('%Y-%m-%d %H:%M', @timestamp, @modifier), @name, @bread_unit, @calories, @notes);";

				rc = sqlite3_prepare_v2(db, sql_input_string.c_str(), -1, &food_insert_stmt, NULL);
				printiferr(rc);
			}

			//blood_pressure
			{
				static const string sql_input_string = "INSERT INTO bloodpressure(timestamp, systole, diastole, tags, notes) "
										"values"
										"(strftime('%Y-%m-%d %H:%M', @timestamp, @modifier), @systole, @diastole, @tags, @notes);";

				rc = sqlite3_prepare_v2(db, sql_input_string.c_str(), -1, &bloodpressure_insert_stmt, NULL);
				printiferr(rc);
			}

			//save_config
			{
				static string sql_input_string = "INSERT INTO config(rowid,init,name, limit_hypogly, limit_hypergly, "
						"BE_day, BE_noon, BE_evening, BE_night, "
						"day_begin,day_end,noon_begin,noon_end,evening_begin,evening_end,night_begin, night_end) "
						"values"
						"(1,1,@name, @limit_hypogly, @limit_hypergly, @BE_day, @BE_noon, @BE_evening, @BE_night, "
						"@day_begin, @day_end, @noon_begin ,@noon_end, @evening_begin, @evening_end, @night_begin, @night_end) "
						"ON CONFLICT (rowid) DO "
						"UPDATE SET name= @name, limit_hypogly= @limit_hypogly, limit_hypergly= @limit_hypergly, "
						"BE_day= @BE_day, BE_noon= @BE_noon, BE_evening= @BE_evening, BE_night= @BE_night, "
						"day_begin= @day_begin, day_end=  @day_end, noon_begin= @noon_begin, noon_end= @noon_end, "
						"evening_begin= @evening_begin, evening_end= @evening_end, night_begin= @night_begin, night_end= @night_end;";

				rc = sqlite3_prepare_v2(db, sql_input_string.c_str(), -1, &config_save_stmt, NULL);
				printiferr(rc);
			}

			//get config
			{
				static string sql_input_string = "select * from config";
				rc = sqlite3_prepare_v2(db, sql_input_string.c_str(), -1, &config_load_stmt, NULL);
				printiferr(rc);
			}

			}

		}

		catch(const std::exception & e){
			std::cerr << "Catched Error" << e.what() << endl;
			return 1;
		}

		return 0;
}

void Gluco_Database::close(){
	if(!ro){
		cout << "Finalizing Prepared Statements" << endl;

		sqlite3_finalize(glucose_insert_stmt);
		sqlite3_finalize(insulin_insert_stmt);
		sqlite3_finalize(food_insert_stmt);
		sqlite3_finalize(bloodpressure_insert_stmt);
		sqlite3_finalize(config_load_stmt);
		sqlite3_finalize(config_save_stmt);

		cout << "Closing database" << endl;
	}
	sqlite3_close(db);
}

fs::path Gluco_Database::filepath(){
	return filepath_v;
}

void Gluco_Database::filepath(const fs::path &filepath_s){
	if(filepath_s != filepath_v){
		filepath_v = filepath_s;
		close();
		init();
	}
}

bool Gluco_Database::readonly(){
	return ro;
}

void Gluco_Database::readonly(const bool &readonly){
	if(readonly != ro){
		close();
		ro = readonly;
		init();
	}
}

void Gluco_Database::reset(){
	cout << "Datenbank wird zurückgesetzt" << endl;
	close();
	fs::remove(filepath_v);
	init();
}

int Gluco_Database::restore(const fs::path & restore_path){

	cout << "Try Database restore from: '" << restore_path << "'" << endl;

	//Check if Backup is valid
	if(!is_init(restore_path)){
		cout << "Database is not init. Aborting" << endl;
		return 2;
	}

	close();

	// Make Backup of current Database
	fs::path appdata;
	appdata = getenv("APPDATA");
	appdata.append("GluAndCoTrack\\restore_backup.db");
	if(fs::exists(appdata)){
		fs::remove(appdata);
	}
	fs::copy(filepath_v,appdata);
	cout << "Made Backup of current Database" << endl;

	// Get new Database
	fs::remove(filepath_v);
	fs::copy(restore_path,filepath_v);

	//Rolling back if error
	if(init()){
		cout << "Restore not successfull, Rolling Back" << endl;

		close();
		fs::remove(filepath_v);
		fs::copy(filepath_v,restore_path);
		init();
		return 1;
	}
	else{

		cout << "Restore successfull" << endl;
		return 0;
	}


}

int Gluco_Database::into_glucose(const string &timestamp, const double &glucose, const string &notes, const string &tags){


	cout << "Attempting Input into 'glucose' ..." << endl;

	int rc;

	// Bind for @timestamp
	rc = sqlite3_bind_text(glucose_insert_stmt,
			sqlite3_bind_parameter_index(glucose_insert_stmt,"@timestamp"), // Getting Bind-Index
			timestamp.c_str(),
			-1,
			NULL);

		printiferr(rc);

	// Timestring Modifier for now
		string date_modi = "+0 hours";
		if(timestamp=="now"){
			date_modi = "+1 hours";
		}

	rc = sqlite3_bind_text(glucose_insert_stmt,
			sqlite3_bind_parameter_index(glucose_insert_stmt,"@modifier"), // Getting Bind-Index
			date_modi.c_str(),
			-1,
			NULL);

		printiferr(rc);

	rc = sqlite3_bind_double(glucose_insert_stmt,
			sqlite3_bind_parameter_index(glucose_insert_stmt,"@glucose"),
			glucose);

		printiferr(rc);

	rc = sqlite3_bind_text(glucose_insert_stmt,
			sqlite3_bind_parameter_index(glucose_insert_stmt,"@notes"),
			notes.c_str(),
			-1,
			NULL);

		printiferr(rc);

	rc = sqlite3_bind_text(glucose_insert_stmt,
			sqlite3_bind_parameter_index(glucose_insert_stmt,"@tags"),
			tags.c_str(),
			-1,
			NULL);

	printiferr(rc);

	// Evaluate statement
	rc = sqlite3_step(glucose_insert_stmt);


	//Reset Statement
	sqlite3_reset(glucose_insert_stmt);

	//Check if insert was successfull
	if(!printiferr(rc)){
		cout << "Success" << '\n' << endl;
		return 0;
	}
	else{
		return 1;
	}

}


int Gluco_Database::into_glucose(const string &timestamp, const double &glucose, const string &notes, const vector<string> &tags){
	return into_glucose(timestamp,glucose,notes,ctag(tags));
}

int Gluco_Database::into_insulin(const string &timestamp,const double &insulin, const string &notes){

	int rc;

	cout << "Attempting input into 'insulin' ..." << endl;

	//timestamp
	rc = sqlite3_bind_text(insulin_insert_stmt,
				sqlite3_bind_parameter_index(insulin_insert_stmt,"@timestamp"),
				timestamp.c_str(),
				-1,
				NULL);

		printiferr(rc);

	// Timestring Modifier for now
		string date_modi = "+0 hours";
		if(timestamp=="now"){
			date_modi = "+1 hours";
		}

	rc = sqlite3_bind_text(insulin_insert_stmt,
			sqlite3_bind_parameter_index(insulin_insert_stmt,"@modifier"), // Getting Bind-Index
			date_modi.c_str(),
			-1,
			NULL);

		printiferr(rc);

	//insulin
	rc = sqlite3_bind_double(insulin_insert_stmt,
					sqlite3_bind_parameter_index(insulin_insert_stmt,"@insulin"),
					insulin);

		printiferr(rc);

	//notes
	rc = sqlite3_bind_text(insulin_insert_stmt,
				sqlite3_bind_parameter_index(insulin_insert_stmt,"@notes"),
				notes.c_str(),
				-1,
				NULL);

		printiferr(rc);

	rc = sqlite3_step(insulin_insert_stmt);

	sqlite3_reset(insulin_insert_stmt);

	//Check if insert was successfull
	if(!printiferr(rc)){
		cout << "Success" << '\n' << endl;
		return 0;
	}
	else{
		return 1;
	}

}

int Gluco_Database::into_food(const string &timestamp, const string &name, const double &bread_unit, const double &calories, const string &notes){

	int rc;

	cout << "Attempting Input into 'food' ..." << endl;

	//timestamp
	rc = sqlite3_bind_text(food_insert_stmt,
				sqlite3_bind_parameter_index(food_insert_stmt,"@timestamp"),
				timestamp.c_str(),
				-1,
				NULL);

		printiferr(rc);

		// Timestring Modifier for now
			string date_modi = "+0 hours";
			if(timestamp=="now"){
				date_modi = "+1 hours";
			}

		rc = sqlite3_bind_text(food_insert_stmt,
				sqlite3_bind_parameter_index(food_insert_stmt,"@modifier"), // Getting Bind-Index
				date_modi.c_str(),
				-1,
				NULL);

			printiferr(rc);

	//name
	rc = sqlite3_bind_text(food_insert_stmt,
					sqlite3_bind_parameter_index(food_insert_stmt,"@name"),
					name.c_str(),
					-1,
					NULL);

		printiferr(rc);

	//bread_units
	rc = sqlite3_bind_double(food_insert_stmt,
					sqlite3_bind_parameter_index(food_insert_stmt,"@bread_unit"),
					bread_unit);

		printiferr(rc);

	//calories
	rc = sqlite3_bind_double(food_insert_stmt,
					sqlite3_bind_parameter_index(food_insert_stmt,"@calories"),
					calories);

		printiferr(rc);

	//notes
	rc = sqlite3_bind_text(food_insert_stmt,
				sqlite3_bind_parameter_index(food_insert_stmt,"@notes"),
				notes.c_str(),
				-1,
				NULL);

		printiferr(rc);


	rc = sqlite3_step(food_insert_stmt);

	sqlite3_reset(food_insert_stmt);

	//Check if insert was successfull
	if(!printiferr(rc)){
		cout << "Success" << '\n' << endl;
		return 0;
	}
	else{
		return 1;
	}

}

int Gluco_Database::into_bloodpressure(const string &timestamp, const double &systole, const double &diastole, const string &notes, const string &tags){

	int rc;

	cout << "Attempting Input into 'bloodpressure' ... " << endl;

	//timestamp
	rc = sqlite3_bind_text(bloodpressure_insert_stmt,
				sqlite3_bind_parameter_index(bloodpressure_insert_stmt,"@timestamp"),
				timestamp.c_str(),
				-1,
				NULL);

		printiferr(rc);

		// Timestring Modifier for now
			string date_modi = "+0 hours";
			if(timestamp=="now"){
				date_modi = "+1 hours";
			}

		rc = sqlite3_bind_text(bloodpressure_insert_stmt,
				sqlite3_bind_parameter_index(bloodpressure_insert_stmt,"@modifier"), // Getting Bind-Index
				date_modi.c_str(),
				-1,
				NULL);

			printiferr(rc);

	//systole
	rc = sqlite3_bind_double(bloodpressure_insert_stmt,
					sqlite3_bind_parameter_index(bloodpressure_insert_stmt,"@systole"),
					systole);

		printiferr(rc);

	//disastole
	rc = sqlite3_bind_double(bloodpressure_insert_stmt,
					sqlite3_bind_parameter_index(bloodpressure_insert_stmt,"@diastole"),
					diastole);

		printiferr(rc);

	//notes
	rc = sqlite3_bind_text(bloodpressure_insert_stmt,
				sqlite3_bind_parameter_index(bloodpressure_insert_stmt,"@notes"),
				notes.c_str(),
				-1,
				NULL);

		printiferr(rc);

	//tags
	rc = sqlite3_bind_text(bloodpressure_insert_stmt,
					sqlite3_bind_parameter_index(bloodpressure_insert_stmt,"@tags"),
					tags.c_str(),
					-1,
					NULL);

		printiferr(rc);

	rc = sqlite3_step(bloodpressure_insert_stmt);

	sqlite3_reset(bloodpressure_insert_stmt);

	//Check if insert was successfull
	if(!printiferr(rc)){
		cout << "Success" << '\n' << endl;
		return 0;
	}
	else{
		return 1;
	}

}

int Gluco_Database::into_bloodpressure(const string &timestamp, const double &systole, const double &diastole, const string &notes, const vector<string> &tags){
	return into_bloodpressure(timestamp,systole,diastole,notes,ctag(tags));
}

vector<vector<string>> Gluco_Database::get_table_data(const string &sql){

	// Output Vector-Matrix for Data
	vector<vector<string>> data;

	// Temporary vector row for pushing into data vector
	vector<string> row;

	//statement obj
	sqlite3_stmt *stmt;

	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
	printiferr(rc);

	// Retrieve next row until there are no more rows
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {

		// Insert column names into data
		if(data.size()==0){

			// Iterate over every column name
			for(int i=0;i<sqlite3_column_count(stmt);++i){
				row.push_back(sqlite3_column_name(stmt,i));
			}

			// Push back row into data vector
			data.push_back(row);
		}

		//Cleaning temp row
		row.clear();

		for(int i=0;i<sqlite3_column_count(stmt);++i){
			// pushback data from database into row
			// Return function has weird datatype: Conversion for use with string
			row.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, i)));
		}

		// Push back row into data vector
		data.push_back(row);
	}



	if (rc != SQLITE_DONE) {
		std::cerr << sqlite3_errmsg(db) << endl;
	}

	// Finalize statement (Delete Return-Object; Finished using it)
	sqlite3_finalize(stmt);

	return data;
}

vector<vector<string>> Gluco_Database::get_table_data_glucose(const string &sortcol, const string &order, const string &date1, const string &date2, const vector<string> tags){

	string sql_select = "select timestamp, glucose, tags, notes from glucose";
	return get_table_data(create_filter_sql(sql_select,sortcol,order,date1,date2,tags));
}

vector<vector<string>> Gluco_Database::get_table_data_insulin(const string &sortcol, const string &order, const string &date1, const string &date2){

	string sql_select = "select timestamp, insulin, notes from insulin";
	return get_table_data(create_filter_sql(sql_select,sortcol,order,date1,date2,{}));

}

vector<vector<string>> Gluco_Database::get_table_data_food(const string &sortcol, const string &order, const string &date1, const string &date2){

	string sql_select = "select timestamp, name, bread_unit, calories, notes from food";
	return get_table_data(create_filter_sql(sql_select,sortcol,order,date1,date2,{}));


}

vector<vector<string>> Gluco_Database::get_table_data_bloodpressure(const string &sortcol, const string &order, const string &date1, const string &date2, const vector<string> tags){
	string sql_select = "select timestamp, systole, diastole, tags, notes from bloodpressure";
	return get_table_data(create_filter_sql(sql_select,sortcol,order,date1,date2,tags));

}

vector<vector<string>> Gluco_Database::get_table_data_config(){
	string sql_select = "select * from config";
	return  get_table_data(sql_select);

}

void Gluco_Database::save_config(const Gluco_config &config){
	cout << "Saving config ..." << endl;

		int rc;

		// Bind for @name
		rc = sqlite3_bind_text(config_save_stmt,
				sqlite3_bind_parameter_index(config_save_stmt,"@name"), // Getting Bind-Index
				config.name.c_str(),
				-1,
				NULL);

		printiferr(rc);

		rc = sqlite3_bind_double(config_save_stmt,
				sqlite3_bind_parameter_index(config_save_stmt,"@limit_hypogly"),
				config.limit_hypogly);

		printiferr(rc);

		rc = sqlite3_bind_double(config_save_stmt,
				sqlite3_bind_parameter_index(config_save_stmt,"@limit_hypergly"),
				config.limit_hypergly);

		printiferr(rc);

		rc = sqlite3_bind_double(config_save_stmt,
				sqlite3_bind_parameter_index(config_save_stmt,"@BE_day"),
				config.BE_day);

		printiferr(rc);

		rc = sqlite3_bind_double(config_save_stmt,
				sqlite3_bind_parameter_index(config_save_stmt,"@BE_noon"),
				config.BE_noon);

		printiferr(rc);

		rc = sqlite3_bind_double(config_save_stmt,
				sqlite3_bind_parameter_index(config_save_stmt,"@BE_evening"),
				config.BE_evening);

		printiferr(rc);

		rc = sqlite3_bind_double(config_save_stmt,
				sqlite3_bind_parameter_index(config_save_stmt,"@BE_night"),
				config.BE_night);

		printiferr(rc);

		rc = sqlite3_bind_text(config_save_stmt,
				sqlite3_bind_parameter_index(config_save_stmt,"@day_begin"), // Getting Bind-Index
				config.day_begin.c_str(),
				-1,
				NULL);

		printiferr(rc);

		rc = sqlite3_bind_text(config_save_stmt,
				sqlite3_bind_parameter_index(config_save_stmt,"@day_end"), // Getting Bind-Index
				config.day_end.c_str(),
				-1,
				NULL);

		printiferr(rc);

		rc = sqlite3_bind_text(config_save_stmt,
				sqlite3_bind_parameter_index(config_save_stmt,"@noon_begin"), // Getting Bind-Index
				config.noon_begin.c_str(),
				-1,
				NULL);

		printiferr(rc);

		rc = sqlite3_bind_text(config_save_stmt,
				sqlite3_bind_parameter_index(config_save_stmt,"@noon_end"), // Getting Bind-Index
				config.noon_end.c_str(),
				-1,
				NULL);

		printiferr(rc);

		rc = sqlite3_bind_text(config_save_stmt,
				sqlite3_bind_parameter_index(config_save_stmt,"@evening_begin"), // Getting Bind-Index
				config.evening_begin.c_str(),
				-1,
				NULL);

		printiferr(rc);

		rc = sqlite3_bind_text(config_save_stmt,
				sqlite3_bind_parameter_index(config_save_stmt,"@evening_end"), // Getting Bind-Index
				config.evening_end.c_str(),
				-1,
				NULL);

		printiferr(rc);

		rc = sqlite3_bind_text(config_save_stmt,
				sqlite3_bind_parameter_index(config_save_stmt,"@night_begin"), // Getting Bind-Index
				config.night_begin.c_str(),
				-1,
				NULL);

		printiferr(rc);

		rc = sqlite3_bind_text(config_save_stmt,
				sqlite3_bind_parameter_index(config_save_stmt,"@night_end"), // Getting Bind-Index
				config.night_end.c_str(),
				-1,
				NULL);

		printiferr(rc);


		// Evaluate statement
		rc = sqlite3_step(config_save_stmt);

		//Debug
		if ( rc != SQLITE_DONE ){
			std::cerr << sqlite3_errmsg(db) << endl;
		}
		else {
			cout << "Success" << endl;
			cout << endl;
		}

		//Reset Statement
		sqlite3_reset(config_save_stmt);

	}

void Gluco_Database::load_config(Gluco_config &config){

	int rc;

	cout << "Get config data" << endl;
	if((rc = sqlite3_step(config_load_stmt)) == SQLITE_ROW){

	config.name = reinterpret_cast<const char*>(sqlite3_column_text(config_load_stmt, 0));
	config.limit_hypogly = sqlite3_column_double(config_load_stmt,1);
	config.limit_hypergly = sqlite3_column_double(config_load_stmt,2);
	config.BE_day = sqlite3_column_double(config_load_stmt,3);
	config.BE_noon = sqlite3_column_double(config_load_stmt,4);
	config.BE_evening = sqlite3_column_double(config_load_stmt,5);
	config.BE_night = sqlite3_column_double(config_load_stmt,6);
	config.day_begin = reinterpret_cast<const char*>(sqlite3_column_text(config_load_stmt, 7));
	config.day_end = reinterpret_cast<const char*>(sqlite3_column_text(config_load_stmt, 8));
	config.noon_begin = reinterpret_cast<const char*>(sqlite3_column_text(config_load_stmt, 9));
	config.noon_end = reinterpret_cast<const char*>(sqlite3_column_text(config_load_stmt, 10));
	config.evening_begin = reinterpret_cast<const char*>(sqlite3_column_text(config_load_stmt, 11));
	config.evening_end = reinterpret_cast<const char*>(sqlite3_column_text(config_load_stmt, 12));
	config.night_begin = reinterpret_cast<const char*>(sqlite3_column_text(config_load_stmt, 13));
	config.night_end = reinterpret_cast<const char*>(sqlite3_column_text(config_load_stmt, 14));

	cout << "config loaded" << endl;

	}
	else{
		cout << "config emtpy" << endl;
	}

	//Debug
	printiferr(rc);

	sqlite3_reset(config_load_stmt);

}

string Gluco_Database::export_tsv(const string &table, const string &date1, const string &date2){

	string tsv{};
	string sql = "SELECT * from " + table;

	if((!date1.empty() || !date2.empty()) && table != "config"){
		sql = create_filter_sql(sql,"timestamp","desc",date1,date2);
	}

	sqlite3_stmt *stmt;

	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
	printiferr(rc);

	// First step for column names
	rc = sqlite3_step(stmt);
	printiferr(rc);

	for(int i=1;i<sqlite3_column_count(stmt);++i){
		tsv.append(sqlite3_column_name(stmt,i));
		tsv.append("\t");
	}
	tsv.pop_back(); // Remove last tab

	tsv.append("\n");

	for(int i=1;i<sqlite3_column_count(stmt);++i){
		tsv.append(reinterpret_cast<const char*>(sqlite3_column_text(stmt, i)));
		tsv.append("\t");
	}

	tsv.pop_back(); // Remove last tab

	tsv.append("\n");


	// Rest of the steps / rows
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW){

		for(int i=1;i<sqlite3_column_count(stmt);++i){
			tsv.append(reinterpret_cast<const char*>(sqlite3_column_text(stmt, i)));
			tsv.append("\t");
		}

		tsv.pop_back(); // Remove last tab

		tsv.append("\n");
	}

	if (rc != SQLITE_DONE) {
		std::cerr << sqlite3_errmsg(db) << endl;
	}
	else {
		cout << "Export tsv-string from " << table << " successful" << endl;
	}

	sqlite3_finalize(stmt);


	return tsv;
}

void Gluco_Database::export_db(const fs::path &filepath,const vector<string> & tables,const string &date1,const string &date2){

	int rc;
	string sql_string = "ATTACH DATABASE '" + filepath.string() + "' AS export;";

	cout << "Beginning export of database\n" << sql_string << endl;
	rc = sqlite3_exec(db, sql_string.c_str(), NULL, 0, NULL);
	printiferr(rc);

	sql_string = "BEGIN TRANSACTION;";
	cout << sql_string << endl;
	rc = sqlite3_exec(db, sql_string.c_str(), NULL, 0, NULL);


	for(auto table : tables){
		cout << "Export Table '" << table << "'" << endl;
		sql_string = "create table export." + table + " as select * from main." + table;

		if(table != "config"){
			sql_string = create_filter_sql(sql_string,"timestamp","desc",date1,date2);
		}

		rc = sqlite3_exec(db, sql_string.c_str(), NULL, 0, NULL);
		printiferr(rc);
	}

	sql_string = "create table if not exists export.config as select init from main.config; UPDATE OR ROLLBACK export.config SET init = 2;";
	rc = sqlite3_exec(db, sql_string.c_str(), NULL, 0, NULL);
	printiferr(rc);

	cout << "END TRANSACTION" << '\n' << "Save at: " << filepath << endl;
	sql_string = "END TRANSACTION; DETACH DATABASE export;";
	rc = sqlite3_exec(db, sql_string.c_str(), NULL, 0, NULL);
	printiferr(rc);

}

int Gluco_Database::import_vector_data(vector<vector<string>> & insertData){

	static const vector<string> glucose_test{"timestamp","glucose","tags","notes"};
	static const vector<string> insulin_test{"timestamp","insulin","notes"};
	static const vector<string> food_test{"timestamp","name","bread_unit","calories","notes"};
	static const vector<string> bloodpressure_test{"timestamp","systole","diastole","tags","notes"};

	//Error int
	int rc;

	vector<string> colnames = insertData.front();
	insertData.erase(insertData.begin());


	if(colnames==insulin_test){

		beginTrans();

		cout << "Database: Begin Mass-Insert into insulin" << endl;
		for(auto row : insertData){
			rc = into_insulin(row[0],std::strtod(row[1].c_str(),NULL),row[2]);
			if(rc){
				rollback();
				break;
			}
		}

		return endTrans();


	}
	else if(colnames==glucose_test){

		beginTrans();

		cout << "Database: Begin Mass-Insert into glucose" << endl;

		for(auto row : insertData){
			rc = into_glucose(row[0],std::strtod(row[1].c_str(),NULL),row[3],row[2]);
			if(rc){
				rollback();
				break;
			}
		}

		return endTrans();
	}
	else if(colnames==food_test){

		beginTrans();

		cout << "Database: Begin Mass-Insert into food" << endl;

		for(auto row : insertData){
			rc = into_food(row[0],row[1],std::strtod(row[2].c_str(),NULL),std::strtod(row[3].c_str(),NULL),row[4]);
			if(rc){
				rollback();
				break;
			}
		}

		return endTrans();
		}
	else if(colnames==bloodpressure_test){

		beginTrans();

		cout << "Database: Begin Mass-Insert into bloodpressure" << endl;

		for(auto row : insertData){
			rc = into_bloodpressure(row[0],std::strtod(row[1].c_str(),NULL),std::strtod(row[2].c_str(),NULL),row[4],row[3]);
			if(rc){
				rollback();
				break;
			}
		}

		return endTrans();
	}
	else{
		std::cerr << "Database: Error Mass-Insert: Wrong Format" << endl;
		return 0x10;
	}



}

string Gluco_Database::ctag(const vector<string> &tag){
	string tmp;
	for(auto n : tag){
		// tmp += n + ", ";
		tmp.append(n);
		tmp.append(",");
	}
	tmp.pop_back();
	return tmp;
}

string Gluco_Database::create_filter_sql(string &sql_string, const string &sortcol, const string &order, const string &date1, const string &date2, const vector<string> tags){

	sql_string.append(" ");

	if(!date1.empty() || !date2.empty() || !tags.size()==0){

		sql_string.append("WHERE ");

		if(!date1.empty() and !date2.empty()){
			sql_string.append("timestamp between '" + date1 + "' AND '" + date2 + "' ");
		}
		else if(date1.empty() and !date2.empty()){
			sql_string.append("timestamp <= '" + date2 + "' ");
		}
		else if(!date1.empty() and date2.empty()){
			sql_string.append("timestamp >= '" + date1 + "' ");
		}
		if (!tags.size()==0){
			sql_string.append(" tags LIKE '%" + tags[1] + "%' ");
			for(unsigned int i=2;i<=tags.size();++i){
				sql_string.append("OR tags LIKE '%" + tags[i] + "%' ");
			}
		}
	}

	sql_string.append("ORDER BY " + sortcol + " " + order + ";");

	//Debug
	cout << sql_string << endl;

	return sql_string;
}

int Gluco_Database::beginTrans(){
	cout << "Database: Beginning Transaction." << endl;
	int rc = sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, 0, NULL);
	return printiferr(rc);
}

int Gluco_Database::endTrans(){
	cout << "Database: Ending Transaction." << endl;
	int rc = sqlite3_exec(db, "END TRANSACTION;", NULL, 0, NULL);
	return printiferr(rc);
}

int Gluco_Database::rollback(){
	cout << "Database: Error in Transaction, rolling back." << endl;
	int rc = sqlite3_exec(db, "ROLLBACK;", NULL, 0, NULL);
	return printiferr(rc);
}
// DEBUG FUNCTIONS

void Gluco_Database::eval_sql(const string &sql){
	try{
			sqlite3_stmt *stmt;

			int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
			if ( rc != SQLITE_OK ){
				std::cerr << sqlite3_errmsg(db) << endl;
			}
			while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
				for(int i=0;i<sqlite3_column_count(stmt);++i){
					cout << sqlite3_column_name(stmt,i) << ": " << sqlite3_column_text(stmt, i) << endl;
				}
				cout << endl;

			}

			if (rc != SQLITE_DONE) {
				std::cerr << sqlite3_errmsg(db) << endl;
			}
			else {
				cout << "SQL: " << sql << " EXEC: OK" << endl << endl;
			}

			sqlite3_finalize(stmt);
		}
		catch(const std::exception & e){
			std::cerr << e.what();
		}
}

void Gluco_Database::print_table(const string &table){
	cout << "Printing Data from table " << table << ":\n" << endl;
	string sql_select_data = "SELECT * from " + table +" ;";

	eval_sql(sql_select_data);

}

void Gluco_Database::interactive(){
	string statement;
	cout << "-----------------------------------------------" << endl;
	cout << "Interactive SQLite-Interface					" << endl;
	cout << "-----------------------------------------------" << endl;
	while(true){
		cout << "-----------------------------------------------" << endl;
		cout << "SQL: " << std::flush;
		std::getline(std::cin,statement);
		if(statement=="exit") break;
		eval_sql(statement);
	}
}

int Gluco_Database::printiferr(const int &rc){
	if ( rc != SQLITE_OK && rc != SQLITE_ROW && rc != SQLITE_DONE ){
			std::cerr << sqlite3_errmsg(db) << endl;
			return 1;
		}
	return 0;
}

const string Gluco_Database::sql_create_tables =
						"CREATE TABLE IF NOT EXISTS glucose ("
						"TrackNr INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
						"timestamp date NOT NULL,"
						"glucose REAL NOT NULL,"
						"tags TEXT,"
						"notes TEXT);"

						"CREATE TABLE IF NOT EXISTS insulin ("
						"TrackNr INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
						"timestamp date NOT NULL,"
						"insulin REAL NOT NULL,"
						"notes TEXT);"

						"CREATE TABLE IF NOT EXISTS food ("
						"TrackNr INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
						"timestamp date NOT NULL,"
						"name TEXT,"
						"bread_unit REAL NOT NULL,"
						"calories REAL,"
						"notes TEXT);"

						"CREATE TABLE IF NOT EXISTS bloodpressure ("
						"TrackNr INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
						"timestamp date NOT NULL,"
						"systole REAL NOT NULL,"
						"diastole REAL NOT NULL,"
						"tags TEXT,"
						"notes TEXT);"

						"CREATE TABLE IF NOT EXISTS config("
						"name TEXT NOT NULL,"
						"limit_hypogly REAL NOT NULL,"
						"limit_hypergly REAL NOT NULL,"
						"BE_day REAL NOT NULL,"
						"BE_noon REAL NOT NULL,"
						"BE_evening REAL NOT NULL,"
						"BE_night REAL NOT NULL,"
						"day_begin date NOT NULL,"
						"day_end date NOT NULL,"
						"noon_begin date NOT NULL,"
						"noon_end date NOT NULL,"
						"evening_begin date NOT NULL,"
						"evening_end date NOT NULL,"
						"night_begin date NOT NULL,"
						"night_end date NOT NULL,"
						"init INTEGER NOT NULL);";

// FUNCTION DECLARATIONS

bool checktimestring(string &timestring,const bool &datestring){
	static const std::regex p_datetime{R"(^(\d\d\d\d)\-(0[1-9]|1[012])\-(0[1-9]|([12]\d)|(3[01])) ([0-1]\d|2[0-3]):([0-5]\d)$)"};
	static const std::regex p_date{R"(^(\d\d\d\d)\-(0[1-9]|1[012])\-(0[1-9]|([12]\d)|(3[01]))$)"};

	if(timestring=="jetzt" || timestring == "now"){
		timestring="now";
		return true;
	}
	else if(regex_match(timestring,p_datetime)){
		return true;
	}
	else if(datestring && regex_match(timestring,p_date)){
		return true;
	}
	else{
		return false;
	}

}

int is_init(const fs::path & filepath){
	int rc;
	int init_state=0;

	// Debug
	cout << "Check if Database is initilized" << endl;

	// Opens the Database in Readonly
	sqlite3* db;
	rc = sqlite3_open_v2(filepath.string().c_str(), &db, SQLITE_OPEN_READONLY, NULL);
	if ( rc != SQLITE_OK ){
		std::cerr << sqlite3_errmsg(db) << endl;
		return false;
	}
	// create stmt
	sqlite3_stmt *init_stmt;
	rc = sqlite3_prepare_v2(db, "select init from config", -1, &init_stmt, NULL);
	if ( rc != SQLITE_OK ){
		std::cerr << sqlite3_errmsg(db) << endl;
		return 0;
	}

	if((rc = sqlite3_step(init_stmt)) == SQLITE_ROW){
		init_state = sqlite3_column_int(init_stmt,0);
	}
	rc = sqlite3_finalize(init_stmt);

	// Check for errors
	if ( rc != SQLITE_OK ){
		std::cerr << sqlite3_errmsg(db) << endl << std::flush;
		return 0;
	}

	if(init_state & 0b01){
		cout << "Database at " << filepath << " is initilized"<< endl;
	}
	else if(init_state & 0b10){
		cout << "Database at " << filepath << " is read-only"<< endl;
	}

	sqlite3_close(db);

	return init_state;
}

} /* namespace Gluco */
