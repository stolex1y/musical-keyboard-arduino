#include <string.h>
#include <inttypes.h>
#include <Arduino.h>

#include "Uart.h"
#include "Command.h"
#include "Menu.h"
#include "MusicalKeyboard.h"

#define UART_BUF_SIZE 128
#define CMD_RESULT_BUF_SIZE 128

#define BUZZER_PIN 3

static char *key_1_command();

static char *key_2_command();

static char *key_3_command();

static char *key_4_command();

static char *key_5_command();

static char *key_6_command();

static char *key_7_command();

static char *key_plus_command();

static char *key_minus_command();

static char *key_a_command();

static char *key_A_command();

static char *key_enter_command();

static char *toggle_debug_mode();

static void sendMessage(const char *msg);

static void receiveAndSendChar();

static void sendCommandResult(const char *commandResult);

static Command *commands[] = {
        command.create("1", (CommandAction) key_1_command, NONE),
        command.create("2", (CommandAction) key_2_command, NONE),
        command.create("3", (CommandAction) key_3_command, NONE),
        command.create("4", (CommandAction) key_4_command, NONE),
        command.create("5", (CommandAction) key_5_command, NONE),
        command.create("6", (CommandAction) key_6_command, NONE),
        command.create("7", (CommandAction) key_7_command, NONE),
        command.create("+", (CommandAction) key_plus_command, NONE),
        command.create("-", (CommandAction) key_minus_command, NONE),
        command.create("A", (CommandAction) key_A_command, NONE),
        command.create("a", (CommandAction) key_a_command, NONE),
        command.create("", (CommandAction) key_enter_command, NONE),
        command.create("D", (CommandAction) toggle_debug_mode, NONE)
};
static uint8_t uartBuf[UART_BUF_SIZE];
static char cmdResultBuf[CMD_RESULT_BUF_SIZE];
static uint8_t uartBufLast = 0;
static uint8_t debugMode = 1;

void setup() {
    Uart.init();

    for (uint8_t i = 0; i < sizeof(commands) / sizeof(Command *); i++) {
        Menu.addItem(commands[i]);
    }

    MusicalKeyboard.init(BUZZER_PIN);

    sendMessage("Started\n");
    MusicalKeyboard.playNote(DO);
}

void loop() {
    Uart.pooling();

    if (Uart.hasNext()) {
        receiveAndSendChar();
        uartBuf[uartBufLast] = '\0';
        const char *commandResult = Menu.executeCommand((char *) uartBuf);
        sendCommandResult(commandResult);
    }
}

static void sendMessage(const char *msg) {
    Uart.transmit((uint8_t *) msg, strlen(msg));
}

static void receiveAndSendChar() {
    if (Uart.receive(uartBuf + uartBufLast, 1)) {
        uint8_t received = uartBuf[uartBufLast];
        uartBufLast++;
        Uart.transmit(&received, 1);
    }
}

static void sendCommandResult(const char *commandResult) {
    Uart.transmit((uint8_t *) commandResult, strlen(commandResult));
    uartBufLast = 0;
}

static char *key_1_command() {
    MusicalKeyboard.playNote(DO);
    if (debugMode) {
        sprintf(cmdResultBuf, "\nCurrent note: %" PRIu8 "\n", MusicalKeyboard.currentNote());
        return cmdResultBuf;
    } else {
        return "";
    }
}

static char *key_2_command() {
    MusicalKeyboard.playNote(RE);
    if (debugMode) {
        sprintf(cmdResultBuf, "\nCurrent note: %" PRIu8 "\n", MusicalKeyboard.currentNote());
        return cmdResultBuf;
    } else {
        return "";
    }
}

static char *key_3_command() {
    MusicalKeyboard.playNote(MI);
    if (debugMode) {
        sprintf(cmdResultBuf, "\nCurrent note: %" PRIu8 "\n", MusicalKeyboard.currentNote());
        return cmdResultBuf;
    } else {
        return "";
    }
}

static char *key_4_command() {
    MusicalKeyboard.playNote(FA);
    if (debugMode) {
        sprintf(cmdResultBuf, "\nCurrent note: %" PRIu8 "\n", MusicalKeyboard.currentNote());
        return cmdResultBuf;
    } else {
        return "";
    }
}

static char *key_5_command() {
    MusicalKeyboard.playNote(SOL);
    if (debugMode) {
        sprintf(cmdResultBuf, "\nCurrent note: %" PRIu8 "\n", MusicalKeyboard.currentNote());
        return cmdResultBuf;
    } else {
        return "";
    }
}

static char *key_6_command() {
    MusicalKeyboard.playNote(LA);
    if (debugMode) {
        sprintf(cmdResultBuf, "\nCurrent note: %" PRIu8 "\n", MusicalKeyboard.currentNote());
        return cmdResultBuf;
    } else {
        return "";
    }
}

static char *key_7_command() {
    MusicalKeyboard.playNote(SI);
    if (debugMode) {
        sprintf(cmdResultBuf, "\nCurrent note: %" PRIu8 "\n", MusicalKeyboard.currentNote());
        return cmdResultBuf;
    } else {
        return "";
    }
}

static char *key_plus_command() {
    MusicalKeyboard.nextOctave();
    if (debugMode) {
        sprintf(cmdResultBuf, "\nOctave: %" PRIu8 "\n", MusicalKeyboard.currentOctave());
        return cmdResultBuf;
    } else {
        return "";
    }
}

static char *key_minus_command() {
    MusicalKeyboard.prevOctave();
    if (debugMode) {
        sprintf(cmdResultBuf, "\nOctave: %" PRIu8 "\n", MusicalKeyboard.currentOctave());
        return cmdResultBuf;
    } else {
        return "";
    }
}

static char *key_a_command() {
    MusicalKeyboard.reduceDuration(100);
    if (debugMode) {
        sprintf(cmdResultBuf, "\nDuration: %" PRIu16 "\n", MusicalKeyboard.currentDuration());
        return cmdResultBuf;
    } else {
        return "";
    }
}

static char *key_A_command() {
    MusicalKeyboard.increaseDuration(100);
    if (debugMode) {
        sprintf(cmdResultBuf, "\nDuration: %" PRIu16 "\n", MusicalKeyboard.currentDuration());
        return cmdResultBuf;
    } else {
        return "";
    }
}

static char *key_enter_command() {
    MusicalKeyboard.playCurrentOctave();
    if (debugMode) {
        char *bufLast = cmdResultBuf;
        const uint16_t *currentNotes = MusicalKeyboard.currentNotes();
        bufLast += sprintf(bufLast, "\nPlaying notes: ");
        for (uint8_t i = 0; i < 7; i++) {
            bufLast += sprintf(bufLast, "%" PRIu16, currentNotes[i]);
            if (i < 6)
                bufLast += sprintf(bufLast, ", ");
            else
                bufLast += sprintf(bufLast, "\n");
        }
        return cmdResultBuf;
    } else {
        return "";
    }
}

static char *toggle_debug_mode() {
    if (debugMode) {
        debugMode = 0;
        return "\nDebug mode off\n";
    } else {
        debugMode = 1;
        return "\nDebug mode on\n";
    }
}