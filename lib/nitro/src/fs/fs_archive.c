// TwlSDK's FS library, fs_archive.c: Black's main 0x02077634-0x020787a0. The file is libntr's (MIT,
// https://github.com/cybervisi0n/libntr) for SDK 5; the names are kh-recoded-decomp's twins' (MIT) or libntr's in the
// same place.

#include "types.h"
#include "nitro/fs.h"
#include "nitro/mi.h"

void FSi_EndCommand(FSFile *param_1, u32 param_2);
FSFile *FSi_NextCommand(FSArchive *param_1, s32 param_2);
s32 FSi_InvokeCommand(FSFile *file, u32 command);
void FSi_ExecuteAsyncCommand(FSFile *param_1);
BOOL FSi_IsEventCommand(s32 param_1);
void OS_Terminate(void);
FSArchive *FS_NormalizePath(const char *param_1, u32 *param_2, char *param_3);
BOOL FSi_SendCommand(FSFile *param_1, s32 param_2, s32 param_3);
u32 FS_UnmountArchive(FSArchive *param_1);
// The SDK's arc_list and current_dir_pos: two variables, both initialized, so MWCC reaches the second through the
// first's address (`[r1, #4]`) while `&arc_list` gets a pool word of its own.
static FSArchive *data_02150b0c = NULL;
static FSDirPos data_02150b10 = {0};
static char data_02150c1c[0x104];
static char data_02150b1c[16][16];
FSArchive *FS_FindArchive(const char *param_1, s32 param_2);
s32 FSi_DecrementSjisPositionToSlash(u8 *param_1, s32 param_2);
s32 FSi_TrimSjisTrailingSlash(u8 *param_1);
BOOL FS_SuspendArchive(FSArchive *param_1);
BOOL FS_ResumeArchive(FSArchive *param_1);
extern int STD_CompareNString(const char * str1, const char * str2, int len);
void FS_InitFile(FSFile *file);

BOOL FSi_IsEventCommand(s32 param_1) {
    s32 flag;
    u32 val;

    val = param_1 - 9;
    flag = 0;
    if (val > 0x1AU) {
        return 0;
    }
    if (0x0400030F & (1 << val)) {
        flag = 1;
    }
    return flag;
}

void FSi_EndCommand(FSFile *param_1, u32 param_2) {
    u32 state;
    FSArchive *arc;
    FSFile **link;
    FSFile *cur;
    u32 priority;

    state = OS_DisableInterrupts();
    arc = param_1->arc;
    if (arc != NULL) {
        link = &arc->pending;
        cur = arc->pending;
        while (cur != NULL) {
            if (cur == param_1) {
                *link = param_1->next;
                break;
            }
            link = &cur->next;
            cur = cur->next;
        }
        param_1->next = NULL;
    }
    priority = (param_1->stat >> 8) & 0xff;
    if (!FSi_IsEventCommand(priority)) {
        if (arc != NULL) {
            arc->unk_18 = priority;
            arc->unk_1c = param_2;
        }
    }
    param_1->error = param_2;
    param_1->stat &= ~0xcf;
    OS_WakeupThread(&param_1->queue);
    OS_RestoreInterrupts(state);
}

// The argument blocks the commands read from FSFile.command
typedef struct { void *buffer; u32 length; } ArgBuffer;
typedef struct { u32 first; u32 second; } ArgPair;
typedef struct { void *ptr; } ArgPtr;
typedef struct { u32 value; } ArgValue;
typedef struct { u32 baseId; const char *path; u32 targetId; BOOL targetIsDirectory; } ArgFindPath;
typedef struct { BOOL isDirectory; char *buffer; u32 length; } ArgGetPath;
typedef struct { u32 id; u32 top; u32 bottom; } ArgOpenDirect;
typedef struct { u32 baseId; const char *path; u32 mode; } ArgPathMode;
typedef struct { u32 baseId; const char *path; void *info; } ArgPathInfo;
typedef struct { s32 offset; s32 from; } ArgSeek;
typedef struct { u32 baseId; const char *path; } ArgPath;
typedef struct { u32 baseIdSrc; const char *pathSrc; u32 baseIdDst; const char *pathDst; } ArgRename;

