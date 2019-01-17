#ifndef __DisplayControler_H__
#define __DisplayControler_H__

#include <Arduino.h>
#include <Nextion.h>
#include "Configuration.h"

#define SETUP_DATETIME_YEAR         0
#define SETUP_DATETIME_MONTH        1
#define SETUP_DATETIME_DAY          2
#define SETUP_DATETIME_DAY_OF_WEEK  3
#define SETUP_DATETIME_HOUR         4
#define SETUP_DATETIME_MINUTE       5

class DisplayControler
{
public:
	DisplayControler(Configuration* configuration);
public:
	void init();
	void loop();
public:
	void refreshTime();
	void refreshIndoorTemperature();
	void refresMainPage();
	void refreshHeatingPage();
	void refreshLightsPage();
	void refreshOtherPage();

	void showOutdoorTemperature(float outdoorTemp);
	void showOutdoorHumidity(float outdoorHumidity);
	void showPressure(float outdoorPressure);
	void showWiFiStatus(int8_t status);
	void showMQTTConnected(bool connected);

	void setSwitch(uint8_t item, String value);  

public: //todo przenieść do private, obudować
	uint8_t currentPage;
	uint8_t currentTimeComponent;
private:
	static uint8_t dayOfWeekPic(uint8_t dayOfWeek);
	static uint8_t monthPic(uint8_t month);
	static uint8_t dayOfMonthPic(uint8_t digit);
private:
	Configuration* configuration;
	//Time
	uint8_t lastMinute;
	uint8_t lastHour;
	uint8_t lastDay;
	uint8_t lastMonth;
	uint8_t lastDayOfWeek;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint8_t dayOfWeek;
	//Indoor sensor
	uint8_t lastTemp1 = 255;
	uint8_t lastTemp2 = 255;
	uint16_t lastHumidity = 999;
};
/*
extern NexText tIndoorTemp1;
extern NexText tIndoorTemp2;
extern NexText tIndoorHumidity;

extern NexText tOutdoorTemp1;
extern NexText tOutdoorTemp2;
extern NexText tOutdoorTempSymbol;
extern NexText tOutdoorHumidity;
extern NexText tOutdoorPreasure;

extern NexPicture pDayOfMonth1;
extern NexPicture pDayOfMonth2;
extern NexPicture pDayOfWeek;
extern NexPicture pMonth;

extern NexText tTime1;
extern NexText tTime2;
extern NexText tTime3;
extern NexText tTime4;
*/


#endif /* #ifndef __DisplayControler_H__ */
