/* 
 * File:    POEpwm.h
 * Author:  Patrick O. Ehrmann (pst69@pst69.de)
 * License: Creative Commons Zero (https://creativecommons.org/publicdomain/zero/1.0/)
 *
 * Created on 2015-08-01
 * Description:
 *   PWM (Pulse Width Modulation) library
 *   This library uses one timer (single frequency) with two output compare's to do phase shifting modes
 * Major Changes:
 *   Version 0: alpha development
 */

#ifndef POEPWM_H
#define	POEPWM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "system_config.h"

#ifdef APP_PWM_OC1_ID

// Initialize TMR/OC module of MCU
void PWM_Initialize(void);

// StartUp TMR/OC module of MCU
void PWM_StartUp(void);

void PWM_Handle_ISR(void);

void PWM_SetValues(
    TMR_PRESCALE pwmPreScale
,   int pwmCycle
,   int pwmStart1
,   int pwmStop1
,   int pwmStart2
,   int pwmStop2
,   int pwmStart3
,   int pwmStop3
,   int pwmStart4
,   int pwmStop4
);

bool PWM_Calculate(
    float pwmFrequency
,   float pwmWidth
,   float pwmPhase
,   float pwmWidth2
,   float pwmPhase2
,   float pwmWidth3
,   float pwmPhase3
,   float pwmWidth4
);

#ifdef APP_PWM2_OC1_ID

void PWM2_Handle_ISR(void);

void PWM2_SetValues(
    TMR_PRESCALE pwmPreScale
,   int pwmCycle
,   int pwmStart1
,   int pwmStop1
,   int pwmStart2
,   int pwmStop2
);

bool PWM2_Calculate(
    float pwmFrequency
,   float pwmWidth
,   float pwmPhase
,   float pwmWidth2
);

#endif // ifdef APP_PWM2_OC1_ID
#endif // ifdef APP_PWM_OC1_ID

#ifdef	__cplusplus
}
#endif

#endif	/* POEADC_H */
