#include "global.h"

#include "battle_castle_app_sprite_manager.h"
#include "overlay_83_battle_castle_self_app.h"
#include "pm_string.h"
#include "save.h"
#include "options.h"
#include "seal_case.h"
#include "pokemon_types_def.h"
#include "palette.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "battle_castle_app_sprite.h"
#include "list_menu.h"
#include "overlay_manager.h"
#include "battle_castle_save.h"
#include "save_frontier.h"
#include "message_printer.h"
#include "struct_defs/resdat_narc_structs.h"
#include "gf_gfx_planes.h"

struct BallSeal;

struct BattleCastleAppSprite;

struct BattleCastleSelfApp;

struct ListMenuCursor;

struct ListMenu2D;

struct ListMenu2DTemplate;

struct Party;

struct Pokemon;

struct ListMenuItem;

struct Window;

typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;

Pokemon *Party_GetMonByIndex(const Party *party, int slot);

u32 GetMonData(Pokemon *mon, int param, void *dest);


BattleCastleAppSprite *BattleCastleAppSprite_New(BattleCastleAppSpriteManager *spriteMan, u32 charResourceID, u32 plttResourceID, u32 cellResourceID, u32 animID, s16 x, s16 y, int priority, const u8 *unused);

void *ov83_0224753C(BattleCastleAppSprite *sprite);

void BattleCastleAppSprite_SetAnim(BattleCastleAppSprite *sprite, u32 animID);

void BattleCastleAppSprite_UpdateMonSpriteAnim(BattleCastleAppSprite *sprite, u8 animID);

BOOL BattleCastleAppSprite_IsAnimated(BattleCastleAppSprite *sprite);

enum TextAlignment {
    TEXT_ALIGN_LEFT,
    TEXT_ALIGN_CENTER,
    TEXT_ALIGN_RIGHT
};

typedef struct ListMenuCursor ListMenuCursor;

typedef struct ListMenu2DTemplate {
    ListMenuItem *choices;
    Window *window;
    u8 fontID;
    u8 xSize;
    u8 ySize;
    u8 lineSpacing : 4;
    u8 suppressCursor : 2;
    u8 loopAround : 2;
} ListMenu2DTemplate;

typedef struct ListMenu2D ListMenu2D;

BOOL ov80_02237D8C(u8 challengeType);

u8 CalculateHpBarColor(u16 curHP, u16 maxHP, u32 barSize);

enum BattleCastleAppState {
    STATE_FADE_IN = 0,
    STATE_MAIN_FLOW,
    STATE_SYNC_PURCHASE,
    STATE_SYNC_BEFORE_EXIT,
    STATE_FADE_OUT
};

void ov83_02240238(BattleCastleSelfApp *app, Window *window, s32 slot, u32 hp);

u32 ov83_022411B0(BattleCastleSelfApp *app, u8 barColor);

u32 ov83_022411DC(BattleCastleSelfApp *app, u8 barColor);

void LoadMainBackground(BattleCastleSelfApp *app, enum GFBgLayer bgLayer);

void LoadSummaryBackground(BattleCastleSelfApp *app, enum GFBgLayer bgLayer);

void LoadMovesListBackground(BattleCastleSelfApp *app, enum GFBgLayer bgLayer);

void LoadItemSelectBackground(BattleCastleSelfApp *app, enum GFBgLayer bgLayer);

void LoadSubScreenBackground(BattleCastleSelfApp *app, enum GFBgLayer bgLayer);


u8 PrintMessage(BattleCastleSelfApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font, enum TextAlignment alignment);

void PrintStaticMessage(BattleCastleSelfApp *app, Window *window, int entryID, u16 xOffset, u16 yOffset, enum TextAlignment alignment);

void ChangeState(BattleCastleSelfApp *app, int *state, enum BattleCastleAppState nextState);

BOOL ov83_02240FAC(BattleCastleSelfApp *app, u8 selectedSlot, u8 menuOption);

u8 ov83_02247768(u8 numSlots, u8 slot);

BattleCastleAppSprite *ov83_02247454(BattleCastleAppSpriteManager *spriteMan, u32 charResourceID, u32 plttResourceID, u32 cellResourceID, u32 animID, s16 x, s16 y, int priority, const u8 *unused);

BOOL ov83_02247624(BattleCastleAppSprite *sprite);

void ov83_02241E18(BattleCastleSelfApp *app);

void ov83_022421E0(BattleCastleSelfApp *app, s32 animID);

void ov83_022475D4(BattleCastleAppSprite *sprite, u32 animID);

void ov83_02247600(BattleCastleAppSprite *, u8);

BOOL ov83_02240FAC(BattleCastleSelfApp *app, u8 selectedSlot, u8 menuOption) {
    u32 hp, maxHp, monAnimID, barAnimID;

    u8 slot = ov83_02247768(app->numSlots, selectedSlot);
    Pokemon *mon = Party_GetMonByIndex(app->party, slot);
    hp = GetMonData(mon, MON_DATA_HP, 0L);
    maxHp = GetMonData(mon, MON_DATA_MAX_HP, 0L);
    monAnimID = ov83_022411B0(app, CalculateHpBarColor(hp, maxHp, 48));
    barAnimID = ov83_022411DC(app, CalculateHpBarColor(hp, maxHp, 48));

    switch (menuOption) {
    case 4:
    case 8:
        app->monSparklesVisible = 0;
        return 1;
    case 1:
    case 2:
    case 3:
        if (app->monSparklesVisible == 0) {
            app->monSparklesVisible = 1;

            u16 xOffset;
            if (ov80_02237D8C(app->challengeType) == 0) {
                xOffset = 64;
            } else {
                xOffset = 32;
            }

            app->monSparklesSprite = ov83_02247454(&app->spriteMan, 0, 0, 0, 8, xOffset + (64 * slot), 62, 0, 0L);
        }

        if (app->monSparklesSprite != 0L) {
            if (!ov83_02247624(app->monSparklesSprite)) {
                ov83_0224753C(app->monSparklesSprite);
                app->monSparklesSprite = 0L;
            }
        }

        if (app->monSparklesSprite == 0L) {
            ov83_02240238(app, &app->windows[SELF_APP_WINDOW_HP_BARS], slot, GetMonData(mon, MON_DATA_HP, 0L));
            if (app->selectedMonSlot == slot) {
                ov83_02241E18(app);
                ov83_022421E0(app, 0);
            }
            ov83_022475D4(app->barSprites[slot], barAnimID);
            ov83_02247600(app->monIconSprites[slot], (u8)monAnimID);
            app->monSparklesVisible = 0;
            return 1;
        }
        break;
    case 6:
    case 7:
        if (app->monSparklesVisible == 0) {
            app->monSparklesVisible = 1;

            u16 xOffset;
            if (ov80_02237D8C(app->challengeType) == 0) {
                xOffset = 64;
            } else {
                xOffset = 32;
            }

            app->monSparklesSprite = ov83_02247454(&app->spriteMan, 0, 0, 0, 16, xOffset + (64 * slot), 62, 0, 0L);
        }

        if (!ov83_02247624(app->monSparklesSprite)) {
            ov83_0224753C(app->monSparklesSprite);
            app->monSparklesSprite = 0L;
            app->monSparklesVisible = 0;
            return 1;
        }
        break;
    case 9:
        app->monSparklesVisible = 0;
        return 1;
        break;
    case 10:
        app->monSparklesVisible = 0;
        return 1;
        break;
    }

    return 0;
}
