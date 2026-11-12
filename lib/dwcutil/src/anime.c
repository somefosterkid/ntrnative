// libdwcutil's anime.c (Black's ov016 0x021bc820-0x021bc9e4): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"
#include "nitro/fx/fx_cp.h"

static const u8 data_ov016_021d9ff8[12] = { 14, 4, 5, 0, 230, 0, 139, 0, 0, 0, 168, 0 };

typedef struct AnimeWork {
    s32 *task;
    GXOamAttr *obj;
    s8 unk_8;
    u8 unk_9;
} AnimeWork;

static AnimeWork *data_ov016_021de504;
void taskAnime(void);

void DWCi_ANIMElInitEx(s32 color) {
    data_ov016_021de504 = DWCi_HEAPlAllocEx(sizeof(AnimeWork), 4);
    data_ov016_021de504->unk_8 = color;
    data_ov016_021de504->obj = DWCi_CEINlSetExObj(NULL, 0x47);
    G2_SetOBJPriority(data_ov016_021de504->obj, 1);
    G2_SetOBJMode(data_ov016_021de504->obj, GX_OAM_MODE_NORMAL, data_ov016_021d9ff8[color]);
    G2_SetOBJPosition(data_ov016_021de504->obj, 230, 139);
    data_ov016_021de504->task = DWCi_TSKlForm(1, (s32)taskAnime, NULL, 0x78);
}

void DWCi_ANIMElEnd(void) {
    DWCi_TSKlDeleteEx(1, data_ov016_021de504->task);
    DWCi_OBJlDelete((u32 *)data_ov016_021de504->obj);
    DWCi_HEAPlFree_((u32 *)&data_ov016_021de504);
}

void taskAnime(void) {
    s32 val;
    s32 *ptr;
    s32 *ptr2;
    int result;
    u8 *ptr3;

    *(u8 *)((u8 *)((u8 *)data_ov016_021de504) + 9) = (u8)FX_ModS32(*(u8 *)((u8 *)((u8 *)data_ov016_021de504) + 9) + 1,
            0x28);
    result = FX_DivS32((s32)(*(u8 *)((u8 *)((u8 *)data_ov016_021de504) + 9)), 5) + 0x47;
    func_ov016_021d52d4(0, result, *(s32 **)((u8 *)((u8 *)data_ov016_021de504) + 4));
    ptr = *(s32 **)(((u8 *)((u8 *)data_ov016_021de504)) + 4);
    *(u16 *)((u8 *)ptr + 4) = (u16)(0x400 | (*(u16 *)((u8 *)ptr + 4) & ~0xC00));
    ptr3 = (s32 *)((u8 *)(&data_ov016_021d9ff8) + *(u8 *)(((u8 *)((u8 *)data_ov016_021de504)) + 8));
    val = *ptr3;
    ptr = *(s32 **)((u8 *)((u8 *)data_ov016_021de504) + 4);
    *(s32 *)ptr &= ~0xC00;
    *(u16 *)((u8 *)ptr + 4) = (u16)((*(u16 *)((u8 *)ptr + 4) & ~0xF000) | (val << 0xC));
    ptr2 = *(s32 **)((u8 *)((u8 *)data_ov016_021de504) + 4);
    *ptr2 = (*ptr2 & 0xFE00FF00) | 0xE6008B;
}
