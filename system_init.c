/*
 * File:   system_init.c
 * Author: patrick
 *
 * Created on 2014-09-13
 */

// PIC32MX250F128B Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>

// #pragma's see configuration_bits.cpp

#include "system_init.h"
#include "system_config.h"

void TMR_Initialize(void) {
    // Timer1 System Clock
    // Stop the timer
    PLIB_TMR_Stop(APP_TMR_CLOCK);
    // Set the prescaler, and set the clock source as internal
    PLIB_TMR_PrescaleSelect(APP_TMR_CLOCK, APP_TMR_CLKPRESCALE);
    PLIB_TMR_ClockSourceSelect(APP_TMR_CLOCK, TMR_CLOCK_SOURCE_PERIPHERAL_CLOCK);
    // Clear the timer
    PLIB_TMR_Counter16BitClear(APP_TMR_CLOCK);
    // and preset interval
    PLIB_TMR_Period16BitSet(APP_TMR_CLOCK, APP_TMR_CLKINTERVAL);
}

#ifndef APP_USE_DIO
void LEDS_Initialize(void) {
    // red LED
    LEDR_Clear;
    LEDR_Direction;
    LEDR_Mode;
    LEDR_Remap;
    LEDR_OD;
    // yellow LED
    LEDY_Clear;
    LEDY_Direction;
    LEDY_Mode;
    LEDY_Remap;
    LEDY_OD;
    // green LED
    LEDG_Clear;
    LEDG_Direction;
    LEDG_Mode;
    LEDG_Remap;
    LEDG_OD;
    // blue LED
    LEDB_Clear;
    LEDB_Direction;
    LEDB_Mode;
    LEDB_Remap;
    LEDB_OD;
    // white LED
    LEDW_Clear;
    LEDW_Direction;
    LEDW_Mode;
    LEDW_Remap;
    LEDW_OD;
}
#endif // ifndef APP_USE_DIO

#ifdef APP_USE_UART
void UART_Initialize(void) {
    // UART 2 RX
    PLIB_USART_Disable(APP_UART_RX_ID);
    // Prepare Port
    PLIB_PORTS_PinClear(APP_UART_RX_PORTS_ID, APP_UART_RX_PORT_CHANNEL, APP_UART_RX_PORT_PIN);
    PLIB_PORTS_PinDirectionInputSet(APP_UART_RX_PORTS_ID, APP_UART_RX_PORT_CHANNEL, APP_UART_RX_PORT_PIN);
    PLIB_PORTS_RemapInput(APP_UART_RX_PORTS_ID, APP_UART_RX_REMAP_FUNC, APP_UART_RX_REMAP_PIN);
    // vv TX on same UART
    // UART 2 TX
    // Prepare Port
    PLIB_PORTS_PinClear(APP_UART_TX_PORTS_ID, APP_UART_TX_PORT_CHANNEL, APP_UART_TX_PORT_PIN);
    PLIB_PORTS_PinDirectionOutputSet(APP_UART_TX_PORTS_ID, APP_UART_TX_PORT_CHANNEL, APP_UART_TX_PORT_PIN);
    PLIB_PORTS_RemapOutput(APP_UART_TX_PORTS_ID, APP_UART_TX_REMAP_FUNC, APP_UART_TX_REMAP_PIN);
    PLIB_PORTS_PinSet(APP_UART_TX_PORTS_ID, APP_UART_TX_PORT_CHANNEL, APP_UART_TX_PORT_PIN);
    // ^^ TX on same UART
    // UART (RX/TX) Config
    PLIB_USART_OperationModeSelect(APP_UART_RX_ID, APP_UART_RX_OPER);
    PLIB_USART_HandshakeModeSelect(APP_UART_RX_ID, APP_UART_RX_HAND);
    PLIB_USART_BaudRateSet(APP_UART_RX_ID, APP_PBCLK_FREQ, APP_UART_RX_BAUD);
    // Select 8 data bits, No parity and one stop bit
    PLIB_USART_LineControlModeSelect(APP_UART_RX_ID, APP_UART_RX_MODE);
    // vv TX on same UART
    PLIB_USART_TransmitterInterruptModeSelect(APP_UART_TX_ID, USART_TRANSMIT_FIFO_EMPTY);
    // ^^ TX on same UART
    PLIB_USART_ReceiverInterruptModeSelect(APP_UART_RX_ID, USART_RECEIVE_FIFO_ONE_CHAR);
    
    //// UART 1 TX
    //PLIB_USART_Disable(APP_UART_TX_ID);
    //// Prepare Port
    //PLIB_PORTS_PinClear(APP_UART_TX_PORTS_ID, APP_UART_TX_PORT_CHANNEL, APP_UART_TX_PORT_PIN);
    //PLIB_PORTS_PinDirectionOutputSet(APP_UART_TX_PORTS_ID, APP_UART_TX_PORT_CHANNEL, APP_UART_TX_PORT_PIN);
    //PLIB_PORTS_RemapOutput(APP_UART_TX_PORTS_ID, APP_UART_TX_REMAP_FUNC, APP_UART_TX_REMAP_PIN);
    //// UART (TX) Config
    //PLIB_USART_OperationModeSelect(APP_UART_TX_ID, APP_UART_TX_OPER);
    //PLIB_USART_HandshakeModeSelect(APP_UART_TX_ID, APP_UART_TX_HAND);
    //PLIB_USART_BaudRateSet(APP_UART_TX_ID, APP_PBCLK_FREQ, APP_UART_TX_BAUD);
    //// Select 8 data bits, No parity and one stop bit
    //PLIB_USART_LineControlModeSelect(APP_UART_TX_ID, APP_UART_TX_MODE);
    //PLIB_USART_TransmitterInterruptModeSelect(APP_UART_TX_ID, UART_TRANSMIT_FIFO_EMPTY);
    
    // enabling UARTs in SYS_Startup
}
#endif // of ifdef APP_USE_UART

