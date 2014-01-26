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
  
  // Oversampling setting
  static const uint8_t OSSR_LOW_POWER = 0x00;
  static const uint8_t OSSR_STANDARD  = 0x01;
  static const uint8_t OSSR_HIGH_RES  = 0x02;
  static const uint8_t OSSR_UHIGH_RES = 0x03;
  
  BMP085 ();
  ~BMP085 ();
  
  int16_t readRawTempSync ();
  int32_t readRawPressureSync (uint8_t oss);
 private:
  uint8_t readReg (const uint8_t _reg);
  void writeReg (const uint8_t _reg, const uint8_t _val);
 
};

#endif
