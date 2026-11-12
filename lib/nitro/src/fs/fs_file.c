// TwlSDK's FS library, fs_file.c: Black's main 0x020787a0-0x02078cb0. The file is libntr's (MIT,
// https://github.com/cybervisi0n/libntr) for SDK 5; the names are kh-recoded-decomp's twins' (MIT) or libntr's in the
// same place.

#include "types.h"
#include "nitro/fs.h"

FSArchive *FS_NormalizePath(const char *param_1, u32 *param_2, char *param_3);
BOOL FSi_SendCommand(FSFile *param_1, s32 param_2, s32 param_3);
u32 FS_GetFileLength(FSFile *param_1);
BOOL FS_OpenFileDirect(FSFile *param_1, FSArchive *param_2, u32 param_3, u32 param_4, u32 param_5);
u32 data_02150d20[2];
static const u8 data_020a19c4[64] = {
    33, 6, 192, 222, 186, 152, 206, 63, 166, 146, 227, 157, 70, 242, 237, 1, 118, 227, 204, 8, 86, 35, 99, 250,
    202, 212, 236, 223, 154, 98, 120, 52, 143, 109, 99, 60, 254, 34, 202, 146, 32, 136, 151, 35, 210, 207, 174,
    194, 50, 103, 141, 254, 202, 131, 100, 152, 172, 253, 62, 55, 135, 70, 88, 36,
};
BOOL FS_OpenFileEx(FSFile *param_1, const char *param_2, u32 param_3);
BOOL FS_OpenFileFast(FSFile *param_1, FSFileID param_2);
s32 FSi_DecrementSjisPosition(u8 *param_1, s32 param_2);
void func_02078c40(void);
FSArchive *FS_FindArchive(const char *param_1, s32 param_2);
s32 FSi_DecrementSjisPositionToSlash(u8 *param_1, s32 param_2);
s32 FSi_TrimSjisTrailingSlash(u8 *param_1);

s32 FSi_DecrementSjisPosition(u8 *param_1, s32 param_2) {
    s32 i = param_2 - 1;
    while (i > 0 && (u32)((param_1[i - 1] ^ 0x20) - 0xa1) < 0x3c) {
        i--;
    }
    return (param_2 - 1) - (((param_2 - 1) - i) & 1);
}

static inline BOOL FSi_IsSjisLeadByte(char c) {
    return (u32)(((u8)c ^ 0x20) - 0xa1) < 0x3c;
}

s32 FSi_IncrementSjisPositionToSlash(const char *param_1, s32 param_2) {
    while (param_1[param_2] != 0 && (u8)param_1[param_2] != 0x2f && (u8)param_1[param_2] != 0x5c) {
        param_2 = param_2 + 1 + FSi_IsSjisLeadByte(param_1[param_2]);
    }
    return param_2;
}

s32 FSi_DecrementSjisPositionToSlash(u8 *str, s32 pos) {
    for (;;) {
        pos = FSi_DecrementSjisPosition(str, pos);
        if (pos < 0 || str[pos] == '/' || str[pos] == '\\') {
            break;
        }
    }
    return pos;
}

s32 FSi_TrimSjisTrailingSlash(u8 *param_1) {
    s32 len = STD_GetStringLength(param_1);
    s32 pos = FSi_DecrementSjisPosition(param_1, len);
    if (pos >= 0) {
        if (param_1[pos] == 0x2f || param_1[pos] == 0x5c) {
            param_1[pos] = 0;
            len = pos;
        }
    }
    return len;
}

void FS_InitFile(FSFile *file) {
    file->arc = NULL;
    file->prev = NULL;
    file->next = NULL;
    file->queue.tail = NULL;
    file->queue.head = NULL;
    file->stat = (u32)file->arc | 0x2300;
    file->command = 0;
    file->error = 0;
}

BOOL FS_ConvertPathToFileID(FSFileID *param_1, const char *param_2) {
    BOOL result = FALSE;
    FSArchive *arc;
    u32 baseId = 0;
    char relativePath[0x118];
    struct {
        u32 baseId;
        char *path;
        u32 targetId;
        u32 isDirectory;
    } args;

    arc = FS_NormalizePath(param_2, &baseId, relativePath);
    if (arc != NULL) {
        FSFile file;
        FS_InitFile(&file);
        file.arc = arc;
        file.command = (u32)&args;
        args.baseId = baseId;
        args.path = relativePath;
        args.isDirectory = 0;
        if (FSi_SendCommand(&file, 4, 1) != 0) {
            param_1->arc = arc;
            param_1->file_id = args.targetId;
            result = TRUE;
        }
    }
    return result;
}

BOOL FS_OpenFileDirect(FSFile *param_1, FSArchive *param_2, u32 param_3, u32 param_4, u32 param_5) {
    struct {
        u32 index;
        u32 top;
        u32 bottom;
        u32 mode;
    } args;

    param_1->arc = param_2;
    param_1->command = (u32)&args;
    args.top = param_3;
    args.index = param_5;
    args.bottom = param_4;
    args.mode = 0;
    return FSi_SendCommand(param_1, 7, 1);
}

