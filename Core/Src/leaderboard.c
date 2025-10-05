/*
 * leaderboard.c
 *
 *  Created on: Aug 8, 2023
 *      Author: meldu
 */


#include "main.h"
#include "leaderboard.h"
#include <stdbool.h>
#include "structs.h"
//#include <gui/screen1_screen/Screen1View.hpp>
#include <string.h>

extern GameState gameState;

bool newHighScore = false;
bool updatedScore = false;

//uint16_t score = 0;

//1st word 'A', 'B', 'C', 'D'
Scores highScores[6]; //sort and show 5

// Function to swap elements
void swap_scores(Scores *a, Scores *b)
{
    Scores temp = *a;
    *a = *b;
    *b = temp;
}

// bubble sort function
void bubbleSort(Scores array[], int n)
{
    int i, j;
    for (i = 0; i < n-1; i++)
    {
        for (j = 0; j < n-i-1; j++)
        {
            if (array[j].score < array[j+1].score)
            {
                swap_scores(&array[j], &array[j+1]);
            }
        }
    }
}


void initScores()
{

	  if( RTC->BKP0R != 0x61626364) //leaderboard header does not exist
	  {								//generate leaderboard

	  	  highScores[0].score = 0;
	  	  highScores[0].scoreName.wName = 0x61616100;

	  	  highScores[1].score = 0;
	  	  highScores[1].scoreName.wName = 0x61616100;

	  	  highScores[2].score = 0;
	  	  highScores[2].scoreName.wName = 0x61616100;

	  	  highScores[3].score = 0;
	  	  highScores[3].scoreName.wName = 0x61616100;

	  	  highScores[4].score = 0;
	  	  highScores[4].scoreName.bName[0] = 0;
	  	  highScores[4].scoreName.bName[1] = 'a';
	  	  highScores[4].scoreName.bName[2] = 'a';
	  	  highScores[4].scoreName.bName[3] = 'a';

	  	  //backup domain access
		  HAL_PWR_EnableBkUpAccess();  //2. Set the DBP bit in the PWR power control register (PWR_CR) to enable access to the backup domain.
		  RTC->WPR = 0xCA;
		  RTC->WPR = 0x53;

		  RTC->BKP0R = 0x61626364;	//setup leaderboard header


	  	  RTC->BKP1R = highScores[0].score;
	  	  RTC->BKP2R = highScores[0].scoreName.wName;

	  	  RTC->BKP3R = highScores[1].score;
	  	  RTC->BKP4R = highScores[1].scoreName.wName;

	  	  RTC->BKP5R = highScores[2].score;
	  	  RTC->BKP6R = highScores[2].scoreName.wName;

	  	  RTC->BKP7R = highScores[3].score;
	  	  RTC->BKP8R = highScores[3].scoreName.wName;

	  	  RTC->BKP9R = highScores[4].score;
	  	  RTC->BKP10R = highScores[4].scoreName.wName;


	  }
	  else
	  {
	  	  highScores[0].score = RTC->BKP1R;
	  	  highScores[0].scoreName.wName = RTC->BKP2R;

	  	  highScores[1].score = RTC->BKP3R;
	  	  highScores[1].scoreName.wName = RTC->BKP4R;

	  	  highScores[2].score = RTC->BKP5R;
	  	  highScores[2].scoreName.wName = RTC->BKP6R;

	  	  highScores[3].score = RTC->BKP7R;
	  	  highScores[3].scoreName.wName = RTC->BKP8R;

	  	  highScores[4].score =  RTC->BKP9R;
	  	  highScores[4].scoreName.wName =  RTC->BKP10R;
	  }



}

bool isHighScore(uint16_t score)
{
    if(score > highScores[4].score)
    {
        return true;
    }
    return false;
}

void updateLeaderboard(uint8_t* newName)
{
    highScores[5].score = gameState.score;	//add latest score to bottom of leaderboard
    highScores[5].scoreName.bName[0] = newName[0];
    highScores[5].scoreName.bName[1] = newName[1];
    highScores[5].scoreName.bName[2] = newName[2];
    highScores[5].scoreName.bName[3] = newName[3];

    bubbleSort(highScores, 6);		//sort leaderboard

                                //store new leaderboard
    RTC->BKP1R = highScores[0].score;
    RTC->BKP2R = highScores[0].scoreName.wName;

    RTC->BKP3R = highScores[1].score;
    RTC->BKP4R = highScores[1].scoreName.wName;

    RTC->BKP5R = highScores[2].score;
    RTC->BKP6R = highScores[2].scoreName.wName;

    RTC->BKP7R = highScores[3].score;
    RTC->BKP8R = highScores[3].scoreName.wName;

    RTC->BKP9R = highScores[4].score;
    RTC->BKP10R = highScores[4].scoreName.wName;
}



void showLeaderboard()
{

}
