#ifndef MUSICAL_KEYBOARD_PINBUTTON_H
#define MUSICAL_KEYBOARD_PINBUTTON_H

#include <stdint.h>

typedef struct PinButton PinButton;

typedef void (*PinButtonPressHandler)();

void pinButtonSetPressHandler(PinButton *pinButton, PinButtonPressHandler pressHandler);
PinButton * pinButtonCreate(uint8_t pin);
void pinButtonEnable(PinButton *pinButton);
void pinButtonDisable(PinButton *pinButton);
uint8_t pinButtonIsEnabled(const PinButton *pinButton);
void pinButtonPolling(PinButton * const button);

static struct {
    void (*setPressHandler)(PinButton *pinButton, PinButtonPressHandler pressHandler);
    PinButton * (*create)(uint8_t pin);
    void (*enable)(PinButton *pinButton);
    void (*disable)(PinButton *pinButton);
    uint8_t (*isEnabled)(const PinButton *pinButton);
    void (*polling)(PinButton *pinButton);
} pinButton = {
        pinButtonSetPressHandler,
        pinButtonCreate,
        pinButtonEnable,
        pinButtonDisable,
        pinButtonIsEnabled,
        pinButtonPolling,
};

#endif //MUSICAL_KEYBOARD_PINBUTTON_H
