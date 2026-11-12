// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/fs.h, the lines in use so
// far.
#ifndef BWPORT_NITRO_FS_H
#define BWPORT_NITRO_FS_H

#include "types.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "nitro/card.h"

// Overlays, and the DMA channel the ROM archive reads with. The target is the proc, MI_PROCESSOR_ARM9 (nitro/mi.h)
#define FS_DMA_NOT_USE 0xffffffff

typedef void (*FSOverlayInitFunc)(void);

// An overlay's entry in the overlay table (the ROM's y9.bin), then where it is loaded for and its file's place in the
// ROM
typedef struct {
    u32 id;
    u8 *ram_address;
    u32 ram_size;
    u32 bss_size;
    FSOverlayInitFunc *sinit_init; // 0x10
    FSOverlayInitFunc *sinit_init_end; // 0x14
    u32 file_id; // 0x18
    u32 compressed : 24; // 0x1c
    u32 flag : 8; // 0x1f: FS_OVERLAY_FLAG_COMP
} FSOverlayInfoHeader;

typedef struct FSOverlayInfo {
    FSOverlayInfoHeader header;
    u32 target; // 0x20
    CARDRomRegion file_pos; // 0x24
} FSOverlayInfo;

#define FS_OVERLAY_FLAG_COMP 0x01

BOOL FS_LoadOverlayInfo(FSOverlayInfo *info, int target, u32 id);
BOOL FS_LoadOverlay(int target, u32 id);
BOOL FS_UnloadOverlay(int target, u32 id);
u32 FS_SetDefaultDMA(u32 dma);

// The digests of the overlays, 20 bytes each in the order of their numbers, and the end of the table: TwlSDK's linker
// script reserves them at the end of ARM9 main's .data (lib/nitro/src/fs/fs_overlay_digest.c)
typedef struct FSOverlayDigest {
    u8 bytes[20];
} FSOverlayDigest;

extern FSOverlayDigest SDK_OVERLAY_DIGEST[];
extern FSOverlayDigest SDK_OVERLAY_DIGEST_END[];

struct FSArchive;
struct FSFile;

// NitroSDK's archive callbacks (nitro/fs/archive.h, FSResult as a word): a read and a write of the archive's image,
// and the proc that takes the commands whose bits FSArchiveContext.procFlag has
typedef u32 (*FS_ARCHIVE_READ_FUNC)(struct FSArchive *arc, void *dst, u32 pos, u32 size);
typedef u32 (*FS_ARCHIVE_WRITE_FUNC)(struct FSArchive *arc, const void *src, u32 pos, u32 size);
typedef s32 (*FS_ARCHIVE_PROC_FUNC)(struct FSFile *file, u32 command);

typedef struct FSArchiveContext {
    u32 base; // 0x0
    u32 fat; // 0x4
    u32 fatSize; // 0x8
    u32 fnt; // 0xc: the directory table, also base2 of the image
    u32 fntSize; // 0x10
    u32 fatBackup; // 0x14
    u32 fntBackup; // 0x18
    u32 copyInMemory; // 0x1c: nonzero when the archive reads by copying memory
    FS_ARCHIVE_READ_FUNC readFunc; // 0x20
    FS_ARCHIVE_WRITE_FUNC writeFunc; // 0x24
    u8 unk_28[0x4];
    FS_ARCHIVE_PROC_FUNC proc; // 0x2c
    u32 procFlag; // 0x30: a bit per command the proc takes
} FSArchiveContext;


// Black's layout of FSFile, the fields the code uses (libntr's FSFile, the same offsets); 0x48 bytes.
typedef struct FSDirPos {
    struct FSArchive *arc;
    u16 ownId;
    u16 index;
    u32 pos;
} FSDirPos;

struct FSFileID;

typedef struct FSFile {
    union {
        u8 data[0x48];
        struct {
            struct FSFile *next; // 0x0
            struct FSFile *prev; // 0x4
            struct FSArchive *arc; // 0x8
            u32 stat;
            u32 command; // 0x10: the address of the command's argument block
            u32 error;
            OSThreadQueue queue; // 0x18
            union {
                struct {
                    u32 own_id;
                    u32 top;
                    u32 bottom;
                    u32 pos;
                } file; // 0x20
                struct {
                    FSDirPos pos; // 0x20
                    u32 parent; // 0x2c
                } dir;
            } prop;
            union {
                struct {
                    u32 top;
                    u32 bottom;
                    u32 index;
                } openfiledirect; // 0x30
                FSDirPos seekdir; // 0x30
                struct {
                    u32 buffer;
                    u32 length;
                    u16 total_len;
                    u16 dir_id;
                } getpath; // 0x30
                struct {
                    struct FSArchive *arc;
                    u16 ownId;
                    u16 unk_36;
                    u32 index;
                    u32 path;
                    u32 isDirectory;
                    union {
                        void *ptr;
                        struct FSFileID *file;
                        FSDirPos *dir;
                    } result;
                } findpath; // 0x30
                struct {
                    void *dst;
                    u32 len_org;
                    u32 len;
                } readfile;
                struct {
                    void *p_entry; // 0x30
                    u32 skip_string; // 0x34
                } readdir;
            } arg;
        };
    };
} FSFile;

