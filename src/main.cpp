#include "headers/PresentationLayer.h"
#include "headers/SessionController.h"
#include "headers/Staff.h"


/*
 * main.cpp
 *
 *  Created on: Mar 21, 2013
 *      Author: phillip
 */


int main(){
	PresentationLayer present;
	present.loading();
	present.complete();
	present.login_header();
	SessionController session;
	present.welcome_back();
    while(session.current_selection != "Exit"){
        session.navigation.clear();
        session.generate_navigation();
        present.navigation(session, session.current_selection);
    }
    if(session.current_selection == "Exit"){
    	cout << "=========================================================================" << endl;
    	cout << "            Goodbye " << session.current_user.name << endl;
    	cout << "=========================================================================" << endl;
    }
    return 0;

}





