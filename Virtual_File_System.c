#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024

char virtualDisk[NUM_BLOCKS][BLOCK_SIZE];
int totalFreeBlocks = 1024;

enum Command
{
    CMD_INVALID = -1,
    CMD_MKDIR,
    CMD_CREATE,
    CMD_WRITE,
    CMD_READ,
    CMD_DELETE,
    CMD_RMDIR,
    CMD_LS,
    CMD_CD,
    CMD_PWD,
    CMD_DF,
    CMD_EXIT
};

typedef struct FreeBlock
{
    int freeBlockIndex;
    struct FreeBlock* next;
    struct FreeBlock* prev;
} FreeBlock;

typedef struct FileNode
{
    char fileName[50];
    int isDirectory;
    struct FileNode* parent;
    struct FileNode* next;
    struct FileNode* prev;
    struct FileNode* child;
    int blockPointers[10];
    int size;
} FileNode;


// Command Identification
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


FreeBlock* initializeFreeBlockList()
{
    FreeBlock* head = malloc(sizeof(FreeBlock));
    head->freeBlockIndex = 0;
    head->next = NULL;
    head->prev = NULL;

    FreeBlock* current = head;

    for (int i = 1; i < NUM_BLOCKS; i++)
    {
        FreeBlock* node = malloc(sizeof(FreeBlock));
        node->freeBlockIndex = i;
        node->next = NULL;
        node->prev = current;
        current->next = node;
        current = node;
    }

    return head; 
}



