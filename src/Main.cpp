#include <string.h>
#include <inttypes.h>

#include <Arduino.h>

#include "hardware/Buffer.h"
#include "hardware/PinButton.h"
#include "Keyboard.h"
#include "Uart.h"
#include "MusicalKeyboard.h"

#define DEBUG_BUF_SIZE 50
#define BUZZER_PIN 3
#define BUTTON_PIN 4

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

static void printDebugMessage(Buffer *b);
static void toggleDebugMode();

static Buffer *debugBuf;
static uint8_t debugEnabled = 0;
static PinButton *debugButton;

void setup() {
    Uart.init();
    Uart.enableEchoMode();
    MusicalKeyboard.init(BUZZER_PIN);
    setupKeyboard();
    debugBuf = buffer.create(DEBUG_BUF_SIZE);
    debugButton = pinButton.create(BUTTON_PIN);
    pinButton.setPressHandler(debugButton, toggleDebugMode);

    MusicalKeyboard.playNote(DO);
    Uart.println("Started");
}

void loop() {
    Uart.polling();
    Keyboard.statePolling();
    pinButton.polling(debugButton);
    if (debugEnabled) {
        printDebugMessage(Keyboard.getDebugBuffer());
        printDebugMessage(MusicalKeyboard.getDebugBuffer());
    }
}

static void playDo() {
    MusicalKeyboard.playNote(DO);
}

static void playRe() {
    MusicalKeyboard.playNote(RE);
}

static void playMi() {
    MusicalKeyboard.playNote(MI);
}

static void playFa() {
    MusicalKeyboard.playNote(FA);
}

static void playSol() {
    MusicalKeyboard.playNote(SOL);
}

static void playLa() {
    MusicalKeyboard.playNote(LA);
}

static void playSi() {
    MusicalKeyboard.playNote(SI);
}

static void nextOctave() {
    MusicalKeyboard.nextOctave();
}

static void prevOctave() {
    MusicalKeyboard.prevOctave();
}

static void increasePlayingDuration() {
    MusicalKeyboard.increaseDuration(100);
}

static void reducePlayingDuration() {
    MusicalKeyboard.reduceDuration(100);
}

static void playOctave() {
    MusicalKeyboard.playCurrentOctave();
}

static void printDebugMessage(Buffer * const b) {
    char tmp[DEBUG_BUF_SIZE];
    if (buffer.popToBuffer(b, debugBuf, buffer.freeSize(debugBuf) - 1) > 0) {
        buffer.push(debugBuf, '\0');
        buffer.popValues(debugBuf, (uint8_t *) tmp, DEBUG_BUF_SIZE);
        Uart.print(tmp);
    }
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

static void toggleDebugMode() {
    debugEnabled = !debugEnabled;
    if (debugEnabled) {
        MusicalKeyboard.debugEnable();
        Keyboard.debugEnable();
    } else {
        MusicalKeyboard.debugDisable();
        Keyboard.debugDisable();
    }
    Uart.println("Button is pressed");
}