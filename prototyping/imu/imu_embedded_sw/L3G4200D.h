/*
 * L3G4200D.h - Library for interfacing with L3G4200D gyroscope
 * Created by Michael P. McKeown, January 25, 2014.
 * Currently just for personal use.
 */
#ifndef L3G4200D_H
#define L3G4200D_H

#include "Arduino.h"
#include "Wire.h"

class L3G4200D
{
 public:
  // Vector structs
  typedef struct vectord_struct
  {
      double   x; 
      double   y;
      double   z;
  } vectord;
  typedef struct vector16b_struct
  {
      int16_t x;
      int16_t y;
      int16_t z;
  } vector16b;
  
  // Callback definitions
  typedef void (*RotationalVelocityCallback) (vector16b _rawRotVel);
  typedef void (*OverrunCallback) ();
  
  
  // ISRs
  typedef void (*ISRFunc) (); // should just call L3G4200D::int2ISR
  
  L3G4200D ();
  ~L3G4200D ();
  
  // Register callbacks
  void registerRotationalVelocityCallback (RotationalVelocityCallback _cb);
  void registerOverrunCallback (OverrunCallback _cb);
  
  // Initialize
  void init ();
  
  // Asynchrnous initialization
  void initAsync (int _int2Pin, ISRFunc _int2ISR);
  
  void calibrateZeroRate ();
  
  // ISR function
  void int2ISR ();
  
  // Read gyro data
  void dataReady (bool &_drdy, bool &_ovrn);
  vector16b readRaw ();
 private:
  // Device parameters
  static const uint8_t ADDRESS        = 0x69;
  static const uint8_t REG_WIDTH      = 1;
  
  // Device registers
  static const uint8_t WHO_AM_I_REG   = 0x0F;

  static const uint8_t CTRL_REG1      = 0x20;
  static const uint8_t DR0            = 0x00;
  static const uint8_t DR1            = 0x40;
  static const uint8_t DR2            = 0x80;
  static const uint8_t DR3            = 0xC0;
  static const uint8_t BW0            = 0x00;
  static const uint8_t BW1            = 0x10;
  static const uint8_t BW2            = 0x20;
  static const uint8_t BW3            = 0x30;
  static const uint8_t PD_DISABLE     = 0x08;
  static const uint8_t Z_ENABLE       = 0x04;
  static const uint8_t Y_ENABLE       = 0x02;
  static const uint8_t X_ENABLE       = 0x01;

  static const uint8_t CTRL_REG2      = 0x21;
  static const uint8_t HPM_REFERENCE  = 0x10;
  static const uint8_t HPM_AUTORESET  = 0x30;
  
  static const uint8_t CTRL_REG3      = 0x22;
  static const uint8_t I1_INT1        = 0x80;
  static const uint8_t I1_BOOT        = 0x40;
  static const uint8_t INT_L_ACTIVE   = 0x20;
  static const uint8_t INT_OPEN_DRAIN = 0x10;
  static const uint8_t I2_DRDY        = 0x08;
  static const uint8_t I2_FIFO_WTM    = 0x04;
  static const uint8_t I2_FIFO_OR     = 0x02;
  static const uint8_t I2_FIFO_EMPTY  = 0x01;
  
  static const uint8_t CTRL_REG4      = 0x23;
  static const uint8_t BDU_ENABLE     = 0x80;
  static const uint8_t BIG_ENDIAN     = 0x40;
  static const uint8_t SCALE_500DPS   = 0x10;
  static const uint8_t SCALE_2000DPS  = 0x20;
  static const uint8_t SELF_TEST_0    = 0x02;
  static const uint8_t SELF_TEST_1    = 0x06;
  static const uint8_t SPI_3WIRE      = 0x01;
  
  static const uint8_t CTRL_REG5      = 0x24;
  static const uint8_t REBOOT_MEM     = 0x80;
  static const uint8_t FIFO_ENABLE    = 0x40;
  static const uint8_t HPF_ENABLE     = 0x10;
  static const uint8_t INT1_SEL01     = 0x04;
  static const uint8_t INT1_SEL10     = 0x08;
  static const uint8_t INT1_SEL11     = 0x0C;
  static const uint8_t OUT_SEL01      = 0x01;
  static const uint8_t OUT_SEL10      = 0x02;
  static const uint8_t OUT_SEL11      = 0x03;

