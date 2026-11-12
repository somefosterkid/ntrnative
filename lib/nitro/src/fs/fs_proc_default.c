// TwlSDK's FS library, fs_proc_default.c: Black's main 0x02079304-0x0207a7c4. The file is libntr's (MIT,
// https://github.com/cybervisi0n/libntr) for SDK 5; the names are kh-recoded-decomp's twins' (MIT) or libntr's in the
// same place.

#include "types.h"
#include "nitro/card.h"
#include "nitro/fs.h"
#include "nitro/mi.h"

s32 FSi_ROMFAT_FindPath(FSArchive *param_1, u32 param_2, const char *param_3, u32 *param_4, BOOL param_5);
s32 FSi_ROMFAT_OpenFileFast(FSArchive *param_1, FSFile *param_2, u32 param_3, u32 param_4);
s32 FSi_TranslateCommand(FSFile *param_1, u32 param_2, s32 param_3);
u32 FS_UnmountArchive(FSArchive *param_1);
BOOL FS_OpenFileDirect(FSFile *param_1, FSArchive *param_2, u32 param_3, u32 param_4, u32 param_5);
// The table read parameter (the archive and the position in it), and a FAT entry: the top and bottom of a file
typedef struct FSTableParam {
    FSArchive *arc;
    u32 pos;
} FSTableParam;
typedef struct FSFatEntry {
    u32 top;
    u32 bottom;
} FSFatEntry;
// A directory table entry: where the directory starts, its index and its parent
typedef struct FSDirTableEntry {
    u32 start;
    u16 index;
    u16 parent;
} FSDirTableEntry;
// The archive's resource: the sizes and handle counts a file system reports (TwlSDK's layout)
typedef struct FSArchiveResource {
    u64 totalSize;
    u64 availableSize;
    u32 maxFileHandles;
    u32 currentFileHandles;
    u32 maxDirectoryHandles;
    u32 currentDirectoryHandles;
    u32 bytesPerSector;
    u32 sectorsPerCluster;
    u32 totalClusters;
    u32 availableClusters;
} FSArchiveResource;
// A directory entry as the directory read fills it, and the info a directory read returns
typedef struct FSDirEntry {
    union {
        struct {
            FSArchive *arc;
            union {
                u32 fileId;
                struct {
                    u16 ownId;
                    u16 index;
                } dir;
            } id;
            u32 unk_8;
        };
        FSDirPos pos;
        FSFileID file;
    };
    u32 isDirectory; // 0xc
    u32 nameLength; // 0x10
    char name[0x80]; // 0x14
} FSDirEntry;
typedef struct FSDirectoryInfo {
    u8 unk_0[0x10];
    u32 shortNameLength; // 0x10
    u8 longName[0x104]; // 0x14
    u32 longNameLength; // 0x118
    u32 attributes; // 0x11c
    u8 unk_120[0x18];
    u32 modifyTime[6]; // 0x138
    u8 unk_150[0x18];
    u32 fileSize; // 0x168
    u32 id; // 0x16c
} FSDirectoryInfo;
// The info a path lookup returns
typedef struct FSPathInfo {
    u32 attributes;
    u8 unk_4[0x48];
    u32 fileSize; // 0x4c
    u32 id; // 0x50
} FSPathInfo;
// A ROM file's property: the prop union of FSFile, which its prev field (+4) points at
typedef struct FSFileProperty {
    u32 own_id;
    u32 top;
    u32 bottom;
    u32 pos;
} FSFileProperty;
s32 FSi_ReadTable(FSTableParam *param_1, void *param_2, u32 param_3);
// A ROM file request's buffer: the file's prev field (+4) points at it
typedef struct FSBuffer {
    u8 unk_0[8];
    u32 capacity;
    u32 used;
} FSBuffer;
u32 FSi_ReadMemCallback(FSArchive *arc, void *dst, u32 pos, u32 size);
u32 FSi_WriteMemCallback(FSArchive *arc, const void *src, u32 pos, u32 size);
BOOL FS_SuspendArchive(FSArchive *param_1);
BOOL FS_ResumeArchive(FSArchive *param_1);
BOOL FS_CloseFile(FSFile *file);
void FS_InitFile(FSFile *file);
BOOL FS_MountArchive(FSArchive *param_1, FSArchiveContext *param_2, const void *param_3, u32 reserved);
s32 FS_ReadFile(FSFile *file, void *dest, s32 size);
s32 FSi_WaitForArchiveCompletion(s32 param_1, s32 param_2);

