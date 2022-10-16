#include "hardware/Uart.h"

//-------------AVR---------------------
#include <Arduino.h>

#include <avr/io.h>      // Contains all the I/O Register Macros
#include <avr/interrupt.h>

#define USART_BAUDRATE 9600 // Desired Baud Rate
#define BAUD_PRESCALER (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#define ASYNCHRONOUS (0<<UMSEL00) // USART Mode Selection

#define DISABLED    (0<<UPM00)
#define EVEN_PARITY (2<<UPM00)
#define ODD_PARITY  (3<<UPM00)
#define PARITY_MODE  DISABLED // USART Parity Bit Selection

#define ONE_BIT (0<<USBS0)
#define TWO_BIT (1<<USBS0)
#define STOP_BIT ONE_BIT      // USART Stop Bit Selection

#define FIVE_BIT  (0<<UCSZ00)
#define SIX_BIT   (1<<UCSZ00)
#define SEVEN_BIT (2<<UCSZ00)
#define EIGHT_BIT (3<<UCSZ00)
#define DATA_BIT   EIGHT_BIT  // USART Data Bit Selection

#define RX_COMPLETE_INTERRUPT         (1<<RXCIE0)
#define DATA_REGISTER_EMPTY_INTERRUPT (1<<UDRIE0)

volatile uint8_t uartHardwareTransmitBuffer;
volatile uint8_t uartHardwareReceiveBuffer;

void uartHardwareInit() {
    // Set Baud Rate
    UBRR0H = BAUD_PRESCALER >> 8;
    UBRR0L = BAUD_PRESCALER;

    // Set Frame Format
    UCSR0C = ASYNCHRONOUS | PARITY_MODE | STOP_BIT | DATA_BIT;

    // Enable Receiver and Transmitter
    UCSR0B = (1<<RXEN0) | (1<<TXEN0);

    sei();
}

uint8_t uartHardwareTransmitPolling(const uint8_t data, const uint16_t timeOut) {
    const uint16_t timeStart = millis();
    while (( UCSR0A & (1<<UDRE0)) == 0) {
        if (millis() - timeStart >= timeOut)
            return 0;
    }
    UDR0 = data;
    return 1;
}

void uartHardwareTransmitInterrupt(uint8_t buffer) {
    uartHardwareTransmitBuffer = buffer;
    UCSR0B |= DATA_REGISTER_EMPTY_INTERRUPT; // Enables the Interrupt
}

ISR(USART_UDRE_vect) {
    UDR0 = uartHardwareTransmitBuffer;
    UCSR0B &= ~DATA_REGISTER_EMPTY_INTERRUPT;
    uartTransmitIntCallback();
}

uint8_t uartHardwareReceivePolling(uint8_t * const data, const uint16_t timeOut) {
    const uint16_t timeStart = millis();
    while (( UCSR0A & (1<<RXC0)) == 0) {
        if (millis() - timeStart >= timeOut)
            return 0;
    }
    *data = UDR0;
    return 1;
}

void uartHardwareReceiveInterrupt() {
    UCSR0B |= RX_COMPLETE_INTERRUPT;
}

ISR(USART_RX_vect) {
    uartHardwareReceiveBuffer = UDR0;
    UCSR0B &= ~RX_COMPLETE_INTERRUPT;
    uartReceiveIntCallback(uartHardwareReceiveBuffer);
}

void uartHardwareDisableInterrupts() {
    UCSR0B &= ~RX_COMPLETE_INTERRUPT;
    UCSR0B &= ~DATA_REGISTER_EMPTY_INTERRUPT;
}

//-------------AVR---------------------

//-------------STM32-------------------

/*


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart6) {
        check();
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart6) {
        if (strcmp(transmitBuffer, "") != 0) {
            char buf[needToTransmit];
            memcpy(buf, transmitBuffer, needToTransmit);
            HAL_UART_Transmit_IT(&huart6, (uint8_t*) buf, needToTransmit);
            needToTransmit = 0;
            strcpy(transmitBuffer, "");		}
    }
}
*/


//-------------STM32-------------------

