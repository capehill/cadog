#ifndef _LOG_H_
#define _LOG_H_

#include <stdio.h>
#include <stdarg.h>

#define LOGFILE		"Log.txt"

class Log
{

public:

	void LogString(char *szString);
	Log();
	~Log();

protected:
	
	FILE *fpLogFile;

};

void LOG( char *szString, ... );

#endif
