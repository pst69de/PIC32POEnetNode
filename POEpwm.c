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

TMR_PRESCALE PWM_PreScale;
int PWM_intPreScale;
int PWM_Cycle;
int PWM_Start1;
int PWM_Stop1;
int PWM_Start2;
int PWM_Stop2;
int PWM_Start3;
int PWM_Stop3;
int PWM_Start4;
int PWM_Stop4;
bool PWM_doInt;
TMR_PRESCALE PWM2_PreScale;
int PWM2_intPreScale;
int PWM2_Cycle;
int PWM2_Start1;
int PWM2_Stop1;
int PWM2_Start2;
int PWM2_Stop2;
bool PWM2_doInt;

void PWM_Initialize(void) {
    // Timer Init
    // Stop the timers
    PLIB_TMR_Stop(APP_PWM_TMR_ID);
#ifdef APP_PWM2_OC1_ID
    PLIB_TMR_Stop(APP_PWM2_TMR_ID);
#endif //ifdef APP_PWM2_OC1_ID
    // Disable OC's
    PLIB_OC_Disable(APP_PWM_OC1_ID);
#ifdef APP_PWM_OC2_ID
    PLIB_OC_Disable(APP_PWM_OC2_ID);
#endif // ifdef APP_PWM_OC2_ID
#ifdef APP_PWM_OC3_ID
    PLIB_OC_Disable(APP_PWM_OC3_ID);
#endif // ifdef APP_PWM_OC3_ID
#ifdef APP_PWM_OC4_ID
    PLIB_OC_Disable(APP_PWM_OC4_ID);
#endif // ifdef APP_PWM_OC4_ID
#ifdef APP_PWM2_OC1_ID
    PLIB_OC_Disable(APP_PWM2_OC1_ID);
#endif // ifdef APP_PWM2_OC1_ID
#ifdef APP_PWM2_OC2_ID
    PLIB_OC_Disable(APP_PWM2_OC2_ID);
#endif // ifdef APP_PWM2_OC2_ID
    // Set the prescaler, and set the clock source as internal
    PLIB_TMR_ClockSourceSelect(APP_PWM_TMR_ID, TMR_CLOCK_SOURCE_PERIPHERAL_CLOCK);
    PLIB_TMR_PrescaleSelect(APP_PWM_TMR_ID, APP_PWM_TMR_PRESCALE);
    PWM_PreScale = APP_PWM_TMR_PRESCALE;
    // Clear the timer
    PLIB_TMR_Counter16BitClear(APP_PWM_TMR_ID);
    // Load the period register
    PLIB_TMR_Period16BitSet(APP_PWM_TMR_ID, APP_PWM_TMR_INIT);
    PWM_Cycle = APP_PWM_TMR_INIT;
#ifdef APP_PWM2_OC1_ID
    PLIB_TMR_ClockSourceSelect(APP_PWM2_TMR_ID, TMR_CLOCK_SOURCE_PERIPHERAL_CLOCK);
    PLIB_TMR_PrescaleSelect(APP_PWM2_TMR_ID, APP_PWM2_TMR_PRESCALE);
    PWM2_PreScale = APP_PWM2_TMR_PRESCALE;
    // Clear the timer
    PLIB_TMR_Counter16BitClear(APP_PWM2_TMR_ID);
    // Load the period register
    PLIB_TMR_Period16BitSet(APP_PWM2_TMR_ID, APP_PWM2_TMR_INIT);
    PWM2_Cycle = APP_PWM2_TMR_INIT;
#endif // ifdef APP_PWM2_OC1_ID
    // OC1 Init
    // port inits
    PLIB_PORTS_PinClear(APP_PWM_OC1_PORTS_ID, APP_PWM_OC1_PORT_CHANNEL, APP_PWM_OC1_PIN);
    PLIB_PORTS_PinDirectionOutputSet(APP_PWM_OC1_PORTS_ID, APP_PWM_OC1_PORT_CHANNEL, APP_PWM_OC1_PIN);
    APP_PWM_OC1_Mode;
    PLIB_PORTS_RemapOutput(APP_PWM_OC1_PORTS_ID, APP_PWM_OC1_Function, APP_PWM_OC1_PPSOut);
    //Select timer base 
    PLIB_OC_TimerSelect(APP_PWM_OC1_ID, APP_PWM_OC_TMR_BASE);
    // Select compare mode 
    PLIB_OC_ModeSelect(APP_PWM_OC1_ID, OC_DUAL_COMPARE_CONTINUOUS_PULSE_MODE);
    // Set buffer size to 16-bits
    PLIB_OC_BufferSizeSelect(APP_PWM_OC1_ID, OC_BUFFER_SIZE_16BIT);
    // Set buffer(primary compare) value
    PLIB_OC_Buffer16BitSet(APP_PWM_OC1_ID, APP_PWM_OC1_On);
    PWM_Start1 = APP_PWM_OC1_On;
    // Set pulse width(secondary compare) value
    PLIB_OC_PulseWidth16BitSet(APP_PWM_OC1_ID, APP_PWM_OC1_Off);
    PWM_Stop1 = APP_PWM_OC1_Off;
#ifdef APP_PWM_OC2_ID
    // OC2 Init
    // port inits
    PLIB_PORTS_PinClear(APP_PWM_OC2_PORTS_ID, APP_PWM_OC2_PORT_CHANNEL, APP_PWM_OC2_PIN);
    PLIB_PORTS_PinDirectionOutputSet(APP_PWM_OC2_PORTS_ID, APP_PWM_OC2_PORT_CHANNEL, APP_PWM_OC2_PIN);
    APP_PWM_OC2_Mode;
    PLIB_PORTS_RemapOutput(APP_PWM_OC2_PORTS_ID, APP_PWM_OC2_Function, APP_PWM_OC2_PPSOut);
    //Select timer base 
    PLIB_OC_TimerSelect(APP_PWM_OC2_ID, APP_PWM_OC_TMR_BASE);
    // Select compare mode
    PLIB_OC_ModeSelect(APP_PWM_OC2_ID, OC_DUAL_COMPARE_CONTINUOUS_PULSE_MODE);
    // Set buffer size to 16-bits
    PLIB_OC_BufferSizeSelect(APP_PWM_OC2_ID, OC_BUFFER_SIZE_16BIT);
    // Set buffer(primary compare) value
    PLIB_OC_Buffer16BitSet(APP_PWM_OC2_ID, APP_PWM_OC2_On);
    PWM_Start2 = APP_PWM_OC2_On;
    // Set pulse width(secondary compare) value
    PLIB_OC_PulseWidth16BitSet(APP_PWM_OC2_ID, APP_PWM_OC2_Off);
    PWM_Stop2 = APP_PWM_OC2_Off;
#endif // ifdef APP_PWM_OC2_ID
#ifdef APP_PWM_OC3_ID
    // OC3 Init
    // port inits
    PLIB_PORTS_PinClear(APP_PWM_OC3_PORTS_ID, APP_PWM_OC3_PORT_CHANNEL, APP_PWM_OC3_PIN);
    PLIB_PORTS_PinDirectionOutputSet(APP_PWM_OC3_PORTS_ID, APP_PWM_OC3_PORT_CHANNEL, APP_PWM_OC3_PIN);
    APP_PWM_OC3_Mode;
    PLIB_PORTS_RemapOutput(APP_PWM_OC3_PORTS_ID, APP_PWM_OC3_Function, APP_PWM_OC3_PPSOut);
    //Select timer base 
    PLIB_OC_TimerSelect(APP_PWM_OC3_ID, APP_PWM_OC_TMR_BASE);
    // Select compare mode
    PLIB_OC_ModeSelect(APP_PWM_OC3_ID, OC_DUAL_COMPARE_CONTINUOUS_PULSE_MODE);
    // Set buffer size to 16-bits
    PLIB_OC_BufferSizeSelect(APP_PWM_OC3_ID, OC_BUFFER_SIZE_16BIT);
    // Set buffer(primary compare) value
    PLIB_OC_Buffer16BitSet(APP_PWM_OC3_ID, APP_PWM_OC3_On);
    PWM_Start3 = APP_PWM_OC3_On;
    // Set pulse width(secondary compare) value
    PLIB_OC_PulseWidth16BitSet(APP_PWM_OC3_ID, APP_PWM_OC3_Off);
    PWM_Stop3 = APP_PWM_OC3_Off;
#endif // ifdef APP_PWM_OC3_ID
#ifdef APP_PWM_OC4_ID
    // OC4 Init
    // port inits
    PLIB_PORTS_PinClear(APP_PWM_OC4_PORTS_ID, APP_PWM_OC4_PORT_CHANNEL, APP_PWM_OC4_PIN);
    PLIB_PORTS_PinDirectionOutputSet(APP_PWM_OC4_PORTS_ID, APP_PWM_OC4_PORT_CHANNEL, APP_PWM_OC4_PIN);
    APP_PWM_OC4_Mode;
    PLIB_PORTS_RemapOutput(APP_PWM_OC4_PORTS_ID, APP_PWM_OC4_Function, APP_PWM_OC4_PPSOut);
    //Select timer base 
    PLIB_OC_TimerSelect(APP_PWM_OC4_ID, APP_PWM_OC_TMR_BASE);
    // Select compare mode
    PLIB_OC_ModeSelect(APP_PWM_OC4_ID, OC_DUAL_COMPARE_CONTINUOUS_PULSE_MODE);
    // Set buffer size to 16-bits
    PLIB_OC_BufferSizeSelect(APP_PWM_OC4_ID, OC_BUFFER_SIZE_16BIT);
    // Set buffer(primary compare) value
    PLIB_OC_Buffer16BitSet(APP_PWM_OC4_ID, APP_PWM_OC4_On);
    PWM_Start4 = APP_PWM_OC4_On;
    // Set pulse width(secondary compare) value
    PLIB_OC_PulseWidth16BitSet(APP_PWM_OC4_ID, APP_PWM_OC4_Off);
    PWM_Stop4 = APP_PWM_OC4_Off;
#endif // ifdef APP_PWM_OC4_ID
#ifdef APP_PWM2_OC1_ID
    // PWM2 / OC1 Init
    // port inits
    PLIB_PORTS_PinClear(APP_PWM2_OC1_PORTS_ID, APP_PWM2_OC1_PORT_CHANNEL, APP_PWM2_OC1_PIN);
    PLIB_PORTS_PinDirectionOutputSet(APP_PWM2_OC1_PORTS_ID, APP_PWM2_OC1_PORT_CHANNEL, APP_PWM2_OC1_PIN);
    APP_PWM2_OC1_Mode;
    PLIB_PORTS_RemapOutput(APP_PWM2_OC1_PORTS_ID, APP_PWM2_OC1_Function, APP_PWM2_OC1_PPSOut);
    //Select timer base 
    PLIB_OC_TimerSelect(APP_PWM2_OC1_ID, APP_PWM2_OC_TMR_BASE);
    // Select compare mode
    PLIB_OC_ModeSelect(APP_PWM2_OC1_ID, OC_DUAL_COMPARE_CONTINUOUS_PULSE_MODE);
    // Set buffer size to 16-bits
    PLIB_OC_BufferSizeSelect(APP_PWM2_OC1_ID, OC_BUFFER_SIZE_16BIT);
    // Set buffer(primary compare) value
    PLIB_OC_Buffer16BitSet(APP_PWM2_OC1_ID, APP_PWM2_OC1_On);
    PWM2_Start1 = APP_PWM2_OC1_On;
    // Set pulse width(secondary compare) value
    PLIB_OC_PulseWidth16BitSet(APP_PWM2_OC1_ID, APP_PWM2_OC1_Off);
    PWM2_Stop1 = APP_PWM2_OC1_Off;
#endif // ifdef APP_PWM2_OC1_ID
#ifdef APP_PWM2_OC2_ID
    // PWM2 / OC2 Init
    // port inits
    PLIB_PORTS_PinClear(APP_PWM2_OC2_PORTS_ID, APP_PWM2_OC2_PORT_CHANNEL, APP_PWM2_OC2_PIN);
    PLIB_PORTS_PinDirectionOutputSet(APP_PWM2_OC2_PORTS_ID, APP_PWM2_OC2_PORT_CHANNEL, APP_PWM2_OC2_PIN);
    APP_PWM2_OC2_Mode;
    PLIB_PORTS_RemapOutput(APP_PWM2_OC2_PORTS_ID, APP_PWM2_OC2_Function, APP_PWM2_OC2_PPSOut);
    //Select timer base 
    PLIB_OC_TimerSelect(APP_PWM2_OC1_ID, APP_PWM2_OC_TMR_BASE);
    // Select compare mode
    PLIB_OC_ModeSelect(APP_PWM2_OC2_ID, OC_DUAL_COMPARE_CONTINUOUS_PULSE_MODE);
    // Set buffer size to 16-bits
    PLIB_OC_BufferSizeSelect(APP_PWM2_OC2_ID, OC_BUFFER_SIZE_16BIT);
    // Set buffer(primary compare) value
    PLIB_OC_Buffer16BitSet(APP_PWM2_OC2_ID, APP_PWM2_OC2_On);
    PWM2_Start2 = APP_PWM2_OC2_On;
    // Set pulse width(secondary compare) value
    PLIB_OC_PulseWidth16BitSet(APP_PWM2_OC2_ID, APP_PWM2_OC2_Off);
    PWM2_Stop2 = APP_PWM2_OC2_Off;
#endif // ifdef APP_PWM2_OC2_ID
}