s32 FSi_ReadTable(FSTableParam *param_1, void *param_2, u32 param_3) {
    FSArchive *arc = param_1->arc;
    FSArchiveContext *context = arc->context;
    s32 result;

    if (context->copyInMemory != 0) {
        MI_CpuCopy8((const void *)param_1->pos, param_2, param_3);
        result = 0;
    } else {
        result = (s32)context->readFunc(arc, param_2, param_1->pos, param_3);
        result = FSi_WaitForArchiveCompletion((s32)arc->pending, result);
    }
    param_1->pos += param_3;
    return result;
}

s32 FSi_SeekDirDirect(FSFile *param_1, u32 param_2) {
    param_1->arg.seekdir.ownId = param_2;
    param_1->arg.seekdir.arc = param_1->arc;
    param_1->arg.seekdir.index = 0;
    param_1->arg.seekdir.pos = 0;
    return FSi_TranslateCommand(param_1, 2, 1);
}

s32 FSi_SeekDirCommand(FSFile *param_1) {
    FSArchive *arc = param_1->arc;
    FSArchiveContext *info = arc->context;
    FSDirPos *arg = &param_1->arg.seekdir;
    FSDirTableEntry entry;
    FSTableParam param;
    s32 result;

    param.arc = arc;
    param.pos = info->fnt + arg->ownId * 8;
    result = FSi_ReadTable(&param, &entry, 8);
    if (result == 0) {
        param_1->prop.dir.pos = *arg;
        if (arg->index == 0 && arg->pos == 0) {
            param_1->prop.dir.pos.index = entry.index;
            param_1->prop.dir.pos.pos = info->fnt + entry.start;
        }
        param_1->prop.dir.parent = entry.parent & 0xfff;
    }
    return result;
}

s32 FSi_ReadDirCommand(FSFile *param_1) {
    FSDirEntry *entry = (FSDirEntry *)param_1->arg.openfiledirect.top;
    FSTableParam param;
    u8 len;
    u16 id;
    s32 result;
    u16 *index = &((u16 *)&param_1->prop.file.top)[1];

    param.arc = param_1->arc;
    param.pos = param_1->prop.file.bottom;
    result = FSi_ReadTable(&param, &len, 1);
    if (result != 0) {
        return result;
    }
    entry->nameLength = len & 0x7f;
    entry->isDirectory = (len >> 7) & 1;
    if (entry->nameLength == 0) {
        return 1;
    }
    if (param_1->arg.openfiledirect.bottom == 0) {
        result = FSi_ReadTable(&param, entry->name, entry->nameLength);
        if (result != 0) {
            return result;
        }
        entry->name[entry->nameLength] = 0;
    } else {
        param.pos += entry->nameLength;
    }
    if (entry->isDirectory == 0) {
        entry->arc = param_1->arc;
        entry->id.fileId = *index;
        *index = *index + 1;
    } else {
        result = FSi_ReadTable(&param, &id, 2);
        if (result == 0) {
            entry->arc = param_1->arc;
            entry->id.dir.ownId = id & 0xfff;
            entry->id.dir.index = 0;
            entry->unk_8 = 0;
        }
    }
    if (result == 0) {
        param_1->prop.file.bottom = param.pos;
    }
    return result;
}

static inline int FSi_StrNICmp(const char *str1, const char *str2, u32 len) {
    int ret = 0;
    u32 i;
    for (i = 0; i < len; ++i) {
        u32 c1 = (u8)(str1[i] - 'A');
        u32 c2 = (u8)(str2[i] - 'A');
        if (c1 <= 'Z' - 'A') {
            c1 += 'a' - 'A';
        }
        if (c2 <= 'Z' - 'A') {
            c2 += 'a' - 'A';
        }
        ret = (int)(c1 - c2);
        if (ret != 0) {
            break;
        }
    }
    return ret;
}

s32 FSi_FindPathCommand(FSFile *file) {
    const char *path = (const char *)file->arg.findpath.path;
    BOOL isDirectory = file->arg.findpath.isDirectory;
    FSDirEntry entry;
    BOOL wantDirectory;
    int length;

    FSi_TranslateCommand(file, 2, TRUE);
    for (; *path != '\0'; path += (*path != '\0') ? 1 : 0) {
        char c;

        length = FSi_IncrementSjisPositionToSlash(path, 0);
        wantDirectory = path[length] != '\0' || isDirectory;
        if (length == 0) {
            return 6;
        }
        c = path[0];
        if (c == '.') {
            if (length == 1) {
                path += 1;
                continue;
            } else if (length == 2 && path[1] == '.') {
                if (file->prop.dir.pos.ownId != 0) {
                    FSi_SeekDirDirect(file, file->prop.dir.parent);
                }
                path += 2;
                continue;
            }
        } else if (c == '*') {
            break;
        }
        if (length > 127) {
            return 11;
        }
        file->arg.readdir.p_entry = &entry;
        file->arg.readdir.skip_string = FALSE;
        for (;;) {
            if (FSi_TranslateCommand(file, 3, TRUE) != 0) {
                return 11;
            }
            if (wantDirectory != entry.isDirectory || length != entry.nameLength) {
                continue;
            }
            if (FSi_StrNICmp(path, entry.name, length) == 0) {
                break;
            }
        }
        if (wantDirectory) {
            file->arg.seekdir = entry.pos;
            path += length;
            FSi_TranslateCommand(file, 2, TRUE);
        } else {
            if (isDirectory) {
                return 11;
            }
            *file->arg.findpath.result.file = entry.file;
            return 0;
        }
    }
    if (isDirectory == FALSE) {
        return 11;
    }
    *file->arg.findpath.result.dir = file->prop.dir.pos;
    return 0;
}

