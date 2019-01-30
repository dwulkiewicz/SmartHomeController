#pragma once

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__


#define CORE_1 0
#define CORE_2 1

#define I2C_ADDRESS_BME280  0x76 //BME280
#define I2C_ADDRESS_SSD1306 0x3C //OLED
#define I2C_ADDRESS_D1307   0x68 //RTC
#define I2C_ADDRESS_AT24C32 0x50 //EEPROM

//ESP32 I2C
//#define I2C_SDA 4
//#define I2C_SCL 15
#define I2C_SCL 13
#define I2C_SDA 14

#define I2C_SCL_1 22
#define I2C_SDA_1 21




//ESP32 UART1
#define UART1_BAUND 57600
//#define UART1_TX 21
//#define UART1_RX 13
#define UART1_TX 16
#define UART1_RX 17

#define DS18B20_PIN 4

#define GPIO_CLK 26       
#define GPIO_DIO 27

#define GPIO_SW_1 36
#define GPIO_SW_2 39
#define GPIO_SW_3 34
#define GPIO_SW_4 32

#define GPIO_BUZZER 2

#define GPIO_RELAY 15

#define HOSTNAME_PREFIX "SmartHomeControler-" ///< Hostename. The setup function adds the Chip ID at the end.

#define TASK_INDOOR_TEMP_SENSOR_LOOP 5000 //ms
#define TASK_DATATIME_LOOP            500 //ms

#define SWITCH_BATH_1_ID	1
#define SWITCH_BATH_2_ID  2
#define SWITCH_BATH_3_ID  3

#define SW_OFF 0
#define SW_ON 1

#define HEATING_STATUS_COOL 0
#define HEATING_STATUS_HEAT 1

#define HEATING_PERIOD_DAY 0
#define HEATING_PERIOD_NIGHT 1

#define DAY_OF_WEEK_SUN 0
#define DAY_OF_WEEK_MO 1
#define DAY_OF_WEEK_TUE 2
#define DAY_OF_WEEK_WE 3
#define DAY_OF_WEEK_THU 4
#define DAY_OF_WEEK_FRI 5
#define DAY_OF_WEEK_SAT 6

#define COLOR_RED    63488
#define COLOR_YELLOW 65504

#define PICTURE_WIFI_ON  5
#define PICTURE_WIFI_OFF 4
#define PICTURE_SWITCH_ON  7
#define PICTURE_SWITCH_OFF 6

#define PIC_HEATING_HEAT 12
#define PIC_EMPTY        51 

#define PIC_HEATING_DAY		14
#define PIC_HEATING_NIGHT   13 

#define PIC_DAY_MON 18
#define PIC_DAY_TUE 19
#define PIC_DAY_WED 20
#define PIC_DAY_THU 21
#define PIC_DAY_FRI 22
#define PIC_DAY_SAT 23
#define PIC_DAY_SUN 24

#define PIC_MONTH_JAN  25
#define PIC_MONTH_FEB  26
#define PIC_MONTH_MAR  27
#define PIC_MONTH_APR  28
#define PIC_MONTH_MAY  29
#define PIC_MONTH_JUN  30
#define PIC_MONTH_JUL  31
#define PIC_MONTH_AUG  32
#define PIC_MONTH_SEPT 33
#define PIC_MONTH_OCT  34
#define PIC_MONTH_NOV  35
#define PIC_MONTH_DEC  36

#define PIC_DIGIT_0  37
#define PIC_DIGIT_1  38
#define PIC_DIGIT_2  39
#define PIC_DIGIT_3  40
#define PIC_DIGIT_4  41
#define PIC_DIGIT_5  42
#define PIC_DIGIT_6  43
#define PIC_DIGIT_7  44
#define PIC_DIGIT_8  45
#define PIC_DIGIT_9  46

//------------/*Strona gĹ‚Ăłwna*/------------
#define PG_MAIN_ID                       0
#define PG_MAIN_NAME              "pgMain"

