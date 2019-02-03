#ifndef __HeatingControler_H__
#define __HeatingControler_H__

#include "RtcControler.h"

class HeatingControler
{
public:
	HeatingControler();
	void init(const DateTime& dateTime, float indoorTemp);
	void onRefreshDateTime(const DateTime& dateTime);
	void onRefreshIndoorTemp(float indoorTemp);
	void onConfigurationChange();
public:
	static String statusToStr(uint8_t value);
	static String periodToStr(uint8_t value);
private:
	void refresh();
	void updatePeriod();
	void updateRequiredTemperature();
	void updateStatus();
private:
	uint8_t status;
	uint8_t period;
	DateTime currDateTime;
	float currIndoorTemp;
	float requiredTemp;
};
extern HeatingControler heatingControler;
#endif /* #ifndef __HeatingControler_H__ */