static inline BOOL FSi_IsDirectory(FSFile *file) {
    return (file->stat & 0x20) ? TRUE : FALSE;
}

s32 FSi_GetPathCommand(FSFile *param_1) {
    FSArchive *const arc = param_1->arc;
    FSDirEntry entry;
    FSFile cursor;
    unsigned long dirId;
    unsigned long fileId;

    FS_InitFile(&cursor);
    cursor.arc = arc;
    if (FSi_IsDirectory(param_1) != FALSE) {
        dirId = param_1->prop.dir.pos.ownId;
        fileId = 0x10000;
    } else {
        unsigned long pos = 0;
        unsigned long dirCount = 0;

        fileId = param_1->prop.file.own_id;
        dirId = 0x10000;
        do {
            FSi_SeekDirDirect(&cursor, pos);
            if (pos == 0) {
                dirCount = cursor.prop.dir.parent;
            }
            cursor.arg.readdir.p_entry = &entry;
            cursor.arg.readdir.skip_string = TRUE;
            while (FSi_TranslateCommand(&cursor, 3, TRUE) == 0) {
                if (!entry.isDirectory && entry.id.fileId == fileId) {
                    dirId = cursor.prop.dir.pos.ownId;
                    break;
                }
            }
        } while (dirId == 0x10000 && ++pos < dirCount);
    }

    if (dirId == 0x10000) {
        param_1->arg.getpath.total_len = 0;
        return 0xb;
    }

    {
        unsigned long id = dirId;
        int total = STD_GetStringLength(FS_GetArchiveName(arc)) + 2;

        FSi_SeekDirDirect(&cursor, id);
        if (fileId != 0x10000) {
            total += entry.nameLength;
        }
        while (id != 0) {
            FSi_SeekDirDirect(&cursor, cursor.prop.dir.parent);
            cursor.arg.readdir.p_entry = &entry;
            cursor.arg.readdir.skip_string = TRUE;
            while (FSi_TranslateCommand(&cursor, 3, TRUE) == 0) {
                if (entry.isDirectory && entry.id.dir.ownId == id) {
                    total += entry.nameLength + 1;
                    break;
                }
            }
            id = cursor.prop.dir.pos.ownId;
        }
        param_1->arg.getpath.total_len = total + 1;
        param_1->arg.getpath.dir_id = dirId;
    }

    if (param_1->arg.getpath.buffer != 0 && param_1->arg.getpath.length >= param_1->arg.getpath.total_len) {
        unsigned long total = param_1->arg.getpath.total_len;
        u8 *dst = (u8 *)param_1->arg.getpath.buffer;
        unsigned long pos = 0;
        unsigned long id = dirId;

        {
            const char *arcName = FS_GetArchiveName(arc);
            int len = STD_GetStringLength(arcName);

            MI_CpuCopy8(arcName, dst + pos, len);
            pos += len;
            MI_CpuCopy8(":/", dst + pos, 2);
            pos += 2;
        }
        FSi_SeekDirDirect(&cursor, id);
        if (fileId != 0x10000) {
            cursor.arg.readdir.p_entry = &entry;
            cursor.arg.readdir.skip_string = FALSE;
            while (FSi_TranslateCommand(&cursor, 3, TRUE) == 0) {
                if (!entry.isDirectory && entry.id.fileId == fileId) {
                    break;
                }
            }
            {
                u32 len = entry.nameLength + 1;

                MI_CpuCopy8(entry.name, dst + total - len, len);
                total -= len;
            }
        } else {
            dst[total - 1] = 0;
            total -= 1;
        }
        while (id != 0) {
            FSi_SeekDirDirect(&cursor, cursor.prop.dir.parent);
            cursor.arg.readdir.p_entry = &entry;
            cursor.arg.readdir.skip_string = FALSE;
            dst[total - 1] = '/';
            total -= 1;
            while (FSi_TranslateCommand(&cursor, 3, TRUE) == 0) {
                if (entry.isDirectory && entry.id.dir.ownId == id) {
                    u32 len = entry.nameLength;

                    MI_CpuCopy8(entry.name, dst + total - len, len);
                    total -= len;
                    break;
                }
            }
            id = cursor.prop.dir.pos.ownId;
        }
    }
    return 0;
}



