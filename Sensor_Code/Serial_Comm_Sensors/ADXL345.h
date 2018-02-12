/***************************************************************************
 * ADXL345.h
 ***************************************************************************/
#ifndef ADXL345_H
#define ADXL345_H
 
#define I2C_BUS_1       1              // I2C BUS 1 is external 
#define ADXL345_ADDR    (0x53)         // ADXL345 I2C address
#define DEV_ID_REG      (0x00)         // Chip ID - set to 0xE5 for ADXL345
#define PWR_ON          (0x08)
 
// Register Definitions
#define POWER_CTL       0x2D     //Power Control Register
#define DATAX0          0x32     //X-axis Data 0
#define DATAX1          0x33     //X-axis Data 1
#define DATAY0          0x34     //Y-axis Data 0
#define DATAY1          0x35     //Y-axis Data 1
#define DATAZ0          0x36     //Z-axis Data 0
#define DATAZ1          0x37     //Z-axis Data 1
 
// Debug Mode Definitions
extern const int DEBUG_MODE;
extern const int D_ALL;
extern const int D_ADXL345;
 
// Struct declerations
typedef struct ADXL345_Struct{
    float X;
    float Y;
    float Z;
}ADXL345_sensor;
 
// User Functions
void init_ADXL345();
void get_ADXL345(ADXL345_sensor * sensor);
 
#endif
 

/****************************************************************************
* ADXL345.c
*
* Reading the outputs for X, Y, and Z accelerations. 
* Most options are left in their default state
* +/- 2G mode - 10 bit resolution
****************************************************************************/
#include <rc_usefulincludes.h> 
#include <roboticscape.h>
#include "ADXL345.h"
 
// Static function declarations
static void read_raw_accel_values(uint8_t * x0, uint8_t * x1, uint8_t * y0, uint8_t * y1, uint8_t * z0, uint8_t * z1);
static short combine_registers(int msb, int lsb);
static void convert_to_Gs(ADXL345_sensor *sensor);
 
