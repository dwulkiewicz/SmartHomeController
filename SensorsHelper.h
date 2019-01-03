#pragma once
#include <Arduino.h>


#ifndef __SENSORS_HELPER_H__
#define __SENSORS_HELPER_H__

class SensorsHelper
{
private:
	SensorsHelper();
	~SensorsHelper();
public:
	static void init();
	static void startMeasure();
	static uint16_t getTemperature();
};

#endif /* #ifndef __SENSORS_HELPER_H__ */



