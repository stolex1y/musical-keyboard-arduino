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

/*
 * stm32
 *
#include "tim.h"


void Buzzer_Init (void) {
	HAL_TIM_OC_Start(&htim2,TIM_CHANNEL_1);
	HAL_TIM_PWM_Init(&htim2);
}

void Buzzer_Set_Freq (uint16_t freq) {
	TIM2->PSC = ((2 * HAL_RCC_GetPCLK1Freq()) / (2 * BUZZER_VOLUME_MAX * freq)) - 1;
}

void Buzzer_Set_Volume (uint16_t volume) {
	if (volume > BUZZER_VOLUME_MAX)
		volume = BUZZER_VOLUME_MAX;
	TIM2->CCR1 = volume;
}
 */