#include <iostream>
#include <string>

using namespace std;

// ERROR: no global variables
int myGlobalVariable = 42;

void doSomething() {}
void doSomethingElse() {}
void doAThirdThing() {}

int main()
{
	// OK: no assignment
	while(false)
	{
		doSomething();
	}

	int condition = 0;

	// ERROR: assignment in condition
	while(condition = 123)
	{
		doSomething();
		condition++;
	}

	// ERROR: single character variables only in loops
	int a = 0;

	// OK: single char variable in loop
	for(int i = 0; i < 10; i++)
	{
		//doSomething();
		condition++;
	}

	// ERROR: no braces for block
	if(true)
		condition++;

	if(false)
	{
		condition++;
	}
	// ERROR: no braces for block
	else if(false)
		condition += 2;
	else if(true)
	{
		condition++;
	}
	// ERROR: no braces for block
	else
		condition += 3;


	return 0;
}