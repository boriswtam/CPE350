#ifndef ADXL345_H
#define ADXL345_H

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define ADXL345_ADDR	(0x53)	// ADXL345 I2C Address

struct ADXL345 {
	float X;
	float Y;
	float Z;
} ADXL345;

int init_ADXL345();
void read_ADXL345();
void write_ADXL345();

#endif
