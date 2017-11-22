#include "slider.h"

//------------------------------------------------------------------------------
// Speed and durations
//------------------------------------------------------------------------------
float allignCornerSpeed = 0.4f;
float allignCenterSpeed = 0.5f;

uint16_t allignCenterDuration = 8500; //ms
uint16_t allignRightOffsetDuration = 3000; //ms

//------------------------------------------------------------------------------
// Pololu TBB6612fng Motor Controller
//------------------------------------------------------------------------------
PwmOut MPWM(PB_13);         //PWM
DigitalOut MDIRA(PC_6);     //Direction - AIN1
DigitalOut MDIRB(PC_8);     //Direction - AIN2
DigitalOut STBY(PC_5);      //Standby - STBY
//------------------------------------------------------------------------------
// Limit Switch Pins
//------------------------------------------------------------------------------
DigitalIn  LSL(PC_9);        //Limit Switch Right
DigitalIn  LSR(PD_2);        //Limit Switch Left
//------------------------------------------------------------------------------
// Interrupt timer and variables for Moving average
//  To average floating interrupt pins
//------------------------------------------------------------------------------
Ticker filter;

Timer allignTimer;

extern int game_Mode;
extern bool escape;

//Left Filter
volatile float alphaLeft = 0.75f;     //Forgetting Factor
volatile float filteredLeft = 0.0f;  //Filtered Output
//Right Filter
volatile float alphaRight = 0.75f;    //Forgetting Factor
volatile float filteredRight = 0.0f; //Filtered Output

volatile bool allignCenterCMD = false;
volatile bool allignRightCMD = false;
volatile bool allignLeftCMD = false;
volatile bool allignRightOffsetCMD = false;

volatile bool isAllignedCenter = false;
volatile bool isAllignedLeft = false;
volatile bool isAllignedRight = false;
volatile bool isAllignedRightOffset = false;

//Used in game
volatile bool leftLimitPushed = false;
volatile bool rightLimitPushed = false;

void sliderInit(void){
    //Run moving average filter at 100Hz
    filter.attach(&filterCalc, 0.05);
    //Enable limit switch GPIO pullDowns (to disable floating gpio)
    LSR.mode(PullDown);
    LSL.mode(PullDown);
    //Enable Motor Driver
    STBY = 1;
    //PWM frequency to 20Khz
    MPWM.period_us(50);

    //cartAllignLeft();
    //cartAllignCenter();
    //cartAllignRight();
}

//------------------------------------------------------------------------------
// Check for limit switches and calculate moving average filter
//  Moving average filter is needed because, limit switch pins are
//  floating when the cart is moving.
//  Too much noise from the motor!
//------------------------------------------------------------------------------
void filterCalc(void){
    if(escape){
        cartStop();
        allignLeftCMD = false;
        allignRightCMD = false;
        allignCenterCMD = false;
        allignRightOffsetCMD = false;
        
        isAllignedLeft = false;
        isAllignedRight = false;
        isAllignedCenter = false;
        isAllignedRightOffset = false;
        
        escape = false;
        allignTimer.reset();
        allignTimer.stop();
    }
    
    //Calculate moving average filter for limit switch RIGHT
    filteredLeft = alphaLeft * LSR.read() + (1-alphaLeft) * filteredLeft;
    //If filtered value for limit switch RIGHT is above 0,9 and game is running
    //Cart just pushed the switch
    if(filteredLeft > 0.9f){
        leftLimitPushed = true;
        if(!isAllignedLeft && allignLeftCMD){
            isAllignedLeft = true;
            isAllignedRight = false;
            allignLeftCMD = false;
            //allignCenterCMD = false;
            cartStop();
        }
    }
    else
        leftLimitPushed = false;

    //Calculate moving average filter for limit switch RIGHT
    filteredRight = alphaRight * LSL.read() + (1-alphaRight) * filteredRight;
    //If filtered value for limit switch LEFT is above 0,9 and game is running
    //Cart just pushed the switch
    if(filteredRight > 0.9f){
        rightLimitPushed = true;
        if(!isAllignedRight && allignRightCMD){
            isAllignedRight = true;
            isAllignedLeft = false;
            allignRightCMD = false;
            //allignCenterCMD = false;
            cartStop();
            if(!allignCenterCMD && !allignRightOffsetCMD)
                pc.printf("Ready\r\n");
        }
    }
    else
        rightLimitPushed = false;
    
    if(allignCenterCMD){
        if(isAllignedRight){
            MPWM.write(allignCenterSpeed);
            cartLeft();
            allignTimer.reset();
            allignTimer.start();
            allignRightCMD = false;
            //isAllignedLeft = false;
            isAllignedRight = false;
        }
        if(!allignRightCMD && !isAllignedRight){
            if(allignTimer.read_ms() > allignCenterDuration){
                allignTimer.reset();
                allignTimer.stop();
                cartStop();
                isAllignedCenter = true;
                allignCenterCMD = false;
                pc.printf("Ready\r\n");
            }
        }
        
    }
    
    if(allignRightOffsetCMD){
        if(isAllignedRight){
            MPWM.write(allignCenterSpeed);
            cartLeft();
            allignTimer.reset();
            allignTimer.start();
            allignRightCMD = false;
            //isAllignedLeft = false;
            isAllignedRight = false;
        }
        if(!allignRightCMD && !isAllignedRight){
            if(allignTimer.read_ms() > allignRightOffsetDuration){
                allignTimer.reset();
                allignTimer.stop();
                cartStop();
                isAllignedRightOffset = true;
                allignRightOffsetCMD = false;
                pc.printf("Ready\r\n");
            }
        }
    }
}
 
