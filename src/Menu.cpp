#include "Menu.h"

#include <string.h>
#include <stdlib.h>

#include "Command.h"

struct MenuItem;

typedef struct MenuItem {
    MenuItem *next;
    Command *command;
} MenuItem;

struct Menu {
    MenuItem *headItem;
} menu = { NULL };

static int32_t commandSepNameFromArg(const char *commandNameWithArg);
static void strTrim(char *str);
static MenuItem * menuItemCreate(Command *command);
static Command * menuFindItemByName(const char * targetName);

void menuAddItem(Command *command) {
    MenuItem *newItem = menuItemCreate(command);
    newItem->next = menu.headItem;
    menu.headItem = newItem;
}

const char * menuExecuteCommand(char * const commandNameWithArg) {
    strTrim(commandNameWithArg);
    const int32_t sep = commandSepNameFromArg(commandNameWithArg);
    const char *commandArg = NULL;
    if (sep != -1) {
        commandNameWithArg[sep] = '\0';
        commandArg = commandNameWithArg + sep + 1;
    }
    const char * const commandName = commandNameWithArg;
    const Command * command = menuFindItemByName(commandName);
    if (command != NULL)
        return commandExecute(command, commandArg);
    else
        return "Command not found\n";
}

static uint8_t isSpaceChar(char ch) {
    return ch == '\n' || ch == ' ' || ch == '\t' || ch == '\r';
}

static void strTrim(char * const str) {
    const int32_t len = strlen(str);
    for (int32_t i = len - 1; i >= 0; i--) {
        if (isSpaceChar(str[i])) {
            str[i] = '\0';
        } else {
            return;
        }
    }
}

static MenuItem * menuItemCreate(Command *command) {
    MenuItem *menuItem = (MenuItem *) malloc(sizeof(MenuItem));
    menuItem->command = command;
    menuItem->next = NULL;
    return menuItem;
}

static int32_t commandSepNameFromArg(const char * const commandNameWithArg) {
    const int32_t len = strlen(commandNameWithArg);
    for (int32_t i = len - 1; i >= 0; i--) {
        if (commandNameWithArg[i] == ' ') {
            return i;
        }
    }
    return -1;
}

static Command * menuFindItemByName(const char * targetName) {
    MenuItem *current = menu.headItem;
    while (current != NULL) {
        if (strcmp(commandGetName(current->command), targetName) == 0)
            return current->command;
        else
            current = current->next;
    }
    return NULL;
}
