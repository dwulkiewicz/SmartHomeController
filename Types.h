#ifndef __Types_H__
#define __Types_H__
#include <Arduino.h>

typedef struct {
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t dayOfWeek;   // day of week
	uint8_t day;
	uint8_t month;
	uint8_t year;   // offset from 2000; 
} 	TDateTime;

typedef struct {
  uint8_t minute;
  uint8_t hour;
}   TShortTime;

typedef struct {
	//Time
	TDateTime dateTime;
	//Indoor sensor
	float indoorTemperature;
	float indoorHumidity;
	float indoorPressure;  
	//Outdoor sensor
	float outdoorTemperature;
	float outdoorHumidity;
	float outdoorPressure;  
	//Heating
	uint8_t heatingStatus;
	//Network
	uint8_t wifiStatus;	
	uint8_t mqttStatus;		
	//Switch
	uint8_t switch1State;
	uint8_t switch2State;
	uint8_t switch3State;	
}	TDisplayBuffer;



#endif /* #ifndef __Types_H__ */