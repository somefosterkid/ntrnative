#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_91_unk_struct_ov115_02261_adc.h"
#include "constants/heap.h"
#include "pm_string.h"
#include "unk_02013534.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_41_sprite_resource_collection.h"

struct BgConfig;

struct G3DPipelineBuffers;

void SetBgPriority(u8 bgLayer, u8 priority);

void BgCommitTilemapBufferToVram(BgConfig *bgConfig, u8 bgLayer);

void BgTilemapRectChangePalette(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, u8 width, u8 height, u8 palette);

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

void ov91_022614FC(UnkStruct_ov115_02261ADC *param0, s16 param1, u16 param2);

void ov91_022614FC(UnkStruct_ov115_02261ADC *param0, s16 param1, u16 param2) {
    int v0;
    s32 v1;

    for (v0 = 0; v0 < 3; v0++) {
        v1 = param1 - v0;

        if (v1 < 0) {
            v1 += 3;
        }

        v1 += 4;

        SetBgPriority(v1, 2 - v0);

        if (v0 == 0) {
            BgTilemapRectChangePalette(param0->unk_00, v1, 0, 0, 32, 32, 2 + (param2 * 2));
        } else {
            BgTilemapRectChangePalette(param0->unk_00, v1, 0, 0, 32, 32, 1 + (param2 * 2));
        }

        BgCommitTilemapBufferToVram(param0->unk_00, v1);
    }
}
