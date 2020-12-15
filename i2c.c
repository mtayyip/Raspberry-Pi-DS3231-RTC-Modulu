
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdint.h>

#define WRITE_BUFFER_SIZE   16


int i2cInit(char* i2cPath, int* handle)
{
	if(i2cPath == NULL)
		return -1;

	*handle = open(i2cPath, O_RDWR);

	if(*handle < 0)
		return -1;
	return 1;
}

void i2cClose(int handle)
{
	close(handle);
}

int i2cSetSlave(const int* handle,unsigned char slaveAddr)
{
    if(*handle < 0)
        return -1;

    if(ioctl(*handle, I2C_SLAVE, slaveAddr) < 0)
        return -1;

    return 1;
}

int i2cWrite(int handle, unsigned char regAddr, unsigned char* data, unsigned char dataLen)
{
	unsigned char writtenBytes = 0;
	unsigned char i = 0;
	unsigned char tempBuffer[WRITE_BUFFER_SIZE] = {0};
	// Address added buffer
	tempBuffer[0] = regAddr;
	
	// data added to buffer
	for(i = 0; i < dataLen; i++)
		tempBuffer[i+1] = data[i];

	// dataLen + addr write to i2c
	writtenBytes = write(handle, tempBuffer, dataLen+1);
	if(writtenBytes == -1)
		return -1;

	return 1;
}

int i2cRead(int handle,unsigned char regAddr, unsigned char* buffer, unsigned char bufsize)
{
	unsigned char readLen = 0;

	write(handle, &regAddr, 1);	

	readLen = read(handle, buffer, bufsize);

	if(readLen == -1)
		return -1;

	return 1;
}

