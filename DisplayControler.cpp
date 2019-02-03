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
#include <PubSubClient.h>

#include "DisplayControler.h"
#include "LightsControler.h"
#include "Constants.h"
#include "Configuration.h"
#include "RtcControler.h"
#include "SensorsHelper.h"
#include "EventDispatcher.h"

//------------/*Strona główna*/------------//
NexPage pgMain = NexPage(PG_MAIN_ID, 0, PG_MAIN_NAME);
//Status WiFI
NexPicture objWiFiStatus(PG_MAIN_ID, OBJ_WIFI_STATUS_ID, OBJ_WIFI_STATUS_NAME);
//Status ogrzewania
NexPicture objHeatingStatus(PG_MAIN_ID, OBJ_HEATING_STATUS_ID, OBJ_HEATING_STATUS_NAME);
NexPicture objHeatingPeriod(PG_MAIN_ID, OBJ_HEATING_PERIOD_ID, OBJ_HEATING_PERIOD_NAME);
NexText objHeatingSetTemp(PG_MAIN_ID, OBJ_HEATING_SET_TEMP_ID, OBJ_HEATING_SET_TEMP_NAME);
//Data i czas
NexPicture pDayOfMonth1 = NexPicture(PG_MAIN_ID, OBJ_DAY_OF_MONTH1_ID, OBJ_DAY_OF_MONTH1_NAME);
NexPicture pDayOfMonth2 = NexPicture(PG_MAIN_ID, OBJ_DAY_OF_MONTH2_ID, OBJ_DAY_OF_MONTH2_NAME);
NexPicture pMonth = NexPicture(PG_MAIN_ID, OBJ_MONTH_ID, OBJ_MONTH_NAME);
NexPicture pDayOfWeek = NexPicture(PG_MAIN_ID, OBJ_DAY_OF_WEEK_ID, OBJ_DAY_OF_WEEK_NAME);
NexText tTime1 = NexText(PG_MAIN_ID, OBJ_TIME1_ID, OBJ_TIME1_NAME);
NexText tTime2 = NexText(PG_MAIN_ID, OBJ_TIME2_ID, OBJ_TIME2_NAME);
NexText tTime3 = NexText(PG_MAIN_ID, OBJ_TIME3_ID, OBJ_TIME3_NAME);
NexText tTime4 = NexText(PG_MAIN_ID, OBJ_TIME4_ID, OBJ_TIME4_NAME);
//Temperatura i wilgotność wewnątrz
NexText tIndoorTemp1 = NexText(PG_MAIN_ID, OBJ_INDOOR_TEMP1_ID, OBJ_INDOOR_TEMP1_NAME);
NexText tIndoorTemp2 = NexText(PG_MAIN_ID, OBJ_INDOOR_TEMP2_ID, OBJ_INDOOR_TEMP2_NAME);
NexText tIndoorHumidity = NexText(PG_MAIN_ID, OBJ_INDOOR_HUMIDITY_ID, OBJ_INDOOR_HUMIDITY_NAME);
//PArametry zewnętrzne
NexText tOutdoorTemp1 = NexText(PG_MAIN_ID, OBJ_OUTDOOR_TEMP1_ID, OBJ_OUTDOOR_TEMP1_NAME);
NexText tOutdoorTemp2 = NexText(PG_MAIN_ID, OBJ_OUTDOOR_TEMP2_ID, OBJ_OUTDOOR_TEMP2_NAME);
NexText tOutdoorTempSymbol = NexText(PG_MAIN_ID, OBJ_OUTDOOR_TEMP_SYMBOL_ID, OBJ_OUTDOOR_TEMP_SYMBOL_NAME);
NexText tOutdoorHumidity = NexText(PG_MAIN_ID, OBJ_OUTDOOR_HUMIDITY_ID, OBJ_OUTDOOR_HUMIDITY_NAME);
NexText tOutdoorPreasure = NexText(PG_MAIN_ID, OBJ_OUTDOOR_PREASURE_ID, OBJ_OUTDOOR_PREASURE_NAME);
//Zał/Wył
NexPicture objBathSw1 = NexPicture(PG_MAIN_ID, OBJ_BATH_SW_1_ID, OBJ_BATH_SW_1_NAME);
NexPicture objBathSw2 = NexPicture(PG_MAIN_ID, OBJ_BATH_SW_2_ID, OBJ_BATH_SW_2_NAME);
NexPicture objBathSw3 = NexPicture(PG_MAIN_ID, OBJ_BATH_SW_3_ID, OBJ_BATH_SW_3_NAME);