// StartUp TMR/OC module of MCU
void PWM_StartUp(void) {
    // Enable OC 1
    PLIB_OC_Enable(APP_PWM_OC1_ID);
#ifdef APP_PWM_OC2_ID
    // Enable OC 2
    PLIB_OC_Enable(APP_PWM_OC2_ID);
#endif // ifdef APP_PWM_OC2_ID
#ifdef APP_PWM_OC3_ID
    // Enable OC 3
    PLIB_OC_Enable(APP_PWM_OC3_ID);
#endif // ifdef APP_PWM_OC3_ID
#ifdef APP_PWM_OC4_ID
    // Enable OC 4
    PLIB_OC_Enable(APP_PWM_OC4_ID);
#endif // ifdef APP_PWM_OC4_ID
    // Start the PWM timer
    PLIB_TMR_Start(APP_PWM_TMR_ID);
#ifdef APP_PWM2_OC1_ID
    // Enable PWM2 OC 1
    PLIB_OC_Enable(APP_PWM2_OC1_ID);
#ifdef APP_PWM2_OC2_ID
    // Enable PWM2 OC 2
    PLIB_OC_Enable(APP_PWM2_OC2_ID);
#endif // ifdef APP_PWM2_OC2_ID
    // Start the PWM2 timer
    PLIB_TMR_Start(APP_PWM2_TMR_ID);
#endif // ifdef APP_PWM2_OC1_ID
}

