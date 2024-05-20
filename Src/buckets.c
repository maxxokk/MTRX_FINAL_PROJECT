#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <buckets.h>
#include <timer.h>

#define NUM_SINGLE 2 //number of single player game modes
#define NUM_2P 2 //number of 2 player game modes
#define PACKET_SIZE 256 //max number of chars in packet to send to PC to update scoreboard

extern uint8_t input[2][BUFFER_SIZE];


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
    gamePtr->turn = 1 - gamePtr->turn;
}

Game* startGame(uint8_t gameMode) {
    Player p1 = {0, 0, 0, 0, 0, 0, 0};
    Player p2 = {0, 0, 0, 0, 0, 0, 0};
    Game gameState = {p1, p2, gameMode, 0};
	return *gameState;
}

//reset necessary parameters if game is to be replayed
void continueGame(Game* gamePtr){
    gamePtr->p1->score = 0;
    gamePtr->p1->tot_dist = 0;

    gamePtr->p2->score = 0;
    gamePtr->p2->tot_dist = 0;

    static uint8_t first_turn = 0; //which player gets the first turn alternates on successive rematches
    first_turn = 1 - first_turn;

    game_ptr->turn = first_turn;
}

//determine game mode number (range [-2, 2]) from serial input (range [1, 4])
uint8_t findGameMode(data[BUFFER_SIZE], int size){
    uint8_t gameNumber = str2uint(data[BUFFER_SIZE], int size);

    if(gameNumber > NUM_SINGLE){
        gameNumber = (gameNumber * -1) + NUM_SINGLE;
    }

    return gameNumber;
}

//take str (array of chars) and convert to uint8_t
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
    //Tom to write function that outputs true when the ball is sitting on top of the holder i.e. when LDR is dark
}

bool flex(void){
    //Marco to write function that outputs true if the flex sensor flexes
}

//check whether the game should end or not.
//0 = continue game
//1 = player 1 wins
//2 = player 2 wins
uint8_t endCondition(Game* gamePtr, uint8_t end_conds[4]){
    max_shots = end_conds[0];
    max_score = end_conds[1];
    max_dist = end_conds[2];
    endCond = end_conds[3];

    bool p1_shots = false;
    bool p2_shots = false;
    bool p1_score = false;
    bool p2_score = false;
    bool p1_dist = false;
    bool p2_dist = false;

    bool result = false;

    if(gamePtr->p1->shots >= max_shots){
        p1_shots = true;
    }
    if(gamePtr->p2->shots >= max_shots){
        p2_shots = true;
    }

    if(gamePtr->p1->score >= max_score){
        p1_score = true;
    }
    if(gamePtr->p2->score >= max_score){
        p2_score = true;
    }

    if(gamePtr->p1->tot_dist >= max_dist){
        p1_dist = true;
    }
    if(gamePtr->p2->tot_dist >= max_dist){
        p2_dist = true;
    }

    if(gamePtr->mode == 1){
        result = 1 * p1_shots;
    }

    else if(gamePtr->mode == 2){
        result = 0;
    }

    else if(gamePtr->mode == 3){
        if(endCond == 1){
            result = 1 * p1_shots;
        }

        else if(endCond == 2){
            result = 1 * p1_score;
        }

        else if(endCond == 3){
            result = 1 * p1_dist;
        }

        else{
            result = 3;
        }
    }

    else if(gamePtr->mode == -1){
        result = (1 + (gamePtr->p2->tot_dist > gamePtr->p1->tot_dist))*(p1_dist || p2_dist);
    }

    else if(gamePtr->mode == -2){
        result = (1 + (gamePtr->p2->score > gamePtr->p1->score))*(p1_shots || p2_shots)
    }

    else if(gamePtr->mode == -3){
        if(endCond == 1){
            result = (1 + (gamePtr->p2->score > gamePtr->p1->score))*(p1_shots && p2_shots && (gamePtr->p1->score != gamePtr->p2->score));
        }

        else if(endCond == 2){
            result = (1 + (gamePtr->p2->score > gamePtr->p1->score))*(p1_score || p2_score);
        }

        else if(endCond == 3){
            result = (1 + (gamePtr->p2->tot_dist > gamePtr->p1->tot_dist))*(p1_dist || p2_dist);
        }
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

void askContinue(data[BUFFER_SIZE], int size){
    if(data[0]=='1'){
        askCont = true;
    }
    contSet = true;
}

void gameLoop(uint8_t data[BUFFER_SIZE], int size){
    extern demonstrate = NULL;
    gameMode = findGameMode(data, size);

    Game* gamePtr = startGame(gameMode);

    if(abs(gameMode)==3){
        demonstrate = &set_end_conds;
        SerialOut("R", &USART1_PORT);
        while(!end_conds_set){}
    }

    else{
        end_conds[1] = DEFAULT_SHOTS;
        end_conds[2] = DEFAULT_SCORE;
        end_conds[3] = DEFAULT_DIST;
    }

    if(gameMode == 1){
        end_cond[0] = 1;
    }

    if(gameMode == 2){
        end_cond[0] = 0;
    }

    if(gameMode == -1){
        end_cond[0] = 3;
    }

    if(gameMode == -2){
        end_cond[0] = 
    }

    if(gameMode ==)

    if(gameMode > 0){
        while(true){
            gamePtr->turn = 0;
            if(gameMode == 1){
                setDist(gamePtr, 0);
                end_conds[3] = 1;
            }
            
            else if(gameMode == 2){
                gamePtr->p1->shot_dist = 1;
            }

            while(!LDR()){}

            while(!endCondition(gamePtr, end_conds)){
                while(LDR()){}
                shotTaken(gamePtr);
                while(!LDR()){
                    if(flex()){
                        score(gamePtr);
                    }
                }
            }

            if(endCondition(gamePtr, end_conds)==1){
                SerialOut("W1", &USART1_PORT);
            }

            else if(endCondition(gamePtr, end_conds)==2){
                SerialOut("W2", &USART1_PORT);
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

    if(gameMode < 0){

    }
}