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
#include "../../LJM_Utilities.h"
#include "../../LJM_StreamUtilities.h"

int runMotor()
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
