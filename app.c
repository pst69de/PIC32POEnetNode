/*
 * File:    app.c
 * Author:  Patrick O. Ehrmann (pst69@pst69.de)
 * License: Creative Commons Zero (https://creativecommons.org/publicdomain/zero/1.0/)
 *
 * Created on 2015-02-26
 * Description:
 *   not yet supplied
 * Major Changes:
 *   Version 0: alpha development
 */

#include <string.h>
#include "app.h"
#include "system_config.h"
#include "POEnet.h"

#ifdef APP_LCD_I2C_ID
#include "POEi2clcd.h"
#endif // ifdef APP_LCD_I2C_ID
#ifdef APP_USE_ADC
#include "POEadc.h"
#endif


// helper routines
void ClearBuffer(char *buffer) {
    buffer = memset(buffer, '\0', APP_BUFFER_SIZE);
    //char *first = buffer;
    //while (*buffer != '\0') {
    //   *buffer = '\0';
    //    buffer++;
    //    if (buffer-first >= APP_BUFFER_SIZE) { break; }
    //}
}

void ClearString(char *str) {
    str = memset(str, '\0', APP_STRING_SIZE);
    //char *first = str;
    //while (*str != '\0') {
    //    *str = '\0';
    //    str++;
    //    if (str-first >= APP_STRING_SIZE) { break; }
    //}
}

// APP's data struct
APP_DATA appData;

// Initialize
void APP_Initialize ( void )
{
    int i;
    // Initialize App Timing to zero 
    appData.time.milliSeconds = 0;
    //appData.time.Seconds = 0;
    //appData.time.Minutes = 0;
    //appData.time.Hours = 0;
    appData.time.Days = 0;
    if (appData.time.Seconds) {
        appData.lastSecond = appData.time.Seconds - 1;
    } else {
        appData.lastSecond = 59;
    }
    appData.pollSecond = false;
    appData.time.Wait = 0;
    // timing repetition 
    appData.timerCount = 0;
    appData.timerRepeat = 0;
    appData.timerExpired = false;
#ifdef APP_USE_UART
    // UART init
    ClearBuffer(&appData.UART_INPUT_BUF[0]);
    //for (i = 0; i < APP_UART_RX_BUFFER_SIZE; i++) { appData.UARTreadBuffer[i] = 0;};
    appData.UART_INPUT_SIZE = 0;
    appData.UART_INPUT_IDX = 0;
    ClearBuffer(&appData.UART_OUTPUT_BUF[0]);
    //for (i = 0; i < APP_UART_TX_BUFFER_SIZE; i++) { appData.UARTwriteBuffer[i] = 0;};
    appData.UART_OUTPUT_SIZE = 0;
    appData.UART_OUTPUT_IDX = 0;
#endif // of ifdef APP_USE_UART
    // POE.net handling
    sprintf(&appData.POEnetUID[0],"%04X", DEVCFG3bits.USERID);
    ClearString(&appData.POEnetCommand[0]);
    ClearString(&appData.POEnetXMLError[0]);
    // Double String Length so repeat 
    ClearString(&appData.POEnetXMLError[20]);    
    appData.POEnet_NodeId = -1;
    appData.pollValues = false;
    appData.pollGranularity = 10;
#ifdef APP_USE_ADC
    appData.ADC_PinIdx = 1;
    appData.ADC_PinValue[0] = 0;
    for (i = 0; i < APP_ADC_MEAN_BUFFER; i++) { appData.ADC_PinMean[0][i] = 0;}
    appData.ADC_Numerator[0] = 102.4f;
    appData.ADC_Denominator[0] = 1024.0f;
    appData.ADC_Offset[0] = 0.0f;
    appData.ADC_Value[0] = 0.0f;
    strcpy(&appData.ADC_Unit[0][0], "Du");
    appData.ADC_outline[0] = 0;
    appData.ADC_outpos[0] = 0;
    appData.ADC_outwidth[0] = 0; // no output
#ifdef APP_ADC2_INPUT_POS
    appData.ADC_PinValue[1] = 0;
    for (i = 0; i < APP_ADC_MEAN_BUFFER; i++) { appData.ADC_PinMean[1][i] = 0;}
    appData.ADC_Numerator[1] = 102.4f;
    appData.ADC_Denominator[1] = 1024.0f;
    appData.ADC_Offset[1] = 0.0f;
    appData.ADC_Value[1] = 0.0f;
    strcpy(&appData.ADC_Unit[1][0], "Du");
    appData.ADC_outline[1] = 0;
    appData.ADC_outpos[1] = 0;
    appData.ADC_outwidth[1] = 0; // no output
#endif // ifdef APP_ADC2_INPUT_POS
#ifdef APP_ADC3_INPUT_POS
    appData.ADC_PinValue[2] = 0;
    for (i = 0; i < APP_ADC_MEAN_BUFFER; i++) { appData.ADC_PinMean[2][i] = 0;}
    appData.ADC_Numerator[2] = 33.94f;
    appData.ADC_Denominator[2] = 1024.0f;
    appData.ADC_Offset[2] = 0.0f;
    appData.ADC_Value[2] = 0.0f;
    strcpy(&appData.ADC_Unit[2][0], "V");
    appData.ADC_outline[2] = 0;
    appData.ADC_outpos[2] = 0;
    appData.ADC_outwidth[2] = 0; // no output
#endif // ifdef APP_ADC3_INPUT_POS
#ifdef APP_ADC4_INPUT_POS
    appData.ADC_PinValue[3] = 0;
    for (i = 0; i < APP_ADC_MEAN_BUFFER; i++) { appData.ADC_PinMean[3][i] = 0;}
    appData.ADC_Numerator[3] = -50.0f;
    appData.ADC_Denominator[3] = 1024.0f;
    appData.ADC_Offset[3] = 65.0f;
    appData.ADC_Value[3] = 0.0f;
    strcpy(&appData.ADC_Unit[3][0], "A");
    appData.ADC_outline[3] = 0;
    appData.ADC_outpos[3] = 0;
    appData.ADC_outwidth[3] = 0; // no output
#endif // ifdef APP_ADC4_INPUT_POS
#endif // ifdef APP_USE_ADC
#ifdef APP_USE_DIO
    for (i = 0; i < APP_DI_COUNT; i++) {
        appData.DI_Value[i] = 0;
        ClearString(&appData.DI_LoValue[i][0]);
        appData.DI_LoValue[i][0] = 'L';
        ClearString(&appData.DI_HiValue[i][0]);
        appData.DI_HiValue[i][0] = 'H';
        appData.DI_outline[i] = 0;
        appData.DI_outpos[i] = 0;
        appData.DI_outwidth[i] = 0; // no output
    }
    for (i = 0; i < APP_DO_COUNT; i++) {
        appData.DO_Value[i] = 0;
        ClearString(&appData.DO_LoValue[i][0]);
        appData.DO_LoValue[i][0] = '0';
        ClearString(&appData.DO_HiValue[i][0]);
        appData.DO_HiValue[i][0] = '1';
        appData.DO_outline[i] = 0;
        appData.DO_outpos[i] = 0;
        appData.DO_outwidth[i] = 0; // no output
    }
#endif // ifdef APP_USE_DIO
#ifdef APP_USE_PWM
    appData.PWM_Frequency = 20.0f;
    appData.PWM_Width     = 5.0f;
    appData.PWM_Phase     = 33.0f;
    appData.PWM_Width2    = 5.0f;
    appData.PWM_Phase2    = 67.0f;
    appData.PWM_Width3    = 5.0f;
    appData.PWM_Phase3    = 0.0f;
    appData.PWM_Width4    = 0.0f;
    appData.PWM_PreScale  = APP_PWM_TMR_PRESCALE;
    appData.PWM_PSFactor  = APP_PWM_TMR_PSFactor;
    appData.PWM_Cycle     = APP_PWM_TMR_INIT;
    appData.PWM_Start1    = APP_PWM_OC1_On;
    appData.PWM_Stop1     = APP_PWM_OC1_Off;
#ifdef APP_PWM_OC2_ID
    appData.PWM_Start2    = APP_PWM_OC2_On;
    appData.PWM_Stop2     = APP_PWM_OC2_Off;
#else
    appData.PWM_Start2    = 0;
    appData.PWM_Stop2     = 0;
#endif
#ifdef APP_PWM_OC3_ID
    appData.PWM_Start3    = APP_PWM_OC3_On;
    appData.PWM_Stop3     = APP_PWM_OC3_Off;
#else
    appData.PWM_Start3    = 0;
    appData.PWM_Stop3     = 0;
#endif
#ifdef APP_PWM_OC4_ID
    appData.PWM_Start4    = APP_PWM_OC4_On;
    appData.PWM_Stop4     = APP_PWM_OC4_Off;
#else
    appData.PWM_Start4    = 0;
    appData.PWM_Stop4     = 0;
#endif
    appData.PWM_Foutline  = 2;
    appData.PWM_Foutpos   = 0;
    appData.PWM_Foutwidth = 9;
    appData.PWM_Woutline  = 2;
    appData.PWM_Woutpos   = 10;
    appData.PWM_Woutwidth = 3;
    appData.PWM_Poutline  = 2;
    appData.PWM_Poutpos   = 14;
    appData.PWM_Poutwidth = 3;
#ifdef APP_USE_PWM2
    appData.PWM2_Frequency = 10000.0f;
    appData.PWM2_Width     = 50.0f;
    appData.PWM2_Phase     = 0.0f;
    appData.PWM2_Width2    = 0.0f;
    appData.PWM2_PreScale  = APP_PWM2_TMR_PRESCALE;
    appData.PWM2_PSFactor  = APP_PWM2_TMR_PSFactor;
    appData.PWM2_Cycle     = APP_PWM2_TMR_INIT;
    appData.PWM2_Start1    = APP_PWM2_OC1_On;
    appData.PWM2_Stop1     = APP_PWM2_OC1_Off;
#ifdef APP_PWM2_OC2_ID
    appData.PWM2_Start2    = APP_PWM2_OC2_On;
    appData.PWM2_Stop2     = APP_PWM2_OC2_Off;
#else
    appData.PWM2_Start2    = 0;
    appData.PWM2_Stop2     = 0;
#endif
    appData.PWM2_Foutline  = 3;
    appData.PWM2_Foutpos   = 0;
    appData.PWM2_Foutwidth = 9;
    appData.PWM2_Woutline  = 3;
    appData.PWM2_Woutpos   = 10;
    appData.PWM2_Woutwidth = 3;
    appData.PWM2_Poutline  = 3;
    appData.PWM2_Poutpos   = 14;
    appData.PWM2_Poutwidth = 0; // no output
#endif // ifdef APP_USE_PWM2
#endif // ifdef APP_USE_PWM
    // Place the App state machine in its initial state.
    appData.state = APP_STATE_INIT;
}

