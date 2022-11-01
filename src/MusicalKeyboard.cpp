#include "MusicalKeyboard.h"

#include <inttypes.h>
#include <stdio.h>

#include "hardware/Buzzer.h"
#include "Keyboard.h"
#include "hardware/Buffer.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define DURATION_MIN 100
#define DURATION_MAX 5000

#define DEBUG_BUF_SIZE 60

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

    uint8_t debugEnabled;
    uint16_t debugCurrentNote;
    uint16_t *debugCurrentNotes;
    Buffer *debugBuffer;
} musicalKeyboard = {0};

static uint16_t calcNoteFreqByOctave(uint16_t baseFreq, uint8_t octave);

static void key_1_command();
static void key_2_command();
static void key_3_command();
static void key_4_command();
static void key_5_command();
static void key_6_command();
static void key_7_command();
static void key_A_command();
static void key_B_command();
static void key_C_command();
static void key_D_command();
static void key_0_command();

static void debugPrintCurrentOctave();
static void debugPrintCurrentNote();
static void debugPrintCurrentNotes();
static void debugPrintCurrentDuration();
static void getDebugMsgFromKb();

void musicalKeyboardInit(const uint8_t pin) {
    buzzerInit(pin);

    musicalKeyboard.debugBuffer = buffer.create(DEBUG_BUF_SIZE);

    Keyboard.init();
    Keyboard.setKeyPressHandler(KEY_1, key_1_command);
    Keyboard.setKeyPressHandler(KEY_2, key_2_command);
    Keyboard.setKeyPressHandler(KEY_3, key_3_command);
    Keyboard.setKeyPressHandler(KEY_4, key_4_command);
    Keyboard.setKeyPressHandler(KEY_5, key_5_command);
    Keyboard.setKeyPressHandler(KEY_6, key_6_command);
    Keyboard.setKeyPressHandler(KEY_7, key_7_command);
    Keyboard.setKeyPressHandler(KEY_0, key_0_command);
    Keyboard.setKeyPressHandler(KEY_A, key_A_command);
    Keyboard.setKeyPressHandler(KEY_B, key_B_command);
    Keyboard.setKeyPressHandler(KEY_C, key_C_command);
    Keyboard.setKeyPressHandler(KEY_D, key_D_command);

    musicalKeyboard.octave = 5;
    musicalKeyboard.noteDurationMs = 1000;
}

void musicalKeyboardPoll() {
    Keyboard.statePolling();
    if (musicalKeyboard.debugEnabled) {
        getDebugMsgFromKb();
    }
}

void musicalKeyboardNextOctave() {
    musicalKeyboard.octave = MIN(9, musicalKeyboard.octave + 1);
    if (musicalKeyboard.debugEnabled)
        debugPrintCurrentOctave();

}

void musicalKeyboardPrevOctave() {
    musicalKeyboard.octave = MAX(1, musicalKeyboard.octave - 1);
    if (musicalKeyboard.debugEnabled)
        debugPrintCurrentOctave();
}

void musicalKeyboardPlayNote(const Note note) {
    uint16_t currentNote = calcNoteFreqByOctave(notesFreq[note], musicalKeyboard.octave);
    buzzerPlayNote(
            currentNote,
            musicalKeyboard.noteDurationMs
    );
    musicalKeyboard.debugCurrentNote = currentNote;
    if (musicalKeyboard.debugEnabled)
        debugPrintCurrentNote();
}

void musicalKeyboardReduceDuration(const uint16_t deltaMs) {
    if (deltaMs > musicalKeyboard.noteDurationMs) {
        musicalKeyboard.noteDurationMs = DURATION_MIN;
        return;
    }
    musicalKeyboard.noteDurationMs = MAX(DURATION_MIN, musicalKeyboard.noteDurationMs - deltaMs);
    if (musicalKeyboard.debugEnabled)
        debugPrintCurrentDuration();
}

void musicalKeyboardIncreaseDuration(const uint16_t deltaMs) {
    if (musicalKeyboard.noteDurationMs + deltaMs > DURATION_MAX) {
        musicalKeyboard.noteDurationMs = DURATION_MAX;
        return;
    }
    musicalKeyboard.noteDurationMs = MIN(DURATION_MAX, musicalKeyboard.noteDurationMs + deltaMs);
    if (musicalKeyboard.debugEnabled)
        debugPrintCurrentDuration();
}