s32 FSi_WaitForArchiveCompletion(s32 param_1, s32 param_2) {
    u32 mask;

    if (param_2 == 0x100) {
        mask = OS_DisableInterrupts();
        while (((*(u32 *)(param_1 + 0xc)) & 8) == 0) {
            OS_SleepThread((OSThreadQueue *)(param_1 + 0x18));
        }
        OS_RestoreInterrupts(mask);
        param_2 = *(s32 *)(param_1 + 0x14);
        *(u32 *)(param_1 + 0xc) = *(u32 *)(param_1 + 0xc) & 0xfffffff7;
    }
    return param_2;
}

s32 FSi_InvokeCommand(FSFile *file, u32 command) {
    s32 result;
    FSArchive *arc = file->arc;
    const FSArchiveProcs *procs = arc->vtbl;

    if (command >= 0x23) {
        result = 4;
    } else if (((void *const *)procs)[command] == NULL) {
        result = 4;
    } else {
        switch (command) {
        case 0: {
            ArgBuffer *arg = (ArgBuffer *)file->command;
            result = procs->readFile(arc, file, arg->buffer, &arg->length);
            break;
        }
        case 1: {
            ArgBuffer *arg = (ArgBuffer *)file->command;
            result = procs->writeFile(arc, file, arg->buffer, &arg->length);
            break;
        }
        case 2: {
            ArgPair *arg = (ArgPair *)file->command;
            result = procs->seekDirectory(arc, file, arg->first, arg->second);
            break;
        }
        case 3: {
            ArgPtr *arg = (ArgPtr *)file->command;
            result = procs->readDirectory(arc, file, arg->ptr);
            break;
        }
        case 4: {
            ArgFindPath *arg = (ArgFindPath *)file->command;
            result = procs->findPath(arc, arg->baseId, arg->path, &arg->targetId, arg->targetIsDirectory);
            break;
        }
        case 5: {
            ArgGetPath *arg = (ArgGetPath *)file->command;
            result = procs->getPath(arc, file, arg->isDirectory, arg->buffer, &arg->length);
            break;
        }
        case 6: {
            ArgPair *arg = (ArgPair *)file->command;
            result = procs->openFileFast(arc, file, arg->first, arg->second);
            break;
        }
        case 7: {
            ArgOpenDirect *arg = (ArgOpenDirect *)file->command;
            result = procs->openFileDirect(arc, file, arg->top, arg->bottom, &arg->id);
            break;
        }
        case 8:
            result = procs->closeFile(arc, file);
            break;
        case 9:
            procs->activate(arc);
            return 0;
        case 10:
            procs->idle(arc);
            return 0;
        case 11:
            procs->suspend(arc);
            return 0;
        case 12:
            procs->resume(arc);
            return 0;
        case 13: {
            ArgPathMode *arg = (ArgPathMode *)file->command;
            result = procs->openFile(arc, file, arg->baseId, arg->path, arg->mode);
            break;
        }
        case 14: {
            ArgSeek *arg = (ArgSeek *)file->command;
            result = procs->seekFile(arc, file, &arg->offset, arg->from);
            break;
        }
        case 15: {
            ArgValue *arg = (ArgValue *)file->command;
            result = procs->getFileLength(arc, file, &arg->value);
            break;
        }
        case 16: {
            ArgValue *arg = (ArgValue *)file->command;
            result = procs->getFilePosition(arc, file, &arg->value);
            break;
        }
        case 17:
            procs->mount(arc);
            return 0;
        case 18:
            procs->unmount(arc);
            return 0;
        case 19: {
            ArgValue *arg = (ArgValue *)file->command;
            result = procs->getArchiveCaps(arc, &arg->value);
            break;
        }
        case 20: {
            ArgPathMode *arg = (ArgPathMode *)file->command;
            result = procs->createFile(arc, arg->baseId, arg->path, arg->mode);
            break;
        }
        case 21: {
            ArgPath *arg = (ArgPath *)file->command;
            result = procs->deleteFile(arc, arg->baseId, arg->path);
            break;
        }
        case 22: {
            ArgRename *arg = (ArgRename *)file->command;
            result = procs->renameFile(arc, arg->baseIdSrc, arg->pathSrc, arg->baseIdDst, arg->pathDst);
            break;
        }
        case 23: {
            ArgPathInfo *arg = (ArgPathInfo *)file->command;
            result = procs->getPathInfo(arc, arg->baseId, arg->path, arg->info);
            break;
        }
        case 24: {
            ArgPathInfo *arg = (ArgPathInfo *)file->command;
            result = procs->setPathInfo(arc, arg->baseId, arg->path, arg->info);
            break;
        }
        case 25: {
            ArgPathMode *arg = (ArgPathMode *)file->command;
            result = procs->createDirectory(arc, arg->baseId, arg->path, arg->mode);
            break;
        }
        case 26: {
            ArgPath *arg = (ArgPath *)file->command;
            result = procs->deleteDirectory(arc, arg->baseId, arg->path);
            break;
        }
        case 27: {
            ArgRename *arg = (ArgRename *)file->command;
            result = procs->renameDirectory(arc, arg->baseIdSrc, arg->pathSrc, arg->baseIdDst, arg->pathDst);
            break;
        }
        case 28: {
            ArgPtr *arg = (ArgPtr *)file->command;
            result = procs->getArchiveResource(arc, arg->ptr);
            break;
        }
        case 30:
            result = procs->flushFile(arc, file);
            break;
        case 31: {
            ArgValue *arg = (ArgValue *)file->command;
            result = procs->setFileLength(arc, file, arg->value);
            break;
        }
        case 32: {
            ArgPathMode *arg = (ArgPathMode *)file->command;
            result = procs->openDirectory(arc, file, arg->baseId, arg->path, arg->mode);
            break;
        }
        case 33:
            result = procs->closeDirectory(arc, file);
            break;
        case 34: {
            ArgBuffer *arg = (ArgBuffer *)file->command;
            result = procs->setSeekCache(arc, file, arg->buffer, arg->length);
            break;
        }
        default:
            result = 4;
            break;
        }
    }
    if (!FSi_IsEventCommand(command)) {
        if ((file->stat & 4) != 0) {
            result = FSi_WaitForArchiveCompletion((s32)file, result);
        } else if (result != 0x100) {
            FSi_EndCommand(file, result);
        }
    }
    return result;
}


