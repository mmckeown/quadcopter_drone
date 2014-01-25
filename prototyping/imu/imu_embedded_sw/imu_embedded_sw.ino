#include "Wire.h"
#include "L3G4200D.h"

// LED blinking
const int LED = 13;
int led_val = LOW;

// Gyro
L3G4200D gyro;
double angleX = 0;
double angleY = 0;
double angleZ = 0;

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
    Serial.print(frac,DEC) ;
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
  
  
  
  // Disable power down state
  gyro.writeReg (L3G4200D::CTRL_REG1, L3G4200D::DR0 | 
                                      L3G4200D::BW0 |
                                      L3G4200D::PD_DISABLE |
                                      L3G4200D::Z_ENABLE |
                                      L3G4200D::Y_ENABLE |
                                      L3G4200D::X_ENABLE);
}

void loop ()
{
  uint8_t val = 0;
  
  // Read status reg
  val = gyro.readReg (L3G4200D::STATUS_REG);
  if (val & L3G4200D::ZYXDA_MASK)
  {
    if (val & L3G4200D::ZYXOR_MASK)
      Serial.println  ("Too Slow!");
    
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
    
    val = gyro.readReg (L3G4200D::OUT_TEMP_REG);
    Serial.print ("OUT_TEMP=");
    Serial.println (val, DEC);
    
    
    L3G4200D::vector16b raw;
    raw = gyro.readRaw ();
    Serial.print ("RawX=");
    Serial.println (raw.x, DEC);
    Serial.print ("RawY=");
    Serial.println (raw.y, DEC);
    Serial.print ("RawZ=");
    Serial.println (raw.z, DEC);
    
    Serial.println ("");
    
    double vec_x, vec_y, vec_z;
    vec_x = ((double) raw.x) * 0.00875;
    vec_y = ((double) raw.y) * 0.00875;
    vec_z = ((double) raw.z) * 0.00875;
    
    Serial.print ("VecX=");
    printDouble (vec_x, 100);
    Serial.println (" dps");
    Serial.print ("VecY=");
    printDouble (vec_y, 100);
    Serial.println (" dps");
    Serial.print ("VecZ=");
    printDouble (vec_z, 100);
    Serial.println (" dps");
    Serial.println ("");

    angleX += vec_x * 0.01;
    angleY += vec_y * 0.01;
    angleZ += vec_z * 0.01;
    
    Serial.print ("AngleX=");
    printDouble (angleX, 100);
    Serial.println (" degrees");
    Serial.print ("AngleY=");
    printDouble (angleY, 100);
    Serial.println (" degrees");
    Serial.print ("AngleZ=");
    printDouble (angleZ, 100);
    Serial.println (" degrees");
  }
  
  delay (1);
}
