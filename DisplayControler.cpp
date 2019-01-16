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

#include "DisplayControler.h"
#include "Constants.h"
#include "Configuration.h"
#include "RtcHelper.h"
#include "SensorsHelper.h"

extern Configuration configuration;
extern DisplayControler displayControler;

//------------/*Strona główna*/------------//
NexPage pgMain = NexPage(PG_MAIN_ID, 0, PG_MAIN_NAME);

NexPicture pDayOfMonth1 = NexPicture(PG_MAIN_ID, PIC_DAY_OF_MONTH1_ID, PIC_DAY_OF_MONTH1_NAME);
NexPicture pDayOfMonth2 = NexPicture(PG_MAIN_ID, PIC_DAY_OF_MONTH2_ID, PIC_DAY_OF_MONTH2_NAME);
NexPicture pMonth = NexPicture(PG_MAIN_ID, PIC_MONTH_ID, PIC_MONTH_NAME);
NexPicture pDayOfWeek = NexPicture(PG_MAIN_ID, PIC_DAY_OF_WEEK_ID, PIC_DAY_OF_WEEK_NAME);

NexText tTime1 = NexText(PG_MAIN_ID, LBL_TIME1_ID, LBL_TIME1_NAME);
NexText tTime2 = NexText(PG_MAIN_ID, LBL_TIME2_ID, LBL_TIME2_NAME);
NexText tTime3 = NexText(PG_MAIN_ID, LBL_TIME3_ID, LBL_TIME3_NAME);
NexText tTime4 = NexText(PG_MAIN_ID, LBL_TIME4_ID, LBL_TIME4_NAME);

NexText tIndoorTemp1 = NexText(PG_MAIN_ID, LBL_INDOOR_TEMP1_ID, LBL_INDOOR_TEMP1_NAME);
NexText tIndoorTemp2 = NexText(PG_MAIN_ID, LBL_INDOOR_TEMP2_ID, LBL_INDOOR_TEMP2_NAME);
NexText tIndoorHumidity = NexText(PG_MAIN_ID, LBL_INDOOR_HUMIDITY_ID, LBL_INDOOR_HUMIDITY_NAME);

NexText tOutdoorTemp1 = NexText(PG_MAIN_ID, LBL_OUTDOOR_TEMP1_ID, LBL_OUTDOOR_TEMP1_NAME);
NexText tOutdoorTemp2 = NexText(PG_MAIN_ID, LBL_OUTDOOR_TEMP2_ID, LBL_OUTDOOR_TEMP2_NAME);
NexText tOutdoorTempSymbol = NexText(PG_MAIN_ID, LBL_OUTDOOR_TEMP_SYMBOL_ID, LBL_OUTDOOR_TEMP_SYMBOL_NAME);
NexText tOutdoorHumidity = NexText(PG_MAIN_ID, LBL_OUTDOOR_HUMIDITY_ID, LBL_OUTDOOR_HUMIDITY_NAME);
NexText tOutdoorPreasure = NexText(PG_MAIN_ID, LBL_OUTDOOR_PREASURE_ID, LBL_OUTDOOR_PREASURE_NAME);

NexPicture switchBathroomMainLight = NexPicture(PG_MAIN_ID, PIC_SWITCH_BATHROOM_MAIN_LIGHT_ID, PIC_SWITCH_BATHROOM_MAIN_LIGHT_NAME);

//------------/*Ogrzewanie*/------------
NexPage pgHeating = NexPage(PG_HEATING_ID, 0, PG_HEATING_NAME);
/*Temperatura w dzień*/
NexText lblDayTempValue = NexText(PG_HEATING_ID, LBL_DAY_TEMP_VALUE_ID, LBL_DAY_TEMP_VALUE_NAME);
NexButton btnDayTempDec = NexButton(PG_HEATING_ID, BTN_DAY_TEMP_DEC_ID, BTN_DAY_TEMP_DEC_NAME);
NexButton btnDayTempInc = NexButton(PG_HEATING_ID, BTN_DAY_TEMP_INC_ID, BTN_DAY_TEMP_INC_NAME);
/*Temperatura w noc*/
NexText lblNightTempValue = NexText(PG_HEATING_ID, LBL_NIGHT_TEMP_VALUE_ID, LBL_NIGHT_TEMP_VALUE_NAME);
NexButton btnNightTempDec = NexButton(PG_HEATING_ID, BTN_NIGHT_TEMP_DEC_ID, BTN_NIGHT_TEMP_DEC_NAME);
NexButton btnNightTempInc = NexButton(PG_HEATING_ID, BTN_NIGHT_TEMP_INC_ID, BTN_NIGHT_TEMP_INC_NAME);

