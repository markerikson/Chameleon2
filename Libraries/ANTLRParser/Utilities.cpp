

#include <vcclr.h>
#include <msclr\marshal.h>
#include <vector>
#include <queue>
#include <string>

#include "Utilities.h"

#using <System.dll>
#using <mscorlib.dll>

using namespace System;
using namespace System::Collections::Generic;
using namespace msclr::interop;
using namespace System::Runtime::InteropServices;

using namespace std;

String^ ConvertString(const string& nativeString)
{
	String^ managedString = marshal_as<String^>(nativeString.c_str());
	return managedString;
}



string ConvertString(String^ managedString)
{
	marshal_context context;
	string nativeString = context.marshal_as<const char*>(managedString);
	return nativeString;
}