s32 FSi_OpenFileFastCommand(FSFile *param_1) {
    FSArchive *arc = param_1->arc;
    FSArchiveContext *context = arc->context;
    u32 index = param_1->arg.openfiledirect.bottom;
    u32 pos = index * 8;
    FSFatEntry fat;
    FSTableParam param;
    s32 result;

    if (pos >= context->fatSize) {
        return 11;
    }
    param.arc = arc;
    param.pos = context->fat + pos;
    result = FSi_ReadTable(&param, &fat, 8);
    if (result != 0) {
        return result;
    }
    param_1->arg.openfiledirect.top = fat.top;
    param_1->arg.openfiledirect.bottom = fat.bottom;
    param_1->arg.openfiledirect.index = index;
    return FSi_TranslateCommand(param_1, 7, 1);
}

s32 FSi_OpenFileDirectCommand(FSFile *param_1) {
    param_1->prop.file.top = param_1->arg.openfiledirect.top;
    param_1->prop.file.pos = param_1->arg.openfiledirect.top;
    param_1->prop.file.bottom = param_1->arg.openfiledirect.bottom;
    param_1->prop.file.own_id = param_1->arg.openfiledirect.index;
    return 0;
}

s32 FSi_ReadFileCommand(FSFile *param_1) {
    FSArchive *arc = param_1->arc;
    FSArchiveContext *context = arc->context;
    u32 position = param_1->prop.file.pos;
    u32 length = param_1->arg.readfile.len;
    void *dst = param_1->arg.readfile.dst;

    param_1->prop.file.pos += length;
    return context->readFunc(arc, dst, position, length);
}

s32 FSi_WriteFileCommand(FSFile *param_1) {
    FSArchive *arc = param_1->arc;
    FSArchiveContext *context = arc->context;
    u32 position = param_1->prop.file.pos;
    u32 length = param_1->arg.readfile.len;
    void *src = param_1->arg.readfile.dst;

    param_1->prop.file.pos += length;
    return context->writeFunc(arc, src, position, length);
}

s32 FSi_CloseFileCommand(FSFile *param_1) {
    return 0;
}

s32 FSi_TranslateCommand(FSFile *param_1, u32 param_2, s32 param_3) {
    s32 result = 0x101;
    FSArchiveContext *const context = param_1->arc->context;
    const s32 bit = 1 << param_2;

    if ((context->procFlag & bit) != 0) {
        result = context->proc(param_1, param_2);
        switch (result) {
        case 0:
        case 1:
        case 4:
            break;
        case 0x100:
            break;
        case 0x102:
            result = 0x101;
            context->procFlag &= ~bit;
            break;
        }
    }
    if (result == 0x101) {
        // The default commands, by command number
        static s32 (*const data_020a1a04[])(FSFile *) = {
            FSi_ReadFileCommand,  FSi_WriteFileCommand, FSi_SeekDirCommand,      FSi_ReadDirCommand,
            FSi_FindPathCommand,  FSi_GetPathCommand,   FSi_OpenFileFastCommand, FSi_OpenFileDirectCommand,
            FSi_CloseFileCommand, FSi_CloseFileCommand, FSi_CloseFileCommand,    FSi_CloseFileCommand,
            FSi_CloseFileCommand,
        };
        if (param_2 < 0xd) {
            result = data_020a1a04[param_2](param_1);
        } else {
            result = 4;
        }
    }
    if (param_3 != 0) {
        result = FSi_WaitForArchiveCompletion((s32)param_1, result);
    }
    return result;
}

s32 FSi_ROMFAT_ReadFile(FSArchive *param_1, FSFile *param_2, void *param_3, u32 *param_4) {
    FSBuffer *buffer = (FSBuffer *)param_2->prev;
    u32 requested = *param_4;
    u32 remaining = buffer->capacity - buffer->used;

    if (requested > remaining) {
        *param_4 = remaining;
    }
    param_2->arg.readfile.len_org = requested;
    param_2->arg.readfile.dst = param_3;
    param_2->arg.readfile.len = *param_4;
    return FSi_TranslateCommand(param_2, 0, 0);
}

s32 FSi_ROMFAT_WriteFile(FSArchive *param_1, FSFile *param_2, void *param_3, u32 *param_4) {
    FSBuffer *buffer = (FSBuffer *)param_2->prev;
    u32 requested = *param_4;
    u32 remaining = buffer->capacity - buffer->used;

    if (requested > remaining) {
        *param_4 = remaining;
    }
    param_2->arg.readfile.len_org = requested;
    param_2->arg.readfile.dst = param_3;
    param_2->arg.readfile.len = *param_4;
    return FSi_TranslateCommand(param_2, 1, 0);
}

