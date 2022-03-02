#include "SparkFunLIS3DH.h"
#include "Wire.h"
#include "SPI.h"

LIS3DH myIMU; //Default constructor is I2C, addr 0x19.

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000); //relax...
  Serial.println("Processor came out of reset.\n");

  //Call .begin() to configure the IMU
  myIMU.begin();

}

int runCount =0;
void loop()
{
  //Get all parameters
  //Serial.print("\nAccelerometer:\n");
 // Serial.print(" X = ");
  Serial.print(runCount++);
  Serial.print(",");
  Serial.print(myIMU.readFloatAccelX(), 4);
  Serial.print(",");
  //Serial.print(" Y = ");
  Serial.print(myIMU.readFloatAccelY(), 4);
  Serial.print(",");
  //Serial.print(" Z = ");
  Serial.print(myIMU.readFloatAccelZ(), 4);
  Serial.print('\n');

  delayMicroseconds(10);
}