//------------/*Ogrzewanie*/------------
NexPage pgHeating = NexPage(PG_HEATING_ID, 0, PG_HEATING_NAME);
//Temperatura w dzień
NexText lblDayTempValue = NexText(PG_HEATING_ID, OBJ_DAY_TEMP_VALUE_ID, OBJ_DAY_TEMP_VALUE_NAME);
NexButton btnDayTempDec = NexButton(PG_HEATING_ID, OBJ_DAY_TEMP_DEC_ID, OBJ_DAY_TEMP_DEC_NAME);
NexButton btnDayTempInc = NexButton(PG_HEATING_ID, OBJ_DAY_TEMP_INC_ID, OBJ_DAY_TEMP_INC_NAME);
//Temperatura w noc
NexText lblNightTempValue = NexText(PG_HEATING_ID, OBJ_NIGHT_TEMP_VALUE_ID, OBJ_NIGHT_TEMP_VALUE_NAME);
NexButton btnNightTempDec = NexButton(PG_HEATING_ID, OBJ_NIGHT_TEMP_DEC_ID, OBJ_NIGHT_TEMP_DEC_NAME);
NexButton btnNightTempInc = NexButton(PG_HEATING_ID, OBJ_NIGHT_TEMP_INC_ID, OBJ_NIGHT_TEMP_INC_NAME);
//Włącz ogrzewanie rano w tygodniu
NexText objHeatingWorkingDaysMorningOnVal(PG_HEATING_ID, OBJ_HEATING_WORKING_DAYS_MORNING_ON_VAL_ID, OBJ_HEATING_WORKING_DAYS_MORNING_ON_VAL_NAME);
NexButton objHeatingWorkingDaysMorningOnDec(PG_HEATING_ID, OBJ_HEATING_WORKING_DAYS_MORNING_ON_DEC_ID, OBJ_HEATING_WORKING_DAYS_MORNING_ON_DEC_NAME);
NexButton objHeatingWorkingDaysMorningOnInc(PG_HEATING_ID, OBJ_HEATING_WORKING_DAYS_MORNING_ON_INC_ID, OBJ_HEATING_WORKING_DAYS_MORNING_ON_INC_NAME);
//Wyłącz ogrzewanie rano w tygodniu
NexText objHeatingWorkingDaysMorningOffVal(PG_HEATING_ID, OBJ_HEATING_WORKING_DAYS_MORNING_OFF_VAL_ID, OBJ_HEATING_WORKING_DAYS_MORNING_OFF_VAL_NAME);
NexButton objHeatingWorkingDaysMorningOffDec(PG_HEATING_ID, OBJ_HEATING_WORKING_DAYS_MORNING_OFF_DEC_ID, OBJ_HEATING_WORKING_DAYS_MORNING_OFF_DEC_NAME);
NexButton objHeatingWorkingDaysMorningOffInc(PG_HEATING_ID, OBJ_HEATING_WORKING_DAYS_MORNING_OFF_INC_ID, OBJ_HEATING_WORKING_DAYS_MORNING_OFF_INC_NAME);
//Włącz ogrzewanie popołudniu w tygodniu
NexText objHeatingWorkingDaysAfternoonOnVal(PG_HEATING_ID, OBJ_HEATING_WORKING_DAYS_AFTERNOON_ON_VAL_ID, OBJ_HEATING_WORKING_DAYS_AFTERNOON_ON_VAL_NAME);
NexButton objHeatingWorkingDaysAfternoonOnDec(PG_HEATING_ID, OBJ_HEATING_WORKING_DAYS_AFTERNOON_ON_DEC_ID, OBJ_HEATING_WORKING_DAYS_AFTERNOON_ON_DEC_NAME);
NexButton objHeatingWorkingDaysAfternoonOnInc(PG_HEATING_ID, OBJ_HEATING_WORKING_DAYS_AFTERNOON_ON_INC_ID, OBJ_HEATING_WORKING_DAYS_AFTERNOON_ON_INC_NAME);
//Wyłącz ogrzewanie popołudniu w tygodniu
NexText objHeatingWorkingDaysAfternoonOffVal(PG_HEATING_ID, OBJ_HEATING_WORKING_DAYS_AFTERNOON_OFF_VAL_ID, OBJ_HEATING_WORKING_DAYS_AFTERNOON_OFF_VAL_NAME);
NexButton objHeatingWorkingDaysAfternoonOffDec(PG_HEATING_ID, OBJ_HEATING_WORKING_DAYS_AFTERNOON_OFF_DEC_ID, OBJ_HEATING_WORKING_DAYS_AFTERNOON_OFF_DEC_NAME);
NexButton objHeatingWorkingDaysAfternoonOffInc(PG_HEATING_ID, OBJ_HEATING_WORKING_DAYS_AFTERNOON_OFF_INC_ID, OBJ_HEATING_WORKING_DAYS_AFTERNOON_OFF_INC_NAME);
//Włącz ogrzewanie rano w weekend
NexText objHeatingWeekendMorningOnVal(PG_HEATING_ID, OBJ_HEATING_WEEKEND_MORNING_ON_VAL_ID, OBJ_HEATING_WEEKEND_MORNING_ON_VAL_NAME);
NexButton objHeatingWeekendMorningOnDec(PG_HEATING_ID, OBJ_HEATING_WEEKEND_MORNING_ON_DEC_ID, OBJ_HEATING_WEEKEND_MORNING_ON_DEC_NAME);
NexButton objHeatingWeekendMorningOnInc(PG_HEATING_ID, OBJ_HEATING_WEEKEND_MORNING_ON_INC_ID, OBJ_HEATING_WEEKEND_MORNING_ON_INC_NAME);
//Wyłącz ogrzewanie rano w weekend
NexText objHeatingWeekendMorningOffVal(PG_HEATING_ID, OBJ_HEATING_WEEKEND_MORNING_OFF_VAL_ID, OBJ_HEATING_WEEKEND_MORNING_OFF_VAL_NAME);
NexButton objHeatingWeekendMorningOffDec(PG_HEATING_ID, OBJ_HEATING_WEEKEND_MORNING_OFF_DEC_ID, OBJ_HEATING_WEEKEND_MORNING_OFF_DEC_NAME);
NexButton objHeatingWeekendMorningOffInc(PG_HEATING_ID, OBJ_HEATING_WEEKEND_MORNING_OFF_INC_ID, OBJ_HEATING_WEEKEND_MORNING_OFF_INC_NAME);
//Włącz ogrzewanie popołudniu w weekend
NexText objHeatingWeekendAfternoonOnVal(PG_HEATING_ID, OBJ_HEATING_WEEKEND_AFTERNOON_ON_VAL_ID, OBJ_HEATING_WEEKEND_AFTERNOON_ON_VAL_NAME);
NexButton objHeatingWeekendAfternoonOnDec(PG_HEATING_ID, OBJ_HEATING_WEEKEND_AFTERNOON_ON_DEC_ID, OBJ_HEATING_WEEKEND_AFTERNOON_ON_DEC_NAME);
NexButton objHeatingWeekendAfternoonOnInc(PG_HEATING_ID, OBJ_HEATING_WEEKEND_AFTERNOON_ON_INC_ID, OBJ_HEATING_WEEKEND_AFTERNOON_ON_INC_NAME);
//Wyłącz ogrzewanie popołudniu w weekend
NexText objHeatingWeekendAfternoonOffVal(PG_HEATING_ID, OBJ_HEATING_WEEKEND_AFTERNOON_OFF_VAL_ID, OBJ_HEATING_WEEKEND_AFTERNOON_OFF_VAL_NAME);
NexButton objHeatingWeekendAfternoonOffDec(PG_HEATING_ID, OBJ_HEATING_WEEKEND_AFTERNOON_OFF_DEC_ID, OBJ_HEATING_WEEKEND_AFTERNOON_OFF_DEC_NAME);
NexButton objHeatingWeekendAfternoonOffInc(PG_HEATING_ID, OBJ_HEATING_WEEKEND_AFTERNOON_OFF_INC_ID, OBJ_HEATING_WEEKEND_AFTERNOON_OFF_INC_NAME);

//------------/*Oświetlenie*/------------
NexPage pgLightsBath(PG_LIGHTS_ID, 0, PG_LIGHTS_NAME);
//Scenariusz 1
NexSlider compLightBathMainVal1(PG_LIGHTS_ID, COMP_LIGHT_BATH_MAIN_1_ID, COMP_LIGHT_BATH_MAIN_1_NAME);
NexSlider compLightBathHolderVal1(PG_LIGHTS_ID, COMP_LIGHT_BATH_HOLDER_1_ID, COMP_LIGHT_BATH_HOLDER_1_NAME);
NexSlider compLightBathTapeWhiteVal1(PG_LIGHTS_ID, COMP_LIGHT_BATH_TAPE_1_ID, COMP_LIGHT_BATH_TAPE_1_NAME);
NexSlider compLightBathTapeRgbVal1(PG_LIGHTS_ID, COMP_LIGHT_BATH_RGB_V_1_ID, COMP_LIGHT_BATH_RGB_V_1_NAME);
NexSlider compLightBathTapeRgbHue1(PG_LIGHTS_ID, COMP_LIGHT_BATH_RGB_H_1_ID, COMP_LIGHT_BATH_RGB_H_1_NAME);
//Scenariusz 2
NexSlider compLightBathMainVal2(PG_LIGHTS_ID, COMP_LIGHT_BATH_MAIN_2_ID, COMP_LIGHT_BATH_MAIN_2_NAME);
NexSlider compLightBathHolderVal2(PG_LIGHTS_ID, COMP_LIGHT_BATH_HOLDER_2_ID, COMP_LIGHT_BATH_HOLDER_2_NAME);
NexSlider compLightBathTapeWhiteVal2(PG_LIGHTS_ID, COMP_LIGHT_BATH_TAPE_2_ID, COMP_LIGHT_BATH_TAPE_2_NAME);
NexSlider compLightBathTapeRgbVal2(PG_LIGHTS_ID, COMP_LIGHT_BATH_RGB_V_2_ID, COMP_LIGHT_BATH_RGB_V_2_NAME);
NexSlider compLightBathTapeRgbHue2(PG_LIGHTS_ID, COMP_LIGHT_BATH_RGB_H_2_ID, COMP_LIGHT_BATH_RGB_H_2_NAME);
//Scenariusz 3
NexSlider compLightBathMainVal3(PG_LIGHTS_ID, COMP_LIGHT_BATH_MAIN_3_ID, COMP_LIGHT_BATH_MAIN_3_NAME);
NexSlider compLightBathHolderVal3(PG_LIGHTS_ID, COMP_LIGHT_BATH_HOLDER_3_ID, COMP_LIGHT_BATH_HOLDER_3_NAME);
NexSlider compLightBathTapeWhiteVal3(PG_LIGHTS_ID, COMP_LIGHT_BATH_TAPE_3_ID, COMP_LIGHT_BATH_TAPE_3_NAME);
NexSlider compLightBathTapeRgbVal3(PG_LIGHTS_ID, COMP_LIGHT_BATH_RGB_V_3_ID, COMP_LIGHT_BATH_RGB_V_3_NAME);
NexSlider compLightBathTapeRgbHue3(PG_LIGHTS_ID, COMP_LIGHT_BATH_RGB_H_3_ID, COMP_LIGHT_BATH_RGB_H_3_NAME);

