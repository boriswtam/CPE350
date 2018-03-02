#include "rpm_sensor.h"
#include "sys/time.h"
#include "time.h"

#define HIGH 2.5 // switching voltage

void rpm_sensor_init(int handle, struct rpm_sensor_struct * rpm_sensor) {
	clock_t start_t = 0, end_t = 0, total_t = 0;
	int interrupts = 0;

	int err;
	double value = 0;
	const char * NAME = "AIN2";

	// Read AIN from the LabJack
	err = LJM_eReadName(handle, NAME, &value);

	if (value > HIGH) {

	}

	start_t = clock();
	
	end_t = clock();
}
