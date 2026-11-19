#include "global.h"

#include "constants/heap.h"

#include "library/spl_emitter.h"
#include "library/spl_manager.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "obj_pltt_transfer.h"
#include "palette.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02014DA0.h"
#include "filesystem_files_def.h"
#include "particle_system.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_07_generic_emitter.h"
#include "overlay_07_battle_anim_tracking.h"

struct BattleAnimSystem;

struct ParticleSystem;

struct Camera;

struct CameraAngle;

struct CameraPerspective;

struct CameraPositionHistory;

struct GenericEmitterCallbackData;

struct SPLEmitter;

struct SPLEmitterState;

enum CameraProjection {
    CAMERA_PROJECTION_PERSPECTIVE = 0,
    CAMERA_PROJECTION_ORTHOGRAPHIC,
    CAMERA_PROJECTION_COUNT
};

typedef struct CameraPositionHistory CameraPositionHistory;

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

typedef struct PokemonSpriteTransforms_hk3 PokemonSpriteTransforms_hk3;

typedef struct PokemonSpriteTemplate_hk3 PokemonSpriteTemplate_hk3;

typedef struct ChatotCry_hk3 ChatotCry_hk3;

typedef struct PokemonSpriteData_hk3 PokemonSpriteData_hk3;

typedef struct SpriteAnimFrame_hk3 SpriteAnimFrame_hk3;

typedef struct PokemonSprite_hk3 PokemonSprite_hk3;

typedef struct PokemonSpriteShadow_hk3 PokemonSpriteShadow_hk3;

typedef struct BattleBackgroundReference_hk3 BattleBackgroundReference_hk3;

typedef struct BattleAnimContext_hk3 BattleAnimContext_hk3;

typedef struct CustomBgScrollContext_hk3 CustomBgScrollContext_hk3;

typedef struct BattleAnimScriptLoop_hk3 BattleAnimScriptLoop_hk3;

typedef struct PokemonSpriteDrawContext_hk3 PokemonSpriteDrawContext_hk3;

typedef struct BgWaveStrip_hk3 BgWaveStrip_hk3;

typedef struct WaveAnimContext_hk3 WaveAnimContext_hk3;

typedef struct BattleBgAnim_hk3 BattleBgAnim_hk3;

typedef struct SpriteTrackingTask_hk3 SpriteTrackingTask_hk3;

typedef struct BgTrackingTask_hk3 BgTrackingTask_hk3;

typedef struct BattleAnimSystem_hk3 BattleAnimSystem_hk3;

typedef struct BufferManager_hk3 BufferManager_hk3;

typedef struct VBlankDMAController_hk3 VBlankDMAController_hk3;

typedef struct GenericEmitterCallbackData_hk3 GenericEmitterCallbackData_hk3;

struct PokemonSpriteTransforms_hk3 {
    s16 xCenter;
    s16 yCenter;
    int zCenter;
    s16 xOffset;
    s16 yOffset;
    int zOffset;
    s16 scaleX;
    s16 scaleY;
    u16 rotationX;
    u16 rotationY;
    u16 rotationZ;
    u16 padding_1A;
    s16 xPivot;
    s16 yPivot;
    u8 drawXOffset;
    u8 drawYOffset;
    u8 drawWidth;
    u8 drawHeight;
    u8 fadeInitAlpha;
    u8 fadeTargetAlpha;
    u8 fadeDelayCounter;
    u8 fadeDelayLength;
    u32 fadeTargetColor;
    u32 diffuseR : 5;
    u32 diffuseG : 5;
    u32 diffuseB : 5;
    u32 ambientR : 5;
    u32 ambientG : 5;
    u32 ambientB : 5;
    u32 padding_2C_30 : 2;
    u32 hide : 1;
    u32 partialDraw : 1;
    u32 alpha : 5;
    u32 padding_30_07 : 2;
    u32 flipH : 1;
    u32 flipV : 1;
    u32 hide2 : 1;
    u32 fadeActive : 1;
    u32 mosaicIntensity : 4;
    u32 padding_30_17 : 15;
};

typedef u32 (*SPLTexVRAMAllocFunc)(u32 size, BOOL is4x4comp);

typedef u32 (*SPLPalVRAMAllocFunc)(u32 size, BOOL is4pal);

