#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "obj_char_transfer.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "touchscreen.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "touch_hitbox_controller.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct TextOBJ;
struct TouchHitboxController;
struct UnkStruct_02012744_t;
typedef struct {
    int unk_00;
    int unk_04[5];
    int unk_18;
    String *unk_1C;
    Options *options;
    int unk_24;
    u32 unk_28;
    u32 unk_2C;
    u32 unk_30;
} UnkStruct_02089438;
typedef struct {
    NARC *unk_00;
    SpriteSystem *unk_04;
    SpriteManager *unk_08;
    BgConfig *unk_0C;
    PaletteData *unk_10;
    TouchHitboxController *unk_14;
    TouchscreenHitbox unk_18[28];
    BOOL unk_88;
    UnkStruct_02012744 *unk_8C;
    FontOAM *unk_90[2];
    UnkStruct_02021AC8 unk_98[2];
    Window unk_B0;
} UnkStruct_02089688_sub1;
typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
} UnkStruct_02089688_sub2;
typedef struct {
    s16 unk_00;
    s16 unk_02;
    u8 unk_04;
    u8 unk_05;
} UnkStruct_0208AF44_sub1;
typedef struct {
    int unk_00;
    int unk_04;
    BOOL unk_08;
    ManagedSprite *unk_0C;
    TouchscreenHitbox *unk_10;
    UnkStruct_0208AF44_sub1 unk_14;
} UnkStruct_0208AF44;
typedef struct {
    UnkStruct_0208AF44 unk_00[16];
    UnkStruct_0208AF44 unk_1C0[3];
    UnkStruct_0208AF44 unk_214[3];
    UnkStruct_0208AF44 unk_268[2];
    s16 unk_2A0[5];
    u16 unk_2AA[5][2];
    u8 pad_2BE[2];
    int unk_2C0;
    int unk_2C4;
    int unk_2C8;
    int unk_2CC;
    int unk_2D0;
    int unk_2D4;
    int unk_2D8;
    int unk_2DC;
    int unk_2E0;
    int unk_2E4;
    int unk_2E8;
    UnkStruct_02089688_sub1 unk_2EC;
    UnkStruct_02089688_sub2 unk_3AC;
    UnkStruct_02089438 unk_3B8;
    int unk_3EC;
    int unk_3F0;
} UnkStruct_02089688;
void sub_02086FE8(UnkStruct_02089688 *param0);
SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);
static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);
static inline void inline_ov61_0222C3B0_sub(SysTask *, void *);

struct DotArtistGraphics;
typedef struct DotArtistGraphics DotArtistGraphics;
void sub_020136B4(void *ptr);
void sub_02086C80(DotArtistGraphics *graphics);

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
enum AffineOverwriteMode {
    AFFINE_OVERWRITE_MODE_NONE = 0,
    AFFINE_OVERWRITE_MODE_NORMAL,
    AFFINE_OVERWRITE_MODE_DOUBLE,
};
int sub_02086D98(int param0, BOOL param1);
void sub_02086AB4(UnkStruct_02089688 *param0, int param1, BOOL param2);
void sub_02086AE4(UnkStruct_02089688 *param0, int param1);
void sub_02086B2C(UnkStruct_02089688 *param0, int param1);

void sub_020866CC(UnkStruct_02089688 *param0);
void sub_02086758(UnkStruct_02089688 *param0);
void sub_020868A0(UnkStruct_02089688 *param0);
void sub_020869BC(UnkStruct_02089688 *param0);
void sub_02086AB4(UnkStruct_02089688 *param0, int param1, BOOL param2);
void sub_02086AE4(UnkStruct_02089688 *param0, int param1);
void sub_02086B2C(UnkStruct_02089688 *param0, int param1);
void sub_02086B6C(UnkStruct_02089688 *param0, int param1, int param2);
void sub_02086BB4(UnkStruct_02089688 *param0);

