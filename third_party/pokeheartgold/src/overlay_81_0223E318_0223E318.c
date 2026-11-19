#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "save.h"
#include "screen_fade.h"
#include "sprite.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_020379A0.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_74_main_menu_util.h"
#include "pokemon_summary_app.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "list_menu_items.h"
#include "sav_chatot.h"
#include "overlay_87_scratch_off_card_app.h"
#include "struct_defs/resdat_narc_structs.h"
#include "list_menu_2d.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

typedef struct ListMenuCursor ListMenuCursor;

typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct SpriteAnimFrame SpriteAnimFrame;

struct MessageBankEntry {
    u32 offset;
    u32 length;
};

struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
};

struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
};

typedef u32 TextColor;

struct ListMenuCursor {
    TextColor color;
    String *string;
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

s32 ov80_02237254(u8);

void ov81_02240658(BattleFactoryApp *, u32);

s32 ov81_02240F08(BattleFactoryApp *, u32);

BOOL ov81_02242DD8(BattleFactoryAppPanelSprite *panelSprite);

const VecFx32 *ov81_02242F30(BattleFactoryAppPokeballSprite *ballSprite);

void ov81_02242F60(BattleFactoryAppPokeballSprite *ballSprite);

void ov81_02242F94(BattleFactoryAppPokeballSprite *ballSprite, u32 animID);

u32 ov81_0223E318(BattleFactoryApp *app);

u32 ov81_0223E318(BattleFactoryApp *app) {
    int i;
    u32 j;

    switch (app->subState) {
    case 0:
        if (ov80_02237254(app->challengeType) == 1) {
            sub_02037BEC();
            sub_02037AC0(0xed);
        }
        app->subState++;
        break;
    case 1:
        if (ov80_02237254(app->challengeType) == 1) {
            if (sub_02037B38(0xed) == 1) {
                sub_02037BEC();
                app->subState++;
            }
        } else {
            app->subState++;
        }
        break;
    case 2:
        for (i = 0; i < app->numPokeballs; i++) {
            ov81_02242F30(app->ballSprites[i]);
            ov81_02242F60(app->ballSprites[i]);
        }
        BgSetPosTextAndCommit(app->bgConfig, 6, BG_POS_OP_SET_ROT, 0x108);
        Pokepic_SetAttr((Pokepic *)app->monSprites[0], 6, 1);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 3, HEAP_ID_100);
        app->subState++;
        break;
    case 3:
        if (IsPaletteFadeFinished() == 0) {
            break;
        }
        PlaySE(0x611);
        app->animationTimer = 0;
        app->subState++;
        break;
    case 4:
        if (ov81_022404B4(app) == 1) {
            StopSE(0x611, 0);
            PlaySE(0x678);
            for (i = 0; i < app->numPokeballs; i++) {
                ov81_02242F94(app->ballSprites[i], 4);
            }
            ov81_02240658(app, 0);
            app->wheelPaletteCounter = 8;
            app->animationTimer = 0;
            app->subState++;
        }
        break;
    case 5:
        if (ov81_02242DD8(app->unk_380) == 1) {
            break;
        }
        app->animationTimer = 0;
        app->subState++;
        break;
    case 6:
        if (app->animationTimer == 0) {
            GfGfx_EngineATogglePlanes(2, 0);
            if (ov81_02240F08(app, 0) == 1) {
                Pokepic_SetAttr((Pokepic *)app->monSprites[0], 6, 0);
                Pokepic_StartPaletteFadeAll((PokepicManager *)app->monSpriteMan, 0x10, 0, 1, 0xffff);
            } else {
                for (j = 0; j < app->numMonsToFade; j++) {
                    Pokepic_SetAttr((Pokepic *)app->monSprites[j], 6, 0);
                }
                Pokepic_StartPaletteFadeAll((PokepicManager *)app->monSpriteMan, 0x10, 0, 1, 0xffff);
            }
        }
        Pokepic_ResumePaletteFade((Pokepic *)app->monSprites[0]);
        app->animationTimer++;
        if (app->animationTimer < 2) {
            break;
        }
        app->animationTimer = 0;
        ov81_02241398(app);
        return 1;
    }
    return 0;
}
