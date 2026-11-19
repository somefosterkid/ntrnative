#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "heap.h"
#include "pm_string.h"
#include "pokemon_storage_system.h"
#include "message_format.h"
#include "msgdata.h"
#include "string_util.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021F5718(u8 *param_1, void *param_2, u32 param_3, u32 param_4, u32 param_5);
void ov14_021F57B8(u8 *param_1);
void ov14_021F58B8(u8 *param_1);

u8 ov14_021F4F24(Window *param_1, String *param_2, u32 param_3, u32 param_4, enum Font param_5, u32 param_6, s32 param_7);

void ov14_021F5718(u8 *param_1, void *param_2, u32 param_3, u32 param_4, u32 param_5) {
    Window *window;
    String *dest;

    window = Heap_AllocAtEnd(HEAP_ID_10, sizeof(Window));
    AddWindowParameterized(((UnkStruct_ov14_boxapp *)param_1)->sys->unk_14, window, 3, 0, 0, (u8)param_4, (u8)param_5, 0, param_3);
    MIi_CpuCopy32(param_2, window->pixelBuffer, param_4 * param_5 * 0x20);
    dest = String_New(0x14, HEAP_ID_10);
    PCStorage_GetBoxName(((PCStorage *)((UnkStruct_ov14_boxapp *)param_1)->storage), (u32) ((UnkStruct_ov14_boxapp *)param_1)->unk1F, dest);
    ov14_021F4F24(window, dest, (param_4 & 0x1fffffff) << 2, (param_5 & 0x1fffffff) * 4 - 8, FONT_SYSTEM, 0x20100, 2);
    String_Delete(dest);
    CopyWindowPixelsToVram_TextMode(window);
    RemoveWindow(window);
    Heap_Free(window);
    return;
}

void ov14_021F57B8(u8 *param_1) {
    String *pSVar1;
    s32 num;
    Window WStack_1c;

    InitWindow(&WStack_1c);
    AddTextWindowTopLeftCorner(((UnkStruct_ov14_boxapp *)param_1)->sys->unk_14, &WStack_1c, 0xc, 2, 0, 0);
    pSVar1 = String_New(0x14, HEAP_ID_10);
    PCStorage_GetBoxName(((PCStorage *)((UnkStruct_ov14_boxapp *)param_1)->storage), (u32) ((UnkStruct_ov14_boxapp *)param_1)->unk25, pSVar1);
    ov14_021F4F24(&WStack_1c, pSVar1, 0x30, 0, FONT_SYSTEM, 0x10200, 2);
    String_Delete(pSVar1);
    ov14_021F4EA0(*((void **)&((UnkStruct_ov14_boxapp *)param_1)->sys), &WStack_1c, 0);
    RemoveWindow(&WStack_1c);
    InitWindow(&WStack_1c);
    AddTextWindowTopLeftCorner(((UnkStruct_ov14_boxapp *)param_1)->sys->unk_14, &WStack_1c, 5, 2, 0, 0);
    pSVar1 = NewString_ReadMsgData(((UnkStruct_ov14_boxapp *)param_1)->sys->unk_20, 0x18);
    num = PCStorage_CountMonsAndEggsInBox(((PCStorage *)((UnkStruct_ov14_boxapp *)param_1)->storage), (u32) ((UnkStruct_ov14_boxapp *)param_1)->unk25);
    BufferIntegerAsString(((UnkStruct_ov14_boxapp *)param_1)->sys->unk_24, 0, num, 2, PRINTING_MODE_LEFT_ALIGN, 1);
    BufferIntegerAsString(((UnkStruct_ov14_boxapp *)param_1)->sys->unk_24, 1, 0x1e, 2, PRINTING_MODE_LEFT_ALIGN, 1);
    StringExpandPlaceholders(((UnkStruct_ov14_boxapp *)param_1)->sys->unk_24,
        ((UnkStruct_ov14_boxapp *)param_1)->sys->unk_28,
        pSVar1);
    ov14_021F4F24(&WStack_1c, ((UnkStruct_ov14_boxapp *)param_1)->sys->unk_28, 0x14, 0, FONT_SYSTEM, 0x10200, 2);
    String_Delete(pSVar1);
    ov14_021F4EA0(*((void **)&((UnkStruct_ov14_boxapp *)param_1)->sys), &WStack_1c, 1);
    RemoveWindow(&WStack_1c);
    return;
}

void ov14_021F58B8(u8 *param_1) {
    BOOL BVar1;
    String *string;
    u32 uVar2;
    Window WStack_1c;

    InitWindow(&WStack_1c);
    AddTextWindowTopLeftCorner(((UnkStruct_ov14_boxapp *)param_1)->sys->unk_14, &WStack_1c, 0xc, 2, 0, 0);
    uVar2 = (u32) *((u8 *)&((UnkStruct_ov14_boxapp *)param_1)->sys->unk_44D);
    if (uVar2 >= 0x10) {
        BVar1 = PCStorage_IsBonusWallpaperUnlocked(*(PCStorage **)(param_1 + 4), uVar2 - 0x10);
        if (BVar1 == 0) {
            string = NewString_ReadMsgData(((UnkStruct_ov14_boxapp *)param_1)->sys->unk_20, 0x3b);
        } else {
            string = NewString_ReadMsgData(((UnkStruct_ov14_boxapp *)param_1)->sys->unk_20,
                *((u8 *)&((UnkStruct_ov14_boxapp *)param_1)->sys->unk_44D) + 0x23);
        }
    } else {
        string = NewString_ReadMsgData(((UnkStruct_ov14_boxapp *)param_1)->sys->unk_20, uVar2 + 0x23);
    }
    ov14_021F4F24(&WStack_1c, string, 0x30, 0, FONT_SYSTEM, 0x10200, 2);
    String_Delete(string);
    ov14_021F4EA0(*((void **)&((UnkStruct_ov14_boxapp *)param_1)->sys), &WStack_1c, 0);
    RemoveWindow(&WStack_1c);
    return;
}
