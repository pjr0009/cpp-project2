/*
 * PresentationLayer.h
 *
 *  Created on: Mar 24, 2013
 *      Author: phillip
 */
#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <iomanip>
#include "Staff.h"
using namespace std;

 class SessionController;

class PresentationLayer{
public:
	void loading();
	void complete();
	void login_header();
	void sub_nav_header(string page);
	void welcome_back();
	void navigation(SessionController& session, string page);
    void search_results(vector <map <string, string> > results, string type);

};


