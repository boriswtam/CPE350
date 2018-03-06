/*
 * Name: main_all_sensors.c
 * 
 * Contains the main function to initialize and perform communication for all sensors:
 * TMP007 Temperature Sensor, 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LabJackM.h>

#include "tmp007_i2c.h"
#include "sound_detector.h"
#include "current_sensor.h"
#include "rpm_sensor.h"
#include "run_motor.h"
#include "LJM_Utilities.h"
#include "LabJackMModbusMap.h"

char * input_handle(int argc, char * fp) {
	char * file_name;
        file_name = malloc(strlen(fp) + 5);
	if (argc < 2) {
		printf("Please specify an output file\n");
		exit(1);
	}
	else {
		if (strstr(fp, ".csv") == NULL) {
			strcpy(file_name, fp);				
			strcat(file_name, ".csv");				
                        return file_name;
		}	
		return fp;
	}
}

int main(int argc, char * argv[]) {
	char * file_name = input_handle(argc, argv[1]);
	
	int handle;

	// Sensor value declerations
	struct tmp007_sensor_struct tmp007_sensor;
	struct sound_detector_struct sound_detector;
	struct current_sensor_struct current_sensor;
	struct rpm_sensor_struct rpm_sensor;

	// Open first found LabJack
	handle = OpenOrDie(LJM_dtANY, LJM_ctANY, "LJM_idANY");
	PrintDeviceInfoFromHandle(handle);
	printf("\n");

	FILE *file = fopen(file_name, "a+");
	fprintf(file, "%s\n", "tmp local (C), tmp object (C), sound voltage (V), current (A),");
        fflush(file);
	run_motor(handle);

	while(1) {
		tmp007_i2c(handle, &tmp007_sensor);
		sound_detector_init(handle, &sound_detector);
		current_sensor_init(handle, &current_sensor);
		rpm_sensor_init(handle, &rpm_sensor);
		fprintf(file, "%.2f%c%.2f%c%.2lf%c%.2lf%c%d\n", tmp007_sensor.local_tmp, ',', tmp007_sensor.object_tmp, ',',
			 sound_detector.voltage, ',', current_sensor.current, ',', rpm_sensor.rpm);
		fflush(file);
	}

	fclose(file);
	CloseOrDie(handle);
	WaitForUserIfWindows();
	return LJME_NOERROR;
}