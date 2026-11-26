#include "global.h"
#include "sprite_resources_header.h"

#include "system.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "list_menu.h"
#include "options.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_02005D10.h"
#include "unk_0200A090.h"
#include "overlay_43_unk_struct_ov64_0222_e21_c.h"
#include "overlay_43_unk_struct_ov64_0222_e060.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_43_unk_struct_ov64_02230904.h"
#include "overlay_43_palpad_fns.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct MsgData;
struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);
static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);
static inline void inline_ov61_0222C3B0_sub(SysTask *, void *);
typedef struct {
    s16 unk_00;
    s16 unk_02;
    u8 unk_04;
    u8 unk_05;
} UnkStruct_ov64_0223221C;
typedef struct {
    void *unk_00[2];
    NNSG2dScreenData *unk_08[2];
} UnkStruct_ov64_022300E4;
typedef struct {
    String *unk_00;
    String *unk_04;
} UnkStruct_ov64_0222E060_sub1;
typedef struct {
    const u16 *unk_00;
    BOOL unk_04;
} UnkStruct_ov64_0222E060_sub2;
typedef struct {
    u32 unk_00;
    u8 unk_04[32];
} UnkStruct_ov64_022301B4;
typedef struct {
    void *unk_00[7];
    NNSG2dScreenData *unk_1C[7];
} UnkStruct_ov64_02230F60;
typedef struct {
    UnkStruct_ov64_0223221C unk_00;
    u32 unk_08;
    Window *unk_0C[7];
    u32 unk_28[7];
} UnkStruct_ov64_02230F98;
void ov43_0222AD20(PalPadGfx *param0, BOOL param1);
void ov43_0222D4C4(PalPadDetails *param0, PalPadShared *param1, PalPadGfx *param2, u32 param3);

int ov43_0222D47C(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx) {
    u32 v0;
    u32 ret;
    v0 = 0;
    if (!(0xC0 & gSystem.heldKeys)) {
        details->friendChangeHeld = 0;
    }
    ret = ov43_0222D3B8(details, shared, gfx, &v0);
    if (v0 != 0) {
        shared->inputState = 1;
        return ret;
    }
    return ov43_0222D2FC(details, shared, gfx);
}

static inline void inline_ov61_0222C3B0(UnkStruct_ov61_0222C3B0 *param0, NARC *param1, int param2, enum HeapID heapID) {
    NNSG2dPaletteData *v0;
    void *v1;

    MI_CpuClear8(param0, sizeof(UnkStruct_ov61_0222C3B0));

    v1 = GfGfxLoader_GetPlttDataFromOpenNarc(param1, param2, &v0, heapID);

    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_08, 4 * 0x20);
    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_88, 4 * 0x20);
    Heap_Free(v1);
    inline_ov61_0222C3B0_sub_1(param0);

    param0->unk_04 = 1;
    param0->unk_00 = SysTask_ExecuteOnVBlank(inline_ov61_0222C3B0_sub, param0, 20);
}

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0) {
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1);
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1) {
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}

void ov43_0222D4C4(PalPadDetails *param0, PalPadShared *param1, PalPadGfx *param2, u32 param3) {
    u32 v0;

    param0->scrollStep = 0;

    ov43_0222AD20(param2, 0);

    if (param0->direction == 2) {
        v0 = 2;
    } else {
        v0 = 1;
    }

    ov43_0222DAE8(param0, param1, param2, v0, param0->page, param3);
    PlaySE(1505);
}