void APP_LCD_ClearSpace(int line, int pos, int width) {
    if (width != 0) {
        int i;
        for (i = pos; i < pos + width; i++) {
            APP_LCD_PrintChar(line, i, ' ');
        }
    }
}

void APP_LCD_WriteChar(int line, int pos, int width, char toprint) {
    if (width >= 1) {
        APP_LCD_ClearSpace(line,pos,width);
        APP_LCD_PrintChar(line,pos,toprint);
    }
}

void APP_LCD_WriteString(int line, int pos, int width, char *toprint) {
    if (width >= 1) {
        APP_LCD_ClearSpace(line,pos,width);
        APP_LCD_Print(line,pos,toprint);
    }
}

const char numChar[10] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

// Write Size Information to LCD
void APP_LCD_WriteSize(int line, int pos, int width, int size) {
    // only if configured to display
    if (width >= 3) {
        APP_LCD_ClearSpace(line,pos,width);
        int i = size;
        APP_LCD_PrintChar(line, pos + 2, numChar[i%10]);
        i /= 10;
        APP_LCD_PrintChar(line, pos + 1, numChar[i%10]);
        i /= 10;
        APP_LCD_PrintChar(line, pos, numChar[i%10]);
        i /= 10;
        // not exact, but as we are dealing with values up to 1024 write A24 instead
        if (i) { APP_LCD_PrintChar(line, pos, 'A');}
    }
}

void APP_LCD_WriteFrequency(int line, int pos, int width, float frequency) {
    char strFreq[APP_STRING_SIZE];
    if (width >= 1) {
        ClearString(&strFreq[0]);
        APP_LCD_ClearSpace(line,pos,width);
        sprintf(&strFreq[0],"%5.1fHz", frequency);
        APP_LCD_Print(line,pos,strFreq);
    }
}

void APP_LCD_WritePercent(int line, int pos, int width, float percent) {
    char strPerc[APP_STRING_SIZE];
    if (width >= 1) {
        ClearString(&strPerc[0]);
        APP_LCD_ClearSpace(line,pos,width);
        sprintf(&strPerc[0],"%2f", percent);
        strPerc[2] = '%';
        strPerc[3] = '\0';
        APP_LCD_Print(line,pos,strPerc);
    }
}

void APP_LCD_SetNodeId(void) {
    APP_LCD_Print(1, 15, &appData.POEnetUID[0]);
    APP_LCD_PrintChar(1, 19, numChar[appData.POEnet_NodeId]);
}

// APP Time Base
void APP_TimingCallback ( void ) {
    // Adjust App Timing
    appData.time.milliSeconds++;
    if (appData.time.milliSeconds == 1000) {
        appData.time.milliSeconds = 0;
        appData.time.Seconds++;
        if (appData.time.Seconds == 60) {
            appData.time.Seconds = 0;
            appData.time.Minutes++;
            if (appData.time.Minutes == 60) {
                appData.time.Minutes = 0;
                appData.time.Hours++;
                if (appData.time.Hours == 24) {
                    appData.time.Hours = 0;
                    appData.time.Days++;
                }
            }
        }
        // Set Hours to Time field
        APP_LCD_PrintChar(0,2,numChar[appData.time.Hours % 10]);
        APP_LCD_PrintChar(0,1,numChar[appData.time.Hours / 10]);
        // Set Minutes to Time field
        APP_LCD_PrintChar(0,5,numChar[appData.time.Minutes % 10]);
        APP_LCD_PrintChar(0,4,numChar[appData.time.Minutes / 10]);
        // Set Seconds to Time field
        APP_LCD_PrintChar(0,8,numChar[appData.time.Seconds % 10]);
        APP_LCD_PrintChar(0,7,numChar[appData.time.Seconds / 10]);
        appData.pollSecond = true;
    }
    if (appData.timerCount > 0) {
        appData.timerCount--;
        if (appData.timerCount == 0) {
            appData.timerExpired = true;
            appData.timerCount = appData.timerRepeat;
        }
    }
    if (appData.time.Wait > 0) {
        appData.time.Wait--;
    }
    if (!(appData.time.milliSeconds%appData.pollGranularity)) {
        appData.pollValues = true;
    }
}

