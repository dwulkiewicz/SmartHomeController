#include "HeatingControler.h"
#include "EventDispatcher.h"
#include "Configuration.h"
#include "Constants.h"

HeatingControler::HeatingControler()
{
	status = HEATING_STATUS_UNDEF;
	period = HEATING_PERIOD_UNDEF;
	requiredTemp = 0;
}
//----------------------------------------------------------------------------------------
void HeatingControler::init(const DateTime& dateTime, float indoorTemp) {
	pinMode(GPIO_RELAY, OUTPUT);
	digitalWrite(GPIO_RELAY, LOW);
	currDateTime = dateTime;
	currIndoorTemp = indoorTemp;
}
//----------------------------------------------------------------------------------------
void HeatingControler::onRefreshDateTime(const DateTime& dateTime) {
	if (currDateTime.minute() != dateTime.minute() || currDateTime.hour() != dateTime.hour() || currDateTime.dayOfTheWeek() != dateTime.dayOfTheWeek()) {
		currDateTime = dateTime;
		//String dayOfWeekStr = RtcControler::dayOfWeekName(currDateTime.dayOfWeek);  
		//logger.log(debug, "HeatingControler::onRefreshDateTime() %s %02d:%02d\r\n", dayOfWeekStr.c_str(), currDateTime.hour, dateTime.minute);
		refresh();
	}
}
//----------------------------------------------------------------------------------------
void HeatingControler::onRefreshIndoorTemp(float indoorTemp) {
	if (round(currIndoorTemp * 10) != round(indoorTemp * 10)) {
		currIndoorTemp = indoorTemp;
		//logger.log(debug, "HeatingControler::onRefreshIndoorTemp() %.1f\r\n", currIndoorTemp);
		refresh();
	}
}
//----------------------------------------------------------------------------------------
void HeatingControler::onConfigurationChange() {
	//logger.log(debug,"HeatingControler::onConfigurationChange()\r\n");
	refresh();
}
//----------------------------------------------------------------------------------------
void HeatingControler::refresh() {
	uint16_t minuteOfTheDay = currDateTime.hour() * 60 + currDateTime.minute();
	//weekend
	uint8_t currPeriod;
	if (currDateTime.dayOfTheWeek() == DAY_OF_WEEK_SAT || currDateTime.dayOfTheWeek() == DAY_OF_WEEK_SUN) {
		if ((configuration.getHeatingTime(HEATING_WEEKEND_MORNING_ON).minuteOfTheDay <= minuteOfTheDay && minuteOfTheDay <= configuration.getHeatingTime(HEATING_WEEKEND_MORNING_OFF).minuteOfTheDay) ||
			(configuration.getHeatingTime(HEATING_WEEKEND_AFTERNOON_ON).minuteOfTheDay <= minuteOfTheDay && minuteOfTheDay <= configuration.getHeatingTime(HEATING_WEEKEND_AFTERNOON_OFF).minuteOfTheDay))
			currPeriod = HEATING_PERIOD_DAY;
		else
			currPeriod = HEATING_PERIOD_NIGHT;
	}
	//working day
	else {
		if ((configuration.getHeatingTime(HEATING_WORKING_DAYS_MORNING_ON).minuteOfTheDay <= minuteOfTheDay && minuteOfTheDay <= configuration.getHeatingTime(HEATING_WORKING_DAYS_MORNING_OFF).minuteOfTheDay) ||
			(configuration.getHeatingTime(HEATING_WORKING_DAYS_AFTERNOON_ON).minuteOfTheDay <= minuteOfTheDay && minuteOfTheDay <= configuration.getHeatingTime(HEATING_WORKING_DAYS_AFTERNOON_OFF).minuteOfTheDay))
			currPeriod = HEATING_PERIOD_DAY;
		else
			currPeriod = HEATING_PERIOD_NIGHT;
	}
	float requiredTemp = (currPeriod == HEATING_PERIOD_DAY) ? configuration.getDayTemperature() : configuration.getNightTemperature();
	setRequiredTemp(requiredTemp);
	//grzeje
	if (status == HEATING_STATUS_HEAT) {
		requiredTemp += configuration.getHisteresisTemp() / 2.0;
		//i osiągnęło wymaganą temperaturę to wyłącz ....
		if (currIndoorTemp >= requiredTemp) {
			setStatus(HEATING_STATUS_COOL);
			digitalWrite(GPIO_RELAY, LOW);
		}
		//jeszcze nie nagrzało, ma grzać dalej
		else {
			//sprawdzenie czy faktycznie grzeje
		}
	}
	//chłodzi
	else {
		requiredTemp -= configuration.getHisteresisTemp() / 2.0;
		//i osiągnęło minimalną temperaturę to włącz ....
		if (currIndoorTemp <= requiredTemp) {
			setStatus(HEATING_STATUS_HEAT);
			digitalWrite(GPIO_RELAY, HIGH);
		}
		//jeszcze nie słodziło, ma schładzać dalej
		else {
			//sprawdzenie czy faktycznie grzeje
		}
	}
	setPeriod(currPeriod);

	String dayOfWeekStr = RtcControler::dayOfWeekName(currDateTime.dayOfTheWeek());
	logger.log(info, "HeatingControler::refresh() time: %s %02d:%02d period: %s status: %s requiredTemp: %.1f currTemp: %.1f \r\n", dayOfWeekStr.c_str(), currDateTime.hour(), currDateTime.minute(), periodToStr(period).c_str(), statusToStr(status).c_str(), requiredTemp, currIndoorTemp);
}
//----------------------------------------------------------------------------------------
void HeatingControler::setStatus(uint8_t status) {
	if (this->status != status) {
		this->status = status;
		//logger.log(debug,"HeatingControler::setStatus() status: %s \r\n", statusToStr(status).c_str());
		eventDispatcher.onHeatingStatusChange(status);
	}
}
//----------------------------------------------------------------------------------------
void HeatingControler::setPeriod(uint8_t period) {
	if (this->period != period) {
		this->period = period;
		eventDispatcher.onHeatingPeriodChange(period);
	}
}
//----------------------------------------------------------------------------------------
void HeatingControler::setRequiredTemp(float value) {
	if (this->requiredTemp != value) {
		this->requiredTemp = value;
		eventDispatcher.onHeatingRequiredTempChange(value);
	}
}
//----------------------------------------------------------------------------------------
String HeatingControler::statusToStr(uint8_t value) {
	switch (value) {
	case HEATING_STATUS_UNDEF: return "UNDEF";
	case HEATING_STATUS_HEAT: return "HEAT";
	case HEATING_STATUS_COOL: return "COOL";
	default: return "[" + String(value) + "]";
	}
}
//----------------------------------------------------------------------------------------
String HeatingControler::periodToStr(uint8_t value) {
	switch (value) {
	case HEATING_PERIOD_UNDEF: return "UNDEF";
	case HEATING_PERIOD_DAY: return "DAY";
	case HEATING_PERIOD_NIGHT: return "NIGHT";
	default: return "[" + String(value) + "]";
	}
}

HeatingControler heatingControler;
