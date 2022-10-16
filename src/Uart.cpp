#include "Uart.h"

#include <string.h>

#include "hardware/Buffer.h"
#include "hardware/Uart.h"

#define TX_BUFFER_SIZE 128
#define RX_BUFFER_SIZE 64

static Buffer *txBuffer;
static Buffer *rxBuffer;
static volatile uint8_t interruptionsEnabled = 0;
static volatile uint8_t transmitIntEnd = 1;

static void hardwareReceive();

static void hardwareReceiveIT();

static void hardwareTransmit();

static void hardwareTransmitIT();

uint16_t uartTransmit(const uint8_t *const msg, const uint16_t msgSize) {
    if (msg == NULL)
        return 0;
    const uint16_t transmitted = bufferPushValues(txBuffer, msg, msgSize);
    if (interruptionsEnabled) {
        hardwareTransmitIT();
    }
    return transmitted;
}

uint8_t uartHasNext() {
    return bufferHasValues(rxBuffer);
}

void uartInit() {
    txBuffer = bufferCreate(TX_BUFFER_SIZE);
    rxBuffer = bufferCreate(RX_BUFFER_SIZE);
    UartHardware.init();
}

void uartPooling() {
    if (!interruptionsEnabled) {
        hardwareTransmit();
        hardwareReceive();
    }
}

uint16_t uartReceive(uint8_t *const buffer, const uint16_t size) {
    const uint16_t received = bufferPopValues(rxBuffer, buffer, size);
    if (interruptionsEnabled) {
        hardwareReceiveIT();
    }
    return received;
}

void uartEnableInterruption() {
    interruptionsEnabled = 1;
    transmitIntEnd = 1;
    hardwareTransmitIT();
    hardwareReceiveIT();
}

void uartDisableInterruption() {
    interruptionsEnabled = 0;
    UartHardware.disableInterrupts();
}

uint8_t uartIsInterruptionEnabled() {
    return interruptionsEnabled;
}

static void hardwareReceive() {
    if (bufferGetFreeSize(rxBuffer) > 0) {
        uint8_t data = 0;
        if (UartHardware.receivePolling(&data, 5)) {
            bufferPush(rxBuffer, data);
        }
    }
}

static void hardwareReceiveIT() {
    UartHardware.receiveInterrupt();
}

static void hardwareTransmitIT() {
    if (!transmitIntEnd)
        return;

    if (bufferHasValues(txBuffer)) {
        transmitIntEnd = 0;
        uint8_t data = bufferPop(txBuffer);
        UartHardware.transmitInterrupt(data);
    }
}

static void hardwareTransmit() {
    if (bufferHasValues(txBuffer)) {
        const uint8_t byte = bufferPeek(txBuffer);
        if (UartHardware.transmitPolling(byte, 5))
            bufferPop(txBuffer);
    }
}

extern void uartReceiveIntCallback(uint8_t data) {
    bufferPush(rxBuffer, data);
    if (interruptionsEnabled)
        hardwareReceiveIT();
}

extern void uartTransmitIntCallback() {
    transmitIntEnd = 1;
    if (interruptionsEnabled)
        hardwareTransmitIT();
}