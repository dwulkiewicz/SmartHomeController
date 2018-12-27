/************************************************************************/
/*                                                                      */
/*              ESP32 WiFi Lora - DS1307 Nextion                  */
/*              Hardware: ESP32 (Heltec Wifi-Lora-32)                   */
/*                                                                      */
/*              Author: Dariusz Wulkiewicz                              */
/*                      d.wulkiewicz@gmail.com                          */
/*                                                                      */
/*              Date: 12.2018                                           */
/************************************************************************/
#include "Arduino.h"
#include "Wire.h"
#include "uRTCLib.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <math.h>
#include "Nextion.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>

//#include "BluetoothSerial.h"

#include "Configuration.h"


//#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
//#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
//#endif

//BluetoothSerial SerialBT;

#define I2C_ADDRESS_SSD1306 0x3C
#define I2C_ADDRESS_D1307   0x68
#define I2C_ADDRESS_AT24C32 0x50

//ESP32 I2C
#define I2C_SDA 4
#define I2C_SCL 15

//ESP32 UART1
#define UART1_BAUND 57600
#define UART1_TX 21
#define UART1_RX 13

#define DS18B20_PIN 22

#define BUILT_LED 25

uRTCLib rtc(I2C_ADDRESS_D1307, I2C_ADDRESS_AT24C32);

//HardwareSerial Serial2(2);

#define ONE_WIRE_BUS DS18B20_PIN

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress insideThermometer;

unsigned long entry;

TaskHandle_t Task1;
TaskHandle_t Task2;


WiFiClient espClient;
PubSubClient client(espClient);
//const char* prefixClientID = "ESP32Client";
#define HOSTNAME_PREFIX "SmartHomeControler-" ///< Hostename. The setup function adds the Chip ID at the end.

/*
 * Declare a object [page id:0,component id:1, component name: "t0"]. 
 */

/*Ustawienie temp na dzień*/
#define PG_DAY_TEMP					1
#define BTN_DAY_TEMP_INC_ID			3
#define BTN_DAY_TEMP_DEC_ID			4
#define BTN_DAY_TEMP_MAIN_PG_ID		5
#define BTN_DAY_TEMP_NEXT_PG_ID		6
#define LBL_DAY_TEMP_TITLE_ID		1
#define LBL_DAY_TEMP_VALUE_ID		2	    

/*Ustawienie temp noc*/
#define PG_NIGHT_TEMP				2
#define BTN_NIGHT_TEMP_INC_ID		3
#define BTN_NIGHT_TEMP_DEC_ID		4
#define BTN_NIGHT_TEMP_PREV_PG_ID	6
#define BTN_NIGHT_TEMP_MAIN_PG_ID	5
#define BTN_NIGHT_TEMP_NEXT_PG_ID	7
#define LBL_NIGHT_TEMP_TITLE_ID		1
#define LBL_NIGHT_TEMP_VALUE_ID		2

/*Program 1*/
#define PAGE_3						3
#define BTN_PG3_NEXT_PAGE_ID		3
#define BTN_PG3_MAIN_PAGE_ID		1
#define BTN_PG3_PREV_PAGE_ID		2

/*Program 2*/


/*Strona ustawiania czasu*/
#define PG_TIME						4
#define BTN_TIME_SET_ID				6
#define BTN_TIME_NEXT_ID			7
#define BTN_TIME_MAIN_PAGE_ID		1
#define BTN_TIME_PREV_PAGE_ID		2
#define LBL_TIME_TITLE				14
#define LBL_TIME_YEAR				8
#define LBL_TIME_MONTH				10
#define LBL_TIME_DAY				12
#define LBL_TIME_DAY_OF_WEEK		13
#define LBL_TIME_HOUR				3
#define LBL_TIME_MINUTE				5	     

#define COLOR_RED 63488
#define COLOR_YELLOW 65504

#define PIC_WIFI_OFF_ID				1
#define PIC_WIFI_ON_ID				2

/*Strona główna*/
#define PG_MAIN                         0
#define BTN_MAIN_NEXT_PG_ID             7
#define PIC_WIFI_STATUS                 8
#define PIC_SWITCH_BATHROOM_MAIN_LIGHT 13
#define LBL_DAY_OF_MONTH_ID             2
#define LBL_DAY_OF_WEEK_ID              7
#define LBL_TIME_ID                    12
#define LBL_DAY_OF_MONTH_ID             2
#define LBL_INTERNAL_TEMP_ID            1

