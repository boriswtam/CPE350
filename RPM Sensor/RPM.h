/****************************************************************************
  * RPM.h
  *
  ***************************************************************************/
#ifndef RPM_H
#define RPM_H
// Debug mode values
extern const int DEBUG_MODE;
extern const int D_ALL;
extern const int D_RPM;
// Definitions for sensor configuration
#define RPM_INTERRUPT_PIN 98 //GPIO 1 Header - Pin 3.2
#define motorPoles 14
// Definitions used internally
#define POLL_BUF_LEN 1024   // Polling buffer length
#define POLL_TIMEOUT 100    // 0.1 seconds
/****************************************************************************
 * Interrupt thread function declarations
 ****************************************************************************/
int initialize_rpm();
void* RPM_rise_handler(void* ptr);
int set_rpm_rise_func(void (*func)(void));
void on_rpm_rise();
int wait_for_RPM_handler_to_join();
/****************************************************************************
 * Thread structs
 ****************************************************************************/
pthread_t RPM_rise_thread;
/****************************************************************************
 * Globals used during interrupt processing
 ****************************************************************************/
extern volatile int rpmInterruptCount;
extern volatile int rpmValue;
extern volatile int rpmFrequency;
extern volatile uint64_t rpmStartTime, rpmEndTime;
extern struct timespec rpmStartTimeStruct,rpmEndTimeStruct;
/***************************************************************************/
#endif


