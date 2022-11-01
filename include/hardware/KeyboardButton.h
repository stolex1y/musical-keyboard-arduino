#ifndef KEYBOARDBUTTON_H
#define KEYBOARDBUTTON_H

#include <stdint.h>

#include "hardware/Buffer.h"


typedef struct KeyboardButton KeyboardButton;

typedef void (*KeyboardButtonPressHandler)();

void keyboardButtonSetPressHandler(KeyboardButton *button, KeyboardButtonPressHandler pressHandler);
KeyboardButton * keyboardButtonCreate(uint16_t code);
void keyboardButtonEnable(KeyboardButton *button);
void keyboardButtonDisable(KeyboardButton *button);
uint8_t keyboardButtonIsEnabled(const KeyboardButton *button);
void keyboardButtonUpdateState(KeyboardButton *button, uint8_t isPressed);
void keyboardButtonDebugEnable(KeyboardButton *keyboardButton);
void keyboardButtonDebugDisable(KeyboardButton *keyboardButton);
Buffer * keyboardButtonGetDebugBuffer(KeyboardButton *keyboardButton);

static struct {
    void (*setPressHandler)(KeyboardButton *button, KeyboardButtonPressHandler pressHandler);
    KeyboardButton * (*create)(uint16_t code);
    void (*enable)(KeyboardButton *button);
    void (*disable)(KeyboardButton *button);
    uint8_t (*isEnabled)(const KeyboardButton *button);
    void (*updateState)(KeyboardButton *button, uint8_t newState);
    void (*debugEnable)(KeyboardButton * const keyboardButton);
    void (*debugDisable)(KeyboardButton * const keyboardButton);
    Buffer * (*getDebugBuffer)(KeyboardButton * const keyboardButton);
} keyboardButton = {
        keyboardButtonSetPressHandler,
        keyboardButtonCreate,
        keyboardButtonEnable,
        keyboardButtonDisable,
        keyboardButtonIsEnabled,
        keyboardButtonUpdateState,
        keyboardButtonDebugEnable,
        keyboardButtonDebugDisable,
        keyboardButtonGetDebugBuffer
};

#endif //KEYBOARDBUTTON_H