void sub_020866CC(UnkStruct_02089688 *param0) {
    int v0;

    for (v0 = 0; v0 < param0->unk_2D0; v0++) {
        Sprite_DeleteAndFreeResources(param0->unk_00[v0].unk_0C);
    }

    for (v0 = 0; v0 < param0->unk_3EC; v0++) {
        Sprite_DeleteAndFreeResources(param0->unk_1C0[v0].unk_0C);
    }

    for (v0 = 0; v0 < 2; v0++) {
        Sprite_DeleteAndFreeResources(param0->unk_268[v0].unk_0C);
    }

    for (v0 = 0; v0 < 3; v0++) {
        Sprite_DeleteAndFreeResources(param0->unk_214[v0].unk_0C);
    }

    sub_02086FE8(param0);
    RemoveWindow(&param0->unk_2EC.unk_B0);
}

void sub_02086758(UnkStruct_02089688 *param0) {
    int i;
    int v0 = 0, v1 = 0;

    ManagedSpriteTemplate v2;
    SpriteSystem *v3 = param0->unk_2EC.unk_04;
    SpriteManager *v4 = param0->unk_2EC.unk_08;

    v2.x = 0;
    v2.y = 0;
    v2.z = 0;
    v2.animation = 0;
    v2.drawPriority = 10;
    v2.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    v2.bgPriority = 0;
    v2.vramTransfer = 0;
    v2.pal = 0;
    v2.resIdList[0] = 1000;
    v2.resIdList[1] = 1000;
    v2.resIdList[2] = 1000;
    v2.resIdList[3] = 1000;
    v2.resIdList[4] = -1;
    v2.resIdList[5] = -1;

    u32 v5 = param0->unk_3B8.unk_28;
    for (i = param0->unk_3F0 - 1; i >= 0; i--) {
        param0->unk_00[i].unk_00 = (v5 % 10) + 1;
        v5 /= 10;
    }

    for (i = 0; i < param0->unk_2D0 + param0->unk_3EC; i++) {
        if (param0->unk_3EC != 0 && i == param0->unk_1C0[v0].unk_00 + v0 + 1) {
            param0->unk_1C0[v0].unk_0C = SpriteSystem_NewSprite(v3, v4, &v2);
            ManagedSprite_SetPositionXY(param0->unk_1C0[v0].unk_0C, 76 + i * 8, 24);
            ManagedSprite_SetAnim(param0->unk_1C0[v0].unk_0C, 22);
            ManagedSprite_TickFrame(param0->unk_1C0[v0].unk_0C);
            v0++;
        } else {
            param0->unk_00[v1].unk_0C = SpriteSystem_NewSprite(v3, v4, &v2);
            ManagedSprite_SetPositionXY(param0->unk_00[v1].unk_0C, 76 + i * 8, 24);
            ManagedSprite_SetAnim(param0->unk_00[v1].unk_0C, sub_02086D98(param0->unk_00[v1].unk_00, param0->unk_00[v1].unk_08));
            ManagedSprite_SetAffineOverwriteMode(param0->unk_00[v1].unk_0C, AFFINE_OVERWRITE_MODE_DOUBLE);
            ManagedSprite_TickFrame(param0->unk_00[v1].unk_0C);
            v1++;
        }
    }
}

