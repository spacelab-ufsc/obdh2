#ifndef CMD_ENGINE_H
#define CMD_ENGINE_H

typedef struct{
    char *name;
    void (*handler)(int argc, char *argv[]);
}command;

void executeCmd(char *cmd);

void CMD_Init(command *table,int count);

#endif