enum BattleAnimSystemArc {
    BATTLE_ANIM_SYSTEM_ARC_BATT_BG = 0,
    BATTLE_ANIM_SYSTEM_ARC_BATT_OBJ,
    BATTLE_ANIM_SYSTEM_ARC_WECHAR,
    BATTLE_ANIM_SYSTEM_ARC_WEPLTT,
    BATTLE_ANIM_SYSTEM_ARC_WECELL,
    BATTLE_ANIM_SYSTEM_ARC_WECELLANM,

    BATTLE_ANIM_SYSTEM_ARC_COUNT,
};

struct PokemonSpriteTemplate_hk3 {
    u16 narcID;
    u16 character;
    u16 palette;
    u16 spindaSpots;

    u8 dummy;

    u32 personality;
};

struct ChatotCry_hk3 {
    BOOL valid;
    s8 audioData[1000];
};

struct PokemonSpriteData_hk3 {
    u8 *tiles;
    int narcID;
    int palette;
    int yOffset;
};

struct SpriteAnimFrame_hk3 {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
};

typedef void(PokemonSpriteCallback)(PokemonSprite_hk3 *, PokemonSpriteTransforms_hk3 *);

struct PokemonSpriteShadow_hk3 {
    u16 plttSlot : 2;
    u16 shouldFollowX : 1;
    u16 shouldFollowY : 1;
    u16 isAffine : 1;
    u16 size : 2;
    u16 padding_00 : 9;
    s8 height;
    u8 padding_03;
    s16 x;
    s16 y;
    s16 xOffset;
    s16 yOffset;
};

struct PokemonSprite_hk3 {
    u32 active : 1;
    u32 polygonID : 6;
    u32 needReloadChar : 1;
    u32 needReloadPltt : 1;
    u32 padding_00_9 : 23;
    PokemonSpriteTemplate_hk3 template;
    PokemonSpriteTemplate_hk3 templateBackup;
    PokemonSpriteTransforms_hk3 transforms;
    u8 animActive;
    u8 currAnimFrame;
    u8 animFrameDelay;
    u8 currSpriteFrame;
    u8 animLoopTimers[10];
    u8 padding_66[2];
    PokemonSpriteCallback *callback;
    PokemonSpriteShadow_hk3 shadow;
    PokemonSpriteShadow_hk3 shadowBackup;
    SpriteAnimFrame_hk3 animFrames[10];
};

struct BattleBackgroundReference_hk3 {
    int narcID;
    int tilesNarcMemberIdx;
    int paletteNarcMemberIdx;
    int tilemapNarcMemberIdx;
    int paletteDestStart;
    int paletteSrcSize;
};

struct BattleAnimContext_hk3 {
    u8 command;
    u8 unk_01;

    u16 move;
    s32 damage;
    u16 power;
    u16 friendship;
    u32 fieldConditions;
    u16 effectChance;
    u16 terrain;

    u16 attacker;
    u16 defender;
    int currentParticleSystem;
    ParticleSystem *particleSystems[16];
    SPLEmitter *emitters[20];
    SpriteSystem *spriteSystem;

    PokemonSpriteData_hk3 *pokemonSpriteData[4];
    u8 battlerTypes[4];
    PokemonSprite_hk3 *battlerSprites[4];
    u32 battleType;
    u16 battlerSpecies[4];
    u8 battlerGenders[4];
    u8 battlerShinyFlags[4];
    u8 battlerForms[4];
    u32 battlerPersonalities[4];
    u32 battlerMoveEffects[4];
    ChatotCry_hk3 *chatotCry;
    u8 *bgTiles;
    u16 *bgPaletteBuffer;
    BOOL isTransformed;
};

struct BattleAnimScriptLoop_hk3 {
    u32 *startPos;
    u8 curIteration;
    u8 maxIterations;
    BOOL isActive;
};

struct PokemonSpriteDrawContext_hk3 {
    struct BattleAnimSystem_hk3 *battleAnimSys;
    SpriteManager *spriteManager;
    ManagedSprite *sprite;
    BOOL active;
};

struct BgWaveStrip_hk3 {
    s16 startScanline;
    s16 endScanline;
    s16 scrollSpeed;
    s16 scrollAccum;
    int baseOffset;
};

struct WaveAnimContext_hk3 {
    BgWaveStrip_hk3 strips[16];
    struct CustomBgScrollContext_hk3 *bgScrollCtx;
};

