/*
 * Name: main_all_sensors.c
 * 
 * Contains the main function to initialize and perform communication for all sensors:
 * TMP007 Temperature Sensor, 
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <LabJackM.h>
#include <pthread.h>

#include "tmp007_i2c.h"
#include "sound_detector.h"
#include "current_sensor.h"
#include "rpm_sensor.h"
#include "run_motor.h"
#include "LJM_Utilities.h"
#include "LabJackMModbusMap.h"

char * input_handle(int argc, char * fp) {
	char * file_name;
        if (argc < 2) {
                printf("Please specify an output file\n");
                exit(1);
        }
        file_name = malloc(strlen(fp) + 5);
	if (strstr(fp, ".csv") == NULL) {
		strcpy(file_name, fp);				
		strcat(file_name, ".csv");				
        	return file_name;
	}	
	return fp;
}

int main(int argc, char * argv[]) {
	char * file_name = input_handle(argc, argv[1]);
	int error = 0;
	
	int handle;

	int retval;
	fd_set rfds;
	struct timeval tv;
	char user_input[3000];	
	pthread_t my_thread;

	// Sensor value declerations
	struct tmp007_sensor_struct tmp007_sensor;
	struct sound_detector_struct sound_detector;
	struct current_sensor_struct current_sensor;
	struct rpm_sensor_struct rpm_sensor;
        struct run_motor_struct rms;

	// Open first found LabJack
	handle = OpenOrDie(LJM_dtANY, LJM_ctANY, "LJM_idANY");
	PrintDeviceInfoFromHandle(handle);
	printf("\n");
	tmp007_sensor.handle = handle;
	sound_detector.handle = handle;
	current_sensor.handle = handle;
	rpm_sensor.handle = handle;

        rms.supply_voltage = 12;
        rms.frequency = 50;
        rms.duty_cycle = .070;
        run_motor(handle, &rms);
        sleep(1);
        rms.duty_cycle = .025;
        run_motor(handle, &rms);
        sleep(1);
        rms.duty_cycle = .070;
        run_motor(handle, &rms);
      //  init_motor(handle, rms);


	FILE *file = fopen(file_name, "a+");
        fflush(file);

 	tv.tv_sec = 0;
	tv.tv_usec = 0;

	pthread_create(&my_thread, NULL, (void *)&tmp007_i2c, (void *)(&tmp007_sensor));
	pthread_create(&my_thread, NULL, (void *)&sound_detector_init, (void *)(&sound_detector));
	pthread_create(&my_thread, NULL, (void *)&current_sensor_init, (void *)(&current_sensor));
	pthread_create(&my_thread, NULL, (void *)&rpm_sensor_init, (void *)(&rpm_sensor));

	char buffer[10][10000];
	int i = 0;

	while(1) {
		//tmp local (C), tmp object (C), sound voltage (V), current (A), rpm, supply voltage (V), frequency (Hz), Duty Cycle, Error
		sprintf(buffer[i], "%.2f%c%.2f%c%.2lf%c%.4lf%c%d%c%d%c%d%c%.4f%c%d\n", tmp007_sensor.local_tmp, ',', tmp007_sensor.object_tmp, ',',
			 sound_detector.voltage, ',', current_sensor.current, ',', rpm_sensor.rpm, ',', rms.supply_voltage, ',', rms.frequency, ',', rms.duty_cycle, ',', error);
		fflush(file);
		i++;
		if (i == 10) {
			for (i = 0; i < 10; i++) {
				printf("Printing Line: %d\n", i);
				fprintf(file, "%s", buffer[i]);
			}
			i = 0;
		}
		printf("DONE PRINTING\n");
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);
		retval = select(1, &rfds, NULL, NULL, &tv);
		if (retval) {
			fgets(user_input, 3000, stdin);
			rms.duty_cycle = atof(user_input);
			run_motor(handle, &rms);
		}
		usleep(100000);
	}

	fclose(file);
	CloseOrDie(handle);
	WaitForUserIfWindows();
	return LJME_NOERROR;
}

