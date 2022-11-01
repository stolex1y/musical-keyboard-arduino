#include "hardware/KeyboardButton.h"

#include <stdlib.h>

#include <Arduino.h>

#include "hardware/Buffer.h"

#define PRESS_TIME_MS 50
#define RELEASE_TIME_MS 50
#define DEBUG_BUF_SIZE 22

typedef enum ButtonState {
    RELEASE_START,
    RELEASE,
    PRESS_START,
    PRESS
} ButtonState;

struct KeyboardButton {
    uint16_t code;
    KeyboardButtonPressHandler clickHandler;
    uint8_t enabled;
    uint32_t clickStart;
    uint32_t releaseStart;
    ButtonState state;
    Buffer *buffer;
    uint8_t debugModeEnabled;
};

KeyboardButton * keyboardButtonCreate(uint16_t code) {
    KeyboardButton *keyboardButton = (KeyboardButton *) malloc(sizeof(KeyboardButton));
    keyboardButton->enabled = 1;
    keyboardButton->debugModeEnabled = 0;
    keyboardButton->clickHandler = NULL;
    keyboardButton->state = RELEASE;
    keyboardButton->buffer = buffer.create(DEBUG_BUF_SIZE);
    keyboardButton->code = code;
    return keyboardButton;
}

void keyboardButtonSetPressHandler(KeyboardButton *button, KeyboardButtonPressHandler pressHandler) {
    button->clickHandler = pressHandler;
}

void keyboardButtonDisable(KeyboardButton * const keyboardButton) {
    keyboardButton->enabled = 0;
}

void keyboardButtonEnable(KeyboardButton * const keyboardButton) {
    keyboardButton->enabled = 1;
}

uint8_t keyboardButtonIsEnabled(const KeyboardButton * const keyboardButton) {
    return keyboardButton->enabled;
}

void keyboardButtonDebugEnable(KeyboardButton * const keyboardButton) {
    keyboardButton->debugModeEnabled = 1;
}

void keyboardButtonDebugDisable(KeyboardButton * const keyboardButton) {
    keyboardButton->debugModeEnabled = 0;
    buffer.clear(keyboardButton->buffer);
}

Buffer * keyboardButtonGetDebugBuffer(KeyboardButton * const keyboardButton) {
    return keyboardButton->buffer;
}

void keyboardButtonUpdateState(KeyboardButton * const keyboardButton, uint8_t isPressed) {
    if (!keyboardButton->enabled)
        return;
    uint32_t time = millis();

    switch (keyboardButton->state) {
        case RELEASE_START:
            if (isPressed) {
                keyboardButton->state = PRESS;
            } else if (time - keyboardButton->releaseStart > RELEASE_TIME_MS) {
                keyboardButton->state = RELEASE;
            }
            break;
        case RELEASE:
            if (isPressed) {
                keyboardButton->state = PRESS_START;
                keyboardButton->clickStart = time;
            }
            break;
        case PRESS_START:
            if (!isPressed) {
                keyboardButton->state = RELEASE;
            } else if (time - keyboardButton->clickStart > PRESS_TIME_MS) {
                keyboardButton->state = PRESS;
                if (keyboardButton->debugModeEnabled) {
                    char debugMsg[25];
                    sprintf(debugMsg, "Button %" PRIu16 " is pressed\n", keyboardButton->code);
                    buffer.pushValues(keyboardButton->buffer, (uint8_t *) debugMsg, 25);
                }
                if (keyboardButton->clickHandler != NULL) {
                    keyboardButton->clickHandler();
                }
            }
            break;
        case PRESS:
            if (!isPressed) {
                keyboardButton->state = RELEASE_START;
                keyboardButton->releaseStart = time;
            }
            break;
    }
}