NexText tDayOfMonth	= NexText(PG_MAIN, LBL_DAY_OF_MONTH_ID, "tDayOfMonth");
NexText tDayOfWeek	= NexText(PG_MAIN, LBL_DAY_OF_WEEK_ID, "tDayOfWeek");
NexText tTime  = NexText(PG_MAIN, LBL_TIME_ID, "tTime");
NexText tInernalTemp	= NexText(PG_MAIN, LBL_INTERNAL_TEMP_ID, "tInernalTemp");
NexButton btnMainNextPage  = NexButton(PG_MAIN, BTN_MAIN_NEXT_PG_ID, "bMainNextPage");
NexPicture picWiFiStatus = NexPicture(PG_MAIN, PIC_WIFI_STATUS, "picWiFi");

NexPicture switchBathroomMainLight = NexPicture(PG_MAIN, PIC_SWITCH_BATHROOM_MAIN_LIGHT, "picSwitchMain");

/*Temperatura w dzień*/
NexText lblDayTempValue  = NexText(PG_DAY_TEMP, LBL_DAY_TEMP_VALUE_ID, "lDayTempVal");
NexButton btnDayTempInc = NexButton(PG_DAY_TEMP, BTN_DAY_TEMP_INC_ID, "bDayTempPlus");
NexButton btnDayTempDec = NexButton(PG_DAY_TEMP, BTN_DAY_TEMP_DEC_ID, "btnDayTempDec");
NexButton btnDayTempNextPage = NexButton(PG_DAY_TEMP, BTN_DAY_TEMP_NEXT_PG_ID, "bDayTempNextPg");

/*Temperatura w noc*/
NexText lblNightTempValue = NexText(PG_NIGHT_TEMP, LBL_NIGHT_TEMP_VALUE_ID, "lNightTempVal");
NexButton btnNightTempInc = NexButton(PG_NIGHT_TEMP, BTN_NIGHT_TEMP_INC_ID, "btnNightTempInc");
NexButton btnNightTempDec = NexButton(PG_NIGHT_TEMP, BTN_NIGHT_TEMP_DEC_ID, "btnNightTempDec");

/*Strona 3*/
NexButton btnNextOnPg3	= NexButton(PAGE_3, BTN_PG3_NEXT_PAGE_ID, "btnNextOnPg3");

