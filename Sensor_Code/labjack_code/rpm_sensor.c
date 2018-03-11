#include <LabJackM.h>
#include "LJM_Utilities.h"
#include "rpm_sensor.h"
#include "sys/time.h"
#include "time.h"

#define HIGH 2.5 // switching voltage

void rpm_sensor_init(struct rpm_sensor_struct * rpm_sensor) {
	while (1) {
		struct timespec start_t, current_t;
		int rotations = 0;

		int err = 0;
		double value = 0;
		const char * NAME = "AIN0";
        	int last_value = 0;
		int number_checks = 0;

	// Read AIN from the LabJack

		ErrorCheck(err, "LJM_eReadName");

        	clock_gettime(CLOCK_MONOTONIC, &start_t);
		clock_gettime(CLOCK_MONOTONIC, &current_t);
		int num = 0;
		while (current_t.tv_sec - start_t.tv_sec < 1) {
                	err = LJM_eReadName(rpm_sensor->handle, NAME, &value);
			if (value < HIGH && last_value != 1) {
				rotations++;
                        	last_value = 1;
			}
                	else if (value > HIGH) {
				last_value = 0;
			}
			number_checks++;
			clock_gettime(CLOCK_MONOTONIC, &current_t);
			num++;
		}
		rotations = rotations * 30;
		rpm_sensor->rpm = rotations;
	}
}
