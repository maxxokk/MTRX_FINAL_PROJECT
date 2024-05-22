#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// Constants
#define NUM_SINGLE 2 // Number of single-player game modes
#define NUM_2P 2 // Number of 2-player game modes
#define PACKET_SIZE 256 // Max number of chars in the packet to send to PC to update scoreboard
#define BUFFER_SIZE 64

// External variables
extern void (*received)();
extern void (*inpDetect)();
extern void (*demonstrate)();
extern uint8_t inpStr[2][BUFFER_SIZE];


// Player structure
typedef struct {
    uint8_t score;
    uint8_t shots;
    uint8_t shot_dist;
    unsigned long int tot_dist;
    uint8_t pb_score;
    uint8_t pb_shot_dist;
    unsigned long int pb_tot_dist;
} Player;

// Game structure
typedef struct {
    Player* p1;
    Player* p2;
    signed char mode;
    uint8_t turn;
} Game;

// Function prototypes
bool button(void);
Player* setPlayer(Game* gamePtr, uint8_t p_index);
uint8_t getScore(Game* gamePtr, uint8_t p_index);
void score(Game* gamePtr);
void shotTaken(Game* gamePtr);
void switchTurn(Game* gamePtr);
Game* startGame(signed char gameMode);
void continueGame(Game* gamePtr);
uint8_t findGameMode(uint8_t data[BUFFER_SIZE], int size);
uint8_t str2uint(uint8_t data[BUFFER_SIZE], int size);
uint16_t getDist(void);
void setDist(Game* gamePtr, uint8_t player);
uint8_t endCondition(Game* gamePtr, uint8_t end_conds[4]);
void updateScoreboard(Game* gamePtr);
void gameLoop(unsigned char gamemode);

// External variables related to end conditions and continue question
extern uint8_t end_conds[4];
extern bool end_conds_set;
extern bool contSet;
extern bool askCont;

#endif // GAME_H
