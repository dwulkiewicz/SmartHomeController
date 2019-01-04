
#include "RtcHelper.h"
#include "Constants.h"



uRTCLib rtc(I2C_ADDRESS_D1307, I2C_ADDRESS_AT24C32);


RtcHelper::RtcHelper()
{
}


RtcHelper::~RtcHelper()
{
}


void RtcHelper::dayOfWeekName(char* buf, uint8_t dayOfWeek) {
	switch (dayOfWeek) {
	case 1: buf[0] = 'P'; buf[1] = 'n'; buf[2] = NULL; break;
	case 2: buf[0] = 'W'; buf[1] = 't'; buf[2] = NULL; break;
	case 3: buf[0] = 0xA6; buf[1] = 'r'; buf[2] = NULL; break;
	case 4: buf[0] = 'C'; buf[1] = 'z'; buf[2] = 'w'; buf[3] = NULL; break;
	case 5: buf[0] = 'P'; buf[1] = 't'; buf[2] = NULL; break;
	case 6: buf[0] = 'S'; buf[1] = 'b'; buf[2] = NULL; break;
	case 7: buf[0] = 'N'; buf[1] = 'd'; buf[2] = NULL; break;
	default: buf[0] = 'U'; buf[1] = 'n'; buf[2] = 'd'; buf[3] = 'e'; buf[4] = 'f'; buf[5] = NULL; break;
	}
}

String RtcHelper::monthName(uint8_t month) {
	switch (month) {
	case 1: return "styczen";
	case 2: return "luty";
	case 3: return "marzec";
	case 4: return "kwiecieñ";
	case 5: return "maj";
	case 6: return "czerwiec";
	case 7: return "lipiec";
	case 8: return "sierpieñ";
	case 9: return "wrzesieñ";
	case 10: return "paŸdziernik";
	case 11: return "listopad";
	case 12: return "grudzien";
	}
	return "undefined";
}