  static const uint8_t REFERENCE      = 0x25;

  static const uint8_t OUT_TEMP_REG   = 0x26;

  static const uint8_t STATUS_REG     = 0x27;
  static const uint8_t ZYXOR_MASK     = 0x80;
  static const uint8_t ZOR_MASK       = 0x40;
  static const uint8_t YOR_MASK       = 0x20;
  static const uint8_t XOR_MASK       = 0x10;
  static const uint8_t ZYXDA_MASK     = 0x08;
  static const uint8_t ZDA_MASK       = 0x04;
  static const uint8_t YDA_MASK       = 0x02;
  static const uint8_t XDA_MASK       = 0x01;

  static const uint8_t OUT_X_L_REG    = 0x28;
  static const uint8_t OUT_X_H_REG    = 0x29;

  static const uint8_t OUT_Y_L_REG    = 0x2A;
  static const uint8_t OUT_Y_H_REG    = 0x2B;

  static const uint8_t OUT_Z_L_REG    = 0x2C;
  static const uint8_t OUT_Z_H_REG    = 0x2D;

  static const uint8_t FIFO_CTRL_REG  = 0x2E;
  static const uint8_t FIFO_MODE      = 0x20;
  static const uint8_t STREAM_MODE    = 0x40;
  static const uint8_t STRM_FIFO_MODE = 0x60;
  static const uint8_t BYP_STRM_MODE  = 0x80; 
  
  static const uint8_t FIFO_SRC_REG   = 0x2F;
  static const uint8_t WTM_MASK       = 0x80;
  static const uint8_t OVRN_MASK      = 0x40;
  static const uint8_t EMPTY_MASK     = 0x20;
  static const uint8_t FSS_MASK       = 0x1F;
  
  static const uint8_t INT1_CFG       = 0x30;
  static const uint8_t OR_INTS        = 0x00;
  static const uint8_t AND_INTS       = 0x80;
  static const uint8_t INT_REQ_LATCH  = 0x40;
  static const uint8_t Z_H_INT_ENABLE = 0x20;
  static const uint8_t Z_L_INT_ENABLE = 0x10;
  static const uint8_t Y_H_INT_ENABLE = 0x08;
  static const uint8_t Y_L_INT_ENABLE = 0x04;
  static const uint8_t X_H_INT_ENABLE = 0x02;
  static const uint8_t X_L_INT_ENABLE = 0x01;
  
  static const uint8_t INT1_SRC       = 0x31;
  static const uint8_t INT_ACT_MASK   = 0x40;
  static const uint8_t INT_Z_H_MASK   = 0x20;
  static const uint8_t INT_Z_L_MASK   = 0x10;
  static const uint8_t INT_Y_H_MASK   = 0x08;
  static const uint8_t INT_Y_L_MASK   = 0x04;
  static const uint8_t INT_X_H_MASK   = 0x02;
  static const uint8_t INT_X_L_MASK   = 0x01;
  
  static const uint8_t INT1_THS_XH    = 0x32;
  static const uint8_t INT1_THS_XL    = 0x33;
  static const uint8_t INT1_THS_YH    = 0x34;
  static const uint8_t INT1_THS_YL    = 0x35;
  static const uint8_t INT1_THS_ZH    = 0x36;
  static const uint8_t INT1_THS_ZL    = 0x37;
  
  static const uint8_t INT1_DURATION  = 0x38;
  static const uint8_t WAIT_ENABLE    = 0x80;
  
  // Zero rate calibration samples
  static const int32_t ZERO_RATE_SAMPLES = 100;
  
  // Initialized
  bool                          m_initialized;
  
  // Timer for async operation
  IntervalTimer                 m_timer;
  
  // Zero rate offset calibration
  bool                          m_zeroRateInit;
  vector16b                     m_zeroRate;
  
  // Calbacks for asynchronous operation
  RotationalVelocityCallback    m_rotVelCB;
  OverrunCallback               m_ovrnCB;
  
  // Read and write regs
  uint8_t readReg (const uint8_t _reg);
  void writeReg (const uint8_t _reg, const uint8_t _val);
};

#endif