void musicalKeyboardPlayCurrentOctave() {
    uint16_t noteFreq[7];
    uint16_t notesDuration[7];
    for (uint16_t i = 0; i < 7; i++) {
        notesDuration[i] = musicalKeyboard.noteDurationMs;
        noteFreq[i] = calcNoteFreqByOctave(notesFreq[i], musicalKeyboard.octave);
    }
    buzzerPlayNotes(noteFreq, 7, notesDuration);
    musicalKeyboard.debugCurrentNotes = noteFreq;
    if (musicalKeyboard.debugEnabled)
        debugPrintCurrentNotes();
}


uint16_t musicalKeyboardGetCurrentDuration() {
    return musicalKeyboard.noteDurationMs;
}

uint8_t musicalKeyboardGetCurrentOctave() {
    return musicalKeyboard.octave;
}

uint16_t musicalKeyboardGetCurrentNote() {
    return musicalKeyboard.debugCurrentNote;
}

const uint16_t * musicalKeyboardGetCurrentNotes() {
    return musicalKeyboard.debugCurrentNotes;
}

void musicalKeyboardDebugEnable() {
    musicalKeyboard.debugEnabled = 1;
    Keyboard.debugEnable();
}

void musicalKeyboardDebugDisable() {
    musicalKeyboard.debugEnabled = 0;
    Keyboard.debugDisable();
    buffer.clear(musicalKeyboard.debugBuffer);
}

Buffer * musicalKeyboardGetDebugBuffer() {
    return musicalKeyboard.debugBuffer;
}

static uint16_t calcNoteFreqByOctave(uint16_t baseFreq, uint8_t octave) {
    return baseFreq >> (9 - octave);
}

/*
static float calcNoteFreqByOctave(float baseFreq, uint8_t octave) {
    return baseFreq * (1 << (octave - 1));
}
*/

static void key_1_command() {
    musicalKeyboardPlayNote(DO);
}

static void key_2_command() {
    musicalKeyboardPlayNote(RE);
}

static void key_3_command() {
    musicalKeyboardPlayNote(MI);
}

static void key_4_command() {
    musicalKeyboardPlayNote(FA);
}

static void key_5_command() {
    musicalKeyboardPlayNote(SOL);
}

static void key_6_command() {
    musicalKeyboardPlayNote(LA);
}

static void key_7_command() {
    musicalKeyboardPlayNote(SI);
}

static void key_A_command() {
    musicalKeyboardNextOctave();
}

static void key_B_command() {
    musicalKeyboardPrevOctave();
}

static void key_C_command() {
    musicalKeyboardIncreaseDuration(100);
}

static void key_D_command() {
    musicalKeyboardReduceDuration(100);
}

static void key_0_command() {
    musicalKeyboardPlayCurrentOctave();
}

static void debugPrintCurrentNote() {
    char msg[30];
    uint16_t msgSize = sprintf(msg, "Current note: %" PRIu16 "\n", musicalKeyboard.debugCurrentNote);
    buffer.pushValues(musicalKeyboard.debugBuffer, (uint8_t *) msg, msgSize);
}

static void debugPrintCurrentNotes() {
    char msg[50];
    char *msgLast = msg;
    const uint16_t *currentNotes = MusicalKeyboard.currentNotes();
    msgLast += sprintf(msgLast, "Playing notes: ");
    for (uint8_t i = 0; i < 7; i++) {
        msgLast += sprintf(msgLast, "%" PRIu16, currentNotes[i]);
        if (i < 6)
            msgLast += sprintf(msgLast, ", ");
        else
            msgLast += sprintf(msgLast, "\n");
    }
    buffer.pushValues(musicalKeyboard.debugBuffer, (uint8_t *) msg, msgLast - msg);
}

static void debugPrintCurrentDuration() {
    char msg[30];
    uint16_t msgSize = sprintf(msg, "Duration: %" PRIu16 "\n", musicalKeyboard.noteDurationMs);
    buffer.pushValues(musicalKeyboard.debugBuffer, (uint8_t *) msg, msgSize);
}

static void debugPrintCurrentOctave() {
    char msg[30];
    uint16_t msgSize = sprintf(msg, "Octave: %" PRIu8 "\n", musicalKeyboard.octave);
    buffer.pushValues(musicalKeyboard.debugBuffer, (uint8_t *) msg, msgSize);
}

static void getDebugMsgFromKb() {
    const uint8_t tempSize = 50;
    uint8_t temp[tempSize];
    uint16_t kbDebugMsgSize = MIN(buffer.size(Keyboard.getDebugBuffer()), tempSize);
    if (kbDebugMsgSize > 0) {
        buffer.popValues(Keyboard.getDebugBuffer(), temp, kbDebugMsgSize);
        buffer.pushValues(musicalKeyboard.debugBuffer, temp, kbDebugMsgSize);
    }
}

