#pragma once

#ifndef __SENSORS_HELPER_H__
#define __SENSORS_HELPER_H__


#include <Arduino.h>


class SensorsHelper
{
private:
	SensorsHelper();
	~SensorsHelper();
public:
	static void init();
	//static void startMeasure();
	static uint16_t getTemperature();
  static uint16_t getHumidity();
  static uint16_t getPreasure();
};

#endif /* #ifndef __SENSORS_HELPER_H__ */
