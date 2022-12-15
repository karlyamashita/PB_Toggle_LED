/*
 * PollingRoutine.c
 *
 *  Created on: Dec 15, 2022
 *      Author: karl.yamashita
 */

#include <PollingRoutine.h>
#include "main.h"

extern TimerCallbackStruct timerCallback[MAX_TIMER_CALLBACK]; // already defined in TimerCallback.c

/*
 * Description: Initialize before starting main loop
 *
 */
void PollingInit(void)
{
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET); // Turn On LED

	TimerCallbackRegister(timerCallback, ToggleLED, 50, TIMER_NO_REPEAT); // debounce button for 50ms and set for one shot
	TimerCallbackEnable(timerCallback, ToggleLED, TIMER_DISABLED); // stop timer for now
}

/*
 * Description: main loop
 *
 */
void PollingRoutine(void)
{
	TimerCallbackCheck(timerCallback);
}

/*
 * Description: TimerCallback set to call this function
 *
 */
void ToggleLED(void)
{
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}

/*
 * Description: HAL GPIO interrupt callback
 *
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	uint32_t buttonState;

	if(GPIO_Pin == PB_Pin) // test if PB cause interrupt
	{
		buttonState = HAL_GPIO_ReadPin(PB_GPIO_Port, PB_Pin);

		if(!buttonState) // pressed
		{
			TimerCallbackResetEnable(timerCallback, ToggleLED); // start debounce timer
		}
		else // released
		{
			TimerCallbackEnable(timerCallback, ToggleLED, TIMER_DISABLED); // stop timer
		}
	}
}
