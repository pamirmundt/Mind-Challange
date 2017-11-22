#ifndef GAME_H
#define GAME_H

#include "slider.h"

typedef enum
{
    noGame                      = 0x00,
    singlePlayerClassic         = 0x01,
    singlePlayerHardcore        = 0x02,
    multiplayerClassic          = 0x03,
    multiplayerFootball         = 0x04
} gameMode;

void singlePlayerClassicInit(void);
void singlePlayerHardcoreInit(void);
void multiPlayerClassicInit(void);
void multiPlayerFootballInit(void);

void playGame(int mode);

void play_singleGameClassic(void);
void play_singlePlayerHardcore(void);
void play_multiplayerClassic(void);
void play_multiplayerFootball(void);

#endif
