

#ifndef __I2C_H__
#define __I2C_H__


int i2cInit(char* i2cPath, int* handle);

void i2cClose(int handle);

int i2cSetSlave(const int* handle, unsigned char slaveAddr);

int i2cWrite(int handle, unsigned char regAddr, unsigned char* data, unsigned char dataLen);

int i2cRead(int handle, unsigned char regAddr, unsigned char* buffer, unsigned char bufsize);

#endif  
