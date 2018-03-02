/*******************************************************************************
 * roboticscape.h
 *
 * This contains the complete Robotics Cape API. All functions declared here can
 * be executed by linking to /usr/lib/robotics_cape.so
 *
 * All functions return 0 on success or -1 on failure unless otherwise stated.
 *
 * James Strawson - 2016
 *******************************************************************************/

/*
 functions that need to be written:
 Documentation found in the following link:
 https://github.com/StrawsonDesign/Robotics_Cape_Installer/blob/master/libraries/roboticscape.h#L981
 
 
 ** Functions that haven't been found:
 RPM_rise_func
 set_rpm_rise_func
 
 pthread_timedjoin_np
 
 ** Functions that have been found (their declarations and descriptions are below:
 
 * GPIOs:
 
 int rc_gpio_export(unsigned int gpio);
 int rc_gpio_set_dir(int gpio, rc_pin_direction_t dir);
 int rc_gpio_set_edge(unsigned int gpio, rc_pin_edge_t edge);
 int rc_gpio_fd_open(unsigned int gpio);
 int rc_gpio_fd_close(int fd);
 
 * Timers:
 
 * @ void rc_usleep(uint64_t ns)
 *
 * The traditional usleep function, however common, is deprecated in linux as it
 * uses SIGALARM which interferes with alarm and timer functions. This uses the
 * new POSIX standard nanosleep to accomplish the same thing which further
 * supports sleeping for lengths longer than 1 second. This also handles
 * restarting nanosleep with the remaining time in the event that nanosleep is
 * interrupted by a signal. There is no upper limit on the time requested.
 void rc_usleep(unsigned int us);
 
 * @ uint64_t rc_timespec_to_micros(timespec ts)
 *
 * Returns a number of microseconds corresponding to a timespec struct.
 * Useful because timespec structs are annoying.
 *
 rc_timespec_to_micros
 
 * States:
 * The flow state variable is kept safely in the robotics cape library's memory
 * space and should be read and modified by the rc_get_state() and rc_set_state()
 int rc_set_state(rc_state_t new_state);
 */

#ifndef ROBOTICS_CAPE
#define ROBOTICS_CAPE

// library version, can also be printed from the command line with the included
// example program rc_version
#define RC_LIB_VERSION_FLOAT   0.34
#define RC_LIB_VERSION_STRING   0.3.4

// necessary types for function prototypes
#include <stdint.h> // for uint8_t types etc
typedef struct timespec   timespec;
typedef struct timeval timeval;


/*******************************************************************************
 * Pin Multiplexing (pinmux)
 *
 * On the Robotics Cape, we allow changing the pinmux on the SPI, GPS, and UART1
 * headers in case you wish to expose GPIO, CAN, or PWM functionality.
 * We use the GPIO number to identify the pins even though they may be used
 * for things other than GPIO as this provides consistency with the GPIO
 * functions which will likely be used. A list of defines are also given here
 * to make your code easier to read and to indicate which pins are available
 * for pinmuxing.
 *
 * enum rc_pinmux_mode_t gives options for pinmuxing. Not every mode if available on
 * each pin. refer to the pin table for which to use.
 *
 * rc_set_default_pinmux() puts everything back to standard and is used by
 * initialize_cape
 *******************************************************************************/
// Cape and Blue
#define GPS_HEADER_PIN_3      2   // P9_22, normally GPS UART2 RX
#define GPS_HEADER_PIN_4      3   // P9_21, normally GPS UART2 TX
#define UART1_HEADER_PIN_3      14   // P9_26, normally UART1 RX
#define UART1_HEADER_PIN_4      15   // P9_24, normally UART1 TX
#define SPI_HEADER_PIN_3      112   // P9_30, normally SPI1 MOSI
#define SPI_HEADER_PIN_4      111   // P9_29, normally SPI1 MISO
#define SPI_HEADER_PIN_5      110   // P9_31, normally SPI1 SCLK

