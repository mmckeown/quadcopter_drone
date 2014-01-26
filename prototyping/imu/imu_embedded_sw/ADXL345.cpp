/*
 * ADXL345.cpp - Library for interfacing with ADXL345 accelerometer
 * Created by Michael P. McKeown, January 25, 2014.
 * Currently just for personal use.
 */
 
#include "ADXL345.h"

ADXL345::ADXL345 ()
{
}

ADXL345::~ADXL345 ()
{
}

uint8_t ADXL345::readReg (const uint8_t _reg)
{
    // Send request to read reg
    Wire.beginTransmission (ADDRESS);
    Wire.write (_reg);
    Wire.endTransmission ();
  
    // Receive reg value back
    uint8_t val;
    Wire.requestFrom (ADDRESS, REG_WIDTH);
    while (Wire.available ())
        val = Wire.read();
    
    return val;
}

void ADXL345::writeReg (const uint8_t _reg, const uint8_t _val)
{ 
    // Send request to write
    Wire.beginTransmission (ADDRESS);
    Wire.write (_reg);
    Wire.write (_val);
    Wire.endTransmission ();
}

ADXL345::vector16b ADXL345::readRaw ()
{
  // Send request with auto-increment enabled
  Wire.beginTransmission (ADDRESS);
  Wire.write (DATAX0_REG);
  Wire.endTransmission ();
  
  // Receive 6 byte successive transmission
  Wire.requestFrom (ADDRESS, 6);
  while (Wire.available () < 6);
  uint8_t xl = Wire.read();
  uint8_t xh = Wire.read ();
  uint8_t yl = Wire.read ();
  uint8_t yh = Wire.read ();
  uint8_t zl = Wire.read ();
  uint8_t zh = Wire.read ();
  
  // Aggregate high and low bytes
  vector16b retval;
  retval.x = (int16_t)(xh << 8 | xl);
  retval.y = (int16_t)(yh << 8 | yl);
  retval.z = (int16_t)(zh << 8 | zl);
  
  return retval;
}