// APP Timed LED callback registration routine
// boolean return for future extension e.g. with SYS_TMR-Registration
bool APP_RegisterTimedLED ( int thisTimeMs) {
    appData.timerRepeat = thisTimeMs;
    appData.timerCount = appData.timerRepeat;
    return true;
}

// APP Timed LED routine (blink on different operation states)
bool APP_CheckTimer (void) {
    bool Result = false;
    if (appData.timerExpired) {
        // Toggle LED & clear flag
        LEDR_Toggle;
        appData.timerExpired = false;
    }
    if (appData.pollSecond) {
        // every second 
        appData.pollSecond = false;
        if (APP_LCD_Ready) {
            // actualize LCD
            if (appData.state != APP_LCD_UPDATE) {
                appData.LCD_Return_AppState = appData.state;
                appData.state = APP_LCD_UPDATE;
                Result = true;
            }
        } else {
            // try reinit LCD
            if (appData.state != APP_STATE_LCD_INIT) {
                appData.LCD_Init_Return = appData.state;
                appData.state = APP_STATE_LCD_INIT;
                Result = true;
            }
        }
    }
    if (appData.pollValues) {
        appData.pollValues = false;
        // not while LCD_Update Or Polling itself
        if ((appData.state != APP_LCD_UPDATE)
#ifdef APP_USE_ADC
            & (appData.state != APP_STATE_START_ADC)
            & (appData.state != APP_STATE_CONVERT_ADC)
            & (appData.state != APP_STATE_READ_ADC)
#endif
            & (appData.state != APP_STATE_POLL_DATA)) {
            appData.poll_Return_AppState = appData.state;
            appData.state = APP_STATE_POLL_DATA;
            appData.ADC_PinIdx = 0; // start with digital polling
            Result = true; // set break on current action
        }
    } 
    return Result;
}

// LCD I2C Routines see POEi2clcd.h / POEi2clcd.c

#ifdef APP_USE_UART
// UART routines 
// INT Handling Read
void APP_UART_Read(void) {
#ifdef APP_UART_DEBUG
    APP_LCD_PrintChar(0,10,'*');
#endif //ifdef APP_UART_DEBUG
    while (PLIB_USART_ReceiverDataIsAvailable(APP_UART_RX_ID)) {
        uint8_t readByte = PLIB_USART_ReceiverByteReceive(APP_UART_RX_ID);
        appData.UART_INPUT_BUF[appData.UART_INPUT_IDX++] = readByte;
#ifdef APP_UART_DEBUG
        // APP_LCD_PrintChar(0,10,'R');
        APP_LCD_WriteSize(0,11,0,appData.UART_INPUT_IDX);
#endif //ifdef APP_UART_DEBUG
        // POE.net: LF/zero is sent for termination
        if (!readByte | (readByte == 10)) {
            appData.UART_INPUT_SIZE = appData.UART_INPUT_IDX;
#ifdef APP_UART_DEBUG
            APP_LCD_PrintChar(0,10,'R');
#endif //ifdef APP_UART_DEBUG
        }
    }
}

// INT Handling Write 
void APP_UART_Write(void) {
    if (appData.UART_OUTPUT_IDX == appData.UART_OUTPUT_SIZE) {
        PLIB_USART_TransmitterDisable(APP_UART_TX_ID);
#ifdef APP_UART_DEBUG
        APP_LCD_PrintChar(0,15,'T');
#endif //ifdef APP_UART_DEBUG
        // after disabling the transmitter the interrupt will call a second time,
        // nulling the transmitted bytes :(
#ifdef APP_UART_DEBUG
        if (appData.UART_OUTPUT_IDX > 0) { APP_LCD_WriteSize(0,16,0,appData.UART_OUTPUT_IDX); }
#endif //ifdef APP_UART_DEBUG
        appData.UART_OUTPUT_SIZE = 0;
        appData.UART_OUTPUT_IDX = 0;
    }
    if (appData.UART_OUTPUT_SIZE > 0) {
#ifdef APP_UART_DEBUG
        APP_LCD_PrintChar(0,15,'*');
#endif //ifdef APP_UART_DEBUG
        if (!PLIB_USART_TransmitterBufferIsFull(APP_UART_TX_ID) & (appData.UART_OUTPUT_IDX < appData.UART_OUTPUT_SIZE)) {
            PLIB_USART_TransmitterByteSend(APP_UART_TX_ID, appData.UART_OUTPUT_BUF[ appData.UART_OUTPUT_IDX++]);
#ifdef APP_UART_DEBUG
            APP_LCD_WriteSize(0,16,0,appData.UART_OUTPUT_IDX);
#endif //ifdef APP_UART_DEBUG
        }
    }
}
#endif // of ifdef APP_USE_UART

//***************************************************
// This is the standard texts replied to host
//***************************************************
char nokPrompt[] = "NOK\n";
char okPrompt[] = "OK\n";
char str2int[12] = "\0";

