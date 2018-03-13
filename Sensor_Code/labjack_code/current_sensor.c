/**
 * Name: current_sensor.c
 *
 */

#include <LabJackM.h>
#include "LJM_Utilities.h"
#include "current_sensor.h"
#include "sys/time.h"
#include "time.h"


void current_sensor_init(struct current_sensor_struct * current_sensor)
{
	while (1) {
        struct timespec my_start_t, my_current_t;

	// Set up for reading AIN value
	double value = 0;
	const char * NAME = "AIN1";

	double max_value = -100;
	double min_value = 100;

	// Read AIN from the LabJack
        clock_gettime(CLOCK_MONOTONIC, &my_start_t);
        clock_gettime(CLOCK_MONOTONIC, &my_current_t);
        while (my_current_t.tv_sec - my_start_t.tv_sec < 1) {
                LJM_eReadName(current_sensor->handle, NAME, &value);
                if (value > max_value) {
			max_value = value;
                }
                else if (value < min_value) {
                        min_value = value;
                }
                clock_gettime(CLOCK_MONOTONIC, &my_current_t);
        }
	current_sensor->current = (max_value - min_value) / .028;
	}
}
