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
#define I2C_SCL 13 //21
#define I2C_SDA 14 //22


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


#define HOSTNAME_PREFIX "SmartHomeControler-" ///< Hostename. The setup function adds the Chip ID at the end.


#define COLOR_RED 63488
#define COLOR_YELLOW 65504

#define PICTURE_WIFI_ON  5
#define PICTURE_WIFI_OFF 4
#define PICTURE_SWITCH_ON  7
#define PICTURE_SWITCH_OFF 6

#define PIC_DAY_MON 21
#define PIC_DAY_TUE 22
#define PIC_DAY_WED 23
#define PIC_DAY_THU 24
#define PIC_DAY_FRI 25
#define PIC_DAY_SAT 26
#define PIC_DAY_SUN 27

#define PIC_MONTH_JAN  28
#define PIC_MONTH_FEB  29
#define PIC_MONTH_MAR  30
#define PIC_MONTH_APR  31
#define PIC_MONTH_MAY  32
#define PIC_MONTH_JUN  33
#define PIC_MONTH_JUL  34
#define PIC_MONTH_AUG  35
#define PIC_MONTH_SEPT 36
#define PIC_MONTH_OCT  37
#define PIC_MONTH_NOV  38
#define PIC_MONTH_DEC  39

#define PIC_DIGIT_0  40
#define PIC_DIGIT_1  41
#define PIC_DIGIT_2  42
#define PIC_DIGIT_3  43
#define PIC_DIGIT_4  44
#define PIC_DIGIT_5  45
#define PIC_DIGIT_6  46
#define PIC_DIGIT_7  47
#define PIC_DIGIT_8  48
#define PIC_DIGIT_9  49

//------------/*Strona główna*/------------
#define PG_MAIN                                            0

#define BTN_GO_LIGHT_PG_ID                                 3
#define BTN_GO_LIGHT_PG_NAME                       "bLights"
#define BTN_GO_HEATING_PG_ID                               8
#define BTN_GO_HEATING_PG_NAME                    "bHeating"
#define BTN_GO_TIME_PG_ID                                 23
#define BTN_GO_TIME_PG_NAME                          "bTime"
#define PIC_WIFI_STATUS_ID                                 4
#define PIC_WIFI_STATUS_NAME                       "picWiFi"
#define PIC_SWITCH_BATHROOM_MAIN_LIGHT_ID                  7
#define PIC_SWITCH_BATHROOM_MAIN_LIGHT_NAME  "picSwitchMain"

#define PIC_DAY_OF_MONTH1_ID                        39
#define PIC_DAY_OF_MONTH1_NAME				      "p4"
#define PIC_DAY_OF_MONTH2_ID                        40
#define PIC_DAY_OF_MONTH2_NAME				      "p5"

#define PIC_MONTH_ID                               38
#define PIC_MONTH_NAME                           "p3"
#define PIC_DAY_OF_WEEK_ID                         37
#define PIC_DAY_OF_WEEK_NAME                     "p2"

#define LBL_TIME1_ID                              33
#define LBL_TIME1_NAME                         "t10"
#define LBL_TIME2_ID                              34
#define LBL_TIME2_NAME                         "t11"
#define LBL_TIME3_ID                              35
#define LBL_TIME3_NAME                         "t12"
#define LBL_TIME4_ID                              36
#define LBL_TIME4_NAME                         "t13"

#define LBL_INDOOR_TEMP1_ID                      16
#define LBL_INDOOR_TEMP1_NAME            "tInTemp1"
#define LBL_INDOOR_TEMP2_ID                      17
#define LBL_INDOOR_TEMP2_NAME            "tInTemp2"
#define LBL_INDOOR_HUMIDITY_ID                   14
#define LBL_INDOOR_HUMIDITY_NAME               "t4"

#define LBL_OUTDOOR_TEMP1_ID                     19
#define LBL_OUTDOOR_TEMP1_NAME                 "t6"
#define LBL_OUTDOOR_TEMP2_ID                     20
#define LBL_OUTDOOR_TEMP2_NAME                 "t8"
#define LBL_OUTDOOR_TEMP_SYMBOL_ID               21
#define LBL_OUTDOOR_TEMP_SYMBOL_NAME           "t0"

#define LBL_OUTDOOR_HUMIDITY_ID                 12
#define LBL_OUTDOOR_HUMIDITY_NAME              "t2"

#define LBL_OUTDOOR_PREASURE_ID                 11
#define LBL_OUTDOOR_PREASURE_NAME              "t1"

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
