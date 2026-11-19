#include "global.h"

#include "battle/battler_sprite_context.h"
#include "sprite_resources_header.h"
#include "overlay_07_generic_emitter.h"
#include "overlay_07_battle_anim_system.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"
#include "bg_window.h"
#include "palette.h"
#include "pokepic.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "sprite_system.h"
#include "camera.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"
#include "library/spl_emitter.h"
#include "library/spl_texture.h"
#include "library/spl_manager.h"
#include "particle_system.h"
#include "overlay_07_bg_scroll_context.h"
#include "sav_chatot.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "overlay_07_battle_anim_context.h"
#include "overlay_07_battle_bg_anim.h"
#include "overlay_07_battle_anim_tracking.h"

struct BattleAnimContext;

struct BattleAnimScriptLoop;

struct BattleAnimSystem;

struct BattleBackgroundReference;

struct BattleBgAnim;

struct BgConfig;

struct BgTrackingTask;

struct BgWaveStrip;

struct CameraPerspective;

struct CameraPositionHistory;

struct SOUND_CHATOT;

struct CustomBgScrollContext;

struct GenericEmitterCallbackData;

struct ManagedSprite;

struct NARC;

struct SelectedPaletteData;

struct ParticleSystem;

struct Pokepic;

struct PokemonSpriteDrawContext;

struct PokepicShadow;

struct PokemonSpriteTrackingTask;

struct PokepicDrawParam;

struct SPLAlphaAnim;

struct SPLBehavior;

struct SPLChildResource;

struct SPLColorAnim;

struct SPLEmitter;

struct SPLEmitterState;

struct SPLParticle;

struct SPLResourceHeader;

struct SPLScaleAnim;

struct SPLTexAnim;

struct SpriteAnimFrame;

struct SpriteManager;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct SpriteTrackingTask;

struct SysTask;

struct SysTaskQueue;

struct WaveAnimContext;

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

typedef struct BattleBackgroundReference {
    int narcID;
    int tilesNarcMemberIdx;
    int paletteNarcMemberIdx;
    int tilemapNarcMemberIdx;
    int paletteDestStart;
    int paletteSrcSize;
} BattleBackgroundReference;

typedef struct CameraPositionHistory {
    int historySize;
    int currentHistoryIndex;
    int nextHistoryIndex;
    int delay;
    BOOL delayReached;
    BOOL delayX;
    BOOL delayY;
    BOOL delayZ;
    VecFx32 *positions;
} CameraPositionHistory;

typedef struct SPLBehavior {
    void (*applyFunc)(const void *, struct SPLParticle *, VecFx32 *, struct SPLEmitter *);
    const void *object;
} SPLBehavior;

typedef union SPLResourceFlags {
    u32 all;
    struct {
        u32 emissionType : 4;
        u32 drawType : 2;
        u32 circleAxis : 2;
        u32 hasScaleAnim : 1;
        u32 hasColorAnim : 1;
        u32 hasAlphaAnim : 1;
        u32 hasTexAnim : 1;
        u32 hasRotation : 1;
        u32 randomInitAngle : 1;

        u32 selfMaintaining : 1;
        u32 followEmitter : 1;
        u32 hasChildResource : 1;
        u32 polygonRotAxis : 2;
        u32 polygonReferencePlane : 1;
        u32 randomizeLoopedAnim : 1;
        u32 drawChildrenFirst : 1;
        u32 hideParent : 1;
        u32 useViewSpace : 1;
        u32 hasGravityBehavior : 1;
        u32 hasRandomBehavior : 1;
        u32 hasMagnetBehavior : 1;
        u32 hasSpinBehavior : 1;
        u32 hasCollisionPlaneBehavior : 1;
        u32 hasConvergenceBehavior : 1;
        u32 hasFixedPolygonID : 1;
        u32 childHasFixedPolygonID : 1;
    };
} SPLResourceFlags;

typedef union SPLChildResourceFlags {
    u16 all;
    struct {
        u16 usesBehaviors : 1;
        u16 hasScaleAnim : 1;
        u16 hasAlphaAnim : 1;
        u16 rotationType : 2;
        u16 followEmitter : 1;
        u16 useChildColor : 1;
        u16 drawType : 2;
        u16 polygonRotAxis : 2;
        u16 polygonReferencePlane : 1;
        u16 : 4;
    };
} SPLChildResourceFlags;

