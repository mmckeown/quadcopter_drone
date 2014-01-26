/*
 * HMC5883L.h - Library for interfacing with HMC5883L magnometer
 * Created by Michael P. McKeown, January 25, 2014.
 * Currently just for personal use.
 */
#ifndef HMC5883L_H
#define HMC5883L_H

#include "Arduino.h"
#include "Wire.h"

class HMC5883L
{
 public:
  // Device parameters
  static const uint8_t ADDRESS            = 0x1E;
  static const uint8_t REG_WIDTH          = 1;
   
  // Device registers
  static const uint8_t CONFIG_REGA        = 0x00;
  static const uint8_t SAMPLES_AVG_1      = 0x00;
  static const uint8_t SAMPLES_AVG_2      = 0x20;
  static const uint8_t SAMPLES_AVG_4      = 0x40;
  static const uint8_t SAMPLES_AVG_8      = 0x60;
  static const uint8_t DOR_0p75_HZ        = 0x00;
  static const uint8_t DOR_1p5_HZ         = 0x04;
  static const uint8_t DOR_3_HZ           = 0x08;
  static const uint8_t DOR_7p5_HZ         = 0x0C;
  static const uint8_t DOR_15_HZ          = 0x10;
  static const uint8_t DOR_30_HZ          = 0x14;
  static const uint8_t DOR_75_HZ          = 0x18;
  static const uint8_t MS_POS_BIAS        = 0x01;
  static const uint8_t MS_NEG_BIAS        = 0x02;
  
  static const uint8_t CONFIG_REGB        = 0x01;
  static const uint8_t GAIN_1370          = 0x00;
  static const uint8_t GAIN_1090          = 0x20;
  static const uint8_t GAIN_820           = 0x40;
  static const uint8_t GAIN_660           = 0x60;
  static const uint8_t GAIN_440           = 0x80;
  static const uint8_t GAIN_390           = 0xA0;
  static const uint8_t GAIN_330           = 0xC0;
  static const uint8_t GAIN_230           = 0xE0;
  
  static const uint8_t MODE_REG           = 0x02;
  static const uint8_t HS_I2C_3400_KHZ    = 0x80;
  static const uint8_t CONTINUOUS_MODE    = 0x00;
  static const uint8_t SINGLE_MEAS_MODE   = 0x01;
  static const uint8_t IDLE_MODE          = 0x02;
  
  static const uint8_t DATA_OUT_X_MSB_REG = 0x03;
  static const uint8_t DATA_OUT_X_LSB_REG = 0x04;
  static const uint8_t DATA_OUT_Z_MSB_REG = 0x05;
  static const uint8_t DATA_OUT_Z_LSB_REG = 0x06;
  static const uint8_t DATA_OUT_Y_MSB_REG = 0x07;
  static const uint8_t DATA_OUT_Y_LSB_REG = 0x08;
  
  static const uint8_t STATUS_REG         = 0x09;
  static const uint8_t LOCK_MASK          = 0x02;
  static const uint8_t RDY_MASK           = 0x01;
  
  static const uint8_t ID_REG_A           = 0x0A;
  static const uint8_t ID_REG_B           = 0x0B;
  static const uint8_t ID_REG_C           = 0x0C;
  
  // Vector structs
  typedef struct vectorf_struct
  {
      float   x; 
      float   y;
      float   z;
  } vectorf;
  typedef struct vector16b_struct
  {
      int16_t x;
      int16_t y;
      int16_t z;
  } vector16b;
 
  HMC5883L ();
  ~HMC5883L();

  // Read and write regs
  uint8_t readReg (const uint8_t _reg);
  void writeReg (const uint8_t _reg, const uint8_t _val);
  
  // Read magnometer data
  vector16b readRaw ();
 private:
};

#endif
