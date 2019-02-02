#ifndef __RtcControler_H__
#define __RtcControler_H__

#include <RTClib.h>
#include "Types.h"

class RtcControler
{
public:
	RtcControler();
	void init();
	void loop();
	void adjust(const DateTime& dateTime);
	DateTime now();
public:
	static String dayOfWeekName(uint8_t dayOfWeek);
	static String monthName(uint8_t month);
private:
	DateTime dateTime;
	RTC_DS1307 rtc;
};
extern RtcControler rtcControler;
#endif /* #ifndef __RtcControler_H__ */
