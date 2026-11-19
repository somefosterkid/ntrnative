#include "global.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "seal_case.h"
#include "pm_string.h"
#include "mail_message_types.h"
#include "pokemon_types_def.h"
#include "options.h"
#include "pokepic.h"
#include "transform_data.h"
#include "pokemon_anim.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "player_data.h"
#include "overlay_95_egg_hatch_app.h"
#include "list_menu_items.h"
#include "camera.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"
#include "library/spl_emitter.h"
#include "library/spl_texture.h"
#include "library/spl_manager.h"
#include "particle_system.h"
#include "gf_gfx_planes.h"
#include "filesystem_files_def.h"
#include "palette.h"
#include "struct_defs/resdat_narc_structs.h"
#include "sprite_system.h"
#include "egg_hatch_particle_system.h"

typedef u16 charcode_t;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

BOOL SpriteSystem_LoadCellResObj(SpriteSystem *spriteSys, SpriteManager *spriteMan, enum NarcId narcID, int memberIdx, BOOL compressed, int resourceID);

BOOL SpriteSystem_LoadAnimResObj(SpriteSystem *spriteSys, SpriteManager *spriteMan, enum NarcId narcID, int memberIdx, BOOL compressed, int resourceID);

enum NamingScreenType {
    NAMING_SCREEN_TYPE_PLAYER,
    NAMING_SCREEN_TYPE_POKEMON,
    NAMING_SCREEN_TYPE_BOX,
    NAMING_SCREEN_TYPE_RIVAL,
    NAMING_SCREEN_TYPE_UNK4,
    NAMING_SCREEN_TYPE_GROUP,
    NAMING_SCREEN_TYPE_SHAYMIN_TABLET,
    NAMING_SCREEN_TYPE_PAL_PAD,
};

typedef u32 TextColor;

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

enum EggHatchShakeAnimID {
    EGG_MINOR_SHAKE = 0,
    EGG_BIG_SHAKE,
    EGG_BIGGER_SHAKE,
    EGG_EXPLOSIVE_SHAKE,
};

void ov95_021E5C44(BgConfig *bgConfig, Window *window, enum GFBgLayer bgLayer, int tilemapTop, int tilemapLeft, int width, int height, int baseTile, int palette);

ParticleSystem *NewParticleSystem(enum HeapID heapID);

ParticleSystem *CreateParticleSystem(enum HeapID heapID, int narcID, int narcMemberIdx);

BOOL ov95_021E60A4(EggHatchCutscene *eggHatch, enum EggHatchShakeAnimID idx);

int EggHatch_ShakeEgg(EggHatchCutscene *eggHatch, enum EggHatchShakeAnimID idx);

void ov95_021E5EF8(EggHatchCutscene *eggHatch);

typedef struct {
    int a[2][4];
} ov95_021E7594_Array;

extern const ov95_021E7594_Array ov95_021E7594;

void ov95_021E5EF8(EggHatchCutscene *eggHatch) {
    ov95_021E7594_Array narcIdx = ov95_021E7594;
    int narcID = (enum NarcId)115;
    int vRamType = NNS_G2D_VRAM_TYPE_2DMAIN;

    SpriteSystem *spriteSys = eggHatch->graphics.spriteSys;
    SpriteManager *spriteMan = eggHatch->graphics.spriteMan;
    PaletteData *plttData = eggHatch->graphics.plttData;

    SpriteSystem_LoadCharResObj(spriteSys, spriteMan, (enum NarcId)(narcID), narcIdx.a[0][0], 1, (enum NNS_G2D_VRAM_TYPE)(vRamType), 20000);
    SpriteSystem_LoadPaletteBuffer((struct PaletteData *)((plttData)), PLTTBUF_MAIN_OBJ, spriteSys, spriteMan, (enum NarcId)(narcID), narcIdx.a[0][1], 0, 1, (enum NNS_G2D_VRAM_TYPE)(vRamType), 20001);
    SpriteSystem_LoadCellResObj(spriteSys, spriteMan, (enum NarcId)(narcID), narcIdx.a[0][2], 1, 20002);
    SpriteSystem_LoadAnimResObj(spriteSys, spriteMan, (enum NarcId)(narcID), narcIdx.a[0][3], 1, 20003);

    SpriteSystem_LoadCharResObj(spriteSys, spriteMan, (enum NarcId)(narcID), 7, 1, (enum NNS_G2D_VRAM_TYPE)(vRamType), 25000);
    SpriteSystem_LoadPaletteBuffer(plttData, PLTTBUF_MAIN_OBJ, spriteSys, spriteMan, (enum NarcId)(narcID), 10, 0, 1, (enum NNS_G2D_VRAM_TYPE)(vRamType), 25001);
    SpriteSystem_LoadCellResObj(spriteSys, spriteMan, (enum NarcId)(narcID), 6, 1, 25002);
    SpriteSystem_LoadAnimResObj(spriteSys, spriteMan, (enum NarcId)(narcID), 5, 1, 25003);
}
