#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "heap.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sys_task.h"
#include "gf_gfx_planes.h"
#include "overlay_71_trade3dmodel.h"
#include "pokepic.h"
#include "overlay_71_trade_sequence_data.h"
#include "overlay_71_sprite_anim_resources.h"

struct Pokepic;

struct PokepicManager;

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

void ov71_02247498(Trade3DScene *tradeScene);

typedef struct PokemonSpriteTransforms PokemonSpriteTransforms;

typedef void(PokemonSpriteCallback)(Pokepic *, PokemonSpriteTransforms *);

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

void PokepicManager_Delete(PokepicManager *monSpriteMan);

void Pokepic_Delete(Pokepic *monSprite);

typedef struct TradeReceivePhase {
    TradeSequenceData *sequenceData;
    int subStepCounter;
    int timer;
    BOOL fanfarePlaying;
    PokepicManager *spriteManager;
    Pokepic *pokemonSprite;
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

void ov71_0224B8EC(TradeReceivePhase *trPhase);

void ov71_0224B970(TradeReceivePhase *trPhase);

void ov71_0224BA48(SysTask *task);

void ov71_0224B198(void *param);

void ov71_0224B198(void *param) {
    TradeReceivePhase *trPhase = param;

    if (trPhase) {
        ov71_0224B970(trPhase);
        ov71_0224B8EC(trPhase);
        ov71_0224BA48(trPhase->task1);

        String_Delete(trPhase->string0);
        String_Delete(trPhase->string1);

        if (trPhase->scene) {
            ov71_02247498(trPhase->scene);
        }

        FreeBgTilemapBuffer(trPhase->bgConfig, GF_BG_LYR_MAIN_1);
        RemoveWindow(&(trPhase->window));

        if (trPhase->pokemonSprite) {
            Pokepic_Delete(trPhase->pokemonSprite);
        }

        NARC_Delete(trPhase->pokeDataNarc);
        PokepicManager_Delete(trPhase->spriteManager);
        Heap_Free(trPhase);
    }
}
