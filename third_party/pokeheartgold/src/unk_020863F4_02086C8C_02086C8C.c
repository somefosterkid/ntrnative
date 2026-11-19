#include "global.h"

#include "constants/heap.h"

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
#include "unk_020863F4.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "unk_02013534.h"
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
    UnkStruct_02013534 *unk_8C;
    TextOBJ *unk_90[2];
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

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);

static inline void inline_ov61_0222C3B0_sub(SysTask *, void *);

s32 sub_02086D98(s32 arg0, s32 arg1);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);
static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);
void sub_02086DE4(UnkStruct_02089688 *param0, int param1);
void sub_02086F44(UnkStruct_02089688 *param0);
void sub_02086FCC(UnkStruct_02089688 *param_1);
void sub_02086FE8(UnkStruct_02089688 *param_1);
u8 sub_02087028(UnkStruct_02089688 *param_1);
void sub_02087064(UnkStruct_02089688 *param_1);
void sub_02087090(UnkStruct_02089688 *param_1, s32 param_2, u32 param_3, u32 param_4, s32 param_5);

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

void sub_02086C80(TextOBJ *param0, int param1, int param2);

void sub_02086C8C(UnkStruct_02089688 *param0);

void sub_02086C8C(UnkStruct_02089688 *param0) {
    int v0;
    s16 v1, v2;

    for (v0 = 0; v0 < 2; v0++) {
        v1 = (param0->unk_2EC.unk_18[v0 + 26].rect.left + param0->unk_2EC.unk_18[v0 + 26].rect.right) / 2;
        v1 -= 40;
        v2 = (param0->unk_2EC.unk_18[v0 + 26].rect.top + param0->unk_2EC.unk_18[v0 + 26].rect.bottom) / 2;
        v2 -= 7;

        switch (param0->unk_268[v0].unk_00) {
        case 0:
            param0->unk_268[v0].unk_14.unk_04 = 0;
            break;
        case 1:
            param0->unk_268[v0].unk_14.unk_04++;

            if (param0->unk_268[v0].unk_14.unk_04 == 1) {
                ManagedSprite_SetAnim(param0->unk_268[v0].unk_0C, 1);
                sub_02086C80(param0->unk_2EC.unk_90[v0], v1, v2 - 0);
            } else if (param0->unk_268[v0].unk_14.unk_04 == 2) {
                ManagedSprite_SetAnim(param0->unk_268[v0].unk_0C, 2);
                sub_02086C80(param0->unk_2EC.unk_90[v0], v1, v2 - 1);
            } else if (param0->unk_268[v0].unk_14.unk_04 == 10) {
                ManagedSprite_SetAnim(param0->unk_268[v0].unk_0C, 0);
                sub_02086C80(param0->unk_2EC.unk_90[v0], v1, v2 + 0);
                param0->unk_268[v0].unk_00++;
            }
            break;
        default:
            ManagedSprite_SetAnim(param0->unk_268[v0].unk_0C, 0);
            sub_02086C80(param0->unk_2EC.unk_90[v0], v1, v2);
            param0->unk_268[v0].unk_00 = 0;
            break;
        }
    }
}

s32 sub_02086D98(s32 arg0, s32 arg1) {
    s32 v0;

    v0 = 0;
    if (arg1 == 0) {
        v0 = 0xB;
    }
    return v0 + arg0;
}

void sub_02086DA4(void *arg0) {
    s32 v0;
    void *v1;

    v0 = 0;
    if ((s32) * (s32 *)((u8 *)arg0 + 0x2D0) > 0) {
        v1 = arg0;
        do {
            if ((v0 >= (s32) * (s32 *)((u8 *)arg0 + 0x2DC)) && (v0 < (s32) * (s32 *)((u8 *)arg0 + 0x2E0))) {
                *(s32 *)((u8 *)v1 + 8) = 1;
            } else {
                *(s32 *)((u8 *)v1 + 8) = 0;
            }
            v0 += 1;
            v1 += 0x1C;
        } while (v0 < (s32) * (s32 *)((u8 *)arg0 + 0x2D0));
    }
}

