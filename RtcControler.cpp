
#include <uRTCLib.h>
#include "RtcControler.h"
#include "Constants.h"
#include "EventsHandler.h"

uRTCLib rtc(I2C_ADDRESS_D1307, I2C_ADDRESS_AT24C32);
//----------------------------------------------------------------------------------------
RtcControler::RtcControler()
{
}
//----------------------------------------------------------------------------------------
void RtcControler::init()
{
}
//----------------------------------------------------------------------------------------
void RtcControler::loop()
{
	rtc.refresh();	
	dateTime.year = rtc.year();
	dateTime.month = rtc.month();
	dateTime.dayOfWeek = rtc.dayOfWeek();
	dateTime.month = rtc.month();
	dateTime.day = rtc.day();
	dateTime.dayOfWeek = rtc.dayOfWeek();
	dateTime.hour = rtc.hour();
	dateTime.minute = rtc.minute();
	dateTime.second = rtc.second();
 
	eventsHandler.onRefreshDateTime(dateTime);
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