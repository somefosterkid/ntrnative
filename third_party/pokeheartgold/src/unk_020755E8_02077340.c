#include "global.h"

#include "unk_02005D10.h"
#include "constants/heap.h"
#include "library/spl_emitter.h"
#include "bag_types_def.h"
#include "bg_window.h"
#include "filesystem.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "sys_task.h"
#include "particle_system.h"
#include "pokemon_types_def.h"
#include "overlay_74_main_menu_util.h"
#include "pokemon_summary_app.h"
#include "pokedex.h"
#include "evolution_data.h"
#include "overlay_manager.h"
#include "unk_02088288.h"

BOOL sub_02077340(void *param_1, u32 param_2);

struct G3DPipelineBuffers;
struct GameRecords;
struct MessageLoader;
struct PaletteAnimator;
struct ParticleSystem;
struct Party;
struct Pokedex;
struct Pokemon;
struct PokemonAnimManager;
struct PokemonSprite;
struct PokemonSpriteManager;
struct PokemonSpriteTransforms;
struct PokemonSummary;
struct Poketch;
struct StringTemplate;
typedef struct {
    enum HeapID heapID;
    int unk_04;
} UnkStruct_0207C894;
typedef struct {
    UnkStruct_0207C894 unk_00;
    SPLEmitter *unk_08;
    ParticleSystem *unk_0C;
} UnkStruct_0207C8C4;
typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;
typedef struct MessageLoader MessageLoader;
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
    PokemonSpriteTemplate template;
    PokemonSpriteTemplate templateBackup;
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
typedef struct PokemonAnimManager PokemonAnimManager;
typedef struct StringTemplate StringTemplate;
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
typedef struct GameRecords GameRecords;
typedef struct Poketch Poketch;
typedef struct PaletteAnimator PaletteAnimator;
typedef void (*UnkFuncPtr_020157E4)(PaletteAnimator *, u16 *);
struct PaletteAnimator {
    SysTask *unk_00;
    UnkFuncPtr_020157E4 unk_04;
    PaletteData *unk_08;
    u16 unk_0C[16];
    u16 unk_2C[16];
    u8 unk_4C;
    u8 unk_4D;
    u8 unk_4E;
    u8 unk_4F;
};
void sub_02077394(EvolutionData *evolutionData);

BOOL sub_02077340(void *param_1, u32 param_2) {
    BOOL BVar1;

    BVar1 = 0;
    switch (param_2) {
    case 1:
        BVar1 = GF_IsAnySEPlaying();
        break;
    case 2:
        BVar1 = IsFanfarePlaying();
        break;
    case 3:
        PlayFanfare(0x4a4);
        break;
    case 4:
        PlaySE(0x5e6);
        break;
    case 5:
        PlayFanfare(0x4a0);
    }
    return BVar1;
}

void sub_02077394(EvolutionData *evolutionData) {
    evolutionData->summaryScreenAppMan = OverlayManager_New(&gOverlayTemplate_PokemonSummary, evolutionData->monSummary, evolutionData->heapID);
}
