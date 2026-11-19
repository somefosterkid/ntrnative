#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sys_task.h"
#include "overlay_71_trade3dmodel.h"
#include "overlay_71_trade_sequence_data.h"
#include "overlay_71_sprite_anim_resources.h"





struct PokemonSprite;

struct PokemonSpriteManager;

struct PokemonSpriteTransforms;

struct Trade3DModel;

struct Trade3DScene;

struct TradeSequenceData;

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef struct Trade3DScene Trade3DScene;

typedef struct PokemonSprite PokemonSprite;

typedef struct PokemonSpriteTransforms PokemonSpriteTransforms;

typedef void(PokemonSpriteCallback)(PokemonSprite *, PokemonSpriteTransforms *);

struct PokemonSpriteTransforms {
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

typedef struct PokemonSpriteShadow {
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
} PokemonSpriteShadow;

struct PokemonSprite {
    u32 active : 1;
    u32 polygonID : 6;
    u32 needReloadChar : 1;
    u32 needReloadPltt : 1;
    u32 padding_00_9 : 23;
    PokepicTemplate template;
    PokepicTemplate templateBackup;
    PokemonSpriteTransforms transforms;
    u8 animActive;
    u8 currAnimFrame;
    u8 animFrameDelay;
    u8 currSpriteFrame;
    u8 animLoopTimers[10];
    u8 padding_66[2];
    PokemonSpriteCallback *callback;
    PokemonSpriteShadow shadow;
    PokemonSpriteShadow shadowBackup;
    SpriteAnimFrame animFrames[10];
};

typedef struct PokemonSpriteManager PokemonSpriteManager;

PokemonSprite *PokepicManager_CreatePokepic(PokemonSpriteManager *monSpriteMan, PokepicTemplate *spriteTemplate, int x, int y, int z, int polygonID, SpriteAnimFrame *animFrames, PokemonSpriteCallback *callback);

void GetBoxmonSpriteCharAndPlttNarcIds(PokepicTemplate *spriteTemplate, BoxPokemon *mon, u8 face, BOOL preferDP);

u8 sub_02070854(BoxPokemon *boxMon, u8 face, BOOL preferDP);

void NARC_ReadPokepicAnimScript(NARC *narc, SpriteAnimFrame *frames, u16 species, u16 clientType);

const BoxPokemon *ov71_02247398(TradeSequenceData *sequenceData);

u16 ov71_022473BC(TradeSequenceData *sequenceData);

typedef struct TradeReceivePhase {
    TradeSequenceData *sequenceData;
    int subStepCounter;
    int timer;
    BOOL fanfarePlaying;
    PokemonSpriteManager *spriteManager;
    PokemonSprite *pokemonSprite;
    SpriteAnimFrame animFrames[10];
    Sprite *sprites[2];
    SpriteAnimResources animResources;
    BgConfig *bgConfig;
    Window window;
    String *string0;
    String *string1;
    Trade3DScene *scene;
    Trade3DModel *model;
    SysTask *task0;
    SysTask *task1;
    NARC *pokeDataNarc;
} TradeReceivePhase;

PokemonSprite *ov71_0224B7EC(TradeReceivePhase *trPhase);

PokemonSprite *ov71_0224B7EC(TradeReceivePhase *trPhase) {
    PokepicTemplate template;
    BoxPokemon *boxMon;
    int y;

    boxMon = (BoxPokemon *)ov71_02247398(trPhase->sequenceData);

    GetBoxmonSpriteCharAndPlttNarcIds(&template, boxMon, 2, 0);
    NARC_ReadPokepicAnimScript(trPhase->pokeDataNarc, trPhase->animFrames, ov71_022473BC(trPhase->sequenceData), 1);

    y = (100 - 20) + sub_02070854(boxMon, 2, 0);
    return PokepicManager_CreatePokepic(trPhase->spriteManager, &template, 128, y, 0, 0, trPhase->animFrames, 0L);
}
