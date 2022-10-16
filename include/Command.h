#ifndef COMAND_H
#define COMAND_H

typedef struct Command Command;
typedef const char * (*CommandAction)(const void *);
typedef enum OnOffStatus { ON, OFF } OnOffStatus;
typedef enum CommandArgument { INT, ON_OFF, NONE } CommandArgument;

Command * commandCreate(const char *name, CommandAction action, CommandArgument argType);
const char * commandExecute(const Command *command, const char *argument);
void commandSetAction(Command *command, CommandAction action);
const char * commandGetName(const Command *command);
CommandAction commandGetAction(const Command * command);

static struct {
    Command * (*create)(const char *name, CommandAction action, CommandArgument argType);
    const char * (*execute)(const Command *command, const char *argument);
    void (*setAction)(Command *command, CommandAction action);
    const char * (*name)(const Command *command);
    CommandAction (*action)(const Command * command);
} const command = {
        commandCreate,
        commandExecute,
        commandSetAction,
        commandGetName,
        commandGetAction
};


#endif //COMAND_H
