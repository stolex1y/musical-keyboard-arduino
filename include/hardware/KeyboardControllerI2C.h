#ifndef MUSICAL_KEYBOARD_KEYBOARDCONTROLLERI2C_H
#define MUSICAL_KEYBOARD_KEYBOARDCONTROLLERI2C_H

#include <stdint.h>

#define RECEIVER_ADDR 0xE2
#define INPUT_ADDR 0x00
#define OUTPUT_ADDR 0x01
#define POLARITY_INVERSION_ADDR 0x02
#define CONFIG_ADDR 0x03

uint8_t keyboardControllerReadInput();
uint8_t keyboardControllerWriteConfig(uint8_t config);
void keyboardControllerInit();

static struct {
    uint8_t (*readInput)();
    uint8_t (*writeConfig)(uint8_t config);
    void (*init)();
} const KeyboardControllerI2C = {
        keyboardControllerReadInput,
        keyboardControllerWriteConfig,
        keyboardControllerInit
};

#endif //MUSICAL_KEYBOARD_KEYBOARDCONTROLLERI2C_H
