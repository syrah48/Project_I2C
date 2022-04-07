#include "SparkFunLIS3DH.h"
#include "Wire.h"
#include "SPI.h"
#include "arduinoFFT.h"

LIS3DH myIMU(SPI_MODE,7); //Default constructor is I2C, addr 0x19. Set to SPI, CS Pin 10

void setup() {
  // put your setup code here, to run once:
  
  // Test both LEDs
  pinMode(6, OUTPUT); //Green LED
  pinMode(5, OUTPUT); //Red LED
  digitalWrite(5, HIGH);//Green LED
  digitalWrite(6, LOW);//Red LED
  
  Serial.begin(115200); //(Max 115200 without issues)
  delay(1000); // relax...
  //Call .begin() to configure the IMU
  myIMU.begin();
}

int average = 0;
float dataAverage = 0;
float sampleFrequency = 0;
unsigned int samplecount = 0;
int temp = 0;
int temp_offset = 19; // Degrees Celsius
const int sampleSize = 128; // Power of 2 ex(32,64,128,256...) Max 128 in SRAM

unsigned long mytime[sampleSize]; //Time stamps stored here
int accy[sampleSize]; // Acceleration Y (Vertical (+ Up))

//Defining temporary variables
float y = 0;
uint8_t status_reg;
void printSamples(void){

  // Read temperature value after  (or after a set amount of time)
  temp = (myIMU.read10bitADC3()>>6) + temp_offset; // digit/Â°C //applied offset to reflect actual temperature
  Serial.print("T = ");
  Serial.print(temp);
  Serial.print(" °C");
  Serial.print('\n');

  // Export Sample over Serial line
  Serial.println("t(us) X(G)  Y(G)  Z(G)");
  for (int i=0; i<sampleSize; i++) {
    
    // Convert to float here
    // All 8-bit values are converted back to 16 bit before function call
    //x = myIMU.calcAccel(accx[i]<<8);
    y = myIMU.calcAccel(accy[i]<<8);
    //z = myIMU.calcAccel(accz[i]<<8);

    // Time Stamp
    Serial.print(mytime[i]); // t (microseconds)
    Serial.print(" ");
    // Accelerometer values (16mg/digit in Low Power Mode)
    //Serial.print(x,4); // x (Gs)
    //Serial.print(" ");
    Serial.print(y,4); // y (Gs)
    Serial.print(" ");
    //Serial.print(z,4); // z (Gs)
//    //Status Register
//    Serial.print(" ");
////    Serial.print(status_reg[i],HEX);
//    for (int b = 7; b >= 0; b--)
//    {
//      Serial.print(bitRead(status_reg[i], b));
//    }
    Serial.println();
    
  }
  
  Serial.println(); // End of Sample
 // delay(1000); // Delay between each sample
 return;
}
double out_r[128]={};   //real part of transform
double out_im[128]={};  //imaginory part of transform
int in_ps[128]={};     //input for sequencing

//arduinoFFT setup
arduinoFFT FFT = arduinoFFT(); /* Create FFT object */
/*
These values can be changed in order to evaluate the functions
*/


const double startFrequency = 2;
const double stopFrequency = 16.4;
const double step_size = 0.1;

unsigned long time;

#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

double averageAbove2k(){
  int above2kcounter = 0;
  int avg = 0;
  for(int i = 0; i < 64; i++){
    //Serial.println(out_im[i]);
    if(((i * 1.0 * sampleFrequency) / sampleSize) > 2000){
      
      avg += out_r[i];
      above2kcounter++;
    }
  }
  avg = avg/above2kcounter;
  return avg;
}

int runningAverage=0;
int runningAverageCount=0;
void loop()
{
  
  // Get all parameters from Accelerometer
  for (int i=0; i<sampleSize; i++) {

    // Only read new data
    // Loops until new data is available
    // STATUS_REG (27h) ZYXDA[3] ZDA[2] YDA[1] XDA[0] //
    // bitread: (((value) >> (bit)) & 0x01)
    for (uint8_t ZYXDA = 0x00; ZYXDA == 0; ZYXDA = (((status_reg) >> (1)) & 0x01) ){
      // Read DA register to see if there is new data available
      myIMU.readRegister(&status_reg, LIS3DH_STATUS_REG2); // readRegister(uint8_t* outputPointer, uint8_t offset)
    }
    //Get timestamp of data
    mytime[i]=micros();
    // Read the Highest Significant byte of the 16-bit Accelerometer registers (only 8-bit if set to LP mode)
   // accx[i] = myIMU.readRawAccelX()>>8; // X is read first so read this first
    accy[i] = myIMU.readRawAccelY()>>8;
   // accz[i] = myIMU.readRawAccelZ()>>8;
    
    // delayMicroseconds(100); // Allow time for Accelerometer values to update
  }


  
  //Serial.println("my frequency: ");
  sampleFrequency = 1/(0.000001*(mytime[127] - mytime[0])/sampleSize);
  //Serial.println(sampleFrequency);
  //Serial.println(sampleFrequency);
  for(int i = 0; i < sampleSize; i++){
    out_r[i] = accy[i];
    out_im[i] = 0;
  }
  
  //FFT.Windowing(out_r, sampleSize, FFT_WIN_TYP_RECTANGLE, FFT_FORWARD);	/* Weigh data */
  //Serial.println("Weighed data:");
  //PrintVector(out_r, sampleSize, SCL_TIME);
  FFT.Compute(out_r, out_im, sampleSize, FFT_FORWARD); /* Compute FFT */
  //Serial.println("Computed Real values:");
  //PrintVector(out_r, sampleSize, SCL_INDEX);
  //Serial.println("Computed Imaginary values:");
  //PrintVector(out_r, sampleSize, SCL_INDEX);
  FFT.ComplexToMagnitude(out_r, out_im, sampleSize); /* Compute magnitudes */
  // PrintVector(out_r, sampleSize, SCL_FREQUENCY);
  average = averageAbove2k();
  runningAverage = (runningAverage*runningAverageCount + average)/(runningAverageCount+1);
  runningAverageCount++;
  if(runningAverageCount > 100){
     if(  runningAverage > 79 ){//193* average
      //Serial.println("I'm Bad");
     digitalWrite(5, LOW);//Green LED
      digitalWrite(6, HIGH);//Red LED
    }
    else {
     //Serial.println("I'm Good");
      digitalWrite(5, HIGH);//Green LED
      digitalWrite(6, LOW);//Red LED
   }
    //Serial.println(runningAverage);
    runningAverageCount = 0;
    runningAverage = 0;
  }
  
    
}

void PrintVector(double *vData, uint16_t bufferSize, uint8_t scaleType)
{
  for (uint16_t i = 0; i < bufferSize; i++)
  {
    double abscissa;
    /* Print abscissa value */
    switch (scaleType)
    {
      case SCL_INDEX:
        abscissa = (i * 1.0);
	break;
      case SCL_TIME:
        abscissa = ((i * 1.0) / sampleFrequency);
	break;
      case SCL_FREQUENCY:
        abscissa = ((i * 1.0 * sampleFrequency) / sampleSize);
	break;
    }
    Serial.print(abscissa, 6);
    if(scaleType==SCL_FREQUENCY)
      //Serial.print("Hz");
    Serial.print(" ");
    Serial.println(vData[i], 4);
  }
  Serial.println();
}
