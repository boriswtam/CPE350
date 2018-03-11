/**
 * Name: tmp007_i2c.c
 * Desc: Demonstrates I2C communication using the LJM driver.
 *       The demonstration uses a LJTick-DAC connected to FIO0/FIO1 for the T7
 *       or FIO4/FIO5 for the T4 and configures the I2C settings.
 *       This example configures I2C communication on the device, reads user
 *       memory on the LJTick-DAC, writes random bytes to user memory on the
 *       LJTick-DAC, then reads the user memory again.
**/

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <LabJackM.h>

// For LabJackM helper functions, such as OpenOrDie, PrintDeviceInfoFromHandle,
// ErrorCheck, etc.
#include "LJM_Utilities.h"
#include "tmp007_i2c.h"

void tmp007_i2c(struct tmp007_sensor_struct * tmp007_sensor)
{
	while (1) {
	//struct tmp007_sensor;

	// Variables for eNames
	const char * I2C_WRITE_NAME = "I2C_DATA_TX";
	const char * I2C_READ_NAME = "I2C_DATA_RX";
	int numBytes;
	char aBytes[5] = {0}; // TX/RX bytes will go here

	// Configure the I2C communication.
	if (GetDeviceType(tmp007_sensor->handle) == LJM_dtT4) {
		// For the T4, using FIO4 and FIO5 for SCL and SDA pins. FIO0 to FIO3 are
		// reserved for analog inputs, and digital lines are required.
		WriteNameOrDie(tmp007_sensor->handle, "I2C_SDA_DIONUM", 5);  // SDA pin number = 5 (FIO5)
		WriteNameOrDie(tmp007_sensor->handle, "I2C_SCL_DIONUM", 4);  // SCL pin number = 4 (FIO4)
	}
	else {
		// For the T7 and other devices, using FIO0 and FIO1 for the SCL and SDA
		// pins.
		WriteNameOrDie(tmp007_sensor->handle, "I2C_SDA_DIONUM", 13);  // SDA pin number = 1 (FIO1)
		WriteNameOrDie(tmp007_sensor->handle, "I2C_SCL_DIONUM", 12);  // SCL pin number = 0 (FIO0)
	}

	// Speed throttle is inversely proportional to clock frequency. 0 = max.
	WriteNameOrDie(tmp007_sensor->handle, "I2C_SPEED_THROTTLE", 0);

	// Options bits:
	//   bit0: Reset the I2C bus.
	//   bit1: Restart w/o stop
	//   bit2: Disable clock stretching.
	WriteNameOrDie(tmp007_sensor->handle, "I2C_OPTIONS", 0);

	// Slave Address of the I2C chip = 80 (0x50)
	WriteNameOrDie(tmp007_sensor->handle, "I2C_SLAVE_ADDRESS", 0x40);

	// Initial read of EEPROM bytes 0-3 in the user memory area. We need a
	// single I2C transmission that writes the chip's memory pointer and then
	// reads the data.
	WriteNameOrDie(tmp007_sensor->handle, "I2C_NUM_BYTES_TX", 1); // Set the number of bytes to transmit
	WriteNameOrDie(tmp007_sensor->handle, "I2C_NUM_BYTES_RX", 4); // Set the number of bytes to receive

	// Set the TX bytes. We are sending 1 byte for the address.
	numBytes = 1;

	aBytes[0] = 0x01; // Byte 0: Memory pointer = 0
	WriteNameByteArrayOrDie(tmp007_sensor->handle, I2C_WRITE_NAME, numBytes, aBytes);
	WriteNameOrDie(tmp007_sensor->handle, "I2C_GO", 1); // Do the I2C communications.

	// Read the RX bytes.
	numBytes = 1;
	ReadNameByteArrayOrDie(tmp007_sensor->handle, I2C_READ_NAME, numBytes, aBytes);
	tmp007_sensor->local_tmp = aBytes[0];
	
	// Set the TX bytes. We are sending 1 byte for the address.
	numBytes = 1;
	aBytes[0] = 0x03; // Byte 0: Memory pointer = 0
	WriteNameByteArrayOrDie(tmp007_sensor->handle, I2C_WRITE_NAME, numBytes, aBytes);

	WriteNameOrDie(tmp007_sensor->handle, "I2C_GO", 1); // Do the I2C communications.

	// Read the RX bytes.
	numBytes = 1;
	ReadNameByteArrayOrDie(tmp007_sensor->handle, I2C_READ_NAME, numBytes, aBytes);
	tmp007_sensor->object_tmp = aBytes[0];
	sleep(1);
	}
}

