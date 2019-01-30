#include "Logger.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Logger::Logger()
{
}
//------------------------------------------------------------------
void Logger::log(logLevel_t level, const char *format, ...) {
	va_list arg;
	va_list copy;
	va_start(arg, format);
	va_copy(copy, arg);
	Serial.printf(format,arg);
}

Logger logger;

