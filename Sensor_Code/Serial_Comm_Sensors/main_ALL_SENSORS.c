/****************************************************************************
* main_ALL_SENSORS.c 
*
* This is the main source file for testing all implemented sensors for the 
* NASA VTOL PHM project.
*
* DEBUG CODES:
* ---------------------
* D_ALL             = 1
* D_ADXL345         = 2
* D_TMP36           = 3
* D_LTSR            = 4
* D_Sound_Detector  = 5
* D_RPM             = 6
* D_TMP007          = 7
****************************************************************************/
//#include <rc_usefulincludes.h> 
//#include <roboticscape.h>
//#include <inttypes.h>
//#include "DEBUG.h"           
#include "ADXL345.h"
#include "TMP36.h"
#include "LTSR.h"
#include "Sound_Detector.h"
#include "RPM.h"
#include "TMP007.h"
/****************************************************************************
* Button function declarations 
****************************************************************************/
void on_pause_pressed();
void on_pause_released();
void on_mode_released();
int check_file_exists();
/****************************************************************************
* RPM interrupt thread declarations 
****************************************************************************/
volatile int rpmInterruptCount;
volatile int rpmValue;
volatile int rpmFrequency;
volatile uint64_t rpmStartTime, rpmEndTime;
struct timespec rpmStartTimeStruct,rpmEndTimeStruct; 
/****************************************************************************
* Logging Mode -- toggled by MODE button 
****************************************************************************/
int loggingMode = 0;        // Logging inactive by default
int printOut = 1;           // Printing out data enabled by default
int MAX_LOG_SIZE = 200000;  // In terms of bytes
/****************************************************************************
* int main() 
*
* The main function follows standard robotics_cape layout 
* - call to rc_initialize() at the beginning
* - main while loop that continuously checks for EXITING condition
* - rc_cleanup() at the end
****************************************************************************/
int main(int argc, char * argv[]){
    // Initialize cape library first
    if(rc_initialize()){
       fprintf(stderr,"ERROR: failed to initialize rc_initialize(),    are 
    you root?\n");
    return -1;
    }
    // Done initializing so set state to RUNNING
    rc_set_state(RUNNING); 
    // Check input arguments
    int argInc;
    for(argInc = 0; argInc < argc; argInc++){
        printf("%s\n", argv[argInc]);
        if(!strcmp(argv[argInc],"-no-print")) {
            printOut = 0;    
        }
        if(!strcmp(argv[argInc], "-paused")){
            rc_set_state(PAUSED);
        }
        if(!strcmp(argv[argInc], "-log")){
            loggingMode = 1;
        }
    }
    // Display startup message
    printf("\nHello from BeagleBone Blue!\n");
    printf("Press Mode button to toggle logging\n");
    printf("Press Pause button to pause program\n");
    printf("Hold Pause to terminate program\n");
    printf("Pressing Ctrl + C also terminates program\n");
    printf("\nThis is a test of all implemented sensors\n");
    // Setup button handlers
    rc_set_pause_pressed_func(&on_pause_pressed);
    rc_set_pause_released_func(&on_pause_released);
    rc_set_mode_released_func(&on_mode_released);
    // Open logging file for writing
    FILE * logFile;
    char logFilePath[100];
    struct stat st;
    float logFileSize = 0;
    int activeLoopCount = 0;
    // Time related variables
    struct timeval tv;
    struct tm* ptm;
    char time_string[40];
    char log_filename_time_string[40];
    long milliseconds;
    char currentTime[50];
    /* Obtain the time of day, and convert it to a tm struct.  */
    gettimeofday (&tv, NULL);
    ptm = localtime (&tv.tv_sec);
    /* Format the date and time, down to a single second.  */
    strftime (time_string, sizeof (time_string), "%Y-%m-%d %H:%M:%S", ptm);
    strftime (log_filename_time_string, sizeof (log_filename_time_string),  
    "%Y-%m-%d_%H-%M-%S", ptm);
    /* Compute milliseconds from microseconds.  */
    milliseconds = tv.tv_usec / 1000;
    snprintf(logFilePath, sizeof(logFilePath),    
    "/home/esaero/VTOL_Sensors_Test/logs/loggedData_%s.csv", 
    log_filename_time_string);
    logFile = fopen(logFilePath, "w");
    fprintf(logFile, "Time, ADXL345 X (G),ADXL345 Y (G),ADXL345 Z (G),TMP36
    (V),TMP36 (°C) \
    ,TMP36 (°F),TMP007 Object °C,TMP007 Object °F,TMP007 Local °C,TMP007 
    Local °F,\
    Sound (V),Sound (Raw),Sound (Scaled),RPM Interrupts (Hz),RPM,Current 
    (A)\n");
    // Initialize ADXL345 Accelerometer
    ADXL345_sensor accel_sensor;
    init_ADXL345();
    // Initialize TMP36  
    int TMP36_Channel = 0;
    float TMP36TempInVolts;
    float TMP36TempInC;
    float TMP36TempInF;
    // Initialize LTSR 25-NP 
    int LTSR_Channel = 2;
    float LTSRCurrentInAmps;
    // Initialize Sound Detector 
    int Sound_Detector_Channel = 1;
    int soundRaw;
    float soundInVolts;
    float soundScaled;
    // Initialize RPM Sensor 
    initialize_rpm();
    set_rpm_rise_func(&on_rpm_rise);
    // Initialize TMP007 Infrared Thermopile Sensor 
    TMP007_sensor TMP007_temp_sensor;
    init_TMP007();
    float TMP007ObjectCelsius;
    float TMP007ObjectFahrenheit;
    float TMP007LocalCelsius;
    float TMP007LocalFahrenheit;
 
    printf("\n");
 
    // Keep looping until state changes to EXITING
    while(rc_get_state()!= EXITING){
        // Handle other states
        if(rc_get_state()== RUNNING){
            // Program running
            //--------------------------------------------------------------
            /* Obtain the time of day, and convert it to a tm struct.  */
            gettimeofday (&tv, NULL);
            ptm = localtime (&tv.tv_sec);
            /* Format the date and time, down to a single second.  */
            strftime (time_string, sizeof (time_string), 
        "%Y-%m-%d %H:%M:%S", ptm);
            /* Compute milliseconds from microseconds.  */
            milliseconds = tv.tv_usec / 1000;
            float startMS = milliseconds;
            /* Print the formatted time, in seconds, with decimal point
               and the milliseconds.  */
            snprintf(currentTime, sizeof(currentTime), "%s.%03ld", 
        time_string, milliseconds);
            /***ADXL345Readings ********************************************/
            get_ADXL345(&accel_sensor);
            /***TMP36 Readings**********************************************/
            TMP36TempInVolts = get_TMP36_volts(TMP36_Channel);
            TMP36TempInC = get_TMP36_C(TMP36_Channel);
            TMP36TempInF = get_TMP36_F(TMP36_Channel);  
            /***LTSR 25-NP Readings*****************************************/
            LTSRCurrentInAmps = get_LTSR_amps(LTSR_Channel);
            /***Sound Detector Readings*************************************/
            soundInVolts = get_Sound_Detector_volts(Sound_Detector_Channel);
            soundScaled = get_Sound_Detector_scaled(Sound_Detector_Channel);
            soundRaw = get_Sound_Detector_raw(Sound_Detector_Channel);
            /***RPM Readings********************************************/
            // Stored in rpmFrequency and rpmValue through interrupts
            /***TMP007 Readings*********************************************/
            get_TMP007_objectTemp(&TMP007_temp_sensor);
            TMP007ObjectCelsius = TMP007_temp_sensor.objectTemp;
            TMP007ObjectFahrenheit = 
        convert_to_fahrenheit(TMP007ObjectCelsius);
            get_TMP007_localTemp(&TMP007_temp_sensor);
            TMP007LocalCelsius = TMP007_temp_sensor.localTemp;
            TMP007LocalFahrenheit =
        convert_to_fahrenheit(TMP007LocalCelsius);
            /***Print Values in table***************************************/
            if (printOut){
                printf("\r");
                printf("\nHello from BeagleBone Blue!\n");
                printf("Press Mode button to toggle logging\n");
                printf("Press Pause button to pause program\n");
                printf("Hold Pause to terminate program\n");
                printf("Pressing Ctrl + C also terminates program\n");
                printf("\nThis is a test of all implemented sensors\n");
                printf("\n\n");
                printf("+----------------------------------------------------
         ----------------------------+\n");
                printf("|                           VTOL PHM Sensor Readings                             
         |\n");
                printf("|                             Logging: %s                                  
         |\n", \
                loggingMode ? "Active  ":"Inactive");
                printf("| Location:%s|\n", \
                logFilePath);
                printf("|                             Log Size: %6.3f KB / 
         %6.3f KB                   |\n", \
                logFileSize/1000, MAX_LOG_SIZE/1000.0); 
                printf("+----------------------------------------------------
         ----------------------------+\n");
                printf("| ADXL345            | X:                   | Y:                  
         | Z:           |\n");
                printf("| Accelerometer      | %6.3f G             | %6.3f G            
         | %6.3f G     |\n", \
                accel_sensor.X, accel_sensor.Y, accel_sensor.Z);
                printf("+--------------------+----------------------+--------
          -------------+--------------+\n");
                printf("| TPM36              | Volts:               | °C                  
         | °F           |\n");
                printf("| Temperature Sensor | %6.3f V             | %6.3f °C           
         | %6.3f °F    |\n", \
                TMP36TempInVolts, TMP36TempInC, TMP36TempInF);
                printf("+--------------------+----------------------+--------
         -------------+--------------+\n");
                printf("| TMP007             | Object Temperature   | Local 
         Temperature   |              |\n");
                printf("| Temperature Sensor | %6.3f °C| %6.3f °F | %6.3f °C| 
         %6.3f °F|              |\n", \
                TMP007ObjectCelsius, TMP007ObjectFahrenheit, 
         TMP007LocalCelsius, TMP007LocalFahrenheit);
                printf("+--------------------+----------------------+--------
         -------------+--------------+\n");
                printf("| Sound Detector     | Volts:               | Raw:                
         | Scaled:      |\n");
                printf("|                    | %6.3f V             | %d                 
         | %6.3f       |\n", \
                soundInVolts, soundRaw, soundScaled);
                printf("+--------------------+----------------------+--------
         -------------+--------------+\n");
                printf("| Eagle Tree         | Interrupt Frequency: | RPM:                
         | Motor Poles: |\n");
                printf("| RPM Sensor         |  %4d Hz             | %5d               
         | %2d           |\n",\
                rpmFrequency, rpmValue, motorPoles);
                printf("+--------------------+----------------------+--------
         -------------+--------------+\n");
                printf("| LEM LTSR 25-NP     | Amps:                |                     
         |              |\n");
                printf("| Current Sensor     | %6.3f A             |                     
         |              |\n", \
                LTSRCurrentInAmps); 
                printf("+--------------------+----------------------+--------
         -------------+--------------+\n"); 
                printf("\033[2J");  // VT100 Terminal code for clearing 
            }
            /***Update filesize before write********************************/
            stat(logFilePath, &st);
            logFileSize = st.st_size;
            // Check if logging mode active (set using Mode button)
            if (loggingMode){
                // Check if file size limit has reached
                if(logFileSize > MAX_LOG_SIZE){
                    fclose(logFile);
                    // Obtain the time of day, and convert it to a tm struct
                    gettimeofday (&tv, NULL);
                    ptm = localtime (&tv.tv_sec);
                    /* Format the date and time, down to a single second.  */
                    strftime (time_string, sizeof (time_string), "%Y-%m-%d 
             %H:%M:%S", ptm);
                    strftime (log_filename_time_string, sizeof 
             (log_filename_time_string), "%Y-%m-%d_%H-%M-%S", ptm);
                    /* Compute milliseconds from microseconds.  */
                    milliseconds = tv.tv_usec / 1000;
                    // Print the formatted time,and the milliseconds.  */
                    snprintf(currentTime, sizeof(currentTime), "%s.%03ld", 
             time_string, milliseconds);
                    snprintf(logFilePath, sizeof(logFilePath), 
             "/home/esaero/VTOL_Sensors_Test/logs/
             loggedData_%s.csv", log_filename_time_string);
                    logFile = fopen(logFilePath, "w");
                    fprintf(logFile, "Time,ADXL345 X (G),ADXL345 Y 
             (G),ADXL345 Z (G),TMP36 (V),TMP36 (°C) \
                    ,TMP36 (°F),TMP007 Object °C,TMP007 Object °F,TMP007 
             Local °C,TMP007 Local °F,\
                    Sound (V),Sound (Raw),Sound (Scaled),RPM Interrupts 
             (Hz),RPM,Current (A)\n");
                    usleep(100);
                }
                fprintf(logFile, "%s,%6.3f,%6.3f,%6.3f,%6.3f,%6.3f,%6.3f,
         %6.3f,%6.3f,%6.3f,%6.3f,%6.3f,%d,%6.3f,%d,%d,%6.3f\n", \
                currentTime, accel_sensor.X, accel_sensor.Y, accel_sensor.Z, 
         TMP36TempInVolts, TMP36TempInC, TMP36TempInF, \
                TMP007ObjectCelsius, TMP007ObjectFahrenheit, 
         TMP007LocalCelsius, TMP007LocalFahrenheit, \
                soundInVolts, soundRaw, soundScaled, rpmFrequency, rpmValue, 
         LTSRCurrentInAmps);
                usleep(100);
                // Switch LED mode every 50 loops/logs
                if (activeLoopCount % 100 < 50) {
                    rc_set_led(GREEN, ON);
                    rc_set_led(RED, OFF);
                } else {
                    rc_set_led(GREEN, OFF);
                    rc_set_led(RED, OFF);
                }
            } else {
                if (activeLoopCount % 100 < 50) {
                    rc_set_led(GREEN, OFF);
                    rc_set_led(RED, ON);
                } else {
                    rc_set_led(GREEN, OFF);
                    rc_set_led(RED, OFF);
                }
            }
            // Increment loop count
            activeLoopCount++;
    } else if(rc_get_state()== PAUSED){
            // Program paused 
            printf("\r");
            printf("PAUSED");
            printf("%*c", 100, ' '); fflush(stdout);
 
            /***Hold Red LED on*******************************************/
            rc_set_led(GREEN, OFF);
            rc_set_led(RED, ON);
        }
        // Always sleep at some point
        if(printOut){
            // Sleep longer in printOut mode
            usleep(50000);
        } else {
            // Sleep for a shorter duration to allow faster logging.
            usleep(20000);
        }
    }
    // Exit cleanly
        fclose(logFile);
       wait_for_RPM_handler_to_join();
    rc_cleanup(); 
    return 0;
}
 
/****************************************************************************
* void on_pause_released() 
*    
* Make the Pause button toggle between paused and running states.
****************************************************************************/
void on_pause_released(){
    // toggle betewen paused and running modes
    if(rc_get_state()== RUNNING)        rc_set_state(PAUSED);
    else if(rc_get_state()== PAUSED)    rc_set_state(RUNNING);
    return;
}
 
/****************************************************************************
* void on_pause_pressed() 
*
* If the user holds the pause button for 2 seconds, set to exiting which 
* triggers the rest of the program to exit cleanly.
****************************************************************************/
void on_pause_pressed(){
    int i = 0;
    const int samples = 100;        // Check for release 100 times
    const int us_wait = 2000000;    // 2 seconds
    
    // Now keep checking to see if the button is still held down
    for(i=0;i<samples;i++){
        rc_usleep(us_wait/samples);
        if(rc_get_pause_button() == RELEASED) return;
    }
    printf("\nLong press detected, shutting down\n");
    rc_set_state(EXITING);
    return;
}
/****************************************************************************
* void on_mode_released() 
*    
* Toggles logging mode for the sensors. 
****************************************************************************/
void on_mode_released(){
    loggingMode = !loggingMode;
    return;
}
/****************************************************************************
 * int check_file_exists(int num) - currently unused
 *
 * Checks to see if the numbered file exists.
****************************************************************************/
int check_file_exists(int num){
    FILE * fileCheck;
    char logFilePath[25];
    sprintf(logFilePath, "./logs/loggedData%d.csv", num);
    fileCheck = fopen(logFilePath, "r");
    if (fileCheck){
        fclose(fileCheck);
        return 1;
    } else {
        return 0;
    }
}
/***************************************************************************/