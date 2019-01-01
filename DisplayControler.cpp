#include "DisplayControler.h"
#include "Constants.h"

DisplayControler::DisplayControler(Configuration* configuration)
{
  this->configuration = configuration;
  picWiFiStatus = new NexPicture(PG_MAIN, PIC_WIFI_STATUS_ID, PIC_WIFI_STATUS_NAME); 
}


void DisplayControler::setup(){
    
}
  
void DisplayControler::loop(){
  
}