FSFile *FSi_NextCommand(FSArchive *arc, s32 start) {
    FSFile *next = NULL;
    u32 interrupts;
    u32 activateInterrupts;
    u32 runInterrupts;
    BOOL activate;

    interrupts = OS_DisableInterrupts();
    if (arc->flag & 0x20) {
        FSFile *file = arc->pending;

        arc->flag &= ~0x20;
        while (file != NULL) {
            FSFile *following = file->next;

            if (FS_IsCanceling(file)) {
                FSi_EndCommand(file, 3);
                if (following == NULL) {
                    following = arc->pending;
                }
            }
            file = following;
        }
    }
    OS_RestoreInterrupts(interrupts);

    activateInterrupts = OS_DisableInterrupts();
    if (!(arc->flag & 0x40) && !(arc->flag & 0x8) && arc->pending != NULL) {
        activate = start && !(arc->flag & 0x10);
        if (activate) {
            arc->flag |= 0x10;
        }
        OS_RestoreInterrupts(activateInterrupts);
        if (activate) {
            FSi_InvokeCommand(arc->pending, 9);
        }
        runInterrupts = OS_DisableInterrupts();
        if (start || activate) {
            next = arc->pending;
            next->stat |= 0x40;
        }
        if (start && (next->stat & 0x4)) {
            OS_WakeupThread(&next->queue);
            next = NULL;
        }
        OS_RestoreInterrupts(runInterrupts);
    } else {
        if (start) {
            if (arc->flag & 0x10) {
                FSFile file;

                FS_InitFile(&file);
                file.arc = arc;
                arc->flag &= ~0x10;
                FSi_InvokeCommand(&file, 10);
            }
            if (arc->flag & 0x40) {
                arc->flag = (arc->flag & ~0x40) | 0x8;
                OS_WakeupThread(&arc->queue);
            }
        }
        OS_RestoreInterrupts(activateInterrupts);
    }
    return next;
}

