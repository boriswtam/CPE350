/****************************************************************************
* TMP007.c
*
*  
* Configuration register is left in the default state
* Conversion ON
* 4 Averages per conversion (1.01s for new value)
* Transient correction enabled
****************************************************************************/
#include <rc_usefulincludes.h> 
#include <roboticscape.h>
#include "TMP007.h"
 
/****************************************************************************
 * init_TMP007()
 *
 * Initialize I2C bus 1
 * Attempt to verify parameters:
 * I2C Address:     0x40
 * Device ID:       0x78 
****************************************************************************/
void init_TMP007(){
    if(rc_i2c_init(I2C_BUS_1, TMP007_ADDR)<0){
        printf("TMP007: ERROR -  failed to initialize i2c bus\n");
        printf("TMP007: aborting rc_i2c_init\n");
        rc_set_state(EXITING);
    }
 
    // I2C Command: read chip ID address and make sure it is correct
    uint16_t dev_addr_read;
    rc_i2c_read_word(I2C_BUS_1,TMP007_REG_DEV_ID, &dev_addr_read);
    if(dev_addr_read != TMP007_DEFAULT_DEV_ID) {
        printf("TMP007: not matching device ID\n");
        rc_set_state(EXITING);
    } else {
        printf("TMP007: Device ID of the TMP007 sensor is: 
    %#04x\n",dev_addr_read);
        printf("TMP007: I2C Address of the TMP007 sensor is: %#4x\n", 
    TMP007_ADDR);
    }
}
 
/****************************************************************************
 * get_TMP007_objectTemp(TMP007_sensor * sensor)
 *
 * Get TMP007 readings in terms of °C.
 * Input: TMP007_sensor struct
 ***************************************************************************/
void get_TMP007_objectTemp(TMP007_sensor * sensor){
    float conversionFactor = 0.03125;
    uint16_t objectTempRegValue = 0;
    rc_i2c_claim_bus(I2C_BUS_1);   // Claim I2C bus 
    rc_i2c_set_device_address(I2C_BUS_1, TMP007_ADDR);   // Claim I2C bus 
    rc_i2c_read_word(I2C_BUS_1, TMP007_REG_OBJECT_TEMPERATURE, 
    &objectTempRegValue);
    objectTempRegValue = (objectTempRegValue >> 2);
    sensor->objectTemp = objectTempRegValue * conversionFactor; 
    if(DEBUG_MODE == D_TMP007 || DEBUG_MODE == D_ALL){
        printf("TMP007: Object value is: %4.2f ", sensor->objectTemp);
    }
    rc_i2c_release_bus(I2C_BUS_1);  // relase I2C bus
}
/****************************************************************************
 * get_TMP007_localTemp(TMP007_sensor * sensor)
 *
 * Get TMP007 local temp readings in terms of °C.
 * Input: TMP007_sensor struct
 ***************************************************************************/
void get_TMP007_localTemp(TMP007_sensor * sensor){
    float conversionFactor = 0.03125;
    uint16_t localTempRegValue = 0;
    rc_i2c_claim_bus(I2C_BUS_1);   // claim I2C bus 
    rc_i2c_set_device_address(I2C_BUS_1, TMP007_ADDR);   // claim I2C bus 
    rc_i2c_read_word(I2C_BUS_1, TMP007_REG_LOCAL_TEMPERATURE, 
    &localTempRegValue);
    localTempRegValue = (localTempRegValue >> 2);   // Only top 14 bits 
    sensor->localTemp = localTempRegValue * conversionFactor; 
    if(DEBUG_MODE == D_TMP007 || DEBUG_MODE == D_ALL){
        printf("TMP007: Local value is: %4.2f ", sensor->localTemp);
    }
    rc_i2c_release_bus(I2C_BUS_1);  // release I2C bus
}
 
/****************************************************************************
 * float convert_to_fahrenheit(float celsius) 
 *
 * Convert Celsius to Fahrenheit
****************************************************************************/
float convert_to_fahrenheit(float celsius){
   return (celsius * 1.8) + 32; 
}
/***************************************************************************/