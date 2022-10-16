#ifndef UART_H
#define UART_H

#include <inttypes.h>

uint16_t uartReceive(uint8_t *buffer, uint16_t size);
uint16_t uartTransmit(const uint8_t *msg, uint16_t msgSize);
uint8_t uartHasNext();
void uartEnableInterruption();
void uartDisableInterruption();
void uartInit();
void uartPooling();
uint8_t uartIsInterruptionEnabled();

static struct UartInterface {
    uint16_t (*receive)(uint8_t *buffer, uint16_t size);
    uint16_t (*transmit)(const uint8_t *msg, uint16_t msgSize);
    uint8_t (*hasNext)();
    void (*enableInterruption)();
    void (*disableInterruption)();
    void (*init)();
    void (*pooling)();
    uint8_t (*isInterruptionEnabled)();
} const Uart = {
        uartReceive,
        uartTransmit,
        uartHasNext,
        uartEnableInterruption,
        uartDisableInterruption,
        uartInit,
        uartPooling,
        uartIsInterruptionEnabled
};

#endif //UART_H
