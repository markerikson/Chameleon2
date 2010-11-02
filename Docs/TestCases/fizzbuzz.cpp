#include <iostream>
#include <string>

using namespace std;



class TestClass
{
public:
	void TestFunc(int param1);
	bool GetBoolFunc();
}

void TestClass::TestFunc(int param1)
{
	int q;
	int z = 42;
	
	q = z;
}

bool TestClass::GetBoolFunc()
{
	return true;
}

int main()
{
    cout << "Starting program" << endl;
    int x = 1;
	bool b = true;
	bool* pb = &b;
	
	TestClass tc;
	
	if(pb)
	{
		
	}
	
	if(tc.GetBoolFunc())
	{
		
	}
	

    while(x < 101)
    {
        if(x % 3 == 0 && x % 5 == 0)
        {
            cout << "FizzBuzz" << endl;
        }
        else
        {
            if(x % 3 == 0)
            {
                cout << "Fizz" << endl;
            }
            else
            {
                if(x % 5 == 0)
                {
                    cout << "Buzz" << endl;
                }
                else
                {
                    cout << x << endl;
                }
            }
        }
        sleep(300);
        x += 1;

    }

    return 0;
}