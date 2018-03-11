#include <LabJackM.h>
#include "LJM_Utilities.h"
#include "rpm_sensor.h"
#include "sys/time.h"
#include "time.h"

#define HIGH 2.5 // switching voltage

void rpm_sensor_init(int handle, struct rpm_sensor_struct * rpm_sensor) {
	clock_t start_t, total_t = 0;
	int rotations = 0;

	int err = 0;
	double value = 0;
	const char * NAME = "AIN0";
        int last_value = 0;

	// Read AIN from the LabJack

	ErrorCheck(err, "LJM_eReadName");

        start_t = clock();
	while ((total_t/(CLOCKS_PER_SEC / 6)) < 1) {
                err = LJM_eReadName(handle, NAME, &value);
		if (value < HIGH && last_value != 1) {
			rotations++;
                        last_value = 1;
		}
                else {
			last_value = 0;
		}
		total_t = clock() - start_t;
	}
	rpm_sensor->rpm = rotations * 180;
}