NexDSButton btnProg01 = NexDSButton(PAGE_3, 4, "bt0");
NexDSButton btnProg02 = NexDSButton(PAGE_3, 5, "bt1");
NexDSButton btnProg03 = NexDSButton(PAGE_3, 6, "btnProg03");
NexDSButton btnProg04 = NexDSButton(PAGE_3, 5, "btnProg04");
NexDSButton btnProg05 = NexDSButton(PAGE_3, 5, "btnProg05");
NexDSButton btnProg06 = NexDSButton(PAGE_3, 5, "btnProg06");
NexDSButton btnProg07 = NexDSButton(PAGE_3, 5, "btnProg07");
NexDSButton btnProg08 = NexDSButton(PAGE_3, 5, "btnProg08");
NexDSButton btnProg09 = NexDSButton(PAGE_3, 5, "btnProg09");
NexDSButton btnProg10 = NexDSButton(PAGE_3, 5, "btnProg10");
NexDSButton btnProg11 = NexDSButton(PAGE_3, 5, "btnProg11");
NexDSButton btnProg12 = NexDSButton(PAGE_3, 5, "btnProg12");
NexDSButton btnProg13 = NexDSButton(PAGE_3, 5, "btnProg13");
NexDSButton btnProg14 = NexDSButton(PAGE_3, 5, "btnProg14");
NexDSButton btnProg15 = NexDSButton(PAGE_3, 5, "btnProg15");
NexDSButton btnProg16 = NexDSButton(PAGE_3, 5, "btnProg16");
NexDSButton btnProg17 = NexDSButton(PAGE_3, 5, "btnProg17");
NexDSButton btnProg18 = NexDSButton(PAGE_3, 5, "btnProg18");
NexDSButton btnProg19 = NexDSButton(PAGE_3, 5, "btnProg19");
NexDSButton btnProg20 = NexDSButton(PAGE_3, 5, "btnProg20");
NexDSButton btnProg21 = NexDSButton(PAGE_3, 5, "btnProg21");
NexDSButton btnProg22 = NexDSButton(PAGE_3, 5, "btnProg22");
NexDSButton btnProg23 = NexDSButton(PAGE_3, 5, "btnProg23");
NexDSButton btnProg24 = NexDSButton(PAGE_3, 5, "btnProg24");
NexDSButton btnProg25 = NexDSButton(PAGE_3, 5, "btnProg25");
NexDSButton btnProg26 = NexDSButton(PAGE_3, 5, "btnProg26");
NexDSButton btnProg27 = NexDSButton(PAGE_3, 5, "btnProg27");
NexDSButton btnProg28 = NexDSButton(PAGE_3, 5, "btnProg28");
NexDSButton btnProg29 = NexDSButton(PAGE_3, 5, "btnProg29");
NexDSButton btnProg30 = NexDSButton(PAGE_3, 5, "btnProg30");
NexDSButton btnProg31 = NexDSButton(PAGE_3, 5, "btnProg31");
NexDSButton btnProg33 = NexDSButton(PAGE_3, 5, "btnProg33");
NexDSButton btnProg34 = NexDSButton(PAGE_3, 5, "btnProg34");
NexDSButton btnProg35 = NexDSButton(PAGE_3, 5, "btnProg35");
NexDSButton btnProg36 = NexDSButton(PAGE_3, 5, "btnProg36");
NexDSButton btnProg37 = NexDSButton(PAGE_3, 5, "btnProg37");
NexDSButton btnProg38 = NexDSButton(PAGE_3, 5, "btnProg38");
NexDSButton btnProg39 = NexDSButton(PAGE_3, 5, "btnProg39");
NexDSButton btnProg40 = NexDSButton(PAGE_3, 5, "btnProg40");
NexDSButton btnProg41 = NexDSButton(PAGE_3, 5, "btnProg41");
NexDSButton btnProg42 = NexDSButton(PAGE_3, 5, "btnProg42");
NexDSButton btnProg43 = NexDSButton(PAGE_3, 5, "btnProg43");
NexDSButton btnProg44 = NexDSButton(PAGE_3, 5, "btnProg44");
NexDSButton btnProg45 = NexDSButton(PAGE_3, 5, "btnProg45");
NexDSButton btnProg46 = NexDSButton(PAGE_3, 5, "btnProg46");
NexDSButton btnProg47 = NexDSButton(PAGE_3, 5, "btnProg47");
NexDSButton btnProg48 = NexDSButton(PAGE_3, 5, "btnProg48");

/*Ustawianie czasu*/
NexPage page1 = NexPage(PG_TIME, BTN_TIME_MAIN_PAGE_ID, "btnPageMain");

NexText tYear	= NexText(PG_TIME, LBL_TIME_YEAR, "lblYear");
NexText tMonth	= NexText(PG_TIME, LBL_TIME_MONTH, "lblMonth");
NexText tDay	= NexText(PG_TIME, LBL_TIME_DAY, "lblDay");
NexText tDayW = NexText(PG_TIME, LBL_TIME_DAY_OF_WEEK, "lblDayW");
NexText tHour	= NexText(PG_TIME, LBL_TIME_HOUR, "lblHour");
NexText tMinute	= NexText(PG_TIME, LBL_TIME_MINUTE, "lblMinute");

NexButton bDateTimeNext = NexButton(PG_TIME, BTN_TIME_NEXT_ID, "btnTimeNext");
NexButton bDateTimeSet = NexButton(PG_TIME, BTN_TIME_SET_ID, "btnTimeSet");

/*
* Register a button object to the touch event list.
*/
NexTouch *nex_listen_list[] =
{
	&page1,
  &switchBathroomMainLight,
	&btnMainNextPage,
	&btnDayTempInc,
	&btnDayTempDec,
	&btnDayTempNextPage,
	&btnNightTempInc,
	&btnNightTempDec,
	&btnNextOnPg3,
	&bDateTimeNext,
	&bDateTimeSet,
	NULL
};


Configuration configuration;


#define SETUP_DATETIME_YEAR         0
#define SETUP_DATETIME_MONTH        1
#define SETUP_DATETIME_DAY          2 
#define SETUP_DATETIME_DAY_OF_WEEK  3 
#define SETUP_DATETIME_HOUR         4
#define SETUP_DATETIME_MINUTE       5

uint8_t setup_datetime_current = SETUP_DATETIME_YEAR;

String dayOfWeekName(uint8_t dayOfWeek) {
	switch (dayOfWeek) {
	case 1: return "poniedzialek";
	case 2: return "wtorek";
	case 3: return "sroda";
	case 4: return "czwartek";
	case 5: return "piatek";
	case 6: return "sobota";
	case 7: return "niedziela";
	}
  return "undefined";
}

