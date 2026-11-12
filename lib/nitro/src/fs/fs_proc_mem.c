// TwlSDK's FS library, fs_proc_mem.c: Black's main 0x0207a7c4-0x0207a980. The file is libntr's (MIT,
// https://github.com/cybervisi0n/libntr) for SDK 5; the names are kh-recoded-decomp's twins' (MIT) or libntr's in the
// same place.

#include "types.h"
#include "nitro/fs.h"

void OS_Terminate(void);
BOOL FS_OpenFileDirect(FSFile *param_1, FSArchive *param_2, u32 param_3, u32 param_4, u32 param_5);
void FSi_InitMemArchive(void);
FSArchive data_02150d44;
// A directory entry as the directory read fills it
typedef struct FSDirEntry {
    FSDirPos pos;
    u32 isDirectory; // 0xc
    u32 nameLength; // 0x10
    char name[0x80]; // 0x14
} FSDirEntry;
const char *FS_GetArchiveName(const FSArchive *arc);
int STD_GetStringLength(const char *str);
int STD_TSPrintf(char *dst, const char *fmt, ...);
s32 FSi_MemArchiveProc(FSFile *param_1, u32 param_2);
static s32 data_02150d40;

s32 FSi_MemArchiveProc(FSFile *param_1, u32 param_2) {
    switch (param_2) {
    case 3: {
        u32 result;
        FSDirEntry *entry = (FSDirEntry *)param_1->arg.readdir.p_entry;
        FSArchive *current = (FSArchive *)param_1->prop.dir.pos.pos;
        if (current == NULL) {
            entry->nameLength = 0;
            entry->isDirectory = 0;
            result = 1;
        } else {
            const char *arcname = FS_GetArchiveName(current);
            entry->nameLength = STD_GetStringLength(arcname) + 1;
            entry->isDirectory = TRUE;
            if (param_1->arg.readdir.skip_string == 0) {
                STD_TSPrintf(entry->name, "%s:", arcname);
                entry->pos.arc = param_1->arc;
                entry->pos.ownId = 0;
                entry->pos.index = 0;
                entry->pos.pos = 0;
            }
            result = 0;
            param_1->prop.dir.pos.pos = (u32)current->next;
        }
        return result;
    }
    case 2:
    case 4:
    case 5:
    case 6:
        return 4;
    default:
        return 0x102;
    }
}

void FSi_InitMemArchive(void) {
    s32 flag;
    u32 state;

    state = OS_DisableInterrupts();
    if (data_02150d40 != 0) {
        if (data_02150d44.flag & 2) {
            flag = 1;
        } else {
            flag = 0;
        }
        if (flag == 0) {
            goto block_5;
        }
    } else {
block_5:
        FS_InitArchive(&data_02150d44);
        FS_SetArchiveProc(&data_02150d44, FSi_MemArchiveProc, (u32)-1);
        if (FS_LoadArchive(&data_02150d44, 0U, 0U, 0U, 0U, 0U, NULL, NULL) == 0) {
            OS_Terminate();
        }
        data_02150d40 = 1;
    }
    OS_RestoreInterrupts(state);
}

BOOL FS_CreateFileFromMemory(FSFile *file, void *data, u32 size) {
    FSi_InitMemArchive();
    return FS_OpenFileDirect((FSFile *)file, (void *)&data_02150d44, (u32)data, (s32)(u8 *)data + size, 0);
}
