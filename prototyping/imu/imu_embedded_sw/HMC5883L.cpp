/*
 * HMC5883L.cpp - Library for interfacing with HMC5883L magnometer
 * Created by Michael P. McKeown, January 25, 2014.
 * Currently just for personal use.
 */
 
#include "HMC5883L.h"

HMC5883L::HMC5883L ()
{
}

HMC5883L::~HMC5883L ()
{
}

uint8_t HMC5883L::readReg (const uint8_t _reg)
{
    // Send request to read reg
    Wire.beginTransmission (ADDRESS);
    Wire.write (_reg);
    Wire.endTransmission ();
  
    // Receive reg value back
    uint8_t val;
    Wire.requestFrom (ADDRESS, REG_WIDTH);
    while (Wire.available () < 0);
        val = Wire.read();
    
    return val;
}

void HMC5883L::writeReg (const uint8_t _reg, const uint8_t _val)
{ 
    // Send request to write
    Wire.beginTransmission (ADDRESS);
    Wire.write (_reg);
    Wire.write (_val);
    Wire.endTransmission ();
}

HMC5883L::vector16b HMC5883L::readRaw ()
{
  // Send request with auto-increment enabled
  Wire.beginTransmission (ADDRESS);
  Wire.write (DATA_OUT_X_MSB_REG);
  Wire.endTransmission ();
  
  // Receive 6 byte successive transmission
  Wire.requestFrom (ADDRESS, 6);
  while (Wire.available () < 6);
  uint8_t xh = Wire.read();
  uint8_t xl = Wire.read ();
  uint8_t zh = Wire.read ();
  uint8_t zl = Wire.read ();
  uint8_t yh = Wire.read ();
  uint8_t yl = Wire.read ();
  
  // Aggregate high and low bytes
  vector16b retval;
  retval.x = (int16_t)(xh << 8 | xl);
  retval.y = (int16_t)(yh << 8 | yl);
  retval.z = (int16_t)(zh << 8 | zl);
  
  return retval;
}
