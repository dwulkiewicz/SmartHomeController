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
#include <math.h>

#include <ArduinoOTA.h>
#include "Nextion.h"

#include "Wire.h"
#include "uRTCLib.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#include "Configuration.h"

#include "Constants.h"
#include "DisplayControler.h"
#include "NetworkControler.h"

#define CORE_1 0
#define CORE_2 1

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

#define ONE_WIRE_BUS DS18B20_PIN

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress insideThermometer;


NexText tDayOfMonth	= NexText(PG_MAIN, LBL_DAY_OF_MONTH_ID, "tDayOfMonth");
NexText tDayOfWeek	= NexText(PG_MAIN, LBL_DAY_OF_WEEK_ID, "tDayOfWeek");

NexText tTime1  = NexText(PG_MAIN, LBL_TIME1_ID, LBL_TIME1_NAME);
NexText tTime2  = NexText(PG_MAIN, LBL_TIME2_ID, LBL_TIME2_NAME);
NexText tIndoorTemp1	= NexText(PG_MAIN, LBL_INDOOR_TEMP1_ID, LBL_INDOOR_TEMP1_NAME);
NexText tIndoorTemp2  = NexText(PG_MAIN, LBL_INDOOR_TEMP2_ID, LBL_INDOOR_TEMP2_NAME);

NexButton btnGoHeatingPage  = NexButton(PG_MAIN, BTN_GO_HEATING_PG_ID, BTN_GO_HEATING_PG_NAME);
NexButton btnGoLightsPage = NexButton(PG_MAIN, BTN_GO_LIGHT_PG_ID, BTN_GO_LIGHT_PG_NAME);
NexButton btnGoTimePage  = NexButton(PG_MAIN, BTN_GO_TIME_PG_ID, BTN_GO_TIME_PG_NAME);



NexPicture switchBathroomMainLight = NexPicture(PG_MAIN, PIC_SWITCH_BATHROOM_MAIN_LIGHT_ID, PIC_SWITCH_BATHROOM_MAIN_LIGHT_NAME);

//------------/*Ogrzewanie*/------------
/*Temperatura w dzień*/
NexText lblDayTempValue  = NexText(PG_HEATING_SETTING, LBL_DAY_TEMP_VALUE_ID, LBL_DAY_TEMP_VALUE_NAME);
NexButton btnDayTempDec = NexButton(PG_HEATING_SETTING, BTN_DAY_TEMP_DEC_ID, BTN_DAY_TEMP_DEC_NAME);
NexButton btnDayTempInc = NexButton(PG_HEATING_SETTING, BTN_DAY_TEMP_INC_ID, BTN_DAY_TEMP_INC_NAME);
/*Temperatura w noc*/
NexText lblNightTempValue = NexText(PG_HEATING_SETTING, LBL_NIGHT_TEMP_VALUE_ID, LBL_NIGHT_TEMP_VALUE_NAME);
NexButton btnNightTempDec = NexButton(PG_HEATING_SETTING, BTN_NIGHT_TEMP_DEC_ID, BTN_NIGHT_TEMP_DEC_NAME);
NexButton btnNightTempInc = NexButton(PG_HEATING_SETTING, BTN_NIGHT_TEMP_INC_ID, BTN_NIGHT_TEMP_INC_NAME);

/*Ustawianie czasu*/
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


Configuration configuration;
DisplayControler displayControler(&configuration);
NetworkControler networkControler(&configuration,&displayControler);


#define SETUP_DATETIME_YEAR         0
#define SETUP_DATETIME_MONTH        1
#define SETUP_DATETIME_DAY          2 
#define SETUP_DATETIME_DAY_OF_WEEK  3 
#define SETUP_DATETIME_HOUR         4
#define SETUP_DATETIME_MINUTE       5

uint8_t setup_datetime_current = SETUP_DATETIME_YEAR;

