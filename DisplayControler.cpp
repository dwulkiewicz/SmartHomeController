/************************************************************************/
/*                                                                      */
/*              Project:  SmartHomeControler							*/
/*              Hardware: ESP32, Nextion 7.0, DS1307, BME280            */
/*                                                                      */
/*              Author: Dariusz Wulkiewicz                              */
/*                      d.wulkiewicz@gmail.com                          */
/*                                                                      */
/*              Date: 01.2019                                           */
/************************************************************************/

#include <WiFiType.h>

#include "DisplayControler.h"
#include "LightsControler.h"
#include "Constants.h"
#include "Configuration.h"
#include "RtcControler.h"
#include "SensorsHelper.h"
#include "EventsHandler.h"

//------------/*Strona główna*/------------//
NexPage pgMain = NexPage(PG_MAIN_ID, 0, PG_MAIN_NAME);

NexPicture pDayOfMonth1 = NexPicture(PG_MAIN_ID, OBJ_DAY_OF_MONTH1_ID, OBJ_DAY_OF_MONTH1_NAME);
NexPicture pDayOfMonth2 = NexPicture(PG_MAIN_ID, OBJ_DAY_OF_MONTH2_ID, OBJ_DAY_OF_MONTH2_NAME);
NexPicture pMonth = NexPicture(PG_MAIN_ID, OBJ_MONTH_ID, OBJ_MONTH_NAME);
NexPicture pDayOfWeek = NexPicture(PG_MAIN_ID, OBJ_DAY_OF_WEEK_ID, OBJ_DAY_OF_WEEK_NAME);

NexText tTime1 = NexText(PG_MAIN_ID, OBJ_TIME1_ID, OBJ_TIME1_NAME);
NexText tTime2 = NexText(PG_MAIN_ID, OBJ_TIME2_ID, OBJ_TIME2_NAME);
NexText tTime3 = NexText(PG_MAIN_ID, OBJ_TIME3_ID, OBJ_TIME3_NAME);
NexText tTime4 = NexText(PG_MAIN_ID, OBJ_TIME4_ID, OBJ_TIME4_NAME);

NexText tIndoorTemp1 = NexText(PG_MAIN_ID, OBJ_INDOOR_TEMP1_ID, OBJ_INDOOR_TEMP1_NAME);
NexText tIndoorTemp2 = NexText(PG_MAIN_ID, OBJ_INDOOR_TEMP2_ID, OBJ_INDOOR_TEMP2_NAME);
NexText tIndoorHumidity = NexText(PG_MAIN_ID, OBJ_INDOOR_HUMIDITY_ID, OBJ_INDOOR_HUMIDITY_NAME);

NexText tOutdoorTemp1 = NexText(PG_MAIN_ID, OBJ_OUTDOOR_TEMP1_ID, OBJ_OUTDOOR_TEMP1_NAME);
NexText tOutdoorTemp2 = NexText(PG_MAIN_ID, OBJ_OUTDOOR_TEMP2_ID, OBJ_OUTDOOR_TEMP2_NAME);
NexText tOutdoorTempSymbol = NexText(PG_MAIN_ID, OBJ_OUTDOOR_TEMP_SYMBOL_ID, OBJ_OUTDOOR_TEMP_SYMBOL_NAME);
NexText tOutdoorHumidity = NexText(PG_MAIN_ID, OBJ_OUTDOOR_HUMIDITY_ID, OBJ_OUTDOOR_HUMIDITY_NAME);
NexText tOutdoorPreasure = NexText(PG_MAIN_ID, OBJ_OUTDOOR_PREASURE_ID, OBJ_OUTDOOR_PREASURE_NAME);

NexPicture objWiFiStatus = NexPicture(PG_MAIN_ID, OBJ_WIFI_STATUS_ID, OBJ_WIFI_STATUS_NAME);
NexPicture objBathSw1 = NexPicture(PG_MAIN_ID, OBJ_BATH_SW_1_ID, OBJ_BATH_SW_1_NAME);
NexPicture objBathSw2 = NexPicture(PG_MAIN_ID, OBJ_BATH_SW_2_ID, OBJ_BATH_SW_2_NAME);
NexPicture objBathSw3 = NexPicture(PG_MAIN_ID, OBJ_BATH_SW_3_ID, OBJ_BATH_SW_3_NAME);

