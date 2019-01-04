#include "DisplayControler.h"
#include "Constants.h"
#include "Configuration.h"
#include "RtcHelper.h"

extern Configuration configuration;

NexVariable vaIntTemp1 = NexVariable(PG_MAIN, 31, "vaIntTemp1");

NexText tDayOfMonth = NexText(PG_MAIN, LBL_DAY_OF_MONTH_ID, "tDayOfMonth");
NexText tDayOfWeek = NexText(PG_MAIN, LBL_DAY_OF_WEEK_ID, "tDayOfWeek");
NexText tTime1 = NexText(PG_MAIN, LBL_TIME1_ID, LBL_TIME1_NAME);
NexText tTime2 = NexText(PG_MAIN, LBL_TIME2_ID, LBL_TIME2_NAME);
NexText tIndoorTemp1 = NexText(PG_MAIN, LBL_INDOOR_TEMP1_ID, LBL_INDOOR_TEMP1_NAME);
NexText tIndoorTemp2 = NexText(PG_MAIN, LBL_INDOOR_TEMP2_ID, LBL_INDOOR_TEMP2_NAME);
NexButton btnGoHeatingPage = NexButton(PG_MAIN, BTN_GO_HEATING_PG_ID, BTN_GO_HEATING_PG_NAME);
NexButton btnGoLightsPage = NexButton(PG_MAIN, BTN_GO_LIGHT_PG_ID, BTN_GO_LIGHT_PG_NAME);
NexButton btnGoTimePage = NexButton(PG_MAIN, BTN_GO_TIME_PG_ID, BTN_GO_TIME_PG_NAME);
NexPicture switchBathroomMainLight = NexPicture(PG_MAIN, PIC_SWITCH_BATHROOM_MAIN_LIGHT_ID, PIC_SWITCH_BATHROOM_MAIN_LIGHT_NAME);

//------------/*Ogrzewanie*/------------
/*Temperatura w dzieñ*/
NexText lblDayTempValue = NexText(PG_HEATING_SETTING, LBL_DAY_TEMP_VALUE_ID, LBL_DAY_TEMP_VALUE_NAME);
NexButton btnDayTempDec = NexButton(PG_HEATING_SETTING, BTN_DAY_TEMP_DEC_ID, BTN_DAY_TEMP_DEC_NAME);
NexButton btnDayTempInc = NexButton(PG_HEATING_SETTING, BTN_DAY_TEMP_INC_ID, BTN_DAY_TEMP_INC_NAME);
/*Temperatura w noc*/
NexText lblNightTempValue = NexText(PG_HEATING_SETTING, LBL_NIGHT_TEMP_VALUE_ID, LBL_NIGHT_TEMP_VALUE_NAME);
NexButton btnNightTempDec = NexButton(PG_HEATING_SETTING, BTN_NIGHT_TEMP_DEC_ID, BTN_NIGHT_TEMP_DEC_NAME);
NexButton btnNightTempInc = NexButton(PG_HEATING_SETTING, BTN_NIGHT_TEMP_INC_ID, BTN_NIGHT_TEMP_INC_NAME);

/*Ustawianie czasu*/
NexText tYear = NexText(PG_TIME, LBL_TIME_YEAR, "lblYear");
NexText tMonth = NexText(PG_TIME, LBL_TIME_MONTH, "lblMonth");
NexText tDay = NexText(PG_TIME, LBL_TIME_DAY, "lblDay");
NexText tDayW = NexText(PG_TIME, LBL_TIME_DAY_OF_WEEK, "lblDayW");
NexText tHour = NexText(PG_TIME, LBL_TIME_HOUR, "lblHour");
NexText tMinute = NexText(PG_TIME, LBL_TIME_MINUTE, "lblMinute");

NexButton bDateTimeNext = NexButton(PG_TIME, BTN_TIME_NEXT_ID, BTN_TIME_NEXT_NAME);
NexButton bDateTimeSet = NexButton(PG_TIME, BTN_TIME_SET_ID, BTN_TIME_SET_NAME);

/*
  Register a button object to the touch event list.
*/
NexTouch *nex_listen_list[] =
{
  &switchBathroomMainLight,
  &btnGoHeatingPage,
  &btnGoLightsPage,
  &btnGoTimePage,
  &btnDayTempInc,
  &btnDayTempDec,
  &btnNightTempInc,
  &btnNightTempDec,
  &bDateTimeNext,
  &bDateTimeSet,
  NULL
};


