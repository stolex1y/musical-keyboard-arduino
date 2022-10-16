#include "MusicalKeyboard.h"

#include "hardware/Buzzer.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define DURATION_MIN 100
#define DURATION_MAX 5000

static const uint16_t notesFreq[] = {
        [DO] = 4186,
        [RE] = 4699,
        [MI] = 5274,
        [FA] = 5588,
        [SOL] = 6272,
        [LA] = 7040,
        [SI] = 7902,
};

/*static const float notesFreq[] = {
        [DO] = 16.352f,
        [RE] = 18.354f,
        [MI] = 20.602f,
        [FA] = 21.827f,
        [SOL] = 24.500f,
        [LA] = 27.500f,
        [SI] = 30.868f,
};*/

struct MusicalKeyboard {
    uint16_t noteDurationMs;
    uint8_t octave;

    uint16_t debug_currentNote;
    uint16_t *debug_currentNotes;
} musicalKeyboard = {0};

static uint16_t calcNoteFreqByOctave(uint16_t baseFreq, uint8_t octave);

void musicalKeyboardInit(const uint8_t pin) {
    buzzerInit(pin);
    musicalKeyboard.octave = 5;
    musicalKeyboard.noteDurationMs = 1000;
}

void musicalKeyboardNextOctave() {
    musicalKeyboard.octave = MIN(9, musicalKeyboard.octave + 1);
}

void musicalKeyboardPrevOctave() {
    musicalKeyboard.octave = MAX(1, musicalKeyboard.octave - 1);
}

void musicalKeyboardPlayNote(const Note note) {
    uint16_t currentNote = calcNoteFreqByOctave(notesFreq[note], musicalKeyboard.octave);
    buzzerPlayNote(
            currentNote,
            musicalKeyboard.noteDurationMs
    );
    musicalKeyboard.debug_currentNote = currentNote;
}

void musicalKeyboardReduceDuration(const uint16_t deltaMs) {
    if (deltaMs > musicalKeyboard.noteDurationMs) {
        musicalKeyboard.noteDurationMs = DURATION_MIN;
        return;
    }
    musicalKeyboard.noteDurationMs = MAX(DURATION_MIN, musicalKeyboard.noteDurationMs - deltaMs);
}

void musicalKeyboardIncreaseDuration(const uint16_t deltaMs) {
    if (musicalKeyboard.noteDurationMs + deltaMs > DURATION_MAX) {
        musicalKeyboard.noteDurationMs = DURATION_MAX;
        return;
    }
    musicalKeyboard.noteDurationMs = MIN(DURATION_MAX, musicalKeyboard.noteDurationMs + deltaMs);
}


static uint16_t noteFreq[7];
static uint16_t notesDuration[7];
void musicalKeyboardPlayCurrentOctave() {
    for (uint16_t i = 0; i < 7; i++) {
        notesDuration[i] = musicalKeyboard.noteDurationMs;
        noteFreq[i] = calcNoteFreqByOctave(notesFreq[i], musicalKeyboard.octave);
    }
    buzzerPlayNotes(noteFreq, 7, notesDuration);
    musicalKeyboard.debug_currentNotes = noteFreq;
}

uint16_t musicalKeyboardGetCurrentDuration() {
    return musicalKeyboard.noteDurationMs;
}

uint8_t musicalKeyboardGetCurrentOctave() {
    return musicalKeyboard.octave;
}

uint16_t musicalKeyboardGetCurrentNote() {
    return musicalKeyboard.debug_currentNote;
}

const uint16_t *musicalKeyboardGetCurrentNotes() {
    return musicalKeyboard.debug_currentNotes;
}

static uint16_t calcNoteFreqByOctave(uint16_t baseFreq, uint8_t octave) {
    return baseFreq >> (9 - octave);
}

/*
static float calcNoteFreqByOctave(float baseFreq, uint8_t octave) {
    return baseFreq * (1 << (octave - 1));
}
*/