FileNode* initializeRootDirectory()
{
    FileNode* root = malloc(sizeof(FileNode));

    strcpy(root->fileName, "/");
    root->isDirectory = 1;
    root->parent = NULL;
    root->next = NULL;
    root->prev = NULL;
    root->child = NULL;

    for (int i = 0; i < 10; i++)
        root->blockPointers[i] = -1;

    root->size = 0;

    return root;
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



FileNode* findFile(const FileNode* cwd, const char* name, int isDir)
{
    if (!cwd->child)
        return NULL;

    FileNode* temp = cwd->child;

    do
    {
        if (strcmp(temp->fileName, name) == 0 && temp->isDirectory == isDir)
            return temp;

        temp = temp->next;

    } while (temp != cwd->child);

    return NULL;
}


void makeDirectory(FileNode* cwd, const char* dirName)
{
    if (findFile(cwd, dirName, 1) || findFile(cwd, dirName, 0))
    {
        printf("Error: Name '%s' already exists.\n", dirName);
        return;
    }

    FileNode* newDir = malloc(sizeof(FileNode));

    strcpy(newDir->fileName, dirName);
    newDir->isDirectory = 1;
    newDir->parent = cwd;
    newDir->child = NULL;

    for (int i = 0; i < 10; i++)
        newDir->blockPointers[i] = -1;

    newDir->size = 0;

    if (!cwd->child)
    {
        cwd->child = newDir;
        newDir->next = newDir->prev = newDir;
    }
    else
    {
        FileNode* first = cwd->child;
        FileNode* last = first->prev;

        last->next = newDir;
        newDir->prev = last;
        newDir->next = first;
        first->prev = newDir;
    }

    printf("Directory '%s' created successfully.\n", dirName);
}


void createFile(FileNode* cwd, const char* fileName)
{
    if (findFile(cwd, fileName, 1) || findFile(cwd, fileName, 0))
    {
        printf("Error: Name '%s' already exists.\n", fileName);
        return;
    }

    FileNode* newFile = malloc(sizeof(FileNode));

    strcpy(newFile->fileName, fileName);
    newFile->isDirectory = 0;
    newFile->parent = cwd;
    newFile->child = NULL;

    for (int i = 0; i < 10; i++)
        newFile->blockPointers[i] = -1;

    newFile->size = 0;

    if (!cwd->child)
    {
        cwd->child = newFile;
        newFile->next = newFile->prev = newFile;
    }
    else
    {
        FileNode* first = cwd->child;
        FileNode* last = first->prev;

        last->next = newFile;
        newFile->prev = last;
        newFile->next = first;
        first->prev = newFile;
    }

    printf("File '%s' created successfully.\n", fileName);
}


int allocateBlocks(int needed, int* blocks, FreeBlock** head)
{
    for (int i = 0; i < needed; i++)
    {
        if (!(*head))
        {
            printf("Error: Not enough free blocks.\n");
            return 0;
        }

        FreeBlock* temp = *head;
        blocks[i] = temp->freeBlockIndex;
        *head = (*head)->next;

        free(temp);
    }

    return 1;
}


void writeFile(FileNode* cwd, const char* rawInput, FreeBlock** head)
{
    char buf[500];
    strcpy(buf, rawInput);
    strtok(buf, " ");

    char* file = strtok(NULL, " ");
    if (!file)
    {
        printf("Usage: write <filename> <text>\n");
        return;
    }

    const char* text = strstr(rawInput, file);
    text += strlen(file);

    if (*text == ' ')
        text++;

    FileNode* f = findFile(cwd, file, 0);
    if (!f)
    {
        printf("Error: File '%s' not found.\n", file);
        return;
    }

    f->size = strlen(text);

    int needed = (f->size + BLOCK_SIZE - 1) / BLOCK_SIZE;

    if (needed > 10)
    {
        printf("Error: File too large.\n");
        return;
    }

    int blocks[needed];

    if (!allocateBlocks(needed, blocks, head))
        return;

    int written = 0;

    for (int i = 0; i < needed; i++)
    {
        f->blockPointers[i] = blocks[i];

        int remain = f->size - written;
        int chunk = remain > BLOCK_SIZE ? BLOCK_SIZE : remain;

        memcpy(virtualDisk[blocks[i]], text + written, chunk);

        written += chunk;
    }

    for (int i = needed; i < 10; i++)
        f->blockPointers[i] = -1;

    totalFreeBlocks -= needed;

    printf("Data written to '%s' successfully.\n", file);
}


void readFile(const FileNode* cwd, const char* file)
{
    FileNode* f = findFile(cwd, file, 0);
    if (!f)
    {
        printf("Error: File '%s' not found.\n", file);
        return;
    }

    if (f->size == 0)
    {
        printf("File '%s' is empty.\n", file);
        return;
    }

    printf("Content of '%s':\n", file);

    int remain = f->size;
    char buf[BLOCK_SIZE + 1];

    for (int i = 0; i < 10 && f->blockPointers[i] != -1 && remain > 0; i++)
    {
        int block = f->blockPointers[i];
        int chunk = remain > BLOCK_SIZE ? BLOCK_SIZE : remain;

        memcpy(buf, virtualDisk[block], chunk);
        buf[chunk] = '\0';

        printf("%s", buf);

        remain -= chunk;
    }

    printf("\n");
}


void deleteFile(FileNode* cwd, const char* file, FreeBlock** head)
{
    FileNode* f = findFile(cwd, file, 0);
    if (!f)
    {
        printf("Error: File '%s' not found.\n", file);
        return;
    }

    for (int i = 0; i < 10 && f->blockPointers[i] != -1; i++)
    {
        FreeBlock* newNode = malloc(sizeof(FreeBlock));
        newNode->freeBlockIndex = f->blockPointers[i];
        newNode->next = NULL;

        if (!*head)
        {
            *head = newNode;
        }
        else
        {
            FreeBlock* t = *head;
            while (t->next)
                t = t->next;

            t->next = newNode;
        }

        totalFreeBlocks++;
    }

    if (f == cwd->child && f->next == f)
        cwd->child = NULL;

    else if (f == cwd->child)
    {
        cwd->child = f->next;
        f->prev->next = f->next;
        f->next->prev = f->prev;
    }
    else
    {
        f->prev->next = f->next;
        f->next->prev = f->prev;
    }

    free(f);

    printf("File '%s' deleted successfully.\n", file);
}


void removeDirectory(FileNode* cwd, const char* dir)
{
    FileNode* d = findFile(cwd, dir, 1);
    if (!d)
    {
        printf("Error: Directory '%s' not found.\n", dir);
        return;
    }

    if (d->child)
    {
        printf("Cannot remove '%s': Directory not empty.\n", dir);
        return;
    }

    if (d == cwd->child && d->next == d)
        cwd->child = NULL;

    else if (d == cwd->child)
    {
        cwd->child = d->next;
        d->prev->next = d->next;
        d->next->prev = d->prev;
    }
    else
    {
        d->prev->next = d->next;
        d->next->prev = d->prev;
    }

    free(d);

    printf("Directory '%s' removed successfully.\n", dir);
}


void listDirectory(const FileNode* cwd)
{
    if (!cwd->child)
    {
        printf("(empty)\n");
        return;
    }

    FileNode* n = cwd->child;

    do
    {
        if (n->isDirectory)
            printf("%s/ ", n->fileName);
        else
            printf("%s ", n->fileName);

        n = n->next;

    } while (n != cwd->child);

    printf("\n");
}


void changeDirectory(FileNode** cwd, const char* dir)
{
    if (strcmp(dir, "..") == 0)
    {
        if ((*cwd)->parent)
            *cwd = (*cwd)->parent;

        return;
    }

    FileNode* target = findFile(*cwd, dir, 1);

    if (!target)
    {
        printf("Error: Directory '%s' not found.\n", dir);
        return;
    }

    *cwd = target;
}


void printWorkingDirectory(const FileNode* cwd)
{
    if (!cwd->parent)
    {
        printf("/\n");
        return;
    }

    char parts[100][50];
    int count = 0;
    const FileNode* cur = cwd;

    while (cur)
    {
        if (!cur->parent)
            break;

        strcpy(parts[count++], cur->fileName);
        cur = cur->parent;
    }

    for (int i = count - 1; i >= 0; i--)
        printf("/%s", parts[i]);

    printf("\n");
}


void showDiskStatus()
{
    int used = NUM_BLOCKS - totalFreeBlocks;
    double usage = ((double)used / NUM_BLOCKS) * 100.0;

    printf("Total Blocks: %d\nUsed Blocks: %d\nFree Blocks: %d\nDisk Usage: %.2f%%\n",
           NUM_BLOCKS, used, totalFreeBlocks, usage);
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


int main()
{
    FreeBlock* freeList = initializeFreeBlockList();

    FileNode* root = initializeRootDirectory();
    FileNode* cwd = root;

    int running = 1;

    while (running)
    {
        char input[500];
        char raw[500];
        char* args[50];

        printf("%s >", cwd->fileName);
        fgets(input, sizeof(input), stdin);
        strcpy(raw, input);

        parseUserInput(input, args);

        if (!args[0])
            continue;

        enum Command cmd = getCommandID(args[0]);

        handleCommand(cmd, args, raw, &cwd, &root, &freeList, &running);
    }

    return 0;
}