String monthName(uint8_t month) {
  switch (month) {
  case 1: return "styczen";
  case 2: return "luty";
  case 3: return "marzec";
  case 4: return "kwiecień";
  case 5: return "maj";
  case 6: return "czerwiec";
  case 7: return "lipiec";
  case 8: return "sierpień";
  case 9: return "wrzesień";
  case 10: return "październik";
  case 11: return "listopad";
  case 12: return "grudzien";  
  }
  return "undefined";
}

bool switchBathroomMainLightState = false;
void onSwitchBathroomMainLightPop(void *ptr)
{
  dbSerialPrintln("onSwitchBathroomMainLightPop");
  switchBathroomMainLightState = !switchBathroomMainLightState;
  switchBathroomMainLight.setPic(switchBathroomMainLightState ? 8 : 7);  
}

void page1PopCallback(void *ptr)
{
	dbSerialPrintln("page1PopCallback");
	setup_datetime_current = SETUP_DATETIME_YEAR;
}

void onBtnMainNextPagePop(void *ptr)
{
  NexButton *btn = (NexButton *)ptr;

  dbSerialPrintln("onBtnMainNextPagePop");
  dbSerialPrint("ptr=");
  dbSerialPrintln((uint32_t)ptr);

  lblDayTempValue.setText(Configuration::temperatureAsString(configuration.getDayTemperature()).c_str());
}

void onBtnDayTempNextPagePop(void *ptr)
{
	NexButton *btn = (NexButton *)ptr;

	dbSerialPrintln("onBtnMainNextPagePop");
	dbSerialPrint("ptr=");
	dbSerialPrintln((uint32_t)ptr);

	lblNightTempValue.setText(Configuration::temperatureAsString(configuration.getNightTemperature()).c_str());
}

void onBtnTempPop(void *ptr) {
	NexButton *btn = (NexButton *)ptr;
	
	dbSerialPrintln("onBtnTempPop: pageId=" + String(btn->getObjPid()) + " componentId="+ String(btn->getObjCid()) + " name=" + btn->getObjName());
	switch(btn->getObjPid()){
	case PG_DAY_TEMP:
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
		}
		break;
	case PG_NIGHT_TEMP:
		switch (btn->getObjCid()) {
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
		break;
	}
}

void onBtnNextOnPg3(void *ptr)
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
	tDayOfWeek.setText(dayOfWeekName(rtc.dayOfWeek()).c_str());

	uint32_t state;
	bool prog[48];

	for (uint8_t i = 0; i < 48; i++) {
		prog[i] = 0;
	}

	btnProg01.getValue(&state);	prog[0] = state;
	/*
	btnProg02.getValue(&state);	prog[1] = state;
	btnProg03.getValue(&state);	prog[2] = state;
	btnProg04.getValue(&state);	prog[3] = state;
	btnProg05.getValue(&state);	prog[4] = state;
	btnProg06.getValue(&state);	prog[5] = state;
	btnProg07.getValue(&state);	prog[6] = state;
	btnProg08.getValue(&state);	prog[7] = state;
	btnProg09.getValue(&state);	prog[8] = state;
	btnProg10.getValue(&state);	prog[9] = state;
	btnProg11.getValue(&state);	prog[10] = state;
	btnProg12.getValue(&state);	prog[11] = state;
	btnProg13.getValue(&state);	prog[12] = state;
	btnProg14.getValue(&state);	prog[13] = state;
	btnProg15.getValue(&state);	prog[14] = state;
	btnProg16.getValue(&state);	prog[15] = state;
	btnProg17.getValue(&state);	prog[16] = state;
	btnProg18.getValue(&state);	prog[17] = state;
	btnProg19.getValue(&state);	prog[18] = state;
	btnProg20.getValue(&state);	prog[19] = state;
	btnProg21.getValue(&state);	prog[20] = state;
	btnProg22.getValue(&state);	prog[21] = state;
	btnProg23.getValue(&state);	prog[22] = state;
	btnProg24.getValue(&state);	prog[23] = state;
	btnProg25.getValue(&state);	prog[24] = state;
	btnProg26.getValue(&state);	prog[25] = state;
	btnProg27.getValue(&state);	prog[26] = state;
	btnProg28.getValue(&state);	prog[27] = state;
	btnProg29.getValue(&state);	prog[28] = state;
	btnProg30.getValue(&state);	prog[29] = state;
	*/
	char aaa[20];
	memset(aaa, 0, sizeof(aaa));
	btnProg01.getText(aaa, sizeof(aaa));
	dbSerialPrintln("btnProg01Text " + String(aaa) );
	dbSerialPrintln("getObjName " + String(btnProg01.getObjName()));

	//for (uint8_t i = 0; i < 48; i++) {
	//	dbSerialPrintln("btnProg " + String(i) + " = " + String(prog[i]));
	//}


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
      tDayOfWeek.Set_font_color_pco(COLOR_RED);
      setup_datetime_current = SETUP_DATETIME_DAY_OF_WEEK;
      break;
    case SETUP_DATETIME_DAY_OF_WEEK:
      tDayOfWeek.Set_font_color_pco(COLOR_YELLOW);
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

	if (setup_datetime_current == SETUP_DATETIME_YEAR) {
		uint8_t year = rtc.year();
		if (year == 99)
			year = 0;
		else
			year++;

		rtc.set(rtc.second(), rtc.minute(), rtc.hour(), rtc.dayOfWeek(), rtc.day(), rtc.month(), year);

		char buf[5];
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

		char buf[5];
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

		char buf[5];
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

		tDayOfWeek.setText(dayOfWeekName(dayOfWeek).c_str());
	}
	if (setup_datetime_current == SETUP_DATETIME_HOUR) {
		uint8_t hour = rtc.hour();
		if (hour == 23)
			hour = 0;
		else
			hour++;

		rtc.set(rtc.second(), rtc.minute(), hour, rtc.dayOfWeek(), rtc.day(), rtc.month(), rtc.year());

		char buf[5];
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

		char buf[5];
		sprintf(buf, "%02d", minute);
		tMinute.setText(buf);
	}
}

