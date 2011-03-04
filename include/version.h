#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "22";
	static const char MONTH[] = "05";
	static const char YEAR[] = "2010";
	static const double UBUNTU_VERSION_STYLE = 10.05;
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 29;
	static const long BUILD = 2917;
	static const long REVISION = 16022;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 2973;
	#define RC_FILEVERSION 1,29,2917,16022
	#define RC_FILEVERSION_STRING "1, 29, 2917, 16022\0"
	static const char FULLVERSION_STRING[] = "1.29.2917.16022";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 17;
	

}
#endif //VERSION_h
