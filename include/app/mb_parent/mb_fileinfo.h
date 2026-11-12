#ifndef MB_FILEINFO_H
#define MB_FILEINFO_H

#include "nitro/fs.h"
#include "nitro/mb.h"

u32 MB_GetSegmentLength(FSFile *file);
BOOL MB_ReadSegment(FSFile *file, void *buf, u32 len);
BOOL MB_RegisterFile(const MBGameRegistry *game_reg, const void *buf);

#endif