s32 FSi_ROMFAT_SeekDirectory(FSArchive *param_1, FSFile *param_2, u32 param_3, u32 param_4) {
    s32 result;

    param_2->arc = param_1;
    param_2->arg.seekdir.arc = param_1;
    param_2->arg.seekdir.ownId = (u16)param_3;
    param_2->arg.seekdir.index = param_3 >> 16;
    param_2->arg.seekdir.pos = param_4;
    result = FSi_TranslateCommand(param_2, 2, 1);
    if (result == 0) {
        param_2->stat = (param_2->stat | 0x20) & ~0x10;
        param_2->prev = (FSFile *)&param_2->prop;
        param_2->arc = param_1;
    }
    return result;
}

s32 FSi_ROMFAT_ReadDirectory(FSArchive *param_1, FSFile *param_2, void *info) {
    FSDirectoryInfo *param_3 = info;
    s32 result;
    FSDirEntry entry;

    param_2->arg.openfiledirect.top = (u32)&entry;
    param_2->arg.openfiledirect.bottom = 0;
    result = FSi_TranslateCommand(param_2, 3, 1);
    if (result == 0) {
        param_3->shortNameLength = 0;
        param_3->longNameLength = entry.nameLength;
        MI_CpuCopy8(entry.name, param_3->longName, param_3->longNameLength);
        param_3->longName[param_3->longNameLength] = 0;
        if (entry.isDirectory != 0) {
            param_3->attributes = 0x100;
            param_3->id = entry.id.dir.ownId | (entry.id.dir.index << 16);
            param_3->fileSize = 0;
        } else {
            param_3->attributes = 0;
            param_3->id = entry.id.fileId;
            param_3->fileSize = 0;
            {
                FSArchiveContext *const context = param_1->context;
                u32 pos = param_3->id * sizeof(FSFatEntry);

                if (pos < context->fatSize) {
                    FSFatEntry fat;
                    FSTableParam param;

                    param.arc = param_1;
                    param.pos = context->fat + pos;
                    if (FSi_ReadTable(&param, &fat, sizeof(fat)) == 0) {
                        param_3->fileSize = fat.bottom - fat.top;
                        if (FSi_IsUnreadableRomOffset(param_1, fat.top) != 0) {
                            param_3->attributes |= 0x400;
                        }
                    }
                }
            }
        }
        param_3->modifyTime[0] = 0;
        param_3->modifyTime[1] = 0;
        param_3->modifyTime[2] = 0;
        param_3->modifyTime[3] = 0;
        param_3->modifyTime[4] = 0;
        param_3->modifyTime[5] = 0;
    }
    return result;
}

s32 FSi_ROMFAT_FindPath(FSArchive *param_1, u32 param_2, const char *param_3, u32 *param_4, BOOL param_5) {
    FSFile file;
    struct {
        u32 unk_0;
        union {
            u32 fileId;
            u16 ownId;
        } target;
        u32 unk_8;
    } result;
    s32 ret;

    FS_InitFile(&file);
    file.arc = param_1;
    file.arg.findpath.arc = param_1;
    file.arg.findpath.ownId = param_2;
    file.arg.findpath.unk_36 = 0;
    file.arg.findpath.index = 0;
    file.arg.findpath.path = (u32)param_3;
    file.arg.findpath.isDirectory = param_5;
    file.arg.findpath.result.ptr = &result;
    ret = FSi_TranslateCommand(&file, 4, 1);
    if (ret == 0) {
        if (param_5 != 0) {
            *param_4 = result.target.ownId;
        } else {
            *param_4 = result.target.fileId;
        }
    }
    return ret;
}

s32 FSi_ROMFAT_GetPath(FSArchive *param_1, FSFile *param_2, BOOL param_3, char *param_4, u32 *param_5) {
    s32 result;

    param_2->arg.getpath.total_len = 0;
    param_2->arg.getpath.dir_id = 0;
    param_2->arg.getpath.buffer = (u32)param_4;
    param_2->arg.getpath.length = *param_5;
    result = FSi_TranslateCommand(param_2, 5, 1);
    if (result == 0) {
        *param_5 = param_2->arg.getpath.length;
    }
    return result;
}

s32 FSi_ROMFAT_OpenFileFast(FSArchive *param_1, FSFile *param_2, u32 param_3, u32 param_4) {
    s32 result;

    param_2->arg.openfiledirect.bottom = param_3;
    param_2->arg.openfiledirect.top = (u32)param_1;
    result = FSi_TranslateCommand(param_2, 6, 1);
    if (result == 0) {
        param_2->stat = (param_2->stat | 0x10) & ~0x20;
        param_2->prev = (FSFile *)&param_2->prop;
        param_2->arc = (FSArchive *)param_1;
    }
    return result;
}

