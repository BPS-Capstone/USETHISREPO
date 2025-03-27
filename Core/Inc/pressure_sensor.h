/*
 * pressure_sensor.h
 *
 *  Created on: Mar 23, 2025
 *      Author: Barret Klics
 */

#ifndef INC_PRESSURE_SENSOR_H_
#define INC_PRESSURE_SENSOR_H_




uint16_t sample_adc();

float sample_raw_voltage();

float sample_original_voltage();

float sample_mmhg();



#endif /* INC_PRESSURE_SENSOR_H_ */
