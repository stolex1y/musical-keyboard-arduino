#include <string.h>
#include <inttypes.h>

#include <Arduino.h>

#include "hardware/Buffer.h"
#include "Uart.h"
#include "MusicalKeyboard.h"
#include "Keyboard.h"

#define DEBUG_BUF_SIZE 60
#define BUZZER_PIN 3

static void setupKeyboard();
static void playDo();
static void playRe();
static void playMi();
static void playFa();
static void playSol();
static void playLa();
static void playSi();
static void nextOctave();
static void prevOctave();
static void increasePlayingDuration();
static void reducePlayingDuration();
static void playOctave();

static void enableDebugMode();
static void disableDebugMode();

static Buffer *debugBuf;
static uint8_t debugEnabled = 0;

void setup() {
    Uart.init();
    Uart.enableEchoMode();
    MusicalKeyboard.init(BUZZER_PIN);
    setupKeyboard();
    debugBuf = buffer.create(DEBUG_BUF_SIZE);

    MusicalKeyboard.playNote(DO);
    Uart.println("Started");
    enableDebugMode();
}

void loop() {
    Uart.polling();
    Keyboard.statePolling();
    if (debugEnabled) {
        char tmp[DEBUG_BUF_SIZE];

        if (buffer.toBuffer(Keyboard.getDebugBuffer(), debugBuf, buffer.freeSize(debugBuf) - 1) > 0) {
            buffer.push(debugBuf, '\0');
            buffer.popValues(debugBuf, (uint8_t *) tmp, DEBUG_BUF_SIZE);
            Uart.print(tmp);
        }

        if (buffer.toBuffer(MusicalKeyboard.getDebugBuffer(), debugBuf, buffer.freeSize(debugBuf) - 1) > 0) {
            buffer.push(debugBuf, '\0');
            buffer.popValues(debugBuf, (uint8_t *) tmp, DEBUG_BUF_SIZE);
            Uart.print(tmp);
        }
    }
}

static void playDo() {
    musicalKeyboardPlayNote(DO);
}

static void playRe() {
    musicalKeyboardPlayNote(RE);
}

static void playMi() {
    musicalKeyboardPlayNote(MI);
}

static void playFa() {
    musicalKeyboardPlayNote(FA);
}

static void playSol() {
    musicalKeyboardPlayNote(SOL);
}

static void playLa() {
    musicalKeyboardPlayNote(LA);
}

static void playSi() {
    musicalKeyboardPlayNote(SI);
}

static void nextOctave() {
    musicalKeyboardNextOctave();
}

static void prevOctave() {
    musicalKeyboardPrevOctave();
}

static void increasePlayingDuration() {
    musicalKeyboardIncreaseDuration(100);
}

static void reducePlayingDuration() {
    musicalKeyboardReduceDuration(100);
}

static void playOctave() {
    musicalKeyboardPlayCurrentOctave();
}

static void getDebugMsgFromKeyboard() {
    buffer.toBuffer(Keyboard.getDebugBuffer(), debugBuf, buffer.freeSize(debugBuf));
    /*const uint8_t tempSize = 50;
    uint8_t temp[tempSize];
    uint16_t kbDebugMsgSize = MIN(buffer.size(Keyboard.getDebugBuffer()), tempSize);
    if (kbDebugMsgSize > 0) {
        buffer.popValues(Keyboard.getDebugBuffer(), bufferPopValues(debugBuf,), kbDebugMsgSize);
        buffer.pushValues(musicalKeyboard.debugBuffer, temp, kbDebugMsgSize);
    }*/
}

static void setupKeyboard() {
    Keyboard.init();
    Keyboard.setKeyPressHandler(KEY_1, playDo);
    Keyboard.setKeyPressHandler(KEY_2, playRe);
    Keyboard.setKeyPressHandler(KEY_3, playMi);
    Keyboard.setKeyPressHandler(KEY_4, playFa);
    Keyboard.setKeyPressHandler(KEY_5, playSol);
    Keyboard.setKeyPressHandler(KEY_6, playLa);
    Keyboard.setKeyPressHandler(KEY_7, playSi);
    Keyboard.setKeyPressHandler(KEY_0, playOctave);
    Keyboard.setKeyPressHandler(KEY_A, nextOctave);
    Keyboard.setKeyPressHandler(KEY_B, prevOctave);
    Keyboard.setKeyPressHandler(KEY_C, increasePlayingDuration);
    Keyboard.setKeyPressHandler(KEY_D, reducePlayingDuration);
}

static void enableDebugMode() {
    debugEnabled = 1;
    MusicalKeyboard.debugEnable();
    Keyboard.debugEnable();
}

static void disableDebugMode() {
    debugEnabled = 0;
    MusicalKeyboard.debugDisable();
    Keyboard.debugDisable();
}