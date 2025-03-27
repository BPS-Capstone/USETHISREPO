/*
 * pressure_sensor.c
 *
 *  Created on: Mar 23, 2025
 *      Author: Barret Klics
 */

#include "main.h"
extern ADC_HandleTypeDef hadc1;


/*
 * 		//Read ADC
		//1 LSB = Vref+ / 2^n   OR   Vdda/2^n

		float vref = 3.3;
		uint16_t denom = 65535;



	 	float volt = tmp * adc_value;


	 	float recovered = volt * 1.5;

	 	printf("ADC Value =%d\n",adc_value);
	 	printf("Divided Value =%f\n",volt);

	 	printf("Real Value =%f\n",recovered);


 *
 */

#define VREF 3.3
#define WIDTH 65535


uint16_t sample_adc(){
	  HAL_ADC_Start(&hadc1);
	  if(HAL_ADC_PollForConversion(&hadc1, 100)==HAL_OK)
	  {
	 	uint16_t adc_value=HAL_ADC_GetValue(&hadc1);
	 	return adc_value;
	  }
}

float sample_raw_voltage(){
	return VREF/WIDTH * sample_adc();

}

float sample_original_voltage(){
	return VREF/WIDTH * 1.5 * sample_adc();
}

//https://prod-edam.honeywell.com/content/dam/honeywell-edam/sps/siot/en-ca/products/sensors/pressure-sensors/board-mount-pressure-sensors/basic-abp2-series/documents/sps-siot-abp2-series-datasheet-32350268-en.pdf?download=false
//page 40

//Pressure = [(Output - Outputmin) * (Pmax - Pmin)] / (Outputmax - Outputmin) + Pmin

/*
 * Outputmax = output at max pressure (Vdc)
 * Outputmin = output at min pressure (Vdc)
 * Pmax = max value of pressure range
 * Pmin = min value of pressure range
 * Pressure = pressure reading
 * Output = Output voltage (Vdc
 *
 */

/*
 * Our sensor is 'ABP2DRRV015PDAA5XX'
 * ABP2 - Series
 * D - DIP
 * RR- Dual Radial
 * V - Liquid media
 * 015PD - +-15 PSI
 * A - Analog
 * A - Transfer function is 10% to 90% of VSUPP
 * 5 - 5V Vsupp
 * XX - Reserved
 */



#define FILTER_SIZE 64

float readings[FILTER_SIZE];
int index = 0;
float movingAverage = 0.0;


float applyMovingAverage(float newReading) {
    // Subtract the oldest reading from the total
    movingAverage -= readings[index];

    // Add the new reading to the total
    readings[index] = newReading;
    movingAverage += readings[index];

    // Move to the next index in the window, wrap around if needed
    index = (index + 1) % FILTER_SIZE;

    // Return the average
    return movingAverage / FILTER_SIZE;
}

float sample_mmhg(){

	float output = VREF/WIDTH * 1.5 * sample_adc();


	float midpoint = 2.67; //AMBIENT





	int Pmax = 15;
	int Pmin = -15;
	float Outputmax = 0.9 * 5;
	float Outputmin = 0.1 * 5;



	float psi = ((output - Outputmin) * (Pmax - Pmin))/(Outputmax-Outputmin) + Pmin;



	//Room pressure is 760 mmHg

	//65 is correction factor
	float mmhg = psi * 51.7149 - 65;

	return applyMovingAverage(mmhg);

}