void PWM_Handle_ISR(void) {
    PLIB_TMR_Period16BitSet(APP_PWM_TMR_ID, PWM_Cycle);
    PLIB_OC_Buffer16BitSet(APP_PWM_OC1_ID, PWM_Start1);
    PLIB_OC_PulseWidth16BitSet(APP_PWM_OC1_ID, PWM_Stop1);
#ifdef APP_PWM_OC2_ID
    PLIB_OC_Buffer16BitSet(APP_PWM_OC2_ID, PWM_Start2);
    PLIB_OC_PulseWidth16BitSet(APP_PWM_OC2_ID, PWM_Stop2);
#endif // ifdef APP_PWM_OC2_ID
#ifdef APP_PWM_OC3_ID
    PLIB_OC_Buffer16BitSet(APP_PWM_OC3_ID, PWM_Start3);
    PLIB_OC_PulseWidth16BitSet(APP_PWM_OC3_ID, PWM_Stop3);
#endif // ifdef APP_PWM_OC3_ID
#ifdef APP_PWM_OC4_ID
    PLIB_OC_Buffer16BitSet(APP_PWM_OC4_ID, PWM_Start4);
    PLIB_OC_PulseWidth16BitSet(APP_PWM_OC4_ID, PWM_Stop4);
#endif // ifdef APP_PWM_OC4_ID
}

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
) {
    if ((pwmPreScale == TMR_PRESCALE_VALUE_1) & (pwmCycle < 400)) {
        PWM_doInt = false;
    }
    if ((pwmPreScale != PWM_PreScale) | !PWM_doInt) {
        // total update
        // Disable Interrupt / Clear Flag
        PLIB_INT_SourceDisable(APP_INT_ID, APP_PWM_TMR_INT_SOURCE);
        PLIB_INT_SourceFlagClear(APP_INT_ID, APP_PWM_TMR_INT_SOURCE);
        // Stop the timer
        PLIB_TMR_Stop(APP_PWM_TMR_ID);
        // Disable OC's
        PLIB_OC_Disable(APP_PWM_OC1_ID);
#ifdef APP_PWM_OC2_ID
        PLIB_OC_Disable(APP_PWM_OC2_ID);
#endif // ifdef APP_PWM_OC2_ID
#ifdef APP_PWM_OC3_ID
        PLIB_OC_Disable(APP_PWM_OC3_ID);
#endif // ifdef APP_PWM_OC3_ID
#ifdef APP_PWM_OC4_ID
        PLIB_OC_Disable(APP_PWM_OC4_ID);
#endif // ifdef APP_PWM_OC4_ID
        PWM_PreScale = pwmPreScale;
        PWM_Cycle = pwmCycle;
        PWM_Start1 = pwmStart1;
        PWM_Stop1 = pwmStop1;
        PWM_Start2 = pwmStart2;
        PWM_Stop2 = pwmStop2;
        PWM_Start3 = pwmStart3;
        PWM_Stop3 = pwmStop3;
        PWM_Start4 = pwmStart4;
        PWM_Stop4 = pwmStop4;
        // Set the prescaler, and set the clock source as internal
        PLIB_TMR_PrescaleSelect(APP_PWM_TMR_ID, pwmPreScale);
        // Clear the timer
        PLIB_TMR_Counter16BitClear(APP_PWM_TMR_ID);
        // Load the period register
        PLIB_TMR_Period16BitSet(APP_PWM_TMR_ID, pwmCycle);
        // OC1 Init
        // Set buffer(primary compare) value
        PLIB_OC_Buffer16BitSet(APP_PWM_OC1_ID, pwmStart1);
        // Set pulse width(secondary compare) value
        PLIB_OC_PulseWidth16BitSet(APP_PWM_OC1_ID, pwmStop1);
#ifdef APP_PWM_OC2_ID
        // OC2 Init
        // Set buffer(primary compare) value
        PLIB_OC_Buffer16BitSet(APP_PWM_OC2_ID, pwmStart2);
        // Set pulse width(secondary compare) value
        PLIB_OC_PulseWidth16BitSet(APP_PWM_OC2_ID, pwmStop2);
#endif // ifdef APP_PWM_OC2_ID
#ifdef APP_PWM_OC3_ID
        // OC3 Init
        // Set buffer(primary compare) value
        PLIB_OC_Buffer16BitSet(APP_PWM_OC3_ID, pwmStart3);
        // Set pulse width(secondary compare) value
        PLIB_OC_PulseWidth16BitSet(APP_PWM_OC3_ID, pwmStop3);
#endif // ifdef APP_PWM_OC3_ID
#ifdef APP_PWM_OC4_ID
        // OC4 Init
        // Set buffer(primary compare) value
        PLIB_OC_Buffer16BitSet(APP_PWM_OC4_ID, pwmStart4);
        // Set pulse width(secondary compare) value
        PLIB_OC_PulseWidth16BitSet(APP_PWM_OC4_ID, pwmStop4);
#endif // ifdef APP_PWM_OC4_ID
        // Enable OC 1
        PLIB_OC_Enable(APP_PWM_OC1_ID);
#ifdef APP_PWM_OC2_ID
        // Enable OC 2
        PLIB_OC_Enable(APP_PWM_OC2_ID);
#endif // ifdef APP_PWM_OC2_ID
#ifdef APP_PWM_OC3_ID
        // Enable OC 3
        PLIB_OC_Enable(APP_PWM_OC3_ID);
#endif // ifdef APP_PWM_OC3_ID
#ifdef APP_PWM_OC4_ID
        // Enable OC 4
        PLIB_OC_Enable(APP_PWM_OC4_ID);
#endif // ifdef APP_PWM_OC4_ID
        if ((PWM_PreScale == TMR_PRESCALE_VALUE_1) & (PWM_Cycle < 400)) {
            PWM_doInt = false;
        } else {
            PWM_doInt = true;
            // Reenable Interrupt
            PLIB_INT_SourceEnable(APP_INT_ID, APP_PWM_TMR_INT_SOURCE);
        }
        // Start the timer
        PLIB_TMR_Start(APP_PWM_TMR_ID);
    } else {
        // continuos update
        PWM_Cycle = pwmCycle;
        PWM_Start1 = pwmStart1;
        PWM_Stop1 = pwmStop1;
        PWM_Start2 = pwmStart2;
        PWM_Stop2 = pwmStop2;
        PWM_Start3 = pwmStart3;
        PWM_Stop3 = pwmStop3;
        PWM_Start4 = pwmStart4;
        PWM_Stop4 = pwmStop4;
    } 
}


