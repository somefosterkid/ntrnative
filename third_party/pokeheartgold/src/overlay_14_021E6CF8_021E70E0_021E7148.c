#include "global.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F3190(void *, u32, s32);
void ov14_021E7148(struct UnkStruct_ov14_boxapp *arg0, void *arg1);

void ov14_021E7148(UnkStruct_ov14_boxapp *arg0, void *arg1) {
    u8 sp0[5];
    u32 temp_r1;
    u32 var_r2;
    u32 var_r7;
    u8 *var_r6_2;
    void *var_r3;
    void *var_r4;
    u8 *var_r6;
    var_r4 = arg1;
    var_r2 = 0;
    var_r3 = var_r4;
    var_r6 = sp0;
    do {
        if ((*((s32 *)(((u8 *)var_r3) + 0xC))) != 0) {
            *var_r6 = arg0->sys->unk_4094[*((s32 *)(((u8 *)var_r3) + 4))];
        }
        var_r2 += 1;
        var_r3 += 0x20;
        var_r6 += 1;
    } while (var_r2 < 7U);
    var_r7 = 0;
    var_r6_2 = sp0;
    do {
        if ((*((s32 *)(((u8 *)var_r4) + 0xC))) != 0) {
            temp_r1 = *((u32 *)(((u8 *)var_r4) + 8));
            if (!(0x80 & temp_r1)) {
                arg0->sys->unk_4094[temp_r1] = (u8)(*var_r6_2);
                ov14_021F3190(arg0->sys, *((u32 *)(((u8 *)var_r4) + 8)), 1);
            } else {
                if (((u32)(*((u32 *)(((u8 *)var_r4) + 4)))) >= 0x1EU) {
                    arg0->sys->unk_4094[0x23] = (u8)(*var_r6_2);
                }
                ov14_021F3190(arg0->sys, *((u32 *)(((u8 *)var_r4) + 4)), 1);
            }
        }
        var_r7 += 1;
        var_r4 += 0x20;
        var_r6_2 += 1;
    } while (var_r7 < 7U);
}
