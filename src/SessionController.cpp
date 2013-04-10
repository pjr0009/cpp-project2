#include "headers/SessionController.h"
#include <curses.h>k

#include "headers/Staff.h"
#include "headers/DataLayer.h"
#include "headers/PresentationLayer.h"
#include "headers/Account.h"



SessionController::SessionController(){
	  login();
	  current_user.job_title = current_user.admin_level ? "System Administration" : "Branch Staff";
	  current_selection = current_user.job_title;
	  generate_navigation();

}
void SessionController::login(){
    string user_name, password;
	cout << "Enter User Name: ";
	cin >> user_name;
	cout << "\nEnter Password: ";
	cin >> password;
	map<string, string> session_info;
	while(!db.authenticate_staff(user_name, password, session_info)){
		string retry_login = "";
		cout << "Incorrect Username or Password, Try again? (y/n)" << endl;
		cin >> retry_login;
		if(retry_login == "y"){
			cout << "Enter User Name: ";
		    cin >> user_name;
			cout << "Enter Password: ";
		    cin >> password;
		    } else{
		    	exit(0);
		    }
	}
	if(db.authenticate_staff(user_name, password, session_info) == true){
		current_user.id = atoi(session_info["id"].c_str());
		current_user.name = session_info["name"];
		current_user.admin_level = atoi(session_info["admin_level"].c_str());
	}
}

bool SessionController::change_password(){
	map<string, string> staff;
	string password_1, password_2;
	present.sub_nav_header("Change Client Password");
	cout << "enter new password: ";
	cin >> password_1;
	cout << "re-enter new password to confirm, or type 'return to main menu' to exit: ";
	cin >> password_2;
	while(password_2 != "return to main menu" && password_2 != password_1){
		cout << "------ passwords did not match, try again... -------";
		cout << "enter new password: ";
		cin >> password_1;
		cout << "re-enter new password to confirm, or type 'return to main menu' to exit: ";
		cin >> password_2;
		}
	if(password_2 == "return to main menu"){
		return false;
	}
	else{
		staff["password"] = password_1;
		stringstream s;
		s << current_user.id;
		staff["id"] = s.str();
		if(db.update("staff", staff) == true){
			cout << "Successfully changed staff password" << endl;
			return true;
		} else {
			cout << "Could not change password at this time, please try again" << endl;
		}
	}
}

bool SessionController::add_client(){
	map<string, string> orm;
	present.sub_nav_header("Add a Client");
	cout << "Enter client name: ";
	cin.ignore();
	getline(cin, orm["name"]);
	cout << "Enter client address: ";
	getline(cin, orm["address"]);
	cout << "Enter client employer: ";
	getline(cin, orm["employer"]);
	cout << "Enter client social: ";
	getline(cin, orm["crypted_social"]);
	cout << "Enter client income: ";
	getline(cin, orm["income"]);
	if(db.persist("CLIENTS", orm) == true){
		cout << "\n1 Client Successfully Added." << endl;
		return true;
	} else{
		cout << "\n0 Clients Created, Please try again later..." << endl;
		return false;
	}
}

