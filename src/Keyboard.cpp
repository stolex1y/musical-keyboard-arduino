#include "Keyboard.h"

#include "hardware/KeyboardControllerI2C.h"
#include "hardware/KeyboardButton.h"
#include "hardware/Buffer.h"

#define KB_HEIGHT 4
#define KB_WIDTH 4
#define DEBUG_BUF_SIZE 30

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

static const KeyboardKeyName keyboardKeysNames[KB_HEIGHT][KB_WIDTH] = {
        { KEY_1, KEY_2, KEY_3, KEY_A, },
        { KEY_4, KEY_5, KEY_6, KEY_B, },
        { KEY_7, KEY_8, KEY_9, KEY_C, },
        { KEY_ASTERISK, KEY_0, KEY_LATTICE, KEY_D, },
};

static struct Keyboard {
    KeyboardButton * keys[KB_HEIGHT * KB_WIDTH];
    Buffer *buffer;
    uint8_t debugModeEnabled;
} keyboard = {};

static void updateDebugMessages();

void keyboardInit() {
    for (uint8_t i = 0; i < KB_HEIGHT; i++) {
        for (uint8_t j = 0; j < KB_WIDTH; j++) {
            KeyboardKeyName keyName = keyboardKeysNames[i][j];
            KeyboardButton *key = keyboardButton.create(keyName);
            keyboard.keys[keyName] = key;
        }
    }
    keyboard.buffer = buffer.create(DEBUG_BUF_SIZE);
    KeyboardControllerI2C.init();
}

uint8_t keyboardStatePolling() {
    for (uint8_t row = 0; row < KB_HEIGHT; row++) {
        if (KeyboardControllerI2C.writeConfig(~(1 << (7 - row)))) // check #i row
            return 1;
        uint8_t kbState = KeyboardControllerI2C.readInput();
        for (uint8_t column = 0; column < KB_WIDTH; column++) {
            uint8_t newKeyState = kbState & (0x8 >> column);
            KeyboardKeyName keyName = keyboardKeysNames[row][column];
            KeyboardButton * key = keyboard.keys[keyName];
            keyboardButton.updateState(key, !newKeyState);
        }
    }
    if (keyboard.debugModeEnabled)
        updateDebugMessages();
    return 0;
}

void keyboardSetKeyPressHandler(KeyboardKeyName key, KeyboardButtonPressHandler keyPressHandler) {
    keyboardButton.setPressHandler(keyboard.keys[key], keyPressHandler);
}

void keyboardDebugEnable() {
    keyboard.debugModeEnabled = 1;
    for (uint8_t i = 0; i < KB_WIDTH * KB_HEIGHT; i++) {
        keyboardButton.debugEnable(keyboard.keys[i]);
    }
}

void keyboardDebugDisable() {
    keyboard.debugModeEnabled = 0;
    for (uint8_t i = 0; i < KB_WIDTH * KB_HEIGHT; i++) {
        keyboardButton.debugDisable(keyboard.keys[i]);
    }
    buffer.clear(keyboard.buffer);
}

Buffer * keyboardGetDebugBuffer() {
    return keyboard.buffer;
}

static void updateDebugMessages() {
    const uint8_t tempSize = 50;
    uint8_t temp[tempSize];
    for (uint8_t i = 0; i < KB_HEIGHT * KB_WIDTH; i++) {
        KeyboardButton * key = keyboard.keys[i];
        Buffer * keyDebugBuf = keyboardButton.getDebugBuffer(key);
        while (buffer.hasValues(keyDebugBuf)) {
            uint16_t msgSize = MIN(buffer.size(keyDebugBuf), tempSize);
            buffer.popValues(keyDebugBuf, temp, msgSize);
            buffer.pushValues(keyboard.buffer, temp, msgSize);
        }
    }
}