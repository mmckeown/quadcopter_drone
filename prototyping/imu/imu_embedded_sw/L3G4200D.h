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
    static const uint8_t ADDRESS        = 0x69;
    static const uint8_t REG_WIDTH      = 1;
    
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
    static const uint8_t PD_ENABLE      = 0x00;
    static const uint8_t PD_DISABLE     = 0x08;
    static const uint8_t Z_DISABLE      = 0x00;
    static const uint8_t Z_ENABLE       = 0x04;
    static const uint8_t Y_DISABLE      = 0x00;
    static const uint8_t Y_ENABLE       = 0x02;
    static const uint8_t X_DISABLE      = 0x00;
    static const uint8_t X_ENABLE       = 0x01;

    static const uint8_t CTRL_REG2      = 0x21;
    static const uint8_t CTRL_REG3      = 0x22;
    static const uint8_t CTRL_REG4      = 0x23;
    static const uint8_t CTRL_REG5      = 0x24;

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
    static const uint8_t FIFO_SRC_REG   = 0x2F;

    static const uint8_t INT1_CFG       = 0x30;
    static const uint8_t INT1_SRC       = 0x31;
    static const uint8_t INT1_THS_XH    = 0x32;
    static const uint8_t INT1_THS_XL    = 0x33;
    static const uint8_t INT1_THS_YH    = 0x34;
    static const uint8_t INT1_THS_YL    = 0x35;
    static const uint8_t INT1_THS_ZH    = 0x36;
    static const uint8_t INT1_THS_ZL    = 0x37;
    static const uint8_t INT1_DURATION  = 0x38;

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
    
    L3G4200D ();
    ~L3G4200D ();
    
    uint8_t readReg (const uint8_t _reg);
    void writeReg (const uint8_t _reg, const uint8_t _val);
    
    vector16b readRaw ();
private:
    float       m_sensitivity;
    vectorf     m_angle;
};

#endif