bool SessionController::edit_client(){
	string client_id;
	vector<map<string, string> > results;
	map<string, string> client;
	bool valid_selection = false;
	string name;
	present.sub_nav_header("Edit Client Information");
	cout << "Enter Client Name (leave blank for all): ";
	cin.ignore();
	getline(cin, name);
	bool continue_loop = true;
    while(!db.find("clients", "name", name, results, "false")){
    	cout << "\n----------------- 0 results found for '" << name << "' -----------------" << endl;
    	cout << "\nEnter Client Name (first, last), or type 'return to main menu' to exit: ";
    	getline(cin, name);
    	if(name == "return to main menu"){
    		return false;
    	}else {
    		db.find("clients", "name", name, results, "false");
    		continue_loop = false;
    	}
    }
    present.search_results(results, "client");
    cout << "\nEnter a Client ID: ";
    getline(cin, client_id);

    for (unsigned i = 0; i < results.size(); ++i){
    	if(results.at(i)["id"] == client_id){
    		valid_selection = true;
    		client = results.at(i);
    	}
	}

    while(!valid_selection){
    	cout << "----- Invalid client id entered, enter a valid client id or type 'return to main menu' to exit -----" << endl;
    	cout << "Enter a Client ID: ";
    	getline(cin, client_id);
    	if(client_id == "return to main menu"){
    		return false;
    	}

        for (unsigned i = 0; i < results.size(); ++i){
        	if(results.at(i)["id"] == client_id){
        		valid_selection = true;
        		client = results.at(i);
        	}
    	}
    }
    string temp = "";
    cout << "Enter client name (leave blank if unchanged): ";
	getline(cin, temp);
	if(temp.length() > 0){
		client["name"] = temp;
		temp = "";
	}
	cout << "Enter client address (leave blank if unchanged): ";
	getline(cin, temp);
	if(temp.length() > 0){
		client["address"] = temp;
		temp = "";
	}
	cout << "Enter client employer (leave blank if unchanged): ";
	getline(cin, temp);
	if(temp.length() > 0){
		client["employer"] = temp;
		temp = "";
	}
	cout << "Enter client social (leave blank if unchanged): ";
	getline(cin, temp);
	if(temp.length() > 0){
		client["crypted_social"] = temp;
		temp = "";
	}
	cout << "Enter client income (leave blank if unchanged): ";
	getline(cin, temp);
	if(temp.length() > 0){
		client["income"] = temp;
		temp = "";
	}
	cout << "\n----------------- Confirm New Client Details -----------------" << endl;
	map<string, string>::iterator iter;
    for (iter = client.begin(); iter != client.end(); ++iter) {
    	cout << iter->first << ": " << iter->second << endl;
    }
	cout << "\n---------------------------------------------------------" << endl << endl;
    string save_client;
    cout << "Save Changes to Client (y/n)?: ";
    cin >> save_client;
    if(save_client == "y"){
    	if(db.update("clients", client) == true){
    		cout << "Successfully Updated Client" << endl;
    		return true;

    	} else{ 
    		cout << "Could not update client at this time, please try agian later..." << endl;
    		return false;
    	}
		
	    }else {
	    	cout << "Client not updated, Returning to main menu..." << endl;
    		return false;
    }

}
bool SessionController::manage_account(){
	string account_id;
	vector<map<string, string> > results;
	map<string, string> client;
	map<string, string> account;
	bool valid_selection = false;
	string name;
	present.sub_nav_header("Manage an Account");

	cout << "Enter The Account Holder's Name (leave blank for all account holders): ";
	cin.ignore();
	getline(cin, name);
	bool continue_loop = true;
    while(!db.find("clients", "name", name, results, "accounts")){
    	cout << "\n----------------- 0 results found for '" << name << "' -----------------" << endl;
    	cout << "\nEnter Client Name (first, last), or type 'return to main menu' to exit: ";
    	getline(cin, name);
    	if(name == "return to main menu"){
    		return false;
    	}else {
    		db.find("clients", "name", name, results, "accounts");
    		continue_loop = false;
    	}
    }
    present.search_results(results, "account");
    cout << "\nEnter an Account ID: ";
    getline(cin, account_id);

    for (unsigned i = 0; i < results.size(); ++i){
    	if(results.at(i)["id"] == account_id){
    		valid_selection = true;
    		client = results.at(i);
    	}
	}

    while(!valid_selection){
    	cout << "----- Invalid account id entered, enter a valid account id or type 'return to main menu' to exit -----" << endl;
    	cout << "Enter a Client ID: ";
    	getline(cin, account_id);
    	if(account_id == "return to main menu"){
    		return false;
    	}
        for (unsigned i = 0; i < results.size(); ++i){
        	if(results.at(i)["id"] == account_id){
        		valid_selection = true;
        		client = results.at(i);
        	}
    	}
    }
  
    cout << "------------------- Account Management Options------------------" << endl;
    cout << "1) Deposit" << endl;
	cout << "2) Withdraw"  << endl;
	cout << "3) Cancel" << endl;
	cout << "Please choose an option: ";
	int selection = 0;
	int deposit_amt = 0;
	int withdraw_amt = 0;

	cin >> selection;
	switch(selection){
		case 1: 
			cout << "Enter Deposit Amount: ";
			cin >> deposit_amt;
			break;
		case 2:
			cout << "Enter Withdraw Amount: ";
			cin >> withdraw_amt;
			break;
		default:
			return false;
	}
	if(deposit_amt > 0){
		int balance = 0;
		cout << balance << endl << endl;
		stringstream s;
		 s << atoi(client["balance"].c_str()) + deposit_amt;
		 s >>  balance;
		 cout << balance << endl;
		client["balance"] = s.str();
		client["action"] = "Deposit";
	}
	if(withdraw_amt > 0){
		int balance = 0;
		
		stringstream s;
		s << atoi(client["balance"].c_str()) - withdraw_amt;
		s >> balance;
		cout << balance << endl;
		client["balance"] = s.str();
		client["action"] = "Withdraw";
	}
	account["balance"] = client["balance"];
    account["id"] = client["id"];
    cout << "\n----------------- Confirm Account " << client["action"] << " -----------------" << endl;
	cout << "Account Holder: " << client["name"] << endl;
	cout << "New Balance: $"   << account["balance"] << endl;
	cout << "\n---------------------------------------------------------------------------" << endl << endl;
    string save_client;
    cout << "Save Changes to Account (y/n)?: ";
    cin >> save_client;
    if(save_client == "y"){
     	if(db.update("accounts", account) == true){
    		cout << "Successfully Updated Client" << endl;
    		return true;

    	} else{ 
    		cout << "Could not update the account at this time, please try agian later..." << endl;
    		return false;
    	}
		
	    }
	    else {
    		return false;
    }

    return false;
 
}