#define OBJ_WIFI_STATUS_ID               4
#define OBJ_WIFI_STATUS_NAME     "picWiFi"

#define OBJ_HEATING_STATUS_ID            7
#define OBJ_HEATING_STATUS_NAME       "p0"
#define OBJ_HEATING_PERIOD_ID           15
#define OBJ_HEATING_PERIOD_NAME       "p1"
#define OBJ_HEATING_SET_TEMP_ID			11
#define OBJ_HEATING_SET_TEMP_NAME	  "t3"

#define OBJ_BATH_SW_1_ID				         5
#define OBJ_BATH_SW_1_NAME	  	"pBathSw1"
#define OBJ_BATH_SW_2_ID				        32
#define OBJ_BATH_SW_2_NAME			"pBathSw2"
#define OBJ_BATH_SW_3_ID				        33
#define OBJ_BATH_SW_3_NAME			"pBathSw3"

#define OBJ_DAY_OF_MONTH1_ID                     39
#define OBJ_DAY_OF_MONTH1_NAME				   "p4"
#define OBJ_DAY_OF_MONTH2_ID                     40
#define OBJ_DAY_OF_MONTH2_NAME				   "p5"

#define OBJ_MONTH_ID                             38
#define OBJ_MONTH_NAME                         "p3"
#define OBJ_DAY_OF_WEEK_ID                       37
#define OBJ_DAY_OF_WEEK_NAME                   "p2"

#define OBJ_TIME1_ID                             33
#define OBJ_TIME1_NAME                        "t10"
#define OBJ_TIME2_ID                             34
#define OBJ_TIME2_NAME                        "t11"
#define OBJ_TIME3_ID                             35
#define OBJ_TIME3_NAME                        "t12"
#define OBJ_TIME4_ID                             36
#define OBJ_TIME4_NAME                        "t13"

#define OBJ_INDOOR_TEMP1_ID                      16
#define OBJ_INDOOR_TEMP1_NAME            "tInTemp1"
#define OBJ_INDOOR_TEMP2_ID                      17
#define OBJ_INDOOR_TEMP2_NAME            "tInTemp2"
#define OBJ_INDOOR_HUMIDITY_ID                   14
#define OBJ_INDOOR_HUMIDITY_NAME               "t4"

#define OBJ_OUTDOOR_TEMP1_ID                     19
#define OBJ_OUTDOOR_TEMP1_NAME                 "t6"
#define OBJ_OUTDOOR_TEMP2_ID                     20
#define OBJ_OUTDOOR_TEMP2_NAME                 "t8"
#define OBJ_OUTDOOR_TEMP_SYMBOL_ID               21
#define OBJ_OUTDOOR_TEMP_SYMBOL_NAME           "t0"

#define OBJ_OUTDOOR_HUMIDITY_ID                 12
#define OBJ_OUTDOOR_HUMIDITY_NAME              "t2"

#define OBJ_OUTDOOR_PREASURE_ID                 11
#define OBJ_OUTDOOR_PREASURE_NAME              "t1"

//------------/*Ogrzewanie*/------------
#define PG_HEATING_ID			    1
#define PG_HEATING_NAME   "pgHeating"

/*Temperatura w nocy*/
#define OBJ_NIGHT_TEMP_DEC_ID       16
#define OBJ_NIGHT_TEMP_DEC_NAME   "b18"
#define OBJ_NIGHT_TEMP_INC_ID       17
#define OBJ_NIGHT_TEMP_INC_NAME   "b19"
#define OBJ_NIGHT_TEMP_VALUE_ID     18      
#define OBJ_NIGHT_TEMP_VALUE_NAME  "t9"
/*Temperatura w dzień*/
#define OBJ_DAY_TEMP_DEC_ID       32
#define OBJ_DAY_TEMP_DEC_NAME  "b17"
#define OBJ_DAY_TEMP_INC_ID       31
#define OBJ_DAY_TEMP_INC_NAME  "b16"
#define OBJ_DAY_TEMP_VALUE_ID     33
#define OBJ_DAY_TEMP_VALUE_NAME "t8"

