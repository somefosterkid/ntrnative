#include "global.h"

#include "heap.h"
#include "overlay_manager.h"
#include "pokemon.h"
#include "unk_020755E8.h"
#include "overlay_70_gtsapplication_state.h"

typedef struct UnkStruct_ov70_02241648_temp_r3_2 {
    u8 unk_0[0x10];
    Pokedex *unk_10;
    u8 unk_14[0x10];
    Options *unk_24;
    GameStats *unk_28;
    Bag *unk_2C;
    u8 unk_30[0x8];
    u32 unk_38;
} UnkStruct_ov70_02241648_temp_r3_2;

typedef struct UnkStruct_ov70_02241648_temp_r3 {
    u8 unk_0[0x10];
    Pokedex *unk_10;
    u8 unk_14[0x10];
    Options *unk_24;
    GameStats *unk_28;
    Bag *unk_2C;
    u8 unk_30[0x8];
    u32 unk_38;
} UnkStruct_ov70_02241648_temp_r3;

s32 ov70_02238E50(void *, s32, s32);

Pokemon *ov70_02241868(void *, s32);

s32 ov70_022418A4(void *);

s32 sub_0202DB64(void *, Pokemon *);

s32 ov70_02241648(void *arg0);

s32 ov70_02241648(void *arg0) {
    s32 temp_r1;
    s32 sp28;
    s32 sp24;
    Pokemon *temp_r0_2;
    Pokemon *temp_r0_4;
    Pokemon *temp_r6;
    s32 temp_r0;
    s32 var_r4;
    u16 temp_r0_3;
    u16 temp_r0_5;
    UnkStruct_ov70_02241648_temp_r3 *temp_r3;
    UnkStruct_ov70_02241648_temp_r3_2 *temp_r3_2;

    temp_r0 = *((s32 *)&((GTSApplicationState *)arg0)->currentScreenInstruction);
    var_r4 = 3;
    switch (temp_r0) {
    case 0:
        if (OverlayManager_Run(((OverlayManager *)((GTSApplicationState *)arg0)->appMan)) != 0) {
            OverlayManager_Delete(((OverlayManager *)((GTSApplicationState *)arg0)->appMan));

            temp_r1 = *((s32 *)&((GTSApplicationState *)arg0)->screenArgument);
            if (temp_r1 == 9) {
                temp_r0_2 = ov70_02241868(arg0, temp_r1);
                temp_r0_3 = GetMonEvolution(NULL, temp_r0_2, 1U, (u16)GetMonData(temp_r0_2, 6, NULL), &sp28);
                if (temp_r0_3 != 0) {
                    temp_r3 = *(void **)arg0;
                    ((GTSApplicationState *)arg0)->evolutionTask = sub_02075A7C(NULL, temp_r0_2, (s32)temp_r0_3, temp_r3->unk_24, temp_r3->unk_38, temp_r3->unk_10, temp_r3->unk_2C, temp_r3->unk_28, (u32)sp28, 4U, HEAP_ID_61);
                    *((s32 *)&((GTSApplicationState *)arg0)->currentScreenInstruction) = 1;
                } else {
                    ov70_02238E50(arg0, 1, 0);
                    var_r4 = 4;
                }
            } else if (temp_r1 == 8 || temp_r1 == 10) {
                temp_r6 = ov70_02241868(arg0, temp_r1);
                temp_r0_4 = AllocMonZeroed(HEAP_ID_61);
                sub_0202DB64(*(void **)(*(void **)arg0), temp_r0_4);
                if (GetMonData(temp_r6, 5, NULL) != GetMonData(temp_r0_4, 5, NULL) || GetMonData(temp_r6, 0, NULL) != GetMonData(temp_r0_4, 0, NULL)) {
                    temp_r0_5 = GetMonEvolution(NULL, temp_r6, 1U, (u16)GetMonData(temp_r6, 6, NULL), &sp24);
                    if (temp_r0_5 != 0) {
                        temp_r3_2 = *(void **)arg0;
                        ((GTSApplicationState *)arg0)->evolutionTask = sub_02075A7C(NULL, temp_r6, (s32)temp_r0_5, temp_r3_2->unk_24, temp_r3_2->unk_38, temp_r3_2->unk_10, temp_r3_2->unk_2C, temp_r3_2->unk_28, (u32)sp24, 4U, HEAP_ID_61);
                        *((s32 *)&((GTSApplicationState *)arg0)->currentScreenInstruction) = 1;
                    } else {
                        ov70_02238E50(arg0, 1, 0);
                        var_r4 = 4;
                    }
                } else {
                    ov70_02238E50(arg0, 1, 0);
                    var_r4 = 4;
                }
                Heap_Free(temp_r0_4);
            } else {
                ov70_02238E50(arg0, 1, 0);
                var_r4 = 4;
            }
        }
        break;
    case 1:
        if (sub_02075D3C(((GTSApplicationState *)arg0)->evolutionTask) != 0) {
            sub_02075D4C(((GTSApplicationState *)arg0)->evolutionTask);
            ov70_022418A4(arg0);
            *(s32 *)REG_DISPCNT_ADDR &= 0xFFFF1FFF;
            ov70_02238E50(arg0, 7, 12);
            var_r4 = 4;
        }
        break;
    }
    return var_r4;
}
