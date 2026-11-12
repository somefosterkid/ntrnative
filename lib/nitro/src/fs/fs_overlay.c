// TwlSDK's FS library, fs_overlay.c: Black's main 0x02078cb0-0x02079304. The file is libntr's (MIT,
// https://github.com/cybervisi0n/libntr) for SDK 5; the names are kh-recoded-decomp's twins' (MIT) or libntr's in the
// same place.

#include "types.h"
#include "nitro/card.h"
#include "nitro/fs.h"
#include "nitro/mi.h"

void DC_FlushRange(const void *addr, u32 size);
void OS_Terminate(void);
void func_02004df4(u32 param_1);
// A global destructor: the chain of them the overlays register
typedef struct MWiDestructorChain {
    struct MWiDestructorChain *next;
    void (*dtor)(void *obj);
    void *obj;
} MWiDestructorChain;
extern MWiDestructorChain *__global_destructor_chain;
u32 FS_GetFileLength(FSFile *param_1);
void FS_EndOverlay(FSOverlayInfo *param_1);
extern u32 data_02150d20[];
extern u8 SDK_OVERLAY_NUMBER[];
static u32 data_02150d30[4];
static u32 data_02150d28[2];
BOOL OS_IsRunOnTwl(void);
void SVC_CalcHMACSHA1(u32 *param_1, u32 param_2, u32 param_3, u8 *param_4, u32 param_5);
int MATHi_SetOverlayTableMode(int flag);
void MATH_CalcHMACSHA1(u32 *param_1, u32 param_2, u32 param_3, u8 *param_4, u32 param_5);
BOOL FS_LoadOverlayImage(FSOverlayInfo *param_1);
FSFileID FS_GetOverlayFileID(const FSOverlayInfo *param_1);
BOOL FS_OpenFileFast(FSFile *param_1, FSFileID param_2);
void FS_ClearOverlayImage(FSOverlayInfo *info);
void FS_StartOverlay(FSOverlayInfo *param_1);
int func_02700000(void);
BOOL FS_CloseFile(FSFile *file);
void FS_InitFile(FSFile *file);
s32 FS_ReadFile(FSFile *file, void *dest, s32 size);

u32 FSi_GetOverlayBinarySize(FSOverlayInfo *param_1) {
    u32 size = ((param_1->header.flag & FS_OVERLAY_FLAG_COMP) != 0) ? param_1->header.compressed : param_1->header.ram_size;
    return size;
}

void FS_ClearOverlayImage(FSOverlayInfo *info) {
    u8 *addr;
    u32 imageSize;
    u32 size;

    addr = info->header.ram_address;
    imageSize = info->header.ram_size;
    size = imageSize + info->header.bss_size;
    IC_InvalidateRange(addr, size);
    DC_InvalidateRange(addr, size);
    MI_CpuFill8(addr + imageSize, 0, size - imageSize);
}

FSFileID FS_GetOverlayFileID(const FSOverlayInfo *p_ovi) {
    FSFileID ret;
    ret.arc = (FSArchive *)data_02150d20[1];
    ret.file_id = p_ovi->header.file_id;
    return ret;
}

BOOL FS_LoadOverlayInfo(FSOverlayInfo *info, int target, u32 id) {
    BOOL retval = FALSE;
    const u32 pos = id * 0x20;
    u32 *pr;

    if (target == 0) {
        pr = data_02150d28;
    } else {
        pr = data_02150d30;
    }
    if (id >= (u32)SDK_OVERLAY_NUMBER && !OS_IsRunOnTwl()) {
    } else if (pr[0] != 0) {
        if (pos < pr[1]) {
            FSFile file;
            FS_InitFile(&file);
            MI_CpuCopy8((const void *)(pr[0] + pos), info, 0x20);
            info->target = target;
            if (FS_OpenFileFast(&file, FS_GetOverlayFileID(info))) {
                info->file_pos.offset = FS_GetFileImageTop(&file);
                info->file_pos.length = FS_GetFileLength(&file);
                FS_CloseFile(&file);
                retval = TRUE;
            }
        }
    } else {
        if (target == 0) {
            pr = (u32 *)&CARD_GetOwnRomHeader()->main_ovt;
        } else {
            pr = (u32 *)&CARD_GetOwnRomHeader()->sub_ovt;
        }
        if (pos < pr[1]) {
            FSFile file;
            FS_InitFile(&file);
            if (FS_CreateFileFromRom(&file, pr[0] + pos, pr[0] + pr[1])) {
                if (FS_ReadFile(&file, info, 0x20) != 0x20) {
                    FS_CloseFile(&file);
                } else {
                    FS_CloseFile(&file);
                    info->target = target;
                    if (FS_OpenFileFast(&file, FS_GetOverlayFileID(info))) {
                        info->file_pos.offset = FS_GetFileImageTop(&file);
                        info->file_pos.length = FS_GetFileLength(&file);
                        FS_CloseFile(&file);
                        retval = TRUE;
                    }
                }
            }
        }
    }
    return retval;
}

BOOL FS_LoadOverlayImage(FSOverlayInfo *param_1) {
    FSFile file;
    FSFileID id;
    u32 size;
    BOOL success = FALSE;

    FS_InitFile(&file);
    id = FS_GetOverlayFileID(param_1);
    if (FS_OpenFileFast(&file, id) != 0) {
        size = FSi_GetOverlayBinarySize(param_1);
        FS_ClearOverlayImage(param_1);
        if (FS_ReadFile(&file, param_1->header.ram_address, size) == size) {
            success = TRUE;
        }
        FS_CloseFile(&file);
    }
    return success;
}