//----------------------------------------------------------------------------------------
void setup_wifi() {
	delay(10);
	// We start by connecting to a WiFi network
	Serial.printf("\r\nConnecting to %s\r\n", configuration.wifiSSID.c_str());

  // Set Hostname.
  char buf[15];
  uint64_t chipid = ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
  sprintf(buf, "%04x%08x", (uint16_t)(chipid >> 32) /*High 2 bytes*/, (uint32_t)chipid /*Low 4bytes*/);
  String hostname = HOSTNAME_PREFIX + String(buf); 

	WiFi.begin(configuration.wifiSSID.c_str(), configuration.wifiPassword.c_str());
  WiFi.setHostname(hostname.c_str());

	// Wait for connection
	while (WiFi.status() != WL_CONNECTED) {
		picWiFiStatus.setPic(PIC_WIFI_ON_ID);
		delay(250);
		picWiFiStatus.setPic(PIC_WIFI_OFF_ID);
		delay(250);
		Serial.print(".");
	}

	picWiFiStatus.setPic(PIC_WIFI_ON_ID);



	Serial.println("");
	Serial.println("WiFi connected");
	Serial.printf("IP address: %s Hostname: %s\r\n", WiFi.localIP().toString().c_str(), hostname.c_str());
}


//----------------------------------------------------------------------------------------
// function to print the temperature for a device
void ds18b20Init()
{
  // locate devices on the bus
  Serial.print("Locating DS18B20 devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount()+1, DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: ");
  if (sensors.isParasitePowerMode()) 
    Serial.println("ON");
  else 
    Serial.println("OFF");

  if (!sensors.getAddress(insideThermometer, 0)) 
  {
    Serial.println("Unable to find address for Device 0");
  }
  // show the addresses we found on the bus
  Serial.print("Device 0 Address: ");
  printDS18B20Address(insideThermometer);
  Serial.println();

  // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
  sensors.setResolution(insideThermometer, 12);

  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(insideThermometer), DEC);
  Serial.println();

  sensors.setWaitForConversion(false);
  Serial.println("DONE");  
}

//----------------------------------------------------------------------------------------
// function to print the temperature for a device
void printDS18B20Temperature(DeviceAddress deviceAddress)
{
	float tempC = sensors.getTempC(deviceAddress);
	Serial.print("Temp C: ");
	Serial.print(tempC);
}

//----------------------------------------------------------------------------------------
// function to print a device address
void printDS18B20Address(DeviceAddress deviceAddress)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		if (deviceAddress[i] < 16) Serial.print("0");
		Serial.print(deviceAddress[i], HEX);
	}
}

