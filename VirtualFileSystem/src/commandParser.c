#include "../inc/commandParser.h"
#include "../inc/file.h"
#include "../inc/directory.h"

enum Command getCommandID(const char* cmd)
{
    if (strcmp(cmd, "mkdir") == 0) return CMD_MKDIR;
    if (strcmp(cmd, "create") == 0) return CMD_CREATE;
    if (strcmp(cmd, "write") == 0) return CMD_WRITE;
    if (strcmp(cmd, "read") == 0) return CMD_READ;
    if (strcmp(cmd, "delete") == 0) return CMD_DELETE;
    if (strcmp(cmd, "rmdir") == 0) return CMD_RMDIR;
    if (strcmp(cmd, "ls") == 0) return CMD_LS;
    if (strcmp(cmd, "cd") == 0) return CMD_CD;
    if (strcmp(cmd, "pwd") == 0) return CMD_PWD;
    if (strcmp(cmd, "df") == 0) return CMD_DF;
    if (strcmp(cmd, "exit") == 0) return CMD_EXIT;
    return CMD_INVALID;
}

void parseUserInput(char* input, char* args[])
{
    input[strcspn(input, "\n")] = '\0';

    int count = 0;
    char* token = strtok(input, " ");

    while (token != NULL && count < 2)
    {
        args[count++] = token;
        token = strtok(NULL, " ");
    }

    if (token != NULL)
        args[count++] = token;

    args[count] = NULL;
}

void handleCommand(enum Command cmd, char* args[], char* raw, FileNode** cwd, FileNode** root, FreeBlock** head, int* run)
{
    switch (cmd)
    {
        case CMD_MKDIR:
            if (!args[1])
                printf("Usage: mkdir <dirname>\n");
            else
                makeDirectory(*cwd, args[1]);
            break;

        case CMD_CREATE:
            if (!args[1])
                printf("Usage: create <filename>\n");
            else
                createFile(*cwd, args[1]);
            break;

        case CMD_WRITE:
            if (!args[1])
                printf("Usage: write <filename> <text>\n");
            else
                writeFile(*cwd, raw, head);
            break;

        case CMD_READ:
            if (!args[1])
                printf("Usage: read <filename>\n");
            else
                readFile(*cwd, args[1]);
            break;

        case CMD_DELETE:
            if (!args[1])
                printf("Usage: delete <filename>\n");
            else
                deleteFile(*cwd, args[1], head);
            break;

        case CMD_RMDIR:
            if (!args[1])
                printf("Usage: rmdir <dirname>\n");
            else
                removeDirectory(*cwd, args[1]);
            break;

        case CMD_LS:
            listDirectory(*cwd);
            break;

        case CMD_CD:
            if (!args[1])
                printf("Usage: cd <dirname>\n");
            else
                changeDirectory(cwd, args[1]);
            break;

        case CMD_PWD:
            printWorkingDirectory(*cwd);
            break;

        case CMD_DF:
            showDiskStatus();
            break;

        case CMD_EXIT:
            *run = 0;
            printf("Exiting Virtual File System...\n");
            break;

        default:
            printf("Unknown command.\n");
    }
}
