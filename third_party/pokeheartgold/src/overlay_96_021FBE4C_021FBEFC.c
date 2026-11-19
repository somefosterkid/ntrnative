#include "global.h"

#include "sprite.h"

Sprite *ov96_021EB5B8(void *);

s32 ov96_021FBE54(void *, s32);

void ov96_021FBEFC(void *arg0, s32 arg1);

void ov96_021FBEFC(void *arg0, s32 arg1) {
    VecFx32 sp4;
    Sprite *sp0;
    Sprite *temp_r6;
    VecFx32 *temp_r0;

    if (*(s32 *)arg0 != 0) {
        temp_r6 = ov96_021EB5B8(*(void **)((u8 *)arg0 + 4));
        sp0 = ov96_021EB5B8(*(void **)((u8 *)arg0 + 8));
        temp_r0 = Sprite_GetMatrixPtr(temp_r6);
        sp4 = *(VecFx32 *)((u8 *)temp_r0);
        sp4.y += arg1 << 0xC;
        Sprite_SetMatrix(temp_r6, &sp4);
        Sprite_SetMatrix(sp0, &sp4);
        if (sp4.y >= 0x208000) {
            ov96_021FBE54(arg0, 0);
        }
    }
}
