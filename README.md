# simplescript
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
