/* 
 * File:    app.h
 * Author:  Patrick O. Ehrmann (pst69@pst69.de)
 * License: Creative Commons Zero (https://creativecommons.org/publicdomain/zero/1.0/)
 *
 * Created on 2015-02-26
 * Description:
 *   not yet supplied
 * Major Changes:
 *   Version 0: alpha development
 */

#ifndef APP_H
#define	APP_H

#ifdef	__cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
    
// Application States
typedef enum {
    // Application?s initial state
    APP_STATE_INIT,
    // POE.net node initialization
    APP_STATE_POENET_INIT,
    // Init LCD
    APP_STATE_LCD_INIT,
#ifdef APP_USE_UART
    // Init UART
    APP_STATE_UART_INIT,
#endif // of ifdef APP_USE_UART
    // after init's send reset
    APP_STATE_POENET_RESET,
    // POE.net input phase
    APP_STATE_POENET_INPUT,
    // POE.net input interpretation
    APP_STATE_POENET_INPUT_READY,
    // POE.net command interpretation
    APP_STATE_POENET_COMMAND,
    // POE.net output phase
    APP_STATE_POENET_OUTPUT_PREPARE,
    // POE.net output phase finished
    APP_STATE_POENET_OUTPUT_READY,
#ifdef APP_POEnet_SECONDARY
    // POE.net pass secondary
    APP_STATE_POENET_PASS,
#endif // ifdef APP_POEnet_SECONDARY
    // poll data
    APP_STATE_POLL_DATA,
#ifdef APP_USE_ADC
    // Start a ADC read
    APP_STATE_START_ADC,
    // ADC wait on Sample
    APP_STATE_SAMPLE_ADC,
    // Convert initiated ADC read
    APP_STATE_CONVERT_ADC,
    // wait on ADC conversion
    APP_STATE_READ_ADC,
#endif // ifdef APP_USE_ADC
    // LCD update
    APP_LCD_UPDATE,
    // (not used) Register timer callback
    APP_STATE_REGISTER_TMR,
    // (not used) Check Time
    APP_STATE_OUTPUT_TIME,
    // Exit code for something unexpected happened (give alarm signal)
    APP_STATE_ERROR,
    // Application hold after error
    APP_STATE_HOLD
} APP_STATES;

// App Timing Structure
typedef struct {
    int milliSeconds;
    int Seconds;
    int Minutes;
    int Hours;
    int Days;
    int Wait;
} APP_TIMING;

