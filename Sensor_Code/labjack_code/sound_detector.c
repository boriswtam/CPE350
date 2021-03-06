/**
 * Name: sound_detector.c
 *
 */

#include <LabJackM.h>
#include "LJM_Utilities.h"
#include "sound_detector.h"

void sound_detector_init(struct sound_detector_struct * sound_detector)
{
	while (1) {
	int err;

	// Set up for reading AIN value
	double value = 0;
	const char * NAME = "AIN2";

	// Read AIN from the LabJack
	err = LJM_eReadName(sound_detector->handle, NAME, &value);
	sound_detector->voltage = value;

	ErrorCheck(err, "LJM_eReadName");
	sleep(1);
	}
}
