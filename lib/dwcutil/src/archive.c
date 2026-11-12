// libdwcutil's archive.c (Black's ov016 0x021d486c-0x021d4cf4): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void cbReadRom(void *param_1);
static void *data_ov016_021de66c;
BOOL cmpBack(s8 *param_1, s8 *param_2, s32 param_3);
s32 userProc(FSFile *file, u32 command);
u32 func_ov016_021d4afc(FSArchive *arc, void *dst, u32 pos, u32 size);
u32 cbWrite(FSArchive *arc, const void *src, u32 pos, u32 size);


static const char data_ov016_021daa60[] = "dwc";
void DWCi_ARClInit(void) {
    FSFile file;
    CARDRomRegion fnt;
    CARDRomRegion fat;
    u32 size;
    u8 pass[128];

    data_ov016_021de66c = DWCi_HEAPlAllocEx(0xe8, 4);
    FS_InitFile(&file);
    if (FS_OpenFile(&file, "rom:/dwc/utility.bin") == 0) {
        OS_Terminate();
    }
    *(u16 *)((u8 *)data_ov016_021de66c + 0xE4) = OS_GetLockID();
    size = FS_GetFileImageTop(&file);
    FS_ReadFile(&file, &fnt, 8);
    FS_ReadFile(&file, &fat, 8);
    FS_CloseFile(&file);
    FS_InitArchive((FSArchive *)((u8 *)data_ov016_021de66c + 0x88));
    if (FS_RegisterArchiveName((FSArchive *)((u8 *)data_ov016_021de66c + 0x88), data_ov016_021daa60, 3) == 0) {
        OS_Terminate();
    }
    FS_SetArchiveProc((FSArchive *)((u8 *)data_ov016_021de66c + 0x88), userProc, 0x602);
    if (FS_LoadArchive((FSArchive *)((u8 *)data_ov016_021de66c + 0x88), size, fat.offset, fat.length,
                       fnt.offset, fnt.length, func_ov016_021d4afc, cbWrite) == 0) {
        OS_Terminate();
    }
    size = FS_LoadArchiveTables((FSArchive *)((u8 *)data_ov016_021de66c + 0x88), NULL, 0);
    *(void **)data_ov016_021de66c = DWCi_HEAPlAllocEx(size, 4);
    FS_LoadArchiveTables((FSArchive *)((u8 *)data_ov016_021de66c + 0x88), *(void **)data_ov016_021de66c, size);
    *(u32 *)((u8 *)data_ov016_021de66c + 0x84) = DWCi_QUElFormEx(0x20, (s32)((u8 *)data_ov016_021de66c + 4), 4);
    OS_SPrintf((char *)pass, "%s:/", data_ov016_021daa60);
    func_02078c34((const char *)pass);
}

void func_ov016_021d4a20(void) {
    func_02078c34("rom:/");
    FS_UnloadArchiveTables((FSArchive *)((u8 *)data_ov016_021de66c + 0x88));
    FS_UnloadArchive((FSArchive *)((u8 *)data_ov016_021de66c + 0x88));
    FS_ReleaseArchiveName((FSArchive *)((u8 *)data_ov016_021de66c + 0x88));
    OS_ReleaseLockID(*(u16 *)((u8 *)data_ov016_021de66c + 0xE4));
    *(u16 *)((u8 *)data_ov016_021de66c + 0xE4) = 0;
    DWCi_HEAPlFree_((u32 *)data_ov016_021de66c);
    *(s32 *)data_ov016_021de66c = 0;
    DWCi_HEAPlFree_((u32 *)&data_ov016_021de66c);
}

s32 userProc(FSFile *file, u32 command) {
    switch (command) {
    case 9:
        CARD_LockRom(*(u16 *)((u8 *)(u8 *)data_ov016_021de66c + 0xE4));
        return 0U;
    case 10:
        CARD_UnlockRom(*(u16 *)((u8 *)(u8 *)data_ov016_021de66c + 0xE4));
        return 0U;
    case 1:
        return 4U;
    default:
        return 0x102U;
    }
}

u32 func_ov016_021d4afc(FSArchive *arc, void *dst, u32 pos, u32 size) {
    CARDi_ReadRom(0xffffffff, (const void *)(pos + func_0207a78c(arc)), dst, size, cbReadRom, arc, TRUE);
    return 0x100;
}

void cbReadRom(void *arc) {
    FS_NotifyArchiveAsyncEnd((FSArchive *)arc, 0);
}

u32 cbWrite(FSArchive *arc, const void *src, u32 pos, u32 size) {
    return 1;
}

u32 *DWCi_ARClReadEx(s8 *path, u32 *param_2, s32 alignment) {
    FSFile sp4;
    u32 *sp0;
    s32 val;
    u32 *dest;
    u32 *dest2;
    BOOL result;
    u32 val2;
    u32 size;

    DWCi_QUElPopBack(*(u16 **)((u8 *)(u8 *)data_ov016_021de66c + 0x84));
    FS_InitFile(&sp4);
    if (FS_OpenFile(&sp4, path) == 0) {
        OS_Terminate();
    }
    size = FS_GetLength(&sp4);
    if (param_2 != NULL) {
        *param_2 = size;
    }
    result = cmpBack(path, (u8 *)".l", 2);
    val = -4;
    if (result == 0) {
        val = alignment;
    }
    dest2 = DWCi_HEAPlAllocEx(size, val);
    sp0 = dest2;
    FS_ReadFile(&sp4, dest2, (s32) size);
    FS_CloseFile(&sp4);
    if (val > 0) {
        return sp0;
    }
    val2 = (u32) *sp0 >> 8;
    if (param_2 != NULL) {
        *param_2 = val2;
    }
    dest = DWCi_HEAPlAllocEx(val2, alignment);
    MI_UncompressLZ8(sp0, dest);
    DWCi_HEAPlFree_((u32 *) &sp0);
    return dest;
}

void DWCi_ARClRelease(u32 file) {
    DWCi_HEAPlFree_(&file);
    DWCi_QUElPushBack(*(u16 **)((u8 *)((u8 *)data_ov016_021de66c) + 0x84), file);
}

BOOL cmpBack(s8 *str0, s8 *str1, s32 n) {
    u32 len;
    u32 len2;
    len2 = strlen(str0);
    len = strlen(str1);
    if ((s32) len2 < n || (s32) len < n) {
        return 0;
    }
    if (memcmp(&str0[len2 - n], &str1[len - n], (u32) n) == 0) {
        return 1;
    }
    return 0;
}