bool PWM_Calculate(
    float pwmFrequency
,   float pwmWidth
,   float pwmPhase
,   float pwmWidth2
,   float pwmPhase2
,   float pwmWidth3
,   float pwmPhase3
,   float pwmWidth4
) {
    TMR_PRESCALE PWMPreScale;
    int PWMintPreScale;
    int PWMCycle;
    int PWMStart1;
    int PWMStop1;
    int PWMStart2;
    int PWMStop2;
    int PWMStart3;
    int PWMStop3;
    int PWMStart4;
    int PWMStop4;
    int m;
    // set PWM only if needed
    bool newPWM    = false;
    PWMPreScale    = PWM_PreScale;
    PWMintPreScale = PWM_intPreScale;
    m = (int)(APP_PBCLK_FREQ / (PWMintPreScale * pwmFrequency));
    if ((65535 < m) | (400 > m)){
        // Calculation of PreScale frequency * maxwidth must be greater than prescaled timer clock
        if (65535 > (APP_PBCLK_FREQ / pwmFrequency)) {
            // TMR_PRESCALE_VALUE_1 = 0x00,
            if (PWMPreScale != TMR_PRESCALE_VALUE_1) { 
                newPWM = true;
                PWMPreScale = TMR_PRESCALE_VALUE_1;
                PWMintPreScale = 1;
            }
        }
        else if (65535 > (APP_PBCLK_FREQ / (2 * pwmFrequency))) {
            // TMR_PRESCALE_VALUE_2 = 0x01,
            if (PWMPreScale != TMR_PRESCALE_VALUE_2) { 
                newPWM = true;
                PWMPreScale = TMR_PRESCALE_VALUE_2;
                PWMintPreScale = 2;
            }
        }
        else if (65535 > (APP_PBCLK_FREQ / (4 * pwmFrequency))) {
            // TMR_PRESCALE_VALUE_4 = 0x02,
            if (PWMPreScale != TMR_PRESCALE_VALUE_4) { 
                newPWM = true;
                PWMPreScale = TMR_PRESCALE_VALUE_4;
                PWMintPreScale = 4;
            }
        }
        else if (65535 > (APP_PBCLK_FREQ / (8 * pwmFrequency))) {
            // TMR_PRESCALE_VALUE_8 = 0x03,
            if (PWMPreScale != TMR_PRESCALE_VALUE_8) { 
                newPWM = true;
                PWMPreScale = TMR_PRESCALE_VALUE_8;
                PWMintPreScale = 8;
            }
        }
        else if (65535 > (APP_PBCLK_FREQ / (16 * pwmFrequency))) {
            // TMR_PRESCALE_VALUE_16 = 0x04,
            if (PWMPreScale != TMR_PRESCALE_VALUE_16) { 
                newPWM = true;
                PWMPreScale = TMR_PRESCALE_VALUE_16;
                PWMintPreScale = 16;
            }
        }
        else if (65535 > (APP_PBCLK_FREQ / (32 * pwmFrequency))) {
            // TMR_PRESCALE_VALUE_32 = 0x05,
            if (PWMPreScale != TMR_PRESCALE_VALUE_32) { 
                newPWM = true;
                PWMPreScale = TMR_PRESCALE_VALUE_32;
                PWMintPreScale = 32;
            }
        }
        else if (65535 > (APP_PBCLK_FREQ / (64 * pwmFrequency))) {
            // TMR_PRESCALE_VALUE_64 = 0x06,
            if (PWMPreScale != TMR_PRESCALE_VALUE_64) { 
                newPWM = true;
                PWMPreScale = TMR_PRESCALE_VALUE_64;
                PWMintPreScale = 64;
            }
        }
        else {
            // TMR_PRESCALE_VALUE_256 = 0x07 (lowest)
            if (PWMPreScale != TMR_PRESCALE_VALUE_256) { 
                newPWM = true;
                PWMPreScale = TMR_PRESCALE_VALUE_256;
                PWMintPreScale = 256;
            }
        }
    }
    // Calculation of Timer Width
    PWMCycle = PWM_Cycle;
    m = (int)(APP_PBCLK_FREQ / (PWMintPreScale * pwmFrequency));
    if (PWMCycle != m) {
        newPWM = true;
        PWMCycle = m;
    }
    // POETODO: if any of PWM_Phase1, PWM_Phase1+PWM_Width1, PWM_Phase2, PWM_Phase2+PWM_Width2 > 100% then the MCU gets an invalid value
    // solution may be based on shifting all values around the corner, not yet implemented
    // up to 50%, = Phase%, over 50%, = 100% - Phase% (Channels swapped ???)
    // Calculation of Start1
    PWMStart1 = PWM_Start1;
    m = (int)((PWMCycle * 0.0f / 100.0f) + 1);
    if (PWMStart1 != m) {
        newPWM = true;
        PWMStart1 = m;
    }
    // Calculation of Stop1
    PWMStop1 = PWM_Stop1;
    m = (int)((PWMCycle * (0.0f + pwmWidth) / 100.0f) + 1);
    if (PWMStop1 != m) {
        newPWM = true;
        PWMStop1 = m;
    }
    // Calculation of Start2
    PWMStart2 = PWM_Start2;
    m = (int)((PWMCycle * pwmPhase / 100.0f) + 1);
    if (PWMStart2 != m) {
        newPWM = true;
        PWMStart2 = m;
    }
    // Calculation of Stop2
    PWMStop2 = PWM_Stop2;
    m = (int)((PWMCycle * (pwmPhase + pwmWidth2) / 100.0f) + 1);
    if (PWMStop2 != m) {
        newPWM = true;
        PWMStop2 = m;
    }
    // Calculation of Start3
    PWMStart3 = PWM_Start3;
    m = (int)((PWMCycle * pwmPhase2 / 100.0f) + 1);
    if (PWMStart3 != m) {
        newPWM = true;
        PWMStart3 = m;
    }
    // Calculation of Stop3
    PWMStop3 = PWM_Stop3;
    m = (int)((PWMCycle * (pwmPhase2 + pwmWidth3) / 100.0f) + 1);
    if (PWMStop3 != m) {
        newPWM = true;
        PWMStop3 = m;
    }
    // Calculation of Start4
    PWMStart4 = PWM_Start4;
    m = (int)((PWMCycle * pwmPhase3 / 100.0f) + 1);
    if (PWMStart4 != m) {
        newPWM = true;
        PWMStart4 = m;
    }
    // Calculation of Stop4
    PWMStop4 = PWM_Stop4;
    m = (int)((PWMCycle * (pwmPhase3 + pwmWidth4) / 100.0f) + 1);
    if (PWMStop4 != m) {
        newPWM = true;
        PWMStop4 = m;
    }
    // Set new timing if needed
    if (newPWM) {
        PWM_SetValues( 
              PWMPreScale
            , PWMCycle
            , PWMStart1
            , PWMStop1
            , PWMStart2
            , PWMStop2
            , PWMStart3
            , PWMStop3
            , PWMStart4
            , PWMStop4
            );
    }
    return newPWM;
}


