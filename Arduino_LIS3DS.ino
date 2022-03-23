#include "SparkFunLIS3DH.h"
#include "Wire.h"
#include "SPI.h"

LIS3DH myIMU; //Default constructor is I2C, addr 0x19.

void setup() {
  // put your setup code here, to run once:
  pinMode(10, OUTPUT);
  analogWrite(10, 127);
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
  // Get all parameters
  // 
  unsigned int mytime[100];
  float accx[100];
  float accy[100];
  float accz[100];
  int myarray[100];
  for (int i=0; i<100; i++) {
    mytime[i]=millis();
//    accx[i]=myIMU.readRawAccelX();
//    accy[i]=myIMU.readRawAccelY();
//    accz[i]=myIMU.readRawAccelZ();
    accx[i]=myIMU.readFloatAccelX();
    accy[i]=myIMU.readFloatAccelY();
    accz[i]=myIMU.readFloatAccelZ();
  }
  for (int i=0; i<100; i++) {

    //time
    Serial.print(mytime[i]);
    Serial.print(" ");
//    Accelerometer values
    Serial.print(accx[i],4); // x
    Serial.print(" ");
    Serial.print(accy[i],4); // y
    Serial.print(" ");
    Serial.print(accz[i],4); // z
    Serial.println();

  }
// delay(1000);
}
