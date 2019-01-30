#ifndef __DisplayControler_H__
#define __DisplayControler_H__

#include <Arduino.h>
#include <Nextion.h>

#include "DisplayBuffer.h"

#define SETUP_DATETIME_YEAR         0
#define SETUP_DATETIME_MONTH        1
#define SETUP_DATETIME_DAY          2
#define SETUP_DATETIME_DAY_OF_WEEK  3
#define SETUP_DATETIME_HOUR         4
#define SETUP_DATETIME_MINUTE       5




class DisplayControler
{
public:
	DisplayControler();
public:
	void init();
	void loop();
public:
	void onRefreshDateTime(const DateTime& dateTime);
	void onRefreshIndoorTemperature(float indoorTemp);
	void onRefresIndoorHumidity(float indoorHumidity);
	void onRefreshIndoorPreasure(float indoorPressure);  
	void onRefreshOutdoorTemperature(float outdoorTemp);
	void onRefreshOutdoorHumidity(float outdoorHumidity);
	void onRefreshOutdoorPreasure(float outdoorPressure);
	void onSwitchChanged(uint8_t switchId, uint8_t switchState);
	void refreshHeatingStatus(uint8_t heatingStatus);
	void refreshHeatingPeriod(uint8_t heatingPeriod);

	void refresMainPage();
	void refreshHeatingPage();
	void refreshLightsPage();
	void refreshOtherPage();

	void showWiFiStatus(int8_t status);
	void showMQTTStatus(int status);
public: 
	uint8_t currentPage; //todo przenieść do private, obudować
	uint8_t currentTimeComponent; //todo przenieść do private, obudować
	uint8_t sw1State;//TODO: do przerobienia
	uint8_t sw2State;//TODO: do przerobienia
	uint8_t sw3State;//TODO: do przerobienia
private:
	static uint8_t dayOfWeekPic(uint8_t dayOfWeek);
	static uint8_t monthPic(uint8_t month);
	static uint8_t dayOfMonthPic(uint8_t digit); 

	static void onBtnTempPush(void *ptr);
	static void onBtnbDateTimeSetPush(void *ptr);
	static void onBtnTempHisteresisPush(void *ptr);
  
private:
	void refreshBathSw1();
	void refreshBathSw2();
	void refreshBathSw3();
	void refreshIndoorTemperature();
	void refreshIndoorHumidity();  
	void refreshOutdoorTemperature();
	void refreshOutdoorHumidity();
	void refreshPreasure();
	void decHeatingTime(uint8_t idx, class NexText* obj);
	void incHeatingTime(uint8_t idx, class NexText* obj);
	void showHeatingTime(uint8_t idx, NexText* obj);
private:
	DisplayBuffer disp;
	DisplayBuffer curr;

  //TODO: zmienne przenieść do struktury
  //Switch
	uint8_t lastSw1State;
	uint8_t lastSw2State;
	uint8_t lastSw3State;
 
};
extern DisplayControler displayControler;
#endif /* #ifndef __DisplayControler_H__ */
