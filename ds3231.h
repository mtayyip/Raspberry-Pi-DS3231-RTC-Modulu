/*
 * ds3231.h
 *  Author: MuhammetTayyipCankaya
 *
 */
#ifndef __DS3231_H__
#define __DS3231_H__

typedef enum
{
	SUNDAY = 0,
	MONDAY,
	TUESDAY,
	WEDNESDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY,
	WDAY_ERROR
}Weekday;


typedef struct
{
	unsigned char second;
	unsigned char minute;
	unsigned char hour;
	unsigned char day;
	Weekday weekday;
	unsigned char month;
	int year;
}DS3231;

// ---------------------------------------------------------------------------------------
int ds3231_init(char* devicePath, unsigned char pcf8563_addr);
void ds3231_deinit(void);
// ---------------------------------------------------------------------------------------
int setTime(DS3231* values);
// ---------------------------------------------------------------------------------------
char getHour(void);
char getMinute(void);
char getSecond(void);
int getClock(char* buffer, unsigned char bufferLen);
// ---------------------------------------------------------------------------------------
char getDay(void);
char getMonth(void);
char getYear(void);
int getDate(char* buffer, unsigned char bufferLen);
// ---------------------------------------------------------------------------------------
Weekday getWeekday(void);
int getWeekdayAsStr(char* buffer, unsigned char bufferLen);
// ---------------------------------------------------------------------------------------

#endif