s32 FSi_ROMFAT_OpenFileDirect(FSArchive *param_1, FSFile *param_2, u32 param_3, u32 param_4, u32 *param_5) {
    s32 result;

    param_2->arg.openfiledirect.top = param_3;
    param_2->arg.openfiledirect.bottom = param_4;
    param_2->arg.openfiledirect.index = *param_5;
    result = FSi_TranslateCommand(param_2, 7, 1);
    if (result == 0) {
        param_2->stat = (param_2->stat | 0x10) & ~0x20;
        param_2->prev = (FSFile *)&param_2->prop;
        param_2->arc = (FSArchive *)param_1;
    }
    return result;
}

s32 FSi_ROMFAT_CloseFile(FSArchive *param_1, FSFile *param_2) {
    s32 result;

    result = FSi_TranslateCommand(param_2, 8, 1);
    param_2->prev = NULL;
    param_2->stat &= ~0x30;
    return result;
}

void FSi_ROMFAT_Activate(FSArchive *param_1) {
    FSFile FStack_58;

    FS_InitFile(&FStack_58);
    FStack_58.arc = param_1;
    FSi_TranslateCommand(&FStack_58, 9, 0);
}

void FSi_ROMFAT_Idle(FSArchive *param_1) {
    FSFile FStack_58;

    FS_InitFile(&FStack_58);
    FStack_58.arc = param_1;
    FSi_TranslateCommand(&FStack_58, 10, 0);
}

void FSi_ROMFAT_Suspend(FSArchive *param_1) {
    FSFile FStack_58;

    FS_InitFile(&FStack_58);
    FStack_58.arc = param_1;
    FSi_TranslateCommand(&FStack_58, 0xb, 0);
}

void FSi_ROMFAT_Resume(FSArchive *param_1) {
    FSFile FStack_58;

    FS_InitFile(&FStack_58);
    FStack_58.arc = param_1;
    FSi_TranslateCommand(&FStack_58, 0xc, 0);
}

s32 FSi_ROMFAT_OpenFile(FSArchive *param_1, FSFile *param_2, u32 param_3, const char *param_4, u32 param_5) {
    s32 result;
    u32 value;

    result = FSi_ROMFAT_FindPath(param_1, param_3, param_4, &value, 0);
    if (result != 0) {
        return result;
    }
    return FSi_ROMFAT_OpenFileFast(param_1, param_2, value, param_5);
}

s32 FSi_ROMFAT_SeekFile(FSArchive *param_1, FSFile *param_2, s32 *param_3, s32 param_4) {
    FSFileProperty *property = (FSFileProperty *)param_2->prev;
    s32 pos = *param_3;
    s32 offset;

    switch (param_4) {
    case 0:
        offset = property->top;
        pos += offset;
        break;
    case 1:
    default:
        offset = property->pos;
        pos += offset;
        break;
    case 2:
        offset = property->bottom;
        pos += offset;
        break;
    }
    if (pos < (s32)property->top || pos > (s32)property->bottom) {
        return 6;
    }
    property->pos = pos;
    *param_3 = pos;
    return 0;
}

s32 FSi_ROMFAT_GetFileLength(FSArchive *param_1, FSFile *param_2, u32 *param_3) {
    FSFileProperty *property = (FSFileProperty *)param_2->prev;

    *param_3 = property->bottom - property->top;
    return 0;
}

s32 FSi_ROMFAT_GetFilePosition(FSArchive *param_1, FSFile *param_2, u32 *param_3) {
    FSFileProperty *property = (FSFileProperty *)param_2->prev;

    *param_3 = property->pos - property->top;
    return 0;
}

void FSi_ROMFAT_Unmount(FSArchive *param_1) {
    u32 *ptr;

    ptr = (u32 *)param_1->context;
    FS_IsArchiveTableLoaded(param_1);
    *ptr = 0;
    ptr[1] = 0;
    ptr[2] = 0;
    ptr[3] = 0;
    ptr[4] = 0;
    ptr[5] = 0;
    ptr[6] = 0;
}

s32 FSi_ROMFAT_GetArchiveCaps(FSArchive *param_1, u32 *param_2) {
    *param_2 = 0;
    return 0;
}

s32 FSi_ROMFAT_OpenDirectory(FSArchive *param_1, FSFile *param_2, u32 param_3, const char *param_4, u32 param_5) {
    s32 result;
    u32 value = 0;

    result = FSi_ROMFAT_FindPath(param_1, param_3, param_4, &value, 1);
    if (result != 0) {
        return result;
    }
    return FSi_ROMFAT_SeekDirectory(param_1, param_2, value, 0);
}