#define OBJ_HEATING_WORKING_DAYS_MORNING_ON_VAL_ID        3
#define OBJ_HEATING_WORKING_DAYS_MORNING_ON_VAL_NAME   "t0"
#define OBJ_HEATING_WORKING_DAYS_MORNING_ON_DEC_ID        2
#define OBJ_HEATING_WORKING_DAYS_MORNING_ON_DEC_NAME   "b0"
#define OBJ_HEATING_WORKING_DAYS_MORNING_ON_INC_ID        4
#define OBJ_HEATING_WORKING_DAYS_MORNING_ON_INC_NAME   "b1"

#define OBJ_HEATING_WORKING_DAYS_MORNING_OFF_VAL_ID        8
#define OBJ_HEATING_WORKING_DAYS_MORNING_OFF_VAL_NAME   "t1"
#define OBJ_HEATING_WORKING_DAYS_MORNING_OFF_DEC_ID        7
#define OBJ_HEATING_WORKING_DAYS_MORNING_OFF_DEC_NAME   "b2"
#define OBJ_HEATING_WORKING_DAYS_MORNING_OFF_INC_ID        9
#define OBJ_HEATING_WORKING_DAYS_MORNING_OFF_INC_NAME   "b3"

#define OBJ_HEATING_WORKING_DAYS_AFTERNOON_ON_VAL_ID       11
#define OBJ_HEATING_WORKING_DAYS_AFTERNOON_ON_VAL_NAME   "t2"
#define OBJ_HEATING_WORKING_DAYS_AFTERNOON_ON_DEC_ID       10
#define OBJ_HEATING_WORKING_DAYS_AFTERNOON_ON_DEC_NAME   "b4"
#define OBJ_HEATING_WORKING_DAYS_AFTERNOON_ON_INC_ID       12
#define OBJ_HEATING_WORKING_DAYS_AFTERNOON_ON_INC_NAME   "b5"

#define OBJ_HEATING_WORKING_DAYS_AFTERNOON_OFF_VAL_ID       14
#define OBJ_HEATING_WORKING_DAYS_AFTERNOON_OFF_VAL_NAME   "t3"
#define OBJ_HEATING_WORKING_DAYS_AFTERNOON_OFF_DEC_ID       13
#define OBJ_HEATING_WORKING_DAYS_AFTERNOON_OFF_DEC_NAME   "b6"
#define OBJ_HEATING_WORKING_DAYS_AFTERNOON_OFF_INC_ID       15
#define OBJ_HEATING_WORKING_DAYS_AFTERNOON_OFF_INC_NAME   "b7"

#define OBJ_HEATING_WEEKEND_MORNING_ON_VAL_ID        20
#define OBJ_HEATING_WEEKEND_MORNING_ON_VAL_NAME    "t4"
#define OBJ_HEATING_WEEKEND_MORNING_ON_DEC_ID        19
#define OBJ_HEATING_WEEKEND_MORNING_ON_DEC_NAME    "b8"
#define OBJ_HEATING_WEEKEND_MORNING_ON_INC_ID        21
#define OBJ_HEATING_WEEKEND_MORNING_ON_INC_NAME    "b9"

#define OBJ_HEATING_WEEKEND_MORNING_OFF_VAL_ID       23
#define OBJ_HEATING_WEEKEND_MORNING_OFF_VAL_NAME   "t5"
#define OBJ_HEATING_WEEKEND_MORNING_OFF_DEC_ID       22
#define OBJ_HEATING_WEEKEND_MORNING_OFF_DEC_NAME  "b10"
#define OBJ_HEATING_WEEKEND_MORNING_OFF_INC_ID       24
#define OBJ_HEATING_WEEKEND_MORNING_OFF_INC_NAME  "b11"

