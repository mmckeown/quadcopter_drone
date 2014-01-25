/*
 * L3G4200D.cpp - Library for interfacing with L3G4200D gyroscope
 * Created by Michael P. McKeown, January 25, 2014.
 * Currently just for personal use.
 */
 
#include "L3G4200D.h"
 
L3G4200D::L3G4200D ()
  : m_sensitivity (0.00875)
{
    m_angle.x = 0;
    m_angle.y = 0;
    m_angle.z = 0;
}

L3G4200D::~L3G4200D ()
{
}

uint8_t L3G4200D::readReg (const uint8_t _reg)
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

void L3G4200D::writeReg (const uint8_t _reg, const uint8_t _val)
{
    // TODO: Check for correct reg and change
    //       sensitivity if necessary
  
    // Send request to write
    Wire.beginTransmission (ADDRESS);
    Wire.write (_reg);
    Wire.write (_val);
    Wire.endTransmission ();
}

L3G4200D::vector16b L3G4200D::readRaw ()
{
    // Send request with auto-increment enabled
    Wire.beginTransmission (ADDRESS);
    Wire.write (OUT_X_L_REG | (1 << 7));
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