//------------/*Oświetlenie*/------------
NexPage pgLights = NexPage(PG_LIGHTS_ID, 0, PG_LIGHTS_NAME);

//------------/*Czas i inne*/------------
NexPage pgOther = NexPage(PG_OTHER_ID, 0, PG_OTHER_NAME);
/*Ustawianie czasu*/
NexText tYear = NexText(PG_OTHER_ID, LBL_TIME_YEAR, "lblYear");
NexText tMonth = NexText(PG_OTHER_ID, LBL_TIME_MONTH, "lblMonth");
NexText tDay = NexText(PG_OTHER_ID, LBL_TIME_DAY, "lblDay");
NexText tDayW = NexText(PG_OTHER_ID, LBL_TIME_DAY_OF_WEEK, "lblDayW");
NexText tHour = NexText(PG_OTHER_ID, LBL_TIME_HOUR, "lblHour");
NexText tMinute = NexText(PG_OTHER_ID, LBL_TIME_MINUTE, "lblMinute");
NexButton bDateTimeNext = NexButton(PG_OTHER_ID, BTN_TIME_NEXT_ID, BTN_TIME_NEXT_NAME);
NexButton bDateTimeSet = NexButton(PG_OTHER_ID, BTN_TIME_SET_ID, BTN_TIME_SET_NAME);

