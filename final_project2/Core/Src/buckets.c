#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <buckets.h>
#include "serial.h"
#include "lidar.h"
#include <stdbool.h>

#define NUM_SINGLE 2 //number of single player game modes
#define NUM_2P 2 //number of 2 player game modes
#define PACKET_SIZE 256 //max number of chars in packet to send to PC to update scoreboard

#define DEFAULT_SHOTS 10
#define DEFAULT_SCORE 10
#define DEFAULT_DIST 100
#define DEFAULT_END_COND 1


bool button(void){
	if ((GPIOA->IDR & 0x01) == 1){
		return true;
	}
	return false;
}

Player* setPlayer(Game* gamePtr, uint8_t p_index){ //p_index = 0 for player 1, p_index = 1 for player 2, p_index = -1 for whos turn it is
    Player* player = NULL;

    //if p_index = -1, then we want this function to output the player whos turn it is
    //otherwise we can specify which player we want to output with p_index
    if((p_index == 0)||(p_index == -1 && (gamePtr->turn==0))||gamePtr->mode > 0){
        player = gamePtr->p1;
    }

    //we only want to select player 2 based off the turn if it is a two player mode
    else if((p_index == 1)||((p_index == -1 && gamePtr->turn==1)&&(gamePtr->mode < 0))){
        player = gamePtr->p2;
    }

    return player;
}

uint8_t getScore(Game* gamePtr, uint8_t p_index) {
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
        player->pb_tot_dist = player->tot_dist;

    }

    if(player->shot_dist > player->pb_shot_dist){
        player->pb_shot_dist = player->shot_dist;
    }

    updateScoreboard(gamePtr);
}

void shotTaken(Game* gamePtr){
    Player* player = setPlayer(gamePtr, 0);
    //Player* player = setPlayer(gamePtr, -1);

    player->shots += 1;
    updateScoreboard(gamePtr);
}

void switchTurn(Game* gamePtr){
    gamePtr->turn = 1 - gamePtr->turn;
}

Game* startGame(signed char gameMode) {
    Player p1 = {0, 0, 0, 0, 0, 0, 0};
    Player p2 = {0, 0, 0, 0, 0, 0, 0};
    Game gameState;
    gameState.mode = gameMode;
    gameState.p1 = &p1;
    gameState.p2 = &p2;
    gameState.turn = 0;
	return &gameState;
}

//reset necessary parameters if game is to be replayed
void continueGame(Game* gamePtr){
    gamePtr->p1->score = 0;
    gamePtr->p1->tot_dist = 0;

    gamePtr->p2->score = 0;
    gamePtr->p2->tot_dist = 0;

    static uint8_t first_turn = 0; //which player gets the first turn alternates on successive rematches
    first_turn = 1 - first_turn;

    gamePtr->turn = first_turn;
}

//determine game mode number (range [-2, 2]) from serial input (range [1, 4])
uint8_t findGameMode(uint8_t data[BUFFER_SIZE], int size){
    uint8_t gameNumber = data[0]-49;

    if(gameNumber > NUM_SINGLE){
        gameNumber = (gameNumber * -1) + NUM_SINGLE;
    }

    return gameNumber;
}

//take str (array of chars) and convert to uint8_t
uint8_t str2uint(uint8_t data[BUFFER_SIZE], int size){
    uint8_t result = 0;
    for (int i = 0; i < size; i++) {
        char val = data[i]-48;

        result = result * 10 + val;
    }
    return result;
}


void setDist(Game* gamePtr, uint8_t player_num){
	//    while (!button())
	//    {
	//        // Get the distance from the LiDAR
	//        uint16_t distance = get_distance();
	//
	//        // Format and send the distance measurement via UART
	//        sprintf(string_to_send, "S%i%hu\n", gamemode, distance);
	//        HAL_Delay(100); // Add delay between measurements
	//
	//        SerialOutputString(string_to_send, &USART1_PORT);
	//    }
	//    SerialOutputString("D", &USART1_PORT);

////////////////////////////
    Player* target_player = setPlayer(gamePtr, player_num);
    uint8_t distance;

    uint8_t outputStr[128];

    while(!button()){
        distance = get_distance();
        sprintf(outputStr, "S%hu\n", distance);
        SerialOut(outputStr, &USART1_PORT);
        HAL_Delay(100);
    }

    SerialOut("D\n", &USART1_PORT);
    target_player->shot_dist = distance;
//////////////////////////////

    //Nadeesha to create function that inputs the distance from LIDAR every 0.5 seconds
    //Output this distance via serial e.g. SerialOut("500", &USART1_PORT) would be a distance of 500 (units dont matter)
    //When the GPIO button on the STM32 is pressed:
        //stop taking distance measurements
        //output "D" via serial
        //player->shot_dist = <last distance to be output>
        //return from function
}

