#include "hardware/KeyboardControllerI2C.h"

#include <Wire.h>

uint8_t keyboardControllerReadInput() {
    Wire.beginTransmission(RECEIVER_ADDR);
    Wire.write(INPUT_ADDR);
    Wire.requestFrom(RECEIVER_ADDR, 1);
    Wire.endTransmission();
    return Wire.read();
}

uint8_t keyboardControllerWriteConfig(uint8_t config) {
    Wire.beginTransmission(RECEIVER_ADDR);
    Wire.write(CONFIG_ADDR);
    Wire.write(config);
    return Wire.endTransmission();
}

void keyboardControllerInit() {
    Wire.begin();
}