#include "DisplayBuffer.h"



DisplayBuffer::DisplayBuffer(){
	dateTime.year = 0;
	dateTime.month = 0;
	dateTime.day = 0;
	dateTime.dayOfTheWeek = 0;
	dateTime.hour = 0;
	dateTime.minute = 0;
	dateTime.second = 0;
}

void DisplayBuffer::set(const DateTime& dateTime) {
	this->dateTime.year = dateTime.year() - 2000; 
	this->dateTime.month = dateTime.month();
	this->dateTime.day = dateTime.day();
	this->dateTime.dayOfTheWeek = dateTime.dayOfTheWeek();
	this->dateTime.hour = dateTime.hour();
	this->dateTime.minute = dateTime.minute();
	this->dateTime.second = dateTime.second();
}

