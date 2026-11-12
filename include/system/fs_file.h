#ifndef BWPORT_SYSTEM_FS_FILE_H
#define BWPORT_SYSTEM_FS_FILE_H

#include "types.h"
#include "nitro/fs.h"

BOOL FS_ConvertPathToFileID(FSFileID *id, const char *path);
BOOL FS_OpenFileFast(FSFile *file, FSFileID id);

#endif
