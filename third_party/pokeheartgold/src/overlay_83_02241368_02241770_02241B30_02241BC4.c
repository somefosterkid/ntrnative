#include "global.h"

#include "battle_castle_app_sprite_manager.h"
#include "options.h"
#include "overlay_80_02237A70.h"
#include "player_avatar.h"
#include "save_frontier.h"
#include "unk_02030A98.h"
#include "unk_02035900.h"
#include "unk_0205BFF0.h"
#include "overlay_41_sprite_resource_collection.h"
#include "battle_castle_app_sprite.h"
#include "overlay_83_battle_castle_self_app.h"
#include "battle_castle_save.h"
#include "message_printer.h"
#include "font.h"
#include "string_util.h"
#include "struct_defs/resdat_narc_structs.h"
#include "gf_gfx_planes.h"

struct BattleCastleSelfApp;

struct ListMenuCursor;

struct ListMenu2D;

struct SaveData;

struct Window;

u8 ov84_0223EC88(u8 exitSlot, u8 slotID);

u8 ov83_0224777C(SaveData *saveData, u8 challengeType, u8 rankType);

void BattleCastleApp_SetPartnerName(MessageFormat *template, u32 idx);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov83_02247944(Window *window, u8 frame);
#else
void ov83_02247944(Window *window, int frame);
#endif

enum TextAlignment {
    TEXT_ALIGN_LEFT,
    TEXT_ALIGN_CENTER,
    TEXT_ALIGN_RIGHT
};

typedef struct ListMenuCursor ListMenuCursor;

typedef struct ListMenu2D ListMenu2D;

void ov80_02237FA4(FrontierSave *frontier, u8 challengeType, int castlePoints);

enum BattleCastleAppState {
    STATE_FADE_IN = 0,
    STATE_MAIN_FLOW,
    STATE_SYNC_PURCHASE,
    STATE_SYNC_BEFORE_EXIT,
    STATE_FADE_OUT
};

u8 PrintMessageAndCopyToVRAM(BattleCastleSelfApp *app, int entryID, u8 font);

void SetStringTemplatePlayerName(BattleCastleSelfApp *app, u32 idx);

void CloseYesNoMenu(BattleCastleSelfApp *app);

void PrintPlayersAndPartnersCastlePoints(BattleCastleSelfApp *app, Window *window);

extern const u16 ov83_02247D48[3][3];

extern const u16 ov83_02247D5A[3][3];

void LoadMainBackground(BattleCastleSelfApp *app, enum GFBgLayer bgLayer);

void LoadSummaryBackground(BattleCastleSelfApp *app, enum GFBgLayer bgLayer);

void LoadMovesListBackground(BattleCastleSelfApp *app, enum GFBgLayer bgLayer);

void LoadItemSelectBackground(BattleCastleSelfApp *app, enum GFBgLayer bgLayer);

void LoadSubScreenBackground(BattleCastleSelfApp *app, enum GFBgLayer bgLayer);


u8 PrintMessage(BattleCastleSelfApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font, enum TextAlignment alignment);

void PrintStaticMessage(BattleCastleSelfApp *app, Window *window, int entryID, u16 xOffset, u16 yOffset, enum TextAlignment alignment);


void ChangeState(BattleCastleSelfApp *app, int *state, enum BattleCastleAppState nextState);

void ov83_02241BC4(BattleCastleSelfApp *app, u8 slot, u8 menuOption);

u32 ov83_0224776C(u32 exitSlot, u32 slotID);

void ov83_02240C6C(BattleCastleSelfApp *app, u32 idx);

void ov83_022477C4(MessageFormat *template, u32 idx);

void ov83_02241730(BattleCastleSelfApp *app);

void ov83_02241770(BattleCastleSelfApp *app, Window *window);

u8 ov83_0223FD14(BattleCastleSelfApp *app, int entryID, u8 font);

void ov83_02241BC4(BattleCastleSelfApp *app, u8 slot, u8 menuOption) {
    u8 unused = ov80_02237B24(app->challengeType, 0);

    u8 rankType;
    if (menuOption == 4) {
        rankType = 0;
    } else {
        rankType = 1;
    }

    u8 exitSlot = app->exitSlot;
    u8 unused2 = ov83_0224776C(exitSlot, slot);

    u32 rank;
    if (sub_0203769C() == 0) {
        if (slot < exitSlot) {
            ov83_02240C6C(app, 5);

            rank = ov83_0224777C(app->saveData, app->challengeType, rankType);
            ov80_02237FA4(app->frontier, app->challengeType, ov83_02247D48[rankType][rank]);

            rank = ov83_0224777C(app->saveData, app->challengeType, rankType);
            sub_02031108(Save_Frontier_GetStatic(app->saveData), sub_0205C174(app->challengeType, rankType), sub_0205C268(sub_0205C174(app->challengeType, rankType)), rank + 1);

            if (ov80_02237D8C(app->challengeType) == 1) {
                if (menuOption == 4) {
                    app->syncIncreaseRank = 0 + 1;
                } else {
                    app->syncIncreaseRank = 1 + 1;
                }
            }
        } else {
            ov83_022477C4(app->strTemplate, 5);
            rank = app->partnersRanks[rankType];
            app->partnersCP -= ov83_02247D48[rankType][rank];
            app->partnersRanks[rankType]++;
        }
    } else {
        if (slot < exitSlot) {
            ov83_022477C4(app->strTemplate, 5);
            rank = app->partnersRanks[rankType];
            app->partnersCP -= ov83_02247D48[rankType][rank];
            app->partnersRanks[rankType]++;
        } else {
            ov83_02240C6C(app, 5);
            rank = ov83_0224777C(app->saveData, app->challengeType, rankType);

            ov80_02237FA4(app->frontier, app->challengeType, ov83_02247D48[rankType][rank]);
            rank = ov83_0224777C(app->saveData, app->challengeType, rankType);

            sub_02031108(Save_Frontier_GetStatic(app->saveData), sub_0205C174(app->challengeType, rankType), sub_0205C268(sub_0205C174(app->challengeType, rankType)), rank + 1);

            if (ov80_02237D8C(app->challengeType) == 1) {
                if (menuOption == 4) {
                    app->syncIncreaseRank = 0 + 1;
                } else {
                    app->syncIncreaseRank = 1 + 1;
                }
            }
        }
    }

    ov83_02241730(app);
    ov83_02241770(app, &app->windows[SELF_APP_WINDOW_HEADER]);
    ov83_02247944(&app->windows[SELF_APP_WINDOW_MSG_BOX], Options_GetFrame(app->options));

    app->printerID = ov83_0223FD14(app, ov83_02247D5A[rankType][rank], FONT_MESSAGE);
}
