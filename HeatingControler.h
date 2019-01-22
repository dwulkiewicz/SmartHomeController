#ifndef __HeatingControler_H__
#define __HeatingControler_H__

#include "RtcControler.h"

class HeatingControler
{
public:
	HeatingControler();
	void init();
	void onRefreshDateTime(const TDateTime& dateTime);
	void onRefreshIndoorTemp(float indoorTemp);
private:
	TDateTime currDateTime;
	float currIndoorTemp;
	void refresh();
};
extern HeatingControler heatingControler;
#endif /* #ifndef __HeatingControler_H__ */
