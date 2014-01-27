/*
 * ADXL345.cpp - Library for interfacing with ADXL345 accelerometer
 * Created by Michael P. McKeown, January 25, 2014.
 * Currently just for personal use.
 */
 
#include "ADXL345.h"

const double ADXL345::FULL_RES_RESOLUTION = 3.90625;
const double ADXL345::LP_FILTER_ALPHA = 0.5;

ADXL345::ADXL345 ()
  : m_initialized (false),
    m_rangeSetting (RANGE_2G),
    m_fullResSetting (false),
    m_resolution (3.90625),
    m_lpFilter (false),
    m_accCB (NULL),
    m_prCB (NULL),
    m_ovrnCB (NULL)
{
  m_lpFilterPrev.x = 0;
  m_lpFilterPrev.y = 0;
  m_lpFilterPrev.z = 0;
}

ADXL345::~ADXL345 ()
{
}

void ADXL345::registerAccelerationCallback (AccelerationCallback _cb)
{
  m_accCB = _cb;
}

void ADXL345::registerOverrunCallback (OverrunCallback _cb)
{
  m_ovrnCB = _cb;
}

void ADXL345::registerPitchRollCallback (PitchRollCallback _cb)
{
  m_prCB = _cb;
}

void ADXL345::init ()
{
  if (m_initialized)
    return;
    
  writeReg (POWER_CTRL_REG, MEASURE_ENABLE);
  
  m_initialized = true;
}

// _int1ISR should just call ADXL345::int1ISR
void ADXL345::initAsync (int _int1Pin, ISRFunc _int1ISR)
{ 
  // Disable all interrupts and enter standby mode
  writeReg (POWER_CTRL_REG, 0);
  writeReg (INT_ENABLE_REG, 0);
  
  // Setup hardware interrupt
  pinMode (_int1Pin, INPUT);
  attachInterrupt( _int1Pin, _int1ISR, RISING);
  
  // Enable data ready interrupt on INT1 pin
  writeReg (INT_MAP_REG, 0);
  writeReg (INT_ENABLE_REG, DATA_RDY_ENABLE/* | OVERRUN_ENABLE*/);
  
  // Do normal initialization
  init ();
}

void ADXL345::int1ISR ()
{ 
  bool drdy, ovrn;
  dataReady (drdy, ovrn);

  if ((m_accCB || m_prCB) && drdy)
  {
    // Read raw acceleration data
    vector16b rawAcc = readRaw ();
    
    // Calculate mg acceleration
    vectord accmG;
    accmG.x = ((double) rawAcc.x) * m_resolution;
    accmG.y = ((double) rawAcc.y) * m_resolution;
    accmG.z = ((double) rawAcc.z) * m_resolution;
    
    // Filter signal if enabled
    if (m_lpFilter)
    {
      accmG.x = accmG.x * LP_FILTER_ALPHA + (m_lpFilterPrev.x * (1 - LP_FILTER_ALPHA));
      accmG.y = accmG.y * LP_FILTER_ALPHA + (m_lpFilterPrev.y * (1 - LP_FILTER_ALPHA));
      accmG.z = accmG.z * LP_FILTER_ALPHA + (m_lpFilterPrev.z * (1 - LP_FILTER_ALPHA));
      
      m_lpFilterPrev.x = accmG.x;
      m_lpFilterPrev.y = accmG.y;
      m_lpFilterPrev.z = accmG.z;
    }
    
    // Calculate pitch and roll
    double pitch = (atan2 (accmG.y, sqrt (accmG.x * accmG.x + accmG.z * accmG.z)) * 180.0) / PI;
    double roll = (atan2 (-accmG.x, accmG.z) * 180.0) / PI;
    
    // Make callback
    if (m_accCB)
      m_accCB (rawAcc, accmG);
    if (m_prCB)
      m_prCB (pitch, roll);
  }
  
  if (m_ovrnCB && ovrn)
    m_ovrnCB ();
}

void ADXL345::setRange (RANGE_SETTING _range)
{
  // Read current data format reg
  uint8_t val = readReg (DATA_FORMAT_REG);
  
  // Clear range setting
  val = val & 0xFC;
  
  // Set new range setting
  val = val | ((uint8_t) _range);
  
  // Write new data format reg value
  writeReg (DATA_FORMAT_REG, val);
  
  // Update member variable
  m_rangeSetting = _range;
  
  // Update the resolution
  updateResolution ();
}

void ADXL345::setFullRes (bool _fullRes)
{
  // Read the current data format reg
  uint8_t val = readReg (DATA_FORMAT_REG);
 
  // Change full res bit based on parameter
  if (_fullRes)
    val = val | FULL_RES_ENABLE;
  else
    val = val & (~FULL_RES_ENABLE);
    
  // Write new data format reg value
  writeReg (DATA_FORMAT_REG, val);
  
  // Update member variable
  m_fullResSetting = _fullRes;
  
  // Update the resolution
  updateResolution ();
}

void ADXL345::dataReady (bool &_drdy, bool &_ovrn)
{
  _drdy = false;
  _ovrn = false;
  
  uint8_t val;
  val = readReg (INT_SOURCE_REG);
  if (val & DATA_RDY_MASK)
    _drdy = true;
  if (val & OVERRUN_MASK)
    _ovrn = true;
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

void ADXL345::updateResolution ()
{
  // Calculate real range setting in gs
  double realRange = pow (2, m_rangeSetting + 1);
  
  // Update resolution based on range and full res settings
  if (m_fullResSetting)
    // Bit resolution changed according to range to keep resolution at FULL_RES_RESOLUTION
    m_resolution = FULL_RES_RESOLUTION;
  else
    // Always 10 bits (512 is 2 ^ 9)
    m_resolution = (realRange / 512) * 1000.0;
}
