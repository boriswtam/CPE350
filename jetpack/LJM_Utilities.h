#ifndef LJM_UTILITIES
#define LJM_UTILITIES


#ifdef _WIN32
	#include <Winsock2.h>
	#include <ws2tcpip.h>
#else
	#include <unistd.h> // For sleep() (with Mac OS or Linux).
	#include <arpa/inet.h>  // For inet_ntoa()
	#include <sys/time.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include <LabJackM.h>

#define COMMAND 1
#define RESPONSE 0

#define INITIAL_ERR_ADDRESS -2
// This is just something negative so normal addresses are not confused with it


// Prompts the user to press enter
void WaitForUser();

// Calls WaitForUser if this is compiled on Windows. This call is intended to be
// placed at the end of a program to prevent output from being lost in the case
// that it is being called from Visual Studio.
void WaitForUserIfWindows();

// Returns a new handle or exits on error with a description of the error.
int OpenOrDie(int deviceType, int connectionType, const char * identifier);
int OpenSOrDie(const char *  deviceType, const char *  connectionType,
	const char * identifier);

// Closes the handle or exits on error with a description of the error.
void CloseOrDie(int handle);

// Print device info
void PrintDeviceInfoFromHandle(int handle);
void PrintDeviceInfo(int deviceType, int connectionType, int serialNumber,
	int ipAddressInt, int portOrPipe, int MaxBytesPerMB);

// Queries the device for value and prints the results
double GetAndPrint(int handle, const char * valueName);

// Queries the device for value and prints the results as an IP string
void GetAndPrintIPAddress(int handle, const char * valueName);

// Queries the device for value and prints the results as an MAC string.
// valueName is just a description, while valueAddress the address that
// determines what value is actually read from the device. valueAddress must be
// of type UINT64
void GetAndPrintMACAddressFromValueAddress(int handle, const char * valueName,
	int valueAddress);

// Queries the device for an address and prints the results
void GetAndPrintAddressAndType(int handle, const char * valueDescription,
	int address, int type);

// Queries LJM for the config value via LJM_ReadLibraryConfigS.
// Passes configParameter as Parameter and prints Value.
void GetAndPrintConfigValue(const char * configParameter);

// Queries LJM for the config value via LJM_ReadLibraryConfigStringS.
// Passes configParameter as Parameter and prints Value.
void GetAndPrintConfigString(const char * configParameter);

// Prints the current LJM logging configurations
void DisplayDebugLoggingConfigurations();

// Sets the LJM config value via LJM_WriteLibraryConfigS.
// Outputs error, if any
void SetConfigValue(const char * configParameter, double value);

// Sets the LJM config value via LJM_WriteLibraryConfigStringS.
// Outputs error, if any
void SetConfigString(const char * configParameter, const char * string);

// Performs an LJM_eWriteName call.
// Prints error, if any. The "OrDie" version exits the program upon error
void WriteNameOrDie(int handle, const char * name, double value);
int WriteName(int handle, const char * name, double value);

void WriteNameAltTypeOrDie(int handle, const char * name, int type,
	double value);

// Performs an LJM_eWriteNames call.
// Prints error, if any. The "OrDie" version exits the program upon error
void WriteNamesOrDie(int handle, int NumFrames, const char ** aNames,
	const double * aValues);
int WriteNames(int handle,  int NumFrames, const char ** aNames,
	const double * aValues, int * errorAddress);

// Performs a LJM_eWriteNameArray/LJM_eWriteNameByteArray call.
// On error, prints message and exits the program.
void WriteNameArrayOrDie(int handle, const char * name, int numValues,
	const double * aValues);
void WriteNameByteArrayOrDie(int handle, const char * name, int numBytes,
	const char * aBytes);

// Performs a LJM_eReadNameArray/LJM_eReadNameByteArray call.
// On error, prints message and exits the program.
void ReadNameArrayOrDie(int handle, const char * name, int numValues,
	double * aValues);
void ReadNameByteArrayOrDie(int handle, const char * name, int numBytes,
	char * aBytes);

/**
 * Name: ErrorCheck
 * Desc: If err is not LJME_NOERROR, displays the error and exits the program.
 * Para: err, the error code being checked
 *       formattedDescription, a string representing what sort of operation
 *                             returned err. This can have formatting arguments
 *                             in it, just like a string passed to printf
**/
void ErrorCheck(int err, const char * formattedDescription, ...);

/**
 * Desc: Prints the error if there is an error
**/
void PrintErrorIfError(int err, const char * formattedDescription, ...);
void PrintErrorWithAddressIfError(int err, int errAddress,
	const char * formattedDescription, ...);

// Displays the error, error name, and value name
void CouldNotRead(int err, const char * valueName);

/**
 * Name: ErrorCheckWithAddress
 * Desc: If err is not LJME_NOERROR, displays the error and exits the program.
 *       If errAddress is not negative, outputs the error address corresponding
 *       to err
 * Para: err, the error code being checked
 *       formattedDescription, a string representing what sort of operation
 *                             returned err. This can have formatting arguments
 *                             in it, just like a string passed to printf
 *       errAddress, the address corresponding to err. If this is negative, it
 *                   will not be output. Note that LJM functions such as
 *                   LJM_eReadAddresses will not change the ErrorAddress unless
 *                   relevant
**/
void ErrorCheckWithAddress(int err, int errAddress,
	const char * formattedDescription, ...);

// Sleeps for the given number of milliseconds
void MillisecondSleep(unsigned int milliseconds);

// Turns on the specified level of logging
void EnableLoggingLevel(double logLevel);