/****************************************************************************
 * init_ADXL345()
 *
 * Initialize I2C bus 1
 * Attempt to verify parameters:
 * I2C Address:     0x53
 * Device ID:       0xE5 
****************************************************************************/
void init_ADXL345(){
    if(rc_i2c_init(I2C_BUS_1, ADXL345_ADDR)<0){
        printf("ADXL345: ERROR -  failed to initialize i2c bus\n");
        printf("ADXL345: aborting rc_i2c_init\n");
        rc_set_state(EXITING);
    }
 
    // I2C Command: read chip ID address and make sure it is correct
    uint8_t dev_addr_read;
    rc_i2c_read_byte(I2C_BUS_1,DEV_ID_REG, &dev_addr_read);
    if(dev_addr_read != 0xe5) {
        printf("ADXL345: not matching device ID\n");
        rc_set_state(EXITING);
    } else {
        printf("ADXL345: Device ID of the ADXL345 sensor is: 
    %#04x\n",dev_addr_read);
        printf("ADXL345: I2C Address of the ADXL345 sensor is: %#4x\n", 
    ADXL345_ADDR);
    }
    
    // I2C Command: Turn on measurements
    if(rc_i2c_write_byte(I2C_BUS_1, POWER_CTL, PWR_ON) !=0){
        printf("ADXL345: Failed to power on measurement unit\n");
    }
}
 
/****************************************************************************
 * get_ADXL345(ADXL345 * sensor)
 *
 * Get ADXL345 readings in terms of Gs.
 * Input: ADXL345 struct
 ***************************************************************************/
void get_ADXL345(ADXL345_sensor * sensor){
    rc_i2c_claim_bus(I2C_BUS_1);   // claim I2C bus 
    rc_i2c_set_device_address(I2C_BUS_1, ADXL345_ADDR);   // claim I2C bus 
    uint8_t x0, x1;     //Raw X-axis measurements, stored in 2 registers
    uint8_t y0, y1;     //Raw Y-axis measurements, stored in 2 registers
    uint8_t z0, z1;     //Raw Z-axis measurements, stored in 2 registers
    read_raw_accel_values(&x0, &x1, &y0, &y1, &z0, &z1);
    sensor->X = combine_registers(x1, x0);
    sensor->Y = combine_registers(y1, y0);
    sensor->Z = combine_registers(z1, z0);
    if(DEBUG_MODE == D_ADXL345 || DEBUG_MODE == D_ALL){
        printf("RAW VALUES: X: %4.2f\t Y:%4.2f\t Z: %4.2f\n", sensor->X, 
    sensor->Y, sensor->Z);
    }
    convert_to_Gs(sensor);
    if(DEBUG_MODE == D_ADXL345 || DEBUG_MODE == D_ALL){
        printf("G Values: X: %4.2f\t Y:%4.2f \tZ: %4.2f\n ", sensor->X, 
    sensor->Y, sensor->Z);
    }
    rc_i2c_release_bus(I2C_BUS_1);  // release I2C bus
}
 
/****************************************************************************
* static void read_raw_accel_values(uint8_t * x0, uint8_t * x1, uint8_t * y0, uint8_t * y1, uint8_t * z0, uint8_t * z1)
*
* Reads raw acceleration values from the 6 registers (0x32-0x37)
* MSB in x1, y1, z1
* LSB in x0, y0, z0
* Data in (2's complement form)
****************************************************************************/
static void read_raw_accel_values(uint8_t * x0, uint8_t * x1, uint8_t * y0, uint8_t * y1, uint8_t * z0, uint8_t * z1){
    rc_i2c_read_byte(I2C_BUS_1, DATAX0, x0);  // NO &'s b/c already pointers
    rc_i2c_read_byte(I2C_BUS_1, DATAX1, x1);
    rc_i2c_read_byte(I2C_BUS_1, DATAY0, y0);
    rc_i2c_read_byte(I2C_BUS_1, DATAY1, y1);
    rc_i2c_read_byte(I2C_BUS_1, DATAZ0, z0);
    rc_i2c_read_byte(I2C_BUS_1, DATAZ1, z1); 
    if(DEBUG_MODE == D_ADXL345){
        printf("\r");
        printf("ADXL345: Before Combination: x0: %d\t x1: %d\n", *x0, *x1);
        printf("ADXL345: Before Combination: y0: %d\t y1: %d\n", *y0, *y1);
        printf("ADXL345: Before Combination: z0: %d\t z1: %d\n", *z0, *z1);
    }
}
/****************************************************************************
 * static short combine_registers(int msb, int lsb)
 *
 * Takes the two register values and shifts the 8 bit MSB to the front
 * Must be 'short' to preserve sign bit (2's complement form)
****************************************************************************/
static short combine_registers(int msb, int lsb){
    return (msb << 8) | lsb ;
}
 
/****************************************************************************
 * static void convert_to_Gs(ADXL345_sensor * sensor)
 * 
 * Converts the raw value created from the two registers into G value.
 * factor calculation:
 * gravity range = 4 (+/- 2G)
 * resolution = 1024 (2^10bits)
 * factor = (gravity range) / (resolution)
 * ***************************************************************************/
static void convert_to_Gs(ADXL345_sensor * sensor){
    float factor = .0039 ;
    float originalX = sensor->X;
    float originalY = sensor->Y;
    float originalZ = sensor->Z;
    sensor->X = originalX * factor;
    sensor->Y = originalY * factor;
    sensor->Z = originalZ * factor;
    if (DEBUG_MODE == D_ADXL345){
        printf("ADXL345: Original Z: %4.2f\n", originalZ);
        printf("ADXL345: New sensor->Z %4.2f\n", sensor->Z);
    }
}

/****************************************************************************
 * LTSR.h
 *
 ***************************************************************************/
#ifndef LTSR 
#define LTSR
 
extern const int DEBUG_MODE;
extern const int D_ALL;
extern const int D_LTSR;
 
// User Functions
float get_LTSR_volts(int channel);
float get_LTSR_amps(int channel);
 
#endif