#pragma once

#include "kernel/node/Node.h"

struct DirectoryListing
{
    size_t count;
    DirectoryEntry entries[];
};

struct FsDirectoryEntry
{
    char name[FILE_NAME_LENGTH];
    FsNode *node;
};

struct FsDirectory : public FsNode
{
    List *childs;
};

FsNode *directory_create();
