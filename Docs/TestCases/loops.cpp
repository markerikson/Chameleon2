int main()
{
	for(int i = 0; i < 10; i++)
	{
		doSomething();
	}
	
	for(int j = 10; j > 0; --j)
		doSomething2();
		
	while(true)
	{
		doSomething3();
	}
	
	while(false)
		doSomething4();

	return 0;
}