String dayOfWeekName(uint8_t dayOfWeek) {
	switch (dayOfWeek) {
	case 1: return "Pn";
	case 2: return "Wt";
	case 3: return "Śr";
	case 4: return "Czw";
	case 5: return "Pt";
	case 6: return "Sb";
	case 7: return "Nd";
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
	
	dbSerialPrintln("onBtnTempPop: pageId=" + String(btn->getObjPid()) + " componentId="+ String(btn->getObjCid()) + " name=" + btn->getObjName());

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
	tDayOfWeek.setText(dayOfWeekName(rtc.dayOfWeek()).c_str());
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
void TaskTempSensorLoop(void * pvParameters) {
  Serial.printf("TaskTempSensorLoop() running on core %d\r\n", xPortGetCoreID());
	while (true) {
		sensors.requestTemperatures(); // Send the command to get temperatures
		delay(1000);
    char buf[25];
      /*Indoor temperature*/
    uint16_t t = round(sensors.getTempC(insideThermometer)*10.0);
    String t1 = String(t/10);    
    String t2 = String(t % 10);
    tIndoorTemp1.setText(t1.c_str());
    tIndoorTemp2.setText(t2.c_str());
	}
}
//-----------------------------------------------------------------------------------------
void TaskTimeLoop(void * pvParameters) {
  Serial.printf("TaskTimeLoop() running on core %d\r\n", xPortGetCoreID());
  while (true) {    

  //Serial.printf("TaskTimeLoop() running on core %d\r\n", xPortGetCoreID());
    
    rtc.refresh();
    char buf[25];
    /*Day of month*/
    String monthNameStr = monthName(rtc.month());    
    sprintf(buf, "%02d %s", rtc.day(), monthNameStr.c_str());
    tDayOfMonth.setText(buf);    
    /*Day of week*/
    tDayOfWeek.setText(dayOfWeekName(rtc.dayOfWeek()).c_str());
    /*Time*/
    sprintf(buf, "%02d", rtc.hour());
    tTime1.setText(buf);
    sprintf(buf, "%02d", rtc.minute());
    tTime2.setText(buf);
    delay(1000);      
  }
}
//-----------------------------------------------------------------------------------------
void TaskNextionLoop(void * pvParameters) {
  Serial.printf("TaskNextionLoop() running on core %d\r\n", xPortGetCoreID());    
	while (true) { 
		nexLoop(nex_listen_list);
    vTaskDelay(10);       
	}
}
//-----------------------------------------------------------------------------------------
void TaskOTALoop(void * pvParameters) {
  Serial.printf("TaskOTALoop() running on core %d\r\n", xPortGetCoreID());    
  while (true) {
    ArduinoOTA.handle();
    vTaskDelay(10);  
  }
}

//-----------------------------------------------------------------------------------------
void TaskNetworkControlerLoop(void * pvParameters) {
  Serial.printf("TaskOTALoop() running on core %d\r\n", xPortGetCoreID());    
  while (true) {
    networkControler.loop();
    vTaskDelay(10);  
  }
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void setup() {
	delay(2000);
  Serial.printf("setup() running on core %d\r\n", xPortGetCoreID());
  configuration.init();

  xTaskCreatePinnedToCore(TaskOTALoop,"TaskOTALoop", 4096, NULL, 1, NULL, CORE_1);
  xTaskCreatePinnedToCore(TaskNextionLoop,"TaskNextionLoop", 4096, NULL, 1, NULL, CORE_2);
  xTaskCreatePinnedToCore(TaskTimeLoop,"TaskTempSensorLoop", 4096, NULL, 1, NULL, CORE_1);
  xTaskCreatePinnedToCore(TaskTempSensorLoop,"TaskTempSensorLoop", 4096, NULL, 1, NULL, CORE_1);
  xTaskCreatePinnedToCore(TaskNetworkControlerLoop,"TaskNetworkControlerLoop", 4096, NULL, 1, NULL, CORE_1);
  

	//Serial2.begin(9600, SERIAL_8N1, UART1_RX, UART1_TX);

	/* Set the baudrate which is for debug and communicate with Nextion screen. */
	nexInit(115200, UART1_BAUND, SERIAL_8N1, UART1_RX, UART1_TX);


	/* Register the pop event callback function of the current button component. */
  switchBathroomMainLight.attachPop(onSwitchBathroomMainLightPop,&switchBathroomMainLight);
  
  btnGoHeatingPage.attachPop(onBtnGoHeatingPagePop, &btnGoHeatingPage);
  btnGoLightsPage.attachPop(onBtnGoLightsPagePop, &btnGoLightsPage);
  btnGoTimePage.attachPop(onBtnGoTimePagePop, &btnGoTimePage);

  btnDayTempInc.attachPop(onBtnTempPop, &btnDayTempInc);
  btnDayTempDec.attachPop(onBtnTempPop, &btnDayTempDec);

  btnNightTempInc.attachPop(onBtnTempPop, &btnNightTempInc);
  btnNightTempDec.attachPop(onBtnTempPop, &btnNightTempDec);

  bDateTimeNext.attachPop(onBtnbDateTimeNext, &bDateTimeNext);
  bDateTimeSet.attachPop(onBtnbDateTimeSet, &bDateTimeSet);

  
  pinMode(BUILT_LED, OUTPUT);

  Wire.begin(I2C_SDA, I2C_SCL); 

  ds18b20Init();

  networkControler.setup();


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
void loop(){
  //nope, do nothing here
  vTaskDelay(portMAX_DELAY); 
}
