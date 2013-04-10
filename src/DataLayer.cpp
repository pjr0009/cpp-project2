#include "headers/DataLayer.h"

DataLayer::DataLayer(){


	conn = PQconnectdb("dbname="   DB_NAME
		  	  	  	" host="     DB_HOST
		  	  	  	" user="     DB_USER
		  	  	  	" password=" DB_PW
		  	  	  	);
  if (PQstatus(conn) == CONNECTION_BAD) {
    puts("We were unable to connect to the database");
    exit(0);
  }

}

bool DataLayer::authenticate_staff(string user_name, string password, map<string, string>& session_info){
	std::string query = "SELECT * FROM staff WHERE staff.user_name='" + user_name + "' AND staff.password='" + password + "';";

	PGresult* res = PQexec(conn, query.c_str());
	if(PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) == 0){
	  return false;
	}
	else if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) == 1) {
		session_info["id"]   = PQgetvalue(res, 0, 0);
		session_info["name"] = PQgetvalue(res, 0, 1);
		session_info["admin_level"] = PQgetvalue(res, 0, 4);
		return true;

	}
	return false;

}

bool DataLayer::persist(string table, map<string, string> orm){
	string query = "INSERT INTO " + table;
    string values = "VALUES( ";
	string fields = " ( ";
	map<string, string>::iterator iter;
	unsigned count = 0;
    for (iter = orm.begin(); iter != orm.end(); ++iter, ++count) {
    	if(count < orm.size() - 1){
        fields += iter->first + ", ";
    	values += "'" + iter->second + "', ";
    	}else{
        	fields += iter->first + ") ";
    		values += "'" + iter->second + "' );";
    	}
    }
    query += fields + values;
	PGresult* res = PQexec(conn, query.c_str());
	if(PQresultStatus(res) == PGRES_COMMAND_OK){
	  return true;
	} else {
		return false;
	}
}
bool DataLayer::update(string table, map<string, string> orm){
	string query = "UPDATE " + table + " SET ";
    string values;
	map<string, string>::iterator iter;
	unsigned count = 0;
    for (iter = orm.begin(); iter != orm.end(); ++iter, ++count) {
    	if(count < orm.size() - 1){
    		values += iter-> first + " = '" + iter->second + "', ";	
    	} else{
    		values += iter-> first + " = '" + iter->second + "' ";	

    	}
    }
    query += values + "WHERE id = " + orm["id"] + ";";
   	PGresult* res = PQexec(conn, query.c_str());
	if(PQresultStatus(res) == PGRES_COMMAND_OK){
	  return true;
	} else {
		return false;
	}
}


bool DataLayer::find(string table, string search_by_attr, string attr_value, vector<map<string, string> >& search_results, string joins){
	string query;
	search_results.clear(); // reset the results
	if(joins == "false"){
		query = "SELECT * FROM " + table + " WHERE " + search_by_attr + " ~* '" + attr_value + "';";
	} else {
		// making this less abstracted than originally planned, since I only use it for one purpose
		query = "SELECT * FROM " + table + " INNER JOIN " + joins + " ON (accounts.client_id = clients.id) WHERE " + search_by_attr + " ~* '" + attr_value + "';";
	}
	PGresult* res = PQexec(conn, query.c_str());
	if(PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0){

		for(int i = 0; i < PQntuples(res); i++){
			map<string, string> row;
			for(int j = 0; j < PQnfields(res); j++){
			  row[PQfname(res, j)] = PQgetvalue(res, i, j);
		  }
			search_results.push_back(row);
			row.clear();

		}
		return true;
	}
	return false;
}

bool DataLayer::delete_entity(string table, string id){
	string query = "DELETE FROM " + table + " WHERE id=" + id;
   	PGresult* res = PQexec(conn, query.c_str());
	if(PQresultStatus(res) == PGRES_COMMAND_OK){
	  return true;
	} else {
		return false;
	}
}