void sub_020868A0(UnkStruct_02089688 *param0) {
    ManagedSpriteTemplate v0;
    SpriteSystem *v1 = param0->unk_2EC.unk_04;
    SpriteManager *v2 = param0->unk_2EC.unk_08;
    PaletteData *v3 = param0->unk_2EC.unk_10;

    v0.x = 0;
    v0.y = 0;
    v0.z = 0;
    v0.animation = 0;
    v0.drawPriority = 0;
    v0.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    v0.bgPriority = 0;
    v0.vramTransfer = 0;
    v0.pal = 0;
    v0.resIdList[0] = 1001;
    v0.resIdList[1] = 1001;
    v0.resIdList[2] = 1001;
    v0.resIdList[3] = 1001;
    v0.resIdList[4] = -1;
    v0.resIdList[5] = -1;

    param0->unk_214[0].unk_0C = SpriteSystem_NewSprite(v1, v2, &v0);
    param0->unk_214[1].unk_0C = SpriteSystem_NewSprite(v1, v2, &v0);
    param0->unk_214[2].unk_0C = SpriteSystem_NewSprite(v1, v2, &v0);

    sub_02086AE4(param0, param0->unk_3F0);
    ManagedSprite_SetAnim(param0->unk_214[0].unk_0C, 0);
    ManagedSprite_TickFrame(param0->unk_214[0].unk_0C);

    param0->unk_214[1].unk_14.unk_00 = 0;
    param0->unk_214[1].unk_14.unk_02 = 0;
    param0->unk_214[1].unk_00 = 1;

    sub_02086B2C(param0, 0);
    ManagedSprite_SetAnim(param0->unk_214[1].unk_0C, param0->unk_214[1].unk_00);
    ManagedSprite_TickFrame(param0->unk_214[1].unk_0C);
    ManagedSprite_SetOamMode(param0->unk_214[1].unk_0C, GX_OAM_MODE_XLU);

    param0->unk_214[2].unk_14.unk_00 = 0;
    param0->unk_214[2].unk_14.unk_02 = 0;
    param0->unk_214[2].unk_00 = 1;

    sub_02086B2C(param0, 0);
    ManagedSprite_SetAnim(param0->unk_214[2].unk_0C, param0->unk_214[2].unk_00);
    ManagedSprite_TickFrame(param0->unk_214[2].unk_0C);
    ManagedSprite_SetOamMode(param0->unk_214[2].unk_0C, GX_OAM_MODE_XLU);
    sub_02086AB4(param0, 1, 0);
    sub_02086AB4(param0, 2, 0);
}

void sub_020869BC(UnkStruct_02089688 *param0) {
    ManagedSpriteTemplate v0;
    SpriteSystem *v1 = param0->unk_2EC.unk_04;
    SpriteManager *v2 = param0->unk_2EC.unk_08;
    PaletteData *v3 = param0->unk_2EC.unk_10;

    v0.x = 0;
    v0.y = 0;
    v0.z = 0;
    v0.animation = 0;
    v0.drawPriority = 10;
    v0.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    v0.bgPriority = 0;
    v0.vramTransfer = 0;
    v0.pal = 0;
    v0.resIdList[0] = 1002;
    v0.resIdList[1] = 1002;
    v0.resIdList[2] = 1002;
    v0.resIdList[3] = 1002;
    v0.resIdList[4] = -1;
    v0.resIdList[5] = -1;
    v0.pal = 0;
    param0->unk_268[0].unk_0C = SpriteSystem_NewSprite(v1, v2, &v0);
    v0.pal = 1;
    param0->unk_268[1].unk_0C = SpriteSystem_NewSprite(v1, v2, &v0);

    {
        s16 v4, v5;

        v4 = (param0->unk_2EC.unk_18[26].rect.left + param0->unk_2EC.unk_18[26].rect.right) / 2;
        v5 = (param0->unk_2EC.unk_18[26].rect.top + param0->unk_2EC.unk_18[26].rect.bottom) / 2;

        ManagedSprite_SetPositionXY(param0->unk_268[0].unk_0C, v4, v5);
        ManagedSprite_SetAnim(param0->unk_268[0].unk_0C, 0);
        ManagedSprite_TickFrame(param0->unk_268[0].unk_0C);

        v4 = (param0->unk_2EC.unk_18[27].rect.left + param0->unk_2EC.unk_18[27].rect.right) / 2;
        v5 = (param0->unk_2EC.unk_18[27].rect.top + param0->unk_2EC.unk_18[27].rect.bottom) / 2;

        ManagedSprite_SetPositionXY(param0->unk_268[1].unk_0C, v4, v5);
        ManagedSprite_SetAnim(param0->unk_268[1].unk_0C, 0);
        ManagedSprite_TickFrame(param0->unk_268[1].unk_0C);
    }
}

