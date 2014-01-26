/*
 * ADXL345.h - Library for interfacing with ADXL345 accelerometer
 * Created by Michael P. McKeown, January 25, 2014.
 * Currently just for personal use.
 */
#ifndef ADXL345_H
#define ADXL345_H

#include "Arduino.h"
#include "Wire.h"

class ADXL345
{
 public:
  // Device parameters
  static const uint8_t ADDRESS             = 0x53;
  static const uint8_t REG_WIDTH           = 1;
  
  static const uint8_t DEV_ID_REG          = 0x00;
  
  // Device registers
  static const uint8_t TAP_THRESH_REG      = 0x1D;
  
  static const uint8_t X_OFFSET_REG        = 0x1E;
  static const uint8_t Y_OFFSET_REG        = 0x1F;
  static const uint8_t Z_OFFSET_REG        = 0x20;
  
  static const uint8_t TAP_DURATION_REG    = 0x21;
  static const uint8_t TAP_LATENCY_REG     = 0x22;
  static const uint8_t TAP_WINDOW_REG      = 0x23;
  
  static const uint8_t ACT_THRESH_REG      = 0x24;
  static const uint8_t INACT_THRESH_REG    = 0x25;
  static const uint8_t INACT_TIME_REG      = 0x26;
  
  static const uint8_t ACT_INACT_CTRL      = 0x27;
  static const uint8_t ACT_AC_COUPLE       = 0x80;
  static const uint8_t ACT_X_ENABLE        = 0x40;
  static const uint8_t ACT_Y_ENABLE        = 0x20;
  static const uint8_t ACT_Z_ENABLE        = 0x10;
  static const uint8_t INACT_AC_COUPLE     = 0x08;
  static const uint8_t INACT_X_ENABLE      = 0x04;
  static const uint8_t INACT_Y_ENABLE      = 0x02;
  static const uint8_t INACT_Z_ENABLE      = 0x01;
  
  static const uint8_t FF_THRESH_REG       = 0x28;
  static const uint8_t FF_TIME_REG         = 0x29;
  
  static const uint8_t TAP_AXES_REG        = 0x2A;
  static const uint8_t SUPPRESS            = 0x08;
  static const uint8_t TAP_X_ENABLE        = 0x04;
  static const uint8_t TAP_Y_ENABLE        = 0x02;
  static const uint8_t TAP_Z_ENABLE        = 0x01;
  
  static const uint8_t ACT_TAP_STATUS_REG  = 0x2B;
  static const uint8_t ACT_X_SOURCE_MASK   = 0x40;
  static const uint8_t ACT_Y_SOURCE_MASK   = 0x20;
  static const uint8_t ACT_Z_SOURCE_MASK   = 0x10;
  static const uint8_t ASLEEP_MASK         = 0x08;
  static const uint8_t TAP_X_SOURCE_MASK   = 0x04;
  static const uint8_t TAP_Y_SOURCE_MASK   = 0x02;
  static const uint8_t TAP_Z_SOURCE_MASK   = 0x01;
  
  static const uint8_t BW_RATE_PWR_REG     = 0x2C;
  static const uint8_t LP_OPERATION_ENABLE = 0x10;
  
  static const uint8_t POWER_CTRL_REG      = 0x2D;
  static const uint8_t LINK_ENABLE         = 0x20;
  static const uint8_t AUTO_SLEEP_ENABLE   = 0x10;
  static const uint8_t MEASURE_ENABLE      = 0x08;
  static const uint8_t SLEEP_ENABLE        = 0x04;
  static const uint8_t WAKEUP_8HZ          = 0x00;
  static const uint8_t WAKEUP_4HZ          = 0x01;
  static const uint8_t WAKEUP_2HZ          = 0x02;
  static const uint8_t WAKEUP_1HZ          = 0x03;
  
  static const uint8_t INT_ENABLE_REG      = 0x2E;
  static const uint8_t DATA_RDY_ENABLE     = 0x80;
  static const uint8_t SINGLE_TAP_ENABLE   = 0x40;
  static const uint8_t DOUBLE_TAP_ENABLE   = 0x20;
  static const uint8_t ACTIVITY_ENABLE     = 0x10;
  static const uint8_t INACTIVITY_ENABLE   = 0x08;
  static const uint8_t FREE_FALL_ENABLE    = 0x04;
  static const uint8_t WATERMARK_ENABLE    = 0x02;
  static const uint8_t OVERRUN_ENABLE      = 0x01;
  
  static const uint8_t INT_MAP_REG         = 0x2F;
  static const uint8_t DATA_RDY_INT2       = 0x80;
  static const uint8_t SINGLE_TAP_INT2     = 0x40;
  static const uint8_t DOUBLE_TAP_INT2     = 0x20;
  static const uint8_t ACTIVITY_INT2       = 0x10;
  static const uint8_t INACTIVITY_INT2     = 0x08;
  static const uint8_t FREE_FALL_INT2      = 0x04;
  static const uint8_t WATERMARK_INT2      = 0x02;
  static const uint8_t OVERRUN_INT2        = 0x01;
  
  static const uint8_t INT_SOURCE_REG      = 0x30;
  static const uint8_t DATA_RDY_MASK       = 0x80;
  static const uint8_t SINGLE_TAP_MASK     = 0x40;
  static const uint8_t DOUBLE_TAP_MASK     = 0x20;
  static const uint8_t ACTIVITY_MASK       = 0x10;
  static const uint8_t INACTIIVITY_MASK    = 0x08;
  static const uint8_t FREE_FALL_MASK      = 0x04;
  static const uint8_t WATERMARK_MASK      = 0x02;
  static const uint8_t OVERRUN_MASK        = 0x01;
 
  static const uint8_t DATA_FORMAT_REG     = 0x31; 
  static const uint8_t SELF_TEST_ENABLE    = 0x80;
  static const uint8_t SPI_3WIRE_ENABLE    = 0x40;
  static const uint8_t INT_ACTIVE_LOW      = 0x20;
  static const uint8_t FULL_RES_DISABLE    = 0x08;
  static const uint8_t LEFT_JUSTIFY_ENABLE = 0x04;
  static const uint8_t RANGE_4g            = 0x01;
  static const uint8_t RANGE_8g            = 0x02;
  static const uint8_t RANGE_16g           = 0x03;

  static const uint8_t DATAX0_REG          = 0x32;
  static const uint8_t DATAX1_REG          = 0x33;
  static const uint8_t DATAY0_REG          = 0x34;
  static const uint8_t DATAY1_REG          = 0x35;
  static const uint8_t DATAZ0_REG          = 0x36;
  static const uint8_t DATAZ1_REG          = 0x37;
  
  static const uint8_t FIFO_CTRL_REG       = 0x38;
  static const uint8_t FIFO_MODE           = 0x40;
  static const uint8_t STREAM_MODE         = 0x80;
  static const uint8_t TRIGGER_MODE        = 0xC0;
  static const uint8_t TRIGGER_INT2        = 0x20;
  
  static const uint8_t FIFO_STATUS_REG     = 0x39; 
  static const uint8_t FIFO_TRIG_MASK      = 0x80;
  static const uint8_t FIFO_ENTRIES_MASK   = 0x3F;
  
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

  ADXL345 ();
  ~ADXL345 ();
  
  // Read and write regs
  uint8_t readReg (const uint8_t _reg);
  void writeReg (const uint8_t _reg, const uint8_t _val);
  
  // Read accelerometer data
  vector16b readRaw ();
 private:
};

#endif