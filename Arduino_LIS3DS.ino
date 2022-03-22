#include "SparkFunLIS3DH.h"
#include "Wire.h"
#include "SPI.h"

LIS3DH myIMU; //Default constructor is I2C, addr 0x19.

void setup() {
  // put your setup code here, to run once:
  //sets pin 10 to led output
  pinMode(10, OUTPUT);
  //sets pin 11 to led output
  pinMode(11, OUTPUT);
  //sets pin 10 pwm signal
  analogWrite(10, 127);
  //sets pin 11 pwm signal
  analogWrite(11, 0);
  Serial.begin(9600);
  delay(1000);//relax...
  //Serial.println("Processor came out of reset.\n");

  //Call .begin() to configure the IMU
  myIMU.begin();

}

int runCount =0;
int temp = 0;
int new_temp = 0;
int actual_temp = 0;
void loop()
{
  //Get all parameters
  //Serial.print("\nAccelerometer:\n");
  
  Serial.print(runCount++);
  Serial.print(",");
  Serial.print(" X = ");
  Serial.print(myIMU.readFloatAccelX(), 4);
  Serial.print(",");
  Serial.print(" Y = ");
  Serial.print(myIMU.readFloatAccelY(), 4);
  Serial.print(",");
  Serial.print(" Z = ");
  Serial.print(myIMU.readFloatAccelZ(), 4);
  Serial.print(",");
  //read temp test
  
  temp = int((myIMU.read10bitADC3()>>6));
  new_temp = 19 - temp;
  actual_temp = new_temp + 19;
  //temp = 45- temp;
   Serial.print(" T = ");
  Serial.print(actual_temp);
  Serial.print('\n');

  //delayMicroseconds(1000);
}
