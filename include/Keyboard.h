#ifndef MUSICAL_KEYBOARD_KEYBOARD_H
#define MUSICAL_KEYBOARD_KEYBOARD_H

#include <stdint.h>

#include "hardware/KeyboardButton.h"
#include "hardware/Buffer.h"

typedef enum {
    KEY_0 = 0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_ASTERISK,
    KEY_LATTICE
} KeyboardKeyName;

void keyboardSetKeyPressHandler(KeyboardKeyName key, KeyboardButtonPressHandler keyPressHandler);
uint8_t keyboardStatePolling();
void keyboardInit();
void keyboardDebugEnable();
void keyboardDebugDisable();
Buffer * keyboardGetDebugBuffer();

static struct {
    void (*setKeyPressHandler)(KeyboardKeyName key, KeyboardButtonPressHandler keyPressHandler);
    uint8_t (*statePolling)();
    void (*init)();
    void (*debugEnable)();
    void (*debugDisable)();
    Buffer * (*getDebugBuffer)();
} const Keyboard = {
        keyboardSetKeyPressHandler,
        keyboardStatePolling,
        keyboardInit,
        keyboardDebugEnable,
        keyboardDebugDisable,
        keyboardGetDebugBuffer
};

#endif //MUSICAL_KEYBOARD_KEYBOARD_H
