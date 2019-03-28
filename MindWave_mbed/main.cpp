/*
    Version 1.1.0
*/

#include "mbed.h"
#include <string>

#include "main.h"
#include "game.h"
#include "slider.h"
#include "NeuroskyMobileRead.h"

extern int NS1_value;
extern int NS2_value;

Ticker dummy;

BufferedSerial pc(USBTX, USBRX);

extern volatile bool leftLimitPushed;
extern volatile bool rightLimitPushed;

extern volatile bool allignCenterCMD;
extern volatile bool allignRightCMD;
extern volatile bool allignLeftCMD;
extern volatile bool allignRightOffsetCMD;

extern volatile bool isAllignedCenter;
extern volatile bool isAllignedLeft;
extern volatile bool isAllignedRight;
extern volatile bool isAllignedRightOffset;

// Settings
extern uint16_t allignCenterDuration;
extern float allignCenterSpeed;
extern float allignCornerSpeed;
extern uint8_t singleNormalThreshold;
extern float singleNormalSpeed;
extern uint8_t singleHardThreshold;
extern float singleHardSpeed;
extern float singleHardReverseSpeed;
extern uint16_t allignRightOffsetDuration;
extern float multiSpeed;
extern float footballSpeed;

//extern float filteredLeft;
//extern float filteredRight;

//extern Timer allignTimer;

extern Timer NS1Time;
extern Timer NS2Time;

//Check if timers are already running
extern bool N1time_live;
extern bool N2time_live;

void initMind(void);
void readPC(void);
void printDummy(void);
int readlineTimeout(char buffer[]);
int readByteTimeout(void);
void readSettings(char buffer[], int bufferSize);

int game_Mode = 0;

bool escape = false;
bool gameStarted = false;

int main() {
    
    sliderInit();
    
    pc.baud(9600);
    NeuroskyMobile_1.setBaud(57600);
    NeuroskyMobile_2.setBaud(57600);
    
    //dummy.attach(&printDummy, 1.0);
    
    //initMind();
    
    while(1) {
        readNeuroskyMobile_1();
        readNeuroskyMobile_2();
        readPC();
        if(gameStarted)
            playGame(game_Mode);
        //pc.printf("C:%d R:%d L:%d CO:%d CC:%d CR:%d CL:%d CO:%d G:%d GS:%d R:%d L:%d\r\n", isAllignedCenter, isAllignedRight, isAllignedLeft, allignRightOffsetCMD, allignCenterCMD, allignRightCMD, allignLeftCMD,isAllignedRightOffset, game_Mode, gameStarted, rightLimitPushed,leftLimitPushed);
    }
}

void initMind(){
    NeuroskyMobile_1.flushSerial();
    
    if(NeuroskyMobile_1.exitCMD())
        pc.printf("0) Exited Command Mode\r\n");
        
    if(NeuroskyMobile_1.enterCMD())
        pc.printf("1) Entered Command Mode\r\n");
    
    if(NeuroskyMobile_1.setUART("57"))
        pc.printf("2) UART Baud Changed\r\n");
        
    if(NeuroskyMobile_1.setConfTimer(255))
        pc.printf("3) Continous configuration, local and remote\r\n");
    
    if(NeuroskyMobile_1.setName("mindwave_1"))
        pc.printf("4) Name Set\r\n");
        
    if(NeuroskyMobile_1.setMode(6));
        pc.printf("5) Mode Set\r\n");
        
    if(NeuroskyMobile_1.setPinCode("0000"))
        pc.printf("6) Pin Set\r\n");
        
    if(NeuroskyMobile_1.setSpecialConf(16))
        pc.printf("7) Special Configuration\r\n");
        
    if(NeuroskyMobile_1.setAuthentication(0))
        pc.printf("8) Authentication Disabled\r\n");
        
    if(NeuroskyMobile_1.setRemoteAddress("74E5439C6003"))
        pc.printf("9) Address Set\r\n");

    //NeuroskyMobile_1.printf("+\n");
    //while(1){
    //    pc.printf("%c",NeuroskyMobile_1.readByteBlocking());
    //}
        
    if(NeuroskyMobile_1.reboot())
        pc.printf("Reboot\r\n");
}