void FSi_ExecuteAsyncCommand(FSFile *param_1) {
    FSFile *file = param_1;
    FSArchive *arc = file->arc;
    u32 state;

    if (file == NULL) {
        return;
    }
    do {
        state = OS_DisableInterrupts();
        file->stat |= 0x40;
        if ((file->stat & 0x4) != 0) {
            OS_WakeupThread(&file->queue);
            file = NULL;
        }
        OS_RestoreInterrupts(state);
        if (file == NULL) {
            return;
        }
        if (FSi_InvokeCommand(file, (file->stat >> 8) & 0xff) == 0x100) {
            return;
        }
        file = FSi_NextCommand(arc, 1);
    } while (file != NULL);
}

static inline u32 FSi_GetCurrentCommand(const FSFile *file) {
    return (file->stat >> 8) & 0xff;
}

void FSi_ExecuteSyncCommand(FSFile *param_1) {
    u32 state;
    FSArchive *arc;
    FSFile *next;

    state = OS_DisableInterrupts();
    while ((param_1->stat & 0x40) == 0 && (param_1->stat & 0x1) != 0) {
        OS_SleepThread(&param_1->queue);
    }
    OS_RestoreInterrupts(state);
    if ((param_1->stat & 0x40) == 0) {
        return;
    }
    arc = param_1->arc;
    FSi_EndCommand(param_1, FSi_InvokeCommand(param_1, FSi_GetCurrentCommand(param_1)));
    next = FSi_NextCommand(arc, 1);
    if (next != NULL) {
        FSi_ExecuteAsyncCommand(next);
    }
}

BOOL FSi_SendCommand(FSFile *param_1, s32 param_2, s32 param_3) {
    FSArchive *arc;
    u32 state;
    BOOL owner;
    BOOL result = FALSE;
    BOOL busy;

    arc = param_1->arc;
    busy = FS_IsBusy(param_1);
    if (busy) {
        OS_Terminate();
    }
    if (arc == NULL) {
        param_1->error = 6;
        return FALSE;
    }
    param_1->error = 2;
    param_1->stat = (param_1->stat & ~0xff00) | (param_2 << 8) | 0x1;
    param_1->next = NULL;
    if (param_3 != 0) {
        param_1->stat |= 0x4;
    }
    state = OS_DisableInterrupts();
    if ((arc->flag & 0x80) != 0) {
        FSi_EndCommand(param_1, 3);
    } else {
        FSFile **link = &arc->pending;
        while (*link != NULL) {
            link = &(*link)->next;
        }
        *link = param_1;
    }
    owner = arc->pending == param_1 && (arc->flag & 0x10) == 0;
    OS_RestoreInterrupts(state);
    if (param_1->error != 3) {
        FSFile *next = FSi_NextCommand(arc, owner);
        if (param_3 != 0) {
            FSi_ExecuteSyncCommand(param_1);
            result = (param_1->error == 0);
        } else {
            if (next != NULL) {
                FSi_ExecuteAsyncCommand(next);
            }
            result = TRUE;
        }
    }
    return result;
}

