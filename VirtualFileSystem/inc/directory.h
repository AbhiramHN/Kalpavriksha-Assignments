#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "vfs.h"

void makeDirectory(FileNode* cwd, const char* dirName);
void removeDirectory(FileNode* cwd, const char* dir);
void listDirectory(const FileNode* cwd);
void changeDirectory(FileNode** cwd, const char* dir);
void printWorkingDirectory(const FileNode* cwd);

#endif
