/****************************************************************************
* Sound_Detector.c
*
* Reading the voltage output from the Sparkfun Sound_Detector. 
****************************************************************************/
//#include <rc_usefulincludes.h> 
//#include <roboticscape.h>
#include "Sound_Detector.h"
 
/****************************************************************************
 * get_Sound_Detector_volts(int channel)
 *
 * Get Sound_Detector readings in terms of volts
 ***************************************************************************/
float get_Sound_Detector_volts(int channel){
    float volts = rc_adc_volt(channel);
    if(DEBUG_MODE == D_Sound_Detector || DEBUG_MODE == D_ALL){
        printf("\nSound_Detector: Measured voltage is %2.2f (Volts Func)\n", 
        volts);
    }
    return volts;
}
/****************************************************************************
 * get_Sound_Detector(int channel)
 *
 * Get Sound_Detector scaled readings 
 ***************************************************************************/
float get_Sound_Detector_scaled(int channel){
    float volts = rc_adc_volt(channel);
    if(DEBUG_MODE == D_Sound_Detector || DEBUG_MODE == D_ALL){
        printf("\nSound_Detector: Measured voltage is %2.2f (Scaled Func)\n", 
    volts);
    }
    return volts * 100; 
}
/****************************************************************************
 * get_Sound_Detector_raw(int channel)
 *
 * Get Sound_Detector raw 12-bit ADC reading 
 ***************************************************************************/
int get_Sound_Detector_raw(int channel){
    int raw = rc_adc_raw(channel);
    if(DEBUG_MODE == D_Sound_Detector || DEBUG_MODE == D_ALL){
        printf("\nSound_Detector: raw value is %d (Raw Func)\n", raw);
    }
    return raw; 
}
/***************************************************************************/   
