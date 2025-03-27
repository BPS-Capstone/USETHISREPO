/*
 * menu.c
 *
 *  Created on: Mar 21, 2025
 *      Author: Barret Klics
 */


#include <stdio.h>
#include "waveform.h"
#include "stepper.h"
#include "main.h"
#include "control_system.h"
#include "pressure_sensor.h"

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim14;
void menu(){



	while(1){
		int selection = -1;

		printf("BPS MENU\n");
		printf("------------\n");
		printf("1 - Ping\n");
		printf("2 - Heartbeat\n");


		printf("3 - Pressurize\n");



		printf("4 - Open Valve\n");
		printf("5 - Close Valven");

		printf("6 - Enter Control System\n");

		printf("7 - Stop Pump\n");


		printf("8 - Start pump 1\n");

		printf("9 - Start pump 0\n");


		char buf[64];
		while (copyReadBuffer(buf) == -1);

		sscanf(buf, "%d", &selection);

		printf("You selected: %d\n", selection);


		if (selection == 1){
			printf("pong\n");
		}

		/*
		else if (selection ==2){
			printf("entering heartbeat\n");
			heartbeatt();
		}*/

		else if (selection ==2){
			init_stepper();
			HAL_GPIO_WritePin(STPR_DIR_GPIO_Port, STPR_DIR_Pin, 1); //blow

		}

		else if (selection ==3){
			pressurize();
			printf("done!\n");





			HAL_GPIO_WritePin(STPR_SLEEP_GPIO_Port, STPR_SLEEP_Pin, 0);
		}



		else if (selection ==4){
			HAL_GPIO_WritePin(VALVE_12V_GPIO_Port,VALVE_12V_Pin, 0);

		}


		else if (selection ==5){
			HAL_GPIO_WritePin(VALVE_12V_GPIO_Port,VALVE_12V_Pin, 1);

		}


		else if (selection ==6){
			control_system();
		}



		else if (selection ==7){
			//__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 0);
			__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 0);
			//HAL_GPIO_TogglePin(STPR_SLEEP_GPIO_Port, STPR_SLEEP_Pin);
			//__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 0);
		}
		else if (selection ==8){
			//HAL_GPIO_WritePin(BRUSHLESS_DIR_GPIO_Port, BRUSHLESS_DIR_Pin, 0);
			HAL_GPIO_WritePin(BRUSHLESS_DIR_GPIO_Port, BRUSHLESS_DIR_Pin, 1);
			__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 600);

			//__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 1200);


		}

		else if (selection ==9){

			HAL_GPIO_WritePin(BRUSHLESS_DIR_GPIO_Port, BRUSHLESS_DIR_Pin, 1);
			__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 2400);









		//	init_stepper();
		//	HAL_TIM_Base_Start_IT(&htim16);// OLD STEPPER START

		//	HAL_GPIO_WritePin(STPR_DIR_GPIO_Port, STPR_DIR_Pin,0);
			//__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 65535);
		}


		selection = -1;


	}

}









