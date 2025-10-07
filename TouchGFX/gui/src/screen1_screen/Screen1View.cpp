#include <gui/screen1_screen/Screen1View.hpp>
#include <string.h>
#include <stdlib.h> //rand
#include "main.h"
#include "leaderboard.h"
#include "structs.h"

// Screen dimension constants
const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;

// Ball constants
const int BALL_SIZE = 15;
const int BALL_CENTER = BALL_SIZE / 2;

// Paddle constants
const int PADDLE_WIDTH = 61;
const int PADDLE_HEIGHT = 15;
const int PADDLE_SPEED = 5; //was 5 7

// Brick constants
const int BRICK_WIDTH = 30;
const int BRICK_HEIGHT = 17;
const int NUM_BRICK_ROWS = 4;
const int NUM_BRICK_COLS = 9;
const int TOTAL_BRICKS = NUM_BRICK_ROWS * NUM_BRICK_COLS;

// Game constants
const int INITIAL_LIVES = 3;
const int MAX_BALL_SPEED = 8;
const int MIN_BALL_SPEED = 2;

struct Brick
{
    touchgfx::TiledImage* brick;
    int score;
};

struct Level
{
    bool layout[TOTAL_BRICKS];
    int visibleBricks;
};

const Level levels[] = {
	// Add 36 'true' or 'false' values here for your brick layout

    // Level 1
    {
        {
            true, true, true, true, true, true, true, true, true,
            true, true, true, true, true, true, true, true, true,
            true, true, true, true, true, true, true, true, true,
            true, true, true, true, true, true, true, true, true,
        },
        36 // The number of 'true' values in the layout above
    },
    // Level 2
    {
        {
            true, true, true, true, true, true, true, true, true,
            true, true, true, false, false, false, true, true, true,
            true, true, false, true, false, true, false, true, true,
            true, false, true, false, true, false, true, false, true,
        },
        26
    },
	// Level 3
	{
	    {
			true, true, true, true, true, true, true, true, true,
			true, false, true, false, true, false, true, false, true,
			true, true, true, true, true, true, true, true, true,
			false, true, false, true, false, true, false, true, false,
	    },
	    27
	},
	// Level 4
	{
	    {
			true, true, true, true, true, true, true, true, true,
			true, false, true, false, true, false, true, false, true,
			true, false, true, false, true, false, true, false, true,
			true, false, false, false, false, false, false, false, true,
	    },
	    21
	}


};
const int NUM_LEVELS = sizeof(levels) / sizeof(Level);

bool checkCollision(const touchgfx::TiledImage& brick);

GameState gameState;
Brick bricks[TOTAL_BRICKS];
uint8_t newName[5] = "AAAA";
Unicode::UnicodeChar buffer[20];
static int selectedChar = 0;

//game level
int LEVEL = 1;
bool levelStarted = false;


