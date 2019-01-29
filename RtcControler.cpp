
#include <Wire.h>

#include "RtcControler.h"
#include "Constants.h"
#include "EventsHandler.h"

//----------------------------------------------------------------------------------------
RtcControler::RtcControler(){
}
//----------------------------------------------------------------------------------------
void RtcControler::init(){
	if (!rtc.begin()) {
		Serial.println("RtcControler::init() Couldn't find RTC");
	}

	if (!rtc.isrunning()) {
		Serial.println("RtcControler::init() RTC is NOT running!");
		// following line sets the RTC to the date & time this sketch was compiled
		rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
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
	eventsHandler.onRefreshDateTime(dateTime);
}
//----------------------------------------------------------------------------------------
void RtcControler::adjust(const DateTime& dateTime) {
	rtc.adjust(dateTime);
}
//----------------------------------------------------------------------------------------
void RtcControler::dayOfWeekName(char* buf, uint8_t dayOfWeek) {
	switch (dayOfWeek) {
	case DAY_OF_WEEK_MO: buf[0] = 'P'; buf[1] = 'n'; buf[2] = NULL; break;
	case DAY_OF_WEEK_TUE: buf[0] = 'W'; buf[1] = 't'; buf[2] = NULL; break;
	case DAY_OF_WEEK_WE: buf[0] = 0xA6; buf[1] = 'r'; buf[2] = NULL; break;
	case DAY_OF_WEEK_THU: buf[0] = 'C'; buf[1] = 'z'; buf[2] = 'w'; buf[3] = NULL; break;
	case DAY_OF_WEEK_FRI: buf[0] = 'P'; buf[1] = 't'; buf[2] = NULL; break;
	case DAY_OF_WEEK_SAT: buf[0] = 'S'; buf[1] = 'b'; buf[2] = NULL; break;
	case DAY_OF_WEEK_SUN: buf[0] = 'N'; buf[1] = 'd'; buf[2] = NULL; break;
	default: buf[0] = 'U'; buf[1] = 'n'; buf[2] = 'd'; buf[3] = 'e'; buf[4] = 'f'; buf[5] = NULL; break;
	}
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
