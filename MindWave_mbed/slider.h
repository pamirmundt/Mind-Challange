#ifndef SLIDER_H
#define SLIDER_H

#include "mbed.h"
#include "BufferedSerial.h"

extern BufferedSerial pc;

void sliderInit(void);

void filterCalc(void);


void cartAllignRight(void);
void cartAllignRightWithOffset(void);
void cartAllignLeft(void);
void cartAllignCenter(void);
 
void cartRight(void);
void cartLeft(void);
void cartStop(void);

#endif
