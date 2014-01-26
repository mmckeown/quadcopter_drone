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
const int EOC_PIN = 14;
BMP085     g_barTemp;
int16_t    g_bmp085RawTemp = 0;
double     g_bmp085TempC = 0.0;
double     g_bmp085TempF = 0.0;
int32_t    g_bmp085RawPressure = 0.0;
double     g_bmp085PressurehPa = 0.0;
double     g_bmp085AltitudeM = 0.0;

// ISRs
void bmp085EOCISR ()
{
  g_barTemp.eocISR ();
}

// Callbacks
void bmp085TempCallback (int16_t _rawTemp, double _tempC, double _tempF)
{
  g_bmp085RawTemp = _rawTemp;
  g_bmp085TempC = _tempC;
  g_bmp085TempF = _tempF;
}
void bmp085PressureCallback (int32_t _rawPressure, double _pressurehPa, double _altitudeM)
{
  g_bmp085RawPressure = _rawPressure;
  g_bmp085PressurehPa = _pressurehPa;
  g_bmp085AltitudeM = _altitudeM;
}

// Helper functions
void printDouble( double val, unsigned int precision){
// prints val with number of decimal places determine by precision
// NOTE: precision is 1 followed by the number of zeros for the desired number of decimial places
// example: printDouble( 3.1415, 100); // prints 3.14 (two decimal places)

    Serial.print (int(val));  //prints the int part
    Serial.print("."); // print the decimal point
    unsigned int frac;
    if(val >= 0)
        frac = (val - int(val)) * precision;
    else
        frac = (int(val)- val ) * precision;
    Serial.println(frac,DEC) ;
} 

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
   
   // Initalize barTemp for async mode
   g_barTemp.initAsync (EOC_PIN, bmp085EOCISR); 
   g_barTemp.registerTemperatureCallback (bmp085TempCallback);
   g_barTemp.registerPressureCallback (bmp085PressureCallback);
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
  
  // Print barometer and thermometer data
  Serial.println ("Barometer and Thermometer:");
  Serial.print ("RawTemp=");
  Serial.println (g_bmp085RawTemp, DEC);
  Serial.print ("TempC=");
  printDouble (g_bmp085TempC, 10);
  Serial.print ("TempF=");
  printDouble (g_bmp085TempF, 10);
  Serial.print ("RawPressure=");
  Serial.println (g_bmp085RawPressure, DEC);
  Serial.print ("PressurehPa=");
  printDouble (g_bmp085PressurehPa, 100);
  Serial.print ("AltitudeM=");
  printDouble (g_bmp085AltitudeM, 100);
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
  Serial.println ("");*/
  
  delay (10);
}
