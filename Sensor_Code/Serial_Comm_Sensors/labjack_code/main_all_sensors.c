#include <stdio.h>
#include <stdlib.h>
#include <LabJackM.h>

#include "tmp007_i2c.h"
#include "LJM_Utilities.h"
#include "LabJackMModbusMap.h"

int main(int argc, char * argv[]) {
	int handle;

	// Open first found LabJack
	handle = OpenOrDie(LJM_dtANY, LJM_ctANY, "LJM_idANY");
	// handle = OpenSOrDie("LJM_dtANY", "LJM_ctANY", "LJM_idANY");

	PrintDeviceInfoFromHandle(handle);
	printf("\n");

	tmp007_i2c(handle);

	CloseOrDie(handle);

	WaitForUserIfWindows();

	
	//struct tmp007_sensor;
	return LJME_NOERROR;
}