FSArchive *FS_FindArchive(const char *param_1, s32 param_2) {
    u32 state;
    FSArchive *arc;
    char *name;
    s32 mounted;

    state = OS_DisableInterrupts();
    arc = data_02150b0c;
    while (arc != NULL) {
        if (arc->flag & 0x2) {
            mounted = 1;
        } else {
            mounted = 0;
        }
        if (mounted != 0) {
            name = (char *)FS_GetArchiveName(arc);
            if (STD_CompareNString(name, param_1, param_2) == 0 && name[param_2] == 0) {
                break;
            }
        }
        arc = arc->next;
    }
    OS_RestoreInterrupts(state);
    return arc;
}

BOOL FS_SetCurrentDirectory(const char *path) {
    s32 result = 0;
    u32 outValue = 0;
    char buffer[0x104];
    FSArchive *owner;
    FSFile file;
    struct {
        u32 field0;
        char *buffer;
        u32 field8;
        u32 field_c;
    } descriptor;

    owner = FS_NormalizePath(path, &outValue, buffer);
    if (owner != NULL) {
        data_02150b10.arc = owner;
        data_02150b10.ownId = 0;
        data_02150b10.index = 0;
        data_02150b10.pos = 0;
        STD_CopyLString(data_02150c1c, buffer, 0x104);
        if (((u32 *)owner->vtbl)[4] != 0) {
            FS_InitFile(&file);
            file.arc = owner;
            file.command = (u32)&descriptor;
            descriptor.field0 = outValue;
            descriptor.buffer = buffer;
            descriptor.field_c = 1;
            if (FSi_SendCommand(&file, 4, 1) != 0) {
                data_02150b10.ownId = (u16)descriptor.field8;
                STD_CopyLString(data_02150c1c, buffer, 0x104);
            }
        }
        result = 1;
    }
    return result;
}

s32 FSi_CopySafeString(char *param_1, s32 param_2, const char *param_3, s32 param_4, BOOL *param_5) {
    s32 i;
    s32 count = (param_2 - 1 < param_4) ? (param_2 - 1) : param_4;

    for (i = 0; i < count && param_3[i] != 0; i++) {
        param_1[i] = param_3[i];
    }
    if (i < param_4 && param_3[i] != 0) {
        *param_5 = TRUE;
    }
    param_1[i] = 0;
    return i;
}

static inline BOOL FSi_IsSlash(u32 c) {
    return (c == 0x2f) || (c == 0x5c);
}

static inline BOOL FSi_IsSjisLeadByte(s32 c) {
    return (u32)(((u8)c ^ 0x20) - 0xa1) < 0x3c;
}

static inline BOOL FSi_IsSjisTrailByte(s32 c) {
    return (c != 0x7f) && ((u8)(c - 0x40) <= 0xbc);
}

static inline BOOL FSi_IsSjisCharacter(const char *s) {
    return FSi_IsSjisLeadByte(s[0]) && FSi_IsSjisTrailByte(s[1]);
}

static inline s32 FSi_IncrementSjisPosition(const char *str, s32 pos) {
    return pos + 1 + FSi_IsSjisLeadByte(str[pos]);
}

