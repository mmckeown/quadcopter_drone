#include "Wire.h"
#include "L3G4200D.h"
#include "ADXL345.h"
#include "HMC5883L.h"
#include "BMP085.h"

// LED blinking
const int LED = 13;
int led_val = LOW;

// Gyro
L3G4200D gyro;
uint8_t tempGyro;
L3G4200D::vector16b rawGyro;

// Accelerometer
ADXL345 acc;
ADXL345::vector16b rawAcc;

// Magnometer
HMC5883L magno;
HMC5883L::vector16b rawMagno;

// Barometer and thermometer
BMP085 barTemp;
int16_t  temp;
int32_t  pressure;

//
// Main Program
//

void setup ()
{
  // Setup LED pin
  pinMode (LED, OUTPUT);
  digitalWrite (LED, HIGH);
  led_val = HIGH;
  
  // Begin com libs
  Wire.begin();
  Serial.begin(9600);
  
  // Disable gyro power down state
  gyro.writeReg (L3G4200D::CTRL_REG1, L3G4200D::DR0 | 
                                      L3G4200D::BW0 |
                                      L3G4200D::PD_DISABLE |
                                      L3G4200D::Z_ENABLE |
                                      L3G4200D::Y_ENABLE |
                                      L3G4200D::X_ENABLE);
   // Put accelerometer in measure mode
   acc.writeReg (ADXL345::POWER_CTRL_REG, ADXL345::MEASURE_ENABLE |
                                          ADXL345::WAKEUP_8HZ);
                                          
   // Configure magnometer
   magno.writeReg (HMC5883L::CONFIG_REGA, HMC5883L::SAMPLES_AVG_1 |
                                          HMC5883L::DOR_75_HZ);
   magno.writeReg (HMC5883L::MODE_REG, HMC5883L::CONTINUOUS_MODE);
}

void loop ()
{
  // Flash LED
  if (led_val == LOW)
  {
    digitalWrite (LED, HIGH);
    led_val = HIGH;
  }
  else
  {
    digitalWrite (LED, LOW);
    led_val = LOW;
  }
  
  // Update barometer/thermometer
  temp = barTemp.readRawTempSync ();
  pressure = barTemp.readRawPressureSync (BMP085::OSSR_STANDARD);
  
  // Print barometer and thermometer data
  Serial.println ("Barometer and Thermometer:");
  Serial.print ("RawTemp=");
  Serial.println (temp, DEC);
  Serial.print ("RawPressure=");
  Serial.println (pressure, DEC);
  
  uint8_t val = 0;

  // Update gyro data
  bool gyroOverrun = false;
  val = gyro.readReg (L3G4200D::STATUS_REG);
  if (val & L3G4200D::ZYXDA_MASK)
  {
    // Check for overrun
    if (val & L3G4200D::ZYXOR_MASK)
      gyroOverrun = true;
    
    // Read temp
    tempGyro = gyro.readReg (L3G4200D::OUT_TEMP_REG);
    
    // Read raw gyro data
    rawGyro = gyro.readRaw ();
  }
  
  // Print gyro data
  Serial.println ("Gyro:");
  //if (gyroOverrun)
  //  Serial.println ("Overrun!");
  Serial.print ("RawTemp=");
  Serial.println (tempGyro, DEC);
  Serial.print ("RawX=");
  Serial.println (rawGyro.x, DEC);
  Serial.print ("RawY=");
  Serial.println (rawGyro.y, DEC);
  Serial.print ("RawZ=");
  Serial.println (rawGyro.z, DEC);
  Serial.println ("");
  
  // Update accelerometer data
  bool accOverrun = false;
  val = acc.readReg (ADXL345::INT_SOURCE_REG);
  if (val & ADXL345::DATA_RDY_MASK)
  {
    // Check for overrun
    if (val & ADXL345::OVERRUN_MASK)
      accOverrun = true;
      
    // Read raw accelerometer data
    rawAcc = acc.readRaw ();
  }
  
  // Print accelerometer data
  Serial.println ("Accelerometer:");
  //if (accOverrun)
  //  Serial.println ("Overrun!");
  Serial.print ("RawX=");
  Serial.println (rawAcc.x, DEC);
  Serial.print ("RawY=");
  Serial.println (rawAcc.y, DEC);
  Serial.print ("RawZ=");
  Serial.println (rawAcc.z, DEC);
  Serial.println ("");
  
  // Update magnometer data
  //val = acc.readReg (HMC5883L::STATUS_REG);
  //if (val & HMC5883L::RDY_MASK)
  //{ 
    // Read raw magnometer data
    rawMagno = magno.readRaw ();
  //}
  
  // Print magnometer data
  Serial.println ("Magnometer:");
  Serial.print ("RawX=");
  Serial.println (rawMagno.x, DEC);
  Serial.print ("RawY=");
  Serial.println (rawMagno.y, DEC);
  Serial.print ("RawZ=");
  Serial.println (rawMagno.z, DEC);
  Serial.println ("");
}