struct BattleBgAnim_hk3 {
    BgConfig *bgConfig;
    s16 offsetX;
    s16 offsetY;
    s16 stepX;
    s16 stepY;
    int bg;
    int unusedBg;
    BOOL unused;
    BOOL cancel;
    WaveAnimContext_hk3 *waveAnimCtx;
};

struct SpriteTrackingTask_hk3 {
    ManagedSprite *sprite;
    PokemonSpriteTrackingData data;
    SysTask *task;
};

struct BgTrackingTask_hk3 {
    BgConfig *bg;
    PokemonSpriteTrackingData data;
    SysTask *task;
};

struct BattleAnimSystem_hk3 {
    enum HeapID heapID;
    enum NarcId moveArcID;
    BOOL isContest;
    BOOL isActive;
    BOOL moveActive;
    void *scriptData;
    u32 *scriptPtr;
    u32 *callStack[3];
    BattleAnimScriptLoop_hk3 loopStack[3];
    PokemonSpriteDrawContext_hk3 pokemonSpriteDrawContexts[2];
    u8 cameraProjections[16];
    u8 particleSystemCameraFlip[16];
    s8 unused_88;
    u8 scriptDelay;
    u16 activeAnimTasks;
    u16 activeSoundTasks;
    s32 scriptVars[10];
    void (*executeAnimScriptFunc)(struct BattleAnimSystem_hk3 *);
    BattleAnimContext_hk3 *context;
    BgConfig *bgConfig;
    PaletteData *paletteData;
    SpriteManager *spriteManagers[4];
    ManagedSprite *sprites[10];
    SpriteTemplate lastSpriteTemplate;
    SpriteManager *pokemonSpriteManager;
    ManagedSprite *pokemonSprites[5];
    BOOL usedPokemonSprites[5];
    SpriteTrackingTask_hk3 *spriteTrackingTasks[5];
    BgTrackingTask_hk3 *bgTrackingTask;
    u8 bgSwitchState;
    u8 soundEffectWaitTimer;
    u8 endDelayFrames;
    u8 unused_17B;
    BattleBgAnim_hk3 *bgAnim;
    BattleBackgroundReference_hk3 battleBgRefs;
    int baseBgPalettes;
    u8 bgLayerPriorities[4];
    NARC *arcs[BATTLE_ANIM_SYSTEM_ARC_COUNT];
};

struct BufferManager_hk3 {
    void *buffers[2];
    u8 writeBuffer;
    u8 mode;
    u8 padding[2];
};

struct VBlankDMAController_hk3 {
    SysTask *onVBlankTask;
    SysTask *postVBlankTask;
    BOOL doDMA;
    BOOL swapBuffers;
    VBlankDMAFunc dmaFunc;
    VBlankDMAFunc swapBufferFunc;
    void *param;
};

struct CustomBgScrollContext_hk3 {
    VBlankDMAController_hk3 dmaController;
    BufferManager_hk3 *bufferManager;
    u32 buffer1[192];
    u32 buffer2[192];
    void *offsetReg;
};

struct GenericEmitterCallbackData_hk3 {
    BattleAnimSystem_hk3 *battleAnimSys;
    ParticleSystem *particleSys;
    s8 dir;
    int params[GENERIC_EMITTER_CALLBACK_PARAM_COUNT];
    int startBattler;
    int endBattler;
    VecFx32 position;
};

typedef struct PokemonSpriteTemplate_hk4 PokemonSpriteTemplate_hk4;

typedef struct ChatotCry_hk4 ChatotCry_hk4;

typedef struct PokemonSpriteData_hk4 PokemonSpriteData_hk4;

typedef struct SpriteAnimFrame_hk4 SpriteAnimFrame_hk4;

typedef struct PokemonSprite_hk4 PokemonSprite_hk4;

typedef struct PokemonSpriteTransforms_hk4 PokemonSpriteTransforms_hk4;

typedef struct PokemonSpriteShadow_hk4 PokemonSpriteShadow_hk4;

typedef struct BattleBackgroundReference_hk4 BattleBackgroundReference_hk4;

typedef struct BattleAnimContext_hk4 BattleAnimContext_hk4;

typedef struct CustomBgScrollContext_hk4 CustomBgScrollContext_hk4;

typedef struct BattleAnimScriptLoop_hk4 BattleAnimScriptLoop_hk4;

