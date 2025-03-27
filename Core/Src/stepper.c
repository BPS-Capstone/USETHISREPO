/*
 * stepper.c
 *
 *  Created on: Mar 21, 2025
 *      Author: Barret Klics
 */


#include <stdio.h>
#include "main.h"



void init_stepper(){

	HAL_GPIO_WritePin(STPR_SLEEP_GPIO_Port, STPR_SLEEP_Pin, 1);
	HAL_GPIO_WritePin(STPR_RST_GPIO_Port, STPR_RST_Pin, 1);
	HAL_GPIO_WritePin(EN24V_GPIO_Port, EN24V_Pin, GPIO_PIN_SET);


	return;

}



void set_motor_direction(int dir){
	HAL_GPIO_WritePin(STPR_DIR_GPIO_Port, STPR_DIR_Pin, 1);
	return;
}



void testStep(int steps){


	for (int i = 0; i < steps; i ++){


		//printf("Step %d/%d \n", i, steps);
		HAL_GPIO_WritePin(STPR_STEP_GPIO_Port, STPR_STEP_Pin, 1);
		HAL_Delay(1);
		HAL_GPIO_WritePin(STPR_STEP_GPIO_Port, STPR_STEP_Pin, 0);
		HAL_Delay(1);

	}

}