BOOL func_02078f30(const u8 *param_1, u32 param_2, u32 param_3, s32 param_4) {
    u32 localA[5];
    u8 localB[0x40];
    u32 i;

    MI_CpuFill8(localA, 0, 0x14);
    MI_CpuCopy8((const void *)data_02150d20[6], localB, data_02150d20[7]);
    if (param_4 == 0 && OS_IsRunOnTwl()) {
        SVC_CalcHMACSHA1(localA, param_2, param_3, localB, data_02150d20[7]);
    } else {
        s32 saved = 0;
        if (param_4 != 0) {
            saved = MATHi_SetOverlayTableMode(1);
        }
        MATH_CalcHMACSHA1(localA, param_2, param_3, localB, data_02150d20[7]);
        if (param_4 != 0) {
            MATHi_SetOverlayTableMode(saved);
        }
    }
    for (i = 0; i < 0x14; i += 4) {
        if (*(u32 *)((u8 *)localA + i) != *(u32 *)(param_1 + i)) {
            break;
        }
    }
    return i == 0x14;
}

void FS_StartOverlay(FSOverlayInfo *param_1) {
    u32 size = FSi_GetOverlayBinarySize(param_1);
    BOOL result;
    FSOverlayInitFunc *cur;
    FSOverlayInitFunc *end;

    if ((u32)param_1->header.ram_address >= (u32)func_02700000 && (u32)param_1->header.ram_address < 0x2735b40 && OS_IsRunOnTwl()) {
        if (data_02150d20[0] == 0) {
            data_02150d20[0] = 1;
        }
    }
    if (OS_GetBootType() != 1) {
        result = FALSE;
        if ((param_1->header.flag & 0x2) != 0 && param_1->header.id < SDK_OVERLAY_DIGEST_END - SDK_OVERLAY_DIGEST) {
            result = func_02078f30(SDK_OVERLAY_DIGEST[param_1->header.id].bytes, (u32)param_1->header.ram_address, size, 0);
        }
        if (!result) {
            MI_CpuFill8(param_1->header.ram_address, 0, size);
            OS_Terminate();
            return;
        }
    }
    if ((param_1->header.flag & FS_OVERLAY_FLAG_COMP) != 0) {
        func_02004df4((u32)param_1->header.ram_address + size);
    }
    DC_FlushRange(param_1->header.ram_address, param_1->header.ram_size);
    cur = param_1->header.sinit_init;
    end = param_1->header.sinit_init_end;
    while (cur < end) {
        if (*cur != NULL) {
            (*cur)();
        }
        cur++;
    }
}

void FS_EndOverlay(FSOverlayInfo *param_1) {
    for (;;) {
        MWiDestructorChain *head = NULL;
        MWiDestructorChain *tail = NULL;
        const u32 regionTop = (u32)param_1->header.ram_address;
        const u32 regionBottom = regionTop + (param_1->header.ram_size + param_1->header.bss_size);

        {
            u32 state = OS_DisableInterrupts();
            MWiDestructorChain *prev = NULL;
            MWiDestructorChain *base = __global_destructor_chain;
            MWiDestructorChain *p = base;

            while (p != NULL) {
                MWiDestructorChain *next = p->next;
                const u32 dtor = (u32)p->dtor;
                const u32 obj = (u32)p->obj;

                if ((obj == 0 && dtor >= regionTop && dtor < regionBottom) ||
                    (obj >= regionTop && obj < regionBottom)) {
                    if (tail == NULL) {
                        head = p;
                    } else {
                        tail->next = p;
                    }
                    if (base == p) {
                        base = __global_destructor_chain = next;
                    }
                    tail = p;
                    p->next = NULL;
                    if (prev != NULL) {
                        prev->next = next;
                    }
                } else {
                    prev = p;
                }
                p = next;
            }
            OS_RestoreInterrupts(state);
        }

        if (head == NULL) {
            break;
        }

        do {
            MWiDestructorChain *next = head->next;
            if (head->dtor != NULL) {
                head->dtor(head->obj);
            }
            head = next;
        } while (head != NULL);
    }
}

BOOL FS_UnloadOverlayImage(FSOverlayInfo *param_1) {
    FS_EndOverlay(param_1);
    return 1;
}

BOOL FS_LoadOverlay(int target, u32 id) {
    BOOL result;
    BOOL BVar2;
    BOOL BVar3;
    FSOverlayInfo FStack_38;

    BVar3 = 0;
    BVar2 = FS_LoadOverlayInfo(&FStack_38, target, id);
    if ((BVar2 != 0) && (result = FS_LoadOverlayImage(&FStack_38), result)) {
        FS_StartOverlay(&FStack_38);
        BVar3 = 1;
    }
    return BVar3;
}

BOOL FS_UnloadOverlay(int target, u32 id) {
    BOOL BVar1;
    s32 result;
    FSOverlayInfo FStack_38;

    BVar1 = FS_LoadOverlayInfo(&FStack_38, target, id);
    if ((BVar1 != 0) && (result = FS_UnloadOverlayImage(&FStack_38), result != 0)) {
        return 1;
    }
    return 0;
}