void cartAllignRight(void){
    cartStop();
    isAllignedCenter = false;
    isAllignedLeft = false;
    isAllignedRight = false;
    isAllignedRightOffset = false;
    
    allignLeftCMD = false;
    allignCenterCMD = false;
    allignRightOffsetCMD = false;

    if(!isAllignedRight){
        allignRightCMD = true;
        MPWM.write(allignCornerSpeed);
        cartRight();
    }
}

void cartAllignRightWithOffset(void){
    cartStop();
    
    allignCenterCMD = false;
    allignRightCMD = true;
    allignLeftCMD = false;
    allignRightOffsetCMD = true;
    
    isAllignedRight = false;
    isAllignedLeft = false;
    isAllignedCenter = false;
    isAllignedRightOffset = false;
    
    allignTimer.reset();
    allignTimer.stop();
    
    if(!isAllignedRight){
        MPWM.write(allignCornerSpeed);
        cartRight();
    }
}

void cartAllignLeft(void){
    cartStop();

    isAllignedRight = false;
    isAllignedLeft = false;
    isAllignedCenter = false;
    isAllignedRightOffset = false;
    
    allignRightCMD = false;
    allignCenterCMD = false;
    allignRightOffsetCMD = false;
    
    allignTimer.reset();
    allignTimer.stop();
    
    if(!isAllignedRight){
        allignLeftCMD = true;
        MPWM.write(allignCornerSpeed);
        cartLeft();
    }
}

void cartAllignCenter(){
    cartStop();
    
    allignCenterCMD = true;
    allignRightCMD = true;
    allignLeftCMD = false;
    allignRightOffsetCMD = false;
    
    isAllignedRight = false;
    isAllignedLeft = false;
    isAllignedCenter = false;
    isAllignedRightOffset = false;
    
    allignTimer.reset();
    allignTimer.stop();
    
    if(!isAllignedRight){
        MPWM.write(allignCornerSpeed);
        cartRight();
    }
}

 
void cartRight(void){
    MDIRA = 1;      //Pololu AIN1 Pin High
    MDIRB = 0;      //Pololu AIN2 Pin Low
}

void cartLeft(void){
    MDIRA = 0;      //Pololu AIN1 Pin High
    MDIRB = 1;      //Pololu AIN2 Pin Low
}

void cartStop(void){
    MDIRA = 0;      //Pololu AIN1 Pin Low
    MDIRB = 0;      //Pololu AIN2 Pin High
}

