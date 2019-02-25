#include "SwitchDevice.h"
#include "Constants.h"

SwitchDevice::SwitchDevice()
{
	state = SW_STATE_OFF;
}

SwitchDevice::~SwitchDevice()
{
}

bool SwitchDevice::setState(bool state){
  if(this->state != state){
    this->state = state;
    return true;
  }
  return false;
}

bool SwitchDevice::getState() {
	return state;
}

void SwitchDevice::invertState(){
  state = !state;
}
  
bool SwitchDevice::switchOn() {
	if (state == SW_STATE_ON)
		return false;
	state = SW_STATE_ON;
	return true;
}

bool SwitchDevice::switchOff() {
	if (state == SW_STATE_OFF)
		return false;
	state = SW_STATE_OFF;
	return true;
}
