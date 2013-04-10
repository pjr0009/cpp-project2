#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>
#include "Staff.h"
#include "PresentationLayer.h"
#include "DataLayer.h"

/*
 * SessionController.h
 *
 *  Created on: Mar 23, 2013
 *      Author: phillip
 */

using namespace std;

class SessionController {
public:
	SessionController();
	DataLayer db;
	Staff current_user;
	map<int,string> navigation;
	string current_selection;
	PresentationLayer present;
	void generate_navigation();
	bool add_branch_staff();
	bool delete_branch_staff();
	bool branch_staff_db();
	void login();
	bool add_client();
	bool edit_client();
	bool add_account();
	bool change_password();
	bool manage_account();

};


