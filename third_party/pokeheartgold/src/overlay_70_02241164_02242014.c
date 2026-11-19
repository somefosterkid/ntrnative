#include "global.h"
#include "overlay_70_gtsapplication_state.h"

#include "heap.h"
#include "message_printer.h"
#include "sprite.h"
#include "unk_0201956C.h"
#include "unk_struct_ov70_02242e58.h"

s32 ov70_02238F9C(Sprite *, s32, s32);

UnkStruct_ov70_02242E58 *ov70_02242014(GTSSearchPanelTemplate *arg0, s32 arg1, s32 arg2);

UnkStruct_ov70_02242E58 *ov70_02242014(GTSSearchPanelTemplate *arg0, s32 arg1, s32 arg2) {
    UnkStruct_ov70_02242E58 *v0;

    v0 = Heap_Alloc(HEAP_ID_61, sizeof(UnkStruct_ov70_02242E58));
    v0->unk_0 = arg0->bgConfig;
    v0->unk_4 = arg0->menuButtonWindows;
    v0->unk_8 = arg0->unk_08;
    v0->unk_C = arg0->unk_0C;
    v0->unk_10 = arg0->unk_10;
    v0->unk_14 = arg0->unk_14;
    v0->unk_18 = arg0->cursorSprite;
    v0->unk_24 = arg0->gtsMessageLoader;
    v0->unk_28 = arg0->speciesMessageLoader;
    v0->unk_2C = arg0->countryMessageLoader;
    v0->unk_20 = arg0->pokedex;
    v0->unk_30 = arg0->dexSeenFlags;
    v0->unk_54 = arg1;
    v0->unk_60 = arg2;
    v0->unk_3C = 0;
    v0->unk_3E = 0;
    v0->unk_40 = 0;
    v0->unk_42 = 0;
    v0->unk_44 = 0;
    v0->unk_45 = 0;
    v0->unk_48 = 0;
    v0->unk_7E = 0;
    v0->unk_4C = 0;
    v0->unk_1C = sub_0201956C(v0->unk_0, 2, 1, HEAP_ID_61);
    sub_020195F4(v0->unk_1C, 0, 2, 0x20, 0x14);
    ov70_02238F9C(v0->unk_C, 0x88, 0x28);
    Sprite_SetAnimCtrlSeq(v0->unk_C, 0x2F);
    v0->unk_38 = MessagePrinter_New(0xFU, 0xEU, 2U, HEAP_ID_61);
    return v0;
}

void ov70_022420C4(UnkStruct_ov70_02242E58 *arg0, s16 arg1);

void ov70_022420C4(UnkStruct_ov70_02242E58 *arg0, s16 arg1) {
    arg0->unk_58 = arg1;
    switch (arg1) {
    case 0:
        arg0->unk_48 = 0;
        arg0->unk_7E = 0;
        arg0->unk_3C = 0;
        arg0->unk_3E = 0;
        arg0->unk_4C = 4;
        return;
    case 3:
        arg0->unk_48 = 0;
        arg0->unk_7E = 0;
        arg0->unk_3C = 0;
        arg0->unk_3E = 0;
        arg0->unk_4C = 0x10;
        return;
    case 1:
        arg0->unk_48 = 0;
        arg0->unk_7E = 0;
        arg0->unk_4C = 0x1C;
        return;
    case 2:
        arg0->unk_48 = 0;
        arg0->unk_7E = 0;
        arg0->unk_4C = 0x1F;
    default:
        return;
    }
}

void ov70_0224212C(UnkStruct_ov70_02242E58 *ptr);

void ov70_0224212C(UnkStruct_ov70_02242E58 *ptr) {
    MessagePrinter_Delete(ptr->unk_38);
    sub_020195C0(ptr->unk_1C);
    Heap_Free(ptr);
}