s32 FSi_ROMFAT_CloseDirectory(FSArchive *param_1, FSFile *param_2) {
    param_2->prev = NULL;
    param_2->stat &= ~0x30;
    return 0;
}

s32 FSi_ROMFAT_GetPathInfo(FSArchive *param_1, u32 param_2, const char *param_3, void *info) {
    FSPathInfo *param_4 = info;
    s32 result = 5;
    u32 id = 0;
    FSFatEntry fat;
    FSArchiveContext *context;
    u32 pos;

    MI_CpuFill8(param_4, 0, 0x54);
    if (FSi_ROMFAT_FindPath(param_1, param_2, param_3, &id, 1) == 0) {
        param_4->attributes = 0x100;
        param_4->id = id;
        result = 0;
    } else if (FSi_ROMFAT_FindPath(param_1, param_2, param_3, &id, 0) == 0) {
        param_4->attributes = 0;
        param_4->id = id;
        param_4->fileSize = 0;
        context = param_1->context;
        pos = id * 8;
        if (pos < context->fatSize) {
            FSTableParam param;
            param.arc = param_1;
            param.pos = context->fat + pos;
            if (FSi_ReadTable(&param, &fat, 8) == 0) {
                param_4->fileSize = fat.bottom - fat.top;
                if (FSi_IsUnreadableRomOffset(param_1, fat.top) != 0) {
                    param_4->attributes |= 0x400;
                }
            }
        }
        result = 0;
    }
    param_4->attributes |= 0x200;
    return result;
}

s32 FSi_ROMFAT_GetArchiveResource(FSArchive *param_1, void *resource) {
    FSArchiveResource *param_2 = resource;
    const ROMHeader *header = (const ROMHeader *)CARD_GetRomHeader();

    param_2->bytesPerSector = 0;
    param_2->sectorsPerCluster = 0;
    param_2->totalClusters = 0;
    param_2->availableClusters = 0;
    param_2->totalSize = header->romSize;
    param_2->availableSize = 0;
    param_2->maxFileHandles = 0x7fffffff;
    param_2->currentFileHandles = 0;
    param_2->maxDirectoryHandles = 0x7fffffff;
    param_2->currentDirectoryHandles = 0;
    return 0;
}

// The interface of an archive that has tables
static const FSArchiveProcs FS_VTABLE_BASE = {
    .readFile = FSi_ROMFAT_ReadFile,
    .writeFile = FSi_ROMFAT_WriteFile,
    .seekDirectory = FSi_ROMFAT_SeekDirectory,
    .readDirectory = FSi_ROMFAT_ReadDirectory,
    .findPath = FSi_ROMFAT_FindPath,
    .getPath = FSi_ROMFAT_GetPath,
    .openFileFast = FSi_ROMFAT_OpenFileFast,
    .openFileDirect = FSi_ROMFAT_OpenFileDirect,
    .closeFile = FSi_ROMFAT_CloseFile,
    .activate = FSi_ROMFAT_Activate,
    .idle = FSi_ROMFAT_Idle,
    .suspend = FSi_ROMFAT_Suspend,
    .resume = FSi_ROMFAT_Resume,
    .openFile = FSi_ROMFAT_OpenFile,
    .seekFile = FSi_ROMFAT_SeekFile,
    .getFileLength = FSi_ROMFAT_GetFileLength,
    .getFilePosition = FSi_ROMFAT_GetFilePosition,
    .unmount = FSi_ROMFAT_Unmount,
    .getArchiveCaps = FSi_ROMFAT_GetArchiveCaps,
    .getPathInfo = FSi_ROMFAT_GetPathInfo,
    .getArchiveResource = FSi_ROMFAT_GetArchiveResource,
    .openDirectory = FSi_ROMFAT_OpenDirectory,
    .closeDirectory = FSi_ROMFAT_CloseDirectory,
};

u32 FSi_ReadMemCallback(FSArchive *arc, void *dst, u32 pos, u32 size) {
    void *src;

    src = (void *)FS_GetArchiveOffset(arc, pos);
    MI_CpuCopy8(src, dst, size);
    return 0;
}

u32 FSi_WriteMemCallback(FSArchive *arc, const void *src, u32 pos, u32 size) {
    void *dest;

    dest = (void *)FS_GetArchiveOffset(arc, pos);
    MI_CpuCopy8(src, dest, size);
    return 0;
}

