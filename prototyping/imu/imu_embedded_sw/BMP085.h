/*
 * BMP085.h - Library for interfacing with BMP085 pressure and temperature sensor
 * Created by Michael P. McKeown, January 25, 2014.
 * Currently just for personal use.
 */
#ifndef BMP085_H
#define BMP085_H

#include "Arduino.h"
#include "Wire.h"

class BMP085
{
 public:
  // Oversampling setting enum
  typedef enum OSSR_SETTING_ENUM
  {
    OSSR_LOW_POWER = 0,
    OSSR_STANDARD,
    OSSR_HIGH_RES,
    OSSR_ULTRA_HIGH_RES,
    OSSR_NUM
  } OSSR_SETTING;
  
  // Callback typdefs
  typedef void (*TemperatureCallback)(int16_t _rawTemp, double _tempC, double _tempF);
  typedef void (*PressureCallback) (int32_t _rawPressure, double _pressurehPa, double _altitude);
  
  // ISRs
  typedef void (*ISRFunc) (); // should just call BMP085::eocISR
  
  BMP085 ();
  ~BMP085 ();
  
  // Register callbacks
  void registerTemperatureCallback (TemperatureCallback _cb);
  void registerPressureCallback (PressureCallback _cb);
  
  // Initialize
  void init ();
  
  // Initialize for asynchronous reading style
  void initAsync (int _eocPin, ISRFunc _eocIsr);
  // ISR functions
  void eocISR ();
  // Set and get functions for OSSR setting for async mode
  OSSR_SETTING getAsyncOSSR () {return m_ossrAsync;}
  void setAsyncOSSR (OSSR_SETTING _ossr) {m_ossrAsync = _ossr;} 
  
  // Synchronous poll reads
  int16_t readRawTempSync ();
  int32_t readRawPressureSync (OSSR_SETTING _ossr);
 private:
  // Device parameters
  static const uint8_t ADDRESS        = 0x77;
  static const uint8_t REG_WIDTH      = 1;
   
  // Device registers
  static const uint8_t AC1_MSB_REG    = 0xAA;
  static const uint8_t AC1_LSB_REG    = 0xAB;
  static const uint8_t AC2_MSB_REG    = 0xAC;
  static const uint8_t AC2_LSB_REG    = 0xAD;
  static const uint8_t AC3_MSB_REG    = 0xAE;
  static const uint8_t AC3_LSB_REG    = 0xAF;
  static const uint8_t AC4_MSB_REG    = 0xB0;
  static const uint8_t AC4_LSB_REG    = 0xB1;
  static const uint8_t AC5_MSB_REG    = 0xB2;
  static const uint8_t AC5_LSB_REG    = 0xB3;
  static const uint8_t AC6_MSB_REG    = 0xB4;
  static const uint8_t AC6_LSB_REG    = 0xB5;
  static const uint8_t B1_MSB_REG     = 0xB6;
  static const uint8_t B1_LSB_REG     = 0xB7;
  static const uint8_t B2_MSB_REG     = 0xB8;
  static const uint8_t B2_LSB_REG     = 0xB9;
  static const uint8_t MB_MSB_REG     = 0xBA;
  static const uint8_t MB_LSB_REG     = 0xBB;
  static const uint8_t MC_MSB_REG     = 0xBC;
  static const uint8_t MC_LSB_REG     = 0xBD;
  static const uint8_t MD_MSB_REG     = 0xBE;
  static const uint8_t MD_LSB_REG     = 0xBF;
  
  static const uint8_t CTRL_REG       = 0xF4;
  static const uint8_t TEMPERATURE    = 0x2E;
  static const uint8_t PRESSURE_OSRS0 = 0x34;
  static const uint8_t PRESSURE_OSRS1 = 0x74;
  static const uint8_t PRESSURE_OSRS2 = 0xB4;
  static const uint8_t PRESSURE_OSRS3 = 0xF4;
  
  static const uint8_t VALUE_MSB_REG  = 0xF6;
  static const uint8_t VALUE_LSB_REG  = 0xF7;
  static const uint8_t VALUE_XLSB_REG = 0xF8;
 
  // Random constants for calculations (from datasheet)
  static const int32_t TWO_EXP_TWO       = 4;
  static const int32_t TWO_EXP_FOUR      = 16;
  static const int32_t TWO_EXP_EIGHT     = 256;
  static const int32_t TWO_EXP_ELEVEN    = 2048;
  static const int32_t TWO_EXP_TWELVE    = 4096;
  static const int32_t TWO_EXP_THIRTEEN  = 8192;
  static const int32_t TWO_EXP_FIFTEEN   = 32768;
  static const int32_t TWO_EXP_SIXTEEN   = 65536;
 
  // Pressure at sea level
  static const double PRESSURE_SEA_LEVEL_HPA;
 
  // Array to convert oversampling setting to conversion time
  static const double  OSSR_CONVERSION_TIME[OSSR_NUM];
  
  typedef enum ASYNC_STATE_ENUM
  {
    WAIT_TEMP_CONVERSION = 0,
    WAIT_PRESSURE_CONVERSION,
    ASYNC_STATE_NUM
  } ASYNC_STATE;
  
  // Whether device parameters are initialized
  bool                 m_initialized;
  
  // Device parameters read from EEPROM
  int16_t              m_AC1;
  int16_t              m_AC2;
  int16_t              m_AC3;
  uint16_t             m_AC4;
  uint16_t             m_AC5;
  uint16_t             m_AC6;
  int16_t              m_B1;
  int16_t              m_B2;
  int16_t              m_MB;
  int16_t              m_MC;
  int16_t              m_MD;
  
  // State for asynchronous state machine
  ASYNC_STATE          m_state;
  
  // Whether we are in async mode
  bool                 m_async;
  
  // OSSR setting for async
  OSSR_SETTING         m_ossrAsync;
  
  // Saved temp value across interrupts for async
  int16_t              m_rawTempAsync;
 
  // Calbacks for asynchronous operation
  TemperatureCallback  m_tempCB;
  PressureCallback     m_pressureCB;
  
  // Private helper functions
  uint8_t readReg (const uint8_t _reg);
  void writeReg (const uint8_t _reg, const uint8_t _val);
};

#endif