typedef struct PokemonSpriteDrawContext_hk4 PokemonSpriteDrawContext_hk4;

typedef struct BgWaveStrip_hk4 BgWaveStrip_hk4;

typedef struct WaveAnimContext_hk4 WaveAnimContext_hk4;

typedef struct BattleBgAnim_hk4 BattleBgAnim_hk4;

typedef struct SpriteTrackingTask_hk4 SpriteTrackingTask_hk4;

typedef struct BgTrackingTask_hk4 BgTrackingTask_hk4;

typedef struct BattleAnimSystem_hk4 BattleAnimSystem_hk4;

typedef struct BufferManager_hk4 BufferManager_hk4;

typedef struct VBlankDMAController_hk4 VBlankDMAController_hk4;

typedef struct GenericEmitterCallbackData_hk4 GenericEmitterCallbackData_hk4;

struct PokemonSpriteTemplate_hk4 {
    u16 narcID;
    u16 character;
    u16 palette;
    u16 spindaSpots;

    u8 dummy;

    u32 personality;
};

struct ChatotCry_hk4 {
    BOOL valid;
    s8 audioData[1000];
};

struct PokemonSpriteData_hk4 {
    u8 *tiles;
    int narcID;
    int palette;
    int yOffset;
};

struct SpriteAnimFrame_hk4 {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
};

struct PokemonSpriteTransforms_hk4 {
    s16 xCenter;
    s16 yCenter;
    int zCenter;
    s16 xOffset;
    s16 yOffset;
    int zOffset;
    s16 scaleX;
    s16 scaleY;
    u16 rotationX;
    u16 rotationY;
    u16 rotationZ;
    u16 padding_1A;
    s16 xPivot;
    s16 yPivot;
    u8 drawXOffset;
    u8 drawYOffset;
    u8 drawWidth;
    u8 drawHeight;
    u8 fadeInitAlpha;
    u8 fadeTargetAlpha;
    u8 fadeDelayCounter;
    u8 fadeDelayLength;
    u32 fadeTargetColor;
    u32 diffuseR : 5;
    u32 diffuseG : 5;
    u32 diffuseB : 5;
    u32 ambientR : 5;
    u32 ambientG : 5;
    u32 ambientB : 5;
    u32 padding_2C_30 : 2;
    u32 hide : 1;
    u32 partialDraw : 1;
    u32 alpha : 5;
    u32 padding_30_07 : 2;
    u32 flipH : 1;
    u32 flipV : 1;
    u32 hide2 : 1;
    u32 fadeActive : 1;
    u32 mosaicIntensity : 4;
    u32 padding_30_17 : 15;
};

struct PokemonSpriteShadow_hk4 {
    u16 plttSlot : 2;
    u16 shouldFollowX : 1;
    u16 shouldFollowY : 1;
    u16 isAffine : 1;
    u16 size : 2;
    u16 padding_00 : 9;
    s8 height;
    u8 padding_03;
    s16 x;
    s16 y;
    s16 xOffset;
    s16 yOffset;
};

struct PokemonSprite_hk4 {
    u32 active : 1;
    u32 polygonID : 6;
    u32 needReloadChar : 1;
    u32 needReloadPltt : 1;
    u32 padding_00_9 : 23;
    PokemonSpriteTemplate_hk4 template;
    PokemonSpriteTemplate_hk4 templateBackup;
    PokemonSpriteTransforms_hk4 transforms;
    u8 animActive;
    u8 currAnimFrame;
    u8 animFrameDelay;
    u8 currSpriteFrame;
    u8 animLoopTimers[10];
    u8 padding_66[2];
    PokemonSpriteCallback *callback;
    PokemonSpriteShadow_hk4 shadow;
    PokemonSpriteShadow_hk4 shadowBackup;
    SpriteAnimFrame_hk4 animFrames[10];
};

struct BattleBackgroundReference_hk4 {
    int narcID;
    int tilesNarcMemberIdx;
    int paletteNarcMemberIdx;
    int tilemapNarcMemberIdx;
    int paletteDestStart;
    int paletteSrcSize;
};

struct BattleAnimContext_hk4 {
    u8 command;
    u8 unk_01;

    u16 move;
    s32 damage;
    u16 power;
    u16 friendship;
    u32 fieldConditions;
    u16 effectChance;
    u16 terrain;

