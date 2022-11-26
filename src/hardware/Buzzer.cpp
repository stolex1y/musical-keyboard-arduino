#include "hardware/Buzzer.h"

#include "hardware/Timer.h"

#include "Arduino.h"

#define BUZZER_BUF_SIZE 20

static const TimerN usingTim = TIM1;

typedef struct {
    uint16_t freq;
    uint16_t durationMs;
} BuzzerNote;

typedef struct {
    BuzzerNote *data;
    uint16_t size;
} Notes;

static struct Buzzer {
    uint8_t pin;
    Notes nextNotesArray;
    uint16_t nextNoteIdx;
} buzzer = { 0 };

static void playNextNote();
static void stopBuzzer();
static void playNote(uint16_t freq, uint16_t durationMs);

void buzzerInit(const uint8_t pin) {
    buzzer.pin = pin;
    buzzer.nextNoteIdx = 0;
    buzzer.nextNotesArray = {
            (BuzzerNote *) malloc(BUZZER_BUF_SIZE * sizeof(BuzzerNote)),
            0
    };
}

void buzzerPlayNote(const uint16_t freq, const uint16_t durationMs) {
    stopBuzzer();
    playNote(freq, durationMs);
}

void buzzerPlayNotes(const uint16_t freq[], uint16_t size, const uint16_t durationMs[]) {
    stopBuzzer();
    for (uint16_t i = 0; i < size; i++) {
        BuzzerNote *note = buzzer.nextNotesArray.data + i;
        note->durationMs = durationMs[i];
        note->freq = freq[i];
    }
    buzzer.nextNotesArray.size = size;
    buzzer.nextNoteIdx = 0;
    playNextNote();
}

static void playNote(uint16_t freq, uint16_t durationMs) {
    if (freq > 0)
        tone(buzzer.pin, freq);
    else
        noTone(buzzer.pin);

    if (durationMs > 0) {
        timer.setPeriod(usingTim, durationMs);
        timer.setCallback(usingTim, playNextNote);
        timer.start(usingTim);
    } else {
        timer.stop(usingTim);
    }
}

static void stopBuzzer() {
    timer.stop(usingTim);
    buzzer.nextNotesArray.size = 0;
    buzzer.nextNoteIdx = 0;
    noTone(buzzer.pin);
}

static void playNextNote() {
    if (buzzer.nextNotesArray.size == 0 ||
        buzzer.nextNoteIdx == buzzer.nextNotesArray.size) {
        stopBuzzer();
    } else {
        BuzzerNote note = buzzer.nextNotesArray.data[buzzer.nextNoteIdx++];
        playNote(note.freq, note.durationMs);
    }
}