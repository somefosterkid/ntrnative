#include "global.h"

void sub_02027098(const char *filesystemPath);

void sub_02027098(const char *filesystemPath) {
    FSFile file;
    FS_InitFile(&file);
    if (!FS_OpenFile(&file, filesystemPath)) {
        return;
    }

    u32 fileStartAddress = FS_GetFileImageTop(&file);
    *(u32 *)(HW_MAIN_MEM + 0x007ffc2c) = fileStartAddress;
    OS_ResetSystem(0);
}