void sub_02086DE4(UnkStruct_02089688 *param0, int param1) {
    int v0;
    int v1;
    s16 v2;
    s16 v3, v4;

    v2 = param0->unk_2A0[param0->unk_2D4];
    v1 = 0;

    for (v0 = 0; v0 < param0->unk_2D0; v0++) {
        if ((v0 >= param0->unk_2DC) && (v0 < param0->unk_2E0)) {
            if (v0 == param0->unk_2DC) {
                v2 += ((32 + 8) / 2);
            } else {
                v2 += 32;
            }
        } else {
            if (v0 == 0) {
                v2 += ((32 + 8) / 2);
            } else {
                v2 += 8;
            }
        }

        ManagedSprite_GetPositionXY(param0->unk_00[v0].unk_0C, &v3, &v4);

        if (param1 == 0) {
            ManagedSprite_SetPositionXY(param0->unk_00[v0].unk_0C, v2, v4);
        } else {
            param0->unk_00[v0].unk_14.unk_00 = (v2 - v3) / 2;
            param0->unk_00[v0].unk_14.unk_02 = 0;
            param0->unk_00[v0].unk_14.unk_04 = 2;
            param0->unk_00[v0].unk_14.unk_05 = 0;
        }

        if ((v0 == param0->unk_1C0[v1].unk_00) && (v1 != param0->unk_3EC)) {
            ManagedSprite_GetPositionXY(param0->unk_1C0[v1].unk_0C, &v3, &v4);

            if (param0->unk_2DC == param0->unk_2E0) {
                v2 += 8;
            } else {
                if ((v0 > param0->unk_2DC) && (v0 < param0->unk_2E0)) {
                    v2 += ((32 + 8) / 2);
                } else {
                    v2 += 8;
                }
            }

            if (param1 == 0) {
                ManagedSprite_SetPositionXY(param0->unk_1C0[v1].unk_0C, v2, v4);
            } else {
                param0->unk_1C0[v1].unk_14.unk_00 = (v2 - v3) / 2;
                param0->unk_1C0[v1].unk_14.unk_02 = 0;
                param0->unk_1C0[v1].unk_14.unk_04 = 2;
            }

            v1++;
        }
    }
}

static inline void inline_0208AF44(UnkStruct_02089688 *param0, int param1, s16 param2, s16 param3) {
    s16 v0, v1;
    UnkStruct_0208AF44 *v2 = &param0->unk_00[param1];

    ManagedSprite_GetPositionXY(v2->unk_0C, &v0, &v1);

    v2->unk_10->rect.top = v1 - param3;
    v2->unk_10->rect.left = v0 - param2;
    v2->unk_10->rect.bottom = v1 + param3;
    v2->unk_10->rect.right = v0 + param2;
}

void sub_02086F44(UnkStruct_02089688 *param0) {
    int v0;
    s16 v1;
    s16 v2;

    for (v0 = 0; v0 < param0->unk_2D0; v0++) {
        if ((v0 >= param0->unk_2DC) && (v0 < param0->unk_2E0)) {
            v1 = 32 / 2;
            v2 = 32 / 2;
        } else {
            v1 = 8 / 2;
            v2 = 8;
        }

        inline_0208AF44(param0, v0, v1, v2);
    }
}

void sub_02086FCC(UnkStruct_02089688 *param_1) {
    UnkStruct_02013534 *pUVar1;

    pUVar1 = FontSystem_NewInit(2, HEAP_ID_108);
    param_1->unk_2EC.unk_8C = pUVar1;
    FontID_Alloc(FONT_SUBSCREEN, HEAP_ID_108);
    return;
}

void sub_02086FE8(UnkStruct_02089688 *param_1) {
    FontID_Release(FONT_SUBSCREEN);
    FontOAM_Delete(param_1->unk_2EC.unk_90[0]);
    sub_02021B5C(&param_1->unk_2EC.unk_98[0]);
    FontOAM_Delete(param_1->unk_2EC.unk_90[1]);
    sub_02021B5C(&param_1->unk_2EC.unk_98[1]);
    sub_020135AC(param_1->unk_2EC.unk_8C);
    return;
}

u8 sub_02087028(UnkStruct_02089688 *param_1) {
    u8 uVar1;

    uVar1 = SpriteSystem_LoadPaletteBuffer(param_1->unk_2EC.unk_10, PLTTBUF_MAIN_OBJ, param_1->unk_2EC.unk_04, param_1->unk_2EC.unk_08, NARC_graphic_font, 8, 0, 1, 1, 0x3eb);
    return uVar1;
}

void sub_02087064(UnkStruct_02089688 *param_1) {
    sub_02087028(param_1);
    sub_02087090(param_1, 0, 0x4e, 0xa5, 0);
    sub_02087090(param_1, 1, 0xac, 0xa5, 0);
    return;
}
