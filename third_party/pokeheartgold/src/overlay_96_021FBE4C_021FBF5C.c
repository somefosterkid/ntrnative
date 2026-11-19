#include "global.h"

#include "sprite.h"

Sprite *ov96_021EB5B8(void *);

void ov96_021FBF5C(void *arg0, void *arg1);

void ov96_021FBF5C(void *arg0, void *arg1) {
    VecFx32 *vecFx32;
    s32 v0;
    s32 v1;

    vecFx32 = Sprite_GetMatrixPtr(ov96_021EB5B8(*(void **)((u8 *)arg0 + 4)));
    *(s16 *)((u8 *)arg1 + 4) = 0x40;
    *(s16 *)((u8 *)arg1 + 6) = 0x10;
    v1 = vecFx32->x;
    *(s16 *)arg1 = (s16)(((s32)(v1 + ((u32)(v1 >> 0xB) >> 0x14)) >> 0xC) - 0x20);
    v0 = vecFx32->y;
    *(s16 *)((u8 *)arg1 + 2) = (s16)(((s32)(v0 + ((u32)(v0 >> 0xB) >> 0x14)) >> 0xC) - 0x10);
}
