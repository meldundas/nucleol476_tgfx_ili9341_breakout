#include "sound.h"
#include "tim.h"

extern TIM_HandleTypeDef htim8;

void play_tone(uint32_t frequency)
{
    if (frequency == 0) {
        HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
        return;
    }

    uint32_t prescaler = (80000000 / (frequency * 100)) - 1;
    if (prescaler > 65535) {
        prescaler = 65535;
    }

    __HAL_TIM_SET_PRESCALER(&htim8, prescaler);

    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
    HAL_Delay(50); // 50ms duration
    HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
}