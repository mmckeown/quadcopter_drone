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
const int INT1_PIN = 11;
ADXL345            g_acc;
ADXL345::vector16b g_rawAcc;
ADXL345::vectord   g_accmG;
double             g_accPitch = 0.0;
double             g_accRoll = 0.0;

// Magnometer
HMC5883L magno;
HMC5883L::vector16b rawMagno;

// Barometer and thermometer
const int EOC_PIN = 14;
BMP085     g_barTemp;
int16_t    g_bmp085RawTemp = 0;
double     g_bmp085TempC = 0.0;
double     g_bmp085TempF = 0.0;
int32_t    g_bmp085RawPressure = 0.0;
double     g_bmp085PressurehPa = 0.0;
double     g_bmp085AltitudeM = 0.0;
double     g_bmp085AltitudeF = 0.0;
double     g_bmp085VerticalSpeedMpS = 0.0;
double     g_bmp085VerticalSpeedFpS = 0.0;

// ISRs
void adxl345Int1ISR ()
{
  noInterrupts ();
  g_acc.int1ISR ();
  interrupts ();
}

void bmp085EOCISR ()
{
  noInterrupts ();
  g_barTemp.eocISR ();
  interrupts ();
}

// Callbacks
void adxl345AccelerationCallback (ADXL345::vector16b _rawAcc, ADXL345::vectord _accmG)
{
  g_rawAcc = _rawAcc;
  g_accmG = _accmG;
}

void adxl345PitchRollCallback (double _pitch, double _roll)
{
  g_accPitch = _pitch;
  g_accRoll = _roll;
}

void adxl345OverrunCallback ()
{
  Serial.println ("ADXL345 Overrun!");
}

void bmp085TempCallback (int16_t _rawTemp, double _tempC, double _tempF)
{
  g_bmp085RawTemp = _rawTemp;
  g_bmp085TempC = _tempC;
  g_bmp085TempF = _tempF;
}
void bmp085PressureCallback (int32_t _rawPressure, double _pressurehPa)
{
  g_bmp085RawPressure = _rawPressure;
  g_bmp085PressurehPa = _pressurehPa;
}

void bmp085AltitudeCallback (double _altitudeM, double _altitudeF)
{
  g_bmp085AltitudeM = _altitudeM;
  g_bmp085AltitudeF = _altitudeF;
}

void bmp085VerticalSpeedCallback (double _verticalSpeedMpS, double _verticalSpeedFpS)
{
  g_bmp085VerticalSpeedMpS = _verticalSpeedMpS;
  g_bmp085VerticalSpeedFpS = _verticalSpeedFpS;
}

//
// Main Program
//

void setup ()
{
  // Setup LED pin
  pinMode (LED, OUTPUT);
  digitalWrite (LED, LOW);
  led_val = LOW;
  
  // Begin com libs
  Wire.begin();
  Serial.begin(115200);
  
  noInterrupts ();
  
  // Disable gyro power down state
  //gyro.writeReg (L3G4200D::CTRL_REG1, L3G4200D::DR0 | 
  //                                    L3G4200D::BW0 |
  //                                    L3G4200D::PD_DISABLE |
  //                                    L3G4200D::Z_ENABLE |
  //                                    L3G4200D::Y_ENABLE |
  //                                    L3G4200D::X_ENABLE);
   
   // Initialize accelerometer for async mode
   g_acc.registerAccelerationCallback (adxl345AccelerationCallback);
   g_acc.registerPitchRollCallback (adxl345PitchRollCallback);
   g_acc.registerOverrunCallback (adxl345OverrunCallback);
   g_acc.setRange (ADXL345::RANGE_4G);
   g_acc.setFullRes (true);
   g_acc.setLPFilter (true);
   g_acc.init ();
   g_acc.calibrateOffset ();
   g_acc.initAsync (INT1_PIN, adxl345Int1ISR);  
                                          
   // Configure magnometer
   //magno.writeReg (HMC5883L::CONFIG_REGA, HMC5883L::SAMPLES_AVG_1 |
   //                                       HMC5883L::DOR_75_HZ);
   //magno.writeReg (HMC5883L::MODE_REG, HMC5883L::CONTINUOUS_MODE);
   
   // Initalize barTemp for async mode
   g_barTemp.registerTemperatureCallback (bmp085TempCallback);
   g_barTemp.registerPressureCallback (bmp085PressureCallback);
   g_barTemp.registerAltitudeCallback (bmp085AltitudeCallback);
   g_barTemp.registerVerticalSpeedCallback (bmp085VerticalSpeedCallback);
   g_barTemp.setAsyncOSSR (BMP085::OSSR_ULTRA_HIGH_RES);
   g_barTemp.setAvgFilter (true);
   g_barTemp.initAsync (EOC_PIN, bmp085EOCISR); 
   
   interrupts ();
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
  };
  
    // Print accelerometer data
  Serial.println ("Accelerometer:");
  Serial.print ("RawX=");
  Serial.println (g_rawAcc.x, DEC);
  Serial.print ("RawY=");
  Serial.println (g_rawAcc.y, DEC);
  Serial.print ("RawZ=");
  Serial.println (g_rawAcc.z, DEC);
  Serial.print ("AccXmg=");
  Serial.println (g_accmG.x, DEC);
  Serial.print ("AccYmg=");
  Serial.println (g_accmG.y, DEC);
  Serial.print ("AccZmg=");
  Serial.println (g_accmG.z, DEC);
  Serial.print ("Pitch=");
  Serial.println (g_accPitch, DEC);
  Serial.print ("Roll=");
  Serial.println (g_accRoll, DEC);
  Serial.println ("");
  
  // Print barometer and thermometer data
  Serial.println ("Barometer and Thermometer:");
  Serial.print ("RawTemp=");
  Serial.println (g_bmp085RawTemp, DEC);
  Serial.print ("TempC=");
  Serial.println (g_bmp085TempC, DEC);
  Serial.print ("TempF=");
  Serial.println (g_bmp085TempF, DEC);
  Serial.print ("RawPressure=");
  Serial.println (g_bmp085RawPressure, DEC);
  Serial.print ("PressurehPa=");
  Serial.println (g_bmp085PressurehPa, DEC);
  Serial.print ("AltitudeM=");
  Serial.println (g_bmp085AltitudeM, DEC);
  Serial.print ("AltitudeF=");
  Serial.println (g_bmp085AltitudeF, DEC);
  Serial.print ("VerticalSpeedMpS=");
  Serial.println (g_bmp085VerticalSpeedMpS, DEC);
  Serial.print ("VerticalSpeedFpS=");
  Serial.println (g_bmp085VerticalSpeedFpS, DEC);
  Serial.println ("");
  
  /*uint8_t val = 0;

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
  Serial.println ("");*/
  
  delay (10);
}
