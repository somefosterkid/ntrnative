#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "sys_task.h"
#include "system.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "screen_fade_internal.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

typedef struct System System;

struct OverlayManager;

struct OverlayManagerTemplate;

struct Camera;

struct CharTransferAllocation;

struct Field3DModelAnimation;

struct Field3dModel;

struct Field3dObject;

struct TextOBJ;

struct GF3DVramMan;

struct ManagedSprite;

struct MsgData;

struct NARC;

struct PaletteData;

struct ParticleSystem;

struct SaveData;

struct SpriteManager;

struct SpriteSystem;

struct String;

struct MessageFormat;

struct SysTask;

struct SysTaskQueue;

struct System;

struct PlayerProfile;

struct UnkStruct_02012744_t;

struct UnkStruct_ov114_02025CAD4_t;

struct UnkStruct_ov117_02261280_t;

struct UnkStruct_ov117_022626B0_t;

struct UnkStruct_ov66_0222DFF8_t;

struct Window;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

BOOL ov93_0225FEC4(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_02264E84 *param1);

void ov93_02260F14(UnkStruct_ov117_02261280_t *param0, int param1, s32 param2, UnkStruct_ov117_02264E84 *param3);

BOOL ov93_02260F3C(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02264E84 *param1);

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

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

BOOL PlaySE(u16 seqID);

typedef void (*UnkCallback)(void);

enum ButtonMode {
    BUTTON_MODE_NORMAL = 0,
    BUTTON_MODE_START_IS_X = 1,
    BUTTON_MODE_SWAP_XY = 2,
    BUTTON_MODE_L_IS_A = 3,
};

typedef struct {
    fx32 unk_00;
    fx32 unk_04;
} UnkStruct_ov117_022626B0_sub1_sub1;

typedef struct {
    UnkStruct_ov117_022626B0_sub1_sub1 unk_00;
    UnkStruct_ov117_022626B0_sub1_sub1 unk_08;
    UnkStruct_ov117_022626B0_sub1_sub1 unk_10;
    UnkStruct_ov117_022626B0_sub1_sub1 unk_18;
} UnkStruct_ov117_022626B0_sub1;

typedef struct {
    UnkStruct_ov117_022626B0_sub1_sub1 unk_00;
    UnkStruct_ov117_022626B0_sub1_sub1 unk_08;
    UnkStruct_ov117_022626B0_sub1_sub1 unk_10;
    UnkStruct_ov117_022626B0_sub1_sub1 unk_18;
} UnkStruct_ov117_022626B0_sub2;

struct UnkStruct_ov117_022626B0_t {
    NNSG3dResFileHeader *unk_00;
    NNSG3dResTex *unk_04;
    u16 unk_08;
    UnkStruct_ov117_022626B0_sub1 unk_0C[8];
    UnkStruct_ov117_022626B0_sub2 unk_10C[8];
    int unk_20C;
    int unk_210;
    int unk_214;
    int unk_218;
    int unk_21C;
    int unk_220;
    int unk_224;
    int unk_228;
    int unk_22C;
    fx32 unk_230;
    ManagedSprite *unk_234;
    int unk_238;
    int unk_23C;
    int unk_240;
    int unk_244;
    fx32 unk_248;
    u16 *unk_24C;
    u16 *unk_250;
    u32 unk_254;
    u32 unk_258;
    s32 unk_25C;
    fx32 unk_260;
    fx32 unk_264;
    s16 unk_268;
    s16 unk_26A;
    s16 unk_26C;
    int unk_270;
    u8 unk_274;
    u8 unk_275;
    u8 unk_276;
};

void ov93_0225EB70(UnkStruct_ov117_022626B0 *param0);

BOOL ov93_0225F370(UnkStruct_ov117_022626B0 *param0);

int ov93_0225F44C(UnkStruct_ov117_022626B0 *param0);

void ov93_0225EB38(UnkStruct_ov117_022626B0 *param0);

int ov93_0225EA6C(UnkStruct_ov117_022626B0 *param0, int param1);

void ov93_0225EA98(UnkStruct_ov117_022626B0 *param0, int param1);

BOOL ov93_0225EDB8(UnkStruct_ov117_022626B0 *param0, int param1);

void ov93_0225EDE8(UnkStruct_ov117_022626B0 *param0);

void ov93_0225EDFC(UnkStruct_ov117_022626B0 *param0);

void ov93_0225EAE0(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022626B0 *param1, int param2, int param3);

void ov93_0225FB6C(UnkStruct_ov117_022626B0 *param0, ManagedSprite *param1);

void ov93_0225E898(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022626B0 *param1);

void ov93_0225E898(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022626B0 *param1) {
    param1->unk_244 = 0;

    if (param0->unk_20 == 2) {
        param1->unk_20C = param0->unk_174C.unk_0C;
        param1->unk_210 = param0->unk_174C.unk_10;
        param1->unk_214 = param0->unk_174C.unk_14;
        param1->unk_218 = param0->unk_174C.unk_18;
    } else if ((param0->unk_2FB8 == 0) || (param0->unk_2FBC == 1)) {
        param1->unk_20C = 0;
        param1->unk_210 = 0;
        param1->unk_214 = 0;
        param1->unk_218 = 0;
    } else {
        param1->unk_20C = gSystem.touchX;
        param1->unk_210 = gSystem.touchY;
        param1->unk_214 = gSystem.touchHeld;
        param1->unk_218 = gSystem.touchNew;
    }

    ov93_0225F370(param1);

    if (param1->unk_214 == 0) {
        ov93_0225EB38(param1);
        if ((param1->unk_0C[0].unk_10.unk_04 == 0) && (param1->unk_0C[0].unk_10.unk_04 == 0)) {
            param1->unk_274 = 0;
        }
    }

    ov93_0225F44C(param1);
    ov93_0225EB70(param1);
    ov93_0225FB6C(param1, param1->unk_234);

    if (param1->unk_268 > 0) {
        param1->unk_268 += 8;

        if (param1->unk_268 > 30) {
            param1->unk_268 = 30;
        }

        ov93_0225EDFC(param1);
    }

    if ((param0->unk_2FBC == 0) && (param0->unk_2FB8 == 1)) {
        if (param1->unk_244 > 0) {
            UnkStruct_ov117_02264E84 v0;
            int v1 = 0, v2 = 0;
            s32 v3;

            if (param1->unk_25C < 800) {
                v1 = ov93_0225EDB8(param1, param1->unk_244);

                if (v1 == 1) {
                    param1->unk_268 = 8;
                    param1->unk_275 = 0;
                    PlaySE(1422);
                }
            } else {
                ov93_0225EDE8(param1);
                param1->unk_268 = 0;
                v2 = 1;
            }

            v3 = ov93_0225EA6C(param1, param1->unk_244);

            if (v2 == 1) {
                v3 += v3 * 25 / 100;
            }

            ov93_02260F14(param0, param0->unk_2FC8, v3, &v0);
            ov93_02260F3C(param0, &v0);
            ov93_0225FEC4(param0, &v0);
            ov93_0225EA98(param1, param1->unk_244);
            ov93_0225EAE0(param0, param1, param1->unk_244, v2);
            param1->unk_244 = 0;
        }
    }
}
