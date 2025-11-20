#include "directory.h"

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
    }
    while (n != cwd->child);

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
