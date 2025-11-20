#include "file.h"

char virtualDisk[NUM_BLOCKS][BLOCK_SIZE];
int totalFreeBlocks = NUM_BLOCKS;

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
        newNode->next = *head;
        *head = newNode;
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
