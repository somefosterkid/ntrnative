#include "global.h"

#include "constants/heap.h"

#include "camera.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "palette.h"
#include "sys_task.h"
#include "bg_window.h"
#include "pm_string.h"
#include "pokepic.h"
#include "overlay_71_trade_send_phase.h"
#include "overlay_71_trade_receive_phase.h"
#include "overlay_71_trade3dmodel.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

typedef struct UnkStruct_ov71_022478C8_Heap_Alloc {
    void *unk_0;
    s32 unk_4;
    u8 unk_8[0x4];
    PokepicManager *unk_C;
    s32 unk_10;
    u8 unk_14[0x40];
    s32 unk_54;
    u8 unk_58[0x10];
    String *unk_68;
    String *unk_6C;
    u8 unk_70[0x8];
    s32 unk_78;
    s32 unk_7C;
    NARC *unk_80;
} UnkStruct_ov71_022478C8_Heap_Alloc;

struct Trade3DModel;

typedef struct Trade3DScene {
    Camera *camera;
    VecFx32 target;
    CameraAngle cameraAngle;
    Trade3DModel *models;
    u32 modelCount;
} Trade3DScene;

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

void ov71_022478B8(Camera **arg0);
s32 ov71_02247384(void *);
void *ov71_022478C8(void *arg0);
s32 ov71_02247498(Trade3DScene *);
s32 ov71_0224809C(void *);
s32 ov71_02248120(TradeReceivePhase *arg0);
s32 ov71_022481EC(void *);
void ov71_02247924(void *arg0);

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

void ov71_0224784C(Trade3DScene *tradeScene, fx32 targetX, fx32 targetY, fx32 targetZ);

void ov71_0224784C(Trade3DScene *tradeScene, fx32 targetX, fx32 targetY, fx32 targetZ) {
    VecFx32 upVector;

    tradeScene->camera = Camera_New((enum HeapID)57);
    tradeScene->target.x = targetX;
    tradeScene->target.y = targetY;
    tradeScene->target.z = targetZ;
    tradeScene->cameraAngle.x = ((0 * 0xffff) / 360);
    tradeScene->cameraAngle.y = ((0 * 0xffff) / 360);
    tradeScene->cameraAngle.z = ((0 * 0xffff) / 360);

    Camera_Init_FromTargetDistanceAndAngle(&(tradeScene->target), 1228800, &(tradeScene->cameraAngle), 4004, 0, 1, tradeScene->camera);

    upVector.x = 0;
    upVector.y = FX32_ONE;
    upVector.z = 0;

    Camera_SetLookAtCamUp(&upVector, tradeScene->camera);
    Camera_SetStaticPtr(tradeScene->camera);
    Camera_SetPerspectiveClippingPlane((0 << FX32_SHIFT), (1000 << FX32_SHIFT), tradeScene->camera);
}

void ov71_022478B8(Camera **arg0) {
    Camera_UnsetStaticPtr();
    Camera_Delete(*arg0);
}

void *ov71_022478C8(void *arg0) {
    UnkStruct_ov71_022478C8_Heap_Alloc *v0;

    v0 = Heap_Alloc(HEAP_ID_57, sizeof(UnkStruct_ov71_022478C8_Heap_Alloc));
    if (v0 != NULL) {
        v0->unk_0 = arg0;
        v0->unk_4 = 0;
        v0->unk_54 = ov71_02247384(arg0);
        v0->unk_C = PokepicManager_Create(HEAP_ID_57);
        v0->unk_10 = 0;
        v0->unk_68 = String_New(0x12CU, HEAP_ID_57);
        v0->unk_6C = String_New(0x12CU, HEAP_ID_57);
        v0->unk_78 = 0;
        v0->unk_7C = 0;
        v0->unk_80 = NARC_New(NARC_a_1_8_0, HEAP_ID_57);
    }
    return v0;
}

void ov71_02247924(void *ptr) {
    Pokepic *pokepic;

    if (ptr != NULL) {
        ov71_02248120(ptr);
        ov71_022481EC(ptr);
        ov71_0224809C(ptr);
        String_Delete(((TradeSendPhase *)ptr)->string0);
        String_Delete(((TradeSendPhase *)ptr)->string1);
        ov71_02247498(*((void **)&((TradeSendPhase *)ptr)->scene));
        FreeBgTilemapBuffer(((TradeSendPhase *)ptr)->bgConfig, 1U);
        FreeBgTilemapBuffer(((TradeSendPhase *)ptr)->bgConfig, 2U);
        FreeBgTilemapBuffer(((TradeSendPhase *)ptr)->bgConfig, 6U);
        RemoveWindow(ptr + 0x58);
        NARC_Delete(((TradeSendPhase *)ptr)->pokeDataNarc);
        pokepic = ((Pokepic *)((TradeSendPhase *)ptr)->pokemonSprite);
        if (pokepic != NULL) {
            Pokepic_Delete(pokepic);
        }
        PokepicManager_Delete(((PokepicManager *)((TradeSendPhase *)ptr)->spriteManager));
        Heap_Free(ptr);
    }
}