#define FS_SEEK_SET 0

// A file by archive and number: NitroSDK's FS_ConvertPathToFileID and FS_OpenFileFast, and FS_OpenFileEx (which
// opens with a mode), under swan's names
typedef struct FSArchive FSArchive;

typedef struct FSFileID {
    struct FSArchive *arc;
    u32 file_id;
} FSFileID;

#define FS_FILEMODE_R 1

void FS_InitFile(FSFile *file);
BOOL FS_OpenFileFast(FSFile *file, FSFileID id);
void FS_CancelFile(FSFile *file);
s32 FSi_IncrementSjisPositionToSlash(const char *param_1, s32 param_2);
const char *FS_GetArchiveName(const FSArchive *arc);
BOOL FS_OpenFile(FSFile *file, const char *path);

// The command table of an archive (FSArchive.vtbl), indexed by command number
typedef struct FSArchiveProcs {
    s32 (*readFile)(struct FSArchive *arc, FSFile *file, void *buffer, u32 *length);
    s32 (*writeFile)(struct FSArchive *arc, FSFile *file, void *buffer, u32 *length);
    s32 (*seekDirectory)(struct FSArchive *arc, FSFile *file, u32 id, u32 position);
    s32 (*readDirectory)(struct FSArchive *arc, FSFile *file, void *info);
    s32 (*findPath)(struct FSArchive *arc, u32 baseId, const char *path, u32 *targetId, BOOL targetIsDirectory);
    s32 (*getPath)(struct FSArchive *arc, FSFile *file, BOOL isDirectory, char *buffer, u32 *length);
    s32 (*openFileFast)(struct FSArchive *arc, FSFile *file, u32 id, u32 mode);
    s32 (*openFileDirect)(struct FSArchive *arc, FSFile *file, u32 top, u32 bottom, u32 *id);
    s32 (*closeFile)(struct FSArchive *arc, FSFile *file);
    void (*activate)(struct FSArchive *arc);
    void (*idle)(struct FSArchive *arc);
    void (*suspend)(struct FSArchive *arc);
    void (*resume)(struct FSArchive *arc);
    s32 (*openFile)(struct FSArchive *arc, FSFile *file, u32 baseId, const char *path, u32 mode);
    s32 (*seekFile)(struct FSArchive *arc, FSFile *file, s32 *offset, s32 from);
    s32 (*getFileLength)(struct FSArchive *arc, FSFile *file, u32 *length);
    s32 (*getFilePosition)(struct FSArchive *arc, FSFile *file, u32 *position);
    void (*mount)(struct FSArchive *arc);
    void (*unmount)(struct FSArchive *arc);
    s32 (*getArchiveCaps)(struct FSArchive *arc, u32 *caps);
    s32 (*createFile)(struct FSArchive *arc, u32 baseId, const char *path, u32 permit);
    s32 (*deleteFile)(struct FSArchive *arc, u32 baseId, const char *path);
    s32 (*renameFile)(struct FSArchive *arc, u32 baseIdSrc, const char *pathSrc, u32 baseIdDst, const char *pathDst);
    s32 (*getPathInfo)(struct FSArchive *arc, u32 baseId, const char *path, void *info);
    s32 (*setPathInfo)(struct FSArchive *arc, u32 baseId, const char *path, void *info);
    s32 (*createDirectory)(struct FSArchive *arc, u32 baseId, const char *path, u32 permit);
    s32 (*deleteDirectory)(struct FSArchive *arc, u32 baseId, const char *path);
    s32 (*renameDirectory)(struct FSArchive *arc, u32 baseIdSrc, const char *pathSrc, u32 baseIdDst, const char *pathDst);
    s32 (*getArchiveResource)(struct FSArchive *arc, void *resource);
    void *unused_74;
    s32 (*flushFile)(struct FSArchive *arc, FSFile *file);
    s32 (*setFileLength)(struct FSArchive *arc, FSFile *file, u32 length);
    s32 (*openDirectory)(struct FSArchive *arc, FSFile *file, u32 baseId, const char *path, u32 mode);
    s32 (*closeDirectory)(struct FSArchive *arc, FSFile *file);
    s32 (*setSeekCache)(struct FSArchive *arc, FSFile *file, void *buffer, u32 size);
    u8 reserved[116];
} FSArchiveProcs;

