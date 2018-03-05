/****************************************************************************
 * TMP007.h
 *
 ***************************************************************************/
#ifndef TMP007_H
#define TMP007_H
 
#define I2C_BUS_1                       1         // I2C BUS 1 is external 
#define TMP007_ADDR                     0x40      // TMP007 I2C address
#define TMP007_DEFAULT_DEV_ID           0x78      // TMP007 default device id 
// Register Definitions
#define  TMP007_REG_DEV_ID              0x1F  // Should be 0x78
#define  TMP007_REG_SENSOR_VOLTAGE      0x00
#define  TMP007_REG_LOCAL_TEMPERATURE   0x01
#define  TMP007_REG_CONFIGURATION       0x02
#define  TMP007_REG_OBJECT_TEMPERATURE  0x03
#define  TMP007_REG_STATUS              0x04
// Debug Mode Definitions
extern const int DEBUG_MODE;
extern const int D_ALL;
extern const int D_TMP007;
 
// Struct declerations
typedef struct TMP007_Struct{
   float localTemp;
   float objectTemp;
}TMP007_sensor;
 
// User Functions
void init_TMP007();
void get_TMP007_objectTemp(TMP007_sensor * sensor);
void get_TMP007_localTemp(TMP007_sensor * sensor);
float convert_to_fahrenheit(float celsius);
 
#endif