    u16 attacker;
    u16 defender;
    int currentParticleSystem;
    ParticleSystem *particleSystems[16];
    SPLEmitter *emitters[20];
    SpriteSystem *spriteSystem;

    PokemonSpriteData_hk4 *pokemonSpriteData[4];
    u8 battlerTypes[4];
    PokemonSprite_hk4 *battlerSprites[4];
    u32 battleType;
    u16 battlerSpecies[4];
    u8 battlerGenders[4];
    u8 battlerShinyFlags[4];
    u8 battlerForms[4];
    u32 battlerPersonalities[4];
    u32 battlerMoveEffects[4];
    ChatotCry_hk4 *chatotCry;
    u8 *bgTiles;
    u16 *bgPaletteBuffer;
    BOOL isTransformed;
};

struct BattleAnimScriptLoop_hk4 {
    u32 *startPos;
    u8 curIteration;
    u8 maxIterations;
    BOOL isActive;
};

struct PokemonSpriteDrawContext_hk4 {
    struct BattleAnimSystem_hk4 *battleAnimSys;
    SpriteManager *spriteManager;
    ManagedSprite *sprite;
    BOOL active;
};

struct BgWaveStrip_hk4 {
    s16 startScanline;
    s16 endScanline;
    s16 scrollSpeed;
    s16 scrollAccum;
    int baseOffset;
};

struct WaveAnimContext_hk4 {
    BgWaveStrip_hk4 strips[16];
    struct CustomBgScrollContext_hk4 *bgScrollCtx;
};

struct BattleBgAnim_hk4 {
    BgConfig *bgConfig;
    s16 offsetX;
    s16 offsetY;
    s16 stepX;
    s16 stepY;
    int bg;
    int unusedBg;
    BOOL unused;
    BOOL cancel;
    WaveAnimContext_hk4 *waveAnimCtx;
};

struct SpriteTrackingTask_hk4 {
    ManagedSprite *sprite;
    PokemonSpriteTrackingData data;
    SysTask *task;
};

struct BgTrackingTask_hk4 {
    BgConfig *bg;
    PokemonSpriteTrackingData data;
    SysTask *task;
};

struct BattleAnimSystem_hk4 {
    enum HeapID heapID;
    enum NarcId moveArcID;
    BOOL isContest;
    BOOL isActive;
    BOOL moveActive;
    void *scriptData;
    u32 *scriptPtr;
    u32 *callStack[3];
    BattleAnimScriptLoop_hk4 loopStack[3];
    PokemonSpriteDrawContext_hk4 pokemonSpriteDrawContexts[2];
    u8 cameraProjections[16];
    u8 particleSystemCameraFlip[16];
    s8 unused_88;
    u8 scriptDelay;
    u16 activeAnimTasks;
    u16 activeSoundTasks;
    s32 scriptVars[10];
    void (*executeAnimScriptFunc)(struct BattleAnimSystem_hk4 *);
    BattleAnimContext_hk4 *context;
    BgConfig *bgConfig;
    PaletteData *paletteData;
    SpriteManager *spriteManagers[4];
    ManagedSprite *sprites[10];
    SpriteTemplate lastSpriteTemplate;
    SpriteManager *pokemonSpriteManager;
    ManagedSprite *pokemonSprites[5];
    BOOL usedPokemonSprites[5];
    SpriteTrackingTask_hk4 *spriteTrackingTasks[5];
    BgTrackingTask_hk4 *bgTrackingTask;
    u8 bgSwitchState;
    u8 soundEffectWaitTimer;
    u8 endDelayFrames;
    u8 unused_17B;
    BattleBgAnim_hk4 *bgAnim;
    BattleBackgroundReference_hk4 battleBgRefs;
    int baseBgPalettes;
    u8 bgLayerPriorities[4];
    NARC *arcs[BATTLE_ANIM_SYSTEM_ARC_COUNT];
};

struct BufferManager_hk4 {
    void *buffers[2];
    u8 writeBuffer;
    u8 mode;
    u8 padding[2];
};

struct VBlankDMAController_hk4 {
    SysTask *onVBlankTask;
    SysTask *postVBlankTask;
    BOOL doDMA;
    BOOL swapBuffers;
    VBlankDMAFunc dmaFunc;
    VBlankDMAFunc swapBufferFunc;
    void *param;
};

