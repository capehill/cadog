#include "Log.h"

Log hLog;

Log::Log()
{
/*	fpLogFile = fopen( LOGFILE, "a" );
	return;*/
}

Log::~Log()
{
/*	fclose( fpLogFile );
	fpLogFile = 0;
	return;*/
}

void Log::LogString(char *szString)
{
/*	fprintf( fpLogFile,"%s", szString );
	fflush( fpLogFile );*/
}

// Helper function to log
void LOG( char *szString, ... )
{
/*	char szLogString[256];
	va_list args;

	va_start( args, szString );
	vsprintf( szLogString, szString, args );
	va_end( args );
	hLog.LogString( szLogString );*/
}
