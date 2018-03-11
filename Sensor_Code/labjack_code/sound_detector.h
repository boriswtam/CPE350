#ifndef SOUND_DETECTOR_H
#define SOUND_DETECTOR_H

//#include <LabJackM.h>
#include "LJM_Utilities.h"

struct sound_detector_struct {
	double voltage;
	int handle;
}; 

void sound_detector_init(struct sound_detector_struct *sound_detector);

#endif
