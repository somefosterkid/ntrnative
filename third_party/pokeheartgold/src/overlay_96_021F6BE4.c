#include "global.h"

#include "constants/heap.h"

#include "filesystem_files_def.h"
#include "sprite.h"
#include "overlay_96_unk_struct_ov96_021_f5018.h"
#include "overlay_96_unk_struct_ov96_021_f74a4.h"

void ov96_021F6B50(s32 param_1, s32 param_2, s32 *param_3);

void ov96_021F6BE4(UnkStruct_ov96_021F5018 *param_1, s32 param_2, u8 param_3, s32 param_4);

Sprite *ov96_021EA634(void *, SpriteList *, s32, enum HeapID);
void *ov96_021EB3E4(enum HeapID *, s32, s32, u8, u8);
void ov96_021EB52C(void *, s32, s32);
void ov96_021EB564(void *, s32);
SpriteList *ov96_021EB5E8(enum HeapID *);
void ov96_021EB630(void *, s32);
void ov96_021F74D0(UnkStruct_ov96_021F74A4 *, enum HeapID *, s32);
extern u8 ov96_0221C0B0[2];
extern u8 ov96_0221C0B4[2];
void ov96_021F6C5C(UnkStruct_ov96_021F5018 *arg0, enum HeapID *arg1);

void ov96_021F6BE4(UnkStruct_ov96_021F5018 *param_1, s32 param_2, u8 param_3, s32 param_4) {
    s32 i;
    s32 *row;

    for (i = 0; i < 0x80; i++) {
        (param_1->unk_1A0 + 1)[param_2][i] = 0;
    }
    row = (param_1->unk_1A0 + 1)[param_2];
    row[param_3] = param_4;
    ov96_021F6B50(param_3, row[param_3], row);
}

void ov96_021EB2BC(enum HeapID *param_1, NarcId param_2, s32 param_3, s32 param_4, s32 param_5);

void ov96_021EB2F4(enum HeapID *param_1, NarcId param_2, s32 param_3, s32 param_4, s32 param_5, u8 param_6);

void ov96_021EB334(enum HeapID *param_1, NarcId param_2, s32 param_3, s32 param_4);

void ov96_021EB36C(enum HeapID *param_1, NarcId param_2, s32 param_3, s32 param_4);

void ov96_021F6C18(enum HeapID *param_1);

void ov96_021F6C18(enum HeapID *param_1) {
    ov96_021EB2BC(param_1, NARC_a_1_6_8, 3, 0x65, 3);
    ov96_021EB2F4(param_1, NARC_a_1_6_8, 0, 0x65, 3, 4);
    ov96_021EB334(param_1, NARC_a_1_6_8, 2, 0x65);
    ov96_021EB36C(param_1, NARC_a_1_6_8, 1, 0x65);
    return;
}

void ov96_021F6C5C(UnkStruct_ov96_021F5018 *arg0, enum HeapID *arg1) {
    VecFx32 vec;
    s32 i;
    void *set;

    ov96_021F74D0(arg0->unk_8C, arg1, 2);
    set = ov96_021EB3E4(arg1, 1, 3, 0x65, 3);
    arg0->unk_64 = set;
    ov96_021EB564(set, 0xC);
    ov96_021EB52C(arg0->unk_64, 1, 1);
    for (i = 0; i < 4; i++) {
        set = ov96_021EB3E4(arg1, 1, 3, 0x65, 4);
        arg0->unk_68[i] = set;
        ov96_021EB564(set, i + 0xD);
        ov96_021EB52C(arg0->unk_68[i], 1, 1);
    }
#if defined(SDK_PORT) || defined(NONMATCHING)
    // The DS loops below run 4 times over 3-entry arrays: the 4th ball set lands in unk_13C (set to the countdown
    // later) and the 4th unk_78 set in unk_84[0] (overwritten by the digit sprites below), so both are dropped.
    for (i = 0; i < 4; i++) {
        set = ov96_021EB3E4(arg1, 1, 3, 0x65, 7);
        if (i < 3) {
            arg0->unk_90[i].unk_04 = set;
        }
        ov96_021EB564(set, 0xB);
        ov96_021EB630(set, 9);
    }
    for (i = 0; i < 4; i++) {
        set = ov96_021EB3E4(arg1, 1, 3, 0x65, 8);
        if (i < 3) {
            arg0->unk_78[i] = set;
        }
        ov96_021EB564(set, 9);
        ov96_021EB630(set, 6);
    }
#else
    for (i = 0; i < 4; i++) {
        arg0->unk_90[i].unk_04 = ov96_021EB3E4(arg1, 1, 3, 0x65, 7);
        ov96_021EB564(arg0->unk_90[i].unk_04, 0xB);
        ov96_021EB630(arg0->unk_90[i].unk_04, 9);
    }
    for (i = 0; i < 4; i++) {
        set = ov96_021EB3E4(arg1, 1, 3, 0x65, 8);
        arg0->unk_78[i] = set;
        ov96_021EB564(set, 9);
        ov96_021EB630(arg0->unk_78[i], 6);
    }
#endif
    for (i = 0; i < 2; i++) {
        arg0->unk_84[i] = ov96_021EA634(arg0->unk_188, ov96_021EB5E8(arg1), 1, arg0->heapID);
        Sprite_SetDrawFlag(arg0->unk_84[i], 1);
        vec.x = ov96_0221C0B0[i] << 0xC;
        vec.y = 0x8000;
        vec.z = 0;
        Sprite_SetMatrix(arg0->unk_84[i], &vec);
        Sprite_SetAnimCtrlSeq(arg0->unk_84[i], ov96_0221C0B4[i]);
    }
}
