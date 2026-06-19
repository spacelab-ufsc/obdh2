#include <stdio.h>
#include <string.h>
#include "cmd_engine.h"

static command *g_table;
static int g_count;

void CMD_Init(command *table,int count){
    g_table = table;
    g_count = count;

}

static int ParseLine(char *line,
                     char *argv[])
{
    int argc = 0;

    char *token =
        strtok(line, " ");

    while(token != NULL)
    {
        argv[argc++] = token;

        token =
            strtok(NULL, " ");
    }

    return argc;
}

void executeCmd(char *cmd){
    
    char *argv[10];
    int argc = ParseLine(cmd,argv);

     for(int i=0;i<g_count;i++){
        if(strcmp(argv[0],g_table[i].name)==0){
            g_table[i].handler(argc,argv);
            return;
        }
     }
}