FSArchive *FS_NormalizePath(const char *param_1, u32 *param_2, char *param_3) {
    FSArchive *arc = NULL;
    s32 pos = 0;
    BOOL error = FALSE;

    if (data_02150b10.arc == NULL) {
        data_02150b10.arc = data_02150b0c;
        data_02150b10.ownId = 0;
        data_02150b10.pos = 0;
        data_02150b10.index = 0;
        data_02150c1c[0] = 0;
    }
    if (FSi_IsSlash((u8)*param_1)) {
        arc = data_02150b10.arc;
        param_1++;
        if (param_2 != NULL) {
            *param_2 = 0;
        }
    } else {
        s32 i;
        for (i = 0;; i = FSi_IncrementSjisPosition(param_1, i)) {
            u32 c = (u8)param_1[i];
            if (c == 0 || FSi_IsSlash(c)) {
                arc = data_02150b10.arc;
                if (param_2 != NULL) {
                    *param_2 = data_02150b10.ownId;
                }
                if (param_3 != NULL && data_02150b10.ownId == 0 && data_02150c1c[0] != 0) {
                    pos += FSi_CopySafeString(param_3, 0x104, data_02150c1c, 0x104, &error);
                    pos += FSi_CopySafeString(param_3 + pos, 0x104 - pos, "/", 1, &error);
                }
                break;
            } else if (c == 0x3a) {
                arc = FS_FindArchive(param_1, i);
                param_1 += i + 1;
                if (FSi_IsSlash((u8)*param_1)) {
                    param_1++;
                }
                if (param_2 != NULL) {
                    *param_2 = 0;
                }
                break;
            }
        }
    }
    if (param_3 != NULL) {
        s32 len = 0;
        while (!error) {
            s8 c = param_1[len];
            if (c != 0 && !FSi_IsSlash((u8)c)) {
                len += FSi_IsSjisCharacter(&param_1[len]) ? 2 : 1;
            } else {
                if (len == 0) {
                } else if (len == 1 && param_1[0] == 0x2e) {
                } else if (len == 2 && param_1[0] == 0x2e && param_1[1] == 0x2e) {
                    if (pos > 0) {
                        pos--;
                    }
                    pos = FSi_DecrementSjisPositionToSlash((u8 *)param_3, pos) + 1;
                } else {
                    pos += FSi_CopySafeString(param_3 + pos, 0x104 - pos, param_1, len, &error);
                    if (c != 0) {
                        pos += FSi_CopySafeString(param_3 + pos, 0x104 - pos, "/", 1, &error);
                    }
                }
                if (c == 0) {
                    break;
                }
                param_1 += len + 1;
                len = 0;
            }
        }
        param_3[pos] = 0;
        FSi_TrimSjisTrailingSlash((u8 *)param_3);
    }
    if (error) {
        arc = NULL;
    }
    return arc;
}

void FS_InitArchive(FSArchive *arc) {
    MI_CpuFill8(arc, 0, sizeof(FSArchive));
    OS_InitThreadQueue(&arc->queue);
}

BOOL FS_RegisterArchiveName(FSArchive *param_1, const char *param_2, u32 param_3) {
    BOOL result = FALSE;
    FSArchive *cur;
    FSArchive **link;
    u32 state;
    s32 i;

    state = OS_DisableInterrupts();
    if (FS_FindArchive(param_2, param_3) == NULL) {
        link = &data_02150b0c;
        cur = data_02150b0c;
        while (cur != NULL) {
            link = &cur->next;
            cur = cur->next;
        }
        *link = param_1;
        if (param_3 <= 3) {
            *(u32 *)param_1->name = 0;
            STD_CopyLString(param_1->name, param_2, param_3 + 1);
        } else if (param_3 <= 15) {
            for (i = 0;; i++) {
                if (i >= 16) {
                    OS_Terminate();
                } else if (data_02150b1c[i][0] == 0) {
                    STD_CopyLString(data_02150b1c[i], param_2, param_3 + 1);
                    *(char **)param_1->name = data_02150b1c[i];
                    break;
                }
            }
        } else {
            OS_Terminate();
        }
        param_1->flag |= 0x1;
        result = TRUE;
    }
    OS_RestoreInterrupts(state);
    return result;
}

