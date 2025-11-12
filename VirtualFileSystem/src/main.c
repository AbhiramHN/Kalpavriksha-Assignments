#include "../inc/commandParser.h"
#include "../inc/file.h"
#include "../inc/directory.h"

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
    }
    while (temp != cwd->child);

    return NULL;
}

void showDiskStatus()
{
    int used = NUM_BLOCKS - totalFreeBlocks;
    double usage = ((double)used / NUM_BLOCKS) * 100.0;

    printf("Total Blocks: %d\nUsed Blocks: %d\nFree Blocks: %d\nDisk Usage: %.2f%%\n",
           NUM_BLOCKS, used, totalFreeBlocks, usage);
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
