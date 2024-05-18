#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <buckets.h>
#include <timer.h>

#define NUM_SINGLE 2 //number of single player game modes
#define NUM_2P 2 //number of 2 player game modes
#define PACKET_SIZE 256 //max number of chars in packet to send to PC to update scoreboard


typedef struct {
    uint8_t score;
    uint8_t shots;
    uint8_t shot_dist;
    uint8_t tot_dist;
    uint8_t pb_score;
    uint8_t pb_shot_dist;
    uint8_t pb_tot_dist;
} Player;

typedef struct {
	Player* p1;
    Player* p2;
    signed char mode; //1 player modes are positive, 2 player modes are negative
    uint8_t turn;
} Game;

Player* setPlayer(Game* gamePtr, uint8_t p_index){
    Player* player = NULL;

    //if player_index = -1, then we want this function to output the player whos turn it is
    //otherwise we can specify which player we want to output with p_index
    if((player_index == 0)||(player_index == -1 && (gamePtr->turn==0)||gamePtr->mode > 0)){ 
        player = gamePtr->p1;
    }

    //we only want to select player 2 based off the turn if it is a two player mode
    else if((player_index == 1)||((player_index == -1 && gamePtr->turn==1)&&(gamePtr->mode < 0))){
        player = gamePtr->p2;
    }

    return player;
}

uint8_t getScore(Game* gamePtr, p_index) {
    Player* player = setPlayer(gamePtr, p_index);

    return player->score;
}

void score(Game* gamePtr) {
    Player* player = setPlayer(gamePtr, -1);

    player->score += 1;
    player->tot_dist += player->shot_dist;

    if(player->score > player->pb_score){
        player->pb_score = player->score;
    }

    if(player->tot_dist > player->pb_tot_dist){
        player->pb_tot_dist = player->tot_dist
    }

    if(player->shot_dist > player->pb_shot_dist){
        player->pb_shot_dist = player->shot_dist;
    }

    updateScoreboard(gamePtr);
}

void shotTaken(Game* gamePtr){
    Player* player = setPlayer(gamePtr, -1);

    player->shots += 1;
}

void switchTurn(Game* gamePtr){
    game->turn = 1 - game->turn;
}

Game* startGame(uint8_t gameMode) {
    Player p1 = {0, 0, 0, 0, 0, 0, 0};
    Player p2 = {0, 0, 0, 0, 0, 0, 0};
    Game gameState = {p1, p2, gameMode, 0};
	return *gameState;
}

void continueGame(Game* gamePtr){
    gamePtr->p1->score = 0;
    gamePtr->p1->tot_dist = 0;

    gamePtr->p2->score = 0;
    gamePtr->p2->tot_dist = 0;

    static uint8_t first_turn = 0;
    first_turn = 1 - first_turn;

    game_ptr->turn = first_turn;
}

uint8_t findGameMode(data[BUFFER_SIZE], int size){
    uint8_t gameNumber = str2uint(data[BUFFER_SIZE], int size);

    if(gameNumber > NUM_SINGLE){
        gameNumber = (gameNumber * -1) + NUM_SINGLE;
    }

    return gameNumber;
}

uint8_t str2uint(data[BUFFER_SIZE], int size){
    uint8_t result = 0;
    for (int i = 0; i < size; i++) {
        char val = arr[i]-48;

        result = result * 10 + val;
    }
    return result;
}

void setDist(Game* gamePtr, uint8_t player){
    Player* player = setPlayer(gamePtr, player);

    //Nadeesha to create function that inputs the distance from LIDAR every 0.5 seconds
    //Output this distance via serial with a A in front of it if player = 0, or B if player = 1
    // e.g. SerialOut("A500", &USART1_PORT) would be a distance of 500 (units dont matter) for player 0
    //When the GPIO button on the STM32 is pressed:
        //stop taking distance measurements
        //output "D" via serial
        //player->shot_dist = <last distance to be output>
        //return from function
}

bool LDR(void){
    //Tom to write function that outputs TRUE when the ball is sitting on top of the holder i.e. when LDR is dark
}

bool flex(void){
    //Marco to write function that outputs TRUE if the flex sensor flexes
}

bool endCondition(Game* gamePtr){
    if((gamePtr->p1->shots >= NUM_SHOTS)||(gamePtr->p2->shots >= NUM_SHOTS)){
        return True;
    }
    else{
        return False;
    }
}

void updateScoreboard(Game* gamePtr){
    char packet[PACKET_SIZE];

    snprintf(packet, sizeof(packet),
             "%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%d,%u",
             game->p1->score, game->p1->shots, game->p1->shot_dist, game->p1->tot_dist, game->p1->pb_score, game->p1->pb_shot_dist, game->p1->pb_tot_dist,
             game->p2->score, game->p2->shots, game->p2->shot_dist, game->p2->tot_dist, game->p2->pb_score, game->p2->pb_shot_dist, game->p2->pb_tot_dist,
             game->mode, game->turn);

    SerialOut(packet, &USART1_PORT);
}

void gameLoop(uint8_t data[BUFFER_SIZE], int size){
    extern demonstrate = NULL;
    gameMode = findGameMode(data, size);

    Game* gamePtr = startGame(gameMode);

    if(gameMode == 1){
        setDist(gamePtr, 0);
        while(!LDR()){}

        while(!endCondition()){
            while(LDR()){}
            shotTaken(gamePtr);
            while(!LDR()){
                if(flex(void)){
                    score(gamePtr);
                }
            }
        }


    }

    else if(gameMode)
}