typedef union SPLCurveInOut {
    u16 all;
    struct {
        u16 in : 8;
        u16 out : 8;
    };
} SPLCurveInOut;

typedef union SPLCurveInPeakOut {
    u32 all;
    struct {
        u32 in : 8;
        u32 peak : 8;
        u32 out : 8;
        u32 : 8;
    };
} SPLCurveInPeakOut;

typedef struct SPLResourceHeader {
    SPLResourceFlags flags;
    VecFx32 emitterBasePos;
    fx32 emissionCount;
    fx32 radius;
    fx32 length;
    VecFx16 axis;
    GXRgb color;
    fx32 initVelPosAmplifier;
    fx32 initVelAxisAmplifier;
    fx32 baseScale;
    fx16 aspectRatio;
    u16 startDelay;
    s16 minRotation;
    s16 maxRotation;
    u16 initAngle;
    u16 reserved;
    u16 emitterLifeTime;
    u16 particleLifeTime;

    struct {
        u32 baseScale : 8;
        u32 lifeTime : 8;
        u32 initVel : 8;
        u32 : 8;
    } randomAttenuation;

    struct {
        u32 emissionInterval : 8;
        u32 baseAlpha : 8;
        u32 airResistance : 8;
        u32 textureIndex : 8;
        u32 loopFrames : 8;
        u32 dbbScale : 16;
        u32 textureTileCountS : 2;
        u32 textureTileCountT : 2;
        u32 scaleAnimDir : 3;
        u32 dpolFaceEmitter : 1;
        u32 flipTextureS : 1;
        u32 flipTextureT : 1;
        u32 : 30;
    } misc;
    fx16 polygonX;
    fx16 polygonY;
    struct {
        u32 flags : 8;
        u32 : 24;
    } userData;
} SPLResourceHeader;

typedef struct SPLScaleAnim {
    fx16 start;
    fx16 mid;
    fx16 end;
    SPLCurveInOut curve;
    struct {
        u16 loop : 1;
        u16 : 15;
    } flags;
    u16 padding;
} SPLScaleAnim;

typedef struct SPLColorAnim {
    GXRgb start;
    GXRgb end;
    SPLCurveInPeakOut curve;
    struct {
        u16 randomStartColor : 1;
        u16 loop : 1;
        u16 interpolate : 1;
        u16 : 13;
    } flags;
    u16 padding;
} SPLColorAnim;

typedef struct SPLAlphaAnim {
    union {
        u16 all;
        struct {
            u16 start : 5;
            u16 mid : 5;
            u16 end : 5;
            u16 : 1;
        };
    } alpha;
    struct {
        u16 randomRange : 8;
        u16 loop : 1;
        u16 : 7;
    } flags;
    SPLCurveInOut curve;
    u16 padding;
} SPLAlphaAnim;

typedef struct SPLTexAnim {
    u8 textures[8];
    struct {
        u32 frameCount : 8;
        u32 step : 8;
        u32 randomizeInit : 1;
        u32 loop : 1;
        u32 : 14;
    } param;
} SPLTexAnim;

typedef struct SPLChildResource {
    SPLChildResourceFlags flags;
    fx16 randomInitVelMag;
    fx16 endScale;
    u16 lifeTime;
    u8 velocityRatio;
    u8 scaleRatio;
    GXRgb color;
    struct {
        u32 emissionCount : 8;
        u32 emissionDelay : 8;
        u32 emissionInterval : 8;
        u32 texture : 8;
        u32 textureTileCountS : 2;
        u32 textureTileCountT : 2;
        u32 flipTextureS : 1;
        u32 flipTextureT : 1;
        u32 dpolFaceEmitter : 1;
        u32 : 25;
    } misc;
} SPLChildResource;

typedef struct SPLEmitterState {
    union {
        u32 all;
        struct
        {
            u32 terminate : 1;
            u32 emissionPaused : 1;
            u32 paused : 1;
            u32 renderingDisabled : 1;
            u32 started : 1;
            u32 : 27;
        };
    };
} SPLEmitterState;

typedef struct BattleAnimScriptLoop {
    u32 *startPos;
    u8 curIteration;
    u8 maxIterations;
    BOOL isActive;
} BattleAnimScriptLoop;

