#include "commandParser.h"
#include "file.h"
#include "directory.h"

enum Command getCommandID(const char* cmd)
{
    if (strcmp(cmd, "mkdir") == 0) return MKDIR;
    if (strcmp(cmd, "create") == 0) return CREATE;
    if (strcmp(cmd, "write") == 0) return WRITE;
    if (strcmp(cmd, "read") == 0) return READ;
    if (strcmp(cmd, "delete") == 0) return DELETE;
    if (strcmp(cmd, "rmdir") == 0) return RMDIR;
    if (strcmp(cmd, "ls") == 0) return LS;
    if (strcmp(cmd, "cd") == 0) return CD;
    if (strcmp(cmd, "pwd") == 0) return PWD;
    if (strcmp(cmd, "df") == 0) return DF;
    if (strcmp(cmd, "exit") == 0) return EXIT;
    return INVALID;
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
        case MKDIR:
            if (!args[1])
                printf("Usage: mkdir <dirname>\n");
            else
                makeDirectory(*cwd, args[1]);
            break;

        case CREATE:
            if (!args[1])
                printf("Usage: create <filename>\n");
            else
                createFile(*cwd, args[1]);
            break;

        case WRITE:
            if (!args[1])
                printf("Usage: write <filename> <text>\n");
            else
                writeFile(*cwd, raw, head);
            break;

        case READ:
            if (!args[1])
                printf("Usage: read <filename>\n");
            else
                readFile(*cwd, args[1]);
            break;

        case DELETE:
            if (!args[1])
                printf("Usage: delete <filename>\n");
            else
                deleteFile(*cwd, args[1], head);
            break;

        case RMDIR:
            if (!args[1])
                printf("Usage: rmdir <dirname>\n");
            else
                removeDirectory(*cwd, args[1]);
            break;

        case LS:
            listDirectory(*cwd);
            break;

        case CD:
            if (!args[1])
                printf("Usage: cd <dirname>\n");
            else
                changeDirectory(cwd, args[1]);
            break;

        case PWD:
            printWorkingDirectory(*cwd);
            break;

        case DF:
            showDiskStatus();
            break;

        case EXIT:
            *run = 0;
            printf("Exiting Virtual File System...\n");
            break;

        default:
            printf("Unknown command.\n");
    }
}
