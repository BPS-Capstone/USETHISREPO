/*
 * waveform.h
 *
 *  Created on: Mar 16, 2025
 *      Author: Barret Klics
 */

#ifndef INC_WAVEFORM_H_
#define INC_WAVEFORM_H_


float heartbeat();
void generate_sample_buffer();
float get_heart_sample(int index);
float get_heart_dt(int index);

#endif /* INC_WAVEFORM_H_ */
