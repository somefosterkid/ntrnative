#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

u8 ov14_021F4F24(Window *param_1, String *param_2, u32 param_3, u32 param_4, enum Font param_5, u32 param_6, s32 param_7);

void ov14_021F6408(UnkStruct_ov14_boxapp *param_1, s32 param_2) {
    u8 bVar1;
    void *ptr;
    u32 uVar3;
    String *string;
    UnkStruct_ov14_boxsys *iVar4;
    void *pvVar5;
    u32 uVar6;
    NNSG2dCharacterData *pNStack_18;
    u8 uVar2;
    Window *sum;
    iVar4 = param_1->sys;
    sum = &iVar4->windows[0x2b];
    bVar1 = GetWindowWidth(sum);
    uVar2 = bVar1;
    ptr = GfGfxLoader_GetCharData(NARC_a_0_1_9, 0xe, 1, &pNStack_18, HEAP_ID_10);
    pvVar5 = pNStack_18->pRawData;
    BlitBitmapRect(sum, (void *)(((u8 *)pvVar5) + 0x7a0), 0, 0, 0x18, 8, 0, 0, 0x18, 8, 0xff);
    BlitBitmapRect(sum, (void *)(((u8 *)pvVar5) + 0xba0), 0, 0, 0x18, 8, 0, 8, 0x18, 8, 0xff);
    BlitBitmapRect(sum, (void *)(((u8 *)pvVar5) + 4000), 0, 0, 0x18, 8, 0, 0x10, 0x18, 8, 0xff);
    uVar3 = uVar2 - 1;
    BlitBitmapRect(sum, (void *)(((u8 *)pvVar5) + 0xd00), 0, 0, 8, 8, (u16)(uVar3 << 3), 0, 8, 8, 0xff);
    BlitBitmapRect(sum, (void *)(((u8 *)pvVar5) + 0xce0), 0, 0, 8, 8, (u16)(uVar3 << 3), 8, 8, 8, 0xff);
    BlitBitmapRect(sum, (void *)(((u8 *)pvVar5) + 0xcc0), 0, 0, 8, 8, (u16)(uVar3 << 3), 0x10, 8, 8, 0xff);
    uVar6 = 3;
    if (3 < ((s32)uVar3)) {
        do {
            BlitBitmapRect(sum, (void *)(pvVar5 + 0xc20), 0, 0, 8, 8, (u16)(uVar6 << 3), 0, 8, 8, 0xff);
            BlitBitmapRect(sum, (void *)(pvVar5 + 0xca0), 0, 0, 8, 8, (u16)(uVar6 << 3), 8, 8, 8, 0xff);
            BlitBitmapRect(sum, (void *)(((u8 *)pvVar5) + 0xc80), 0, 0, 8, 8, (u16)(uVar6 << 3), 0x10, 8, 8, 0xff);
            uVar6 = (uVar6 + 1) & 0xff;
        } while (((s32)uVar6) < ((s32)(uVar2 - 1)));
    }
    Heap_Free(ptr);
    string = NewString_ReadMsgData(param_1->sys->unk_20, 0x41);
    ov14_021F4F24(sum, string, (uVar2 * 8) / 2, 4, FONT_UNK_4, 0x90a00, 2);
    String_Delete(string);
    CopyWindowPixelsToVram_TextMode(sum);
    return;
}