#define OBJ_HEATING_WEEKEND_AFTERNOON_ON_VAL_ID       26
#define OBJ_HEATING_WEEKEND_AFTERNOON_ON_VAL_NAME   "t6"
#define OBJ_HEATING_WEEKEND_AFTERNOON_ON_DEC_ID       25
#define OBJ_HEATING_WEEKEND_AFTERNOON_ON_DEC_NAME  "b12"
#define OBJ_HEATING_WEEKEND_AFTERNOON_ON_INC_ID       27
#define OBJ_HEATING_WEEKEND_AFTERNOON_ON_INC_NAME  "b13"

#define OBJ_HEATING_WEEKEND_AFTERNOON_OFF_VAL_ID       29
#define OBJ_HEATING_WEEKEND_AFTERNOON_OFF_VAL_NAME   "t7"
#define OBJ_HEATING_WEEKEND_AFTERNOON_OFF_DEC_ID       28
#define OBJ_HEATING_WEEKEND_AFTERNOON_OFF_DEC_NAME   "b14"
#define OBJ_HEATING_WEEKEND_AFTERNOON_OFF_INC_ID       30
#define OBJ_HEATING_WEEKEND_AFTERNOON_OFF_INC_NAME   "b15"


//------------/*Oświetlenie*/----------------
#define PG_LIGHTS_ID                  2
#define PG_LIGHTS_NAME       "pgLights"

#define OBJ_LIGHT_BATH_MAIN_1_ID         6
#define OBJ_LIGHT_BATH_MAIN_1_NAME    "h1"
#define OBJ_LIGHT_BATH_ADD_1_ID          7
#define OBJ_LIGHT_BATH_ADD_1_NAME     "h4"
#define OBJ_LIGHT_BATH_TAPE_1_ID         4
#define OBJ_LIGHT_BATH_TAPE_1_NAME    "h2"
#define OBJ_LIGHT_BATH_TAPE_1_ID         4
#define OBJ_LIGHT_BATH_TAPE_1_NAME    "h2"
#define OBJ_LIGHT_BATH_RGB_V_1_ID        2
#define OBJ_LIGHT_BATH_RGB_V_1_NAME   "h0"
#define OBJ_LIGHT_BATH_RGB_H_1_ID        5
#define OBJ_LIGHT_BATH_RGB_H_1_NAME   "h3"
//------------/*Inne*/----------------
#define PG_OTHER_ID			          3
#define PG_OTHER_NAME         "pgOther"
/*Zegar*/
#define OBJ_TIME_SET_ID						 4
#define OBJ_TIME_SET_NAME			      "b6"
#define OBJ_TIME_NEXT_ID					 5
#define OBJ_TIME_NEXT_NAME				  "b7" 
#define OBJ_TIME_YEAR_ID			         6
#define OBJ_TIME_YEAR_NAME			 "lblYear"
#define OBJ_TIME_MONTH_ID                    8
#define OBJ_TIME_MONTH_NAME         "lblMonth"
#define OBJ_TIME_DAY_ID                     10
#define OBJ_TIME_DAY_NAME             "lblDay"
//#define OBJ_TIME_DAY_OF_WEEK_ID             11
//#define OBJ_TIME_DAY_OF_WEEK_NAME    "lblDayW"
#define OBJ_TIME_HOUR_ID				     1
#define OBJ_TIME_HOUR_NAME           "lblHour"
#define OBJ_TIME_MINUTE_ID			         3
#define OBJ_TIME_MINUTE_NAME       "lblMinute"

#define OBJ_HEATING_HISTERESIS_VAL_ID       16
#define OBJ_HEATING_HISTERESIS_VAL_NAME   "t0"
#define OBJ_HEATING_HISTERESIS_DEC_ID       14
#define OBJ_HEATING_HISTERESIS_DEC_NAME   "b18"
#define OBJ_HEATING_HISTERESIS_INC_ID       15
#define OBJ_HEATING_HISTERESIS_INC_NAME   "b19"


#endif /* #ifndef __CONSTANTS_H__ */
