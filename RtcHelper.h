#pragma once

#ifndef __RTC_HELPER_H__
#define __RTC_HELPER_H__

#include <uRTCLib.h>

class RtcHelper
{
public:
	RtcHelper();
	~RtcHelper();
public:
	static void dayOfWeekName(char* buf, uint8_t dayOfWeek);
	static String monthName(uint8_t month);
};


extern uRTCLib rtc;


#endif /* #ifndef __RTC_HELPER_H__ */
