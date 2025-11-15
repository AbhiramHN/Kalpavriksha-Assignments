#ifndef VFS_H
#define VFS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024

extern char virtualDisk[NUM_BLOCKS][BLOCK_SIZE];
extern int totalFreeBlocks;

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

// Function declarations

FreeBlock* initializeFreeBlockList();

FileNode* initializeRootDirectory();

FileNode* findFile(const FileNode* cwd, const char* name, int isDir);

int allocateBlocks(int needed, int* blocks, FreeBlock** head);

void showDiskStatus();

#endif
