#pragma once

#include <Arduino.h>


typedef enum {
	debug,
	info,
	warning,
	error
} logLevel_t;


class Logger
{
public:
	Logger();
	void log(logLevel_t level, const char *format, ...);
};
extern Logger logger;