//------------/*Ogrzewanie*/------------
NexPage pgHeating = NexPage(PG_HEATING_ID, 0, PG_HEATING_NAME);
/*Temperatura w dzień*/
NexText lblDayTempValue = NexText(PG_HEATING_ID, OBJ_DAY_TEMP_VALUE_ID, OBJ_DAY_TEMP_VALUE_NAME);
NexButton btnDayTempDec = NexButton(PG_HEATING_ID, OBJ_DAY_TEMP_DEC_ID, OBJ_DAY_TEMP_DEC_NAME);
NexButton btnDayTempInc = NexButton(PG_HEATING_ID, OBJ_DAY_TEMP_INC_ID, OBJ_DAY_TEMP_INC_NAME);
/*Temperatura w noc*/
NexText lblNightTempValue = NexText(PG_HEATING_ID, OBJ_NIGHT_TEMP_VALUE_ID, OBJ_NIGHT_TEMP_VALUE_NAME);
NexButton btnNightTempDec = NexButton(PG_HEATING_ID, OBJ_NIGHT_TEMP_DEC_ID, OBJ_NIGHT_TEMP_DEC_NAME);
NexButton btnNightTempInc = NexButton(PG_HEATING_ID, OBJ_NIGHT_TEMP_INC_ID, OBJ_NIGHT_TEMP_INC_NAME);

//------------/*Oświetlenie*/------------
NexPage pgLights = NexPage(PG_LIGHTS_ID, 0, PG_LIGHTS_NAME);
NexSlider sldMain1(PG_LIGHTS_ID, OBJ_LIGHT_BATH_MAIN_1_ID, OBJ_LIGHT_BATH_MAIN_1_NAME);
NexSlider sldAdd1(PG_LIGHTS_ID, OBJ_LIGHT_BATH_ADD_1_ID, OBJ_LIGHT_BATH_ADD_1_NAME);
NexSlider sldTape1(PG_LIGHTS_ID, OBJ_LIGHT_BATH_TAPE_1_ID, OBJ_LIGHT_BATH_TAPE_1_NAME);
NexSlider sldRgbV1(PG_LIGHTS_ID, OBJ_LIGHT_BATH_RGB_V_1_ID, OBJ_LIGHT_BATH_RGB_V_1_NAME);
NexSlider sldRgbH1(PG_LIGHTS_ID, OBJ_LIGHT_BATH_RGB_H_1_ID, OBJ_LIGHT_BATH_RGB_H_1_NAME);

//------------/*Czas i inne*/------------
NexPage pgOther = NexPage(PG_OTHER_ID, 0, PG_OTHER_NAME);
/*Ustawianie czasu*/
NexText tYear = NexText(PG_OTHER_ID, OBJ_TIME_YEAR_ID, OBJ_TIME_YEAR_NAME);
NexText tMonth = NexText(PG_OTHER_ID, OBJ_TIME_MONTH_ID, OBJ_TIME_MONTH_NAME);
NexText tDay = NexText(PG_OTHER_ID, OBJ_TIME_DAY_ID, OBJ_TIME_DAY_NAME);
NexText tDayW = NexText(PG_OTHER_ID, OBJ_TIME_DAY_OF_WEEK_ID, OBJ_TIME_DAY_OF_WEEK_NAME);
NexText tHour = NexText(PG_OTHER_ID, OBJ_TIME_HOUR_ID, OBJ_TIME_HOUR_NAME);
NexText tMinute = NexText(PG_OTHER_ID, OBJ_TIME_MINUTE_ID, OBJ_TIME_MINUTE_NAME);
NexButton bDateTimeNext = NexButton(PG_OTHER_ID, OBJ_TIME_NEXT_ID, OBJ_TIME_NEXT_NAME);
NexButton bDateTimeSet = NexButton(PG_OTHER_ID, OBJ_TIME_SET_ID, OBJ_TIME_SET_NAME);

