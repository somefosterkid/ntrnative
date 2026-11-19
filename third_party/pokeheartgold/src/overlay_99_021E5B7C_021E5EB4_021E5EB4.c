#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "sprite_system.h"
#include "overlay_99_pokeathlon_course_record.h"

extern s32 ov99_021E95D8;

extern s32 ov99_021E95DC;

extern s32 ov99_021E9690;

void ov99_021E5EB4(void *arg0, s32 arg1, s32 arg2);

void ov99_021E5EB4(void *arg0, s32 arg1, s32 arg2) {
    s32 temp_r4;
    s32 var_r6;
    u8 var_r4;
    if (arg2 != 0) {
        var_r6 = -0x100;
    } else {
        var_r6 = 0x100;
    }
    GfGfx_EngineATogglePlanes(4U, 0U);
    GfGfx_EngineBTogglePlanes(2U, 0U);
    temp_r4 = arg1 * 8;
    GfGfxLoader_LoadScrnDataFromOpenNarc(((PokeathlonCourseRecord *)arg0)->narc, *((s32 *)(((u8 *)&ov99_021E95D8) + temp_r4)), ((PokeathlonCourseRecord *)arg0)->bgConfig, GF_BG_LYR_SUB_1_F, 0U, 0U, 1, *((enum HeapID *)&((PokeathlonCourseRecord *)arg0)->heapId));
    GfGfxLoader_LoadScrnDataFromOpenNarc(((PokeathlonCourseRecord *)arg0)->narc, *((s32 *)(((u8 *)&ov99_021E95DC) + temp_r4)), ((PokeathlonCourseRecord *)arg0)->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 1, *((enum HeapID *)&((PokeathlonCourseRecord *)arg0)->heapId));
    BgSetPosTextAndCommit(((PokeathlonCourseRecord *)arg0)->bgConfig, 2U, BG_POS_OP_SET_ROT, var_r6);
    BgSetPosTextAndCommit(((PokeathlonCourseRecord *)arg0)->bgConfig, 5U, BG_POS_OP_SET_ROT, var_r6);
    GfGfx_EngineATogglePlanes(4U, 1U);
    GfGfx_EngineBTogglePlanes(2U, 1U);
    var_r4 = 0;
    do {
        temp_r4 = var_r4;
        ManagedSprite_OffsetPositionXY(((PokeathlonCourseRecord *)arg0)->sprites[(*((s32 *)((&ov99_021E9690) + temp_r4 * 3)))], (s16)(0 - var_r6), 0);
        var_r4 += 1;
    } while (((u32)var_r4) < 6U);
}