//-----------------------------------------------------------------------------------------
//Task1code: Core 1
void Task1code(void * pvParameters) {
	while (true) {
		sensors.requestTemperatures(); // Send the command to get temperatures
		delay(1000);
    rtc.refresh();
    
		//Serial.printf("Task1code() running on core %d\r\n", xPortGetCoreID());
		char buf[25];
   
    /*Intermal temperatur*/
		String temperature = dtostrf(round(sensors.getTempC(insideThermometer)*10.0) / 10.0, 2, 1/*2*/, buf);
		tInernalTemp.setText(temperature.c_str());

    /*Day of month*/
    String monthNameStr = monthName(rtc.month());    
		sprintf(buf, "%02d %s", rtc.day(), monthNameStr.c_str());
    tDayOfMonth.setText(buf);
    
    /*Time*/
    sprintf(buf, "%02d:%02d", rtc.hour(), rtc.minute());
		tTime.setText(buf);

    /*Day of Week*/
		tDayOfWeek.setText(dayOfWeekName(rtc.dayOfWeek()).c_str());
	}
}

//Task2code: Core 2
void Task2code(void * pvParameters) {
  while (true) {
		nexLoop(nex_listen_list);
		ArduinoOTA.handle();
	}
}


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void setup() {
	delay(2000);
	//SerialBT.begin("ESP32HmiNextionController"); //Bluetooth device name
  
	//create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
	xTaskCreatePinnedToCore(
		Task1code,   /* Task function. */
		"Task1",     /* name of task. */
		10000,       /* Stack size of task */
		NULL,        /* parameter of the task */
		1,           /* priority of the task */
		&Task1,      /* Task handle to keep track of created task */
		0);          /* pin task to core 0 */
	//delay(500);

	//create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1

	xTaskCreatePinnedToCore(
		Task2code,   /* Task function. */
		"Task2",     /* name of task. */
		10000,       /* Stack size of task */
		NULL,        /* parameter of the task */
		1,           /* priority of the task */
		&Task2,      /* Task handle to keep track of created task */
		1);          /* pin task to core 1 */
	//delay(500);


	//Serial2.begin(9600, SERIAL_8N1, UART1_RX, UART1_TX);

	/* Set the baudrate which is for debug and communicate with Nextion screen. */
	nexInit(115200, UART1_BAUND, SERIAL_8N1, UART1_RX, UART1_TX);

	Serial.printf("setup() running on core %d\r\n", xPortGetCoreID());

	configuration.init();

	//Serial.printf("Loaded wifiSSID: %s\r\n", configuration.wifiSSID.c_str());
	//Serial.printf("Loaded password: %s\r\n", configuration.wifiPassword.c_str());
	//Serial.printf("Loaded mqtt_server: %s\r\n", configuration.mqttServer.c_str());

  switchBathroomMainLight.attachPop(onSwitchBathroomMainLightPop,&switchBathroomMainLight);

	/* Register the pop event callback function of the current button component. */
  btnMainNextPage.attachPop(onBtnMainNextPagePop, &btnMainNextPage);
  
  btnDayTempInc.attachPop(onBtnTempPop, &btnDayTempInc);
  btnDayTempDec.attachPop(onBtnTempPop, &btnDayTempDec);
  btnDayTempNextPage.attachPop(onBtnDayTempNextPagePop,&btnDayTempNextPage);

  btnNightTempInc.attachPop(onBtnTempPop, &btnNightTempInc);
  btnNightTempDec.attachPop(onBtnTempPop, &btnNightTempDec);

  btnNextOnPg3.attachPop(onBtnNextOnPg3, &btnNextOnPg3);
  bDateTimeNext.attachPop(onBtnbDateTimeNext, &bDateTimeNext);
  bDateTimeSet.attachPop(onBtnbDateTimeSet, &bDateTimeSet);
  page1.attachPop(page1PopCallback);
  
  pinMode(BUILT_LED, OUTPUT);

  Wire.begin(I2C_SDA, I2C_SCL); 

  ds18b20Init();

  setup_wifi();



  ArduinoOTA
	  .onStart([]() {
	  String type;
	  if (ArduinoOTA.getCommand() == U_FLASH)
		  type = "sketch";
	  else // U_SPIFFS
		  type = "filesystem";

	  // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
	  Serial.println("Start updating " + type);
  })
	  .onEnd([]() {
	  Serial.println("\nEnd");
  })
	  .onProgress([](unsigned int progress, unsigned int total) {
	  Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  })
	  .onError([](ota_error_t error) {
	  Serial.printf("Error[%u]: ", error);
	  if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
	  else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
	  else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
	  else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
	  else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  ArduinoOTA.begin();


}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void loop() {}
