/**
 * Name: single_dio_write.c
 * Desc: Demonstrates how to set a single digital state on a LabJack.
**/

// For printf
#include <stdio.h>

// For the LabJackM Library
#include <LabJackM.h>

// For LabJackM helper functions, such as OpenOrDie, PrintDeviceInfoFromHandle,
// ErrorCheck, etc.
#include "LJM_Utilities.h"

#include "run_motor.h"

void EnableDIO15PulseOut(int handle, int pulseRate, int numPulses, int config, struct run_motor_struct * rms)
{
	// Set FIO0 to do a 50% duty cycle
	// http://labjack.com/support/datasheets/t7/digital-io/extended-features/pulse-out

        int clock_div = 8;
	int rollValue = 80000000 /* 10 MHz */ / pulseRate;

	printf("Enabling pulses on FIO0 at a %d Hz pulse rate\n", pulseRate);

	WriteNameOrDie(handle, "DIO0_EF_ENABLE", 0);
	WriteNameOrDie(handle, "DIO_EF_CLOCK0_DIVISOR", clock_div);
	WriteNameOrDie(handle, "DIO_EF_CLOCK0_ROLL_VALUE", rollValue);
	WriteNameOrDie(handle, "DIO_EF_CLOCK0_ENABLE", 1);
	WriteNameOrDie(handle, "DIO0_EF_INDEX", 2);
	WriteNameOrDie(handle, "DIO0_EF_OPTIONS", 0);
	WriteNameOrDie(handle, "DIO0", 0);
	WriteNameOrDie(handle, "DIO0_EF_CONFIG_A", config);
	WriteNameOrDie(handle, "DIO0_EF_CONFIG_B", 0);
	WriteNameOrDie(handle, "DIO0_EF_CONFIG_C", numPulses);
	WriteNameOrDie(handle, "DIO0_EF_ENABLE", 1);
}

void run_motor(int handle, struct run_motor_struct * rms)
{
	
        int pulseRate = 8 * rms->frequency;
        int config = rms->duty_cycle * (80000000/pulseRate);
        int numPulses = 2147483647;
                 
        EnableDIO15PulseOut(handle, pulseRate, numPulses, config, rms);
}
