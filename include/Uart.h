#ifndef UART_H
#define UART_H

#include <inttypes.h>

void uartDisableEchoMode();
void uartEnableEchoMode();
uint16_t uartReceive(uint8_t *buffer, uint16_t size);
uint16_t uartPrintf(const char *format, ...);
uint16_t uartPrint(const char *msg);
uint16_t uartPrintln(const char *msg);
uint16_t uartTransmit(const uint8_t *msg, uint16_t msgSize);
uint8_t uartHasNext();
void uartEnableInterruption();
void uartDisableInterruption();
void uartInit();
void uartPolling();
uint8_t uartIsInterruptionEnabled();

static struct UartInterface {
    void (*disableEchoMode)();
    void (*enableEchoMode)();
    uint16_t (*receive)(uint8_t *buffer, uint16_t size);
    uint16_t (*printf)(const char *format, ...);
    uint16_t (*print)(const char *msg);
    uint16_t (*println)(const char *msg);
    uint16_t (*transmit)(const uint8_t *msg, uint16_t msgSize);
    uint8_t (*hasNext)();
    void (*enableInterruption)();
    void (*disableInterruption)();
    void (*init)();
    void (*polling)();
    uint8_t (*isInterruptionEnabled)();
} const Uart = {
        uartDisableEchoMode,
        uartEnableEchoMode,
        uartReceive,
        uartPrintf,
        uartPrint,
        uartPrintln,
        uartTransmit,
        uartHasNext,
        uartEnableInterruption,
        uartDisableInterruption,
        uartInit,
        uartPolling,
        uartIsInterruptionEnabled
};

#endif //UART_H