typedef struct PokemonSpriteDrawContext {
    struct BattleAnimSystem *battleAnimSys;
    SpriteManager *spriteManager;
    ManagedSprite *sprite;
    BOOL active;
} PokemonSpriteDrawContext;

BOOL ov07_0221BFC0(BattleAnimSystem *system);

extern BOOL ov07_0221F9A8(BattleAnimSystem *system, int params[], int count);

s8 ov07_02221664(GenericEmitterCallbackData *data);

u8 ov07_02231924(BattleAnimSystem *system, int battler);

void ov07_02231B90(BattleAnimSystem *system, int battler, VecFx32 *pos);

void ov07_02231BC0(BattleAnimSystem *system, int battler, VecFx32 *pos);

void ov07_02231BF0(BattleAnimSystem *system, int battler, VecFx32 *pos);

void ov07_02231CB0(BattleAnimSystem *system, int battler, VecFx32 *pos);

void ov07_02231CE0(BattleAnimSystem *system, int battler, VecFx32 *pos);

void ov07_02231D10(BattleAnimSystem *system, int battler, VecFx32 *pos);

void ov07_02231D40(BattleAnimSystem *system, int battler, VecFx32 *pos);

void ov07_02231DA0(BattleAnimSystem *system, int battler, VecFx32 *pos);

void Heap_Free(void *ptr);

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

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

void ov07_022216A8(GenericEmitterCallbackData *data, int battler, VecFx32 *pos);

void ov07_02221734(GenericEmitterCallbackData *data, VecFx32 *pos);

void ov07_022208F8(SPLEmitter *emitter, GenericEmitterCallbackData *data, VecFx32 *pos);

void ov07_02220938(SPLEmitter *emitter, GenericEmitterCallbackData *data, VecFx32 *pos);

void ov07_02231C20(void *ctx, void *param1, void *param2);

void ov07_02231C50(void *arg0, s32 arg1, void *arg2);

void ov07_02231C80(void *arg0, s32 arg1, void *arg2);

void ov07_02220978(SPLEmitter *emitter, GenericEmitterCallbackData *data);

typedef struct {
    int a[6][2];
} ov07_022354E0_Array;

extern const ov07_022354E0_Array ov07_022354E0;

typedef struct {
    int a[6 + 1][2];
} ov07_02235578_Array;

extern const ov07_02235578_Array ov07_02235578;

typedef struct {
    int a[6 + 1][2];
} ov07_02235540_Array;

extern const ov07_02235540_Array ov07_02235540;

