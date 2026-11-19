#include "global.h"

#include "screen_fade.h"
#include "sprite.h"
#include "bg_window.h"
#include "filesystem.h"
#include "pm_string.h"
#include "sys_task.h"
#include "unk_02005D10.h"
#include "overlay_71_trade_receive_phase.h"
#include "overlay_71_trade3dmodel.h"
#include "pokemon_types_def.h"
#include "overlay_74_main_menu_util.h"
#include "overlay_71_trade_sequence_data.h"
#include "overlay_71_sprite_anim_resources.h"

s32 ov71_0224B450(void *arg0, s32 *arg1);

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
void ov71_02247704(Trade3DModel *tradeModel, BOOL enabled);
typedef u16 charcode_t;
enum PokemonSpriteAttribute {
    MON_SPRITE_X_CENTER = 0,
    MON_SPRITE_Y_CENTER = 1,
    MON_SPRITE_Z_CENTER = 2,
    MON_SPRITE_X_OFFSET = 3,
    MON_SPRITE_Y_OFFSET = 4,
    MON_SPRITE_Z_OFFSET = 5,
    MON_SPRITE_HIDE = 6,
    MON_SPRITE_ROTATION_X = 7,
    MON_SPRITE_ROTATION_Y = 8,
    MON_SPRITE_ROTATION_Z = 9,
    MON_SPRITE_X_PIVOT = 10,
    MON_SPRITE_Y_PIVOT = 11,
    MON_SPRITE_SCALE_X = 12,
    MON_SPRITE_SCALE_Y = 13,
    MON_SPRITE_PARTIAL_DRAW = 14,
    MON_SPRITE_DRAW_X_OFFSET = 15,
    MON_SPRITE_DRAW_Y_OFFSET = 16,
    MON_SPRITE_DRAW_WIDTH = 17,
    MON_SPRITE_DRAW_HEIGHT = 18,
    MON_SPRITE_SHADOW_X = 19,
    MON_SPRITE_SHADOW_Y = 20,
    MON_SPRITE_SHADOW_X_OFFSET = 21,
    MON_SPRITE_SHADOW_Y_OFFSET = 22,
    MON_SPRITE_ALPHA = 23,
    MON_SPRITE_DIFFUSE_R = 24,
    MON_SPRITE_DIFFUSE_G = 25,
    MON_SPRITE_DIFFUSE_B = 26,
    MON_SPRITE_AMBIENT_R = 27,
    MON_SPRITE_AMBIENT_G = 28,
    MON_SPRITE_AMBIENT_B = 29,
    MON_SPRITE_FADE_ACTIVE = 30,
    MON_SPRITE_FADE_TARGET_COLOR = 31,
    MON_SPRITE_FADE_INIT_ALPHA = 32,
    MON_SPRITE_FADE_TARGET_ALPHA = 33,
    MON_SPRITE_FADE_DELAY_COUNTER = 34,
    MON_SPRITE_FLIP_H = 35,
    MON_SPRITE_FLIP_V = 36,
    MON_SPRITE_HIDE_2 = 37,
    MON_SPRITE_CURR_SPRITE_FRAME = 38,
    MON_SPRITE_DUMMY = 39,
    MON_SPRITE_MOSAIC_INTENSITY = 40,
    MON_SPRITE_SHADOW_HEIGHT = 41,
    MON_SPRITE_SHADOW_PLTT_SLOT = 42,
    MON_SPRITE_SHADOW_SHOULD_FOLLOW_X = 43,
    MON_SPRITE_SHADOW_SHOULD_FOLLOW_Y = 44,
    MON_SPRITE_SHADOW_IS_AFFINE = 45,
    MON_SPRITE_SHADOW_SIZE = 46,
};
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
void Pokepic_StartAnim(PokemonSprite *monSprite, int dummy);
void Pokepic_SetAttr(PokemonSprite *monSprite, enum PokemonSpriteAttribute attribute, int value);
u32 GetBoxMonData(BoxPokemon *boxMon, int param, void *dest);
void sub_020729A4(NARC *narc, u8 *cryDelay, u16 species, u16 clientType);
const BoxPokemon *ov71_02247398(TradeSequenceData *sequenceData);
u16 ov71_022473BC(TradeSequenceData *sequenceData);
u16 ov71_022473C4(TradeSequenceData *sequenceData);
void ov71_0224B910(TradeReceivePhase *trPhase, int startValue, int targetValue, int duration);
BOOL ov71_0224B960(TradeReceivePhase *trPhase);
void ov71_0224B9CC(TradeReceivePhase *trPhase, SysTask **task);
int ov71_0224B47C(TradeReceivePhase *trPhase, int *subStepCounter);

s32 ov71_0224B450(void *arg0, s32 *arg1) {
    if ((*arg1 == 0) && (IsPaletteFadeFinished() != 0)) {
        Sprite_SetAnimCtrlSeq(*(Sprite **)((u8 *)arg0 + 0x40), 1);
        Sprite_SetDrawFlag(*(Sprite **)((u8 *)arg0 + 0x40), 1);
        return 1;
    }
    return 0;
}

int ov71_0224B47C(TradeReceivePhase *trPhase, int *subStepCounter) {
    switch (*subStepCounter) {
    case 0:
        ov71_0224B9CC(trPhase, &(trPhase->task1));
        (*subStepCounter)++;
    case 1:
        if (trPhase->task1 == 0L) {
            Sprite_SetAnimCtrlSeq(trPhase->sprites[1], 2);
            Sprite_SetDrawFlag(trPhase->sprites[1], 1);
            ov71_02247704(trPhase->model, 0);
            trPhase->timer = 0;
            (*subStepCounter)++;
        }
        break;
    case 2:
        if (++(trPhase->timer) > 10) {
            Pokepic_SetAttr(trPhase->pokemonSprite, MON_SPRITE_HIDE, 0);
            ov71_0224B910(trPhase, 16, 0, 12);
            trPhase->subStepCounter++;
        }
        break;
    case 3:
        if (ov71_0224B960(trPhase)) {
            const BoxPokemon *boxMon = ov71_02247398(trPhase->sequenceData);

            if (GetBoxMonData((BoxPokemon *)boxMon, MON_DATA_IS_EGG, 0L) == 0) {
                u8 delay;

                sub_020729A4(trPhase->pokeDataNarc, &delay, ov71_022473BC(trPhase->sequenceData), 1);
                sub_020062E0(ov71_022473BC(trPhase->sequenceData), delay, ov71_022473C4(trPhase->sequenceData));
                Pokepic_StartAnim(trPhase->pokemonSprite, 1);
            }

            trPhase->timer = 0;
            (*subStepCounter)++;
        }
        break;
    case 4:
        if (++(trPhase->timer) > 30) {
            return 1;
        }
    }

    return 0;
}