/*
  Register a button object to the touch event list.
*/
NexTouch *nex_listen_list[] =
{
  &pgMain,
  &pgHeating,
  &pgLights,
  &pgOther,
  &objBathSw1,
  &objBathSw2,
  &objBathSw3,
  &btnDayTempInc,
  &btnDayTempDec,
  &btnNightTempInc,
  &btnNightTempDec,
  &bDateTimeNext,
  &bDateTimeSet,
  &sldMain1,
  &sldAdd1,
  &sldTape1,
  &sldRgbV1,
  &sldRgbH1,
  NULL
};
//----------------------------------------------------------------------------------------
void onPageShow(void *ptr)
{
	NexPage *page = (NexPage *)ptr;
	Serial.printf("onPageShow() %s\r\n", page->getObjName());
	displayControler.currentPage = page->getObjPid();

	switch (displayControler.currentPage) {
	case PG_MAIN_ID:
		displayControler.refresMainPage();
		break;
	case PG_HEATING_ID:
		displayControler.refreshHeatingPage();
		break;
	case PG_LIGHTS_ID:
		displayControler.refreshLightsPage();
		break;
	case PG_OTHER_ID:
		displayControler.refreshOtherPage();
		break;
	}
}
//----------------------------------------------------------------------------------------
void onBathSwPush(void *ptr)
{
	NexPicture *obj = (NexPicture *)ptr;
	Serial.printf("onBathSwPush() pageId: %d objId: %d objName: %s\r\n", obj->getObjPid(), obj->getObjCid(), obj->getObjName());
	uint8_t switchId;
	uint8_t switchState;
	switch (obj->getObjCid()) {
	case OBJ_BATH_SW_1_ID:
		switchId = SWITCH_BATH_1_ID;
		switchState	= !displayControler.sw1State;
		break;
	case OBJ_BATH_SW_2_ID:
		switchId = SWITCH_BATH_2_ID;
		switchState = !displayControler.sw2State;
		break;
	case OBJ_BATH_SW_3_ID:
		switchId = SWITCH_BATH_3_ID;
		switchState = !displayControler.sw3State;
		break;
	default:
		return; //nieznany komponent
	}
	eventsHandler.onSwitchChange(switchId, switchState);
}
//----------------------------------------------------------------------------------------
void DisplayControler::onSwitchChanged(uint8_t switchId, uint8_t switchState) {
	Serial.printf("DisplayControler::onSwitchChanged() id: %d state: %d\r\n", switchId, switchState);
	switch (switchId) {
	case SWITCH_BATH_1_ID:
		sw1State = switchState;
		refreshBathSw1();
		break;
	case SWITCH_BATH_2_ID:
		sw2State = switchState;
		refreshBathSw2();
		break;
	case SWITCH_BATH_3_ID:
		sw3State = switchState;
		refreshBathSw3();
		break;
	}
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshBathSw1(void) {
	if (currentPage != PG_MAIN_ID)
		return;
	Serial.printf("DisplayControler::refreshBathSw1()\r\n");
	if (lastSw1State != sw1State) {
		if (currentPage == PG_MAIN_ID){
      objBathSw1.setPic(sw1State == SW_ON ? PICTURE_SWITCH_ON : PICTURE_SWITCH_OFF);//sprawdzić czy ta operacja zwraca coś po poprawnym wykonaniu    
			lastSw1State = sw1State;
		}
	}
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshBathSw2(void) {
	if (currentPage != PG_MAIN_ID)
		return;
	Serial.printf("DisplayControler::refreshBathSw2()\r\n");
	if (lastSw2State != sw2State) {
		if (currentPage == PG_MAIN_ID){
      objBathSw2.setPic(sw2State == SW_ON ? PICTURE_SWITCH_ON : PICTURE_SWITCH_OFF);//sprawdzić czy ta operacja zwraca coś po poprawnym wykonaniu    
			lastSw2State = sw2State;
		}
	}
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshBathSw3(void) {
	if (currentPage != PG_MAIN_ID)
		return;
	Serial.printf("DisplayControler::refreshBathSw3()\r\n");
	if (lastSw3State != sw3State) {
		if (currentPage == PG_MAIN_ID){
      objBathSw3.setPic(sw3State == SW_ON ? PICTURE_SWITCH_ON : PICTURE_SWITCH_OFF);//sprawdzić czy ta operacja zwraca coś po poprawnym wykonaniu 
			lastSw3State = sw3State;
		}
	}
}
//----------------------------------------------------------------------------------------
void onBtnTempPush(void *ptr) {
	NexButton *btn = (NexButton *)ptr;
	Serial.printf("onBtnTempPush: pageId=%d cmponentId=%d name=%s\r\n", btn->getObjPid(), btn->getObjCid(), btn->getObjName());

	switch (btn->getObjCid()) {
	case OBJ_DAY_TEMP_INC_ID:
		if (configuration.incrementDayTemperature()) {
			lblDayTempValue.setText(Configuration::temperatureAsString(configuration.getDayTemperature()).c_str());
		}
		break;
	case OBJ_DAY_TEMP_DEC_ID:
		if (configuration.decrementDayTemperature()) {
			lblDayTempValue.setText(Configuration::temperatureAsString(configuration.getDayTemperature()).c_str());
		}
		break;
	case OBJ_NIGHT_TEMP_INC_ID:
		if (configuration.incrementNightTemperature()) {
			lblNightTempValue.setText(Configuration::temperatureAsString(configuration.getNightTemperature()).c_str());
		}
		break;
	case OBJ_NIGHT_TEMP_DEC_ID:
		if (configuration.decrementNightTemperature()) {
			lblNightTempValue.setText(Configuration::temperatureAsString(configuration.getNightTemperature()).c_str());
		}
		break;
	}
}
//----------------------------------------------------------------------------------------
void DisplayControler::refresMainPage()
{
	//TODO: 
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshHeatingPage()
{
	lblDayTempValue.setText(Configuration::temperatureAsString(configuration.getDayTemperature()).c_str());
	lblNightTempValue.setText(Configuration::temperatureAsString(configuration.getNightTemperature()).c_str());
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshLightsPage()
{
	sldMain1.setValue(lightsControler.bathroomMainLight.getBrightness());
	sldAdd1.setValue(lightsControler.bathroomAdditionalLight.getBrightness());
	sldTape1.setValue(lightsControler.bathroomTapeLight.getBrightness());
	sldRgbV1.setValue(lightsControler.bathroomRGBTapeLight.getBrightness());
	sldRgbH1.setValue(lightsControler.bathroomRGBTapeLight.getHue());
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshOtherPage()
{
	currentTimeComponent = SETUP_DATETIME_YEAR;

	char buf[5];
	sprintf(buf, "%02d", rtc.year());
	tYear.setText(buf);
	sprintf(buf, "%02d", rtc.month());
	tMonth.setText(buf);
	sprintf(buf, "%02d", rtc.day());
	tDay.setText(buf);
	sprintf(buf, "%02d", rtc.hour());
	tHour.setText(buf);
	sprintf(buf, "%02d", rtc.minute());
	tMinute.setText(buf);
	RtcControler::dayOfWeekName(buf, rtc.dayOfWeek());
	tDayW.setText(buf);
}
//----------------------------------------------------------------------------------------
void onBtnbDateTimeNextPush(void *ptr)
{
	NexButton *btn = (NexButton *)ptr;
	Serial.printf("onBtnbDateTimeNextPush: %s\r\n", btn->getObjName());

	switch (displayControler.currentTimeComponent) {
	case SETUP_DATETIME_YEAR:
		tYear.Set_font_color_pco(COLOR_YELLOW);
		tMonth.Set_font_color_pco(COLOR_RED);
		displayControler.currentTimeComponent = SETUP_DATETIME_MONTH;
		break;
	case SETUP_DATETIME_MONTH:
		tMonth.Set_font_color_pco(COLOR_YELLOW);
		tDay.Set_font_color_pco(COLOR_RED);
		displayControler.currentTimeComponent = SETUP_DATETIME_DAY;
		break;
	case SETUP_DATETIME_DAY:
		tDay.Set_font_color_pco(COLOR_YELLOW);
		tDayW.Set_font_color_pco(COLOR_RED);
		displayControler.currentTimeComponent = SETUP_DATETIME_DAY_OF_WEEK;
		break;
	case SETUP_DATETIME_DAY_OF_WEEK:
		tDayW.Set_font_color_pco(COLOR_YELLOW);
		tHour.Set_font_color_pco(COLOR_RED);
		displayControler.currentTimeComponent = SETUP_DATETIME_HOUR;
		break;
	case SETUP_DATETIME_HOUR:
		tHour.Set_font_color_pco(COLOR_YELLOW);
		tMinute.Set_font_color_pco(COLOR_RED);
		displayControler.currentTimeComponent = SETUP_DATETIME_MINUTE;
		break;
	case SETUP_DATETIME_MINUTE:
		tMinute.Set_font_color_pco(COLOR_YELLOW);
		tYear.Set_font_color_pco(COLOR_RED);
		displayControler.currentTimeComponent = SETUP_DATETIME_YEAR;
		break;
	default:
		tYear.Set_font_color_pco(COLOR_RED);
		displayControler.currentTimeComponent = SETUP_DATETIME_YEAR;
		break;
	}
}
//----------------------------------------------------------------------------------------
void onBtnbDateTimeSetPush(void *ptr)
{
	NexButton *btn = (NexButton *)ptr;
	Serial.printf("onBtnbDateTimeSetPush: %s\r\n", btn->getObjName());

	rtc.refresh();
	char buf[10];
	if (displayControler.currentTimeComponent == SETUP_DATETIME_YEAR) {
		uint8_t year = rtc.year();
		if (year == 99)
			year = 0;
		else
			year++;

		rtc.set(rtc.second(), rtc.minute(), rtc.hour(), rtc.dayOfWeek(), rtc.day(), rtc.month(), year);

		sprintf(buf, "%02d", year);
		tYear.setText(buf);
	}
	if (displayControler.currentTimeComponent == SETUP_DATETIME_MONTH) {
		uint8_t month = rtc.month();
		if (month == 12)
			month = 1;
		else
			month++;

		rtc.set(rtc.second(), rtc.minute(), rtc.hour(), rtc.dayOfWeek(), rtc.day(), month, rtc.year());

		sprintf(buf, "%02d", month);
		tMonth.setText(buf);
	}
	if (displayControler.currentTimeComponent == SETUP_DATETIME_DAY) {
		uint8_t day = rtc.day();
		if (day == 31)
			day = 1;
		else
			day++;

		rtc.set(rtc.second(), rtc.minute(), rtc.hour(), rtc.dayOfWeek(), day, rtc.month(), rtc.year());

		sprintf(buf, "%02d", day);
		tDay.setText(buf);
	}
	if (displayControler.currentTimeComponent == SETUP_DATETIME_DAY_OF_WEEK) {
		uint8_t dayOfWeek = rtc.dayOfWeek();
		if (dayOfWeek == 7)
			dayOfWeek = 1;
		else
			dayOfWeek++;

		rtc.set(rtc.second(), rtc.minute(), rtc.hour(), dayOfWeek, rtc.day(), rtc.month(), rtc.year());

		RtcControler::dayOfWeekName(buf, dayOfWeek);
		tDayW.setText(buf);
	}
	if (displayControler.currentTimeComponent == SETUP_DATETIME_HOUR) {
		uint8_t hour = rtc.hour();
		if (hour == 23)
			hour = 0;
		else
			hour++;

		rtc.set(rtc.second(), rtc.minute(), hour, rtc.dayOfWeek(), rtc.day(), rtc.month(), rtc.year());

		sprintf(buf, "%02d", hour);
		tHour.setText(buf);
	}
	if (displayControler.currentTimeComponent == SETUP_DATETIME_MINUTE) {
		uint8_t minute = rtc.minute();
		if (minute == 59)
			minute = 0;
		else
			minute++;

		rtc.set(0, minute, rtc.hour(), rtc.dayOfWeek(), rtc.day(), rtc.month(), rtc.year());

		sprintf(buf, "%02d", minute);
		tMinute.setText(buf);
	}
}
//----------------------------------------------------------------------------------------
void DisplayControler::onRefreshDateTime(const TDateTime& dateTime) {
	currDateTime = dateTime;
	//i tak się nie uda przesłać danych do Nextion jeżeli nie jest obecnie aktywna głowna strona
	if (currentPage != PG_MAIN_ID)
		return;
	/*Hour*/
	if (screenDateTime.hour != currDateTime.hour) {
		if (currentPage == PG_MAIN_ID && tTime1.setText(String(currDateTime.hour / 10).c_str()) && tTime2.setText(String(currDateTime.hour % 10).c_str()))
			screenDateTime.hour = currDateTime.hour;
	}
	/*Minute*/
	if (screenDateTime.minute != currDateTime.minute) {
		if (currentPage == PG_MAIN_ID && tTime3.setText(String(currDateTime.minute / 10).c_str()) && tTime4.setText(String(currDateTime.minute % 10).c_str()))
			screenDateTime.minute = currDateTime.minute;
	}
	/*Month*/
	if (screenDateTime.month != currDateTime.month) {
		if (currentPage == PG_MAIN_ID && pMonth.setPic(monthPic(currDateTime.month)))
			screenDateTime.month = currDateTime.month;
	}
	/*Day of month*/
	if (screenDateTime.day != currDateTime.day) {
		if (currentPage == PG_MAIN_ID && pDayOfMonth1.setPic(dayOfMonthPic(currDateTime.day / 10)) && pDayOfMonth2.setPic(dayOfMonthPic(currDateTime.day % 10)))
			screenDateTime.day = currDateTime.day;
	}
	/*Day of week*/
	if (screenDateTime.dayOfWeek != currDateTime.dayOfWeek) {
		if (currentPage == PG_MAIN_ID && pDayOfWeek.setPic(dayOfWeekPic(currDateTime.dayOfWeek)))
			screenDateTime.dayOfWeek = currDateTime.dayOfWeek;
	}
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshIndoorTemperature() {
	if (currentPage != PG_MAIN_ID)
		return;
	char buf[10];
	/*Indoor temperature*/
	uint16_t t = SensorsHelper::getTemperature();
	uint8_t t1 = t / 10;
	uint8_t t2 = t % 10;
	if (lastTemp1 != t1) {
		sprintf(buf, "%02d", t1);
		if (currentPage == PG_MAIN_ID && tIndoorTemp1.setText(buf))
			lastTemp1 = t1;
	}
	if (lastTemp2 != t2) {
		sprintf(buf, "%02d", t2);
		if (currentPage == PG_MAIN_ID && tIndoorTemp2.setText(buf))
			lastTemp2 = t2;
	}
	/*Indoor humanidity*/
	uint16_t h = SensorsHelper::getHumidity();
	if (lastHumidity != h) {
		sprintf(buf, "%02d%%", h);
		if (currentPage == PG_MAIN_ID && tIndoorHumidity.setText(buf))
			lastHumidity = h;
	}
	/*Indoor preasure*/
	uint16_t p = SensorsHelper::getPreasure();
}
//----------------------------------------------------------------------------------------
void DisplayControler::onRefreshOutdoorTemperature(float outdoorTemp) {
  currOutdoorTemp = outdoorTemp;
}
//----------------------------------------------------------------------------------------
void DisplayControler::onRefreshOutdoorHumidity(float outdoorHumidity) {
  currOutdoorHumidity = outdoorHumidity;
}
//----------------------------------------------------------------------------------------
void DisplayControler::onRefreshOutdoorPreasure(float outdoorPressure) {
  currOutdoorPressure = outdoorPressure;
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshOutdoorTemperature() {  
  if (currentPage == PG_MAIN_ID && screenOutdoorTemp != currOutdoorTemp){
    Serial.printf("DisplayControler::refreshOutdoorTemperature() %.1f\r\n", currOutdoorTemp); 	
    bool ret1 = false;
    bool ret2 = false;
    bool ret3 = false;    
  	int outdoorTempInt = abs(currOutdoorTemp * 10);  
  	//poniżej -10.0
  	if (currOutdoorTemp <= -10.0){
  		ret1 = tOutdoorTempSymbol.setText("-");
  		ret2 = tOutdoorTemp1.setText(String(outdoorTempInt / 10).c_str());
  	}
  	//od -9.9 do -0.1
  	else if (currOutdoorTemp >= -9.9 & currOutdoorTemp <= -0.1){
  		ret1 = tOutdoorTemp1.setText(String(-outdoorTempInt / 10).c_str());
  		ret2 = tOutdoorTempSymbol.setText("");
  	}
  	//od 0 w górę
  	else{
  		ret1 = tOutdoorTemp1.setText(String(outdoorTempInt / 10).c_str());
  		ret2 = tOutdoorTempSymbol.setText("");
  	}
  	ret3 = tOutdoorTemp2.setText(String(outdoorTempInt % 10).c_str());
    if (ret1 && ret2 && ret3){
      screenOutdoorTemp = currOutdoorTemp;    
    }
  }
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshOutdoorHumidity() {
  if (currentPage == PG_MAIN_ID && screenOutdoorHumidity != currOutdoorHumidity)
  {
      Serial.printf("DisplayControler::refreshOutdoorHumidity() %.1f\r\n", currOutdoorHumidity);
      int ih = round(currOutdoorHumidity);
      char buf[10];
      sprintf(buf, "%02d%%", ih);
      if (tOutdoorHumidity.setText(buf))
        screenOutdoorHumidity = currOutdoorHumidity;   
  }
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshOutdoorPreasure() {
  if (currentPage == PG_MAIN_ID && screenOutdoorPressure != currOutdoorPressure){
	Serial.printf("DisplayControler::onRefreshOutdoorPreasure() %.1f\r\n", currOutdoorPressure);
	int ip = round(currOutdoorPressure);
	char buf[10];
	sprintf(buf, "%02dhPa", ip);
	if(tOutdoorPreasure.setText(buf))
    screenOutdoorPressure = currOutdoorPressure;
  }
}
//----------------------------------------------------------------------------------------
void DisplayControler::showWiFiStatus(int8_t status) {
	switch (status) {
	case WL_CONNECTED:
		objWiFiStatus.setPic(PICTURE_WIFI_ON);
		break;
	default: //WL_IDLE_STATUS, WL_NO_SSID_AVAIL, WL_SCAN_COMPLETED, WL_CONNECT_FAILED, WL_CONNECTION_LOST, WL_DISCONNECTED  
		objWiFiStatus.setPic(PICTURE_WIFI_OFF);
		break;
	}
}
//----------------------------------------------------------------------------------------
void DisplayControler::showMQTTConnected(bool connected) {
	//TODO: docelowo zmienić na inną ikonkę na panelu 
	if (connected)
		objWiFiStatus.setPic(PICTURE_WIFI_ON);
	else
		objWiFiStatus.setPic(PICTURE_WIFI_OFF);
}
//----------------------------------------------------------------------------------------
void onSliderLightPop(void *ptr)
{
  //TODO: poprawić zapamiętywanie w EEPROM  !!!!  
	NexSlider *obj = (NexSlider *)ptr;
  Serial.printf("onSliderLightPop() pageId: %d objId: %d objName: %s\r\n", obj->getObjPid(), obj->getObjCid(), obj->getObjName());
	uint32_t value;
	bool ret = obj->getValue(&value);
	if (ret) {
		switch (obj->getObjCid()) {
		case OBJ_LIGHT_BATH_MAIN_1_ID:
			lightsControler.bathroomMainLight.setBrightness(value);
      configuration.setParam(0,value);
			break;
		case OBJ_LIGHT_BATH_ADD_1_ID:
			lightsControler.bathroomAdditionalLight.setBrightness(value);
      configuration.setParam(1,value);     
			break;
		case OBJ_LIGHT_BATH_TAPE_1_ID:
			lightsControler.bathroomTapeLight.setBrightness(value);
      configuration.setParam(2,value);     
			break;
		case OBJ_LIGHT_BATH_RGB_V_1_ID:
			lightsControler.bathroomRGBTapeLight.setBrightness(value);
      configuration.setParam(3,value);      
			break;
		case OBJ_LIGHT_BATH_RGB_H_1_ID:
			lightsControler.bathroomRGBTapeLight.setHue(value);
      configuration.setParam(4,value);     
			break;
		}   
	}
}
//----------------------------------------------------------------------------------------
DisplayControler::DisplayControler()
{
	currentPage = PG_MAIN_ID; //zakładam, że po restarcie systemu aktywna jest główna strona, w razie czego można dodać komendę oczytująca bieżącą stronę
	
	screenDateTime.minute = 99;
	screenDateTime.hour = 99;
	screenDateTime.day = 99;
	screenDateTime.month = 99;
	screenDateTime.dayOfWeek = 99;

	currDateTime.minute = 99;
	currDateTime.hour = 99;
	currDateTime.day = 99;
	currDateTime.month = 99;
	currDateTime.dayOfWeek = 99;

  screenOutdoorTemp = 99;
  screenOutdoorHumidity = 0;
  screenOutdoorPressure = 0;  
  currOutdoorTemp = 99;
  currOutdoorHumidity = 0;
  currOutdoorPressure = 0;  

	uint8_t lastSw1State = SW_ON;
	uint8_t lastSw2State = SW_ON;
	uint8_t lastSw3State = SW_ON;

	uint8_t sw1State = SW_OFF;//dzięki temu po restarcie wyłączy switch
	uint8_t sw2State = SW_OFF;//dzięki temu po restarcie wyłączy switch
	uint8_t sw3State = SW_OFF;//dzięki temu po restarcie wyłączy switch
}
//----------------------------------------------------------------------------------------
void DisplayControler::init() {
	/* Set the baudrate which is for debug and communicate with Nextion screen. */
	nexInit(115200, UART1_BAUND, SERIAL_8N1, UART1_RX, UART1_TX);

	/* Register the pop event callback function of the current button component. */
	pgMain.attachOnShow(onPageShow, &pgMain);
	pgHeating.attachOnShow(onPageShow, &pgHeating);
	pgLights.attachOnShow(onPageShow, &pgLights);
	pgOther.attachOnShow(onPageShow, &pgOther);

	objBathSw1.attachPush(onBathSwPush, &objBathSw1);
	objBathSw2.attachPush(onBathSwPush, &objBathSw2);
	objBathSw3.attachPush(onBathSwPush, &objBathSw3);

	btnDayTempInc.attachPush(onBtnTempPush, &btnDayTempInc);
	btnDayTempDec.attachPush(onBtnTempPush, &btnDayTempDec);

	btnNightTempInc.attachPush(onBtnTempPush, &btnNightTempInc);
	btnNightTempDec.attachPush(onBtnTempPush, &btnNightTempDec);

	bDateTimeNext.attachPush(onBtnbDateTimeNextPush, &bDateTimeNext);
	bDateTimeSet.attachPush(onBtnbDateTimeSetPush, &bDateTimeSet);

	sldMain1.attachPop(onSliderLightPop, &sldMain1);
	sldAdd1.attachPop(onSliderLightPop, &sldAdd1);
	sldTape1.attachPop(onSliderLightPop, &sldTape1);
	sldRgbV1.attachPop(onSliderLightPop, &sldRgbV1);
	sldRgbH1.attachPop(onSliderLightPop, &sldRgbH1);
}
//----------------------------------------------------------------------------------------
uint8_t DisplayControler::dayOfWeekPic(uint8_t dayOfWeek) {
	switch (dayOfWeek) {
	case 1: return PIC_DAY_MON;
	case 2: return PIC_DAY_TUE;
	case 3: return PIC_DAY_WED;
	case 4: return PIC_DAY_THU;
	case 5: return PIC_DAY_FRI;
	case 6: return PIC_DAY_SAT;
	case 7: return PIC_DAY_SUN;
	default: return PIC_DAY_SUN; //TODO: dodać inną ikonę dla błędu
	}
}
//----------------------------------------------------------------------------------------
uint8_t DisplayControler::monthPic(uint8_t month) {
	switch (month) {
	case 1: return PIC_MONTH_JAN;
	case 2: return PIC_MONTH_FEB;
	case 3: return PIC_MONTH_MAR;
	case 4: return PIC_MONTH_APR;
	case 5: return PIC_MONTH_MAY;
	case 6: return PIC_MONTH_JUN;
	case 7: return PIC_MONTH_JUL;
	case 8: return PIC_MONTH_AUG;
	case 9: return PIC_MONTH_SEPT;
	case 10: return PIC_MONTH_OCT;
	case 11: return PIC_MONTH_NOV;
	case 12: return PIC_MONTH_DEC;
	default: return PIC_MONTH_DEC; //TODO: dodać inną ikonę dla błędu
	}
}
//----------------------------------------------------------------------------------------
uint8_t DisplayControler::dayOfMonthPic(uint8_t digit) {
	switch (digit) {
	case 0: return PIC_DIGIT_0;
	case 1: return PIC_DIGIT_1;
	case 2: return PIC_DIGIT_2;
	case 3: return PIC_DIGIT_3;
	case 4: return PIC_DIGIT_4;
	case 5: return PIC_DIGIT_5;
	case 6: return PIC_DIGIT_6;
	case 7: return PIC_DIGIT_7;
	case 8: return PIC_DIGIT_8;
	case 9: return PIC_DIGIT_9;
	default: return PIC_DIGIT_0; //TODO: dodać inną ikonę dla błędu
	}
}
//---------------------------------------------------------------------------------------- 
void DisplayControler::loop() {
	nexLoop(nex_listen_list);

	//TODO: do przeniesienia do innej pętli
	if (sw1State != lastSw1State)
		refreshBathSw1();
	if (sw2State != lastSw2State)
		refreshBathSw2();
	if (sw3State != lastSw3State)
		refreshBathSw3();
  if (screenOutdoorTemp != currOutdoorTemp)
    refreshOutdoorTemperature();
  if (screenOutdoorHumidity != currOutdoorHumidity)
    refreshOutdoorHumidity();
  if (screenOutdoorPressure != currOutdoorPressure)
    refreshOutdoorPreasure();        
   
}

DisplayControler displayControler;