void ov07_02220978(SPLEmitter *emitter, GenericEmitterCallbackData *data) {
    VecFx32 pos;

    switch (data->params[GENERIC_EMITTER_CALLBACK_PARAM_POSITION]) {
    case 0:
        break;
    case 1:
        ov07_02231B90(data->battleAnimSys, data->startBattler, &pos);
        break;
    case 2:
        ov07_02231B90(data->battleAnimSys, data->endBattler, &pos);
        break;
    case 6:
        ov07_02231BC0(data->battleAnimSys, data->startBattler, &pos);
        break;
    case 7:
        ov07_02231BC0(data->battleAnimSys, data->endBattler, &pos);
        break;
    case 10:
        ov07_02231BF0(data->battleAnimSys, data->startBattler, &pos);
        break;
    case 11:
        ov07_02231BF0(data->battleAnimSys, data->endBattler, &pos);
        break;
    case 14:
        ov07_02231C20(data->battleAnimSys, (void *)(data->startBattler), &pos);
        break;
    case 15:
        ov07_02231C20(data->battleAnimSys, (void *)(data->endBattler), &pos);
        break;
    case 16:
        ov07_02231C50((data->battleAnimSys), data->startBattler, (u8 *)(&pos));
        break;
    case 17:
        ov07_02231C50((data->battleAnimSys), data->endBattler, (u8 *)(&pos));
        break;
    case 18:
        ov07_02231C80((data->battleAnimSys), data->startBattler, (u8 *)(&pos));
        break;
    case 19:
        ov07_02231C80((data->battleAnimSys), data->endBattler, (u8 *)(&pos));
        break;
    case 20:
        ov07_02231CB0(data->battleAnimSys, data->startBattler, &pos);
        break;
    case 21:
        ov07_02231CB0(data->battleAnimSys, data->endBattler, &pos);
        break;
    case 22:
        ov07_02231CE0(data->battleAnimSys, data->startBattler, &pos);
        break;
    case 23:
        ov07_02231CE0(data->battleAnimSys, data->endBattler, &pos);
        break;
    case 24:
        ov07_02231D10(data->battleAnimSys, data->startBattler, &pos);
        break;
    case 25:
        ov07_02231D10(data->battleAnimSys, data->endBattler, &pos);
        break;
    case 26:
        ov07_02231D40(data->battleAnimSys, data->startBattler, &pos);
        break;
    case 27:
        ov07_02231D40(data->battleAnimSys, data->endBattler, &pos);
        break;
    case 8:
        ov07_02231DA0(data->battleAnimSys, data->startBattler, &pos);
        break;
    case 9:
        ov07_02231DA0(data->battleAnimSys, data->endBattler, &pos);
        break;
    case 3: {
        int params[4];

        ov07_0221F9A8(data->battleAnimSys, params, 4);
        data->dir = ov07_02221664(data);
        VEC_Set(
            &pos,
            params[1],
            params[2],
            params[3]);

        if (params[0] == 1) {
            data->dir = 1;
        }

        pos.x *= data->dir;
        pos.y *= data->dir;
    } break;
    case 4:
        ov07_02231B90(data->battleAnimSys, data->startBattler, &pos);
        ov07_02221734(data, &pos);
        break;
    case 5:
        ov07_02231B90(data->battleAnimSys, data->endBattler, &pos);
        ov07_02221734(data, &pos);
        break;
    case 100:
        ov07_02231B90(data->battleAnimSys, data->startBattler, &pos);
        ov07_022216A8(data, data->startBattler, &pos);
        break;
    case 101:
        ov07_02231B90(data->battleAnimSys, data->endBattler, &pos);
        ov07_022216A8(data, data->endBattler, &pos);
        break;
    case 12:
        ov07_02220938(emitter, data, &pos);
        ov07_02221734(data, &pos);
        break;
    case 13:
        ov07_022208F8(emitter, data, &pos);
        ov07_02221734(data, &pos);
        break;
    case 28:
        VEC_Set(&pos, ((fx32)(((2.8047) > 0) ? ((2.8047) * (1 << 12) + 0.5f) : ((2.8047) * (1 << 12) - 0.5f))), 0, 0);
        break;
    case 33:
        VEC_Set(&pos, ((fx32)(((-1.2207) > 0) ? ((-1.2207) * (1 << 12) + 0.5f) : ((-1.2207) * (1 << 12) - 0.5f))), ((fx32)(((-1.4648) > 0) ? ((-1.4648) * (1 << 12) + 0.5f) : ((-1.4648) * (1 << 12) - 0.5f))), 0);
        break;
    case 34:
        ov07_02231B90(data->battleAnimSys, data->startBattler, &pos);
        break;

    case 31: {
        ov07_022354E0_Array xyPos = ov07_022354E0;

        int battlerType = ov07_02231924(data->battleAnimSys, data->startBattler);
        VEC_Set(&pos, xyPos.a[battlerType][0], xyPos.a[battlerType][1], 0);
    } break;
    case 30: {
        ov07_02235578_Array xyPos = ov07_02235578;

        int battlerType;
        if (ov07_0221BFC0(data->battleAnimSys) == 1) {
            battlerType = 6;
        } else {
            battlerType = ov07_02231924(data->battleAnimSys, data->startBattler);
        }

        VEC_Set(&pos, xyPos.a[battlerType][0], xyPos.a[battlerType][1], 0);
    } break;
    case 32: {
        ov07_02235540_Array xyPos = ov07_02235540;

        int battlerType;
        if (ov07_0221BFC0(data->battleAnimSys) == 1) {
            battlerType = 6;
        } else {
            battlerType = ov07_02231924(data->battleAnimSys, data->startBattler);
        }

        VEC_Set(&pos, xyPos.a[battlerType][0], xyPos.a[battlerType][1], 0);
    } break;
    }

    data->position = pos;

    SPL_SetEmitterPositionX(emitter, pos.x);
    SPL_SetEmitterPositionY(emitter, pos.y);
    SPL_SetEmitterPositionZ(emitter, pos.z);
}
