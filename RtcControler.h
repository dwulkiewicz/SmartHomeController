#ifndef __RtcControler_H__
#define __RtcControler_H__

#include <uRTCLib.h>
#include "Types.h"

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
