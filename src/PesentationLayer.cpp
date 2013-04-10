/*
 * PesentationLayer.cpp
 *
 *  Created on: Mar 24, 2013
 *      Author: phillip
 */
#include "headers/PresentationLayer.h"
#include "headers/DataLayer.h"
#include "headers/Staff.h"
#include "headers/SessionController.h"


void PresentationLayer::loading(){
	cout << "Loading...";
}
void PresentationLayer::complete(){
	cout << "Complete!" << endl;
}
void PresentationLayer::login_header(){
	cout << "==========================================================================" << endl;
	cout << "|                     Terminal Teller System Login                       |" << endl;
    cout << "=========================================================================" << endl;

}

void PresentationLayer::welcome_back(){
	cout << "Login Successful...";
}

void PresentationLayer::sub_nav_header(string page){
	cout << "=========================================================================" << endl;
	cout << "|       Terminal Teller System - " << page <<"     |" << endl;
    cout << "==========================================================================" << endl;
}

void PresentationLayer::navigation(SessionController& session, string page){
	string staff_title = session.current_user.admin_level ? "System Adminstration" : "Branch Staff";
	cout << endl << endl << endl << endl;
	cout << "=========================================================================" << endl;
	cout << "|       Terminal Teller System - " << session.current_selection <<"     |" << endl;
    cout << "==========================================================================" << endl;
	int selection;
	session.generate_navigation();
	map<int, string>::iterator iter;
	for(iter = session.navigation.begin(); iter != session.navigation.end(); iter++){
		cout << iter -> first << ") ";
		cout << iter -> second << endl;

	}
	cout << "Please choose an option: ";
	cin >> selection;
    session.current_selection = session.navigation[selection];
}


void PresentationLayer::search_results(vector<map <string, string> > results, string type){
	vector<string> header;
	vector<string> attr;
	if(type == "client"){
		header.push_back("Client ID");
		attr.push_back("id");
		header.push_back("Name");
		attr.push_back("name");
		header.push_back("Address");
		attr.push_back("address");

	}else if (type == "account"){
		header.push_back("Account ID");
		attr.push_back("id");
		header.push_back("Type");
		attr.push_back("type");
		header.push_back("Account Holder");
		attr.push_back("name");

	} else if (type == "staff"){
		header.push_back("Staff ID");
		attr.push_back("id");
		header.push_back("User Name");
		attr.push_back("user_name");
		header.push_back("Is Admin? (1/0)");
		attr.push_back("admin_level");
	}

	stringstream s;

	cout << "\n========================================================================="<< endl;
	cout << "|                            Search Results                             |" << endl;
	cout << "========================================================================="<< endl;
	
	cout << setw(15) << left << header.at(0) << setw(25) << left << header.at(1) << header.at(2) << endl;
	cout << "-------------------------------------------------------------------------" << endl;
	for (unsigned i = 0; i < results.size(); ++i){
	map <string, string> current_result = results.at(i);

	s << setw(15) << left << current_result[attr.at(0)] << setw(25) << left << current_result[attr.at(1)] << current_result[attr.at(2)] << endl;
	cout << s.str();
	s.str("");
	}

}