// Cape Only
#define CAPE_SPI_PIN_6_SS1      113   // P9_28, normally SPI mode
#define CAPE_SPI_PIN_6_SS2      49   // P9_23, normally GPIO mode

// Blue Only
#define BLUE_SPI_PIN_6_SS1      29   // gpio 0_29  pin H18
#define BLUE_SPI_PIN_6_SS2      7   // gpio 0_7  pin C18
#define BLUE_GP0_PIN_3         57   // gpio 1_25 pin U16
#define BLUE_GP0_PIN_4         49   // gpio 1_17 pin P9.23
#define BLUE_GP0_PIN_5         116   // gpio 3_20 pin D13
#define BLUE_GP0_PIN_6         113   // gpio 3_17 pin P9_28
#define BLUE_GP1_PIN_3         98   // gpio 3_2  pin J15
#define BLUE_GP1_PIN_4         97   // gpio 3_1  pin H17

typedef enum rc_pinmux_mode_t{
   PINMUX_GPIO,
   PINMUX_GPIO_PU,
   PINMUX_GPIO_PD,
   PINMUX_PWM,
   PINMUX_SPI,
   PINMUX_UART,
   PINMUX_CAN
} rc_pinmux_mode_t;

int rc_set_pinmux_mode(int pin, rc_pinmux_mode_t mode);
int rc_set_default_pinmux();


/*******************************************************************************
 * GPIO
 *******************************************************************************/
#define HIGH 1
#define LOW 0

typedef enum rc_pin_direction_t{
   INPUT_PIN,
   OUTPUT_PIN
}rc_pin_direction_t;

typedef enum rc_pin_edge_t{
   EDGE_NONE,
   EDGE_RISING,
   EDGE_FALLING,
   EDGE_BOTH
}rc_pin_edge_t;

int rc_gpio_export(unsigned int gpio);
int rc_gpio_unexport(unsigned int gpio);
int rc_gpio_set_dir(int gpio, rc_pin_direction_t dir);
int rc_gpio_set_value(unsigned int gpio, int value);
int rc_gpio_get_value(unsigned int gpio);
int rc_gpio_set_edge(unsigned int gpio, rc_pin_edge_t edge);
int rc_gpio_fd_open(unsigned int gpio);
int rc_gpio_fd_close(int fd);
int rc_gpio_set_value_mmap(int pin, int state);
int rc_gpio_get_value_mmap(int pin);


/*******************************************************************************
 * time
 *
 * @ void rc_nanosleep(uint64_t ns)
 *
 * A wrapper for the normal UNIX nanosleep function which takes a number of
 * nanoseconds instead of a timeval struct. This also handles restarting
 * nanosleep with the remaining time in the event that nanosleep is interrupted
 * by a signal. There is no upper limit on the time requested.
 *
 * @ void rc_usleep(uint64_t ns)
 *
 * The traditional usleep function, however common, is deprecated in linux as it
 * uses SIGALARM which interferes with alarm and timer functions. This uses the
 * new POSIX standard nanosleep to accomplish the same thing which further
 * supports sleeping for lengths longer than 1 second. This also handles
 * restarting nanosleep with the remaining time in the event that nanosleep is
 * interrupted by a signal. There is no upper limit on the time requested.
 *
 * @ uint64_t rc_timespec_to_micros(timespec ts)
 *
 * Returns a number of microseconds corresponding to a timespec struct.
 * Useful because timespec structs are annoying.
 *
 * @ uint64_t rc_timespec_to_millis(timespec ts)
 *
 * Returns a number of milliseconds corresponding to a timespec struct.
 * Useful because timespec structs are annoying.
 *
 * @ uint64_t rc_timeval_to_micros(timeval tv)
 *
 * Returns a number of microseconds corresponding to a timespec struct.
 * Useful because timeval structs are annoying.
 *
 * @ uint64_t rc_timeval_to_millis(timeval ts)
 *
 * Returns a number of milliseconds corresponding to a timespec struct.
 * Useful because timeval structs are annoying.
 *
 * @ uint64_t rc_nanos_since_epoch()
 *
 * Returns the number of nanoseconds since epoch using system CLOCK_REALTIME
 * This function itself takes about 1100ns to complete at 1ghz under ideal
 * circumstances.
 *
 * @ uint64_t rc_nanos_since_boot()
 *
 * Returns the number of nanoseconds since system boot using CLOCK_MONOTONIC
 * This function itself takes about 1100ns to complete at 1ghz under ideal
 * circumstances.
 *
 * @ uint64_t rc_nanos_thread_time()
 *
 * Returns the number of nanoseconds from when when the calling thread was
 * started in CPU time. This time only increments when the processor is working
 * on the calling thread and not when the thread is sleeping. This is usually for
 * timing how long blocks of user-code take to execute. This function itself
 * takes about 2100ns to complete at 1ghz under ideal circumstances.
 *
 * @ timespec rc_timespec_diff(timespec start, timespec end)
 *
 * Returns the time difference between two timespec structs as another timespec.
 * Convenient for use with nanosleep() function and accurately timed loops.
 * Unlike timespec_sub defined in time.h, rc_timespec_diff does not care which
 * came first, A or B. A positive difference in time is always returned.
 *
 * @ int rc_timespec_add(timespec* start, double seconds)
 *
 * Adds an amount of time in seconds to a timespec struct. The time added is a
 * floating point value to make respresenting fractions of a second easier.
 * the timespec is passed as a pointer so it can be modified in place.
 * Seconds may be negative.
 *******************************************************************************/
