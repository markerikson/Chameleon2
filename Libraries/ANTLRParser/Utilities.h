#ifndef PARSER_UTILITIES_H
#define PARSER_UTILITIES_H

#include <vcclr.h>
#include <msclr\marshal.h>
#include <vector>
#include <queue>


#using <System.dll>
#using <mscorlib.dll>

using namespace System;
using namespace System::Collections::Generic;
using namespace msclr::interop;
using namespace System::Runtime::InteropServices;

using namespace std;



String^ ConvertString(const string& nativeString);
string ConvertString(String^ managedString);



#endif