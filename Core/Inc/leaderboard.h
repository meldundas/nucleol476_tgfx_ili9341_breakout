/*
 * leaderboard.h
 *
 *  Created on: Aug 8, 2023
 *      Author: meldu
 */

#ifndef INC_LEADERBOARD_H_
#define INC_LEADERBOARD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

typedef union {
	uint8_t bName[4];
	uint32_t wName;
}Name;

typedef struct {
    uint32_t score;
    Name scoreName;
} Scores;

extern RTC_HandleTypeDef hrtc;  //scores

extern bool newHighScore;
extern uint16_t score;

//1st word 'A', 'B', 'C', 'D'
extern Scores highScores[];



//uint32_t *plocation = (uint32_t *) 0x400002854;
//uint8_t *phighScores = (uint8_t *) highScores;


void initScores();
bool isHighScore(uint16_t score);
void updateLeaderboard(uint8_t* newName);
void showLeaderboard();

#ifdef __cplusplus
}
#endif

#endif /* INC_LEADERBOARD_H_ */
