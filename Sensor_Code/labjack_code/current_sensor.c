/**
 * Name: current_sensor.c
 *
 */

#include <LabJackM.h>
#include "LJM_Utilities.h"
#include "current_sensor.h"

void current_sensor_init(int handle, struct current_sensor_struct * current_sensor)
{
	int err;

	// Set up for reading AIN value
	double value = 0;
	const char * NAME = "AIN1";

	// Read AIN from the LabJack
	err = LJM_eReadName(handle, NAME, &value);
	
	// Process the voltage to current
	double i = (value - (2.5)) / (0.028);
	current_sensor->current = i;

	ErrorCheck(err, "LJM_eReadName");
}
