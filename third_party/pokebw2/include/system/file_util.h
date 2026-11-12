#ifndef POKEBW2_SYSTEM_FILE_UTIL_H
#define POKEBW2_SYSTEM_FILE_UTIL_H

#include "types.h"
#include "gfl/heap.h"
#include "nitro/fs.h"

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

// Reading a whole archive file as it is, without its NARC structure (file_util.c)

// Opens an archive's file
void GFL_ArcSysInitRawHandle(FSFile *file, u32 arcId);
// Reads size bytes from offset of an archive's file into a new allocation, or all of the file for a size of 0
void *GFL_ArcSysReadRawResource(u32 arcId, HeapID heapId, u32 offset, u32 size);

#endif // POKEBW2_SYSTEM_FILE_UTIL_H