BOOL FS_LoadArchive(FSArchive *param_1, u32 param_2, u32 param_3, u32 param_4, u32 param_5, u32 param_6,
                    FS_ARCHIVE_READ_FUNC param_7, FS_ARCHIVE_WRITE_FUNC param_8) {
    FSArchiveContext *context = &param_1->ctx;

    context->base = param_2;
    context->fatSize = param_4;
    context->fat = param_3;
    context->fatBackup = param_3;
    context->fntSize = param_6;
    context->fnt = param_5;
    context->fntBackup = param_5;
    context->readFunc = (param_7 != NULL) ? param_7 : FSi_ReadMemCallback;
    context->writeFunc = (param_8 != NULL) ? param_8 : FSi_WriteMemCallback;
    context->copyInMemory = 0;
    return FS_MountArchive(param_1, context, &FS_VTABLE_BASE, 0);
}

BOOL FS_UnloadArchive(FSArchive *arc) {
    return FS_UnmountArchive(arc);
}

BOOL FSi_GetFileLengthIfProc(FSFile *file, u32 *value) {
    u32 fileLength;
    u32 flag;

    flag = 0;
    if (file->arc->vtbl == &FS_VTABLE_BASE && (fileLength = FSi_ROMFAT_GetFileLength(file->arc, file, value), fileLength == 0)) {
        flag = 1;
    }
    return flag;
}

BOOL FSi_GetFilePositionIfProc(FSFile *file, u32 *value) {
    u32 filePosition;
    u32 flag;

    flag = 0;
    if (file->arc->vtbl == &FS_VTABLE_BASE && (filePosition = FSi_ROMFAT_GetFilePosition(file->arc, file, value), filePosition == 0)) {
        flag = 1;
    }
    return flag;
}

void FS_SetArchiveProc(FSArchive *arc, FS_ARCHIVE_PROC_FUNC proc, u32 flags) {
    FSArchiveContext *context = &arc->ctx;

    if (flags == 0) {
        proc = NULL;
    } else if (proc == NULL) {
        flags = 0;
    }
    context->proc = proc;
    context->procFlag = flags;
}

u32 FS_LoadArchiveTables(FSArchive *arc, void *mem, u32 maxSize) {
    FSArchiveContext *context;
    u32 totalSize;

    if (mem != NULL && FS_IsArchiveTableLoaded(arc)) {
        FS_UnloadArchiveTables(arc);
    }
    context = arc->context;
    totalSize = (context->fatSize + context->fntSize + 32 + 31) & ~31;
    if (totalSize <= maxSize) {
        u8 *cache = (u8 *)(((u32)mem + 31) & ~31);
        FSFile tmp;

        FS_InitFile(&tmp);
        if (FS_OpenFileDirect(&tmp, arc, context->fat, context->fat + context->fatSize, (u32)~0)) {
            if (FS_ReadFile(&tmp, cache, (s32)context->fatSize) < 0) {
                MI_CpuFill8(cache, 0, context->fatSize);
            }
            FS_CloseFile(&tmp);
        }
        context->fat = (u32)cache;
        cache += context->fatSize;
        if (FS_OpenFileDirect(&tmp, arc, context->fnt, context->fnt + context->fntSize, (u32)~0)) {
            if (FS_ReadFile(&tmp, cache, (s32)context->fntSize) < 0) {
                MI_CpuFill8(cache, 0, context->fntSize);
            }
            FS_CloseFile(&tmp);
        }
        context->fnt = (u32)cache;
        context->copyInMemory = (u32)mem;
        arc->flag |= FS_ARCHIVE_FLAG_TABLE_LOAD;
    }
    return totalSize;
}

u32 FS_UnloadArchiveTables(FSArchive *param_1) {
    u32 copyInMemory;
    s32 flag;
    FSArchiveContext *context;
    s32 result;


    copyInMemory = 0;
    if (param_1->flag & 2) {
        flag = 1;
    } else {
        flag = 0;
    }
    if (flag != 0) {
        context = param_1->context;
        result = FS_SuspendArchive(param_1);
        if (FS_IsArchiveTableLoaded(param_1) != 0) {
            param_1->flag &= ~4;
            copyInMemory = context->copyInMemory;
            context->fat = context->fatBackup;
            context->fnt = context->fntBackup;
            context->copyInMemory = 0;
        }
        if (result != 0) {
            FS_ResumeArchive(param_1);
        }
    }
    return copyInMemory;
}

// NitroSDK 4's FS_GetArchiveBase
u32 func_0207a78c(const FSArchive *arc) {
    return arc->context->base;
}

u32 FS_GetArchiveOffset(const FSArchive *arc, u32 pos) {
    return arc->context->base + pos;
}

BOOL FS_IsArchiveTableLoaded(const FSArchive *arc) {
    return (arc->flag & FS_ARCHIVE_FLAG_TABLE_LOAD) ? TRUE : FALSE;
}

u32 FS_GetFileImageTop(const FSFile *file) {
    return file->prop.file.top;
}