bool SessionController::add_account(){
	string client_id;
	vector<map<string, string> > results;
	map<string, string> client;
	map<string, string> new_account;
	bool valid_selection = false;
	string name;
	present.sub_nav_header("Add an Account");

	cout << "Enter Client Name (leave blank for all): ";
	cin.ignore();
	getline(cin, name);
	bool continue_loop = true;
    while(!db.find("clients", "name", name, results, "false")){
    	cout << "\n----------------- 0 results found for '" << name << "' -----------------" << endl;
    	cout << "\nEnter Client Name (first, last), or type 'return to main menu' to exit: ";
    	getline(cin, name);
    	if(name == "return to main menu"){
    		return false;
    	}else {
    		db.find("clients", "name", name, results, "false");
    		continue_loop = false;
    	}
    }
    present.search_results(results, "client");
    cout << "\nEnter a Client ID: ";
    getline(cin, client_id);

    for (unsigned i = 0; i < results.size(); ++i){
    	if(results.at(i)["id"] == client_id){
    		valid_selection = true;
    		client = results.at(i);
    	}
	}

    while(!valid_selection){
    	cout << "----- Invalid client id entered, enter a valid client id or type 'return to main menu' to exit -----" << endl;
    	cout << "Enter a Client ID: ";
    	getline(cin, client_id);
    	if(client_id == "return to main menu"){
    		return false;
    	}

        for (unsigned i = 0; i < results.size(); ++i){
        	if(results.at(i)["id"] == client_id){
        		valid_selection = true;
        		client = results.at(i);
        	}
    	}
    }
    new_account["client_id"] = client["id"];
    cout << "Enter Account Type: ";
    getline(cin, new_account["type"]);
    cout << "Enter Account Balance: ";
    getline(cin, new_account["balance"]);
    cout << "\n----------------- Confirm New Account For " << client["name"] << " -----------------" << endl;
	cout << "Account Holder: " << client["name"] << endl;
	cout << "Balance: $"   << new_account["balance"] << endl;
	cout << "\n---------------------------------------------------------------------------" << endl << endl;
    string save_account;
    cout << "Create Account (y/n)?: ";
    cin >> save_account;
    if(save_account == "y"){
    if(db.persist("accounts", new_account) == true){
    	cout << "Successfully created a new account" << endl;
    	return true;
    } else {
    	cout << "Could not create the account at this time, please try agian later..." << endl;
    	return false;
    } 
    } else {
    	return false;
    }



}


