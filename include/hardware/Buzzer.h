#ifndef MUSICAL_KEYBOARD_BUZZER_H
#define MUSICAL_KEYBOARD_BUZZER_H

#include <stdint.h>

void buzzerInit(uint8_t pin);
void buzzerPlayNote(uint16_t freq, uint16_t durationMs);
void buzzerPlayNotes(const uint16_t freq[], uint16_t size, const uint16_t durationMs[]);

static struct {
    void (*init)(uint8_t pin);
    void (*playNote)(uint16_t freq, uint16_t durationMs);
    void (*playNotes)(const uint16_t freq[], uint16_t size, const uint16_t durationMs[]);
} const Buzzer = {
        buzzerInit,
        buzzerPlayNote,
        buzzerPlayNotes
};
/*


void Buzzer_Play (uint32_t* melody, uint32_t* delays, uint16_t len);*/

#endif //MUSICAL_KEYBOARD_BUZZER_H
