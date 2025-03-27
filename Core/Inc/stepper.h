/*
 * stepper.h
 *
 *  Created on: Mar 21, 2025
 *      Author: Barret Klics
 */

#ifndef SRC_STEPPER_H_
#define SRC_STEPPER_H_

void init_stepper();
void set_motor_direction(int dir);
void testStep(int steps);
#endif /* SRC_STEPPER_H_ */
