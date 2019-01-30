#include "Logger.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Logger::Logger()
{
}
//------------------------------------------------------------------
void Logger::log(logLevel_t level, const char *format, ...) {
//#if DEBUG_MODE
	char buffer[256];
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
	Serial.print(buffer);
//#endif
}

Logger logger;

