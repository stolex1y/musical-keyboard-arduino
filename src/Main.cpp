#include <string.h>
#include <inttypes.h>

#include <Arduino.h>

#include "Uart.h"
#include "MusicalKeyboard.h"

//#define UART_BUF_SIZE 128
#define DEBUG_BUF_SIZE 60

#define BUZZER_PIN 3

static void sendMessage(const char *msg);
static void receiveAndSendChar();
static void sendCommandResult(const char *commandResult);

//static uint8_t uartBuf[UART_BUF_SIZE];
static uint8_t debugBuf[DEBUG_BUF_SIZE];
static uint8_t uartBufLast = 0;
static uint8_t debugEnabled = 0;

void setup() {
//    Uart.init();
    Serial.begin(9600);
    MusicalKeyboard.init(BUZZER_PIN);
    MusicalKeyboard.playNote(DO);
    Serial.println("Started");
//    sendMessage("Started\n");
    MusicalKeyboard.debugEnable();
    debugEnabled = 1;
    delay(5);
}

void loop() {
//    Uart.polling();
    MusicalKeyboard.poll();
    if (debugEnabled) {
        uint16_t msgLen = buffer.popValues(MusicalKeyboard.getDebugBuffer(), debugBuf, DEBUG_BUF_SIZE - 1);
        if (msgLen != 0) {
            debugBuf[msgLen] = '\0';
            Serial.print((char *) debugBuf);
        }
    }
}
/*
static void sendMessage(const char *msg) {
    Uart.transmit((uint8_t *) msg, strlen(msg));
}

static void receiveAndSendChar() {
    if (Uart.receive(uartBuf + uartBufLast, 1)) {
        uint8_t received = uartBuf[uartBufLast];
        uartBufLast++;
        Uart.transmit(&received, 1);
    }
}*/
