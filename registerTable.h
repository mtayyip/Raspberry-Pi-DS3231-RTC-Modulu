/*
 * registerTable.h
 *  Author: MuhammetTayyipCankaya
 *
 */
#ifndef __DS3231__
#define __DS3231__

#define DS3231_REG_SEC		0x00
#define DS3231_REG_MIN 		0x01
#define DS3231_REG_HOUR 	0x02
#define DS3231_REG_WEEKDAY 	0x03
#define DS3231_REG_DAY 		0x04
#define DS3231_REG_MONTH 	0x05
#define DS3231_REG_YEAR 	0x06

#define SEC_MASK 			0x7F
#define MIN_MASK 			0x7F
#define HOUR_MASK 			0x3F
#define DAY_MASK 			0x3F
#define WEEKDAY_MASK 			0x07
#define MONTH_MASK 			0x1F

#endif
