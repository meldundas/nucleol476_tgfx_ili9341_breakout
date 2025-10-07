/*
 * structs.h
 *
 *  Created on: Oct 4, 2025
 *      Author: mdundas
 */

#ifndef INC_STRUCTS_H_
#define INC_STRUCTS_H_

// Game state
typedef struct GameState
{
    int lives;
    int score;
    int ballX;
    int ballY;
    int ballSpeedX;
    int ballSpeedY;
    int paddleX;
    bool newHighScore;
    bool gameOver;
    bool gameWin;
    int visibleBricks;
    int level;
} GameState;

#endif /* INC_STRUCTS_H_ */