bool switchBathroomMainLightState = false;
void onSwitchBathroomMainLightPop(void *ptr)
{
	dbSerialPrintln("onSwitchBathroomMainLightPop");
	switchBathroomMainLightState = !switchBathroomMainLightState;
	switchBathroomMainLight.setPic(switchBathroomMainLightState ? PICTURE_SWITCH_ON : PICTURE_SWITCH_OFF);
}

void onBtnGoHeatingPagePop(void *ptr)
{
	NexButton *btn = (NexButton *)ptr;

	dbSerialPrintln("onBtnGoHeatingPagePop");
	dbSerialPrint("ptr=");
	dbSerialPrintln((uint32_t)ptr);

	lblDayTempValue.setText(Configuration::temperatureAsString(configuration.getDayTemperature()).c_str());
	lblNightTempValue.setText(Configuration::temperatureAsString(configuration.getNightTemperature()).c_str());
}

void onBtnTempPop(void *ptr) {
	NexButton *btn = (NexButton *)ptr;

	dbSerialPrintln("onBtnTempPop: pageId=" + String(btn->getObjPid()) + " componentId=" + String(btn->getObjCid()) + " name=" + btn->getObjName());

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

void onBtnGoLightsPagePop(void *ptr)
{
	NexButton *btn = (NexButton *)ptr;

	dbSerialPrintln("onBtnGoLightsPagePop");
	dbSerialPrint("ptr=");
	dbSerialPrintln((uint32_t)ptr);
}

#define SETUP_DATETIME_YEAR         0
#define SETUP_DATETIME_MONTH        1
#define SETUP_DATETIME_DAY          2
#define SETUP_DATETIME_DAY_OF_WEEK  3
#define SETUP_DATETIME_HOUR         4
#define SETUP_DATETIME_MINUTE       5

uint8_t setup_datetime_current = SETUP_DATETIME_YEAR;

void onBtnGoTimePagePop(void *ptr)
{
	NexButton *btn = (NexButton *)ptr;

	dbSerialPrintln("onBtnbDateTimeNext");
	dbSerialPrint("ptr=");
	dbSerialPrintln((uint32_t)ptr);
	setup_datetime_current = SETUP_DATETIME_YEAR;

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


void onBtnbDateTimeNext(void *ptr)
{
	NexButton *btn = (NexButton *)ptr;

	dbSerialPrintln("onBtnbDateTimeNext");
	dbSerialPrint("ptr=");
	dbSerialPrintln((uint32_t)ptr);

	switch (setup_datetime_current) {
	case SETUP_DATETIME_YEAR:
		tYear.Set_font_color_pco(COLOR_YELLOW);
		tMonth.Set_font_color_pco(COLOR_RED);
		setup_datetime_current = SETUP_DATETIME_MONTH;
		break;
	case SETUP_DATETIME_MONTH:
		tMonth.Set_font_color_pco(COLOR_YELLOW);
		tDay.Set_font_color_pco(COLOR_RED);
		setup_datetime_current = SETUP_DATETIME_DAY;
		break;
	case SETUP_DATETIME_DAY:
		tDay.Set_font_color_pco(COLOR_YELLOW);
		tDayW.Set_font_color_pco(COLOR_RED);
		setup_datetime_current = SETUP_DATETIME_DAY_OF_WEEK;
		break;
	case SETUP_DATETIME_DAY_OF_WEEK:
		tDayW.Set_font_color_pco(COLOR_YELLOW);
		tHour.Set_font_color_pco(COLOR_RED);
		setup_datetime_current = SETUP_DATETIME_HOUR;
		break;
	case SETUP_DATETIME_HOUR:
		tHour.Set_font_color_pco(COLOR_YELLOW);
		tMinute.Set_font_color_pco(COLOR_RED);
		setup_datetime_current = SETUP_DATETIME_MINUTE;
		break;
	case SETUP_DATETIME_MINUTE:
		tMinute.Set_font_color_pco(COLOR_YELLOW);
		tYear.Set_font_color_pco(COLOR_RED);
		setup_datetime_current = SETUP_DATETIME_YEAR;
		break;
	default:
		tYear.Set_font_color_pco(COLOR_RED);
		setup_datetime_current = SETUP_DATETIME_YEAR;
		break;
	}
}

void onBtnbDateTimeSet(void *ptr)
{
	NexButton *btn = (NexButton *)ptr;

	dbSerialPrintln("onBtnbDateTimeSet");
	dbSerialPrint("ptr=");
	dbSerialPrintln((uint32_t)ptr);

	rtc.refresh();
	char buf[10];
	if (setup_datetime_current == SETUP_DATETIME_YEAR) {
		uint8_t year = rtc.year();
		if (year == 99)
			year = 0;
		else
			year++;

		rtc.set(rtc.second(), rtc.minute(), rtc.hour(), rtc.dayOfWeek(), rtc.day(), rtc.month(), year);

		sprintf(buf, "%02d", year);
		tYear.setText(buf);
	}
	if (setup_datetime_current == SETUP_DATETIME_MONTH) {
		uint8_t month = rtc.month();
		if (month == 12)
			month = 1;
		else
			month++;

		rtc.set(rtc.second(), rtc.minute(), rtc.hour(), rtc.dayOfWeek(), rtc.day(), month, rtc.year());

		sprintf(buf, "%02d", month);
		tMonth.setText(buf);
	}
	if (setup_datetime_current == SETUP_DATETIME_DAY) {
		uint8_t day = rtc.day();
		if (day == 31)
			day = 1;
		else
			day++;

		rtc.set(rtc.second(), rtc.minute(), rtc.hour(), rtc.dayOfWeek(), day, rtc.month(), rtc.year());

		sprintf(buf, "%02d", day);
		tDay.setText(buf);
	}
	if (setup_datetime_current == SETUP_DATETIME_DAY_OF_WEEK) {
		uint8_t dayOfWeek = rtc.dayOfWeek();
		if (dayOfWeek == 7)
			dayOfWeek = 1;
		else
			dayOfWeek++;

		rtc.set(rtc.second(), rtc.minute(), rtc.hour(), dayOfWeek, rtc.day(), rtc.month(), rtc.year());

		RtcHelper::dayOfWeekName(buf, dayOfWeek);
		tDayW.setText(buf);
	}
	if (setup_datetime_current == SETUP_DATETIME_HOUR) {
		uint8_t hour = rtc.hour();
		if (hour == 23)
			hour = 0;
		else
			hour++;

		rtc.set(rtc.second(), rtc.minute(), hour, rtc.dayOfWeek(), rtc.day(), rtc.month(), rtc.year());

		sprintf(buf, "%02d", hour);
		tHour.setText(buf);
	}
	if (setup_datetime_current == SETUP_DATETIME_MINUTE) {
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




DisplayControler::DisplayControler(Configuration* configuration)
{
  this->configuration = configuration;
  picWiFiStatus = new NexPicture(PG_MAIN, PIC_WIFI_STATUS_ID, PIC_WIFI_STATUS_NAME); 
}


void DisplayControler::init(){
	/* Set the baudrate which is for debug and communicate with Nextion screen. */
	nexInit(115200, UART1_BAUND, SERIAL_8N1, UART1_RX, UART1_TX);

	/* Register the pop event callback function of the current button component. */
	switchBathroomMainLight.attachPop(onSwitchBathroomMainLightPop, &switchBathroomMainLight);

	btnGoHeatingPage.attachPop(onBtnGoHeatingPagePop, &btnGoHeatingPage);
	btnGoLightsPage.attachPop(onBtnGoLightsPagePop, &btnGoLightsPage);
	btnGoTimePage.attachPop(onBtnGoTimePagePop, &btnGoTimePage);

	btnDayTempInc.attachPop(onBtnTempPop, &btnDayTempInc);
	btnDayTempDec.attachPop(onBtnTempPop, &btnDayTempDec);

	btnNightTempInc.attachPop(onBtnTempPop, &btnNightTempInc);
	btnNightTempDec.attachPop(onBtnTempPop, &btnNightTempDec);

	bDateTimeNext.attachPop(onBtnbDateTimeNext, &bDateTimeNext);
	bDateTimeSet.attachPop(onBtnbDateTimeSet, &bDateTimeSet);
}
  
void DisplayControler::loop(){
	nexLoop(nex_listen_list);
}
