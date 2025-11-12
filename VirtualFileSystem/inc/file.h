#ifndef FILE_H
#define FILE_H

#include "vfs.h"

void createFile(FileNode* cwd, const char* fileName);
void writeFile(FileNode* cwd, const char* rawInput, FreeBlock** head);
void readFile(const FileNode* cwd, const char* file);
void deleteFile(FileNode* cwd, const char* file, FreeBlock** head);

#endif
