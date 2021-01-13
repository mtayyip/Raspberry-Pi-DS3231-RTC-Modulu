/*
 * ds3231.c
 *  Author: MuhammetTayyipCankaya
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#include "i2c.h"
#include "ds3231.h"
#include "registerTable.h"


#define BUF_SIZE		7
#define CLOCK_BUF_SIZE		9
#define DATE_BUF_SIZE		11
#define WEEKDAY_BUF_SIZE 	10


int rtcHandle = 0;


unsigned char decToBcd(unsigned char decVal)
{
	return (decVal / 10 << 4 | decVal % 10);
}


unsigned char bcdToDec(unsigned char bcdVal)
{
	return (bcdVal >> 4) * 10 + (bcdVal & 0x0F);
}
 
// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------

int ds3231_init(char* devicePath, unsigned char addr)
{
	if(i2cInit(devicePath, &rtcHandle) == -1)
	{
		printf("DS3231 I2C Init Error\r\n");
		return -1;
	}

	if(i2cSetSlave(&rtcHandle, addr) == -1)
	{
		printf("DS3231 Slave Address Set Error\r\n");
		return -1;
	}

	printf("DS3231 Init Success\r\n");
	return 1;
}

void ds3231_deinit(void)
{
	i2cClose(rtcHandle);
}

// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------

int setTime(DS3231* values)
{
	int retVal = -1;
	unsigned char i2cData[BUF_SIZE] = {0};

	i2cData[0] = decToBcd(values->second);
	i2cData[1] = decToBcd(values->minute);
	i2cData[2] = decToBcd(values->hour);
	i2cData[3] = decToBcd(values->weekday);
	i2cData[4] = decToBcd(values->day);
	i2cData[5] = decToBcd(values->month);
	i2cData[6] = decToBcd(values->year);


	if(i2cWrite(rtcHandle, DS3231_REG_SEC, i2cData, BUF_SIZE) == -1)
	{
		printf("DS3231 Set Time Error\r\n");
		return -1;
	}
   
	printf("DS3231 Set Time Success\r\n");
	return 1;
}

// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------

char getSecond(void)
{
	char second = -1;

	if(i2cRead(rtcHandle, DS3231_REG_SEC, &second,1) == -1)
	{
		printf("DS3231 Second Read Error\r\n");
		return -1;
	}
	return bcdToDec(second & SEC_MASK);
}

char getMinute(void)
{
	char minute = -1;
	if(i2cRead(rtcHandle, DS3231_REG_MIN, &minute,1) == -1)
	{
		printf("DS3231 Minute Read Error\r\n");
		return -1;
	} 
	return bcdToDec(minute & MIN_MASK);
}
 
char getHour(void)
{
	char hour = -1;
	if(i2cRead(rtcHandle, DS3231_REG_HOUR, &hour,1) == -1)
	{
		printf("DS3231 Hour Read Error\r\n");
		return -1;
	}  
	return bcdToDec(hour & HOUR_MASK);
}

int getClock(char* buffer, unsigned char bufferLen)
{
	if(bufferLen < CLOCK_BUF_SIZE)
	{
		printf("DS3231 getClock Buffer Size Error\r\n");
		return -1;
	}

	char hour = getHour();
	char minute = getMinute();
	char second = getSecond();
	memset(buffer, '\0', bufferLen);

	if(hour == -1 || minute == -1 || second == -1)
	{
		printf("DS3231 Clock Read Error\r\n");
		return -1;
	}

	sprintf(buffer, "%d:%d:%d", hour, minute, second);
	return 1;
}

// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------

char getDay(void)
{
	char day = -1;
	if(i2cRead(rtcHandle, DS3231_REG_DAY, &day,1) == -1)
	{
		printf("DS3231 Day Read Error\r\n");
		return -1;
	}
	return bcdToDec(day & DAY_MASK);
}

char getMonth(void)
{
	char month = -1;
	if(i2cRead(rtcHandle, DS3231_REG_MONTH, &month, 1) == -1)
	{
		printf("DS3231 Month Read Error\r\n");
		return -1;
	}
	return bcdToDec(month & MONTH_MASK);
}

char getYear(void)
{
	char year = -1;
	if(i2cRead(rtcHandle, DS3231_REG_YEAR, &year, 1) == -1)
	{
		printf("DS3231 Year Read Error\r\n");
		return -1;
	}
	return bcdToDec(year);
}

int getDate(char* buffer, unsigned char bufferLen)
{
	if(bufferLen < DATE_BUF_SIZE)
	{
		printf("DS3231 getDate Buffer Size Error\r\n");
		return -1;
	}

	char day = getDay();
	char month = getMonth();
	char year  = getYear();

	if(day == -1 || month == -1 || year == -1)
	{
		printf("DS3231 Date Read Error\r\n");
		return -1;
	}

	sprintf(buffer,"20%d-%d-%d",year,month,day);

	return 1;
}

// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------
 
Weekday getWeekday(void)
{
	Weekday weekday = 0;

	if(i2cRead(rtcHandle, DS3231_REG_WEEKDAY, (unsigned char*)&weekday, 1) == -1)
	{
		printf("DS3231 GetWeekday Read Error\r\n");
		return WDAY_ERROR;
	}

	return bcdToDec(weekday & WEEKDAY_MASK);
}
 
int getWeekdayAsStr(char* buffer, unsigned char bufferLen)
{
	if(bufferLen < WEEKDAY_BUF_SIZE)
	{
		printf("DS3231 getWeekdayAsStr Buffer Size Error\r\n");
		return -1;
	}

	Weekday weekday = getWeekday();
	if(weekday == WDAY_ERROR)
		return -1;
	
	char* dayNames[] = {"Sunday", "Monday", "Tuesday", "Wednesday", \
						"Thursday", "Friday", "Saturday"};

	memset(buffer, '\0', bufferLen);
	sprintf(buffer, "%s", dayNames[weekday]);
	return 1;
}

// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------
