class TestClass
{
public:
	void TestFunc(int param1);
	bool GetBoolFunc();
};


void TestClass::TestFunc(int param1)
{
/*
	int q;
	int z = 42;
	
	q = z;
*/
}

bool TestClass::GetBoolFunc()
{
	return true;
}

int main()
{
	cout << "Starting program" << endl;
	int x = 1;
	bool bac = true;
	bool* pb = &bac;
	
	TestClass tc;
	ofstream fout;
	vector<int>::iterator v;
	
	
	if(pb)
	{
		bac = false;
	}
	else if(false)
	{
		x += 3;
	}
	else
	{
		x += 4;
	}
	
	for(int q = 0, z = 96; q < 42; q++)
	{
		bac = true;
	}
	
	while(tc.GetBoolFunc())
	{
		bac = false;
	}
	
	while(int m = 123)
	{
		bac = true;
	}
	
	if(*pb && 
		(x == 1 && 1 < 2))
	{
		bac = false;
	}
	
	switch(i):
	{
		case 0:
		{
		  cout << "case 0" << endl;
		  
		}
		case 1:
		{
		  cout << "case 1" << endl;
		  break;
		}
		default:
		  break;
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