void sub_02086AB4(UnkStruct_02089688 *param0, int param1, BOOL param2) {
    if (param2 == 1) {
        ManagedSprite_SetDrawFlag(param0->unk_214[param1].unk_0C, 1);
    } else {
        ManagedSprite_SetDrawFlag(param0->unk_214[param1].unk_0C, 0);
    }
}

void sub_02086AE4(UnkStruct_02089688 *param0, int param1) {
    s16 v0, v1;
    ManagedSprite *v2;

    if (param1 < param0->unk_3F0) {
        return;
    }

    v2 = param0->unk_00[param1].unk_0C;
    param0->unk_214[0].unk_00 = param1;

    ManagedSprite_GetPositionXY(v2, &v0, &v1);
    ManagedSprite_SetPositionXY(param0->unk_214[0].unk_0C, v0, v1 + 16);
}

void sub_02086B2C(UnkStruct_02089688 *param0, int param1) {
    s16 v0 = (param0->unk_2EC.unk_18[param1 + 16].rect.left + param0->unk_2EC.unk_18[param1 + 16].rect.right) / 2;
    s16 v1 = (param0->unk_2EC.unk_18[param1 + 16].rect.top + param0->unk_2EC.unk_18[param1 + 16].rect.bottom) / 2;

    ManagedSprite_SetPositionXY(param0->unk_214[1].unk_0C, v0, v1);
}

void sub_02086B6C(UnkStruct_02089688 *param0, int param1, int param2) {
    s16 v0 = (param0->unk_2EC.unk_18[param1 + 16].rect.left + param0->unk_2EC.unk_18[param1 + 16].rect.right) / 2;
    s16 v1 = (param0->unk_2EC.unk_18[param1 + 16].rect.top + param0->unk_2EC.unk_18[param1 + 16].rect.bottom) / 2;

    ManagedSprite_SetPositionXY(param0->unk_214[param2].unk_0C, v0, v1);
}

void sub_02086BB4(UnkStruct_02089688 *param0) {
    ManagedSprite_TickFrame(param0->unk_214[0].unk_0C);
    ManagedSprite_TickFrame(param0->unk_214[1].unk_0C);
    ManagedSprite_TickFrame(param0->unk_214[2].unk_0C);

    {
        int v0 = 2;
        int v1;
        BOOL v2;

        for (v0 = 1; v0 < 3; v0++) {
            v1 = ManagedSprite_GetActiveAnim(param0->unk_214[v0].unk_0C);

            if (v1 == 3) {
                v2 = ManagedSprite_IsAnimated(param0->unk_214[v0].unk_0C);

                if (v2 == 0) {
                    ManagedSprite_SetAnim(param0->unk_214[v0].unk_0C, param0->unk_214[v0].unk_00);

                    if (param0->unk_2EC.unk_88 == 1) {
                        sub_02086AB4(param0, 1, 0);
                    } else {
                        sub_02086AB4(param0, 1, 1);
                    }

                    sub_02086AB4(param0, 2, 0);
                }
            } else {
                if (v1 != param0->unk_214[v0].unk_00) {
                    ManagedSprite_SetAnim(param0->unk_214[v0].unk_0C, param0->unk_214[v0].unk_00);
                }

                {
                    v1 = ManagedSprite_GetActiveAnim(param0->unk_214[2].unk_0C);

                    if (v1 != 3) {
                        if (param0->unk_2EC.unk_88 == 1) {
                            sub_02086AB4(param0, 1, 0);
                        } else {
                            sub_02086AB4(param0, 1, 1);
                        }
                    }
                }
            }
        }
    }
}

void sub_02086C80(DotArtistGraphics *graphics) {
    if (graphics != 0L) {
        sub_020136B4(graphics);
    }
}
