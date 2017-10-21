# Simplescript
Is a weakly-typed object-oriented scripting language designed to be easily integrated with C++.
A quick example:

##### fib.ss
```c++
table fibsolver
{
	int get_fib(int n)
	{
		if (n == 0 || n == 1) {	return n; }
		else {
			return this.get_fib(n-1) + this.get_fib(n-2);
		}
	}
}
```

##### main.cpp
```c++
#include "SSInterpreter.h"
#include <iostream>

int main()
{
	SSInterpreter fibss("fib.ss");
  
	auto table = fibss.InstantiateTable("fibsolver");
	auto result = table->CallMethod("get_fib", 10);
  
	std::cout << result.GetIntRef(); //will print 10th fibonacci number, 55

	return 0;
}
```

# Installation
For MSVS:
1. Download SS_shared VS project
2. Add it to your solution via 'Add existing project'
3. Rightclick 'References' item under your project in solution explorer and select 'Add reference'
4. Turn on checkbox for 'SS_shared'
5. Include 'SSInterpreter.h' header in your source code

Alternatively, just download SS_fullsrc, include contents in your project and reference 'SSInterpreter.h' header

# Types
Basic types are int (32), float (32) and string. There are no global variables, variables can only be members of tables.

# Tables
Tables are essentially classes. No code can be outside of table methods. OOP is supported via inheritance and method overloading. 
``` c++
table IBaseClass
{
	void VirtualMethod()
	{
		ss.print("IBaseClass::VirtualMethod called");
	}
	
	string m_text = "member variable, will be inherited by derived";
}

table Derived : IBaseClass
{
	void VirtualMethod()
	{
		ss.print("overloaded Derived::VirtualMethod called");
	}
}
```
Tables are also used as arrays and associative containers:
``` c++
table SomeClass {}

table arrays_example
{
	void Execute()
	{
		table array;
		array[0] = 1;
		array[1] = "text";
		array[2] = "any table or variable";
		PrintArrayContents(array);

		table map;
		SomeClass instance;
		map["text literal"] = "text";
		map["some object"] = instance;
	}
	void PrintArrayContents(table &arr)
	{
		for (int i = 0; i < arr.size(); i = i + 1;)
		{
			ss.print("Element ", i, ":", arr[i], "\n");
		}
	}
}
```
# Exposing C++ code to script
Providing interface to C++ code is done via creating C++ functor class and using it as method for one of tables defined in script. It is possible to add method to all tables by passing ```*``` as table name in AddMethod()

##### apiexample.ss
``` c++
table api_example_table
{
	void Execute()
	{
		ss.print("Calling exposed method:\n");
		ss.print("1 + 2 + 3 = ", this.cppsum(1, 2, 3));
	}
}
```
##### main.cpp
``` c++
#include <iostream>
#include "SSInterpreter.h"
#include "IFunctor.h"

class TestSumFunctor :
	public IFunctor
{
public:
	TestSumFunctor();
	virtual ~TestSumFunctor();

	/*
	The method returns sum of all arguments
	(assuming all arguments are integers)
	*/
	virtual VarPtr Call(SSInterpreter &interp, Table& obj, std::vector<VarPtr> args, IMsgHandler &errhandler)
	{
		int result = 0;

		for (auto &arg : args)
		{
			if (!arg) continue;

			result += arg->GetIntRef();
		}

		return VarPtr(new Variable(result));
	}
};

int main()
{
	SSInterpreter ss;
	API_Collection api;
	
	//table 'api_example_table' receives method 'cppsum', specified by functor
	api.AddMethod("api_example_table", "cppsum", SharedFunctor(new TestSumFunctor()), ss);

	ss.LoadFromFile("apiexample.ss", { api });

	auto t = ss.InstantiateTable("api_example_table");
	t->CallMethod("Execute");

	return 0;
}
```

# Standard library methods
Some standard methods are functors which are added as methods to table named 'ss'. This table is accesible from any place in the simplescript code. Other methods are added to all tables. List of currently existing standard methods:
``` c++
ss.print(<args>); //prints all arguments sequentially
ss.readline(); //reads line from stdin and returns it
ss.readfile(<filename>); //reads filename and returns as table array (every line as new element)
ss.appendtofile(<filename>, <string>); //appends line to file
ss.copy(<object>); //returns copy of object passed as argument
ss.split(<string>); //splits line to words and returns as array
ss.concat(<string args>); //concatenates all args and returns as one string
ss.tostring(<object>); //converts object to string
ss.toint(<object>); //converts object to integer
ss.tofloat(<object>); //converts object to float

*.hasvars(<varnames>); //takes any amount of variable names as args, returns how many of them are members of table
*.size(); //returns number of member variables
*.delete(<varnames>); //deletes member variables
*.getvars(); //returns all member variables names as array
```