Screen1View::Screen1View()
{

}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();

    srand(12345); // Seed the random number generator

    gameState.lives = INITIAL_LIVES;
    gameState.score = 0;
    gameState.ballX = SCREEN_WIDTH / 2;
    gameState.ballY = SCREEN_HEIGHT / 2;
    // gameState.ballSpeedX = MIN_BALL_SPEED;
    // gameState.ballSpeedY = MIN_BALL_SPEED;
    gameState.paddleX = (SCREEN_WIDTH - PADDLE_WIDTH) / 2;
    gameState.newHighScore = false;
    gameState.gameOver = false;
    gameState.gameWin = false;

    Unicode::snprintf(livesTextAreaBuffer, LIVESTEXTAREA_SIZE, "%d", gameState.lives);
    Unicode::snprintf(scoreTextAreaBuffer, SCORETEXTAREA_SIZE, "%d", gameState.score);

    // Initialize bricks
    bricks[0] = {&y8, 10};
    bricks[1] = {&y7, 10};
    bricks[2] = {&y6, 10};
    bricks[3] = {&y5, 10};
    bricks[4] = {&y4, 10};
    bricks[5] = {&y3, 10};
    bricks[6] = {&y2, 10};
    bricks[7] = {&y1, 10};
    bricks[8] = {&y0, 10};

    bricks[9] = {&r8, 20};
    bricks[10] = {&r7, 20};
    bricks[11] = {&r6, 20};
    bricks[12] = {&r5, 20};
    bricks[13] = {&r4, 20};
    bricks[14] = {&r3, 20};
    bricks[15] = {&r2, 20};
    bricks[16] = {&r1, 20};
    bricks[17] = {&r0, 20};

    bricks[18] = {&g8, 30};
    bricks[19] = {&g7, 30};
    bricks[20] = {&g6, 30};
    bricks[21] = {&g5, 30};
    bricks[22] = {&g4, 30};
    bricks[23] = {&g3, 30};
    bricks[24] = {&g2, 30};
    bricks[25] = {&g1, 30};
    bricks[26] = {&g0, 30};

    bricks[27] = {&b8, 50};
    bricks[28] = {&b7, 50};
    bricks[29] = {&b6, 50};
    bricks[30] = {&b5, 50};
    bricks[31] = {&b4, 50};
    bricks[32] = {&b3, 50};
    bricks[33] = {&b2, 50};
    bricks[34] = {&b1, 50};
    bricks[35] = {&b0, 50};

    loadLevel(1);

    gameOver.setVisible(false);
    youWin.setVisible(false);
    enterName.setVisible(false);

    hs0.setVisible(false);
    hs1.setVisible(false);
    hs2.setVisible(false);
    hs3.setVisible(false);
    hs4.setVisible(false);
    hs0n.setVisible(false);
    hs1n.setVisible(false);
    hs2n.setVisible(false);
    hs3n.setVisible(false);
    hs4n.setVisible(false);

    ball.invalidate();
    paddle.invalidate();
    livesTextArea.invalidate();
    scoreTextArea.invalidate();
    newName[4] = 0;
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::loadLevel(int levelNumber)
{
    if (levelNumber < 1 || levelNumber > NUM_LEVELS)
    {
        return;
    }

    const Level& level = levels[levelNumber - 1];

    gameState.level = levelNumber;
    LEVEL = levelNumber;
    Unicode::snprintf(levelTextAreaBuffer, LEVELTEXTAREA_SIZE, "%d", gameState.level);
    levelTextArea.invalidate();
    gameState.visibleBricks = level.visibleBricks;

    for (int i = 0; i < TOTAL_BRICKS; ++i)
    {
        bricks[i].brick->setVisible(level.layout[i]);
        bricks[i].brick->invalidate();
    }

    // Reset ball and paddle
    gameState.paddleX = (SCREEN_WIDTH - PADDLE_WIDTH) / 2;
    gameState.ballX = gameState.paddleX + (PADDLE_WIDTH / 2) - BALL_CENTER;
    gameState.ballY = paddle.getY() - BALL_SIZE;
    ball.setX(gameState.ballX);
    ball.setY(gameState.ballY);
    paddle.setX(gameState.paddleX);
    ball.invalidate();
    paddle.invalidate();

    int current_min_speed = MIN_BALL_SPEED + (levelNumber - 1);
    if (current_min_speed > MAX_BALL_SPEED)
    {
        current_min_speed = MAX_BALL_SPEED;
    }
    gameState.ballSpeedX = current_min_speed;
    gameState.ballSpeedY = current_min_speed;

    levelStarted = false;
}

