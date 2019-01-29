#ifndef __SENSORS_HELPER_H__
#define __SENSORS_HELPER_H__

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

class SensorsHelper
{
public:
	SensorsHelper();
public:
	void init();
	void loop();
	float getTemperature();
	float getHumidity();
	float getPreasure();  
private:
  Adafruit_BME280 bme; // I2C
};
extern SensorsHelper sensorsHelper;
#endif /* #ifndef __SENSORS_HELPER_H__ */
