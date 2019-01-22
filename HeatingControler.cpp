#include "HeatingControler.h"
#include "Configuration.h"
#include "Constants.h"

HeatingControler::HeatingControler()
{
	currDateTime.minute = 99;
	currDateTime.hour = 99;
	currDateTime.day = 99;
	currDateTime.month = 99;
	currDateTime.dayOfWeek = 99;

	currIndoorTemp = 99.9;
}
//----------------------------------------------------------------------------------------
void HeatingControler::init() {
	pinMode(GPIO_RELAY, OUTPUT);
	digitalWrite(GPIO_RELAY, LOW);
}
//----------------------------------------------------------------------------------------
void HeatingControler::onRefreshDateTime(const TDateTime& dateTime) {
	
	if (currDateTime.minute != dateTime.minute || currDateTime.hour != dateTime.hour || currDateTime.dayOfWeek != dateTime.dayOfWeek) {
		currDateTime = dateTime;
		char buf[5];
		RtcControler::dayOfWeekName(buf, currDateTime.dayOfWeek);
		Serial.printf("HeatingControler::onRefreshDateTime() %s %02d:%02d\r\n", buf, currDateTime.hour, dateTime.minute);
		refresh();
	}
}
//----------------------------------------------------------------------------------------
void HeatingControler::onRefreshIndoorTemp(float indoorTemp) {

	if (currIndoorTemp != indoorTemp) {
		currIndoorTemp = indoorTemp;
		Serial.printf("HeatingControler::onRefreshIndoorTemp() %.1f\r\n", currIndoorTemp);
		refresh();
	}
}
//----------------------------------------------------------------------------------------
void HeatingControler::refresh() {
	
	//TODO: oczywiście to poniżej do zmiany

	float dayTemp = configuration.getDayTemperature();

	Serial.printf("HeatingControler::refresh() currTemp: %.1f dayTemp: %.1f\r\n", currIndoorTemp, dayTemp);

	if (currIndoorTemp < dayTemp) {
		digitalWrite(GPIO_RELAY, HIGH);
	}
	else {
		digitalWrite(GPIO_RELAY, LOW);
	}
}

HeatingControler heatingControler;