/**
 * Desc: Gets the device type of an open device handle.
**/
int GetDeviceType(int handle);

/**
 * Name: NumberToDeviceType
 * Desc: Takes an integer representing a device type and returns the device name
 *       as a string
 * Para: deviceType, the device type whose name will be returned
 * Retr: the device name, or "Unknown device type" if deviceType is not
 *       recognized
**/
const char * NumberToDeviceType(int deviceType);

/**
 * Name: NumberToConnectionType
 * Desc: Takes an integer representing a connection type and returns the 
 *       connection type as a string
 * Para: connectionType, the connection type whose name will be returned
 * Retr: the connection type, or "Unknown connection type" if connectionType is
 *       not recognized
**/
const char * NumberToConnectionType(int connectionType);

/**
 * Name: NumberToDebugLogMode
 * Desc: Takes an integer representing a LJM_DEBUG_LOG_MODE and returns the mode
 *       name as a string
 * Retr: the mode name, or "Unknown LJM_DEBUG_LOG_MODE" if mode is not
 *       recognized
 * Note: See LJM_DEBUG_LOG_MODE in LabJackM.h
**/
const char * NumberToDeviceType(int mode);

/**
 * Desc: Returns the current CPU time in milliseconds
**/
unsigned int GetCurrentTimeMS();

/**
 * Desc: Returns IPv4String in integer form, handling error by calling
 *       ErrorAddress
**/
unsigned int IPToNumber(const char * IPv4String);

/**
 * Desc: Returns 0 (false) if v1 and v2 are not within delta of each other,
 *       returns 1 (true) if they are within delta each other
**/
int EqualFloats(double v1, double v2, double delta);

/**
 * Desc: Returns 1 (true) if connectionType is TCP-based,
 *       returns 0 (false) if not.
**/
int IsTCP(int connectionType);

/**
 * Desc: Returns the address of named register.
**/
int GetAddressFromNameOrDie(const char * name);

/**
 * Desc: Returns true if connectionType is a network type,
 *       returns false if connectionType is USB
**/
int IsNetwork(int connectionType);

/**
 * Desc: Determines whether or not a device has WiFi capability. E
 * Retr: Returns 1 if the device referred to by handle has WiFi installed,
 *       returns 0 otherwise
**/
int DoesDeviceHaveWiFi(int handle);


// Source

const char * NumberToDebugLogMode(int mode);

const char * NumberToConnectionType(int connectionType);

const char * NumberToDeviceType(int deviceType);

int GetDeviceType(int handle);

void PrintDeviceInfoFromHandle(int handle);

void PrintDeviceInfo(int deviceType, int connectionType, int serialNumber,
	int ipAddressInt, int portOrPipe, int packetMaxBytes);

void WaitForUserIfWindows();

void WaitForUser();

int OpenOrDie(int deviceType, int connectionType, const char * identifier);

int OpenSOrDie(const char *  deviceType, const char *  connectionType,
	const char * identifier);

void CloseOrDie(int handle);

void PrintErrorAddressHelper(int errAddress);

typedef enum {
	ACTION_PRINT_AND_EXIT,
	ACTION_PRINT
} ErrorAction;

// The "internal" print function for ErrorCheck and ErrorCheckWithAddress
void _ErrorCheckWithAddress(int err, int errAddress, ErrorAction action,
	const char * description, va_list args);

void PrintErrorIfError(int err, const char * formattedDescription, ...);

void PrintErrorWithAddressIfError(int err, int errAddress,
	const char * formattedDescription, ...);

void ErrorCheck(int err, const char * formattedDescription, ...);

void ErrorCheckWithAddress(int err, int errAddress, const char * description,
	...);

void MillisecondSleep(unsigned int milliseconds);

double GetAndPrint(int handle, const char * valueName);

void GetAndPrintIPAddress(int handle, const char * valueName);

void GetAndPrintMACAddressFromValueAddress(int handle, const char * valueName,
	int valueAddress);

void GetAndPrintAddressAndType(int handle, const char * valueDescription,
	int address, int type);

void GetAndPrintConfigValue(const char * configParameter);

void GetAndPrintConfigString(const char * configParameter);

void SetConfigValue(const char * configParameter, double value);

void SetConfigString(const char * configParameter, const char * string);

void WriteNameOrDie(int handle, const char * name, double value);

int WriteName(int handle, const char * name, double value);

void WriteNameAltTypeOrDie(int handle, const char * name, int type,
	double value);

void WriteNamesOrDie(int handle, int NumFrames, const char ** aNames,
	const double * aValues);

int WriteNames(int handle, int NumFrames, const char ** aNames,
	const double * aValues, int * errorAddress);

void WriteNameArrayOrDie(int handle, const char * name, int numValues,
	const double * aValues);

void WriteNameByteArrayOrDie(int handle, const char * name, int numBytes,
	const char * aBytes);

void ReadNameArrayOrDie(int handle, const char * name, int numValues,
	double * aValues);

void ReadNameByteArrayOrDie(int handle, const char * name, int numBytes,
	char * aBytes);

void CouldNotRead(int err, const char * valueName);

void EnableLoggingLevel(double logLevel);

unsigned int GetCurrentTimeMS();

unsigned int IPToNumber(const char * IPv4String);

int EqualFloats(double v1, double v2, double delta);

int IsTCP(int connectionType);

int IsUDP(int connectionType);

int IsNetwork(int connectionType);

int DoesDeviceHaveWiFi(int handle);

void DisplayDebugLoggingConfigurations();

int GetAddressFromNameOrDie(const char * name);
#endif // #define LJM_UTILITIES