void rc_nanosleep(uint64_t ns);
void rc_usleep(unsigned int us);
uint64_t rc_timespec_to_micros(timespec ts);
uint64_t rc_timespec_to_millis(timespec ts);
uint64_t rc_timeval_to_micros(timeval tv);
uint64_t rc_timeval_to_millis(timeval tv);
uint64_t rc_nanos_since_epoch();
uint64_t rc_nanos_since_boot();
uint64_t rc_nanos_thread_time();
timespec rc_timespec_diff(timespec A, timespec B);
void rc_timespec_add(timespec* start, double seconds);


/*******************************************************************************
 * FLOW STATE FOR HIGH LEVEL PROGRAM CONTROL
 *
 * It can be tricky to manage the starting and stopping of mutiple threads and
 * loops. Since the robotics cape library has several background threads in
 * addition to any user-created threads, we encourage the use of the consolidated
 * high-level program flow control method described here.
 *
 * The rc_state_t struct tries to cover the majority of use cases in the context
 * of a robotics application. After the user has called rc_initialize(), the
 * program flow state will be set to UNINITIALIZED. When the user's own
 * initialization sequence is complete they should set the flow state to PAUSED
 * or RUNNING to indicate to their own threads that the program should now behave
 * in normal ongoing operational mode.
 *
 * During normal operation, the user may elect to implement a PAUSED state where
 * the user's own threads may keep running to read sensors but do not actuate
 * motors, leaving their robot in a safe state. For example, pressing the pause
 * button could be assigned to change this state back and forth between RUNNING
 * and PAUSED. This is entirely optional.
 *
 * The most important state here is EXITING. The signal handler described in the
 * Init & Cleanup section intercepts the SIGINT signal when the user pressed
 * Ctrl-C and sets the flow state to EXITING. It is then up to the user's threads
 * to watch for this condition and exit quickly and cleanly. The user may also
 * set the flow state to EXITING at any time to trigger the closing of their own
 * threads and Robotics Cape library's own background threads.
 *
 * The flow state variable is kept safely in the robotics cape library's memory
 * space and should be read and modified by the rc_get_state() and rc_set_state()
 * functions above. The user may optionally use the rc_print_state() function to
 * print a human readable version of the state enum to the screen.
 * All example programs use these functions. See the rc_bare_minimum example
 * for a skeleton outline.
 *******************************************************************************/
typedef enum rc_state_t {
   UNINITIALIZED,
   RUNNING,
   PAUSED,
   EXITING
} rc_state_t;

rc_state_t rc_get_state();
int rc_set_state(rc_state_t new_state);
int rc_print_state();

