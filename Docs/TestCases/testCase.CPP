#include <iostream>
#include <string>

using namespace std;

void foo(string);

int main()
{
	string x = "This is a test";
	foo(x);
	return 0;
}

void foo(int x) {
	cout << x << endl;
}
