#ifndef __DisplayControler_H__
#define __DisplayControler_H__

#include <Arduino.h>
#include <Nextion.h>
#include "RtcControler.h"

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
	void onRefreshDateTime(const TDateTime& dateTime);
  void onRefreshOutdoorTemperature(float outdoorTemp);
  void onRefreshOutdoorHumidity(float outdoorHumidity);
  void onRefreshOutdoorPreasure(float outdoorPressure);
  void onSwitchChanged(uint8_t switchId, uint8_t switchState);
  void refreshHeatingStatus(uint8_t heatingStatus);
  
	void refreshIndoorTemperature();
	void refresMainPage();
	void refreshHeatingPage();
	void refreshLightsPage();
	void refreshOtherPage();

	void showWiFiStatus(int8_t status);
	void showMQTTConnected(bool connected);

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
private:
	void refreshBathSw1(void);
	void refreshBathSw2(void);
	void refreshBathSw3(void);
  void refreshOutdoorTemperature(void);
  void refreshOutdoorHumidity(void);
  void refreshOutdoorPreasure(void);
private:
  //TODO: zmienne przenieść do struktury
	//Time
	TDateTime screenDateTime;
	TDateTime currDateTime;
	//Indoor sensor
	uint8_t lastTemp1 = 255;
	uint8_t lastTemp2 = 255;
	uint16_t lastHumidity = 999;
  //Outdoor sensor
  float screenOutdoorTemp;
  float screenOutdoorHumidity;
  float screenOutdoorPressure;  
  float currOutdoorTemp;
  float currOutdoorHumidity;
  float currOutdoorPressure;    
  //Heating
  uint8_t currHeatingStatus;
  uint8_t screenHeatingStatus;
  //Switch
	uint8_t lastSw1State;
	uint8_t lastSw2State;
	uint8_t lastSw3State;
};
extern DisplayControler displayControler;
#endif /* #ifndef __DisplayControler_H__ */
