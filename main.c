/*
 * main.c
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
#include "ds3231.h"
#include "i2c.h"



void usage(char *arg)
{
	printf("Usage : %s <I2C Device> <I2C Address> -<option>\r\n",arg);
	printf("\toption  : Option \r\n");
	printf("\t-s\t: RTC Chip Set (Example : %s /dev/i2c-1 0x68 -s YY-MM-DD HH:MM:SS)\r\n",arg);
	printf("\t-p\t: Print Time   (Example : %s /dev/i2c-1 0x68 -p)\r\n",arg);
}


void printTime(void)
{
	printf("%02d-%02d-%02d %02d:%02d:%02d\r\n", getYear(),getMonth(),getDay(),
												getHour(),getMinute(),getSecond());
}
 

int setRtcChipTime(char* manDate, char* manClock)
{

	unsigned char clk[3] = {0};
	unsigned char date[3] = {0};

	int i=0;
	char *clkToken = strtok(manClock, ":");
	while( clkToken != NULL ) {
		clk[i] = atoi(clkToken);
		clkToken = strtok(NULL, ":");
		i++;
	}

	i=0;
	char *dateToken = strtok(manDate, "-");
	while( dateToken != NULL ) {
		date[i] = atoi(dateToken);
		dateToken = strtok(NULL, "-");
		i++;
	}

	DS3231 rtcValues;
	rtcValues.second = clk[2];
	rtcValues.minute = clk[1];
	rtcValues.hour = clk[0];

	rtcValues.day = date[2];
	rtcValues.month = date[1];
	rtcValues.year = date[0];

	rtcValues.weekday = MONDAY;

	if(setTime(&rtcValues) == -1)
		return -1;

	printTime();
	return 1;
}
 


int main(int argc, char* argv[])
{

	if(argc < 4)
	{
		usage(argv[0]);
		return -1;
	}

	if(argv[3][0] != '-')
	{
		usage(argv[0]);
		return -1;
	}

	if(argv[3][1] != 's' && argv[3][1] != 'p' )
	{
		usage(argv[0]);
		return -1;
	}
 

	if(ds3231_init(argv[1], strtol(argv[2],NULL,16)) == -1)
	{
		return -1;
	}


	if(argv[3][1] == 's')
	{
		if(argc != 6)
		{
			usage(argv[0]);
			return -1;
		}
		
		if(setRtcChipTime(argv[4], argv[5]) == -1)
		{
			printf("DS3231 RTC Time Manuel Set Error\r\n");
			return -1;
		}
		else
			printf("DS3231 RTC Time Manuel Set OK\r\n");
	}
	else
		printTime();


	ds3231_deinit();

	return 0;
}
