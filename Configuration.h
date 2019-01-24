#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <Arduino.h>
#include "Types.h"

#define HEATING_WORKING_DAYS_MORNING_ON     0 
#define HEATING_WORKING_DAYS_MORNING_OFF    1 
#define HEATING_WORKING_DAYS_AFTERNOON_ON   2 
#define HEATING_WORKING_DAYS_AFTERNOON_OFF  3 
#define HEATING_WEEKEND_MORNING_ON          4 
#define HEATING_WEEKEND_MORNING_OFF         5 
#define HEATING_WEEKEND_AFTERNOON_ON        6 
#define HEATING_WEEKEND_AFTERNOON_OFF       7   

#define HEATING_TIMES_MAX_SAFEGUARD         8  

class Configuration
{
public:
	static String temperatureAsString(float temp);
public:
	Configuration();
public:
	bool init();
  void print();
public:
	float getDayTemperature();
	bool incrementDayTemperature();
	bool decrementDayTemperature();

	float getNightTemperature();
	bool incrementNightTemperature();
	bool decrementNightTemperature();

	float getHisteresisTemp();
	bool incrementHisteresisTemperature();
	bool decrementHisteresisTemperature();
  
  TShortTime getHeatingTime(uint8_t idx);
  bool incrementHeatingTime(uint8_t idx);
  bool decrementHeatingTime(uint8_t idx);
  
  void setParam(uint8_t paramId, uint8_t value); //0-255
  uint8_t getParam(uint8_t paramId); //0-255  
private:
	uint8_t offsetTemp;
	uint8_t dayTemp;
	uint8_t nightTemp;
	uint8_t hysteresisTemp;
  TShortTime heatingTimes[HEATING_TIMES_MAX_SAFEGUARD]; 
  void saveHeatingTime(uint8_t idx);  
public: //TODO: do zmiany na get
	String wifiSSID;
	String wifiPassword;
	String mqttServer;
};
extern Configuration configuration;
#endif /* #ifndef __CONFIGURATION_H__ */
