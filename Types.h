#ifndef __Types_H__
#define __Types_H__
#include <Arduino.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

typedef struct {
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t dayOfTheWeek;   // day of week
	uint8_t day;
	uint8_t month;
	uint8_t year;   // offset from 2000; 
} 	TDateTime;

typedef struct {
  uint8_t minute;
  uint8_t hour;
  uint16_t minuteOfTheDay;
}   TShortTime;


#endif /* #ifndef __Types_H__ */
