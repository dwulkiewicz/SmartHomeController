
#include <Wire.h>

#include "RtcControler.h"
#include "Constants.h"
#include "EventDispatcher.h"

//----------------------------------------------------------------------------------------
RtcControler::RtcControler(){
}
//----------------------------------------------------------------------------------------
void RtcControler::init(){
	if (!rtc.begin()) {
		Serial.print("RtcControler::init() Couldn't find RTC\r\n");
	}

	if (!rtc.isrunning()) {
		Serial.print("RtcControler::init() RTC is NOT running!\r\n");
		// following line sets the RTC to the date & time this sketch was compiled
		//rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
		// This line sets the RTC with an explicit date & time, for example to set
		// January 21, 2014 at 3am you would call:
		// rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
	}
}
//----------------------------------------------------------------------------------------
DateTime RtcControler::now() {
	return rtc.now();
}
//----------------------------------------------------------------------------------------
void RtcControler::loop(){
	dateTime = rtc.now();
	/*
	Serial.print(dateTime.year(), DEC);
	Serial.print('.');
	Serial.print(dateTime.month(), DEC);
	Serial.print('.');
	Serial.print(dateTime.day(), DEC);
	Serial.print(" (");
	Serial.print(dayOfWeekName(dateTime.dayOfTheWeek()));
	Serial.print(") ");
	Serial.print(dateTime.hour(), DEC);
	Serial.print(':');
	Serial.print(dateTime.minute(), DEC);
	Serial.print(':');
	Serial.print(dateTime.second(), DEC);
	Serial.println();
	*/
	eventDispatcher.onRefreshDateTime(dateTime);
}
//----------------------------------------------------------------------------------------
void RtcControler::adjust(const DateTime& dateTime) {
	rtc.adjust(dateTime);
}
//----------------------------------------------------------------------------------------
String RtcControler::dayOfWeekName(uint8_t dayOfWeek){
  switch (dayOfWeek) {
    case DAY_OF_WEEK_MO: return "Mo";
    case DAY_OF_WEEK_TUE: return "Tue";
    case DAY_OF_WEEK_WE: return "We"; 
    case DAY_OF_WEEK_THU: return "Thu"; 
    case DAY_OF_WEEK_FRI: return "Fri"; 
    case DAY_OF_WEEK_SAT: return "Sat"; 
    case DAY_OF_WEEK_SUN: return "Sun"; 
    default: return "???";  break;          
  }
}
//----------------------------------------------------------------------------------------
String RtcControler::monthName(uint8_t month) {
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
RtcControler rtcControler;