void INT_Initialize(void) {
    // enable the multi vector
    PLIB_INT_MultiVectorSelect( APP_INT_ID );
    // set priority LCD
    APP_LCD_InterruptPriority(APP_INT_ID);
    // set priority and enable Timer1
    PLIB_INT_VectorPrioritySet(APP_INT_ID, INT_VECTOR_T1, INT_PRIORITY_LEVEL3);
    PLIB_INT_VectorSubPrioritySet(APP_INT_ID, INT_VECTOR_T1, INT_SUBPRIORITY_LEVEL0);
#ifdef APP_USE_UART
    // set priority and enable UART RX
    PLIB_INT_VectorPrioritySet(APP_INT_ID, INT_VECTOR_UART2, INT_PRIORITY_LEVEL2);
    PLIB_INT_VectorSubPrioritySet(APP_INT_ID, INT_VECTOR_UART2, INT_SUBPRIORITY_LEVEL0);
    // set priority and enable UART TX
    PLIB_INT_VectorPrioritySet(APP_INT_ID, INT_VECTOR_UART1, INT_PRIORITY_LEVEL1);
    PLIB_INT_VectorSubPrioritySet(APP_INT_ID, INT_VECTOR_UART1, INT_SUBPRIORITY_LEVEL0);
#endif // of ifdef APP_USE_UART
    // Enable the global interrupts
    PLIB_INT_Enable(APP_INT_ID);
    APP_LCD_InterruptEnable(APP_INT_ID);
    PLIB_INT_SourceEnable(APP_INT_ID, INT_SOURCE_TIMER_1);
#ifdef APP_USE_UART
    PLIB_INT_SourceEnable(APP_INT_ID, INT_SOURCE_USART_2_ERROR);
    // vv TX on same UART
    PLIB_INT_SourceEnable(APP_INT_ID, INT_SOURCE_USART_2_TRANSMIT);
    // ^^ TX on same UART
    PLIB_INT_SourceEnable(APP_INT_ID, INT_SOURCE_USART_2_RECEIVE);
    
    //PLIB_INT_SourceEnable(APP_INT_ID, INT_SOURCE_UART_1_ERROR);
    //PLIB_INT_SourceEnable(APP_INT_ID, INT_SOURCE_UART_1_TRANSMIT);
#endif
}

void SYS_Startup(void) {
    // LCD I2C com
    APP_LCD_SYS_Startup(APP_INT_ID);
    // system clock 
    PLIB_INT_SourceFlagClear(APP_INT_ID, INT_SOURCE_TIMER_1);
    PLIB_TMR_Start(APP_TMR_CLOCK);
#ifdef APP_USE_UART
    // enable UARTs
    //PLIB_INT_SourceFlagClear(APP_INT_ID, INT_SOURCE_UART_1_ERROR);
    //PLIB_INT_SourceFlagClear(APP_INT_ID, INT_SOURCE_UART_1_TRANSMIT);
    //PLIB_UART_Enable(APP_UART_TX_ID);
    //PLIB_UART_TransmitterEnable(APP_UART_TX_ID);
    // if there is a hardware shortcut Transmitter should be ready before Receiver
    PLIB_INT_SourceFlagClear(APP_INT_ID, INT_SOURCE_USART_2_ERROR);
    // vv TX on same UART
    PLIB_INT_SourceFlagClear(APP_INT_ID, INT_SOURCE_USART_2_TRANSMIT);
    // ^^ TX on same UART
    PLIB_INT_SourceFlagClear(APP_INT_ID, INT_SOURCE_USART_2_RECEIVE);
    // Transmitter should be disabled as long there is no data to send
    // this also keeps the TX under PORT control,
    // which should provide a high signal for the line
    PLIB_USART_TransmitterDisable(APP_UART_TX_ID);
    PLIB_USART_Enable(APP_UART_RX_ID);
    //PLIB_UART_ReceiverEnable(APP_UART_RX_ID);
#endif
}

/* Initialize the System */
void SYS_Initialize ( void *data )
{
    // Initialize all modules and the application
    // Timer initializations
    TMR_Initialize();
    // Set outputting Ports to Output (default is Input), clear analog Input
#ifdef APP_USE_DIO
    DIO_Initialize();
#else // ifdef APP_USE_DIO
    LEDS_Initialize();
#endif // else APP_USE_DIO
    // init LCD
    APP_LCD_Initialize(APP_PBCLK_FREQ);
#ifdef APP_USE_UART
    // init UART
    UART_Initialize();
#endif
#ifdef APP_USE_ADC
    ADC_Initialize();
#endif
    // Initialize the Application
    APP_Initialize();
    
    // Initialize the interrupt system (after all other init's)
    INT_Initialize();
    // startup system
    SYS_Startup();
}
