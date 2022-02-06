//macros for I2C project
//Chandler James - January 2022

//initalization values
#define INIT_REG1 0x67
/*-----------------------------------------------------------------
CTRL REG 1:  ODR3  ODR2  ODR1  ODR0  LPen  Zen  Yen  Xen
             0     1     1     0     0     1    1    1     = 0x67
ODR = output data rate.  Currently 200 Hz
Zen = enables the Z axis measurements
------------------------------------------------------------------*/
#define INIT_REG4 0x82 //default value = 00
/*-----------------------------------------------------------------
CTRL REG 4:  BDU  BLE   FS1   FS0    HR   ST1  ST0  SIM
             1     0     0     0     0     0    1    0     = 0x82
BDU = block data update.  one place new values in regs until they are read
ST  = self-test bits. 01 and 10 are two different test states.
------------------------------------------------------------------*/
#define INIT_REG5 0x40 // enables FIFO
#define INIT_FIFO 0x80 // sets FIFO to stream mode (a circular buffer)

//LIS3DH register addresses
#define ACC_SLAVE       0x18 //slave address for accelerometer
#define ACC_REG1_ADDR   0x20
#define ACC_REG4_ADDR   0x23
#define ACC_REG5_ADDR   0x24
#define ACC_FIFO_ADDR   0x2E
#define XL_ADDR         0x28
#define XH_ADDR         0x29
#define YL_ADDR         0x2A
#define YH_ADDR         0x2B
#define ZL_ADDR         0x2C
#define ZH_ADDR         0x2D
#define ACC_STATUS_ADDR 0x27

//LIS3DH Bit Masks
#define ACC_DATA_READY 0x08     //for reading ZYXDA on STATUS REG

//toggle design ideas
#define TBCNT_2   1     //set to 1 to set TBCNT = 2 for intialization
#define USE_INT1  0     //test of INT1 can notify when data is ready
#define READ_ALL  0     //test if we can send one read message and get all data
#define USE_TIMER 1     //test if timer can be used
#define ACC_DEBUG 0     //debug state
//#define SELF_TEST 0     //set accelerometer to generate arbitrary measurement values
#define SET_CTRL_REG4 1  //if 0, use default reg4 value, so don't send anything

//Timers
#define TimerB0_0CCR_Vector             (TIMER0_B0_VECTOR)
#define TimerB0_1_2_OF_CCR_Vector       (TIMER0_B1_VECTOR)

#define TimerB1_0CCR_Vector             (TIMER1_B0_VECTOR)
#define TimerB1_1_2_OF_CCR_Vector       (TIMER1_B1_VECTOR)

#define TimerB2_0CCR_Vector             (TIMER2_B0_VECTOR)
#define TimerB2_1_2_OF_CCR_Vector       (TIMER2_B1_VECTOR)

/*/old carlson values for timers
#define TB0CCR0_INTERVAL (625)        //interrupt every 5 msec 
#define TB0CCR1_INTERVAL (6250)       //interrupt every 50 msec
#define TB0CCR2_INTERVAL (62500)       //interrupt every 500 msec

#define TB1CCR0_INTERVAL (31250)        //interrupt every 250 msec
#define TB1CCR1_INTERVAL (62500)       //interrupt every 500 msec
#define TB1CCR2_INTERVAL (62500)

#define TB2CCR0_INTERVAL (25000)        //interrupt every 200 msec
#define TB2CCR1_INTERVAL (25000)       //interrupt every 200 msec
#define TB2CCR2_INTERVAL (6250)         //interrupt every 50 msec
*/

//SMCLK = 1 MHz
// Interval = (clk * desired time) / (ID * EX0)

#define TB0CCR0_INTERVAL (15625)        //interrupt every 1 sec 
#define TB0CCR1_INTERVAL (1562)       //interrupt ever 100 msec
#define TB0CCR2_INTERVAL (62500)       //interrupt every 500 msec

#define TB1CCR0_INTERVAL (31250)        //interrupt every 250 msec
#define TB1CCR1_INTERVAL (62500)       //interrupt every 500 msec
#define TB1CCR2_INTERVAL (62500)

#define TB2CCR0_INTERVAL (25000)        //interrupt every 200 msec
#define TB2CCR1_INTERVAL (25000)       //interrupt every 200 msec
#define TB2CCR2_INTERVAL (6250)         //interrupt every 50 msec

//LED ports
#define RED_LED              (0x01) // RED LED 0
#define GRN_LED              (0x40) // GREEN LED

/*
// Turns off both LEDs
  P1OUT &= ~RED_LED;
  P6OUT &= ~GRN_LED;
*/

//I2C
void wait_I2C(void);
void init_I2C(void);
void write_I2C(char slave_addr, char reg_addr, char write_data);
void read_I2C(char slave_addr, char reg_addr);

//accelerometer
void init_lis3dh(void);
void read_lis3dh(void);


//timers
void Init_Timer_B0(void);
void start_timerB0_CCR0(void);
void start_timerB0_CCR1(void);
void start_timerB0_CCR2(void);
void stop_timerB0_CCR0(void);
void stop_timerB0_CCR1(void);
void stop_timerB0_CCR2(void);
void Init_Clocks(void);
