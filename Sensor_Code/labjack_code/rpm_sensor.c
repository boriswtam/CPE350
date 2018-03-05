#include <LabJackM.h>
#include "LJM_Utilities.h"
#include "rpm_sensor.h"
#include "sys/time.h"
#include "time.h"

#define HIGH 2.5 // switching voltage

void rpm_sensor_init(int handle, struct rpm_sensor_struct * rpm_sensor) {
	clock_t start_t, end_t, total_t = 0;
	int rotations = 0;

	int err;
	double value = 0;
	const char * NAME = "AIN2";

	// Read AIN from the LabJack
	err = LJM_eReadName(handle, NAME, &value);

	ErrorCheck(err, "LJM_eReadName");

	while ((total_t/CLOCKS_PER_SEC) < 1) {
		if (value > HIGH) {
			start_t = clock();
			rotations++;
		}
		else {
			end_t = clock();
		}
		total_t = end_t - start_t;
	}

	rpm_sensor->rpm = (total_t / CLOCKS_PER_SEC) * 60;
}
