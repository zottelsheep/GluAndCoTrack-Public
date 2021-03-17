#include <Gluco_FileManipulation.h>

namespace Gluco{

fs::path getlastDatabase(){

	fs::path cwd = fs::current_path();
	cwd.append("Debug\\last_db.txt");

	fs::path appdata;
	appdata = getenv("APPDATA");
	appdata.append("GluAndCoTrack\\last_db.txt");

	string last_db_path;
	if(fs::exists(cwd)){
		last_db_path = cwd.string();
	}
	else if(fs::exists(appdata)){
		last_db_path = appdata.string();
	}
	else{
		cout << "Found no last_db.txt" << endl;
		return "";
	}

	cout << "Found last_db.tx under: " << last_db_path << endl;

	std::ifstream last_db;
	last_db.open(last_db_path);
	if(last_db.good()){
		string filepath;
		std::getline(last_db,filepath);
		last_db.close();
		filepath.pop_back();
		cout << "Database Path: " << filepath << endl;
		return fs::absolute(filepath);
	}
	else{
		cout << "Failed to open last_db.txt" << endl;
		return "";
	}

}

void save_lastdb(fs::path filepath){

	fs::path last_db_path;

	//Check where to save last_db.txt to
	if(fs::exists("./Debug/last_db.txt")){
		last_db_path = "./Debug/last_db.txt" ;
	}
	else{

		//Get Appdata directory
		fs::path appdata;
		appdata = getenv("APPDATA");
		appdata.append("GluAndCoTrack");

		//Create directories if not exist
		if(!fs::exists(appdata)){
			fs::create_directories(appdata);
		}

		appdata.append("last_db.txt");
		last_db_path=appdata;

	}

	cout << "last_db was saved under: " << last_db_path << endl;

	//save last_db.txt
	std::ofstream last_db;
	last_db.open(last_db_path.string(),std::ios::trunc);
	if(last_db.good()){

		last_db << filepath.string() << ';' << endl;
		last_db.close();
	}
}

vector<vector<string>> get_data_from_tsv(fs::path filepath){

	cout << "Get data from tsv at: '" << filepath << "'" << endl;
	std::ifstream tsv(filepath.string());
	vector<vector<string>> table_tsv{};
	vector<string> row{};
	string row_s,item;
	if(tsv.good()){
		cout << "load file" << endl;

		// Get First Row and get number of columns
		getline(tsv,row_s,'\n');
		std::stringstream row_stream(row_s);
		while(getline(row_stream,item,'\t')){
			if(item.empty()){
				std::cerr << "ERROR in FileManipulation: get_data_from_tsv: Column names incomplete!" << endl;
				return{};
			}
			row.push_back(item);
		}
		int n_col = row.size();
		table_tsv.push_back(row);
		row.clear();

		while(getline(tsv,row_s,'\n')){
			cout << row_s << endl;
			std::stringstream row_stream(row_s);
			for(int i=0; i<n_col;++i){
				std::getline(row_stream,item,'\t');
				if(item.empty()){
					row.push_back("");
				}
				else{
					row.push_back(item);
				}
			}

			table_tsv.push_back(row);
			row.clear();

		}
	}
	else{
		return {};
	}

	cout << "Done!" << endl;

	return table_tsv;
}
}
