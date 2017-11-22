#ifndef NEUROSKYMOBILEREAD_H
#define NEUROSKYMOBILEREAD_H

#include "mbed.h"
#include "blueSMIRF.h"
#include "BufferedSerial.h"

//Defined in main
extern BufferedSerial pc;

//Serial NeuroSky1(PA_9 ,PA_10, 57600); //TX RX
//Serial NeuroSky2(PB_10,PB_11, 57600);
extern blueSMIRF NeuroskyMobile_1;
extern blueSMIRF NeuroskyMobile_2;

void readNeuroskyMobile_1();
void readNeuroskyMobile_2();

#endif
