#include "global.h"

#include "field/map_load_manager.h"
#include "field/map_prop.h"

#include "error_handling.h"
#include "heap.h"
#include "map_matrix.h"

s32 ov01_021F652C(MapLoadManager *, s32);

s32 sub_02054A9C(MapProp *, void *, s32 *);

s32 sub_02054DC8(s32, const int, s32 *);

s32 *sub_02054D10(void *arg0, enum HeapID arg1, s32 arg2, void *arg3, u32 arg4);

s32 *sub_02054D10(void *arg0, enum HeapID arg1, s32 arg2, void *arg3, u32 arg4) {
    s32 sp14[3];
    MapPropManager *sp10;
    s32 spC;
    s32 *sp8;
    void *sp4;
    void *sp0;
    s32 var_r0;
    MapProp *temp_r0;
    s32 *var_r1;
    s32 temp_r0_2;
    s32 temp_r5;
    u32 temp_r2;
    u32 var_r4;
    u8 temp_r0_3;
    u8 var_r5;
    sp0 = arg0;
    sp4 = arg3;
    sp8 = Heap_AllocAtEnd(arg1, arg2 * 4);
    var_r0 = 0;
    if (arg2 > 0) {
        var_r1 = sp8;
        do {
            var_r0 += 1;
            *var_r1 = arg4;
            var_r1 += 1;
        } while (var_r0 < arg2);
    }
    var_r4 = 0;
    spC = 0;
loop_4:
    ov01_021F630C(spC, ((FieldSystem *)sp0)->mapLoadManager, &sp10);

    if (sp10 != 0L) {
        temp_r5 = ov01_021F652C(((FieldSystem *)sp0)->mapLoadManager, spC);
        sub_02054DC8(temp_r5, MapMatrix_GetWidth(((FieldSystem *)sp0)->mapMatrix), sp14);
        var_r5 = 0;
loop_6:
        temp_r0 = MapPropManager_GetMapPropByIndex(sp10, var_r5);

        if (sub_02054A9C(temp_r0, sp4, sp14) != 0) {
            temp_r0_2 = MapProp_GetBuildModel(temp_r0);
            if (temp_r0_2 != 0) {
                if (var_r4 >= ((u32)arg2)) {
                    GF_AssertFail();
                    return sp8;
                }
                temp_r2 = var_r4;
                var_r4 = (u32)((u8)(var_r4 + 1));
                sp8[temp_r2] = temp_r0_2;
                goto block_11;
            }
        }
block_11:
        var_r5 += 1;

        if (((u32)var_r5) >= 0x20U) {
            goto block_12;
        }
        goto loop_6;
    }
block_12:
    temp_r0_3 = spC + 1;

    spC = (s32)temp_r0_3;
    if (((u32)temp_r0_3) >= 4U) {
        return sp8;
    }
    goto loop_4;
}
