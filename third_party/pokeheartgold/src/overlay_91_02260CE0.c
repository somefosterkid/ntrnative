#include "global.h"

#include "sprite.h"

typedef struct {
    BOOL unk_00;
    Sprite *unk_04;
} UnkStruct_ov115_02264BA0;

void ov91_02260CE0(UnkStruct_ov115_02264BA0 *param0, s32 param1, s32 param2);

void ov91_02260D00(void *arg0);

void ov91_02260CE0(UnkStruct_ov115_02264BA0 *param0, s32 param1, s32 param2) {
    VecFx32 v0;

    v0.x = param1 << FX32_SHIFT;
    v0.y = (param2 << FX32_SHIFT) + (512 << FX32_SHIFT);

    Sprite_SetMatrix(param0->unk_04, &v0);
}

void ov91_02260D00(void *arg0) {
    Sprite_SetDrawFlag(*(Sprite **)((u8 *)arg0 + 4), 0);
    *(s32 *)arg0 = 0;
}
