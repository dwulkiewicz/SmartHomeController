#ifndef __RtcControler_H__
#define __RtcControler_H__

#include <uRTCLib.h>

typedef struct {
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t dayOfWeek;   // day of week
	uint8_t day;
	uint8_t month;
	uint8_t year;   // offset from 2000; 
} 	TDateTime;


class RtcControler
{
public:
	RtcControler();
	void init();
	void loop();
public:
	static void dayOfWeekName(char* buf, uint8_t dayOfWeek);
  static String dayOfWeekName(uint8_t dayOfWeek);
	static String monthName(uint8_t month);
private:
	TDateTime dateTime;
};
extern RtcControler rtcControler;
#endif /* #ifndef __RtcControler_H__ */



extern uRTCLib rtc;
