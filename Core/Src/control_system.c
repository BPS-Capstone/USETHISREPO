/*
 * control_system.c
 *
 *  Created on: Mar 23, 2025
 *      Author: Barret Klics
 *
 */
#include "main.h"
#include "waveform.h"
#include <stdio.h>
#include "pressure_sensor.h"
extern TIM_HandleTypeDef htim14;

uint32_t period_start_time = 0;
uint32_t period_ms = 600;
uint32_t period_count = 0;

float get_period_percentage(uint32_t period_ms);



float min = 0;
float lastmin = 0;
float peakmin = 0;


// PID controller variables
double previous_error = 0.0;
double integral = 0.0;
double pwm_output = 0.0;
#define PWM_MIN 0.0f    // Minimum PWM output (0%)
#define PWM_MAX 1.0f    // Maximum PWM output (100%)



void pressurize(){

	HAL_GPIO_WritePin(VALVE_12V_GPIO_Port,VALVE_12V_Pin, 1);
	HAL_GPIO_WritePin(BRUSHLESS_DIR_GPIO_Port, BRUSHLESS_DIR_Pin,0);
	__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 600);

	while (sample_mmhg() < 80);

	__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 0);

	HAL_GPIO_WritePin(VALVE_12V_GPIO_Port,VALVE_12V_Pin, 0);
}



void control_system(){


	char buf[64];


	uint16_t i = 0;


	int selection = -1;
	uint16_t speed = 0;


	//MAX SPEED IS 2400;
	uint32_t last_time = 0;

	uint8_t action_flag = 0;
	//HAL_GPIO_WritePin(BRUSHLESS_DIR_GPIO_Port, BRUSHLESS_DIR_Pin,0);
	//__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 600);











	float maxBP = 0;
	int maxBPtime = 0;


	float maxSens = 0;
	int maxSenstime= 0;

	int peakdelay = 0;

	int upstroke = 0;

	generate_sample_buffer();
	uint32_t last_period = period_count;



	HAL_GPIO_WritePin(VALVE_12V_GPIO_Port,VALVE_12V_Pin, 2400);


	int suck = 0;
	//pressurize();
	while (1){


		if (period_count > last_period){


			if (peakmin > min) peakmin = min;
			last_period = period_count;
			lastmin = min;

		}





		if(copyReadBuffer(buf) != -1) break;



		float tmp = sample_mmhg();

		if (tmp < min) min = tmp;

		//else if (min > -150) min = tmp;


		float percentage = 0;
		percentage = get_period_percentage(period_ms);
		int index = percentage * 1000;

		//printf("index: %d", index);

		float bp = get_heart_sample(index);


		/*

		if (tmp <= -50){ //-50 and 50 is 180 and 88
			upstroke = 0;
			HAL_GPIO_WritePin(VALVE_12V_GPIO_Port,VALVE_12V_Pin, 0);
		}
		if (tmp > 50){
			HAL_GPIO_WritePin(VALVE_12V_GPIO_Port,VALVE_12V_Pin, 1);
			upstroke = 1;

		}
*/


		//if (min >  peakmin +20){
		//	HAL_GPIO_WritePin(VALVE_12V_GPIO_Port,VALVE_12V_Pin, 0);

		//}

		if (percentage < 0.4){
			HAL_GPIO_WritePin(VALVE_12V_GPIO_Port,VALVE_12V_Pin, 1);
		}

		else {

			HAL_GPIO_WritePin(VALVE_12V_GPIO_Port,VALVE_12V_Pin, 0);




		}

		//else{ //-50 and 50 is 180 and 88
	//		upstroke = 0;
	//		HAL_GPIO_WritePin(VALVE_12V_GPIO_Port,VALVE_12V_Pin, 0);
	//		__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 2000);
			//__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 1200);
//		}

//*/



			//Needs to upstroke
			if (upstroke){


				HAL_GPIO_WritePin(STPR_DIR_GPIO_Port, STPR_DIR_Pin, 0); //suck

					//HAL_GPIO_WritePin(VALVE_12V_GPIO_Port,VALVE_12V_Pin, 1);
					//HAL_GPIO_WritePin(BRUSHLESS_DIR_GPIO_Port, BRUSHLESS_DIR_Pin,0);
				//	__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 2400);


			}
			//needs to downstroke
			else{

			//	HAL_GPIO_WritePin(STPR_DIR_GPIO_Port, STPR_DIR_Pin, 1); //blow

				//HAL_GPIO_WritePin(VALVE_12V_GPIO_Port,VALVE_12V_Pin, 0);

			//	HAL_GPIO_WritePin(BRUSHLESS_DIR_GPIO_Port, BRUSHLESS_DIR_Pin,1);
			//	__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 2400);
			}



		//if (tmp > 120){
		//	HAL_GPIO_WritePin(BRUSHLESS_DIR_GPIO_Port, BRUSHLESS_DIR_Pin,1);
		//	__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1,0);
		//}

		//Needs to upstroke
		/*
		if (percentage < 0.2){
				//HAL_GPIO_WritePin(VALVE_12V_GPIO_Port,VALVE_12V_Pin, 1);
				HAL_GPIO_WritePin(BRUSHLESS_DIR_GPIO_Port, BRUSHLESS_DIR_Pin,0);
				__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 1200);


		}
		//needs to downstroke
		else{
			//HAL_GPIO_WritePin(VALVE_12V_GPIO_Port,VALVE_12V_Pin, 0);

			HAL_GPIO_WritePin(BRUSHLESS_DIR_GPIO_Port, BRUSHLESS_DIR_Pin,1);
			__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 0);
		}


		//HAL_GPIO_WritePin(BRUSHLESS_DIR_GPIO_Port, BRUSHLESS_DIR_Pin,0);
		//__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 2400);






		*/



		/*
		if (tmp < -200){ //full tilt{

			suck = 0;

		}
		else if (tmp > -10){
			suck = 1;

			//open valve
		}

		if (suck){
			HAL_GPIO_WritePin(VALVE_12V_GPIO_Port,VALVE_12V_Pin, 1);
			HAL_GPIO_WritePin(BRUSHLESS_DIR_GPIO_Port, BRUSHLESS_DIR_Pin, 1);
			__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 2400);

		}

		else{
			HAL_GPIO_WritePin(VALVE_12V_GPIO_Port,VALVE_12V_Pin, 0);

			HAL_GPIO_WritePin(BRUSHLESS_DIR_GPIO_Port, BRUSHLESS_DIR_Pin, 0);
			__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 2400);
		}

	*/
		//float tmp = sample_original_voltage();
		printf("BP %.2f\n", bp);
		printf("PSENS %.2f\n", tmp);
		//printf("TEMP %.2f\n", lastmin);
	//	printf("HR %.2f\n", peakmin);
	}
	//__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 0);




	while(1){
		float tmp = sample_mmhg();
		printf("PSENS %.2f\n", tmp);

	}
}



// Get percentage of period completion (auto-rollover)
float get_period_percentage(uint32_t period_ms)
{
    uint32_t current_time = get_time_ms();
    uint32_t elapsed_time = current_time - period_start_time;

    // Roll over automatically if period is complete
    if (elapsed_time >= period_ms)
    {
        period_start_time += period_ms;  // Shift period start forward
        elapsed_time -= period_ms;       // Subtract period to get remaining time
        period_count++;
    }

    // Calculate and return percentage
    return ((float)elapsed_time / (float)period_ms);
}