bool SessionController::add_branch_staff(){
	map<string, string> new_staff;
	string selection;
	present.sub_nav_header("Add Branch Staff");
	cout << "Enter Staff Name: ";
	cin.ignore();
	getline(cin, new_staff["name"]);
	cout << "Enter Staff Username: ";
	getline(cin, new_staff["user_name"]);
	cout << "Set Staff Password: ";
	getline(cin, new_staff["password"]);
	cout << "Select Administrator Level" << endl;
	cout << "0) Branch Staff" << endl;
	cout << "1) System Administrator" << endl;
	cout << "Enter Selection: ";
	getline(cin, selection);
	while(selection != "0" && selection != "1" && selection != "return to main menu"){
    	cout << "----- Invalid menu option selected, try again or type 'return to main menu' to exit -----" << endl;
    	cout << "Enter Selection: ";
    	getline(cin, selection);

	}
	if(selection == "return to main menu"){
		return false;
	}
	new_staff["admin_level"] = selection;
    cout << "\n----------------- Confirm New Staff Details -----------------" << endl;
	cout << "Name: " << new_staff["name"] << endl;
	cout << "User Name: " << new_staff["user_name"] << endl;
	cout << "Password: " << new_staff["password"] << endl;
	if(new_staff["admin_level"] == "1")
		cout << "Role: System Administrator" << endl;
	else
		cout << "Role: Branch Staff" << endl;
	cout << "\n-------------------------------------------------------------" << endl << endl;
    string save_staff;
    cout << "Save Changes to Client (y/n)?: ";
    cin >> save_staff;
    if(save_staff == "y"){
	if(new_staff["admin_level"] == "1"){		
		if(db.persist("staff", new_staff) == true){
    	cout << "Successfully created a new System Administrator" << endl;			
		return true;
		} else {
			cout << "Could not create System Administrator at this time, please try again later..." << endl;
			return false;
		}


	} else if (new_staff["admin_level"] == "0"){
		if(db.persist("staff", new_staff) == true) {
		   cout << "Successfully created a new Branch Staff" << endl;
		   return true;
		} else{
			cout << "Could not create Branch Staff at this time, please try again later..." << endl;
			return false;
		}
	} else {
		return false;
	}
} else{
	return false;
}



}


bool SessionController::branch_staff_db(){
	vector<map <string, string> > results;
	string continue_loop = "y";
	string query;
	db.find("staff", "name", "", results, "false");
	present.search_results(results, "staff");
	cout << "search for staff member? (y/n): ";
	cin >> continue_loop;
	if(continue_loop != "y"){
		return false;
	}else {
		while(continue_loop == "y"){
			cout << "Enter Query: ";
			cin.ignore();
			getline(cin, query); 
			db.find("staff", "name", query, results, "false");
			present.search_results(results, "staff");
			cout << "search for another staff member? (y/n): ";
			cin >> continue_loop;
		}
	}
	return true;
}

