#pragma once

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#define HOSTNAME_PREFIX "SmartHomeControler-" ///< Hostename. The setup function adds the Chip ID at the end.



#define COLOR_RED 63488
#define COLOR_YELLOW 65504

#define PICTURE_WIFI_ON  5
#define PICTURE_WIFI_OFF 4
#define PICTURE_SWITCH_ON  7
#define PICTURE_SWITCH_OFF 6

//------------/*Strona główna*/------------
#define PG_MAIN                         0
#define BTN_GO_LIGHT_PG_ID              3
#define BTN_GO_LIGHT_PG_NAME            "bLights"
#define BTN_GO_HEATING_PG_ID            8
#define BTN_GO_HEATING_PG_NAME          "bHeating"
#define BTN_GO_TIME_PG_ID               23
#define BTN_GO_TIME_PG_NAME             "bTime"
#define PIC_WIFI_STATUS_ID            4
#define PIC_WIFI_STATUS_NAME            "picWiFi"
#define PIC_SWITCH_BATHROOM_MAIN_LIGHT_ID                  7
#define PIC_SWITCH_BATHROOM_MAIN_LIGHT_NAME  "picSwitchMain"
#define LBL_DAY_OF_MONTH_ID             22
#define LBL_DAY_OF_WEEK_ID              20
#define LBL_INDOOR_TEMP1_ID           16
#define LBL_INDOOR_TEMP1_NAME           "tInTemp1"
#define LBL_INDOOR_TEMP2_ID           17
#define LBL_INDOOR_TEMP2_NAME           "tInTemp2"
#define LBL_TIME1_ID                6
#define LBL_TIME1_NAME                  "tTime1"
#define LBL_TIME2_ID              19
#define LBL_TIME2_NAME                  "tTime2"

//------------/*Ogrzewanie*/------------
#define PG_HEATING_SETTING          1
/*Temperatura w dzień*/
#define BTN_DAY_TEMP_DEC_ID       16
#define BTN_DAY_TEMP_DEC_NAME   "b18"
#define BTN_DAY_TEMP_INC_ID       17
#define BTN_DAY_TEMP_INC_NAME   "b19"
#define LBL_DAY_TEMP_VALUE_ID     18      
#define LBL_DAY_TEMP_VALUE_NAME  "t9"
/*Temperatura w noc*/
#define BTN_NIGHT_TEMP_DEC_ID       32
#define BTN_NIGHT_TEMP_DEC_NAME  "b17"
#define BTN_NIGHT_TEMP_INC_ID       31
#define BTN_NIGHT_TEMP_INC_NAME  "b16"
#define LBL_NIGHT_TEMP_VALUE_ID     33
#define LBL_NIGHT_TEMP_VALUE_NAME "t8"

//------------/*Zegar*/----------------
#define PG_TIME            3
#define BTN_TIME_SET_ID       4
#define BTN_TIME_SET_NAME     "b6"
#define BTN_TIME_NEXT_ID      5
#define BTN_TIME_NEXT_NAME   "b7" 
#define LBL_TIME_YEAR       6
#define LBL_TIME_MONTH        8
#define LBL_TIME_DAY        10
#define LBL_TIME_DAY_OF_WEEK    11
#define LBL_TIME_HOUR       1
#define LBL_TIME_MINUTE       3  

#endif /* #ifndef __CONSTANTS_H__ */
