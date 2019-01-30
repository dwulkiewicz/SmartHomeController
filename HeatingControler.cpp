#include "HeatingControler.h"
#include "EventsHandler.h"
#include "Configuration.h"
#include "Constants.h"

HeatingControler::HeatingControler()
{
	status = HEATING_STATUS_COOL;	
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
	if (currDateTime.minute() != dateTime.minute() || currDateTime.hour() != dateTime.hour() || currDateTime.dayOfTheWeek() != dateTime.dayOfTheWeek() ) {
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
void HeatingControler::onConfigurationChange(){
	//logger.log(debug,"HeatingControler::onConfigurationChange()\r\n");
	refresh();  
}
//----------------------------------------------------------------------------------------
void HeatingControler::refresh() {	
	uint16_t minuteOfTheDay = currDateTime.hour() * 60 + currDateTime.minute();
	//weekend
	if (currDateTime.dayOfTheWeek() == DAY_OF_WEEK_SAT || currDateTime.dayOfTheWeek() == DAY_OF_WEEK_SUN) {
		if ((configuration.getHeatingTime(HEATING_WORKING_DAYS_MORNING_ON).minuteOfTheDay <= minuteOfTheDay && minuteOfTheDay <= configuration.getHeatingTime(HEATING_WORKING_DAYS_MORNING_OFF).minuteOfTheDay) ||
			(configuration.getHeatingTime(HEATING_WORKING_DAYS_AFTERNOON_ON).minuteOfTheDay <= minuteOfTheDay && minuteOfTheDay <= configuration.getHeatingTime(HEATING_WORKING_DAYS_AFTERNOON_OFF).minuteOfTheDay))
			period = HEATING_PERIOD_DAY;
		else
			period = HEATING_PERIOD_NIGHT;
	}
	//working day
	else {
		if ((configuration.getHeatingTime(HEATING_WEEKEND_MORNING_ON).minuteOfTheDay <= minuteOfTheDay && minuteOfTheDay <= configuration.getHeatingTime(HEATING_WEEKEND_MORNING_OFF).minuteOfTheDay) ||
			(configuration.getHeatingTime(HEATING_WEEKEND_AFTERNOON_ON).minuteOfTheDay <= minuteOfTheDay && minuteOfTheDay <= configuration.getHeatingTime(HEATING_WEEKEND_AFTERNOON_OFF).minuteOfTheDay))
			period = HEATING_PERIOD_DAY;
		else
			period = HEATING_PERIOD_NIGHT;
	}

	float requiredTemp = (period == HEATING_PERIOD_DAY) ? configuration.getDayTemperature() : configuration.getNightTemperature();	   
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

	String dayOfWeekStr = RtcControler::dayOfWeekName(currDateTime.dayOfTheWeek());
	logger.log(info, "HeatingControler::refresh() time: %s %02d:%02d period: %s status: %s requiredTemp: %.1f currTemp: %.1f \r\n", dayOfWeekStr.c_str(), currDateTime.hour(), currDateTime.minute(), periodToStr(period).c_str(), statusToStr(status).c_str(), requiredTemp, currIndoorTemp);
}
//----------------------------------------------------------------------------------------
void HeatingControler::setStatus(uint8_t status){
  if(this->status != status){
    this->status = status;
    eventsHandler.onHeatingStatusChange(status);    
  }  
}  
//----------------------------------------------------------------------------------------
String HeatingControler::statusToStr(uint8_t value) {
  switch(value){
    case HEATING_STATUS_HEAT: return "HEATING_STATUS_HEAT"; 
    case HEATING_STATUS_COOL: return "HEATING_STATUS_COOL";
    default: return "HEATING_STATUS_???";    
  }
}
//----------------------------------------------------------------------------------------
String HeatingControler::periodToStr(uint8_t value) {
	switch (value) {
	case HEATING_PERIOD_DAY: return "HEATING_PERIOD_DAY";
	case HEATING_PERIOD_NIGHT: return "HEATING_PERIOD_NIGHT";
	default: return "HEATING_PERIOD_???";
	}
}

HeatingControler heatingControler;
