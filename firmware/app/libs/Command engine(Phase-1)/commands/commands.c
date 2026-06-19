#include<stdio.h>
#include "commands.h"

void cmdPing(int argc,char *argv[]){
    printf("pong\n");
}

void cmdHelp(int argc, char *argv[]){
    printf("Available commands:\n");
    printf("ping\n");
    printf("help\n");
}

void cmdEcho(int argc,char *argv[]){
    for(int i=1;i<argc;i++){
        printf("%s ",argv[i]);
    }
    printf("\n");
}

command commands[]={
    {"ping",cmdPing},
    {"help",cmdHelp},
    {"echo", cmdEcho}
};

int numCommands=sizeof(commands)/sizeof(commands[0]);