struct CustomBgScrollContext_hk4 {
    VBlankDMAController_hk4 dmaController;
    BufferManager_hk4 *bufferManager;
    u32 buffer1[192];
    u32 buffer2[192];
    void *offsetReg;
};

struct GenericEmitterCallbackData_hk4 {
    BattleAnimSystem_hk4 *battleAnimSys;
    ParticleSystem *particleSys;
    s8 dir;
    int params[GENERIC_EMITTER_CALLBACK_PARAM_COUNT];
    int startBattler;
    int endBattler;
    VecFx32 position;
};

struct GenericEmitterCallbackData_hk6;

struct UnkStruct_ov07_0221FA38;

typedef struct UnkStruct_ov07_0221FA38 UnkStruct_ov07_0221FA38;

typedef struct GenericEmitterCallbackData_hk6 {
    UnkStruct_ov07_0221FA38 *battleAnimSys;
    ParticleSystem *particleSys;
    s8 dir;
    int params[6];
    int startBattler;
    int endBattler;
} GenericEmitterCallbackData_hk6;

s32 ov07_02221C8C(s32);

s32 ov07_02221CB4(void);

extern BOOL ov07_0221F9A8(BattleAnimSystem *system, int params[], int count);

BOOL ov07_02221CEC(SPLEmitter *emitter, GenericEmitterCallbackData *data);

void ov07_02221CE0(GenericEmitterCallbackData *data, int params[4]);

s32 ov07_02221CB4(void) {
    s32 v0;
    s32 v1;
    s32 v2;

    v1 = 0;
    v2 = 0;
loop_1:
    v0 = ov07_02221C8C(v1);
    v1 += 1;
    if (v0 != 0xFEFE) {
        v2 += 1;
        if (v2 < 0xFF) {
            goto loop_1;
        }
    }
    if (v2 >= 0xFF) {
        return 0;
    }
    return v1;
}

void ov07_02221CE0(GenericEmitterCallbackData *data, int params[4]) {
    ov07_0221F9A8(data->battleAnimSys, params, 4);
}

BOOL ov07_02221CEC(SPLEmitter *emitter, GenericEmitterCallbackData *data) {
    CameraAngle angle = { 0, 0, 0 };
    Camera *camera = sub_02015524(data->particleSys);
    Camera_SetAnglePos(&angle, camera);

    return 1;
}

BOOL ov07_02221D10(SPLEmitter *emitter, GenericEmitterCallbackData *data);
BOOL ov07_02221D4C(SPLEmitter *emitter, GenericEmitterCallbackData *data);
BOOL ov07_02221D88(SPLEmitter *emitter, GenericEmitterCallbackData_hk3 *data);
s32 ov07_02221DC4(SPLEmitter *emitter, GenericEmitterCallbackData_hk4 *data);
BOOL ov07_02221E24(SPLEmitter *emitter, GenericEmitterCallbackData *data);
BOOL ov07_02221E60(SPLEmitter *emitter, GenericEmitterCallbackData_hk6 *data);
BOOL ov07_02221E9C(SPLEmitter *emitter, GenericEmitterCallbackData *data);
BOOL ov07_02221EC4(SPLEmitter *emitter, GenericEmitterCallbackData *data);

static BOOL (*const ov07_02236444[9])(SPLEmitter *, GenericEmitterCallbackData *) = {
    ov07_02221CEC,
    ov07_02221D10,
    ov07_02221D4C,
    (BOOL(*)(SPLEmitter *, GenericEmitterCallbackData *))ov07_02221D88,
    (BOOL(*)(SPLEmitter *, GenericEmitterCallbackData *))ov07_02221DC4,
    ov07_02221E24,
    (BOOL(*)(SPLEmitter *, GenericEmitterCallbackData *))ov07_02221E60,
    ov07_02221E9C,
    ov07_02221EC4,
};

BOOL ov07_02221D10(SPLEmitter *emitter, GenericEmitterCallbackData *data);

BOOL ov07_02221D10(SPLEmitter *emitter, GenericEmitterCallbackData *data) {

    CameraAngle angle = { ((u16)(((45) * 0xFFFF) / 360)) + 1, ((u16)(((45) * 0xFFFF) / 360)) + 1, 0 };
    Camera *camera = sub_02015524(data->particleSys);

    sub_02015528(data->particleSys, CAMERA_PROJECTION_ORTHOGRAPHIC);
    Camera_SetAnglePos(&angle, camera);

    return 1;
}

