#include <stdio.h>
#include <stdlib.h>
#include <LabJackM.h>

#include "tmp007_i2c.h"
#include "LJM_Utilities.h"
#include "LabJackMModbusMap.h"

int main(int argc, char * argv[]) {
	
	int handle;
	struct tmp007_sensor_struct tmp007_sensor;
	// Open first found LabJack
	handle = OpenOrDie(LJM_dtANY, LJM_ctANY, "LJM_idANY");
	// handle = OpenSOrDie("LJM_dtANY", "LJM_ctANY", "LJM_idANY");

	PrintDeviceInfoFromHandle(handle);
	printf("\n");

	tmp007_i2c(handle, &tmp007_sensor);
	printf("%s%.2f", "Local tmp: ", tmp007_sensor.local_tmp);
	printf("\n");
	printf("%s%.2f", "Object tmp: ", tmp007_sensor.object_tmp);
	printf("\n");

	CloseOrDie(handle);

	WaitForUserIfWindows();

	return LJME_NOERROR;
}