//check whether the game should end or not.
//0 = continue game
//1 = player 1 wins
//2 = player 2 wins
uint8_t endCondition(Game* gamePtr, uint8_t end_conds[4]){
    uint8_t max_shots = end_conds[0];
    uint8_t max_score = end_conds[1];
    uint8_t max_dist = end_conds[2];
    uint8_t endCond = end_conds[3];

    uint8_t result = 0;

    bool p1_shots = (gamePtr->p1->shots >= max_shots);
    bool p2_shots = (gamePtr->p2->shots >= max_shots);

    bool p1_score = (gamePtr->p1->score >= max_score);
    bool p2_score = (gamePtr->p2->score >= max_score);

    bool p1_dist = (gamePtr->p1->tot_dist >= max_dist);
    bool p2_dist = (gamePtr->p2->tot_dist >= max_dist);

    if(gamePtr->mode > 0){
        if(endCond == 1){ //once player has made max amnt of shots, end game
            result = p1_shots;
        }

        else if(endCond == 2){ //once player has reached a certain score, end game
            result = p1_score;
        }

        else if(endCond == 3){ //once player has reached a certain distance, end game
            result = p1_dist;
        }

        else if(endCond != 0){ //if 0, then result remains 0 (i.e. never end). If not 0, 1, 2, or 3: return 3 as an error result
            result = 3;
        }
    }

    else if(gamePtr->mode < 0){
        if(endCond == 1){ //players get a certain amount of shots. If scores are tied after each player has made the max shots, then play continues
            result = (1 + (gamePtr->p2->score > gamePtr->p1->score))*(p1_shots && p2_shots && (gamePtr->p1->score != gamePtr->p2->score));
        }

        else if(endCond == 2){ //player who reaches given score first wins
            result = (1 + (gamePtr->p2->score > gamePtr->p1->score))*(p1_score || p2_score);
        }

        else if(endCond == 3){ //playe who reaches given total distance first wins
            result = (1 + (gamePtr->p2->tot_dist > gamePtr->p1->tot_dist))*(p1_dist || p2_dist);
        }

        else if(endCond!=0){ //if 0, then result remains 0 (i.e. never end). If not 0, 1, 2, or 3: return 3 as an error result
            result = 3;
        }
    }

    return result;

}

void updateScoreboard(Game* game){
    uint8_t packet[PACKET_SIZE];

    sprintf(packet, "%hu,%hu,%lu,%hu,%hu,%lu,%hu,%hu,%lu,%hu,%hu,%lu,%hu\n", game->p1->score, game->p1->shots, game->p1->tot_dist, game->p1->pb_score, game->p1->pb_shot_dist, game->p1->pb_tot_dist, game->p2->score, game->p2->shots, game->p2->tot_dist, game->p2->pb_score, game->p2->pb_shot_dist, game->p2->pb_tot_dist, game->turn);

    SerialOut(packet, &USART1_PORT);
}

uint8_t end_conds[4] = {0, 0, 0, 0}; //{<which condition to end game (0 = never end. 1 = shots, 2 = score, 3 = distance)>, <max_shots>, <max_score>, <max_tot_dist>}
bool end_conds_set = false; //have the end conditions been set by the user? (Either manually or via gameMode selection)

void set_end_conds(uint8_t data[BUFFER_SIZE], int size){
    uint8_t cond, val;
    sscanf(data, "%d:%d", &cond, &val);

    if(cond != 0){
        end_conds[cond] = val;
        end_conds[0] = cond;
    }

    end_conds_set = true;
}

bool contSet = false; //has continue question been responded too?
bool askCont = false; //does user want to continue?

void askContinue(uint8_t data[BUFFER_SIZE], int size){
    if(data[0]=='1'){
        askCont = true;
    }
    contSet = true;
}

void gameLoop(unsigned char gamemode){
    Player p1 = {0, 0, 0, 0, 0, 0, 0};
    Player p2 = {0, 0, 0, 0, 0, 0, 0};
    Game gameState;
    gameState.mode = gamemode;
    gameState.p1 = &p1;
    gameState.p2 = &p2;
    gameState.turn = 0;

    Game* gamePtr = &gameState;

    if(abs(gamemode)==3){
        demonstrate = &set_end_conds;
        SerialOut("R", &USART1_PORT);
        while(!end_conds_set){}
    }

    else{
        end_conds[1] = DEFAULT_SHOTS;
        end_conds[2] = DEFAULT_SCORE;
        end_conds[3] = DEFAULT_DIST;
    }

    if(gamemode == 1){
        end_conds[0] = 1;
    }

    if(gamemode == 2){
        end_conds[0] = 0;
    }

    if(gamemode == -1){
        end_conds[0] = 3;
    }

    if(gamemode == -2){
        end_conds[0] = 1;
    }

    while(!button()){}
    while(button()){}

    if(gamemode > 0){
        while(true){
            gamePtr->turn = 0;

            if(gamemode == 1){
                setDist(gamePtr, 0);
            }

            else if(gamemode == 2){
                gamePtr->p1->shot_dist = 1;
            }

            while(LDR()){}

//            while(!endCondition(gamePtr, end_conds)){
            while(1){
                while(!LDR()){}
                shotTaken(gamePtr);
                while(LDR()){
                    if(button()){
                        score(gamePtr);
                        while(button()){}
                        while(LDR()){}
                    }
                }
            }

            uint8_t winner = endCondition(gamePtr, end_conds);
            if(winner == 1){
                SerialOut("W1\n", &USART1_PORT); //tell scoreboard to display 'player 1 wins'
            }

            else if(winner == 2){
                SerialOut("W2\n", &USART1_PORT); //tell scoreboard to display 'player 2 wins'
            }

            else if(winner == 3){
                return; //error - end condition (endCond[0]) not 0, 1, or 2.
            }

            while(!contSet){
                demonstrate = &askContinue;
            }

            if(askCont){
                continueGame(gamePtr);
                continue;
            }

            else{
                return;
            }
        }
    }
}