/*
  Register a button object to the touch event list.
*/
NexTouch *nex_listen_list[] =
{
  &switchBathroomMainLight,
  &btnDayTempInc,
  &btnDayTempDec,
  &btnNightTempInc,
  &btnNightTempDec,
  &bDateTimeNext,
  &bDateTimeSet,
  &pgMain,
  &pgHeating,
  &pgLights,
  &pgOther,
  NULL
};
//----------------------------------------------------------------------------------------
void onPageShow(void *ptr)
{
	NexPage *page = (NexPage *)ptr;
	dbSerialPrintln("onPageShow: " + String(page->getObjName()));
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
bool switchBathroomMainLightState = false;
void onSwitchBathroomMainLightPop(void *ptr)
{
	dbSerialPrintln("onSwitchBathroomMainLightPop");
	switchBathroomMainLightState = !switchBathroomMainLightState;
	switchBathroomMainLight.setPic(switchBathroomMainLightState ? PICTURE_SWITCH_ON : PICTURE_SWITCH_OFF);
}
//----------------------------------------------------------------------------------------
void onBtnTempPush(void *ptr) {
	NexButton *btn = (NexButton *)ptr;
	dbSerialPrintln("onBtnTempPush: pageId=" + String(btn->getObjPid()) + " componentId=" + String(btn->getObjCid()) + " name=" + btn->getObjName());

	switch (btn->getObjCid()) {
	case BTN_DAY_TEMP_INC_ID:
		if (configuration.incrementDayTemperature()) {
			lblDayTempValue.setText(Configuration::temperatureAsString(configuration.getDayTemperature()).c_str());
		}
		break;
	case BTN_DAY_TEMP_DEC_ID:
		if (configuration.decrementDayTemperature()) {
			lblDayTempValue.setText(Configuration::temperatureAsString(configuration.getDayTemperature()).c_str());
		}
		break;
	case BTN_NIGHT_TEMP_INC_ID:
		if (configuration.incrementNightTemperature()) {
			lblNightTempValue.setText(Configuration::temperatureAsString(configuration.getNightTemperature()).c_str());
		}
		break;
	case BTN_NIGHT_TEMP_DEC_ID:
		if (configuration.decrementNightTemperature()) {
			lblNightTempValue.setText(Configuration::temperatureAsString(configuration.getNightTemperature()).c_str());
		}
		break;
	}
}
//----------------------------------------------------------------------------------------
void DisplayControler::refresMainPage()
{
	//TODO: zrobić
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshHeatingPage()
{
	lblDayTempValue.setText(Configuration::temperatureAsString(configuration->getDayTemperature()).c_str());
	lblNightTempValue.setText(Configuration::temperatureAsString(configuration->getNightTemperature()).c_str());
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshLightsPage()
{
	//TODO: zrobić
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
	RtcHelper::dayOfWeekName(buf, rtc.dayOfWeek());
	tDayW.setText(buf);
}
//----------------------------------------------------------------------------------------
void onBtnbDateTimeNextPush(void *ptr)
{
	NexButton *btn = (NexButton *)ptr;
	dbSerialPrintln("onBtnbDateTimeNextPush: " + String(btn->getObjName()));

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
	dbSerialPrintln("onBtnbDateTimeSetPush: " + String(btn->getObjName()));

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

		RtcHelper::dayOfWeekName(buf, dayOfWeek);
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
void DisplayControler::refreshTime() {
	//jeżeli nie jesteśmy na głównej stronie to nie ma sensu odświeżać jej zawartości
	if (currentPage != PG_MAIN_ID)
		return;
	
	rtc.refresh();

	month = rtc.month();
	day = rtc.day();
	dayOfWeek = rtc.dayOfWeek();
	hour = rtc.hour();
	minute = rtc.minute();

	/*Hour*/
	if (lastHour != hour) {
		if (currentPage == PG_MAIN_ID && tTime1.setText(String(hour / 10).c_str()) && tTime2.setText(String(hour % 10).c_str()))
			lastHour = hour;
	}
	/*Minute*/
	if (lastMinute != minute) {
		if (currentPage == PG_MAIN_ID && tTime3.setText(String(minute / 10).c_str()) && tTime4.setText(String(minute % 10).c_str()))
			lastMinute = minute;
	}
	/*Month*/
	if (lastMonth != month) {
		if (currentPage == PG_MAIN_ID && pMonth.setPic(monthPic(month)))
			lastMonth = month;
	}
	/*Day of month*/
	if (lastDay != day) {
		if (currentPage == PG_MAIN_ID && pDayOfMonth1.setPic(dayOfMonthPic(day / 10)) && pDayOfMonth2.setPic(dayOfMonthPic(day % 10)))
			lastDay = day;
	}
	/*Day of week*/
	if (lastDayOfWeek != dayOfWeek) {
		if (currentPage == PG_MAIN_ID && pDayOfWeek.setPic(dayOfWeekPic(dayOfWeek)))
			lastDayOfWeek = dayOfWeek;
	}
}
//----------------------------------------------------------------------------------------
void DisplayControler::refreshIndoorTemperature() {
	//jeżeli nie jesteśmy na głównej stronie to nie ma sensu odświeżać jej zawartości
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
DisplayControler::DisplayControler(Configuration* configuration)
{
	this->configuration = configuration;
	picWiFiStatus = new NexPicture(PG_MAIN_ID, PIC_WIFI_STATUS_ID, PIC_WIFI_STATUS_NAME);
	currentPage = PG_MAIN_ID; //zakładam, że po restarcie systemu aktywna jest główna strona
	uint8_t lastMinute = 99;
	uint8_t lastHour = 99;
	uint8_t lastDay = 99;
	uint8_t lastMonth = 99;
	uint8_t lastDayOfWeek = 99;
	uint8_t minute = 99;
	uint8_t hour = 99;
	uint8_t day = 99;
	uint8_t month = 99;
	uint8_t dayOfWeek = 99;
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

	switchBathroomMainLight.attachPop(onSwitchBathroomMainLightPop, &switchBathroomMainLight);

	btnDayTempInc.attachPush(onBtnTempPush, &btnDayTempInc);
	btnDayTempDec.attachPush(onBtnTempPush, &btnDayTempDec);

	btnNightTempInc.attachPush(onBtnTempPush, &btnNightTempInc);
	btnNightTempDec.attachPush(onBtnTempPush, &btnNightTempDec);

	bDateTimeNext.attachPush(onBtnbDateTimeNextPush, &bDateTimeNext);
	bDateTimeSet.attachPush(onBtnbDateTimeSetPush, &bDateTimeSet);
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
}