BOOL FS_OpenFileFast(FSFile *param_1, FSFileID param_2) {
    BOOL result = FALSE;
    struct {
        u32 fileId;
        u32 mode;
    } args;

    if (param_2.arc != NULL) {
        param_1->arc = param_2.arc;
        param_1->command = (u32)&args;
        args.fileId = param_2.file_id;
        args.mode = 0;
        result = FSi_SendCommand(param_1, 6, 1);
    }
    return result;
}

BOOL FS_OpenFileEx(FSFile *param_1, const char *param_2, u32 param_3) {
    BOOL result = FALSE;
    FSArchive *arc;
    u32 baseId = 0;
    char relativePath[0x118];
    struct {
        u32 baseId;
        char *path;
        u32 mode;
    } args;

    arc = FS_NormalizePath(param_2, &baseId, relativePath);
    if (arc != NULL) {
        FS_InitFile(param_1);
        param_1->command = (u32)&args;
        param_1->arc = arc;
        args.baseId = baseId;
        args.path = relativePath;
        args.mode = param_3;
        if (FSi_SendCommand(param_1, 0xd, 1) != 0) {
            result = TRUE;
        } else {
            param_1->arc = NULL;
        }
    }
    return result;
}

BOOL FS_CloseFile(FSFile *file) {
    return (u32)FSi_SendCommand(file, 8, 1);
}

u32 FS_GetFileLength(FSFile *param_1) {
    u32 result[1];
    u32 status;
    u32 value;

    result[0] = 0;
    status = FSi_GetFileLengthIfProc(param_1, result);
    if (status == 0) {
        param_1->command = (u32)&value;
        value = 0;
        if (FSi_SendCommand(param_1, 0xf, 1)) {
            result[0] = value;
        }
    }
    return result[0];
}

s32 FS_GetFilePosition(FSFile *param_1) {
    s32 result[1];
    u32 status;
    s32 value;

    result[0] = 0;
    status = FSi_GetFilePositionIfProc(param_1, (u32 *)result);
    if (status == 0) {
        param_1->command = (u32)&value;
        value = 0;
        if (FSi_SendCommand(param_1, 0x10, 1)) {
            result[0] = value;
        }
    }
    return result[0];
}

BOOL FS_SeekFile(FSFile *file, s32 offset, int whence) {
    struct {
        s32 offset;
        int whence;
    } args;

    file->command = (u32)(&args);
    args.offset = offset;
    args.whence = whence;
    return FSi_SendCommand(file, 0xe, 1);
}

s32 FS_ReadFile(FSFile *file, void *dest, s32 size) {
    struct {
        void *buffer;
        s32 length;
    } args;

    file->command = (u32)&args;
    args.buffer = dest;
    args.length = size;
    if (FSi_SendCommand(file, 0, 1) != 0) {
        size = args.length;
    } else {
        size = -1;
        if (file->error != 6) {
            size = args.length;
        }
    }
    return size;
}

s32 FS_WriteFile(FSFile *file, const void *src, s32 size) {
    struct {
        const void *buffer;
        s32 length;
    } args;

    file->command = (u32)&args;
    args.buffer = src;
    args.length = size;
    if (FSi_SendCommand(file, 1, 1) != 0) {
        size = args.length;
    } else {
        size = -1;
        if (file->error != 6) {
            size = args.length;
        }
    }
    return size;
}

// libntr's FS_CancelFile (libraries/fs/src/fs_file.c, as SDK 5 has it): Black's link drops it, as only
// sndarc_stream.c's CancelFileStream calls it, which Black lacks
void FS_CancelFile(FSFile *p_file) {
    OSIntrMode bak_psr = OS_DisableInterrupts();

    if (FS_IsBusy(p_file)) {
        p_file->stat |= FS_FILE_STATUS_CANCEL;
        p_file->arc->flag |= FS_ARCHIVE_FLAG_CANCELING;
    }

    (void)OS_RestoreInterrupts(bak_psr);
}

BOOL FS_OpenFile(FSFile *file, const char *path) {
    return FS_OpenFileEx(file, path, 1);
}

u32 FS_GetLength(FSFile *file) {
    return FS_GetFileLength((FSFile *)file);
}

BOOL func_02078c34(const char *path) {
    return FS_SetCurrentDirectory(path);
}

void func_02078c40(void) {
    if (OS_GetBootType() == 2) {
        data_02150d20[2] = 0xffffffff;
        data_02150d20[3] = 0;
        data_02150d20[4] = 0xffffffff;
        data_02150d20[5] = 0;
    } else {
        data_02150d20[2] = 0;
        data_02150d20[3] = 0;
        data_02150d20[4] = 0;
        data_02150d20[5] = 0;
    }
    data_02150d20[6] = (u32)data_020a19c4;
    data_02150d20[7] = 0x40;
    data_02150d20[1] = (u32)FS_FindArchive("rom", 3);
}
