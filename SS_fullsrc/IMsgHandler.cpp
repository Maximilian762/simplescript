#include "stdafx.h"
#include "IMsgHandler.h"
#include <iostream>
using namespace std;

IMsgHandler::IMsgHandler()
{
}


IMsgHandler::~IMsgHandler()
{
}

void IMsgHandler::OnMsg(std::string msg)
{
	cout << "Error: " << msg << endl;
}
