#include "OnOffDevice.h"
#include "Constants.h"

OnOffDevice::OnOffDevice()
{
	state = SW_OFF;
}

OnOffDevice::~OnOffDevice()
{
}

bool OnOffDevice::setState(bool state){
  if(this->state != state){
    this->state = state;
    return true;
  }
  return false;
}

bool OnOffDevice::getState() {
	return state;
}

void OnOffDevice::invertState(){
  state = !state;
}
  
bool OnOffDevice::switchOn() {
	if (state == SW_ON)
		return false;
	state = SW_ON;
	return true;
}

bool OnOffDevice::switchOff() {
	if (state == SW_OFF)
		return false;
	state = SW_OFF;
	return true;
}
