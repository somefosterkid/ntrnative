// TwlSDK's FS library, fs_proc_rom.c: Black's main 0x0207a980-0x0207ad04. The file is libntr's (MIT,
// https://github.com/cybervisi0n/libntr) for SDK 5; the names are kh-recoded-decomp's twins' (MIT) or libntr's in the
// same place.

#include "types.h"
#include "nitro/card.h"
#include "nitro/fs.h"
#include "nitro/mi.h"
#include "nitro/std.h"

extern char data_020a84b0[4];
extern char data_020a84b4[8];
extern u32 data_02150da0[2];
extern FSArchive data_02150da8;

void FSi_InitRomArchive(u32 param_1);
BOOL FS_OpenFileDirect(FSFile *param_1, FSArchive *param_2, u32 param_3, u32 param_4, u32 param_5);
BOOL OS_IsRunOnTwl(void);
BOOL FS_SuspendArchive(FSArchive *param_1);
BOOL FS_ResumeArchive(FSArchive *param_1);
u32 FSi_IsUnreadableRomOffset(FSArchive *arc, u32 offset) {
    s8 *a;
    BOOL BVar2;
    u32 flag;

    flag = 0;
    a = (s8 *)FS_GetArchiveName(arc);
    if (((STD_CompareString(a, (void *)data_020a84b0) == 0) && (BVar2 = OS_IsRunOnTwl(), BVar2 == 0)) &&
          ((u32)CARD_GetOwnRomHeader()->twl_ltd_area_rom_offset << 0x13 <= offset)) {
        flag = 1;
    }
    return flag;
}

void FSi_OnRomReadDone(void *arc) {
    s32 result;
    u32 ret;

    result = CARD_IsPulledOut();
    ret = 5;
    if (result == 0) {
        ret = 0;
    }
    FS_NotifyArchiveAsyncEnd((FSArchive *)arc, ret);
}

u32 FSi_ReadRomCallback(FSArchive *arc, void *dst, u32 pos, u32 size) {
    CARDi_ReadRom(data_02150da0[0], (const void *)pos, dst, size, FSi_OnRomReadDone, arc, TRUE);
    return 0x100;
}

s32 FSi_RomArchiveProc(FSFile *param_1, u32 param_2) {
    switch (param_2) {
    case 9:
        CARD_LockRom((u16)data_02150da0[1]);
        return 0;
    case 10:
        CARD_UnlockRom((u16)data_02150da0[1]);
        return 0;
    case 7:
        if (FSi_IsUnreadableRomOffset(param_1->arc, param_1->arg.openfiledirect.top) != 0) {
            return 9;
        }
        return 0x101;
    case 1:
        return 4;
    }
    return 0x102;
}

s32 FSi_EmptyArchiveProc(FSFile *file, u32 command) {
    if (command == 1) {
        return 4;
    }
    return 0x102;
}

// The empty archive's read and write: FS_RESULT_UNSUPPORTED
u32 func_0207aaf8(FSArchive *arc, void *dst, u32 pos, u32 size) {
    return 4;
}

u32 func_0207ab00(FSArchive *arc, const void *src, u32 pos, u32 size) {
    return 4;
}

u32 func_0207ab08(FSArchive *archive) {
    return 0;
}

void FSi_InitRomArchive(u32 dma) {
    const CARDRomHeader *header;
    const CARDRomHeader *header2;
    u32 fnt;
    u32 fat;

    CARD_Init();
    data_02150da0[0] = dma;
    data_02150da0[1] = OS_GetLockID();
    FS_InitArchive(&data_02150da8);
    FS_RegisterArchiveName(&data_02150da8, data_020a84b0, 3);
    if (OS_GetBootType() == 1) {
        header = CARD_GetOwnRomHeader();
        header2 = CARD_GetOwnRomHeader();
        FS_SetArchiveProc(&data_02150da8, FSi_RomArchiveProc, 0x682);
        fnt = header->fnt.offset;
        if (fnt != -1U && fnt != 0 && (fat = header2->fat.offset, fat != -1U) && fat != 0) {
            FS_LoadArchive(&data_02150da8, 0, fat, header2->fat.length, fnt, header->fnt.length, FSi_ReadRomCallback, NULL);
        }
    } else {
        func_0207ab08(&data_02150da8);
    }
    if (!FS_IsArchiveLoaded(&data_02150da8)) {
        FS_SetArchiveProc(&data_02150da8, FSi_EmptyArchiveProc, (u32)-1);
        FS_LoadArchive(&data_02150da8, 0, 0, 0, 0, 0, func_0207aaf8, func_0207ab00);
    }
    FS_SetCurrentDirectory(data_020a84b4);
}

u32 FS_SetDefaultDMA(u32 dma) {
    u32 state;
    u32 old;
    BOOL suspended;

    state = OS_DisableInterrupts();
    old = data_02150da0[0];
    suspended = FS_SuspendArchive(&data_02150da8);
    data_02150da0[0] = dma;
    if (suspended) {
        FS_ResumeArchive(&data_02150da8);
    }
    OS_RestoreInterrupts(state);
    return old;
}

u32 FS_TryLoadTable(void *mem, u32 size) {
    return FS_LoadArchiveTables(&data_02150da8, mem, size);
}

BOOL FS_CreateFileFromRom(FSFile *file, u32 offset, u32 size) {
    return FS_OpenFileDirect(file, &data_02150da8, offset, offset + size, 0xffff);
}

// The .data and .bss this file owns; MWCC lays the .bss out in reverse of the definitions
char data_020a84b0[4] = "rom";
char data_020a84b4[8] = "rom:";

FSArchive data_02150da8;
u32 data_02150da0[2];