void FS_ReleaseArchiveName(FSArchive *param_1) {
    u32 state;
    FSArchive *cur;
    FSArchive **link;

    if (param_1 == data_02150b0c) {
        OS_Terminate();
    }
    if (*(u32 *)param_1->name == 0) {
        return;
    }
    state = OS_DisableInterrupts();
    link = (FSArchive **)&data_02150b0c;
    cur = data_02150b0c;
    while (cur != NULL) {
        if (cur == param_1) {
            *link = cur->next;
            break;
        }
        link = &cur->next;
        cur = cur->next;
    }
    if (param_1->name[3] != 0) {
        ((char *)*(u32 *)param_1->name)[0] = 0;
    }
    *(u32 *)param_1->name = 0;
    param_1->next = NULL;
    param_1->flag &= ~0x1;
    if (data_02150b10.arc == param_1) {
        data_02150b10.arc = NULL;
    }
    OS_RestoreInterrupts(state);
}

const char *FS_GetArchiveName(const FSArchive *arc) {
    return (arc->name[3] != 0) ? *(const char *const *)arc->name : arc->name;
}

BOOL FS_MountArchive(FSArchive *param_1, FSArchiveContext *param_2, const void *param_3, u32 reserved) {
    FSFile file;

    param_1->context = param_2;
    param_1->vtbl = param_3;
    FS_InitFile(&file);
    file.arc = param_1;
    FSi_InvokeCommand(&file, 0x11);
    param_1->flag |= 0x2;
    return TRUE;
}

u32 FS_UnmountArchive(FSArchive *param_1) {
    u32 state;
    BOOL suspended;
    FSFile *cur;
    u32 val;
    FSFile *next;
    FSFile file;

    state = OS_DisableInterrupts();
    val = (param_1->flag & 0x2) != 0;
    if (val) {
        suspended = FS_SuspendArchive(param_1);
        param_1->flag |= 0x80;
        cur = param_1->pending;
        while (cur != NULL) {
            next = cur->next;
            FSi_EndCommand(cur, 3);
            cur = next;
        }
        param_1->pending = NULL;
        if (suspended) {
            FS_ResumeArchive(param_1);
        }
        FS_InitFile(&file);
        file.arc = param_1;
        FSi_InvokeCommand(&file, 0x12);
        param_1->flag &= ~0xa2;
    }
    OS_RestoreInterrupts(state);
    return 1;
}

BOOL FS_SuspendArchive(FSArchive *param_1) {
    OSIntrMode bak_psr = OS_DisableInterrupts();
    const BOOL bak_stat = !FS_IsArchiveSuspended(param_1);

    if (bak_stat) {
        if ((param_1->flag & FS_ARCHIVE_FLAG_RUNNING) == 0) {
            param_1->flag |= FS_ARCHIVE_FLAG_SUSPEND;
        } else {
            param_1->flag |= FS_ARCHIVE_FLAG_SUSPENDING;
            FSi_WaitConditionOff(&param_1->flag, FS_ARCHIVE_FLAG_SUSPENDING, &param_1->queue);
        }
    }
    (void)OS_RestoreInterrupts(bak_psr);
    return bak_stat;
}

BOOL FS_ResumeArchive(FSArchive *param_1) {
    OSIntrMode savedState = OS_DisableInterrupts();
    BOOL result = !FS_IsArchiveSuspended(param_1);
    FSFile *file;

    if (!result) {
        param_1->flag &= ~0x8;
    }
    OS_RestoreInterrupts(savedState);
    file = FSi_NextCommand(param_1, 1);
    if (file != NULL) {
        FSi_ExecuteAsyncCommand(file);
    }
    return result;
}

void FS_NotifyArchiveAsyncEnd(FSArchive *arc, u32 result) {
    u32 mask;
    FSFile *file;

    file = arc->pending;
    if ((file->stat & 4) != 0) {
        mask = OS_DisableInterrupts();
        file->stat |= 8;
        file->error = result;
        OS_WakeupThread(&file->queue);
        OS_RestoreInterrupts(mask);
        return;
    }
    FSi_EndCommand(file, result);
    file = FSi_NextCommand(arc, 1);
    if (file == NULL) {
        return;
    }
    FSi_ExecuteAsyncCommand(file);
}