void APP_Tasks ( void )
{
    int i,m;
#ifdef APP_USE_PWM
    bool newPWM;
#endif // ifdef APP_USE_PWM
    
    // check the application state
    switch ( appData.state ) {
        // Application's initial state
        case APP_STATE_INIT:
            // Turn Off LED
            LEDR_Clear;
            LEDB_Clear;
            //appData.state = APP_STATE_ERROR; // Blink Test
            appData.state = APP_STATE_POENET_INIT;
            break;
        case APP_STATE_POENET_INIT:
            POEnet_Node_Init( &appData.POEnet_NodeId, &appData.POEnetUID[0], &appData.time.Hours, &appData.time.Minutes, &appData.time.Seconds);
#ifdef APP_USE_ADC
#ifdef APP_ADC1_INPUT_POS
            POEnet_AddAnalog(1,&appData.ADC_Value[0],&appData.ADC_Numerator[0],&appData.ADC_Denominator[0],&appData.ADC_Offset[0],&appData.ADC_Unit[0][0]);
#endif
#ifdef APP_ADC2_INPUT_POS
            POEnet_AddAnalog(2,&appData.ADC_Value[1],&appData.ADC_Numerator[1],&appData.ADC_Denominator[1],&appData.ADC_Offset[1],&appData.ADC_Unit[1][0]);
#endif
#ifdef APP_ADC3_INPUT_POS
            POEnet_AddAnalog(3,&appData.ADC_Value[2],&appData.ADC_Numerator[2],&appData.ADC_Denominator[2],&appData.ADC_Offset[2],&appData.ADC_Unit[2][0]);
#endif
#ifdef APP_ADC4_INPUT_POS
            POEnet_AddAnalog(4,&appData.ADC_Value[3],&appData.ADC_Numerator[3],&appData.ADC_Denominator[3],&appData.ADC_Offset[3],&appData.ADC_Unit[3][0]);
#endif
#endif // ifdef APP_USE_ADC
#ifdef APP_USE_DIO
#ifdef APP_DI_1
            POEnet_AddDigital(1,&appData.DI_Value[0],&appData.DI_LoValue[0][0],&appData.DI_HiValue[0][0]);
#endif
#ifdef APP_DI_2
            POEnet_AddDigital(2,&appData.DI_Value[1],&appData.DI_LoValue[1][0],&appData.DI_HiValue[1][0]);
#endif
#ifdef APP_DI_3
            POEnet_AddDigital(3,&appData.DI_Value[2],&appData.DI_LoValue[2][0],&appData.DI_HiValue[2][0]);
#endif
#ifdef APP_DI_4
            POEnet_AddDigital(4,&appData.DI_Value[3],&appData.DI_LoValue[3][0],&appData.DI_HiValue[3][0]);
#endif
#ifdef APP_DO_1
            POEnet_AddSwitch(1,&appData.DO_Value[0],&appData.DO_LoValue[0][0],&appData.DO_HiValue[0][0]);
#endif
#ifdef APP_DO_2
            POEnet_AddSwitch(2,&appData.DO_Value[1],&appData.DO_LoValue[1][0],&appData.DO_HiValue[1][0]);
#endif
#ifdef APP_DO_3
            POEnet_AddSwitch(3,&appData.DO_Value[2],&appData.DO_LoValue[2][0],&appData.DO_HiValue[2][0]);
#endif
#ifdef APP_DO_4
            POEnet_AddSwitch(4,&appData.DO_Value[3],&appData.DO_LoValue[3][0],&appData.DO_HiValue[3][0]);
#endif
#endif // ifdef APP_USE_DIO
#ifdef APP_USE_PWM
            POEnet_AddPWM3(
                    1
                    , &appData.PWM_Frequency
                    , &appData.PWM_Width
                    , &appData.PWM_Phase
                    , &appData.PWM_Width2
                    , &appData.PWM_Phase2
                    , &appData.PWM_Width3
                    );
#ifdef APP_USE_PWM2
            POEnet_AddPWM(
                    2
                    , &appData.PWM2_Frequency
                    , &appData.PWM2_Width
                    );
#endif // ifdef APP_USE_PWM2
#endif // ifdef APP_USE_PWM
#ifdef APP_USE_UART
            // init UART before USB
            appData.LCD_Init_Return = APP_STATE_UART_INIT;
#else
            appData.LCD_Init_Return = APP_STATE_HOLD;
#endif // ifdef APP_USE_UART
            appData.state = APP_STATE_LCD_INIT;
            break;
        case APP_STATE_LCD_INIT:
            if (APP_LCD_Init(appData.time.milliSeconds)) {
                if (APP_LCD_Ready) {
                    //LEDG_Clear;
                    // -> use of sprintf for string formatting
                    // Time and Status representation
                    // POE.net status representation
                    APP_LCD_Print( 0, 0, " __:__:__ debugout..");
                    APP_LCD_PrintChar(0,2,numChar[appData.time.Hours % 10]);
                    APP_LCD_PrintChar(0,1,numChar[appData.time.Hours / 10]);
                    APP_LCD_PrintChar(0,5,numChar[appData.time.Minutes % 10]);
                    APP_LCD_PrintChar(0,4,numChar[appData.time.Minutes / 10]);
                    APP_LCD_PrintChar(0,8,numChar[appData.time.Seconds % 10]);
                    APP_LCD_PrintChar(0,7,numChar[appData.time.Seconds / 10]);
                    APP_LCD_Print( 1, 0, "POEnet UID____");
                    APP_LCD_Print( 1, 10, &appData.POEnetUID[0]);
                    APP_LCD_ClearLine(2);
                    APP_LCD_ClearLine(3);
                    APP_LCD_WriteFrequency(appData.PWM_Foutline,appData.PWM_Foutpos,appData.PWM_Foutwidth,appData.PWM_Frequency);
                    APP_LCD_WritePercent(appData.PWM_Woutline,appData.PWM_Woutpos,appData.PWM_Woutwidth,appData.PWM_Width);
                    APP_LCD_WritePercent(appData.PWM_Poutline,appData.PWM_Poutpos,appData.PWM_Poutwidth,appData.PWM_Phase);
                    APP_LCD_WriteFrequency(appData.PWM2_Foutline,appData.PWM2_Foutpos,appData.PWM2_Foutwidth,appData.PWM2_Frequency);
                    APP_LCD_WritePercent(appData.PWM2_Woutline,appData.PWM2_Woutpos,appData.PWM2_Woutwidth,appData.PWM2_Width);
                    appData.LCD_Return_AppState = appData.LCD_Init_Return;
                    appData.state = APP_LCD_UPDATE;
                } else {
                    // LCD Init failed, no LCD avaiable
                    appData.state = appData.LCD_Init_Return;
                } 
            }
            break;
#ifdef APP_USE_UART
        // Init UART
        case APP_STATE_UART_INIT:
            if (APP_CheckTimer()) { break; }
            // if UART Transmitter is enabled, it automatically asks 
            // for the 1st byte via Interrupt, so wait for the
            // transmission to be complete (in APP_STATE_POENET_OUTPUT_PREPARE)
            //PLIB_UART_TransmitterEnable(APP_UART_TX_ID);
            PLIB_USART_ReceiverEnable(APP_UART_RX_ID);
            // goto Input afterwards
            appData.LCD_Return_AppState = APP_STATE_POENET_INPUT;
            appData.state = APP_LCD_UPDATE;
            break;
#endif // of ifdef APP_USE_UART
        // POE.net reset message
        case APP_STATE_POENET_RESET:
#ifdef APP_POEnet_SECONDARY
            strcpy(&appData.POEnetSecOutputBuf[0], "U<reset/>");
            appData.POEnetSecOutputSize = strlen(&appData.POEnetSecOutputBuf[0]);
            appData.POEnetSecOutputBuf[appData.POEnetSecOutputSize++] = APP_UART_TERMINATOR;
            appData.POEnetSecOutputIdx = 0;
#else // ifdef APP_POEnet_SECONDARY
            strcpy(&appData.POEnetPrimOutputBuf[0], "U<reset/>");
            appData.POEnetPrimOutputSize = strlen(&appData.POEnetPrimOutputBuf[0]);
            appData.POEnetPrimOutputBuf[appData.POEnetPrimOutputSize++] = APP_UART_TERMINATOR;
            appData.POEnetPrimOutputIdx = 0;            
#endif // else APP_POEnet_SECONDARY
            // POETODO: try a const_cast to avoid warning http://en.cppreference.com/w/cpp/language/const_cast
            APP_LCD_Print( 1, 7, &POEnet_reset[0]);
            appData.LCD_Return_AppState = APP_STATE_POENET_OUTPUT_PREPARE;
            appData.state = APP_LCD_UPDATE;
            break;
        // POE.net input phase
        case APP_STATE_POENET_INPUT:
            if (APP_CheckTimer()) { break; }
#ifdef APP_UART_DEBUG
            APP_LCD_PrintChar(0,14,'*');
#endif //ifdef APP_UART_DEBUG
#ifdef APP_USE_UART
            // UART may be Secondary, so pass Input as compiled
            // only if there is no USB processing needed
            // *****************************************************************
            // maybe there is the need of a more advanced state switching 
            // as USB inputs override backtransmissions from serial (to be send by USB)
            // *****************************************************************
            if ((appData.state == APP_STATE_POENET_INPUT) & (appData.UART_INPUT_SIZE > 0)) {
#ifdef APP_POEnet_SECONDARY
                // pass to Buffer pass
                appData.state = APP_STATE_POENET_PASS;
#else
                // pass to INPUTReady
                appData.state = APP_STATE_POENET_INPUT_READY;
#endif
                break;
            }
#endif // of ifdef APP_USE_UART
            break;
        // POE.net input buffer filled
        case APP_STATE_POENET_INPUT_READY:
            if (APP_CheckTimer()) { break; }
#ifdef APP_UART_DEBUG
            APP_LCD_PrintChar(0,14,'#');
#endif //ifdef APP_UART_DEBUG
            switch (appData.POEnetPrimInputBuf[0]) {
                case 'U':
                    //POE.net Message -> pass to interpreter
                    POEnet_Interpret(&appData.POEnetPrimInputBuf[1]);
                    APP_LCD_Print( 1, 7, &POEnet_empty[0]);
                    APP_LCD_SetNodeId();
                    appData.LCD_Return_AppState = APP_STATE_POENET_COMMAND;
                    appData.state = APP_LCD_UPDATE;
                    break;
                case 'T':
                    APP_LCD_ClearLine(3);
                    // set the text message
                    APP_LCD_Print(3, 0, &appData.POEnetPrimInputBuf[1]);
                    appData.LCD_Return_AppState = APP_STATE_POENET_INPUT;
                    appData.state = APP_LCD_UPDATE;
                    break;
                case 'B':
                    APP_LCD_Backlight = !APP_LCD_Backlight;
                    appData.LCD_Return_AppState = APP_STATE_POENET_INPUT;
                    appData.state = APP_LCD_UPDATE;
                    break;
                default:
                    // ignore Message
                    appData.state = APP_STATE_POENET_INPUT;
                    break;
            }
            ClearBuffer(&appData.POEnetPrimInputBuf[0]);
            appData.POEnetPrimInputSize = 0;
            appData.POEnetPrimInputIdx = 0;
            break;
        // POE.net command interpretation
        case APP_STATE_POENET_COMMAND:
            if (APP_CheckTimer()) { break; }
            // Handle Error
            if (POEnet_GetError(&appData.POEnetXMLError[0])) {
                APP_LCD_Print( 1, 7, &POEnet_error[0]);
                APP_LCD_Print( 3, 0, &appData.POEnetXMLError[0]);
            } else {
                POEnet_GetCommand(&appData.POEnetCommand[0]);
                APP_LCD_Print( 1, 7, &POEnet_empty[0]);
                APP_LCD_Print( 1, 7, &appData.POEnetCommand[0]);
            }
            //if (!strcmp(&appData.POEnetCommand[0],&POEnet_net[0])) {
            //    // handle net command
            //    POEnet_GetNewNodeId(&appData.POEnet_NodeId);
            //}
            ClearBuffer(&appData.POEnetPrimOutputBuf[0]);
            appData.POEnetPrimOutputBuf[0] = 'U';
            POEnet_Output(&appData.POEnetPrimOutputBuf[1]);
            appData.POEnetPrimOutputSize = strlen(&appData.POEnetPrimOutputBuf[0]);
            appData.POEnetPrimOutputBuf[appData.POEnetPrimOutputSize++] = APP_UART_TERMINATOR;
            // c# serialPort needs a kicker to put the LF in the buffer 
            appData.POEnetPrimOutputBuf[appData.POEnetPrimOutputSize++] = '\0';
            appData.POEnetPrimOutputIdx = 0;
            appData.LCD_Return_AppState = APP_STATE_POENET_OUTPUT_PREPARE;
            appData.state = APP_LCD_UPDATE;
            break;
        // POE.net output phase
        case APP_STATE_POENET_OUTPUT_PREPARE:
            if (APP_CheckTimer()) { break; }
#ifdef APP_POEnet_SECONDARY
            // USB UART Bridge -> pass to UART
            //APP_LCD_PrintChar(2,6,'>');
            // initiate UART transmission by writing 1st Byte
            //APP_UART_Write();
            // if UART Transmitter is enabled, it automatically asks 
            // for the 1st byte via Interrupt
            PLIB_USART_TransmitterEnable(APP_UART_TX_ID);
            appData.state = APP_STATE_POENET_OUTPUT_READY;
#else // ifdef APP_POEnet_SECONDARY
            // no secondary -> pass Output to UART TX 
            // initiate UART transmission by writing 1st Byte
            //APP_UART_Write();
            PLIB_USART_TransmitterEnable(APP_UART_TX_ID);
            appData.state = APP_STATE_POENET_OUTPUT_READY;
#endif // else APP_POEnet_SECONDARY
            break;
        // POE.net output phase finished
        case APP_STATE_POENET_OUTPUT_READY:
            if (APP_CheckTimer()) { break; }
            // primary Out is always UART transmission -> check for fullfillment
            if (appData.POEnetPrimOutputIdx == appData.POEnetPrimOutputSize) {
                ClearBuffer(&appData.POEnetPrimOutputBuf[0]);
                appData.POEnetPrimOutputSize = 0;
                appData.POEnetPrimOutputIdx = 0;
                appData.state = APP_STATE_POENET_INPUT;
            }
            break;
#ifdef APP_POEnet_SECONDARY
        case APP_STATE_POENET_PASS:
            if (APP_CheckTimer()) { break; }
            strcpy( &appData.POEnetSecOutputBuf[0], &appData.POEnetSecInputBuf[0]);
            appData.POEnetSecOutputSize = appData.POEnetSecInputSize;
            // POE.net messages usually terminate with zero \0
            // put a LineFeed at the end of USB transmission instead
            appData.POEnetSecOutputBuf[appData.POEnetSecOutputSize - 1] = '\n';
            appData.POEnetSecOutputIdx = 0;
            ClearBuffer(&appData.POEnetSecInputBuf[0]);
            appData.POEnetSecInputSize = 0;
            appData.POEnetSecInputIdx = 0;
            // if it is Secondary the output always goes to the USB
            appData.state = APP_STATE_USB_WRITE;
            break;
#endif // ifdef APP_POEnet_SECONDARY
        case APP_STATE_POLL_DATA:
            if (appData.ADC_PinIdx == 0) {
#ifdef APP_USE_DIO
#ifdef APP_DI_1
                appData.DI_Value[0] = DIO_ReadDI(1);
                if (appData.DI_Value[0]) {
                    APP_LCD_WriteChar(appData.DI_outline[0],appData.DI_outpos[0],appData.DI_outwidth[0],appData.DI_HiValue[0][0]);
                } else {
                    APP_LCD_WriteChar(appData.DI_outline[0],appData.DI_outpos[0],appData.DI_outwidth[0],appData.DI_LoValue[0][0]);
                }
#endif // ifdef APP_DI_1
#ifdef APP_DI_2
                appData.DI_Value[1] = DIO_ReadDI(2);
                if (appData.DI_Value[1]) {
                    APP_LCD_WriteChar(appData.DI_outline[1],appData.DI_outpos[1],appData.DI_outwidth[1],appData.DI_HiValue[1][0]);
                } else {
                    APP_LCD_WriteChar(appData.DI_outline[1],appData.DI_outpos[1],appData.DI_outwidth[1],appData.DI_LoValue[1][0]);
                }
#endif // ifdef APP_DI_2
#ifdef APP_DI_3
                appData.DI_Value[2] = DIO_ReadDI(3);
                if (appData.DI_Value[2]) {
                    APP_LCD_WriteChar(appData.DI_outline[2],appData.DI_outpos[2],appData.DI_outwidth[2],appData.DI_HiValue[2][0]);
                } else {
                    APP_LCD_WriteChar(appData.DI_outline[2],appData.DI_outpos[2],appData.DI_outwidth[2],appData.DI_LoValue[2][0]);
                }
#endif // ifdef APP_DI_3
#ifdef APP_DI_4
                appData.DI_Value[3] = DIO_ReadDI(4);
                if (appData.DI_Value[3]) {
                    APP_LCD_WriteChar(appData.DI_outline[3],appData.DI_outpos[3],appData.DI_outwidth[3],appData.DI_HiValue[3][0]);
                } else {
                    APP_LCD_WriteChar(appData.DI_outline[3],appData.DI_outpos[3],appData.DI_outwidth[3],appData.DI_LoValue[3][0]);
                }
#endif // ifdef APP_DI_4
#ifdef APP_DO_1
                if (appData.DO_Value[0]) {
                    DIO_SetDO(1);
                    APP_LCD_WriteChar(appData.DO_outline[0],appData.DO_outpos[0],appData.DO_outwidth[0],appData.DO_HiValue[0][0]);
                } else {
                    DIO_ClearDO(1);
                    APP_LCD_WriteChar(appData.DO_outline[0],appData.DO_outpos[0],appData.DO_outwidth[0],appData.DO_LoValue[0][0]);
                }
#endif // ifdef APP_DO_1
#ifdef APP_DO_2
                if (appData.DO_Value[1]) {
                    DIO_SetDO(2);
                    APP_LCD_WriteChar(appData.DO_outline[1],appData.DO_outpos[1],appData.DO_outwidth[1],appData.DO_HiValue[1][0]);
                } else {
                    DIO_ClearDO(2);
                    APP_LCD_WriteChar(appData.DO_outline[1],appData.DO_outpos[1],appData.DO_outwidth[1],appData.DO_LoValue[1][0]);
                }
#endif // ifdef APP_DO_2
#ifdef APP_DO_3
                if (appData.DO_Value[2]) {
                    DIO_SetDO(3);
                    APP_LCD_WriteChar(appData.DO_outline[2],appData.DO_outpos[2],appData.DO_outwidth[2],appData.DO_HiValue[2][0]);
                } else {
                    DIO_ClearDO(3);
                    APP_LCD_WriteChar(appData.DO_outline[2],appData.DO_outpos[2],appData.DO_outwidth[2],appData.DO_LoValue[2][0]);
                }
#endif // ifdef APP_DO_3
#ifdef APP_DO_4
                if (appData.DO_Value[3]) {
                    DIO_SetDO(4);
                    APP_LCD_WriteChar(appData.DO_outline[3],appData.DO_outpos[3],appData.DO_outwidth[3],appData.DO_HiValue[3][0]);
                } else {
                    DIO_ClearDO(4);
                    APP_LCD_WriteChar(appData.DO_outline[3],appData.DO_outpos[3],appData.DO_outwidth[3],appData.DO_LoValue[3][0]);
                }
#endif // ifdef APP_DO_4
#endif // ifdef APP_USE_DIO
#ifdef APP_USE_PWM
                // set PWM only if needed
                newPWM = false;
                m = (int)(APP_PBCLK_FREQ / (appData.PWM_PSFactor * appData.PWM_Frequency));
                if ((65535 < m) | (400 > m)){
                    // Calculation of PreScale frequency * maxwidth must be greater than prescaled timer clock
                    if (65535 > (APP_PBCLK_FREQ / appData.PWM_Frequency)) {
                        // TMR_PRESCALE_VALUE_1 = 0x00,
                        if (appData.PWM_PreScale != TMR_PRESCALE_VALUE_1) { 
                            newPWM = true;
                            appData.PWM_PreScale = TMR_PRESCALE_VALUE_1;
                            appData.PWM_PSFactor = 1;
                        }
                    }
                    else if (65535 > (APP_PBCLK_FREQ / (2 * appData.PWM_Frequency))) {
                        // TMR_PRESCALE_VALUE_2 = 0x01,
                        if (appData.PWM_PreScale != TMR_PRESCALE_VALUE_2) { 
                            newPWM = true;
                            appData.PWM_PreScale = TMR_PRESCALE_VALUE_2;
                            appData.PWM_PSFactor = 2;
                        }
                    }
                    else if (65535 > (APP_PBCLK_FREQ / (4 * appData.PWM_Frequency))) {
                        // TMR_PRESCALE_VALUE_4 = 0x02,
                        if (appData.PWM_PreScale != TMR_PRESCALE_VALUE_4) { 
                            newPWM = true;
                            appData.PWM_PreScale = TMR_PRESCALE_VALUE_4;
                            appData.PWM_PSFactor = 4;
                        }
                    }
                    else if (65535 > (APP_PBCLK_FREQ / (8 * appData.PWM_Frequency))) {
                        // TMR_PRESCALE_VALUE_8 = 0x03,
                        if (appData.PWM_PreScale != TMR_PRESCALE_VALUE_8) { 
                            newPWM = true;
                            appData.PWM_PreScale = TMR_PRESCALE_VALUE_8;
                            appData.PWM_PSFactor = 8;
                        }
                    }
                    else if (65535 > (APP_PBCLK_FREQ / (16 * appData.PWM_Frequency))) {
                        // TMR_PRESCALE_VALUE_16 = 0x04,
                        if (appData.PWM_PreScale != TMR_PRESCALE_VALUE_16) { 
                            newPWM = true;
                            appData.PWM_PreScale = TMR_PRESCALE_VALUE_16;
                            appData.PWM_PSFactor = 16;
                        }
                    }
                    else if (65535 > (APP_PBCLK_FREQ / (32 * appData.PWM_Frequency))) {
                        // TMR_PRESCALE_VALUE_32 = 0x05,
                        if (appData.PWM_PreScale != TMR_PRESCALE_VALUE_32) { 
                            newPWM = true;
                            appData.PWM_PreScale = TMR_PRESCALE_VALUE_32;
                            appData.PWM_PSFactor = 32;
                        }
                    }
                    else if (65535 > (APP_PBCLK_FREQ / (64 * appData.PWM_Frequency))) {
                        // TMR_PRESCALE_VALUE_64 = 0x06,
                        if (appData.PWM_PreScale != TMR_PRESCALE_VALUE_64) { 
                            newPWM = true;
                            appData.PWM_PreScale = TMR_PRESCALE_VALUE_64;
                            appData.PWM_PSFactor = 64;
                        }
                    }
                    else {
                        // TMR_PRESCALE_VALUE_256 = 0x07 (lowest)
                        // at least 2Hz
                        if (appData.PWM_Frequency < 2.0f) {appData.PWM_Frequency = 2.0f;}
                        if (appData.PWM_PreScale != TMR_PRESCALE_VALUE_256) { 
                            newPWM = true;
                            appData.PWM_PreScale = TMR_PRESCALE_VALUE_256;
                            appData.PWM_PSFactor = 256;
                        }
                    }
#ifdef APP_PWM_DEBUG
                    APP_LCD_WriteSize(0,11,3,appData.PWM_PSFactor);
#endif //ifdef APP_PWM_DEBUG
                }
                // Calculation of Timer Width
                m = (int)(APP_PBCLK_FREQ / (appData.PWM_PSFactor * appData.PWM_Frequency));
                if (appData.PWM_Cycle != m) {
                    newPWM = true;
                    appData.PWM_Cycle = m;
                }
                // POETODO: if any of PWM_Phase1, PWM_Phase1+PWM_Width1, PWM_Phase2, PWM_Phase2+PWM_Width2 > 100% then the MCU gets an invalid value
                // solution may be based on shifting all values around the corner, not yet implemented
                // up to 50%, = Phase%, over 50%, = 100% - Phase% (Channels swapped ???)
                // Calculation of Start1
                m = (int)((appData.PWM_Cycle * 0.0f / 100.0f) + 1);
                if (appData.PWM_Start1 != m) {
                    newPWM = true;
                    appData.PWM_Start1 = m;
                }
                // Calculation of Stop1
                m = (int)((appData.PWM_Cycle * (0.0f + appData.PWM_Width) / 100.0f) + 1);
                if (appData.PWM_Stop1 != m) {
                    newPWM = true;
                    appData.PWM_Stop1 = m;
                }
                // Calculation of Start2
                m = (int)((appData.PWM_Cycle * appData.PWM_Phase / 100.0f) + 1);
                if (appData.PWM_Start2 != m) {
                    newPWM = true;
                    appData.PWM_Start2 = m;
                }
                // Calculation of Stop2
                m = (int)((appData.PWM_Cycle * (appData.PWM_Phase + appData.PWM_Width2) / 100.0f) + 1);
                if (appData.PWM_Stop2 != m) {
                    newPWM = true;
                    appData.PWM_Stop2 = m;
                }
                // Calculation of Start3
                m = (int)((appData.PWM_Cycle * appData.PWM_Phase2 / 100.0f) + 1);
                if (appData.PWM_Start3 != m) {
                    newPWM = true;
                    appData.PWM_Start3 = m;
                }
                // Calculation of Stop3
                m = (int)((appData.PWM_Cycle * (appData.PWM_Phase2 + appData.PWM_Width3) / 100.0f) + 1);
                if (appData.PWM_Stop3 != m) {
                    newPWM = true;
                    appData.PWM_Stop3 = m;
                }
                // Calculation of Start4
                m = (int)((appData.PWM_Cycle * appData.PWM_Phase3 / 100.0f) + 1);
                if (appData.PWM_Start4 != m) {
                    newPWM = true;
                    appData.PWM_Start4 = m;
                }
                // Calculation of Stop4
                m = (int)((appData.PWM_Cycle * (appData.PWM_Phase3 + appData.PWM_Width4) / 100.0f) + 1);
                if (appData.PWM_Stop4 != m) {
                    newPWM = true;
                    appData.PWM_Stop4 = m;
                }
                // Set new timing if needed
                if (newPWM) {
                    // set up Display values
                    APP_LCD_ClearLine(3);
                    APP_LCD_WriteFrequency(appData.PWM_Foutline,appData.PWM_Foutpos,appData.PWM_Foutwidth,appData.PWM_Frequency);
                    APP_LCD_WritePercent(appData.PWM_Woutline,appData.PWM_Woutpos,appData.PWM_Woutwidth,appData.PWM_Width);
                    APP_LCD_WritePercent(appData.PWM_Poutline,appData.PWM_Poutpos,appData.PWM_Poutwidth,appData.PWM_Phase);
                    PWM_SetValues( 
                          appData.PWM_PreScale
                        , appData.PWM_Cycle
                        , appData.PWM_Start1
                        , appData.PWM_Stop1
                        , appData.PWM_Start2
                        , appData.PWM_Stop2
                        , appData.PWM_Start3
                        , appData.PWM_Stop3
                        , appData.PWM_Start4
                        , appData.PWM_Stop4
                        );
                }
#ifdef APP_USE_PWM2
                // set PWM only if needed
                newPWM = false;
                m = (int)(APP_PBCLK_FREQ / (appData.PWM2_PSFactor * appData.PWM2_Frequency));
                if ((65535 < m) | (400 > m)){
                    // Calculation of PreScale frequency * maxwidth must be greater than prescaled timer clock
                    if (65535 > (APP_PBCLK_FREQ / appData.PWM2_Frequency)) {
                        // TMR_PRESCALE_VALUE_1 = 0x00,
                        if (appData.PWM2_PreScale != TMR_PRESCALE_VALUE_1) { 
                            newPWM = true;
                            appData.PWM2_PreScale = TMR_PRESCALE_VALUE_1;
                            appData.PWM2_PSFactor = 1;
                        }
                    }
                    else if (65535 > (APP_PBCLK_FREQ / (2 * appData.PWM2_Frequency))) {
                        // TMR_PRESCALE_VALUE_2 = 0x01,
                        if (appData.PWM2_PreScale != TMR_PRESCALE_VALUE_2) { 
                            newPWM = true;
                            appData.PWM2_PreScale = TMR_PRESCALE_VALUE_2;
                            appData.PWM2_PSFactor = 2;
                        }
                    }
                    else if (65535 > (APP_PBCLK_FREQ / (4 * appData.PWM2_Frequency))) {
                        // TMR_PRESCALE_VALUE_4 = 0x02,
                        if (appData.PWM2_PreScale != TMR_PRESCALE_VALUE_4) { 
                            newPWM = true;
                            appData.PWM2_PreScale = TMR_PRESCALE_VALUE_4;
                            appData.PWM2_PSFactor = 4;
                        }
                    }
                    else if (65535 > (APP_PBCLK_FREQ / (8 * appData.PWM2_Frequency))) {
                        // TMR_PRESCALE_VALUE_8 = 0x03,
                        if (appData.PWM2_PreScale != TMR_PRESCALE_VALUE_8) { 
                            newPWM = true;
                            appData.PWM2_PreScale = TMR_PRESCALE_VALUE_8;
                            appData.PWM2_PSFactor = 8;
                        }
                    }
                    else if (65535 > (APP_PBCLK_FREQ / (16 * appData.PWM2_Frequency))) {
                        // TMR_PRESCALE_VALUE_16 = 0x04,
                        if (appData.PWM2_PreScale != TMR_PRESCALE_VALUE_16) { 
                            newPWM = true;
                            appData.PWM2_PreScale = TMR_PRESCALE_VALUE_16;
                            appData.PWM2_PSFactor = 16;
                        }
                    }
                    else if (65535 > (APP_PBCLK_FREQ / (32 * appData.PWM2_Frequency))) {
                        // TMR_PRESCALE_VALUE_32 = 0x05,
                        if (appData.PWM2_PreScale != TMR_PRESCALE_VALUE_32) { 
                            newPWM = true;
                            appData.PWM2_PreScale = TMR_PRESCALE_VALUE_32;
                            appData.PWM2_PSFactor = 32;
                        }
                    }
                    else if (65535 > (APP_PBCLK_FREQ / (64 * appData.PWM2_Frequency))) {
                        // TMR_PRESCALE_VALUE_64 = 0x06,
                        if (appData.PWM2_PreScale != TMR_PRESCALE_VALUE_64) { 
                            newPWM = true;
                            appData.PWM2_PreScale = TMR_PRESCALE_VALUE_64;
                            appData.PWM2_PSFactor = 64;
                        }
                    }
                    else {
                        // TMR_PRESCALE_VALUE_256 = 0x07 (lowest)
                        // at least 2Hz
                        if (appData.PWM2_Frequency < 2.0f) {appData.PWM2_Frequency = 2.0f;}
                        if (appData.PWM2_PreScale != TMR_PRESCALE_VALUE_256) { 
                            newPWM = true;
                            appData.PWM2_PreScale = TMR_PRESCALE_VALUE_256;
                            appData.PWM2_PSFactor = 256;
                        }
                    }
#ifdef APP_PWM_DEBUG
                    APP_LCD_WriteSize(0,15,3,appData.PWM2_PSFactor);
#endif //ifdef APP_PWM_DEBUG
                }
                // Calculation of Timer Width
                m = (int)(APP_PBCLK_FREQ / (appData.PWM2_PSFactor * appData.PWM2_Frequency));
                if (appData.PWM2_Cycle != m) {
                    newPWM = true;
                    appData.PWM2_Cycle = m;
                }
                // Calculation of Start1
                m = (int)((appData.PWM2_Cycle * 0.0f / 100.0f) + 1);
                if (appData.PWM2_Start1 != m) {
                    newPWM = true;
                    appData.PWM2_Start1 = m;
                }
                // Calculation of Stop1
                m = (int)((appData.PWM2_Cycle * (0.0f + appData.PWM2_Width) / 100.0f) + 1);
                if (appData.PWM2_Stop1 != m) {
                    newPWM = true;
                    appData.PWM2_Stop1 = m;
                }
                // Calculation of Start2
                m = (int)((appData.PWM2_Cycle * appData.PWM2_Phase / 100.0f) + 1);
                if (appData.PWM2_Start2 != m) {
                    newPWM = true;
                    appData.PWM2_Start2 = m;
                }
                // Calculation of Stop2
                m = (int)((appData.PWM2_Cycle * (appData.PWM2_Phase + appData.PWM2_Width2) / 100.0f) + 1);
                if (appData.PWM2_Stop2 != m) {
                    newPWM = true;
                    appData.PWM2_Stop2 = m;
                }
                // Set new timing if needed
                if (newPWM) {
                    // set up Display values
                    APP_LCD_WriteFrequency(appData.PWM2_Foutline,appData.PWM2_Foutpos,appData.PWM2_Foutwidth,appData.PWM2_Frequency);
                    APP_LCD_WritePercent(appData.PWM2_Woutline,appData.PWM2_Woutpos,appData.PWM2_Woutwidth,appData.PWM2_Width);
                    PWM2_SetValues( 
                          appData.PWM2_PreScale
                        , appData.PWM2_Cycle
                        , appData.PWM2_Start1
                        , appData.PWM2_Stop1
                        , appData.PWM2_Start2
                        , appData.PWM2_Stop2
                        );
                }
#endif // ifdef APP_USE_PWM2
#endif // ifdef APP_USE_PWM
                // maybe indirect redirect to return, when no ADC is used
                appData.ADC_PinIdx = 1;
#ifdef APP_USE_ADC
                appData.ADC_Return_AppState = APP_STATE_POLL_DATA;
                appData.state = APP_STATE_START_ADC;
            } else {
                appData.ADC_PinIdx++;
                appData.ADC_Return_AppState = APP_STATE_POLL_DATA;
                appData.state = APP_STATE_START_ADC;
#endif // ifdef APP_USE_ADC
            }
            if (appData.ADC_PinIdx > APP_ADC_NUM_PINS) {
                appData.ADC_PinIdx = 0;
                appData.state = appData.poll_Return_AppState;
            }
            break;
#ifdef APP_USE_ADC
        // Start a ADC read
        case APP_STATE_START_ADC:
            ADC_StartSample(appData.ADC_PinIdx);
            appData.ADC_Waits_Sample = APP_ADC_SAMPLE_CYCLE;
            appData.state = APP_STATE_SAMPLE_ADC;
            break;
        case APP_STATE_SAMPLE_ADC:
            if (--appData.ADC_Waits_Sample <= 0) {
                appData.ADC_Waits_Sample = 0;
                appData.state = APP_STATE_CONVERT_ADC;
            }
            break;
        // Convert initiated ADC read
        case APP_STATE_CONVERT_ADC:
            // the ADC should have enough time to draw Voltage from the sampling port 
            // sampling time should be at least 200ns, which is done in 5 Operations @ 25MHz
            // a loop through the APP State machine is far more than 5 operations
            ADC_ConvertSample();
            appData.state = APP_STATE_READ_ADC;
            break;
        // wait on ADC conversion
        case APP_STATE_READ_ADC:
            if (ADC_ResultIfReady(&appData.ADC_PinValue[appData.ADC_PinIdx - 1])) {
                m = appData.ADC_PinValue[appData.ADC_PinIdx - 1];
                for (i = 1; i < APP_ADC_MEAN_BUFFER; i++) { 
                    m += appData.ADC_PinMean[appData.ADC_PinIdx - 1][i];
                    appData.ADC_PinMean[appData.ADC_PinIdx - 1][i - 1] = appData.ADC_PinMean[appData.ADC_PinIdx - 1][i];
                }
                appData.ADC_PinMean[appData.ADC_PinIdx - 1][APP_ADC_MEAN_BUFFER - 1] = appData.ADC_PinValue[appData.ADC_PinIdx - 1];
                appData.ADC_PinValue[appData.ADC_PinIdx - 1] = m / APP_ADC_MEAN_BUFFER;
                appData.ADC_Value[appData.ADC_PinIdx - 1] = appData.ADC_PinValue[appData.ADC_PinIdx - 1] 
                                                          * appData.ADC_Numerator[appData.ADC_PinIdx - 1] 
                                                          / appData.ADC_Denominator[appData.ADC_PinIdx - 1]
                                                          + appData.ADC_Offset[appData.ADC_PinIdx - 1];
                sprintf(&appData.ADC_Representation[appData.ADC_PinIdx - 1][0], "%.1f%s",appData.ADC_Value[appData.ADC_PinIdx - 1],&appData.ADC_Unit[appData.ADC_PinIdx - 1][0]);
                APP_LCD_WriteString(
                  appData.ADC_outline[appData.ADC_PinIdx - 1]
                , appData.ADC_outpos[appData.ADC_PinIdx - 1]
                , appData.ADC_outwidth[appData.ADC_PinIdx - 1]
                , &appData.ADC_Representation[0][0]
                );
                appData.state = appData.ADC_Return_AppState;
            }
            break;
#endif // ifdef APP_USE_ADC
        case APP_LCD_UPDATE:
            if (APP_CheckTimer()) { break; }
            if (APP_LCD_I2C_Ready) {
                APP_LCD_Update;
                appData.state = appData.LCD_Return_AppState;
            } else {
                if (!APP_LCD_Ready) {
                    // LCD may not be connected
                    appData.state = appData.LCD_Return_AppState;
                }
            }
            break;
        case APP_STATE_ERROR:
            if (APP_CheckTimer()) { break; }
            if (APP_RegisterTimedLED( 200)) {
                LEDR_Clear;
                appData.state = APP_STATE_HOLD;
            }
            break;
        case APP_STATE_HOLD:
            if (APP_CheckTimer()) { break; }
            break;
        // The default state should never be executed. 
        default:
            break;
    }
}