//------------/*Czas i inne*/------------
NexPage pgTime(PG_TIME_ID, 0, PG_TIME_NAME);
//Ustawianie czasu
NexText tYear = NexText(PG_TIME_ID, OBJ_TIME_YEAR_ID, OBJ_TIME_YEAR_NAME);
NexText tMonth = NexText(PG_TIME_ID, OBJ_TIME_MONTH_ID, OBJ_TIME_MONTH_NAME);
NexText tDay = NexText(PG_TIME_ID, OBJ_TIME_DAY_ID, OBJ_TIME_DAY_NAME);
NexText tHour = NexText(PG_TIME_ID, OBJ_TIME_HOUR_ID, OBJ_TIME_HOUR_NAME);
NexText tMinute = NexText(PG_TIME_ID, OBJ_TIME_MINUTE_ID, OBJ_TIME_MINUTE_NAME);
NexButton bDateTimeNext = NexButton(PG_TIME_ID, OBJ_TIME_NEXT_ID, OBJ_TIME_NEXT_NAME);
NexButton bDateTimeSet = NexButton(PG_TIME_ID, OBJ_TIME_SET_ID, OBJ_TIME_SET_NAME);

//------------/*Światło PIR*/----------------
NexPage pgLightsBathAuto(PG_LIGHT_AUTO_ID, 0, PG_LIGHT_AUTO_NAME);


//------------/*Sieć*/----------------
NexPage pgNetwork(PG_NETWORK_ID, 0, PG_NETWORK_NAME);
NexText objWiFiSsid(PG_NETWORK_ID, OBJ_NETWORK_WIFI_SSID_ID, OBJ_NETWORK_WIFI_SSID_NAME);
NexText objWiFiPass(PG_NETWORK_ID, OBJ_NETWORK_WIFI_PASS_ID, OBJ_NETWORK_WIFI_PASS_NAME);
NexText objMqttServer(PG_NETWORK_ID, OBJ_NETWORK_MQTT_SERVER_ID, OBJ_NETWORK_MQTT_SERVER_NAME);
NexNumber objMqttPort(PG_NETWORK_ID, OBJ_NETWORK_MQTT_PORT_ID, OBJ_NETWORK_MQTT_PORT_NAME);

//------------/*Pozostałe*/----------------
//Histereza ogrzewania
NexPage pgOther(PG_OTHER_ID, 0, PG_OTHER_NAME);
NexText objHeatingHisteresisVal(PG_OTHER_ID, OBJ_HEATING_HISTERESIS_VAL_ID, OBJ_HEATING_HISTERESIS_VAL_NAME);
NexButton objHeatingHisteresisDec(PG_OTHER_ID, OBJ_HEATING_HISTERESIS_DEC_ID, OBJ_HEATING_HISTERESIS_DEC_NAME);
NexButton objHeatingHisteresisInc(PG_OTHER_ID, OBJ_HEATING_HISTERESIS_INC_ID, OBJ_HEATING_HISTERESIS_INC_NAME);

//------------/*Ustawienia*/------------
NexPage pgSettings(PG_SETTINGS_ID, 0, PG_SETTINGS_NAME);
//------------/*Klawiatura A*/------------
NexPage pgKeybdA(PG_KEYBOARD_A_ID, 0, PG_KEYBOARD_A_NAME);
//------------/*Klawiatura B*/------------
NexPage pgKeybdB(PG_KEYBOARD_B_ID, 0, PG_KEYBOARD_B_NAME);


/*
  Register a button object to the touch event list.
*/
NexTouch *nex_listen_list[] =
{
  &pgMain,
  &pgHeating,
  &pgLightsBath,
  &pgLightsBathAuto,
  &pgTime,
  &pgNetwork,
  &pgOther,
  &pgSettings,
  &pgKeybdA,
  &pgKeybdB,
  &objBathSw1,
  &objBathSw2,
  &objBathSw3,
  &btnDayTempInc,
  &btnDayTempDec,
  &btnNightTempInc,
  &btnNightTempDec,
  &bDateTimeNext,
  &bDateTimeSet,
  &compLightBathMainVal1,
  &compLightBathHolderVal1,
  &compLightBathTapeWhiteVal1,
  &compLightBathTapeRgbVal1,
  &compLightBathTapeRgbHue1,
  &compLightBathMainVal2,
  &compLightBathHolderVal2,
  &compLightBathTapeWhiteVal2,
  &compLightBathTapeRgbVal2,
  &compLightBathTapeRgbHue2,
  &compLightBathMainVal3,
  &compLightBathHolderVal3,
  &compLightBathTapeWhiteVal3,
  &compLightBathTapeRgbVal3,
  &compLightBathTapeRgbHue3,
  &objHeatingWorkingDaysMorningOnDec,
  &objHeatingWorkingDaysMorningOnInc,
  &objHeatingWorkingDaysMorningOffDec,
  &objHeatingWorkingDaysMorningOffInc,
  &objHeatingWorkingDaysAfternoonOnDec,
  &objHeatingWorkingDaysAfternoonOnInc,
  &objHeatingWorkingDaysAfternoonOffDec,
  &objHeatingWorkingDaysAfternoonOffInc,
  &objHeatingWeekendMorningOnDec,
  &objHeatingWeekendMorningOnInc,
  &objHeatingWeekendMorningOffDec,
  &objHeatingWeekendMorningOffInc,
  &objHeatingWeekendAfternoonOnDec,
  &objHeatingWeekendAfternoonOnInc,
  &objHeatingWeekendAfternoonOffDec,
  &objHeatingWeekendAfternoonOffInc,
  &objHeatingHisteresisDec,
  &objHeatingHisteresisInc,
  NULL
};

