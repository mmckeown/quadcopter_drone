/*
 * BMP085.cpp - Library for interfacing with BMP085 pressure and temperature sensor
 * Created by Michael P. McKeown, January 25, 2014.
 * Currently just for personal use.
 */
 
#include "BMP085.h"

const double BMP085::OSSR_CONVERSION_TIME[OSSR_NUM] = {4.5, 7.5, 13.5, 25.5};
const double BMP085::PRESSURE_SEA_LEVEL_HPA = 1013.25;

BMP085::BMP085 ()
  : m_initialized (false),
    m_AC1 (0),
    m_AC2 (0),
    m_AC3 (0),
    m_AC4 (0),
    m_AC5 (0),
    m_AC6 (0),
    m_B1 (0),
    m_B2 (0),
    m_MB (0),
    m_MC (0),
    m_MD (0),
    m_state (WAIT_TEMP_CONVERSION),
    m_async (false),
    m_ossrAsync (OSSR_STANDARD),
    m_rawTempAsync (0),
    m_tempCB (NULL),
    m_pressureCB (NULL)
{
}

BMP085::~BMP085 ()
{
}

void BMP085::registerTemperatureCallback (TemperatureCallback _cb)
{
  m_tempCB = _cb;
}

void BMP085::registerPressureCallback (PressureCallback _cb)
{
  m_pressureCB = _cb;
}

void BMP085::init ()
{
  if (m_initialized)
    return;
  
  // Read device params from EEPROM
  m_AC1 = ((readReg (AC1_MSB_REG) << 8) | readReg (AC1_LSB_REG));
  m_AC2 = ((readReg (AC2_MSB_REG) << 8) | readReg (AC2_LSB_REG));
  m_AC3 = ((readReg (AC3_MSB_REG) << 8) | readReg (AC3_LSB_REG));
  m_AC4 = ((readReg (AC4_MSB_REG) << 8) | readReg (AC4_LSB_REG));
  m_AC5 = ((readReg (AC5_MSB_REG) << 8) | readReg (AC5_LSB_REG));
  m_AC6 = ((readReg (AC6_MSB_REG) << 8) | readReg (AC6_LSB_REG));
  m_B1 = ((readReg (B1_MSB_REG) << 8) | readReg (B1_LSB_REG));
  m_B2 = ((readReg (B2_MSB_REG) << 8) | readReg (B2_LSB_REG));
  m_B1 = ((readReg (B1_MSB_REG) << 8) | readReg (B1_LSB_REG));
  m_MB = ((readReg (MB_MSB_REG) << 8) | readReg (MB_LSB_REG));
  m_MC = ((readReg (MC_MSB_REG) << 8) | readReg (MC_LSB_REG));
  m_MD = ((readReg (MD_MSB_REG) << 8) | readReg (MD_LSB_REG));
  
  m_initialized = true;
}

// _eocISR should just call BMP085::eocISR
void BMP085::initAsync (int _eocPin, ISRFunc _eocIsr)
{
  m_async = true;
  
  // Perform regular initialization
  init ();
  
  // Configure interrupt
  pinMode (_eocPin, INPUT);
  attachInterrupt (_eocPin, _eocIsr, RISING);
  
  // Enable interrupts
  interrupts ();
  
  // Set the initial state
  m_state = WAIT_TEMP_CONVERSION;
  
  // Initialize temperature reading
  writeReg (CTRL_REG, TEMPERATURE);
}

int16_t BMP085::readRawTempSync ()
{
  if (m_async)
    return 0;
  
  writeReg (CTRL_REG, TEMPERATURE);
  
  delay (4.5);
  
  return ((readReg (VALUE_MSB_REG) << 8) | readReg (VALUE_LSB_REG));
}

int32_t BMP085::readRawPressureSync (OSSR_SETTING _ossr)
{ 
  if (m_async)
    return 0;
  
  if (_ossr >= OSSR_NUM)
    return 0;
  
  writeReg (CTRL_REG, PRESSURE_OSRS0 | (_ossr << 6));
  
  delay (OSSR_CONVERSION_TIME[_ossr]);
  
  return (((readReg (VALUE_MSB_REG) << 16) | (readReg (VALUE_LSB_REG) << 8) | readReg (VALUE_XLSB_REG)) >> (8 - _ossr));
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

void BMP085::eocISR ()
{
  switch (m_state)
  {
    case WAIT_TEMP_CONVERSION:
    {
      // Read temperature
      m_rawTempAsync = ((readReg (VALUE_MSB_REG) << 8) | readReg (VALUE_LSB_REG));
      
      // Start a pressure reading
      writeReg (CTRL_REG, PRESSURE_OSRS0 | (m_ossrAsync << 6));
      
      // Transition to waiting for pressure conversion state
      m_state = WAIT_PRESSURE_CONVERSION;
      break;
    }
    case WAIT_PRESSURE_CONVERSION:
    {
      // Read pressure
      int32_t pressure = (((readReg (VALUE_MSB_REG) << 16) | (readReg (VALUE_LSB_REG) << 8) | readReg (VALUE_XLSB_REG)) >> (8 - m_ossrAsync));
      
      // Calculate true temperature
      int32_t X1 = (m_rawTempAsync - m_AC6) * m_AC5 / TWO_EXP_FIFTEEN;
      int32_t X2 = m_MC * TWO_EXP_ELEVEN / (X1 + m_MD);
      int32_t B5 = X1 + X2;
      int32_t T = (B5 + 8) / TWO_EXP_FOUR;
      double tempC = T * 0.1;
      double tempF = (tempC * 9 / 5) + 32;
      
      int32_t B6 = B5 - 4000;
      X1 = (m_B2 * (B6 * B6 / TWO_EXP_TWELVE)) / TWO_EXP_ELEVEN;
      X2 = m_AC2 * B6 / TWO_EXP_ELEVEN;
      int32_t X3 = X1 + X2;
      int32_t B3 = ((m_AC1 * 4 + X3) << m_ossrAsync + 2) / 4;
      X1 = m_AC3 * B6 / TWO_EXP_THIRTEEN;
      X2 = (m_B1 * (B6 * B6 / TWO_EXP_TWELVE)) / TWO_EXP_SIXTEEN;
      X3 = ((X1 + X2) + 2) / TWO_EXP_TWO;
      uint32_t B4 = m_AC4 * (uint32_t)(X3 + 32768) / TWO_EXP_FIFTEEN;
      uint32_t B7 = ((uint32_t)pressure - B3) * (50000 >> m_ossrAsync);
      int32_t p;
      if (B7 < 0x80000000)
        p = (B7 * 2) / B4;
      else
        p = (B7 / B4) * 2;
      X1 = (p / TWO_EXP_EIGHT) * (p / TWO_EXP_EIGHT);
      X1 = (X1 * 3038) / TWO_EXP_SIXTEEN;
      X2 = (-7357 * p) / TWO_EXP_SIXTEEN;
      p = p + (X1 + X2 + 3791) / TWO_EXP_FOUR;
      
      double pressurehPa = ((double) p) / 100.0;
      
      double altitudeM = 44330.0 * (1.0 - pow (pressurehPa / PRESSURE_SEA_LEVEL_HPA, 1 / 5.255)); 
      
      // Make callbacks
      if (m_tempCB)
        m_tempCB (m_rawTempAsync, tempC, tempF);
      if (m_pressureCB)
        m_pressureCB (pressure, pressurehPa, altitudeM);
      
      // start another temperature reading
      writeReg (CTRL_REG, TEMPERATURE);
      
      // Transition back to waiting for temperature conversion
      m_state = WAIT_TEMP_CONVERSION;
      break;
    }
  }
}
