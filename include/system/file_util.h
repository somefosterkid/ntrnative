// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/file_util.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_SYSTEM_FILE_UTIL_H
#define BWPORT_SYSTEM_FILE_UTIL_H

#include "types.h"
#include "gfl/heap.h"
#include "nitro/fs.h"

// Opens an archive's file
void GFL_ArcSysInitRawHandle(FSFile *file, u32 arcId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Reads size bytes from offset of an archive's file into a new allocation, or all of the file for a size of 0
void *GFL_ArcSysReadRawResource(u32 arcId, HeapID heapId, u32 offset, u32 size);

#endif // BWPORT_SYSTEM_FILE_UTIL_H
