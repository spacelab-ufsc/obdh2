#ifndef COMMANDS_H
#define COMMANDS_H

#include "../core/cmd_engine.h"

void cmdPing(int argc,char *argv[]);
void cmdHelp(int argc,char *argv[]);
void cmdEcho(int argc,char *argv[]);

extern command commands[];
extern int numCommands;

#endif