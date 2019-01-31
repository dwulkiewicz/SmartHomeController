#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <Arduino.h>
#include "Logger.h"
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


#define LIGHT_BATH_MAIN_1_IDX         0
#define LIGHT_BATH_HOLDER_1_IDX       1
#define LIGHT_BATH_TAPE_1_IDX         2
#define LIGHT_BATH_RGB_V_1_IDX        3
#define LIGHT_BATH_RGB_H_1_IDX        4
#define LIGHT_BATH_MAIN_2_IDX         5
#define LIGHT_BATH_HOLDER_2_IDX       6
#define LIGHT_BATH_TAPE_2_IDX         7
#define LIGHT_BATH_RGB_V_2_IDX        8
#define LIGHT_BATH_RGB_H_2_IDX        9
#define LIGHT_BATH_MAIN_3_IDX        10
#define LIGHT_BATH_HOLDER_3_IDX      11
#define LIGHT_BATH_TAPE_3_IDX        12
#define LIGHT_BATH_RGB_V_3_IDX       13
#define LIGHT_BATH_RGB_H_3_IDX       14

#define LIGHT_MAX_SAFEGUARD          15  

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

	uint8_t getLightsBathValue(uint8_t idx);
	void setLightsBathValue(uint8_t idx, uint8_t value);
private:
	uint8_t offsetTemp;
	uint8_t dayTemp;
	uint8_t nightTemp;
	uint8_t hysteresisTemp;
	TShortTime heatingTimes[HEATING_TIMES_MAX_SAFEGUARD];
	uint8_t lightValues[LIGHT_MAX_SAFEGUARD];
	void saveHeatingTime(uint8_t idx);
	bool loadFromJSON();
	bool loadFromEEPROM();
public: //TODO: do zmiany na get
	String wifiSSID;
	String wifiPassword;
	String mqttServer;
};
extern Configuration configuration;
#endif /* #ifndef __CONFIGURATION_H__ */
