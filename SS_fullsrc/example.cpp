#include "stdafx.h"
/*#include "SSInterpreter.h"
#include <iostream>
#include <iostream>
#include <ctime>
#include "TestSumFunctor.h"
using namespace std;

int main()
{
	SSInterpreter ss;

	API_Collection api;

	//if interpreter encounters class 'kek', it will add method 'testsum' to it
	api.AddMethod("kek", "testsum", SharedFunctor(new TestSumFunctor()), ss);
	
	ss.LoadFromFile("newtests.ss", { api });

	clock_t running_start = clock();

	auto t = ss.InstantiateTable("main");
	t->CallMethod("start");

	clock_t running_end = clock();

	cout << "\nRunning completed. Time = " << running_end - running_start << endl;
	
	int _a; std::cin >> _a;
    return 0;
}*/