// A file system's archive, such as "rom". Only its name, the first field, is known
struct FSArchive {
    char name[4];
    struct FSArchive *next; // 0x4: the next registered archive
    struct FSFile *pending; // 0x8: the list of commands waiting on the archive
    OSThreadQueue queue; // 0xc
    u32 flag; // 0x14: 0x8 suspended
    u32 unk_18;
    u32 unk_1c;
    FSArchiveContext *context; // 0x20: the mounted archive's userdata
    const FSArchiveProcs *vtbl; // 0x24
    FSArchiveContext ctx; // 0x28: the archive's own context, which context points at when mounted
};

// NitroSDK 4's archive functions, which TwlSDK 5 keeps over FS_MountArchive (fs_archive.c, fs_proc_default.c), and
// FS_GetArchiveBase (func_0207a78c), FS_GetArchiveOffset and FS_IsArchiveTableLoaded, inline in NitroSDK 4
void FS_InitArchive(FSArchive *arc);
BOOL FS_RegisterArchiveName(FSArchive *arc, const char *name, u32 nameLength);
void FS_ReleaseArchiveName(FSArchive *arc);
BOOL FS_LoadArchive(FSArchive *arc, u32 base, u32 fat, u32 fatSize, u32 fnt, u32 fntSize, FS_ARCHIVE_READ_FUNC readFunc,
                    FS_ARCHIVE_WRITE_FUNC writeFunc);
BOOL FS_UnloadArchive(FSArchive *arc);
u32 FS_LoadArchiveTables(FSArchive *arc, void *mem, u32 maxSize);
u32 FS_UnloadArchiveTables(FSArchive *arc);
void FS_SetArchiveProc(FSArchive *arc, FS_ARCHIVE_PROC_FUNC proc, u32 flags);
u32 func_0207a78c(const FSArchive *arc);
u32 FS_GetArchiveOffset(const FSArchive *arc, u32 pos);
BOOL FS_IsArchiveTableLoaded(const FSArchive *arc);
// TwlSDK 5's FS_GetFileImageTop, a function: where the file starts in its archive
u32 FS_GetFileImageTop(const FSFile *file);
// The current directory: NitroSDK's FS_SetCurrentDirectory and TwlSDK 5's FS_ChangeDir (func_02078c34), which calls it
BOOL FS_SetCurrentDirectory(const char *path);
BOOL func_02078c34(const char *path);
// An archive's asynchronous command has ended with result (the archive's read callback's FS_RESULT_PROC_ASYNC)
void FS_NotifyArchiveAsyncEnd(FSArchive *arc, u32 result);
// A file of the ROM archive from offset, size bytes
BOOL FS_CreateFileFromRom(FSFile *file, u32 offset, u32 size);
// fs_proc_rom.c: whether offset is in the TWL area of the ROM, which a DS can't read
u32 FSi_IsUnreadableRomOffset(FSArchive *arc, u32 offset);
// fs_proc_default.c: the file's length and position, when its archive takes the default commands
BOOL FSi_GetFileLengthIfProc(FSFile *file, u32 *length);
BOOL FSi_GetFilePositionIfProc(FSFile *file, u32 *position);

u32 FS_GetLength(FSFile *file);
s32 FS_ReadFile(FSFile *file, void *dest, s32 size);
BOOL FS_CloseFile(FSFile *file);
BOOL FS_SeekFile(FSFile *file, s32 offset, int whence);

BOOL FS_CreateFileFromMemory(FSFile *file, void *data, u32 size);

// FS_TryLoadTable gives the size of the file tables, which it reads when given memory
void FS_Init(u32 dma);
u32 FS_TryLoadTable(void *mem, u32 size);

// fs_file.c's, at 0x02078aec
s32 FS_GetFilePosition(FSFile *file);

// NitroSDK's FS_IsAvailable, FS_OpenFileEx (with a mode), FS_GetFileLength and FS_WriteFile
BOOL FS_IsAvailable(void);
BOOL FS_OpenFileEx(FSFile *file, const char *path, u32 mode);
u32 FS_GetFileLength(FSFile *file);
s32 FS_WriteFile(FSFile *file, const void *src, s32 size);

