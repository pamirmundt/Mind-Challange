#include "game.h"

float singleNormalSpeed = 0.3f;        //0.0 - 1.0
float singleHardSpeed = 0.35f;          //0.0 - 1.0
float singleHardReverseSpeed = 0.3f;   //0.0 - 1.0
float multiSpeed = 0.3f;                //0.0 - 1.0
float footballSpeed = 0.4f;             //0.0 - 1.0

//Thresholds
uint8_t singleNormalThreshold = 40;
uint8_t singleHardThreshold = 30;

//Multi mode counters
//Couting the total time
Timer NS1Time;
Timer NS2Time;

bool N1time_live = false;
bool N2time_live = false;

extern int NS1_value;
extern int NS2_value;

extern PwmOut MPWM;

extern int game_Mode;
extern bool escape;
extern bool gameStarted;

extern volatile bool isAllignedCenter;
extern volatile bool isAllignedLeft;
extern volatile bool isAllignedRight;
extern volatile bool isAllignedRightOffset;

extern volatile bool leftLimitPushed;
extern volatile bool rightLimitPushed;

void singlePlayerClassicInit(void){
    if(!isAllignedRight)
        cartAllignRight();
}

void singlePlayerHardcoreInit(void){
    if(!isAllignedRightOffset)
        cartAllignRightWithOffset();
}

void multiPlayerClassicInit(void){
    if(!isAllignedCenter)
        cartAllignCenter();
}

void multiPlayerFootballInit(void){
    if(!isAllignedCenter)
        cartAllignCenter();
}

void playGame(int mode){
    switch(mode){
        case singlePlayerClassic:
            play_singleGameClassic();
            break;
        case singlePlayerHardcore:
            play_singlePlayerHardcore();
            break;
        case multiplayerClassic:
            play_multiplayerClassic();
            break;
        case multiplayerFootball:
            play_multiplayerFootball();
            break;
    }
}

void play_singleGameClassic(void){
    if(leftLimitPushed){
        cartStop();
        pc.printf("W1\r\n");
        game_Mode = noGame;
        gameStarted = false;
    }
    else{
        if(NS1_value >= singleNormalThreshold){
            MPWM.write(singleNormalSpeed);
            cartLeft();
        }
        else
            cartStop();
    }
}

void play_singlePlayerHardcore(void){
    //Win
    if(leftLimitPushed){
        cartStop();
        pc.printf("W1\r\n");
        game_Mode = noGame;
        gameStarted = false;
    }
    //Lose
    else if(rightLimitPushed){
        cartStop();
        pc.printf("WN\r\n");
        game_Mode = noGame;
        gameStarted = false;
    }
    //In game
    else{
        if(NS1_value >= singleHardThreshold){
            MPWM.write(singleHardSpeed);
            cartLeft();
        }
        else{
            MPWM.write(singleHardReverseSpeed);
            cartRight();
        }
    }
}

void play_multiplayerClassic(void){
    if(NS1_value > NS2_value){
        MPWM.write(multiSpeed);
        cartRight();
        if(!N1time_live){
            NS1Time.start();
            N1time_live = true;
        }
        if(N2time_live){
            NS2Time.stop();
            N2time_live = false;
        }
    }
    else if(NS1_value < NS2_value){
        MPWM.write(multiSpeed);
        cartLeft();
        if(N1time_live){
            NS1Time.stop();
            N1time_live = false;
        }
        if(!N2time_live){
            NS2Time.start();
            N2time_live = true;
        }
    }
    else{
        cartStop();
        if(N1time_live)
            NS1Time.stop();
        if(N2time_live)
            NS2Time.stop();
    }
    
    //Game Ends
    if(leftLimitPushed){
        cartStop();
        game_Mode = noGame;
        gameStarted = false;
        pc.printf("W2\r\n");
        NS1Time.reset();
        NS2Time.reset();
        NS1Time.stop();
        NS2Time.stop();
    }
    else if(rightLimitPushed){
        cartStop();
        game_Mode = noGame;
        gameStarted = false;
        pc.printf("W1\r\n");
        NS1Time.reset();
        NS2Time.reset();
        NS1Time.stop();
        NS2Time.stop();
    }
}

void play_multiplayerFootball(void){
    if(NS1_value < NS2_value){
        MPWM.write(footballSpeed);
        cartRight();
    }
    else if(NS1_value > NS2_value){
        MPWM.write(footballSpeed);
        cartLeft();
    }
    else{
        cartStop();
    }
    
    //Game Ends
    if(leftLimitPushed){
        cartStop();
        //game_Mode = noGame;
        gameStarted = false;
        pc.printf("W1\r\n");
        multiPlayerFootballInit();
    }
    else if(rightLimitPushed){
        cartStop();
        //game_Mode = noGame;
        gameStarted = false;
        pc.printf("W2\r\n");
        multiPlayerFootballInit();
    }
}