#ifdef APP_PWM2_OC1_ID
void PWM2_Handle_ISR(void) {
    PLIB_TMR_Period16BitSet(APP_PWM2_TMR_ID, PWM2_Cycle);
    PLIB_OC_Buffer16BitSet(APP_PWM2_OC1_ID, PWM2_Start1);
    PLIB_OC_PulseWidth16BitSet(APP_PWM2_OC1_ID, PWM2_Stop1);
#ifdef APP_PWM2_OC2_ID
    PLIB_OC_Buffer16BitSet(APP_PWM2_OC2_ID, PWM2_Start2);
    PLIB_OC_PulseWidth16BitSet(APP_PWM2_OC2_ID, PWM2_Stop2);
#endif // ifdef APP_PWM2_OC2_ID
}

void PWM2_SetValues(
    TMR_PRESCALE pwmPreScale
,   int pwmCycle
,   int pwmStart1
,   int pwmStop1
,   int pwmStart2
,   int pwmStop2
) {
    if ((pwmPreScale == TMR_PRESCALE_VALUE_1) & (pwmCycle < 400)) {
        PWM2_doInt = false;
    }
    if ((pwmPreScale != PWM2_PreScale) | !PWM2_doInt) {
        // total update
        // Disable Interrupt / Clear Flag
        PLIB_INT_SourceDisable(APP_INT_ID, APP_PWM2_TMR_INT_SOURCE);
        PLIB_INT_SourceFlagClear(APP_INT_ID, APP_PWM2_TMR_INT_SOURCE);
        // Stop the timer
        PLIB_TMR_Stop(APP_PWM2_TMR_ID);
        // Disable OC's
        PLIB_OC_Disable(APP_PWM2_OC1_ID);
#ifdef APP_PWM2_OC2_ID
        PLIB_OC_Disable(APP_PWM2_OC2_ID);
#endif // ifdef APP_PWM2_OC2_ID
        PWM2_PreScale = pwmPreScale;
        PWM2_Cycle = pwmCycle;
        PWM2_Start1 = pwmStart1;
        PWM2_Stop1 = pwmStop1;
        PWM2_Start2 = pwmStart2;
        PWM2_Stop2 = pwmStop2;
        // Set the prescaler, and set the clock source as internal
        PLIB_TMR_PrescaleSelect(APP_PWM2_TMR_ID, pwmPreScale);
        // Clear the timer
        PLIB_TMR_Counter16BitClear(APP_PWM2_TMR_ID);
        // Load the period register
        PLIB_TMR_Period16BitSet(APP_PWM2_TMR_ID, pwmCycle);
        // OC1 Init
        // Set buffer(primary compare) value
        PLIB_OC_Buffer16BitSet(APP_PWM2_OC1_ID, pwmStart1);
        // Set pulse width(secondary compare) value
        PLIB_OC_PulseWidth16BitSet(APP_PWM2_OC1_ID, pwmStop1);
#ifdef APP_PWM2_OC2_ID
        // OC2 Init
        // Set buffer(primary compare) value
        PLIB_OC_Buffer16BitSet(APP_PWM2_OC2_ID, pwmStart2);
        // Set pulse width(secondary compare) value
        PLIB_OC_PulseWidth16BitSet(APP_PWM2_OC2_ID, pwmStop2);
#endif // ifdef APP_PWM2_OC2_ID
        // Enable OC 1
        PLIB_OC_Enable(APP_PWM2_OC1_ID);
#ifdef APP_PWM2_OC2_ID
        // Enable OC 2
        PLIB_OC_Enable(APP_PWM2_OC2_ID);
#endif // ifdef APP_PWM2_OC2_ID
        if ((PWM2_PreScale == TMR_PRESCALE_VALUE_1) & (PWM2_Cycle < 400)) {
            PWM2_doInt = false;
        } else {
            PWM2_doInt = true;
            // Reenable Interrupt
            PLIB_INT_SourceEnable(APP_INT_ID, APP_PWM2_TMR_INT_SOURCE);
        }
        // Start the timer
        PLIB_TMR_Start(APP_PWM2_TMR_ID);
    } else {
        // continuos update
        PWM2_Cycle = pwmCycle;
        PWM2_Start1 = pwmStart1;
        PWM2_Stop1 = pwmStop1;
        PWM2_Start2 = pwmStart2;
        PWM2_Stop2 = pwmStop2;
    } 
}

