#ifndef TMP007_I2C_h
#define TMP007_I2C_h

//#include <LabJackM.h>
#include "LJM_Utilities.h"

struct tmp007_sensor_struct {
	float local_tmp;
	float voltage;
	float object_tmp;
	int handle;
}; 

void tmp007_i2c(struct tmp007_sensor_struct * tmp007_sensor);

#endif
