#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include "vfs.h"

enum Command getCommandID(const char* cmd);
void parseUserInput(char* input, char* args[]);
void handleCommand(enum Command cmd, char* args[], char* raw, FileNode** cwd, FileNode** root, FreeBlock** head, int* run);

#endif