bool PWM2_Calculate(
    float pwmFrequency
,   float pwmWidth
,   float pwmPhase
,   float pwmWidth2
) {
    TMR_PRESCALE PWMPreScale;
    int PWMintPreScale;
    int PWMCycle;
    int PWMStart1;
    int PWMStop1;
    int PWMStart2;
    int PWMStop2;
    int m;
    // set PWM only if needed
    bool newPWM    = false;
    PWMPreScale    = PWM2_PreScale;
    PWMintPreScale = PWM2_intPreScale;
    m = (int)(APP_PBCLK_FREQ / (PWMintPreScale * pwmFrequency));
    if ((65535 < m) | (400 > m)){
        // Calculation of PreScale frequency * maxwidth must be greater than prescaled timer clock
        if (65535 > (APP_PBCLK_FREQ / pwmFrequency)) {
            // TMR_PRESCALE_VALUE_1 = 0x00,
            if (PWMPreScale != TMR_PRESCALE_VALUE_1) { 
                newPWM = true;
                PWMPreScale = TMR_PRESCALE_VALUE_1;
                PWMintPreScale = 1;
            }
        }
        else if (65535 > (APP_PBCLK_FREQ / (2 * pwmFrequency))) {
            // TMR_PRESCALE_VALUE_2 = 0x01,
            if (PWMPreScale != TMR_PRESCALE_VALUE_2) { 
                newPWM = true;
                PWMPreScale = TMR_PRESCALE_VALUE_2;
                PWMintPreScale = 2;
            }
        }
        else if (65535 > (APP_PBCLK_FREQ / (4 * pwmFrequency))) {
            // TMR_PRESCALE_VALUE_4 = 0x02,
            if (PWMPreScale != TMR_PRESCALE_VALUE_4) { 
                newPWM = true;
                PWMPreScale = TMR_PRESCALE_VALUE_4;
                PWMintPreScale = 4;
            }
        }
        else if (65535 > (APP_PBCLK_FREQ / (8 * pwmFrequency))) {
            // TMR_PRESCALE_VALUE_8 = 0x03,
            if (PWMPreScale != TMR_PRESCALE_VALUE_8) { 
                newPWM = true;
                PWMPreScale = TMR_PRESCALE_VALUE_8;
                PWMintPreScale = 8;
            }
        }
        else if (65535 > (APP_PBCLK_FREQ / (16 * pwmFrequency))) {
            // TMR_PRESCALE_VALUE_16 = 0x04,
            if (PWMPreScale != TMR_PRESCALE_VALUE_16) { 
                newPWM = true;
                PWMPreScale = TMR_PRESCALE_VALUE_16;
                PWMintPreScale = 16;
            }
        }
        else if (65535 > (APP_PBCLK_FREQ / (32 * pwmFrequency))) {
            // TMR_PRESCALE_VALUE_32 = 0x05,
            if (PWMPreScale != TMR_PRESCALE_VALUE_32) { 
                newPWM = true;
                PWMPreScale = TMR_PRESCALE_VALUE_32;
                PWMintPreScale = 32;
            }
        }
        else if (65535 > (APP_PBCLK_FREQ / (64 * pwmFrequency))) {
            // TMR_PRESCALE_VALUE_64 = 0x06,
            if (PWMPreScale != TMR_PRESCALE_VALUE_64) { 
                newPWM = true;
                PWMPreScale = TMR_PRESCALE_VALUE_64;
                PWMintPreScale = 64;
            }
        }
        else {
            // TMR_PRESCALE_VALUE_256 = 0x07 (lowest)
            if (PWMPreScale != TMR_PRESCALE_VALUE_256) { 
                newPWM = true;
                PWMPreScale = TMR_PRESCALE_VALUE_256;
                PWMintPreScale = 256;
            }
        }
    }
    // Calculation of Timer Width
    PWMCycle = PWM2_Cycle;
    m = (int)(APP_PBCLK_FREQ / (PWMintPreScale * pwmFrequency));
    if (PWMCycle != m) {
        newPWM = true;
        PWMCycle = m;
    }
    // POETODO: if any of PWM_Phase1, PWM_Phase1+PWM_Width1, PWM_Phase2, PWM_Phase2+PWM_Width2 > 100% then the MCU gets an invalid value
    // solution may be based on shifting all values around the corner, not yet implemented
    // up to 50%, = Phase%, over 50%, = 100% - Phase% (Channels swapped ???)
    // Calculation of Start1
    PWMStart1 = PWM2_Start1;
    m = (int)((PWMCycle * 0.0f / 100.0f) + 1);
    if (PWMStart1 != m) {
        newPWM = true;
        PWMStart1 = m;
    }
    // Calculation of Stop1
    PWMStop1 = PWM2_Stop1;
    m = (int)((PWMCycle * (0.0f + pwmWidth) / 100.0f) + 1);
    if (PWMStop1 != m) {
        newPWM = true;
        PWMStop1 = m;
    }
    // Calculation of Start2
    PWMStart2 = PWM2_Start2;
    m = (int)((PWMCycle * pwmPhase / 100.0f) + 1);
    if (PWMStart2 != m) {
        newPWM = true;
        PWMStart2 = m;
    }
    // Calculation of Stop2
    PWMStop2 = PWM2_Stop2;
    m = (int)((PWMCycle * (pwmPhase + pwmWidth2) / 100.0f) + 1);
    if (PWMStop2 != m) {
        newPWM = true;
        PWMStop2 = m;
    }
    // Set new timing if needed
    if (newPWM) {
        PWM2_SetValues( 
              PWMPreScale
            , PWMCycle
            , PWMStart1
            , PWMStop1
            , PWMStart2
            , PWMStop2
            );
    }
    return newPWM;
}

#endif //ifdef APP_PWM2_OC1_ID

#endif //ifdef APP_PWM_OC1_ID
