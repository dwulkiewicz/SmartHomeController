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
		//Serial.printf("HeatingControler::onRefreshDateTime() %s %02d:%02d\r\n", dayOfWeekStr.c_str(), currDateTime.hour, dateTime.minute);
		refresh();
	}
}
//----------------------------------------------------------------------------------------
void HeatingControler::onRefreshIndoorTemp(float indoorTemp) {
	if (round(currIndoorTemp * 10) != round(indoorTemp * 10)) {
		currIndoorTemp = indoorTemp;
		//Serial.printf("HeatingControler::onRefreshIndoorTemp() %.1f\r\n", currIndoorTemp);
		refresh();
	}
}
//----------------------------------------------------------------------------------------
void HeatingControler::onConfigurationChange(){
	//Serial.printf("HeatingControler::onConfigurationChange()\r\n");
	refresh();  
}
//----------------------------------------------------------------------------------------
void HeatingControler::refresh() {	
	float dayTemp = configuration.getDayTemperature();
	float histeresisTemp = configuration.getHisteresisTemp();
	String dayOfWeekStr = RtcControler::dayOfWeekName(currDateTime.dayOfTheWeek() );   
	Serial.printf("HeatingControler::refresh() time: %s %02d:%02d currTemp: %.1f dayTemp: %.1f histeresisTemp: %.1f\r\n", dayOfWeekStr.c_str(), currDateTime.hour(), currDateTime.minute(), currIndoorTemp, dayTemp, histeresisTemp);
	
	//TODO: oczywiście to poniżej do zmiany  
	
	if (currIndoorTemp < dayTemp){
		setStatus(HEATING_STATUS_HEAT);
		digitalWrite(GPIO_RELAY, HIGH);
	}
	else {
		setStatus(HEATING_STATUS_COOL);
		digitalWrite(GPIO_RELAY, LOW);
	}
}
//----------------------------------------------------------------------------------------
void HeatingControler::setStatus(uint8_t status){
  if(this->status != status){
    this->status = status;
    eventsHandler.onHeatingStatusChange(status);    
  }  
}  
//----------------------------------------------------------------------------------------
String HeatingControler::heatingStatusName(uint8_t status) {
  switch(status){
    case HEATING_STATUS_HEAT: return "HEATING_STATUS_HEAT"; 
    case HEATING_STATUS_COOL: return "HEATING_STATUS_COOL";
    default: return "HEATING_STATUS_???";    
  }
}



HeatingControler heatingControler;