// Application Data
typedef struct
{
    // The application's current state
    APP_STATES        state;
    // system timer handling
    int               timerCount;
    int               timerRepeat;
    bool              timerExpired;
    // Timing structure
    APP_TIMING        time;
    int               lastSecond;
    bool              pollSecond;
    // LCD (Re-)Init function callback
    APP_STATES        LCD_Init_Return;
    // LCD Refresh function callback
    APP_STATES        LCD_Return_AppState;
    // LCD Dummy switch for Backlight
    bool              LCD_Dummy_switch;
    char              POEnetPrimInputBuf[APP_BUFFER_SIZE];
    int               POEnetPrimInputSize;
    int               POEnetPrimInputIdx;
    char              POEnetPrimOutputBuf[APP_BUFFER_SIZE];
    int               POEnetPrimOutputSize;
    int               POEnetPrimOutputIdx;
#ifdef APP_POEnet_SECONDARY
    char              POEnetSecInputBuf[APP_BUFFER_SIZE];
    int               POEnetSecInputSize;
    int               POEnetSecInputIdx;
    char              POEnetSecOutputBuf[APP_BUFFER_SIZE];
    int               POEnetSecOutputSize;
    int               POEnetSecOutputIdx;
#endif // ifdef APP_POEnet_SECONDARY
// -> USB Handling migrated to POEusb; UART needs only buffer handling
    // POE.net handling
    char              POEnetUID[APP_STRING_SIZE];
    char              POEnetCommand[APP_STRING_SIZE];
    char              POEnetXMLError[2 * APP_STRING_SIZE];
    int               POEnet_NodeId;
    bool              pollValues;
    int               pollGranularity;
    APP_STATES        poll_Return_AppState;
// ADC_PinIdx is used as polling state for data polling
    int               ADC_PinIdx;
#ifdef APP_USE_ADC
    int               ADC_Waits_Sample;
    APP_STATES        ADC_Return_AppState;
    int               ADC_PinValue[APP_ADC_NUM_PINS];
    int               ADC_PinMean[APP_ADC_NUM_PINS][APP_ADC_MEAN_BUFFER];
    float             ADC_Numerator[APP_ADC_NUM_PINS];
    float             ADC_Denominator[APP_ADC_NUM_PINS];
    float             ADC_Offset[APP_ADC_NUM_PINS];
    float             ADC_Value[APP_ADC_NUM_PINS];
    char              ADC_Unit[APP_ADC_NUM_PINS][APP_STRING_SIZE];
    char              ADC_Representation[APP_ADC_NUM_PINS][APP_STRING_SIZE];
    int               ADC_outline[APP_ADC_NUM_PINS];
    int               ADC_outpos[APP_ADC_NUM_PINS];
    int               ADC_outwidth[APP_ADC_NUM_PINS];
#endif // ifdef APP_USE_ADC
#ifdef APP_USE_DIO
    int               DI_Value[APP_DI_COUNT];
    char              DI_LoValue[APP_DI_COUNT][APP_STRING_SIZE];
    char              DI_HiValue[APP_DI_COUNT][APP_STRING_SIZE];
    int               DI_outline[APP_DI_COUNT];
    int               DI_outpos[APP_DI_COUNT];
    int               DI_outwidth[APP_DI_COUNT];
    int               DO_Value[APP_DO_COUNT];
    char              DO_LoValue[APP_DO_COUNT][APP_STRING_SIZE];
    char              DO_HiValue[APP_DO_COUNT][APP_STRING_SIZE];
    int               DO_outline[APP_DO_COUNT];
    int               DO_outpos[APP_DO_COUNT];
    int               DO_outwidth[APP_DO_COUNT];
#endif // ifdef APP_USE_DIO
#ifdef APP_USE_PWM
    float             PWM_Frequency;
    float             PWM_Width;
    float             PWM_Phase;
    float             PWM_Width2;
    float             PWM_Phase2;
    float             PWM_Width3;
    float             PWM_Phase3;
    float             PWM_Width4;
    int               PWM_Foutline;
    int               PWM_Foutpos;
    int               PWM_Foutwidth;
    int               PWM_Woutline;
    int               PWM_Woutpos;
    int               PWM_Woutwidth;
    int               PWM_Poutline;
    int               PWM_Poutpos;
    int               PWM_Poutwidth;
    TMR_PRESCALE      PWM_PreScale;
    int               PWM_PSFactor;
    int               PWM_Cycle;
    int               PWM_Start1;
    int               PWM_Stop1;
    int               PWM_Start2;
    int               PWM_Stop2;
    int               PWM_Start3;
    int               PWM_Stop3;
    int               PWM_Start4;
    int               PWM_Stop4;
#ifdef APP_USE_PWM2
    float             PWM2_Frequency;
    float             PWM2_Width;
    float             PWM2_Phase;
    float             PWM2_Width2;
    int               PWM2_Foutline;
    int               PWM2_Foutpos;
    int               PWM2_Foutwidth;
    int               PWM2_Woutline;
    int               PWM2_Woutpos;
    int               PWM2_Woutwidth;
    int               PWM2_Poutline;
    int               PWM2_Poutpos;
    int               PWM2_Poutwidth;
    TMR_PRESCALE      PWM2_PreScale;
    int               PWM2_PSFactor;
    int               PWM2_Cycle;
    int               PWM2_Start1;
    int               PWM2_Stop1;
    int               PWM2_Start2;
    int               PWM2_Stop2;
#endif // ifdef APP_USE_PWM2
#endif // ifdef APP_USE_PWM
} APP_DATA;

// -> USB Handling migrated to POEusb

// helper routines
void ClearBuffer(char *buffer);

void ClearString(char *str);

// APP routines
void APP_Initialize ( void );

void APP_TimingCallback ( void );

void APP_CheckTimedLED ( void );

#ifdef APP_USE_UART
void APP_UART_Read(void);

void APP_UART_Write(void);
#endif // of ifdef APP_USE_UART

void APP_Tasks ( void );

// *****************************************************************************
// Section: extern declarations
// *****************************************************************************

extern APP_DATA appData;

#ifdef	__cplusplus
}
#endif

#endif	/* APP_H */