void readPC(void){
    if(pc.readable()){
        uint8_t byteRead = pc.getc();
        pc.printf("%c\r\n", byteRead);
        switch(byteRead){
            case 'S':
                gameStarted = false;
                game_Mode = singlePlayerClassic;
                singlePlayerClassicInit();
                break;
            case 'H':
                gameStarted = false;
                game_Mode = singlePlayerHardcore;
                singlePlayerHardcoreInit();
                break;
            case 'M':
                gameStarted = false;
                game_Mode = multiplayerClassic;
                multiPlayerClassicInit();
                break;
            case 'F':
                gameStarted = false;
                game_Mode = multiplayerFootball;
                multiPlayerFootballInit();
                break;
                
            case 'E':
                //If we are in multiplayer game and game ends with 'E'
                if(game_Mode ==  multiplayerClassic && gameStarted == true){
                    if(NS1Time.read_ms() > NS2Time.read_ms())
                        pc.printf("W1\r\n");
                    else if(NS1Time.read_ms() < NS2Time.read_ms())
                        pc.printf("W2\r\n");
                    else
                        pc.printf("WN\r\n");
                }
                NS1Time.reset();
                NS2Time.reset();
                NS1Time.stop();
                NS2Time.stop();
                N1time_live = false;
                N2time_live = false;
                
                escape = true;
                game_Mode = noGame;
                gameStarted = false;
                break;
            case 'N':
                if(isAllignedCenter || isAllignedRight || isAllignedLeft || isAllignedRightOffset){
                    gameStarted = true;
                    //Game Started - No allignment
                    isAllignedCenter = 0;
                    isAllignedRight = 0;
                    isAllignedLeft = 0;
                    isAllignedRightOffset = 0;
                    NS1Time.reset();
                    NS2Time.reset();
                    NS1Time.stop();
                    NS2Time.stop();
                    N1time_live = false;
                    N2time_live = false;
                }
                break;
                
            // Settings
            case '*':
                char buffer[10];
                memset(buffer, 0, sizeof(buffer));
                int index = readlineTimeout(buffer);
                
                readSettings(buffer, sizeof(buffer));
                
                break;
        }
    }
}

void readSettings(char buffer[], int bufferSize){

    string tmp = buffer;
    
    // alignmentDuration - AD
    if(tmp.compare(0,2,"AD") == 0){
        int value = atoi(buffer + 2);
        allignCenterDuration = value;
    }
    // centerAlignmentSpeed - CEAS
    else if(tmp.compare(0,4,"CEAS") == 0){
        int value = atoi(buffer + 4);
        allignCenterSpeed = float(value)/100.0f;
    }
    // cornerAlignmentSpeed - COAS
    else if(tmp.compare(0,4,"COAS") == 0){
        int value = atoi(buffer + 4);
        allignCornerSpeed = float(value)/100.0f;
    }
    // singleNormalThreshold - SNT
    else if(tmp.compare(0,3,"SNT") == 0){
        int value = atoi(buffer + 3);
        singleNormalThreshold = value;
    }
    // singleNormalSpeed - SNS
    else if(tmp.compare(0,3,"SNS") == 0){
        int value = atoi(buffer + 3);
        singleNormalSpeed = float(value)/100.0f;
    }
    //singleAdvancedThreshold - SAT
    else if(tmp.compare(0,3,"SAT") == 0){
        int value = atoi(buffer + 3);
        singleHardThreshold = value;
    }
    //singleAdvancedSpeed - SAS
    else if(tmp.compare(0,3,"SAS") == 0){
        int value = atoi(buffer + 3);
        singleHardSpeed = float(value)/100.0f;
    }
    //singleAdvancedReverseSpeed - SARS
    else if(tmp.compare(0,4,"SARS") == 0){
        int value = atoi(buffer + 4);
        singleHardReverseSpeed = float(value)/100.0f;
    }
    //singleAdvancedAlignDuration - SAAD
    else if(tmp.compare(0,4,"SAAD") == 0){
        int value = atoi(buffer + 4);
        allignRightOffsetDuration = value;
    }
    //multiChallengeSpeed - MCS
    else if(tmp.compare(0,3,"MCS") == 0){
        int value = atoi(buffer + 3);
        multiSpeed = float(value)/100.0f;
    }
    //multiFootballSpeed - MFS
    else if(tmp.compare(0,3,"MFS") == 0){
        int value = atoi(buffer + 3);
        footballSpeed = float(value)/100.0f;
    }
}

int readlineTimeout(char buffer[]){
    
    int index = 0;
    
    while(true){
        int c = readByteTimeout();
        
        if ( (c != '\n') && (c != '\r') && (c != -1) ){
            buffer[index] = c;
            index++;
        }

        if( (c == '\n') || (c == '\r') || (c == -1) )
            return index;
    }
}

int readByteTimeout(void){
    Timer timeOut;
    timeOut.start();
    while(!pc.readable()){
         if(timeOut.read_ms() > 5)
            return -1;
    }
    
    return pc.getc();
}

void printDummy(void){
    NS1_value = rand()%101;
    pc.printf("NS1,%d\r\n",NS1_value);
    NS2_value = rand()%101;
    pc.printf("NS2,%d\r\n",NS2_value);
}
