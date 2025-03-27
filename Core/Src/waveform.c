/*
 * waveform.c
 *
 *  Created on: Mar 16, 2025
 *      Author: Barret Klics
 */

#include <stdio.h>
#include "math.h"
#include "main.h"


#define HEARTBEAT_PERIOD 1.0  // seconds per beat
#define SAMPLE_RATE 1000      // samples per second



/* DEPRECATED
double bloodPressure(double t) {
    double t_mod = fmod(t, HEARTBEAT_PERIOD);
    double P_dias = 80.0;   // Diastolic pressure (mmHg)
    double P_sys  = 120.0;  // Systolic pressure (mmHg)
    double pressure;

    if (t_mod < 0.2) {
        // Rapid upstroke from diastolic to systolic
        double fraction = t_mod / 0.2;
        pressure = P_dias + (P_sys - P_dias) * fraction;
    } else if (t_mod < 0.3) {
        // Plateau at systolic pressure
        pressure = P_sys;
    } else if (t_mod < 0.5) {
        // Dicrotic notch: a slight drop of 10 mmHg
        double fraction = (t_mod - 0.3) / 0.2;
        pressure = P_sys - 10 * fraction;
    } else {
        // Gradual decline from (P_sys - 10) back to diastolic pressure
        double fraction = (t_mod - 0.5) / (HEARTBEAT_PERIOD - 0.5);
        pressure = (P_sys - 10) - ((P_sys - 10) - P_dias) * fraction;
    }

    return pressure;
} */

/*  DEPRECATED
double bloodPressure2(double t) {
    double T1 = 0.1;  // Upstroke period (smooth rise from diastolic to systolic)
    double T2 = 0.1;  // Plateau at systolic pressure
    double T3 = 0.1;  // Dicrotic notch period (smooth dip)
    double T4 = 0.7;  // Decline period back to diastolic pressure


    double P_dias = 80.0;   // Diastolic pressure (mmHg)
    double P_sys  = 120.0;  // Systolic pressure (mmHg)
    double notch_depth = 10.0;  // Maximum drop in pressure during the notch (mmHg)

    // Use modulo to get the time within the current heartbeat cycle.
    double t_mod = fmod(t, HEARTBEAT_PERIOD);
    double pressure = 0.0;

    if (t_mod < T1) {
        // Upstroke: smooth sine interpolation from diastolic to systolic.
        // sin(0) = 0 and sin(pi/2) = 1, producing a smooth rise.
        double norm = t_mod / T1; // Normalized time [0,1]
        pressure = P_dias + (P_sys - P_dias) * sin(norm * (M_PI / 2));
    } else if (t_mod < T1 + T2) {
        // Plateau: maintain systolic pressure.
        pressure = P_sys;
    } else if (t_mod < T1 + T2 + T3) {
        // Dicrotic notch: smooth dip using a cosine function.
        // The function produces a smooth transition from systolic pressure,
        // dipping by "notch_depth" (scaled by cosine), and then transitioning to the decline phase.
        double norm = (t_mod - (T1 + T2)) / T3;  // Normalized time [0,1]
        pressure = P_sys - notch_depth * (1 - cos(M_PI * norm)) / 2;
    } else {
        // Decline: smooth decline from the end of the notch back to diastolic.
        // At t = T1+T2+T3, pressure is P_sys - notch_depth.
        double norm = (t_mod - (T1 + T2 + T3)) / T4;  // Normalized time [0,1]
        pressure = (P_sys - notch_depth) - ((P_sys - notch_depth) - P_dias) * sin(norm * (M_PI / 2));
    }

    return pressure;
}*/

#define HEART_BUF_SIZE 1000
float heartBuf[HEART_BUF_SIZE];
float heartBuf_dt[HEART_BUF_SIZE];


