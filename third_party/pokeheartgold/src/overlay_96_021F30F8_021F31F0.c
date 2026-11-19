#include "global.h"

#include "error_handling.h"
#include "sprite.h"

s32 ov96_021EB52C(void *, s32, s32);

Sprite *ov96_021EB5B8(void *);

extern s32 ov96_0221DBC8;

void ov96_021F31F0(void *arg0, u8 arg1);

void ov96_021F31F0(void *arg0, u8 arg1) {
    s32 i;
    VecFx32 sp4;
    Sprite *var_r6;
    s32 temp_r0;
    void *var_r4;
    void *var_r5;
    var_r5 = arg0;
    i = 0;
    var_r4 = (void *)(*((void **)((&ov96_0221DBC8) + arg1)));
    do {
        if ((*((s32 *)var_r4)) != 0) {
            temp_r0 = (*((s32 *)(((u8 *)var_r5) + 0x80))) * 0x10;
            var_r6 = var_r5 + temp_r0;
            if ((*((s32 *)(var_r5 + temp_r0))) == 1) {
                GF_AssertFail();
            }
            var_r6->matrix.x = 1;
            var_r6->matrix.z = arg1;
            var_r6->matrix.y = (s32)var_r4;
            var_r6 = ov96_021EB5B8(*((void **)(((u8 *)(var_r5 + ((*((s32 *)(((u8 *)var_r5) + 0x80))) * 0x10))) + 0xC)));
            ov96_021EB52C(*((void **)(((u8 *)(var_r5 + ((*((s32 *)(((u8 *)var_r5) + 0x80))) * 0x10))) + 0xC)), 1, 1);
            Sprite_SetAnimCtrlSeq(var_r6, (*((s32 *)var_r4)) - 1);
            sp4.z = 0;
            sp4.x = (*((u16 *)(((u8 *)var_r4) + 4))) << 0xC;
            sp4.y = (*((u16 *)(((u8 *)var_r4) + 6))) << 0xC;
            Sprite_SetMatrix(var_r6, &sp4);
            *((s32 *)(((u8 *)var_r5) + 0x80)) = ((*((s32 *)(((u8 *)var_r5) + 0x80))) + 1) % 8;
        }
        i += 1;
        var_r4 += 8;
    } while (i < 4);
}
