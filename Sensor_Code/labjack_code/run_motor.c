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

void EnableDIO15PulseOut(int handle, int pulseRate, int numPulses, int config)
{
	// Set FIO0 to do a 50% duty cycle
	// http://labjack.com/support/datasheets/t7/digital-io/extended-features/pulse-out

	int rollValue = 80000000 /* 10 MHz */ / pulseRate;

	printf("Enabling pulses on FIO0 at a %d Hz pulse rate\n", pulseRate);

	WriteNameOrDie(handle, "DIO15_EF_ENABLE", 0);
	WriteNameOrDie(handle, "DIO_EF_CLOCK0_DIVISOR", 8);
	WriteNameOrDie(handle, "DIO_EF_CLOCK0_ROLL_VALUE", rollValue);
	WriteNameOrDie(handle, "DIO_EF_CLOCK0_ENABLE", 1);
	WriteNameOrDie(handle, "DIO15_EF_INDEX", 2);
	WriteNameOrDie(handle, "DIO15_EF_OPTIONS", 0);
	WriteNameOrDie(handle, "DIO15", 0);
	WriteNameOrDie(handle, "DIO15_EF_CONFIG_A", config);
	WriteNameOrDie(handle, "DIO15_EF_CONFIG_B", 0);
	WriteNameOrDie(handle, "DIO15_EF_CONFIG_C", numPulses);
	WriteNameOrDie(handle, "DIO15_EF_ENABLE", 1);
}

int run_motor()
{
	int handle;

	// Open first found LabJack
	handle = OpenOrDie(LJM_dtANY, LJM_ctANY, "LJM_idANY");
	// handle = OpenSOrDie("LJM_dtANY", "LJM_ctANY", "LJM_idANY");

	PrintDeviceInfoFromHandle(handle);

                 
        EnableDIO15PulseOut(handle, 400, 2147483647, 15000);
        sleep(2);
        EnableDIO15PulseOut(handle, 400, 2147483647, 5000);
        sleep(2);
        EnableDIO15PulseOut(handle, 400, 2147483647, 20000);
  
	CloseOrDie(handle);

	WaitForUserIfWindows();

	return LJME_NOERROR;
}