BOOL ov07_02221D4C(SPLEmitter *emitter, GenericEmitterCallbackData *data);

BOOL ov07_02221D4C(SPLEmitter *emitter, GenericEmitterCallbackData *data) {
    int params[4];
    CameraAngle angle = { 0, 0, 0, 0 };

    ov07_02221CE0(data, params);

    angle.x = params[1];
    angle.y = params[2];
    angle.z = params[3];

    Camera *camera = sub_02015524(data->particleSys);
    Camera_SetAnglePos(&angle, camera);

    return 1;
}

BOOL ov07_02221D88(SPLEmitter *emitter, GenericEmitterCallbackData_hk3 *data);

BOOL ov07_02221D88(SPLEmitter *emitter, GenericEmitterCallbackData_hk3 *data) {
    Camera *camera;
    CameraAngle CStack_18 = { 0xD000, 0x2000, 0x1000, 0 };

    camera = sub_02015524(data->particleSys);
    sub_02015528(data->particleSys, 1);
    Camera_SetAnglePos(&CStack_18, camera);
    return 1;
}

s32 ov07_0221BFC0(void *arg0);

s32 ov07_02221DC4(SPLEmitter *emitter, GenericEmitterCallbackData_hk4 *data);

s32 ov07_02221DC4(SPLEmitter *emitter, GenericEmitterCallbackData_hk4 *data) {
    CameraAngle v0 = { 0xC200, 0x1740, 0x1000, 0 };
    CameraAngle delta = { 0x2420, 0x0EA0, 0, 0 };
    Camera *v1;

    v1 = sub_02015524(data->particleSys);
    sub_02015528(data->particleSys, 1);
    if (ov07_0221BFC0(data->battleAnimSys) == 1) {
        Camera_SetAnglePos(&v0, v1);
    } else {
        Camera_SetAnglePos(&delta, v1);
    }
    return 1;
}

BOOL ov07_02221E24(SPLEmitter *emitter, GenericEmitterCallbackData *data);

BOOL ov07_02221E24(SPLEmitter *emitter, GenericEmitterCallbackData *data) {
    CameraAngle angle = { 0xC200, 0x1740, 0x1000, 0 };
    Camera *camera = sub_02015524(data->particleSys);

    sub_02015528(data->particleSys, CAMERA_PROJECTION_ORTHOGRAPHIC);
    Camera_SetAnglePos(&angle, camera);

    return 1;
}

BOOL ov07_02221E60(SPLEmitter *emitter, GenericEmitterCallbackData_hk6 *data);

BOOL ov07_02221E60(SPLEmitter *emitter, GenericEmitterCallbackData_hk6 *data) {
    CameraAngle angle = { 0, 0, 0xE890, 0 };
    Camera *camera = sub_02015524(data->particleSys);
    sub_02015528(data->particleSys, 1);
    Camera_SetAnglePos(&angle, camera);
    return TRUE;
}

void ov07_02231D70(BattleAnimSystem *system, int battler, VecFx32 *pos);

BOOL ov07_02221E9C(SPLEmitter *emitter, GenericEmitterCallbackData *data);

BOOL ov07_02221E9C(SPLEmitter *emitter, GenericEmitterCallbackData *data) {
    Camera *camera = sub_02015524(data->particleSys);

    VecFx32 target;
    ov07_02231D70(data->battleAnimSys, data->startBattler, &target);
    Camera_SetLookAtCamTarget(&target, camera);

    return 1;
}

BOOL ov07_02221EC4(SPLEmitter *emitter, GenericEmitterCallbackData *data);

BOOL ov07_02221EC4(SPLEmitter *emitter, GenericEmitterCallbackData *data) {
    Camera *camera = sub_02015524(data->particleSys);

    VecFx32 target;
    ov07_02231D70(data->battleAnimSys, data->endBattler, &target);
    Camera_SetLookAtCamTarget(&target, camera);

    return 1;
}

void ov07_02221EEC(int funcID, SPLEmitter *emitter, GenericEmitterCallbackData *data);

void ov07_02221EEC(int funcID, SPLEmitter *emitter, GenericEmitterCallbackData *data) {
    ov07_02236444[funcID](emitter, data);
}
