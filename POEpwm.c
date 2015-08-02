/* 
 * File:    POEpwm.c
 * Author:  Patrick O. Ehrmann (pst69@pst69.de)
 * License: Creative Commons Zero (https://creativecommons.org/publicdomain/zero/1.0/)
 *
 * Created on 2015-08-01
 * Description:
 *   PWM (Pulse Width Modulation) library
 * Major Changes:
 *   Version 0: alpha development
 */

#include "POEpwm.h"

#ifdef APP_PWM_OC1_ID

#include "peripheral/tmr/plib_tmr.h"
#include "peripheral/oc/plib_oc.h"

void PWM_Initialize(void) {
    // Timer Init
    // Stop the timer
    PLIB_TMR_Stop(APP_PWM_TMR_ID);
    // Disable OC's
    PLIB_OC_Disable(APP_PWM_OC1_ID);
#ifdef APP_PWM_OC2_ID
    PLIB_OC_Disable(APP_PWM_OC2_ID);
#endif // ifdef APP_PWM_OC2_ID
    // Set the prescaler, and set the clock source as internal
    PLIB_TMR_ClockSourceSelect(APP_PWM_TMR_ID, TMR_CLOCK_SOURCE_PERIPHERAL_CLOCK);
    PLIB_TMR_PrescaleSelect(APP_PWM_TMR_ID, APP_PWM_TMR_PRESCALE);
    // Clear the timer
    PLIB_TMR_Counter16BitClear(APP_PWM_TMR_ID);
    // Load the period register
    PLIB_TMR_Period16BitSet(APP_PWM_TMR_ID, APP_PWM_TMR_INIT);
    // OC1 Init
    // port inits
    PLIB_PORTS_PinClear(APP_PWM_OC1_PORTS_ID, APP_PWM_OC1_PORT_CHANNEL, APP_PWM_OC1_PIN);
    PLIB_PORTS_PinDirectionOutputSet(APP_PWM_OC1_PORTS_ID, APP_PWM_OC1_PORT_CHANNEL, APP_PWM_OC1_PIN);
    APP_PWM_OC1_Mode;
    PLIB_PORTS_RemapOutput(APP_PWM_OC1_PORTS_ID, APP_PWM_OC1_Function, APP_PWM_OC1_PPSOut);
    // Select compare mode 
    PLIB_OC_ModeSelect(APP_PWM_OC1_ID, OC_DUAL_COMPARE_CONTINUOUS_PULSE_MODE);
    // Set buffer size to 16-bits
    PLIB_OC_BufferSizeSelect(APP_PWM_OC1_ID, OC_BUFFER_SIZE_16BIT);
    // Set buffer(primary compare) value
    PLIB_OC_Buffer16BitSet(APP_PWM_OC1_ID, APP_PWM_OC1_On);
    // Set pulse width(secondary compare) value
    PLIB_OC_PulseWidth16BitSet(APP_PWM_OC1_ID, APP_PWM_OC1_Off);
#ifdef APP_PWM_OC2_ID
    // OC2 Init
    // port inits
    PLIB_PORTS_PinClear(APP_PWM_OC2_PORTS_ID, APP_PWM_OC2_PORT_CHANNEL, APP_PWM_OC2_PIN);
    PLIB_PORTS_PinDirectionOutputSet(APP_PWM_OC2_PORTS_ID, APP_PWM_OC2_PORT_CHANNEL, APP_PWM_OC2_PIN);
    APP_PWM_OC2_Mode;
    PLIB_PORTS_RemapOutput(APP_PWM_OC2_PORTS_ID, APP_PWM_OC2_Function, APP_PWM_OC2_PPSOut);
    // Select compare mode
    PLIB_OC_ModeSelect(APP_PWM_OC2_ID, OC_DUAL_COMPARE_CONTINUOUS_PULSE_MODE);
    // Set buffer size to 16-bits
    PLIB_OC_BufferSizeSelect(APP_PWM_OC2_ID, OC_BUFFER_SIZE_16BIT);
    // Set buffer(primary compare) value
    PLIB_OC_Buffer16BitSet(APP_PWM_OC2_ID, APP_PWM_OC2_On);
    // Set pulse width(secondary compare) value
    PLIB_OC_PulseWidth16BitSet(APP_PWM_OC2_ID, APP_PWM_OC2_Off);
#endif // ifdef APP_PWM_OC2_ID
}

