#ifndef CURRENT_SENSOR_H
#define CURRENT_SENSOR_H

#include "LJM_Utilities.h"

struct current_sensor_struct {
	double current;
}; 

void current_sensor_init(int handle, struct current_sensor_struct * current_sensor);

#endif