//----------------------------------------------------------------------------------------
DisplayControler::DisplayControler()
{
	currentPage = PG_MAIN_ID; //zakładam, że po restarcie systemu aktywna jest główna strona, w razie czego można dodać komendę oczytująca bieżącą stronę

	disp.outdoorTemperature = 99;
	disp.outdoorHumidity = 0;
	disp.pressure = 0;

	curr.outdoorTemperature = 99;
	curr.outdoorHumidity = 0;
	curr.pressure = 0;

	lastSw1State = SW_ON;
	lastSw2State = SW_ON;
	lastSw3State = SW_ON;

	sw1State = SW_OFF;//dzięki temu po restarcie wyłączy switch
	sw2State = SW_OFF;//dzięki temu po restarcie wyłączy switch
	sw3State = SW_OFF;//dzięki temu po restarcie wyłączy switch

	curr.heatingStatus = HEATING_STATUS_HEAT;
	disp.heatingStatus = HEATING_STATUS_HEAT;
}
//----------------------------------------------------------------------------------------
void DisplayControler::init() {
	/* Set the baudrate which is for debug and communicate with Nextion screen. */
	nexInit(115200, UART1_BAUND, SERIAL_8N1, UART1_RX, UART1_TX);

	/* Register the pop event callback function of the current button component. */
	pgMain.attachOnShow(onPageShow, &pgMain);
	pgHeating.attachOnShow(onPageShow, &pgHeating);
	pgLightsBath.attachOnShow(onPageShow, &pgLightsBath);
	pgLightsBathAuto.attachOnShow(onPageShow, &pgLightsBathAuto);	
	pgTime.attachOnShow(onPageShow, &pgTime);
	pgNetwork.attachOnShow(onPageShow, &pgNetwork);
	pgOther.attachOnShow(onPageShow, &pgOther);
	pgSettings.attachOnShow(onPageShow, &pgSettings);
	pgKeybdA.attachOnShow(onPageShow, &pgKeybdA);
	pgKeybdB.attachOnShow(onPageShow, &pgKeybdB);

	objBathSw1.attachPush(onSwitchPush, &objBathSw1);
	objBathSw2.attachPush(onSwitchPush, &objBathSw2);
	objBathSw3.attachPush(onSwitchPush, &objBathSw3);

	btnDayTempInc.attachPush(onBtnTempPush, &btnDayTempInc);
	btnDayTempDec.attachPush(onBtnTempPush, &btnDayTempDec);

	btnNightTempInc.attachPush(onBtnTempPush, &btnNightTempInc);
	btnNightTempDec.attachPush(onBtnTempPush, &btnNightTempDec);

	objHeatingWorkingDaysMorningOnDec.attachPush(onBtnTempPush, &objHeatingWorkingDaysMorningOnDec);
	objHeatingWorkingDaysMorningOnInc.attachPush(onBtnTempPush, &objHeatingWorkingDaysMorningOnInc);
	objHeatingWorkingDaysMorningOffDec.attachPush(onBtnTempPush, &objHeatingWorkingDaysMorningOffDec);
	objHeatingWorkingDaysMorningOffInc.attachPush(onBtnTempPush, &objHeatingWorkingDaysMorningOffInc);
	objHeatingWorkingDaysAfternoonOnDec.attachPush(onBtnTempPush, &objHeatingWorkingDaysAfternoonOnDec);
	objHeatingWorkingDaysAfternoonOnInc.attachPush(onBtnTempPush, &objHeatingWorkingDaysAfternoonOnInc);
	objHeatingWorkingDaysAfternoonOffDec.attachPush(onBtnTempPush, &objHeatingWorkingDaysAfternoonOffDec);
	objHeatingWorkingDaysAfternoonOffInc.attachPush(onBtnTempPush, &objHeatingWorkingDaysAfternoonOffInc);
	objHeatingWeekendMorningOnDec.attachPush(onBtnTempPush, &objHeatingWeekendMorningOnDec);
	objHeatingWeekendMorningOnInc.attachPush(onBtnTempPush, &objHeatingWeekendMorningOnInc);
	objHeatingWeekendMorningOffDec.attachPush(onBtnTempPush, &objHeatingWeekendMorningOffDec);
	objHeatingWeekendMorningOffInc.attachPush(onBtnTempPush, &objHeatingWeekendMorningOffInc);
	objHeatingWeekendAfternoonOnDec.attachPush(onBtnTempPush, &objHeatingWeekendAfternoonOnDec);
	objHeatingWeekendAfternoonOnInc.attachPush(onBtnTempPush, &objHeatingWeekendAfternoonOnInc);
	objHeatingWeekendAfternoonOffDec.attachPush(onBtnTempPush, &objHeatingWeekendAfternoonOffDec);
	objHeatingWeekendAfternoonOffInc.attachPush(onBtnTempPush, &objHeatingWeekendAfternoonOffInc);

	bDateTimeNext.attachPush(onDateTimeNextPush, &bDateTimeNext);
	bDateTimeSet.attachPush(onBtnbDateTimeSetPush, &bDateTimeSet);
	objHeatingHisteresisDec.attachPush(onBtnTempHisteresisPush, &objHeatingHisteresisDec);
	objHeatingHisteresisInc.attachPush(onBtnTempHisteresisPush, &objHeatingHisteresisInc);

	compLightBathMainVal1.attachPop(onSliderLightPop, &compLightBathMainVal1);
	compLightBathHolderVal1.attachPop(onSliderLightPop, &compLightBathHolderVal1);
	compLightBathTapeWhiteVal1.attachPop(onSliderLightPop, &compLightBathTapeWhiteVal1);
	compLightBathTapeRgbVal1.attachPop(onSliderLightPop, &compLightBathTapeRgbVal1);
	compLightBathTapeRgbHue1.attachPop(onSliderLightPop, &compLightBathTapeRgbHue1);

	compLightBathMainVal2.attachPop(onSliderLightPop, &compLightBathMainVal2);
	compLightBathHolderVal2.attachPop(onSliderLightPop, &compLightBathHolderVal2);
	compLightBathTapeWhiteVal2.attachPop(onSliderLightPop, &compLightBathTapeWhiteVal2);
	compLightBathTapeRgbVal2.attachPop(onSliderLightPop, &compLightBathTapeRgbVal2);
	compLightBathTapeRgbHue2.attachPop(onSliderLightPop, &compLightBathTapeRgbHue2);

	compLightBathMainVal3.attachPop(onSliderLightPop, &compLightBathMainVal3);
	compLightBathHolderVal3.attachPop(onSliderLightPop, &compLightBathHolderVal3);
	compLightBathTapeWhiteVal3.attachPop(onSliderLightPop, &compLightBathTapeWhiteVal3);
	compLightBathTapeRgbVal3.attachPop(onSliderLightPop, &compLightBathTapeRgbVal3);
	compLightBathTapeRgbHue3.attachPop(onSliderLightPop, &compLightBathTapeRgbHue3);
}
//----------------------------------------------------------------------------------------
void DisplayControler::onPageShow(void *ptr)
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
	case PG_TIME_ID:
		displayControler.refreshTimePage();
		break;
	case PG_LIGHT_AUTO_ID:
		displayControler.refreshLightsAutoPage();
		break;
	case PG_NETWORK_ID:
		displayControler.refreshNetworkPage();
		break;
	case PG_OTHER_ID:
		displayControler.refreshOtherPage();
		break;
	}
}
//----------------------------------------------------------------------------------------
void DisplayControler::onSwitchPush(void *ptr)
{
	NexPicture *obj = (NexPicture *)ptr;
	Serial.printf("onBathSwPush() pageId: %d objId: %d objName: %s\r\n", obj->getObjPid(), obj->getObjCid(), obj->getObjName());
	uint8_t switchId;
	uint8_t switchState;
	switch (obj->getObjCid()) {
	case OBJ_BATH_SW_1_ID:
		switchId = SWITCH_BATH_1_ID;
		switchState = !displayControler.sw1State;
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
	eventDispatcher.onSwitchChange(switchId, switchState);
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshHeatingStatus(uint8_t heatingStatus) {
	curr.heatingStatus = heatingStatus;
	if (currentPage != PG_MAIN_ID)
		return;
	if (disp.heatingStatus != curr.heatingStatus) {
		String statusStr = HeatingControler::statusToStr(curr.heatingStatus);
		int icoId = curr.heatingStatus == HEATING_STATUS_HEAT ? PIC_HEATING_HEAT : PIC_HEATING_COOL;
		logger.log(debug, "DisplayControler::refreshHeatingStatus() status: %s icoId: %d\r\n", statusStr.c_str(), icoId);

		if (currentPage == PG_MAIN_ID 
			&& objHeatingStatus.setPic(icoId)) {
			disp.heatingStatus = curr.heatingStatus;
		}
	}
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshHeatingPeriod(uint8_t heatingPeriod) {
	curr.heatingPeriod = heatingPeriod;
	if (currentPage != PG_MAIN_ID)
		return;
	if (disp.heatingPeriod != curr.heatingPeriod) {
		String periodStr = HeatingControler::periodToStr(heatingPeriod);
		logger.log(debug, "DisplayControler::refreshHeatingPeriod() period: %s\r\n", periodStr.c_str());
		if (currentPage == PG_MAIN_ID 
			&& objHeatingPeriod.setPic(curr.heatingPeriod == HEATING_PERIOD_DAY ? PIC_HEATING_DAY : PIC_HEATING_NIGHT)) {
			disp.heatingPeriod = curr.heatingPeriod;
		}
	}
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshHeatingRequiredTemp(float value) {
	curr.heatingRequiredTemp = value;
	if (currentPage != PG_MAIN_ID)
		return;
	if (disp.heatingRequiredTemp != curr.heatingRequiredTemp) {
		char buf[10];
		sprintf(buf, "%02.1fC", curr.heatingRequiredTemp);
		logger.log(debug, "DisplayControler::refreshHeatingRequiredTemp() temp: %.1f\r\n", curr.heatingRequiredTemp);
		if (currentPage == PG_MAIN_ID
			&& objHeatingSetTemp.setText(buf)) {
			disp.heatingRequiredTemp = curr.heatingRequiredTemp;
		}
	}
}
//----------------------------------------------------------------------------------------
void DisplayControler::onSwitchChanged(uint8_t switchId, uint8_t switchState) {
	Serial.printf("DisplayControler::onSwitchChanged() id: %d state: %d\r\n", switchId, switchState);
	switch (switchId) {
	case SWITCH_BATH_1_ID:
		sw1State = switchState;
		//refreshBathSw1();
		break;
	case SWITCH_BATH_2_ID:
		sw2State = switchState;
		//refreshBathSw2();
		break;
	case SWITCH_BATH_3_ID:
		sw3State = switchState;
		//refreshBathSw3();
		break;
	}
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshBathSw1(void) {
	if (currentPage != PG_MAIN_ID)
		return;
	Serial.printf("DisplayControler::refreshBathSw1()\r\n");
	if (lastSw1State != sw1State) {
		if (currentPage == PG_MAIN_ID) {
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
		if (currentPage == PG_MAIN_ID) {
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
		if (currentPage == PG_MAIN_ID) {
			objBathSw3.setPic(sw3State == SW_ON ? PICTURE_SWITCH_ON : PICTURE_SWITCH_OFF);//sprawdzić czy ta operacja zwraca coś po poprawnym wykonaniu 
			lastSw3State = sw3State;
		}
	}
}
//----------------------------------------------------------------------------------------
void DisplayControler::onBtnTempPush(void *ptr) {
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
	case OBJ_HEATING_WORKING_DAYS_MORNING_ON_DEC_ID:
		displayControler.decHeatingTime(HEATING_WORKING_DAYS_MORNING_ON, &objHeatingWorkingDaysMorningOnVal);
		break;
	case OBJ_HEATING_WORKING_DAYS_MORNING_ON_INC_ID:
		displayControler.incHeatingTime(HEATING_WORKING_DAYS_MORNING_ON, &objHeatingWorkingDaysMorningOnVal);
		break;
	case OBJ_HEATING_WORKING_DAYS_MORNING_OFF_DEC_ID:
		displayControler.decHeatingTime(HEATING_WORKING_DAYS_MORNING_OFF, &objHeatingWorkingDaysMorningOffVal);
		break;
	case OBJ_HEATING_WORKING_DAYS_MORNING_OFF_INC_ID:
		displayControler.incHeatingTime(HEATING_WORKING_DAYS_MORNING_OFF, &objHeatingWorkingDaysMorningOffVal);
		break;
	case OBJ_HEATING_WORKING_DAYS_AFTERNOON_ON_DEC_ID:
		displayControler.decHeatingTime(HEATING_WORKING_DAYS_AFTERNOON_ON, &objHeatingWorkingDaysAfternoonOnVal);
		break;
	case OBJ_HEATING_WORKING_DAYS_AFTERNOON_ON_INC_ID:
		displayControler.incHeatingTime(HEATING_WORKING_DAYS_AFTERNOON_ON, &objHeatingWorkingDaysAfternoonOnVal);
		break;
	case OBJ_HEATING_WORKING_DAYS_AFTERNOON_OFF_DEC_ID:
		displayControler.decHeatingTime(HEATING_WORKING_DAYS_AFTERNOON_OFF, &objHeatingWorkingDaysAfternoonOffVal);
		break;
	case OBJ_HEATING_WORKING_DAYS_AFTERNOON_OFF_INC_ID:
		displayControler.incHeatingTime(HEATING_WORKING_DAYS_AFTERNOON_OFF, &objHeatingWorkingDaysAfternoonOffVal);
		break;
	case OBJ_HEATING_WEEKEND_MORNING_ON_DEC_ID:
		displayControler.decHeatingTime(HEATING_WEEKEND_MORNING_ON, &objHeatingWeekendMorningOnVal);
		break;
	case OBJ_HEATING_WEEKEND_MORNING_ON_INC_ID:
		displayControler.incHeatingTime(HEATING_WEEKEND_MORNING_ON, &objHeatingWeekendMorningOnVal);
		break;
	case OBJ_HEATING_WEEKEND_MORNING_OFF_DEC_ID:
		displayControler.decHeatingTime(HEATING_WEEKEND_MORNING_OFF, &objHeatingWeekendMorningOffVal);
		break;
	case OBJ_HEATING_WEEKEND_MORNING_OFF_INC_ID:
		displayControler.incHeatingTime(HEATING_WEEKEND_MORNING_OFF, &objHeatingWeekendMorningOffVal);
		break;
	case OBJ_HEATING_WEEKEND_AFTERNOON_ON_DEC_ID:
		displayControler.decHeatingTime(HEATING_WEEKEND_AFTERNOON_ON, &objHeatingWeekendAfternoonOnVal);
		break;
	case OBJ_HEATING_WEEKEND_AFTERNOON_ON_INC_ID:
		displayControler.incHeatingTime(HEATING_WEEKEND_AFTERNOON_ON, &objHeatingWeekendAfternoonOnVal);
		break;
	case OBJ_HEATING_WEEKEND_AFTERNOON_OFF_DEC_ID:
		displayControler.decHeatingTime(HEATING_WEEKEND_AFTERNOON_OFF, &objHeatingWeekendAfternoonOffVal);
		break;
	case OBJ_HEATING_WEEKEND_AFTERNOON_OFF_INC_ID:
		displayControler.incHeatingTime(HEATING_WEEKEND_AFTERNOON_OFF, &objHeatingWeekendAfternoonOffVal);
		break;
	}
	eventDispatcher.onHeatingConfigurationChange();
}
//----------------------------------------------------------------------------------------
void DisplayControler::onBtnTempHisteresisPush(void *ptr) {
	NexButton *btn = (NexButton *)ptr;
	Serial.printf("onBtnTempHisteresisPush(): pageId=%d cmponentId=%d name=%s\r\n", btn->getObjPid(), btn->getObjCid(), btn->getObjName());

	char buf[8];
	switch (btn->getObjCid()) {
	case OBJ_HEATING_HISTERESIS_INC_ID:
		if (configuration.incrementHisteresisTemperature()) {
			dtostrf(configuration.getHisteresisTemp(), 1, 1, buf);
			objHeatingHisteresisVal.setText(buf);
			eventDispatcher.onHeatingConfigurationChange();
		}
		break;
	case OBJ_HEATING_HISTERESIS_DEC_ID:
		if (configuration.decrementHisteresisTemperature()) {
			dtostrf(configuration.getHisteresisTemp(), 1, 1, buf);
			objHeatingHisteresisVal.setText(buf);
			eventDispatcher.onHeatingConfigurationChange();
		}
		break;
	}
}
//----------------------------------------------------------------------------------------
void DisplayControler::decHeatingTime(uint8_t idx, class NexText* obj) {
	if (configuration.decrementHeatingTime(idx)) {
		showHeatingTime(idx, obj);
	}
}
//----------------------------------------------------------------------------------------
void DisplayControler::incHeatingTime(uint8_t idx, class NexText* obj) {
	if (configuration.incrementHeatingTime(idx)) {
		showHeatingTime(idx, obj);
	}
}
//----------------------------------------------------------------------------------------
void DisplayControler::showHeatingTime(uint8_t idx, NexText* obj) {
	char buf[8];
	sprintf(buf, "%02d:%02d", configuration.getHeatingTime(idx).hour, configuration.getHeatingTime(idx).minute);
	obj->setText(buf);
}
//----------------------------------------------------------------------------------------
void DisplayControler::refresMainPage() {
	//TODO: 
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshHeatingPage() {
	lblDayTempValue.setText(Configuration::temperatureAsString(configuration.getDayTemperature()).c_str());
	lblNightTempValue.setText(Configuration::temperatureAsString(configuration.getNightTemperature()).c_str());

	showHeatingTime(HEATING_WORKING_DAYS_MORNING_ON, &objHeatingWorkingDaysMorningOnVal);
	showHeatingTime(HEATING_WORKING_DAYS_MORNING_OFF, &objHeatingWorkingDaysMorningOffVal);
	showHeatingTime(HEATING_WORKING_DAYS_AFTERNOON_ON, &objHeatingWorkingDaysAfternoonOnVal);
	showHeatingTime(HEATING_WORKING_DAYS_AFTERNOON_OFF, &objHeatingWorkingDaysAfternoonOffVal);
	showHeatingTime(HEATING_WEEKEND_MORNING_ON, &objHeatingWeekendMorningOnVal);
	showHeatingTime(HEATING_WEEKEND_MORNING_OFF, &objHeatingWeekendMorningOffVal);
	showHeatingTime(HEATING_WEEKEND_AFTERNOON_ON, &objHeatingWeekendAfternoonOnVal);
	showHeatingTime(HEATING_WEEKEND_AFTERNOON_OFF, &objHeatingWeekendAfternoonOffVal);
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshLightsPage()
{
	compLightBathMainVal1.setValue(configuration.getLightsBathValue(LIGHT_BATH_MAIN_1_IDX));
	compLightBathHolderVal1.setValue(configuration.getLightsBathValue(LIGHT_BATH_HOLDER_1_IDX));
	compLightBathTapeWhiteVal1.setValue(configuration.getLightsBathValue(LIGHT_BATH_TAPE_1_IDX));
	compLightBathTapeRgbVal1.setValue(configuration.getLightsBathValue(LIGHT_BATH_RGB_V_1_IDX));
	compLightBathTapeRgbHue1.setValue(configuration.getLightsBathValue(LIGHT_BATH_RGB_H_1_IDX));

	compLightBathMainVal2.setValue(configuration.getLightsBathValue(LIGHT_BATH_MAIN_2_IDX));
	compLightBathHolderVal2.setValue(configuration.getLightsBathValue(LIGHT_BATH_HOLDER_2_IDX));
	compLightBathTapeWhiteVal2.setValue(configuration.getLightsBathValue(LIGHT_BATH_TAPE_2_IDX));
	compLightBathTapeRgbVal2.setValue(configuration.getLightsBathValue(LIGHT_BATH_RGB_V_2_IDX));
	compLightBathTapeRgbHue2.setValue(configuration.getLightsBathValue(LIGHT_BATH_RGB_H_2_IDX));

	compLightBathMainVal3.setValue(configuration.getLightsBathValue(LIGHT_BATH_MAIN_3_IDX));
	compLightBathHolderVal3.setValue(configuration.getLightsBathValue(LIGHT_BATH_HOLDER_3_IDX));
	compLightBathTapeWhiteVal3.setValue(configuration.getLightsBathValue(LIGHT_BATH_TAPE_3_IDX));
	compLightBathTapeRgbVal3.setValue(configuration.getLightsBathValue(LIGHT_BATH_RGB_V_3_IDX));
	compLightBathTapeRgbHue3.setValue(configuration.getLightsBathValue(LIGHT_BATH_RGB_H_3_IDX));
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshTimePage()
{
	currentTimeComponent = SETUP_DATETIME_YEAR;
	tYear.Set_font_color_pco(COLOR_RED);

	char buf[5];
	sprintf(buf, "%04d", curr.dateTime.year + 2000);
	tYear.setText(buf);
	sprintf(buf, "%02d", curr.dateTime.month);
	tMonth.setText(buf);
	sprintf(buf, "%02d", curr.dateTime.day);
	tDay.setText(buf);
	sprintf(buf, "%02d", curr.dateTime.hour);
	tHour.setText(buf);
	sprintf(buf, "%02d", curr.dateTime.minute);
	tMinute.setText(buf);
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshLightsAutoPage() {

}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshNetworkPage() {
	objWiFiSsid.setText(configuration.wifiSSID.c_str());
	objWiFiPass.setText(configuration.wifiPassword.c_str());
	objMqttServer.setText(configuration.mqttServer.c_str());
	objMqttPort.setValue(configuration.mqttPort);
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshOtherPage() {
	char buf[5];
	dtostrf(configuration.getHisteresisTemp(), 1, 1, buf);
	objHeatingHisteresisVal.setText(buf);
}
//----------------------------------------------------------------------------------------
void DisplayControler::onDateTimeNextPush(void *ptr)
{
	NexButton *btn = (NexButton *)ptr;
	Serial.printf("onBtnbDateTimeNextPush: %s\r\n", btn->getObjName());

	switch (displayControler.currentTimeComponent) {
	case SETUP_DATETIME_YEAR:
		tYear.Set_font_color_pco(COLOR_BLACK);
		tMonth.Set_font_color_pco(COLOR_RED);
		displayControler.currentTimeComponent = SETUP_DATETIME_MONTH;
		break;
	case SETUP_DATETIME_MONTH:
		tMonth.Set_font_color_pco(COLOR_BLACK);
		tDay.Set_font_color_pco(COLOR_RED);
		displayControler.currentTimeComponent = SETUP_DATETIME_DAY;
		break;
	case SETUP_DATETIME_DAY:
		tDay.Set_font_color_pco(COLOR_BLACK);
		tHour.Set_font_color_pco(COLOR_RED);
		displayControler.currentTimeComponent = SETUP_DATETIME_HOUR;
		break;
	case SETUP_DATETIME_HOUR:
		tHour.Set_font_color_pco(COLOR_BLACK);
		tMinute.Set_font_color_pco(COLOR_RED);
		displayControler.currentTimeComponent = SETUP_DATETIME_MINUTE;
		break;
	case SETUP_DATETIME_MINUTE:
		tMinute.Set_font_color_pco(COLOR_BLACK);
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
void DisplayControler::onBtnbDateTimeSetPush(void *ptr)
{
	NexButton *btn = (NexButton *)ptr;
	Serial.printf("onBtnbDateTimeSetPush: %s\r\n", btn->getObjName());

	char buf[10];
	if (displayControler.currentTimeComponent == SETUP_DATETIME_YEAR) {
		uint16_t year = displayControler.curr.dateTime.year;
		if (year >= 99)
			year = 0;
		else
			year++;

		DateTime dateTime(year,
			displayControler.curr.dateTime.month,
			displayControler.curr.dateTime.day,
			displayControler.curr.dateTime.hour,
			displayControler.curr.dateTime.minute,
			0);
		rtcControler.adjust(dateTime);

		//rtc.set(rtc.second(), rtc.minute(), rtc.hour(), rtc.dayOfWeek(), rtc.day(), rtc.month(), year);

		sprintf(buf, "%04d", year + 2000);
		tYear.setText(buf);
	}
	if (displayControler.currentTimeComponent == SETUP_DATETIME_MONTH) {
		uint8_t month = displayControler.curr.dateTime.month;
		if (month >= 12)
			month = 1;
		else
			month++;

		DateTime dateTime(displayControler.curr.dateTime.year + 2000,
			month,
			displayControler.curr.dateTime.day,
			displayControler.curr.dateTime.hour,
			displayControler.curr.dateTime.minute,
			0);
		rtcControler.adjust(dateTime);

		//rtc.set(rtc.second(), rtc.minute(), rtc.hour(), rtc.dayOfWeek(), rtc.day(), month, rtc.year());

		sprintf(buf, "%02d", month);
		tMonth.setText(buf);
	}
	if (displayControler.currentTimeComponent == SETUP_DATETIME_DAY) {
		uint8_t day = displayControler.curr.dateTime.day;
		if (day >= 31)
			day = 1;
		else
			day++;

		DateTime dateTime(displayControler.curr.dateTime.year + 2000,
			displayControler.curr.dateTime.month,
			day,
			displayControler.curr.dateTime.hour,
			displayControler.curr.dateTime.minute,
			0);
		rtcControler.adjust(dateTime);

		//rtc.set(rtc.second(), rtc.minute(), rtc.hour(), rtc.dayOfWeek(), day, rtc.month(), rtc.year());

		sprintf(buf, "%02d", day);
		tDay.setText(buf);
	}
	if (displayControler.currentTimeComponent == SETUP_DATETIME_HOUR) {
		uint8_t hour = displayControler.curr.dateTime.hour;
		if (hour >= 23)
			hour = 0;
		else
			hour++;

		DateTime dateTime(displayControler.curr.dateTime.year + 2000,
			displayControler.curr.dateTime.month,
			displayControler.curr.dateTime.day,
			hour,
			displayControler.curr.dateTime.minute,
			0);
		rtcControler.adjust(dateTime);

		//rtc.set(rtc.second(), rtc.minute(), hour, rtc.dayOfWeek(), rtc.day(), rtc.month(), rtc.year());

		sprintf(buf, "%02d", hour);
		tHour.setText(buf);
	}
	if (displayControler.currentTimeComponent == SETUP_DATETIME_MINUTE) {
		uint8_t minute = displayControler.curr.dateTime.minute;
		if (minute >= 59)
			minute = 0;
		else
			minute++;

		DateTime dateTime(displayControler.curr.dateTime.year + 2000,
			displayControler.curr.dateTime.month,
			displayControler.curr.dateTime.day,
			displayControler.curr.dateTime.hour,
			minute,
			0);
		rtcControler.adjust(dateTime);
		//rtc.set(0, minute, rtc.hour(), rtc.dayOfWeek(), rtc.day(), rtc.month(), rtc.year());

		sprintf(buf, "%02d", minute);
		tMinute.setText(buf);
	}
}
//----------------------------------------------------------------------------------------
void DisplayControler::onRefreshDateTime(const DateTime& dateTime) {
	curr.set(dateTime);
	//i tak się nie uda przesłać danych do Nextion jeżeli nie jest obecnie aktywna głowna strona
	if (currentPage != PG_MAIN_ID)
		return;
	/*Hour*/
	if (disp.dateTime.hour != curr.dateTime.hour) {
		if (currentPage == PG_MAIN_ID && tTime1.setText(String(curr.dateTime.hour / 10).c_str()) && tTime2.setText(String(curr.dateTime.hour % 10).c_str()))
			disp.dateTime.hour = curr.dateTime.hour;
	}
	/*Minute*/
	if (disp.dateTime.minute != curr.dateTime.minute) {
		if (currentPage == PG_MAIN_ID && tTime3.setText(String(curr.dateTime.minute / 10).c_str()) && tTime4.setText(String(curr.dateTime.minute % 10).c_str()))
			disp.dateTime.minute = curr.dateTime.minute;
	}
	/*Month*/
	if (disp.dateTime.month != curr.dateTime.month) {
		if (currentPage == PG_MAIN_ID && pMonth.setPic(monthPic(curr.dateTime.month)))
			disp.dateTime.month = curr.dateTime.month;
	}
	/*Day of month*/
	if (disp.dateTime.day != curr.dateTime.day) {
		if (currentPage == PG_MAIN_ID && pDayOfMonth1.setPic(dayOfMonthPic(curr.dateTime.day / 10)) && pDayOfMonth2.setPic(dayOfMonthPic(curr.dateTime.day % 10)))
			disp.dateTime.day = curr.dateTime.day;
	}
	/*Day of week*/
	if (disp.dateTime.dayOfTheWeek != curr.dateTime.dayOfTheWeek) {
		if (currentPage == PG_MAIN_ID && pDayOfWeek.setPic(dayOfWeekPic(curr.dateTime.dayOfTheWeek)))
			disp.dateTime.dayOfTheWeek = curr.dateTime.dayOfTheWeek;
	}
}
//----------------------------------------------------------------------------------------
void DisplayControler::onRefreshIndoorTemperature(float indoorTemp) {
	curr.indoorTemperature = round(indoorTemp * 10);
}
//----------------------------------------------------------------------------------------
void DisplayControler::onRefresIndoorHumidity(float indoorHumidity) {
	curr.indoorHumidity = round(indoorHumidity);
}
//----------------------------------------------------------------------------------------
void DisplayControler::onRefreshIndoorPreasure(float indoorPressure) {
	curr.pressure = round(indoorPressure);
}
//----------------------------------------------------------------------------------------
void DisplayControler::onRefreshOutdoorTemperature(float outdoorTemp) {
	curr.outdoorTemperature = round(outdoorTemp * 10); ;
}
//----------------------------------------------------------------------------------------
void DisplayControler::onRefreshOutdoorHumidity(float outdoorHumidity) {
	curr.outdoorHumidity = round(outdoorHumidity);
}
//----------------------------------------------------------------------------------------
void DisplayControler::onRefreshOutdoorPreasure(float outdoorPressure) {
	//wewnętrzny sensor BME280 będzie dokładniejszy bo ma stałą temperaturę działania
	//curr.pressure = round(outdoorPressure);
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshIndoorTemperature() {
	if (currentPage == PG_MAIN_ID && disp.indoorTemperature != curr.indoorTemperature) {
		uint8_t t1 = curr.indoorTemperature / 10;
		uint8_t t2 = curr.indoorTemperature % 10;
		char buf1[3];
		char buf2[3];
		sprintf(buf1, "%02d", t1);
		sprintf(buf2, "%01d", t2);
		Serial.printf("DisplayControler::refreshIndoorTemperature() %s.%s*C\r\n", buf1, buf2);
		if (currentPage == PG_MAIN_ID && tIndoorTemp1.setText(buf1) && tIndoorTemp2.setText(buf2)) {
			disp.indoorTemperature = curr.indoorTemperature;
		}
	}
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshIndoorHumidity() {
	if (currentPage == PG_MAIN_ID && disp.indoorHumidity != curr.indoorHumidity) {
		Serial.printf("DisplayControler::refreshIndoorHumidity() %2d%%\r\n", curr.indoorHumidity);
		char buf[5];
		sprintf(buf, "%02d%%", curr.indoorHumidity);
		if (currentPage == PG_MAIN_ID && tIndoorHumidity.setText(buf))
			disp.indoorHumidity = curr.indoorHumidity;
	}
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshOutdoorTemperature() {
	if (currentPage == PG_MAIN_ID && disp.outdoorTemperature != curr.outdoorTemperature) {
		Serial.printf("DisplayControler::refreshOutdoorTemperature() %.1f*C\r\n", (float)curr.outdoorTemperature / 10.0);

		bool ret1 = false;
		bool ret2 = false;
		bool ret3 = false;
		int outdoorTempInt = abs(curr.outdoorTemperature);
		//poniżej -10.0
		if (curr.outdoorTemperature <= -100) {
			ret1 = tOutdoorTempSymbol.setText("-");
			ret2 = tOutdoorTemp1.setText(String(outdoorTempInt / 10).c_str());
		}
		//od -9.9 do -1.0
		else if (curr.outdoorTemperature >= -99 & curr.outdoorTemperature <= -10) {
			ret1 = tOutdoorTemp1.setText(String(-outdoorTempInt / 10).c_str());
			ret2 = tOutdoorTempSymbol.setText("");
		}
		//od -0.9 do -0.1
		else if (curr.outdoorTemperature >= -9 & curr.outdoorTemperature <= -1) {
			char buf[3];
			sprintf(buf, "-%01d%", outdoorTempInt / 10);
			ret1 = tOutdoorTemp1.setText(buf);
			ret2 = tOutdoorTempSymbol.setText("");
		}
		//od 0 w górę
		else {
			ret1 = tOutdoorTemp1.setText(String(outdoorTempInt / 10).c_str());
			ret2 = tOutdoorTempSymbol.setText("");
		}
		ret3 = tOutdoorTemp2.setText(String(outdoorTempInt % 10).c_str());
		if (ret1 && ret2 && ret3) {
			disp.outdoorTemperature = curr.outdoorTemperature;
		}
	}
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshOutdoorHumidity() {
	if (currentPage == PG_MAIN_ID && disp.outdoorHumidity != curr.outdoorHumidity) {
		Serial.printf("DisplayControler::refreshOutdoorHumidity() %2d%%\r\n", curr.outdoorHumidity);
		char buf[10];
		sprintf(buf, "%02d%%", curr.outdoorHumidity);
		if (tOutdoorHumidity.setText(buf))
			disp.outdoorHumidity = curr.outdoorHumidity;
	}
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshPreasure() {
	if (currentPage == PG_MAIN_ID && disp.pressure != curr.pressure) {
		Serial.printf("DisplayControler::refreshPreasure() %3dhPa\r\n", curr.pressure);
		char buf[10];
		sprintf(buf, "%03dhPa", curr.pressure);
		if (tOutdoorPreasure.setText(buf))
			disp.pressure = curr.pressure;
	}
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshWiFiStatus(int8_t status) {
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
void DisplayControler::refreshMQTTStatus(int status) {
	//TODO: docelowo zmienić na inną ikonkę na panelu   
	if (status == MQTT_CONNECTED)
		objWiFiStatus.setPic(PICTURE_WIFI_ON);
	else
		objWiFiStatus.setPic(PICTURE_WIFI_OFF);
}
//----------------------------------------------------------------------------------------
void DisplayControler::onSliderLightPop(void *ptr)
{
	NexSlider *obj = (NexSlider *)ptr;
	Serial.printf("onSliderLightPop() pageId: %d objId: %d objName: %s\r\n", obj->getObjPid(), obj->getObjCid(), obj->getObjName());
	uint32_t value;
	bool ret = obj->getValue(&value);
	uint8_t idx;
	if (ret) {
		switch (obj->getObjCid()) {
		case COMP_LIGHT_BATH_MAIN_1_ID: idx = LIGHT_BATH_MAIN_1_IDX; break;
		case COMP_LIGHT_BATH_HOLDER_1_ID: idx = LIGHT_BATH_HOLDER_1_IDX; break;
		case COMP_LIGHT_BATH_TAPE_1_ID: idx = LIGHT_BATH_TAPE_1_IDX; break;
		case COMP_LIGHT_BATH_RGB_V_1_ID: idx = LIGHT_BATH_RGB_V_1_IDX; break;
		case COMP_LIGHT_BATH_RGB_H_1_ID: idx = LIGHT_BATH_RGB_H_1_IDX; break;
		case COMP_LIGHT_BATH_MAIN_2_ID: idx = LIGHT_BATH_MAIN_2_IDX; break;
		case COMP_LIGHT_BATH_HOLDER_2_ID: idx = LIGHT_BATH_HOLDER_2_IDX; break;
		case COMP_LIGHT_BATH_TAPE_2_ID: idx = LIGHT_BATH_TAPE_2_IDX; break;
		case COMP_LIGHT_BATH_RGB_V_2_ID: idx = LIGHT_BATH_RGB_V_2_IDX; break;
		case COMP_LIGHT_BATH_RGB_H_2_ID: idx = LIGHT_BATH_RGB_H_2_IDX; break;
		case COMP_LIGHT_BATH_MAIN_3_ID: idx = LIGHT_BATH_MAIN_3_IDX; break;
		case COMP_LIGHT_BATH_HOLDER_3_ID: idx = LIGHT_BATH_HOLDER_3_IDX; break;
		case COMP_LIGHT_BATH_TAPE_3_ID: idx = LIGHT_BATH_TAPE_3_IDX; break;
		case COMP_LIGHT_BATH_RGB_V_3_ID: idx = LIGHT_BATH_RGB_V_3_IDX; break;
		case COMP_LIGHT_BATH_RGB_H_3_ID: idx = LIGHT_BATH_RGB_H_3_IDX; break;
		default:
			return;
		}
		eventDispatcher.onLightValueChange(idx, value);
	}
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

	if (disp.indoorTemperature != curr.indoorTemperature)
		refreshIndoorTemperature();
	if (disp.indoorHumidity != curr.indoorHumidity)
		refreshIndoorHumidity();
	if (disp.outdoorTemperature != curr.outdoorTemperature)
		refreshOutdoorTemperature();
	if (disp.outdoorHumidity != curr.outdoorHumidity)
		refreshOutdoorHumidity();
	if (disp.pressure != curr.pressure)
		refreshPreasure();

	if (disp.heatingStatus != curr.heatingStatus)
		refreshHeatingStatus(curr.heatingStatus);
	if (disp.heatingPeriod != curr.heatingPeriod)
		refreshHeatingPeriod(curr.heatingPeriod);
	if (disp.heatingRequiredTemp != curr.heatingRequiredTemp)
		refreshHeatingRequiredTemp(curr.heatingRequiredTemp);

}

DisplayControler displayControler;
