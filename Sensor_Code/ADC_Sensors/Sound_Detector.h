/****************************************************************************
 * Sound_Detector.h
 *
 ***************************************************************************/
#ifndef Sound_Detector 
#define Sound_Detector
 
extern const int DEBUG_MODE;
extern const int D_ALL;
extern const int D_Sound_Detector;
 
// User Functions
float get_Sound_Detector_volts(int channel);
float get_Sound_Detector_scaled(int channel);
int get_Sound_Detector_raw(int channel);
 
#endif