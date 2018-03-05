#include <stdio.h>
#include <stdlib.h>
#include <LabJackM.h>

#include "tmp007_i2c.h"
#include "sound_sensor.h"
#include "LJM_Utilities.h"

int main(int argc, char * argv[]) {
	int handle;

	// Open first found LabJack
	handle = OpenOrDie(LJM_dtANY, LJM_ctANY, "LJM_idANY");
	// handle = OpenSOrDie("LJM_dtANY", "LJM_ctANY", "LJM_idANY");

	PrintDeviceInfoFromHandle(handle);
	printf("\n");

        while (1) {
	   tmp007_i2c(handle);
           sound_sensor(handle);
           sleep(.1);
        }

	CloseOrDie(handle);

	WaitForUserIfWindows();

	
	//struct tmp007_sensor;
	return LJME_NOERROR;
}
