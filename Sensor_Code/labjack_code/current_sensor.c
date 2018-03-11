/**
 * Name: current_sensor.c
 *
 */

#include <LabJackM.h>
#include "LJM_Utilities.h"
#include "current_sensor.h"
#include "sys/time.h"
#include "time.h"


void current_sensor_init(int handle, struct current_sensor_struct * current_sensor)
{
        clock_t start_t, total_t = 0;

	// Set up for reading AIN value
	double value = 0;
	const char * NAME = "AIN2";

	double max_value = -100;
	double min_value = 100;

	// Read AIN from the LabJack
        start_t = clock();
        while ((total_t/(CLOCKS_PER_SEC / 10)) < 1) {
                LJM_eReadName(handle, NAME, &value);
                if (value > max_value) {
			max_value = value;
                }
                else if (value < min_value) {
                        min_value = value;
                }
                total_t = clock() - start_t;
        }

	current_sensor->current = (max_value - min_value) / .028;
}
