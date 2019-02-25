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
	//DateTime
	void onRefreshDateTime(const DateTime& dateTime);
	//Indoor
	void onRefreshIndoorTemperature(float indoorTemp);
	void onRefresIndoorHumidity(float indoorHumidity);
	void onRefreshIndoorPreasure(float indoorPressure);  
	//Outdoor
	void onRefreshOutdoorTemperature(float outdoorTemp);
	void onRefreshOutdoorHumidity(float outdoorHumidity);
	void onRefreshOutdoorPreasure(float outdoorPressure);
	//Switches
	void onSwitchChange(uint8_t src, uint8_t switchId, uint8_t switchState);
	//Heating
	void refreshHeatingStatus(uint8_t heatingStatus);
	void refreshHeatingPeriod(uint8_t heatingPeriod);
	void refreshHeatingRequiredTemp(float value);
	//Network
	void refreshWiFiStatus(int8_t status);
	void refreshMQTTStatus(int status);
public: 
	uint8_t currentPage; //todo przenieść do private, obudować
	uint8_t currentTimeComponent; //todo przenieść do private, obudować
	uint8_t sw1State;//TODO: do przerobienia
	uint8_t sw2State;//TODO: do przerobienia
	uint8_t sw3State;//TODO: do przerobienia
	uint8_t kitchenSw1State;
	uint8_t kitchenSw2State;
	uint8_t sonOffSwitch01;//TODO: do przerobienia

private:
	static uint8_t dayOfWeekPic(uint8_t dayOfWeek);
	static uint8_t monthPic(uint8_t month);
	static uint8_t dayOfMonthPic(uint8_t digit); 

	static void onPageShow(void *ptr);
	static void onBtnTempPush(void *ptr);
	static void onBtnbDateTimeSetPush(void *ptr);
	static void onBtnTempHisteresisPush(void *ptr);
	static void onSwitchPush(void *ptr);	
	static void onDateTimeNextPush(void *ptr);
	static void onSliderLightPop(void *ptr);  
private:
	void refresMainPage();
	void refreshHeatingPage();
	void refreshLightsPage();
	void refreshLightsAutoPage();
	void refreshTimePage();
	void refreshNetworkPage();
	void refreshOtherPage();
	
	void refreshBathSw1();//TODO: do przerobienia na jedną funkcję
	void refreshBathSw2();//TODO: do przerobienia na jedną funkcję
	void refreshBathSw3();//TODO: do przerobienia na jedną funkcję
	void refreshKitchenSw1();//TODO: do przerobienia na jedną funkcję
	void refreshKitchenSw2();//TODO: do przerobienia na jedną funkcję
	void refreshSonOffSwitch01();//TODO: do przerobienia na jedną funkcję

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

	uint8_t lastKitchenSw1State; //TODO: do przerobienia tablicę
	uint8_t lastKitchenSw2State; //TODO: do przerobienia tablicę
	uint8_t lastSw1State; //TODO: do przerobienia tablicę
	uint8_t lastSw2State; //TODO: do przerobienia tablicę
	uint8_t lastSw3State; //TODO: do przerobienia tablicę
	uint8_t lastSonOffSwitch01; //TODO: do przerobienia tablicę
 
};
extern DisplayControler displayControler;
#endif /* #ifndef __DisplayControler_H__ */
