#include "global.h"

#include "sprite.h"
#include "bg_window.h"
#include "overlay_90_unk_struct_ov114_0225dac0.h"
#include "overlay_90_unk_struct_ov114_0225cbf0.h"

struct BgConfig;

struct Sprite;

struct Window;


typedef struct {
    u8 unk_00[4];
    u8 unk_04[4];
    u8 unk_08[4];
} UnkStruct_ov114_02260204;

extern const s16 ov90_0225C274[4][2];

extern const UnkStruct_ov114_02260204 Unk_ov114_02260204[4];

void ov90_02259FC8(UnkStruct_ov114_0225DAC0 *param0, s32 param1);

extern const UnkStruct_ov114_02260204 ov90_0225C31C[4];

void ov90_02259FC8(UnkStruct_ov114_0225DAC0 *param0, s32 param1) {
    u8 v0;
    u8 v1, v2;
    VecFx32 v3;

    v0 = ov90_0225C31C[param0->unk_18 - 1].unk_08[param0->unk_1B];
    v1 = ov90_0225C274[v0][0];
    v2 = (ov90_0225C31C[param0->unk_18 - 1].unk_00[param0->unk_1B] * 8) + ov90_0225C274[v0][1];

    v3.x = (v1 + param1) << FX32_SHIFT;
    v3.y = v2 << FX32_SHIFT;

    Sprite_SetMatrix(param0->unk_10, &v3);

    v1 = 24;
    v2 = (ov90_0225C31C[param0->unk_18 - 1].unk_00[param0->unk_1B] * 8) + 21;

    v3.x = (v1 + param1) << FX32_SHIFT;
    v3.y = v2 << FX32_SHIFT;

    Sprite_SetMatrix(param0->unk_14, &v3);
}