void Screen1View::newJoystickValue(Position newValue)
{
    static Position lastValue = {50, 50};

    if (enterName.isVisible())
    {
        // Horizontal movement (select character)
        if (newValue.y > 55 && lastValue.y <= 55) // Left
        {
            selectedChar--;
            if (selectedChar < 0)
                selectedChar = 3;
        }
        else if (newValue.y < 45 && lastValue.y >= 45) // Right
        {
            selectedChar++;
            if (selectedChar > 3)
                selectedChar = 0;
        }

        // Vertical movement (change character)
        if (newValue.x < 20 && lastValue.x >= 20) // Up
        {
            newName[selectedChar]++;
            if (newName[selectedChar] > 'Z')
                newName[selectedChar] = 'A';
        }
        else if (newValue.x > 80 && lastValue.x <= 80) // Down
        {
            newName[selectedChar]--;
            if (newName[selectedChar] < 'A')
                newName[selectedChar] = 'Z';
        }

        lastValue = newValue;

        Unicode::strncpy(enterNameBuffer, (const char*)newName, ENTERNAME_SIZE);
        enterName.invalidate();

        if (HAL_GPIO_ReadPin(JOYSTICK_SW_GPIO_Port, JOYSTICK_SW_Pin) == GPIO_PIN_RESET) // Joystick button pressed
        {
            updateLeaderboard(newName);
            enterName.setVisible(false);

            // update leaderboard scores display
            displayLeaderboard();
        }


    }
    else //if (!gameState.gameOver && !gameState.gameWin)
    {
        if (!levelStarted)
        {
            if (HAL_GPIO_ReadPin(JOYSTICK_SW_GPIO_Port, JOYSTICK_SW_Pin) == GPIO_PIN_RESET)
            {
                levelStarted = true;
            }
        }
        paddle.invalidate(); // Invalidate old paddle position
        // move paddle
        if (newValue.y > 60) // left
        {
            gameState.paddleX -= PADDLE_SPEED;
        }

        if (newValue.y < 40) // right
        {
            gameState.paddleX += PADDLE_SPEED;
        }

        if (gameState.paddleX < 0)
            gameState.paddleX = 0;
        if (gameState.paddleX + PADDLE_WIDTH > SCREEN_WIDTH)
            gameState.paddleX = SCREEN_WIDTH - PADDLE_WIDTH;

        paddle.setX(gameState.paddleX);
        paddle.invalidate(); // Invalidate new paddle position
    }
}

void Screen1View::moveBall()
{
    if (gameState.gameOver || gameState.gameWin)
    {
        return;
    }

    if (!levelStarted)
    {
        // Ball follows the paddle
        ball.invalidate();
        gameState.ballX = gameState.paddleX + (PADDLE_WIDTH / 2) - BALL_CENTER;
        ball.setX(gameState.ballX);
        ball.invalidate();
        return;
    }

    ball.invalidate(); // Invalidate old ball position

    gameState.ballX += gameState.ballSpeedX;
    gameState.ballY += gameState.ballSpeedY;

    // Wall collision
    if (gameState.ballX > SCREEN_WIDTH - BALL_SIZE || gameState.ballX < 0)
    {
        gameState.ballSpeedX *= -1;
    }

    if (gameState.ballY < 0)
    {
        gameState.ballSpeedY *= -1;
    }

    // Bottom of screen
    if (gameState.ballY > SCREEN_HEIGHT - BALL_SIZE)
    {
        gameState.lives--;
        Unicode::snprintf(livesTextAreaBuffer, LIVESTEXTAREA_SIZE, "%d", gameState.lives);
        livesTextArea.invalidate();


        if (gameState.lives == 0)
        {
            gameState.gameOver = true;
            gameOver.setVisible(true);
            gameOver.invalidate();

            if (isHighScore(gameState.score) == true)
            {
                enterName.setVisible(true);
                enterName.invalidate();
            }
            else
            {
                // update leaderboard scores display
            	displayLeaderboard();


            }
        }
        else
        {
            gameState.ballX = SCREEN_WIDTH / 2;
            gameState.ballY = SCREEN_HEIGHT / 2;
        }
    }

    // Paddle collision
    bool collision_handled = false;

    // Top collision
    int prevBallY = gameState.ballY - gameState.ballSpeedY;
    if (gameState.ballSpeedY > 0 && // Check if the ball is moving downwards
        prevBallY + BALL_SIZE <= paddle.getY() &&
        gameState.ballY + BALL_SIZE > paddle.getY() &&
        gameState.ballX + BALL_CENTER >= gameState.paddleX &&
        gameState.ballX + BALL_CENTER <= gameState.paddleX + PADDLE_WIDTH)
    {
        gameState.ballSpeedY *= -1;
        gameState.ballY = paddle.getY() - BALL_SIZE; // Move ball to the top of the paddle
        collision_handled = true;
    }

    // Side collision
    if (!collision_handled &&
        (gameState.ballY + BALL_SIZE > paddle.getY() && gameState.ballY < paddle.getY() + PADDLE_HEIGHT))
    {
        int prevBallX = gameState.ballX - gameState.ballSpeedX;

        // Left side
        if (gameState.ballSpeedX > 0 && // moving right
            prevBallX + BALL_SIZE <= gameState.paddleX &&
            gameState.ballX + BALL_SIZE > gameState.paddleX)
        {
            gameState.ballSpeedX *= -1;
            collision_handled = true;
        }

        // Right side
        if (!collision_handled &&
            gameState.ballSpeedX < 0 && // moving left
            prevBallX >= gameState.paddleX + PADDLE_WIDTH &&
            gameState.ballX < gameState.paddleX + PADDLE_WIDTH)
        {
            gameState.ballSpeedX *= -1;
            collision_handled = true;
        }
    }

    // Brick collision
    for (int i = 0; i < TOTAL_BRICKS; ++i)
    {
        if (bricks[i].brick->isVisible())
        {
            if (checkCollision(*bricks[i].brick))
            {
                bricks[i].brick->setVisible(false);
                bricks[i].brick->invalidate();
                gameState.score += bricks[i].score * LEVEL;
                gameState.ballSpeedY *= -1;
                gameState.visibleBricks--;

                if (gameState.visibleBricks == 0)
                {
                    if(LEVEL < NUM_LEVELS)
                    {
                        loadLevel(LEVEL + 1);
                    }
                    else
                    {
                        gameState.gameWin = true;
                        if (isHighScore(gameState.score) == true)
                        {
                            enterName.setVisible(true);
                            enterName.invalidate();
                        }
                        else
                        {
                            youWin.setVisible(true);
                        }
                    }
                }
            }
        }
    }

    Unicode::snprintf(scoreTextAreaBuffer, SCORETEXTAREA_SIZE, "%d", gameState.score);
    scoreTextArea.invalidate();

    ball.setX(gameState.ballX);
    ball.setY(gameState.ballY);

    ball.invalidate(); // Invalidate new ball position
}