//https://pmc.ncbi.nlm.nih.gov/articles/PMC10942507/
double bloodPressure3(double t) {



	//0.1 to 0.2 is rise from diasolic phase endpoint to  systolic phase peak. (exponential?)   (AKA SYSTOLIC PHASE DURATION - SYSTOLIC DECAY PHASE)

	//0.2 to o.4 is "Systolic Decay Phase" which drops from systolic phase peak to dicrotic notch (AKA SYSTOLIC DECAY PHASE)

	//0.4 to 0.5 is "DICROTIC NOTCH TO DIASTOLIC PHASE PEAK"                        |
	//0.5 to 1 is Diastolic phase peak to DIASTOLIC PHASE END POINT (AKA DIASTOLIC PHASE DURATION)


	//double T0 = 0;
    double T1 = 0.13;  // Time that peak systolic pressure occurs
    double T2 = 0.35;  // time that notch occurs
    double T3 = 0.43;  // time that diastolic max occurs
    double T4 = 1;  // time that finish


    double t_sys_start = 0;
    double t_sys_peak = 0.13;
	double t_notch =0.35;
	double t_dia_peak = 0.43;
	double t_dia_end = 1.0;



    double p_dia_min = 80.0;
    double p_dia_peak = 101.0;
    double p_sys_peak  = 120.0;
    double p_notch = 99;


    double fc=2*M_PI;

    double dT = 0;
    double mid = 0;
    // Use modulo to get the time within the current heartbeat cycle.
    double t_mod = fmod(t, HEARTBEAT_PERIOD);
    double pressure = 0.0;
    double x = 0;
    x = t_mod;

    if (x <= T1) {
    	mid = (p_dia_min+p_sys_peak)/2;
    	dT = 1/(t_sys_peak-t_sys_start);
        pressure = (p_sys_peak-mid) * cos((0.5*dT) * fc * (x - t_sys_start) - M_PI) + mid;

    } else if ((T1 < x) && (x <= T2)) {
        mid = (p_sys_peak+p_notch)/2;
        dT = 1/(t_notch-t_sys_peak);
        pressure = (p_sys_peak-mid) * cos((0.5*dT) * fc * (x - t_sys_peak)- 0) + mid;

    } else if ((T2 < x) && (x <= T3)) {
        mid = (p_notch+p_dia_peak)/2;
        dT = 1/(t_dia_peak-t_notch);
        pressure = (p_dia_peak-mid) * cos((0.5*dT) * fc * (x - t_notch) - M_PI) + mid;
    }
    else if ((T3 < x) && (x <= T4)) {
        mid = (p_dia_peak + p_dia_min)/2;
        dT = 1/(t_dia_end -t_dia_peak);
        pressure = (p_dia_peak-mid) * cos((0.5*dT) * fc * (x - t_dia_peak) - 0) + mid;
    }
    return pressure;
}






void generate_sample_buffer() //double t_start, double t_step
{
    for (int i = 0; i < HEART_BUF_SIZE; i++)
    {
        double t = 0 + i * 0.001;
        heartBuf[i] = bloodPressure3(t);

        printf("Generated %d: %f\n", i, heartBuf[i] );

    }

    for (int i = i; i < HEART_BUF_SIZE; i++)
    {
        heartBuf_dt[i] = heartBuf[i] - heartBuf[i-1];
        heartBuf_dt[0] = heartBuf[1];
    }


}

float get_heart_sample(int index){
	//printf("returning hear sample %f from index %d\n", heartBuf[index], index);
	if (index < 0) return heartBuf[HEART_BUF_SIZE - index];
	return heartBuf[index];

}

float get_heart_dt(int index){
	return heartBuf_dt[index];

}




float heartbeat(){
	 unsigned long sample = 0;
	    // Continuous loop: each iteration represents one sample period.
	while (1) {
		double t = sample / (double)SAMPLE_RATE;
		double bp = bloodPressure3(t);

		// Output the blood pressure value over UART (one value per line)
		printf("BP %.2f\n", bp);
		printf("PSENS %.2f\n", 70+fmod(t, HEARTBEAT_PERIOD)*50);
		sample++;
		HAL_Delay(1);  // Delay for 1 millisecond to maintain 1000 Hz sample rate
	}

	return 0;
}
