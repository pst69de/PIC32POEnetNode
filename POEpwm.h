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

#ifdef APP_PWM_OC2_ID
void PWM_SetValues(
    TMR_PRESCALE pwmPreScale
,   int pwmWidth
,   int pwmStart1
,   int pwmStop1
,   int pwmStart2
,   int pwmStop2
);
#else // ifdef APP_PWM_OC2_ID
void PWM_SetValues(
    TMR_PRESCALE pwmPreScale
,   int pwmWidth
,   int pwmStart1
,   int pwmStop1
);
#endif // else APP_PWM_OC2_ID

#endif // ifdef APP_PWM_OC1_ID

#ifdef	__cplusplus
}
#endif

#endif	/* POEADC_H */