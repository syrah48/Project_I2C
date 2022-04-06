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
  digitalWrite(6, HIGH);//Red LED
  // delay(500);
  // digitalWrite(5, LOW);//Green LED
  // digitalWrite(6, LOW);//Red LED
  // delay(500);
  // digitalWrite(5, HIGH);//Green LED
  // digitalWrite(6, HIGH);//Red LED
  
  Serial.begin(115200); //(Max 115200 without issues)
  delay(1000); // relax...
  //Call .begin() to configure the IMU
  myIMU.begin();
  // float out_im = malloc(128*sizeof(float));
  // float out_r = malloc(128*sizeof(float));
  
}

float runningAverage = 0;
float dataAverage = 0;
unsigned int samplecount = 0;
//uint8_t f_peaks[5]; // top 5 frequencies peaks in descending order
//uint8_t maxAmplitude[5];
//int runCount =0;
int temp = 0;
int temp_offset = 19; // Degrees Celsius
const int sampleSize = 128; // Power of 2 ex(32,64,128,256...) Max 128 in SRAM

unsigned long mytime[sampleSize]; //Time stamps stored here
// Accelerometer values are 8 bit, only use in Low Power Mode
// stored in 8 bit arrays to save on size
//int8_t accx[sampleSize]; // Acceleration X (Horizontal (+ away from junction box))
int accy[sampleSize]; // Acceleration Y (Vertical (+ Up))
//int8_t accz[sampleSize]; // Acceleration Z (Horizontal (+ towards output shaft))

//Defining temporary variables
//float x = 0;
float y = 0;
//float z = 0;
uint8_t status_reg;
// void printPeaks(void){
//   for(int i = 0; i< 5;i++)
//     Serial.println(f_peaks[i]);

//   return;
// }
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
double out_r[128]={};   //real part of transform  //////////////TODO: clear these arrays before next computation
double out_im[128]={};  //imaginory part of transform
int in_ps[128]={};     //input for sequencing
void FFT(int in[],int N,float Frequency)
{
/*
Code to perform FFT on arduino,
setup:
paste sine_data [91] at top of program [global variable], paste FFT function at end of program
Term:
1. in[]     : Data array, 
2. N        : Number of sample (recommended sample size 2,4,8,16,32,64,128...)
3. Frequency: sampling frequency required as input (Hz)
If sample size is not in power of 2 it will be clipped to lower side of number. 
i.e, for 150 number of samples, code will consider first 128 sample, remaining sample  will be omitted.
For Arduino nano, FFT of more than 128 sample not possible due to mamory limitation (64 recomended)
For higher Number of sample may arise Mamory related issue,
Code by ABHILASH
Contact: abhilashpatel121@gmail.com 
Documentation:https://www.instructables.com/member/abhilash_patel/instructables/
2/3/2021: change data type of N from float to int for >=256 samples
*/
unsigned int data[13]={1,2,4,8,16,32,64,128,256,512,1024,2048};
  int a,c1,f,o,x;
  a=N;  
  //clear arrays. testing if data changes
  for(int i = 0; i < 128;i++){
    out_im[i] = 0;
    out_r[i] = 0;
    in_ps[i]= 0;
  }
                                 
      for(int i=0;i<12;i++)                 //calculating the levels
         { if(data[i]<=a){o=i;} }
        //  Serial.print("level calc number : ");
        //  Serial.println(o);


//Serial.print("imnothere");

           
x=0;  
      for(int b=0;b<o;b++)                     // bit reversal
         {
          c1=data[b];
          f=data[o]/(c1+c1);
                for(int j=0;j<c1;j++)
                    { 
                     x=x+1;
                     in_ps[x]=in_ps[j]+f;
                    }
         }

 
      for(int i=0;i<data[o];i++)            // update input array as per bit reverse order
         {
          if(in_ps[i]<a)
          {out_r[i]=in[in_ps[i]];}
          if(in_ps[i]>a)
          {out_r[i]=in[in_ps[i]-a];}      
         }


int i10,i11,n1;
float e,c,s,tr,ti;

    for(int i=0;i<o;i++)                                    //fft
    {
     i10=data[i];              // overall values of sine/cosine  :
     i11=data[o]/data[i+1];    // loop with similar sine cosine:
     e=360/data[i+1];
     e=0-e;
     n1=0;

          for(int j=0;j<i10;j++)
          {
          c=cos(e*j);
          s=sin(e*j);    
          n1=j;
          
                for(int k=0;k<i11;k++)
                 {
                 tr=c*out_r[i10+n1]-s*out_im[i10+n1];
                 ti=s*out_r[i10+n1]+c*out_im[i10+n1];
          
                 out_r[n1+i10]=out_r[n1]-tr;
                 out_r[n1]=out_r[n1]+tr;
          
                 out_im[n1+i10]=out_im[n1]-ti;
                 out_im[n1]=out_im[n1]+ti;          
          
                 n1=n1+i10+i10;
                  }       
             }
     }
//---> here onward out_r contains amplitude and out_im conntains frequency (Hz)
    Serial.println();
    Serial.println("EasyFFT: ");
    // Serial.print("FFT finish o value : ");
    // Serial.println(o);
    for(int i=0;i<data[o-1];i++)               // getting amplitude from complex number
        {
         out_r[i]=sqrt(out_r[i]*out_r[i]+out_im[i]*out_im[i]); // to  increase the speed delete sqrt
         out_im[i]=i*(Frequency/N);

         //print freuqency then amplitude 
         
         Serial.print(out_im[i]);
         Serial.print(" ");
         Serial.print(out_r[i]/10);
         Serial.println();
        }
        Serial.println();
}

float sampleFrequency = 0;
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


  
  Serial.println("mytimes: ");
  
  sampleFrequency = 1/(0.000001*(mytime[127] - mytime[0])/sampleSize);
  Serial.println(sampleFrequency);
  //Serial.println(sampleFrequency);
  printSamples();
  FFT(accy,sampleSize,sampleFrequency);
  
  // samplecount++;
  // if(samplecount == 0 || runningAverage < 0){
  //   runningAverage = 0;
  //   samplecount = 0;
  // }
  // else
  //  runningAverage = (runningAverage*(samplecount-1) + f_peaks[0])/samplecount;
  // Serial.println(runningAverage);
  // Serial.println(samplecount);
  // if(runningAverage > 0 && samplecount >2000){// whatever we want to see
  //   //Serial.println("I'm Bad");
  //   digitalWrite(5, LOW);//Green LED
  //   digitalWrite(6, HIGH);//Red LED
   
  // }
  // else {
  //   //Serial.println("I'm Good");
  //   digitalWrite(5, LOW);//Red LED
  //   digitalWrite(6, HIGH);//Green LED
  // }
    
}
