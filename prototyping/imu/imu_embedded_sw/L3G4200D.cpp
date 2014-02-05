/*
 * L3G4200D.cpp - Library for interfacing with L3G4200D gyroscope
 * Created by Michael P. McKeown, January 25, 2014.
 * Currently just for personal use.
 */
 
#include "L3G4200D.h"
 
L3G4200D::L3G4200D ()
  : m_initialized (false),
    m_timer (),
    m_zeroRateInit (false),
    m_rotVelCB (NULL),
    m_ovrnCB (NULL)
{
  m_zeroRate.x = 0;
  m_zeroRate.y = 0;
  m_zeroRate.z = 0;
}

L3G4200D::~L3G4200D ()
{
}

void L3G4200D::registerRotationalVelocityCallback (RotationalVelocityCallback _cb)
{
  m_rotVelCB = _cb;
}

void L3G4200D::registerOverrunCallback (OverrunCallback _cb)
{
  m_ovrnCB = _cb;
}

void L3G4200D::init ()
{
  if (m_initialized)
    return;
    
  // Initialize to default data rate and bandwidth,
  // exit power down mode, and enabled all axes  
  writeReg (CTRL_REG1, L3G4200D::DR0 | 
                       L3G4200D::BW0 |
                       L3G4200D::PD_DISABLE |
                       L3G4200D::Z_ENABLE |
                       L3G4200D::Y_ENABLE |
                       L3G4200D::X_ENABLE);
    
  m_initialized = true;
}

// _int2ISR should just call L3G4200D::int2ISR
void L3G4200D::initAsync (int _int2Pin, ISRFunc _int2ISR)
{ 
  // Disable all axes and enter power down mode
  writeReg (CTRL_REG1, 0);
  m_initialized = false;
  writeReg (CTRL_REG3, 0);
  
  // Unfortunately, the breakout board does not break out the int2 pint
  // thus I have to use a timer.  This should be an interrupt tho
  // Setup hardware interrupt
  //pinMode (_int2Pin, INPUT);
  //attachInterrupt( _int2Pin, _int2ISR, RISING);
  
  // Enable data ready interrupt on INT2 pin
  //writeReg (CTRL_REG3, I2_DRDY);
  
  // Setup timer
  m_timer.begin (_int2ISR, 100000); // Fire timer at 10 Hz (should be higher)
  
  // Do normal initialization
  init ();
}

void L3G4200D::calibrateZeroRate ()
{
  if (!m_initialized)
    return;
 
  if (!m_zeroRateInit)
  {
    vectord cum;
    cum.x = 0;
    cum.y = 0;
    cum.z = 0;
    for (int32_t i = 0; i < ZERO_RATE_SAMPLES; i++)
    {
      // Wait for data to be ready
      bool drdy = false;
      bool ovrn;
      while (!drdy)
        dataReady (drdy, ovrn);
        
      // Read data
      vector16b rawData = readRaw ();
      cum.x += rawData.x;
      cum.y += rawData.y;
      cum.z += rawData.z;
    }
    
    m_zeroRate.x = ((int32_t) -cum.x) / ZERO_RATE_SAMPLES;
    m_zeroRate.y = ((int32_t) -cum.y) / ZERO_RATE_SAMPLES;
    m_zeroRate.z = ((int32_t) -cum.z) / ZERO_RATE_SAMPLES;
    
    m_zeroRateInit = true;
  }
}

void L3G4200D::int2ISR ()
{
  bool drdy, ovrn;
  dataReady (drdy, ovrn);
  
  if (m_rotVelCB && drdy)
  { 
    // Read raw rotational velocity
    vector16b rawRotVel = readRaw ();
    
    // Compensate for zero rate
    rawRotVel.x += m_zeroRate.x;
    rawRotVel.y += m_zeroRate.y;
    rawRotVel.z += m_zeroRate.z;
      
    // Make callback
    m_rotVelCB (rawRotVel);
  }
  
  if (m_ovrnCB && ovrn)
    m_ovrnCB ();
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

void L3G4200D::dataReady (bool &_drdy, bool &_ovrn)
{
  _drdy = false;
  _ovrn = false;
  
  uint8_t val;
  val = readReg (STATUS_REG);
  if (val & ZYXDA_MASK)
    _drdy = true;
  if (val & ZYXOR_MASK)
    _ovrn = true;
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