bool SessionController::delete_branch_staff(){
	string staff_id;
	vector<map<string, string> > results;
	map<string, string> staff;
	bool valid_selection = false;
	string name;
	present.sub_nav_header("Remove Branch Staff");

	cout << "Enter Staff Name (leave blank for all): ";
	cin.ignore();
	getline(cin, name);
	bool continue_loop = true;
    while(!db.find("staff", "name", name, results, "false")){
    	cout << "\n----------------- 0 results found for '" << name << "' -----------------" << endl;
    	cout << "\nEnter Staff Name (first, last), or type 'return to main menu' to exit: ";
    	getline(cin, name);
    	if(name == "return to main menu"){
    		return false;
    	}else {
    		db.find("staff", "name", name, results, "false");
    		continue_loop = false;
    	}
    }
    present.search_results(results, "staff");
    cout << "\nEnter a Staff ID: ";
    getline(cin, staff_id);

    for (unsigned i = 0; i < results.size(); ++i){
    	if(results.at(i)["id"] == staff_id){
    		valid_selection = true;
    		staff = results.at(i);
    	}
	}

    while(!valid_selection){
    	cout << "----- Invalid staff id entered, enter a valid staff id or type 'return to main menu' to exit -----" << endl;
    	cout << "Enter a Client ID: ";
    	getline(cin, staff_id);
    	if(staff_id == "return to main menu"){
    		return false;
    	}

        for (unsigned i = 0; i < results.size(); ++i){
        	if(results.at(i)["id"] == staff_id){
        		valid_selection = true;
        		staff = results.at(i);
        	}
    	}
    }
    cout << "\n----------------------------- Confirm Delete ------------------------------" << endl;
	cout << "Name: " << staff["name"] << endl;
	cout << "\n---------------------------------------------------------------------------" << endl << endl;
    string save_staff;
    cout << "Delete Staff (y/n)?: ";
    cin >> save_staff;
    if(save_staff == "y"){
     	if(db.delete_entity("staff", staff["id"]) == true){
    		cout << "Successfully Deleted Staff" << endl;
    		return true;

    	} else{ 
    		cout << "Could not Delete the Staff member at this time, please try agian later..." << endl;
    		return false;
    	}
		
	    }
	    else {
    		return false;
    }

    return false;



}

void SessionController::generate_navigation(){
		if(current_selection == "System Administration" || current_selection == "Branch Staff"){
			if(current_user.admin_level == 1){
				navigation[1] = "Client and Account Management";
				navigation[2] = "Add a branch staff";
				navigation[3] = "Delete a branch staff";
				navigation[4] = "Display branch staffs";
				navigation[5] = "Change password";
				navigation[6] = "Exit";
			} else if (current_user.admin_level == 0) {
				navigation[1] = "Client and Account Management";
				navigation[2] = "Change password";
				navigation[3] = "Exit";

			}
		}

		else if(current_selection == "Client and Account Management"){
			navigation[1] =  "Add a client";
			navigation[2] = "Add an account";
			navigation[3] = "Edit Client Information";
			navigation[4] = "Manage an Account";
			navigation[5] = "Save Client and Account Information";
			navigation[6] = "Main Menu";
			navigation[7] = "Exit";

		}
		else if(current_selection == "Add a client"){
			add_client();
			current_selection = current_user.job_title;
		}
		else if(current_selection == "Add an account"){
			add_account();
			current_selection = current_user.job_title;
		}
		else if (current_selection == "Edit Client Information"){
			edit_client();
			current_selection = current_user.job_title;
		}
		else if (current_selection == "Manage an Account"){
			manage_account();
			current_selection = current_user.job_title;
		}
		else if (current_selection == "Add a branch staff")
		{
			add_branch_staff();
			current_selection = current_user.job_title;
		}
		else if (current_selection == "Main Menu"){
			current_selection = current_user.job_title;
		}
		else if (current_selection == "Display branch staffs"){
			branch_staff_db();
			current_selection = current_user.job_title;

		}
		else if (current_selection == "Change password"){
			change_password();
			current_selection = current_user.job_title;
		}
		else if(current_selection == "Delete a branch staff"){
			delete_branch_staff();
			current_selection = current_user.job_title;
		}
		else{
			cout << "--------- INVALID SELECTION, PLEASE CHOOSE A DISPLAYED OPTION ---------" << endl;
			current_selection = current_user.job_title;
		}
}
