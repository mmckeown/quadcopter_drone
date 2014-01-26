/*
 * BMP085.cpp - Library for interfacing with BMP085 pressure and temperature sensor
 * Created by Michael P. McKeown, January 25, 2014.
 * Currently just for personal use.
 */
 
#include "BMP085.h"

BMP085::BMP085 ()
{
}

BMP085::~BMP085 ()
{
}

int16_t BMP085::readRawTempSync ()
{
  writeReg (CTRL_REG, TEMPERATURE);
  
  delay (4.5);
  
  return ((readReg (VALUE_MSB_REG) << 8) | readReg (VALUE_LSB_REG));
}

int32_t BMP085::readRawPressureSync (uint8_t _oss)
{ 
  writeReg (CTRL_REG, PRESSURE_OSRS0 | (_oss << 6));
  
  delay (7.5);
  
  return (((readReg (VALUE_MSB_REG) << 16) | (readReg (VALUE_LSB_REG) << 8) | readReg (VALUE_XLSB_REG)) >> (8 - _oss));
}

uint8_t BMP085::readReg (const uint8_t _reg)
{
  uint8_t retval;
  
  Wire.beginTransmission (ADDRESS);
  Wire.write (_reg);
  Wire.endTransmission ();
  
  Wire.requestFrom (ADDRESS, REG_WIDTH);
  while (Wire.available ())
    retval = Wire.read ();
    
  return retval;
}

void BMP085::writeReg (const uint8_t _reg, const uint8_t _val)
{
  Wire.beginTransmission (ADDRESS);
  Wire.write (_reg);
  Wire.write (_val);
  Wire.endTransmission ();
}
