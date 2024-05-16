/*
 * buckets.h
 *
 *  Created on: May 3, 2024
 *      Author: tbray
 */

#ifndef BUCKETS_H_
#define BUCKETS_H_

struct Game;
//typedef struct Game Game;

uint8_t getScore(struct Game* gamePtr);
void score(struct Game* gamePtr, uint8_t points, uint8_t distance);
uint8_t getTime(struct Game* gamePtr);
void setTime(struct Game* gamePtr, uint8_t newTime);
struct Game InitialiseGame();

#endif /* BUCKETS_H_ */