bool checkCollision(const touchgfx::TiledImage& brick)
{
    return (gameState.ballX < brick.getX() + BRICK_WIDTH &&
            gameState.ballX + BALL_SIZE > brick.getX() &&
            gameState.ballY < brick.getY() + BRICK_HEIGHT &&
            gameState.ballY + BALL_SIZE > brick.getY());
}


void Screen1View::displayLeaderboard()
{
	//update leaderboard scores display
    Unicode::snprintf(hs0Buffer, HS0_SIZE, "%d", highScores[0].score);
    Unicode::snprintf(hs1Buffer, HS1_SIZE, "%d", highScores[1].score);
    Unicode::snprintf(hs2Buffer, HS2_SIZE, "%d", highScores[2].score);
    Unicode::snprintf(hs3Buffer, HS3_SIZE, "%d", highScores[3].score);
    Unicode::snprintf(hs4Buffer, HS4_SIZE, "%d", highScores[4].score);

    // update leaderboard names display

	Unicode::strncpy(hs0nBuffer, (const char*)highScores[0].scoreName.bName, HS0N_SIZE-1);
	Unicode::strncpy(hs1nBuffer, (const char*)highScores[1].scoreName.bName, HS1N_SIZE-1);
	Unicode::strncpy(hs2nBuffer, (const char*)highScores[2].scoreName.bName, HS2N_SIZE-1);
	Unicode::strncpy(hs3nBuffer, (const char*)highScores[3].scoreName.bName, HS3N_SIZE-1);
	Unicode::strncpy(hs4nBuffer, (const char*)highScores[4].scoreName.bName, HS4N_SIZE-1);


    // display leaderboard
    hs0.setVisible(true);
    hs1.setVisible(true);
    hs2.setVisible(true);
    hs3.setVisible(true);
    hs4.setVisible(true);

    hs0n.setVisible(true);
    hs1n.setVisible(true);
    hs2n.setVisible(true);
    hs3n.setVisible(true);
    hs4n.setVisible(true);

    hs0.invalidate();
    hs1.invalidate();
    hs2.invalidate();
    hs3.invalidate();
    hs4.invalidate();

    hs0n.invalidate();
    hs1n.invalidate();
    hs2n.invalidate();
    hs3n.invalidate();
    hs4n.invalidate();
}
