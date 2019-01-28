#include "HeatingControler.h"
#include "EventsHandler.h"
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

  status = HEATING_STATUS_COOL;
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
    //String dayOfWeekStr = RtcControler::dayOfWeekName(currDateTime.dayOfWeek);  
		//Serial.printf("HeatingControler::onRefreshDateTime() %s %02d:%02d\r\n", dayOfWeekStr.c_str(), currDateTime.hour, dateTime.minute);
		refresh();
	}
}
//----------------------------------------------------------------------------------------
void HeatingControler::onRefreshIndoorTemp(float indoorTemp) {
	if (round(currIndoorTemp * 10) != round(indoorTemp * 10) {
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
  String dayOfWeekStr = RtcControler::dayOfWeekName(currDateTime.dayOfWeek);   
	Serial.printf("HeatingControler::refresh() time: %s %02d:%02d currTemp: %f dayTemp: %f histeresisTemp: %f\r\n", dayOfWeekStr.c_str(), currDateTime.hour, currDateTime.minute, currIndoorTemp, dayTemp, histeresisTemp);
	
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
