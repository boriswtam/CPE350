/****************************************************************************
 * RPM.c
 * Lines with functions that require BeagleBone libraries
 ****************************************************************************/
/*
#include <rc_usefulincludes.h>
#include <roboticscape.h>*/
#include <inttypes.h>
#include <stdio.h>
#include "rc_usefulincludes.h"
#include "RPM.h"

/****************************************************************************
  * Internal Function Declarations
  ***************************************************************************/
void (*RPM_rise_func)(void)   = &rc_null_func; //^
/****************************************************************************
  * void on_rpm_rise()
  *
  * Function called from the interrupt handler. Setup set_rpm_rise_func().
  * Captures the time and compares it to the start time. If one second passes,
  * multiple the total amount of interrupts in one second by 60 to get an
  * estimated RPM value.
  ***************************************************************************/
void on_rpm_rise(){
   rpmInterruptCount++; // Increment interrupt count
   clock_gettime(CLOCK_REALTIME, &rpmEndTimeStruct); // Save time
   rpmEndTime = rc_timespec_to_micros(rpmEndTimeStruct);// Use nice rc_ func  ^
   uint64_t difference = rpmEndTime - rpmStartTime;     // Measure time
   if(DEBUG_MODE == D_RPM || DEBUG_MODE == D_ALL){
      printf("Called on RPM rise function!\n");
      printf("Current END Time is: %"PRIu64"\n", rpmEndTime);
      printf("Differnce is: %"PRIu64"\n", difference);
   }
   if (difference > 1000000){ // 1 second
      rpmFrequency = rpmInterruptCount;   // save as freq. b/c count resets
      rpmValue = (rpmInterruptCount * 60) / motorPoles; // Obtain RPM
      rpmInterruptCount = 0;
      clock_gettime(CLOCK_REALTIME, &rpmStartTimeStruct);  // Start timer
      rpmStartTime = rc_timespec_to_micros(rpmStartTimeStruct); //^
      if(DEBUG_MODE == D_RPM || DEBUG_MODE == D_ALL){
         printf("New START time is: %"PRIu64"\n", rpmStartTime);
      }
   }
   return;
}
/****************************************************************************
  * int initialize_rpm()
  *
  * Configures a GPIO pin as an input, rising edge interrupt pin.
  * Start an interrupt handler thread that constantly poll for rising edge on
  * the RPM pin.
  ***************************************************************************/
int initialize_rpm(){
   struct sched_param params; // Create a params struct
   if(rc_gpio_export(RPM_INTERRUPT_PIN)<0){ //^
      fprintf(stderr,"ERROR: failed to export GPIO %d",
              RPM_INTERRUPT_PIN);
      return -1;
   }
   if(rc_gpio_set_dir(RPM_INTERRUPT_PIN, INPUT_PIN)<0){ //^
      fprintf(stderr,"ERROR: failed to configure GPIO %d",
              RPM_INTERRUPT_PIN);
      return -1;
   }
   if(rc_gpio_set_edge(RPM_INTERRUPT_PIN, EDGE_RISING)<0){ //^
      fprintf(stderr,"ERROR: failed to configure GPIO %d",
              RPM_INTERRUPT_PIN);
      return -1;
   }
   /** Start the interrupt handler thread **/
   set_rpm_rise_func(&rc_null_func);   // Initialize RPM interrupt ^
   // New thread for interrupts
   pthread_create(&RPM_rise_thread, NULL, RPM_rise_handler,(void*)NULL);
   params.sched_priority = sched_get_priority_max(SCHED_FIFO)- 5;
   pthread_setschedparam(RPM_rise_thread, SCHED_FIFO, &params);
   clock_gettime(CLOCK_REALTIME, &rpmStartTimeStruct);
   rpmStartTime = rc_timespec_to_micros(rpmStartTimeStruct);
   rc_usleep(1000); //^
   if(DEBUG_MODE == D_RPM || DEBUG_MODE == D_ALL){
      printf("Initialized RPM pin for interrupts\n");
   }
   return 0;
}
/****************************************************************************
  * void* RPM_rise_handler(void* ptr)
  *
  * Handler for the Rpm_rise_thread. Constantly checks the gpio/value file for
  * updates due to rising edge.
  ***************************************************************************/
void* RPM_rise_handler(void* ptr){
   struct pollfd fdset[1];
   char buf[POLL_BUF_LEN];
   int gpio_fd = rc_gpio_fd_open(RPM_INTERRUPT_PIN); //^
   fdset[0].fd = gpio_fd;
   fdset[0].events = POLLPRI; // High-priority interrupt
   // Keep running until the program/thread closes
   while(rc_get_state() != EXITING) { //^
      // System thread waits here until FIFO interrupt
      clock_gettime(CLOCK_REALTIME, &rpmEndTimeStruct);
      rpmEndTime = rc_timespec_to_micros(rpmEndTimeStruct);
      uint64_t difference = rpmEndTime - rpmStartTime;
      if(difference > 1500000) {
         rpmFrequency = 0;
         rpmValue = 0;
      }
      poll(fdset, 1, POLL_TIMEOUT);
      if (fdset[0].revents & POLLPRI) {
         lseek(fdset[0].fd, 0, SEEK_SET);
         read(fdset[0].fd, buf, POLL_BUF_LEN);
         RPM_rise_func();
         // Purge any interrupts that may have stacked up
         lseek(fdset[0].fd, 0, SEEK_SET);
         read(fdset[0].fd, buf, POLL_BUF_LEN);
      }
   }
   // Handler killed
   rc_gpio_fd_close(gpio_fd); //^
   if(DEBUG_MODE == D_RPM || DEBUG_MODE == D_ALL){
      printf("RPM interrupt HANDLING END\n");
   }
   return 0;
}

/****************************************************************************
  * int set_rpm_rise_func(void (*func)(void))
  *
  * User can set the function that the RPM_handler calls
  ***************************************************************************/
int set_rpm_rise_func(void (*func)(void)){
   if(func==NULL){
      fprintf(stderr,"ERROR: trying to assign NULL pointer to RPM_interrupt_func\n");
      return -1;
   }
   RPM_rise_func = func;
   if(DEBUG_MODE == D_RPM || DEBUG_MODE == D_ALL){
      printf("RPM rise function SET\n");
   }
   return 0;
}
/****************************************************************************
 * int wait_for_RPM_handler_to_join()
 *
 * Called during cleanup and when program flow is in exiting state.
 * Waits 3 seconds for the RPM interrupt thread to close.
 ****************************************************************************/
int wait_for_RPM_handler_to_join(){
  int ret = 0;
  // Allow up to 3 seconds for thread cleanup
  struct timespec thread_timeout;
  clock_gettime(CLOCK_REALTIME, &thread_timeout);
  thread_timeout.tv_sec += 3;
  
  int thread_err = 0;
  thread_err = pthread_timedjoin_np(RPM_rise_thread, NULL, \
                                    &thread_timeout); //^
  if(thread_err == ETIMEDOUT){
     printf("WARNING: RPM_rise_thread exit timeout\n");
     ret = -1;
  }
  if(DEBUG_MODE == D_RPM || DEBUG_MODE == D_ALL){
     printf("RPM thread joined to die\n");
  }
  return ret;
}
/***************************************************************************/

      
