#include <stdio.h>
#include <string.h>
#include "../commands/commands.h"
#include "../core/cmd_engine.h"

int main(void)
{
    char cmd[100];
    
    printf("enter command:");
    fgets(cmd, sizeof(cmd), stdin);
    
    CMD_Init(commands,numCommands);
    executeCmd(cmd);

    return 0;
}