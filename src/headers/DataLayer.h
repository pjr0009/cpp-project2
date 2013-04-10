/*
 * DataLayer.h
 *
 *  Created on: Mar 23, 2013
 *      Author: phillip
 */

#pragma once
#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>
#include "config.h"
#include <libpq-fe.h>




using namespace std;

class DataLayer {
public:
	DataLayer();
	bool find(string table, string search_by_attr, string attr_value, vector<map<string, string> >& search_results, string joins);
	bool persist(string table, map<string, string> orm);
	bool update(string table, map<string, string> orm);
	bool authenticate_staff(string user_name, string password, map<string, string>& session_info);
	bool delete_entity(string table, string id);
private:
    PGconn *conn;

};



