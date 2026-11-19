#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "options.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "overlay_71_trade_send_phase.h"
#include "overlay_71_trade3dmodel.h"
#include "overlay_71_sprite_anim_resources.h"
#include "overlay_74_main_menu_util.h"
#include "overlay_70_gtsapplication_state.h"
#include "overlay_71_trade_sequence_data.h"
#include "msgdata.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;


typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct SpriteAnimFrame SpriteAnimFrame;

typedef struct Trade3DScene Trade3DScene;

typedef u16 charcode_t;

struct MessageBankEntry {
    u32 offset;
    u32 length;
};

struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
};


struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
};

struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
};

struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
};

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

enum TradeType {
    TRADE_TYPE_NORMAL = 1,
    TRADE_TYPE_SEND_ONLY,
    TRADE_TYPE_RECEIVE_ONLY = 4,
};

enum TradeBackground {
    TRADE_BACKGROUND_DAY = 0,
    TRADE_BACKGROUND_EVENING,
    TRADE_BACKGROUND_NIGHT,
    TRADE_BACKGROUND_WIFI,
};

struct Trade3DScene {
    Camera *camera;
    VecFx32 target;
    CameraAngle cameraAngle;
    Trade3DModel *models;
    u32 modelCount;
};

s32 ov71_02248110(void *arg0);

void ov71_0224817C(void *arg0);

s32 ov71_022481C8(void *arg0);

s32 ov71_022481D8(void *arg0);

void ov71_02247704(Trade3DModel *, u32);

void ov71_022480C0(TradeSendPhase *, u32, u32, u32);

s32 ov71_02247DEC(TradeSendPhase *tsPhase, s32 *subStepCounter);

s32 ov71_02247DEC(TradeSendPhase *tsPhase, s32 *subStepCounter) {
    s32 iVar1;
    BOOL BVar2;
    s32 sVar3;

    switch (*subStepCounter) {
    case 0:
        PlaySE(0x6a7);
        ov71_022480C0(tsPhase, 0, 0x10, 0xc);
        *subStepCounter = *subStepCounter + 1;
        break;
    case 1:
        sVar3 = ov71_02248110(tsPhase);
        if (sVar3 != 0) {
            Pokepic_Delete((Pokepic *)tsPhase->pokemonSprite);
            Sprite_SetAnimCtrlSeq(tsPhase->sprites[1], 0);
            Sprite_SetDrawFlag(tsPhase->sprites[1], 1);
            *subStepCounter = *subStepCounter + 1;
        }
        break;
    case 2:
        iVar1 = Sprite_IsAnimated(tsPhase->sprites[1]);
        if (iVar1 == 0) {
            ov71_02247704(tsPhase->model, 1);
            ov71_022480C0(tsPhase, 0x10, 0, 0x10);
            ov71_0224817C(tsPhase);
            *subStepCounter = *subStepCounter + 1;
        }
        break;
    case 3:
        sVar3 = ov71_022481D8(tsPhase);
        if ((sVar3 >= 2) || (sVar3 = ov71_022481C8(tsPhase), sVar3 == 1)) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 0x18, 1, HEAP_ID_57);
            *subStepCounter = *subStepCounter + 1;
        }
        break;
    case 4:
        BVar2 = IsPaletteFadeFinished();
        if ((BVar2 != 0) && (sVar3 = ov71_022481C8(tsPhase), sVar3 != 0)) {
            return 1;
        }
    }
    return 0;
}