// StartUp TMR/OC module of MCU
void PWM_StartUp(void) {
    // Enable OC 1
    PLIB_OC_Enable(APP_PWM_OC1_ID);
#ifdef APP_PWM_OC2_ID
    // Enable OC 2
    PLIB_OC_Enable(APP_PWM_OC2_ID);
#endif // ifdef APP_PWM_OC2_ID
    // Start the timer
    PLIB_TMR_Start(APP_PWM_TMR_ID);
}

#ifdef APP_PWM_OC2_ID
void PWM_SetValues(
    TMR_PRESCALE pwmPreScale
,   int pwmWidth
,   int pwmStart1
,   int pwmStop1
,   int pwmStart2
,   int pwmStop2
) {
    // Stop the timer
    PLIB_TMR_Stop(APP_PWM_TMR_ID);
    // Disable OC's
    PLIB_OC_Disable(APP_PWM_OC1_ID);
    PLIB_OC_Disable(APP_PWM_OC2_ID);
    // Set the prescaler, and set the clock source as internal
    PLIB_TMR_PrescaleSelect(APP_PWM_TMR_ID, pwmPreScale);
    // Clear the timer
    PLIB_TMR_Counter16BitClear(APP_PWM_TMR_ID);
    // Load the period register
    PLIB_TMR_Period16BitSet(APP_PWM_TMR_ID, pwmWidth);
    // OC1 Init
    // Set buffer(primary compare) value
    PLIB_OC_Buffer16BitSet(APP_PWM_OC1_ID, pwmStart1);
    // Set pulse width(secondary compare) value
    PLIB_OC_PulseWidth16BitSet(APP_PWM_OC1_ID, pwmStop1);
    // OC2 Init
    // Set buffer(primary compare) value
    PLIB_OC_Buffer16BitSet(APP_PWM_OC2_ID, pwmStart2);
    // Set pulse width(secondary compare) value
    PLIB_OC_PulseWidth16BitSet(APP_PWM_OC2_ID, pwmStop2);
    // Enable OC 1
    PLIB_OC_Enable(APP_PWM_OC1_ID);
    // Enable OC 2
    PLIB_OC_Enable(APP_PWM_OC2_ID);
    // Start the timer
    PLIB_TMR_Start(APP_PWM_TMR_ID);
}

#else // ifdef APP_PWM_OC2_ID
void PWM_SetValues(
    TMR_PRESCALE pwmPreScale
,   int pwmWidth
,   int pwmStart1
,   int pwmStop1
) {
    // Stop the timer
    PLIB_TMR_Stop(APP_PWM_TMR_ID);
    // Disable OC's
    PLIB_OC_Disable(APP_PWM_OC1_ID);
    // Set the prescaler, and set the clock source as internal
    PLIB_TMR_PrescaleSelect(APP_PWM_TMR_ID, pwmPreScale);
    // Clear the timer
    PLIB_TMR_Counter16BitClear(APP_PWM_TMR_ID);
    // Load the period register
    PLIB_TMR_Period16BitSet(APP_PWM_TMR_ID, pwmWidth);
    // OC1 Init
    // Set buffer(primary compare) value
    PLIB_OC_Buffer16BitSet(APP_PWM_OC1_ID, pwmStart1);
    // Set pulse width(secondary compare) value
    PLIB_OC_PulseWidth16BitSet(APP_PWM_OC1_ID, pwmStop1);
    // Enable OC 1
    PLIB_OC_Enable(APP_PWM_OC1_ID);
    // Start the timer
    PLIB_TMR_Start(APP_PWM_TMR_ID);
}

#endif // else APP_PWM_OC2_ID


#endif //ifdef APP_PWM_OC1_ID
