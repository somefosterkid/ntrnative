#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "pm_string.h"
#include "overlay_90_unk_struct_ov114_0225cef0.h"
#include "overlay_90_unk_struct_ov114_02260284.h"
#include "overlay_90_unk_struct_ov114_0225cbf0.h"

typedef struct {
    Window unk_00;
    u32 unk_10;
    String *unk_14;
    UnkStruct_ov114_0225CBF0 unk_18;
    s16 unk_2C;
    s16 unk_2E;
} UnkStruct_ov114_0225E1A4;

BOOL ov90_02258D4C(UnkStruct_ov114_0225CBF0 *param0, s32 param1);

void ov90_0225A5BC(UnkStruct_ov114_0225E1A4 *param0, UnkStruct_ov114_0225CEF0 *param1, s32 param2);

void ov90_0225A60C(UnkStruct_ov114_0225E1A4 *param0, UnkStruct_ov114_0225CEF0 *param1, s32 param2);

BOOL ov90_0225A544(UnkStruct_ov114_0225E1A4 *param0, UnkStruct_ov114_0225CEF0 *param1);

BOOL ov90_0225A544(UnkStruct_ov114_0225E1A4 *param0, UnkStruct_ov114_0225CEF0 *param1) {
    BOOL v0;

    switch (param0->unk_2E) {
    case 0:
        v0 = ov90_02258D4C(&param0->unk_18, param0->unk_2C);
        ov90_0225A60C(param0, param1, param0->unk_18.unk_00 >> 12);
        param0->unk_2C++;

        if (v0 == 1) {
            param0->unk_2E++;
        }
        break;
    case 1:
        ClearWindowTilemapAndCopyToVram(&param0->unk_00);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, 0);

        ov90_0225A5BC(param0, param1, 0);

        BgClearTilemapBufferAndCommit(param1->unk_00, GF_BG_LYR_MAIN_0);
        BgClearTilemapBufferAndCommit(param1->unk_00, GF_BG_LYR_MAIN_1);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, 1);
        return 1;
    }

    return 0;
}
