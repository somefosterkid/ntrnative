// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/arc.h, the lines in use so far.
#ifndef BWPORT_GFL_ARC_H
#define BWPORT_GFL_ARC_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Archives (arc_tool.c): NARC files, from the file system by the path of each archive ID, or from memory. An ArcTool
// keeps an archive open to read its files

ArcTool *GFL_ArcSysCreateFileHandle(u32 arcId, HeapID heapId);
void GFL_ArcToolFree(ArcTool *handle);
// Read an archive's files in pieces: the offset of a file in the archive, and seeking to an offset and reading from it
u32 GFL_ArcToolGetDataOfs(ArcTool *handle, u32 fileId);
void GFL_ArcToolSeek(ArcTool *handle, u32 offset);
void GFL_ArcToolReadRaw(ArcTool *handle, u32 size, void *dest);

void GFL_ArcSysReadRange(void *dest, u32 arcId, u32 fileId, u32 offset, u32 size);
void *GFL_ArcSysReadHeapNew(u32 arcId, u32 fileId, HeapID heapId);
// Reads a file of the archive at path into a new allocation
void *GFL_ArcSysReadHeapNewDirect(const char *path, u32 fileId, HeapID heapId);
void GFL_ArcToolReadRange(ArcTool *handle, u32 fileId, u32 offset, u32 size, void *dest);
u32 GFL_ArcToolGetDataLength(ArcTool *handle, u32 fileId);

u32 GFL_ArcToolGetDataMax(ArcTool *handle);

u32 GFL_ArcSysGetDataLength(u32 arcId, u32 fileId);
void GFL_ArcSysRead(void *dest, u32 arcId, u32 fileId);
// The count of files in the archive
u32 GFL_ArcSysGetDataMax(u32 arcId);

ArcTool *GFL_ArcSysCreateMemoryHandle(const void *data, u32 size, HeapID heapId);

// Sets the paths of the archives, by archive ID
void GFL_ArcSysInit(const char **paths, u32 count);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
const char *GFL_ArcSysGetResourcePath(u32 arcId);
void GFL_ArcToolRead(ArcTool *handle, u32 fileId, void *dest);
void *GFL_ArcToolReadHeapNew(ArcTool *handle, u32 fileId, HeapID heapId);
void GFL_ArcToolCopyDataOfs(ArcTool *handle, u32 fileId, u32 *offset);
void *GFL_ArcSysReadHeapNewRange(u32 arcId, u32 fileId, HeapID heapId, u32 offset, u32 size);

#endif // BWPORT_GFL_ARC_H
