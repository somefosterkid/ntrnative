#include "global.h"
#include "sprite_resources_header.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "heap.h"
#include "obj_char_transfer.h"
#include "pm_string.h"
#include "unk_0200A090.h"
#include "overlay_91_unk_struct_ov115_02261_adc.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_41_sprite_resource_collection.h"
#include "gf_gfx_planes.h"

struct G3DPipelineBuffers;
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
void ov91_0225FA60(UnkStruct_ov115_02261ADC *param0);

void ov91_0225FA60(UnkStruct_ov115_02261ADC *param0) {
    FreeBgTilemapBuffer(param0->unk_00, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(param0->unk_00, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(param0->unk_00, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(param0->unk_00, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(param0->unk_00, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(param0->unk_00, GF_BG_LYR_SUB_3);
    Heap_Free(param0->unk_00);
}