#define FS_ARCHIVE_FLAG_LOADED 0x02 // the file tables were handed to FS_LoadArchive
#define FS_ARCHIVE_FLAG_TABLE_LOAD 0x04 // FS_LoadArchiveTables copied the file tables into memory
#define FS_ARCHIVE_FLAG_SUSPEND 0x08 // suspended
#define FS_ARCHIVE_FLAG_RUNNING 0x10 // a command is running
#define FS_ARCHIVE_FLAG_CANCELING 0x20 // a file's command is being canceled (FS_CancelFile)
#define FS_ARCHIVE_FLAG_SUSPENDING 0x40 // waiting for the running command to suspend

// NitroSDK's FS_IsArchiveLoaded: whether the archive is loaded
static inline BOOL FS_IsArchiveLoaded(volatile const FSArchive *arc) {
    return (arc->flag & FS_ARCHIVE_FLAG_LOADED) ? TRUE : FALSE;
}

// NitroSDK's FS_IsArchiveSuspended: whether the archive is suspended
static inline BOOL FS_IsArchiveSuspended(volatile const FSArchive *arc) {
    return (arc->flag & FS_ARCHIVE_FLAG_SUSPEND) ? TRUE : FALSE;
}

// The SDK's FSi_WaitConditionChange (libraries/fs/include/util.h): sleeps on queue until a bit of on is set or the
// bits of off are clear
static inline void FSi_WaitConditionChange(u32 *flags, u32 on, u32 off, OSThreadQueue *queue) {
    OSIntrMode bak = OS_DisableInterrupts();
    while ((!on || ((*flags & on) == 0)) && (!off || ((*flags & off) != 0))) {
        OS_SleepThread(queue);
    }
    (void)OS_RestoreInterrupts(bak);
}

static inline void FSi_WaitConditionOn(u32 *flags, u32 bits, OSThreadQueue *queue) {
    FSi_WaitConditionChange(flags, bits, 0, queue);
}

static inline void FSi_WaitConditionOff(u32 *flags, u32 bits, OSThreadQueue *queue) {
    FSi_WaitConditionChange(flags, 0, bits, queue);
}

#define FS_FILE_STATUS_BUSY 0x01 // a command is pending or running
#define FS_FILE_STATUS_CANCEL 0x02 // its command is being canceled (FS_CancelFile)
#define FS_FILE_STATUS_IS_FILE 0x10 // it is an open file

// NitroSDK's FS_IsBusy: whether a command of the file is pending or running
static inline BOOL FS_IsBusy(volatile const FSFile *file) {
    return (file->stat & FS_FILE_STATUS_BUSY) ? TRUE : FALSE;
}

// NitroSDK's FS_IsFile: whether the file is open
static inline BOOL FS_IsFile(volatile const FSFile *file) {
    return (file->stat & FS_FILE_STATUS_IS_FILE) ? TRUE : FALSE;
}

// NitroSDK's FS_IsCanceling: whether the command of the file is being canceled
static inline BOOL FS_IsCanceling(volatile const FSFile *file) {
    return (file->stat & 0x02) ? TRUE : FALSE;
}

// NitroSDK's static inline functions that Black's headers lacked, as a final ROM compiles them (no assertions).
// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/fs/api.h, include/nitro/fs/file.h, include/nitro/fs/overlay.h
static inline u32 FS_GetTableSize(void) { return FS_TryLoadTable(NULL, 0); }
static inline BOOL FS_LoadTable(void *mem, u32 size) {
  return (FS_TryLoadTable(mem, size) <= size);
}
static inline BOOL FS_SeekFileToBegin (FSFile * p_file)
{
	return FS_SeekFile(p_file, 0, FS_SEEK_SET);
}
static inline FSArchive * FS_GetAttachedArchive (const FSFile * p_file)
{
	return p_file->arc;
}
static inline void FS_RegisterOverlayToDebugger (const FSOverlayInfo * p_ovi, const FSFile * p_file)
{
	(void)p_ovi;
	(void)p_file;
}

BOOL FS_ConvertPathToFileID(FSFileID *fileId, const char *path); // swan's fs_resolve_file
BOOL FS_OpenFileFast(FSFile *file, FSFileID fileId);             // swan's romfs_fopen_id
BOOL FS_OpenFileEx(FSFile *file, const char *path, u32 mode);    // swan's romfs_fopen_core
// NitroSDK's FS_CancelFile: asks the file's archive to stop the command running on it
void FS_CancelFile(FSFile *file);

#endif // BWPORT_NITRO_FS_H
