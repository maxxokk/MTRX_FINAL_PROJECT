//#include <stdint.h>
//#include <stdlib.h>
//#include <buckets.h>
//#include <timer.h>
//
//struct Game {
//	uint8_t score;
//	uint8_t distRecord;
//	uint8_t time;
//	uint8_t mode;
//};
//
//uint8_t getScore(struct Game* gamePtr) {
//	return gamePtr->score;
//}
//
//void score(struct Game* gamePtr, uint8_t points, uint8_t distance) {
//	gamePtr->score += points;
//	if (distance > gamePtr->distRecord) {
//		gamePtr->distRecord = distance;
//	}
//}
//uint8_t getTime(struct Game* gamePtr) {
//	return gamePtr->time;
//}
//void setTime(struct Game* gamePtr, uint8_t newTime) {
//	gamePtr->time = newTime;
//}
//
//struct Game* InitialiseGame(uint8_t gameTime, uint8_t gameMode) {
//	struct Game* newGame = malloc(sizeof(Game));
//	newGame->score = 0b00000000;
//	newGame->distRecord = 0b00000000;
//	newGame->time = gameTime;
//	newGame->mode = gameMode;
//	return newGame;
//}
