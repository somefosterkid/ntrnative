#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "list_menu_2d.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "unk_0200A090.h"
#include "overlay_82_battle_hall_app.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_manager.h"

struct OverlayManager;

struct BattleHallAppSprite;

struct Menu;

struct ListMenu2DTemplate;

struct MsgData;

struct SpriteList;

struct SpriteResourceCollection;

struct MessageFormat;


typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

void *OverlayManager_GetData(OverlayManager *appMan);

typedef struct BattleHallAppSprite BattleHallAppSprite;

BOOL ov80_0223792C(u8 challengeType);

typedef struct Menu Menu;

enum BattleHallAppState {
    STATE_FADE_IN = 0,
    STATE_MON_SUMMARY,
    STATE_SELECT_BATTLE,
    STATE_MULTI_CONFIRM,
    STATE_SYNC_BEFORE_EXIT,
    STATE_FADE_OUT,
    STATE_CLOSE_MESSAGE,
};

BOOL ov82_0223DFBC(BattleHallApp *app);

BOOL ov82_0223E2A4(BattleHallApp *app);

#if defined(SDK_PORT) || defined(NONMATCHING)
u32 ov82_0223E2EC(BattleHallApp *app);
#else
BOOL ov82_0223E2EC(BattleHallApp *app);
#endif

BOOL ov82_0223E5D4(BattleHallApp *app);

BOOL ov82_0223E7E8(BattleHallApp *app);

BOOL ov82_0223E820(BattleHallApp *app);

BOOL ov82_0223E888(BattleHallApp *app);

void ov82_0223F2F8(BattleHallApp *app, int *state, enum BattleHallAppState nextState);

void ov82_0223F834(BattleHallApp *app);

BOOL ov82_0223DE20(OverlayManager *appMan, int *state);

BOOL ov82_0223DE20(OverlayManager *appMan, int *state) {
    BattleHallApp *app = OverlayManager_GetData(appMan);

    if (app->selectionID != 0xff && *state == STATE_SELECT_BATTLE && !app->summaryScreenOpen) {
        ov82_0223F834(app);
        ov82_0223F2F8(app, state, STATE_MULTI_CONFIRM);
    }

    switch (*state) {
    case STATE_MON_SUMMARY:
        if (ov82_0223E2A4(app) == 1) {
            ov82_0223F2F8(app, state, STATE_FADE_IN);
        } else {
            return 0;
        }
    case STATE_FADE_IN:
        if (ov82_0223DFBC(app) == 1) {
            ov82_0223F2F8(app, state, STATE_SELECT_BATTLE);
        }
        break;
    case STATE_SELECT_BATTLE:
        if (ov82_0223E2EC(app) == 1) {
            if (app->startMultiSelectionConfirm == 1) {
                ov82_0223F2F8(app, state, STATE_MULTI_CONFIRM);
            } else if (app->summaryScreenOpened == 1) {
                ov82_0223F2F8(app, state, STATE_MON_SUMMARY);
                return 0;
            } else if (ov80_0223792C(app->challengeType) == 1) {
                ov82_0223F2F8(app, state, STATE_SYNC_BEFORE_EXIT);
            } else {
                ov82_0223F2F8(app, state, STATE_FADE_OUT);
            }
        }
        break;
    case STATE_MULTI_CONFIRM:
        if (ov82_0223E5D4(app) == 1) {
            if (app->selectionCanceled == 1) {
                app->selectionCanceled = 0;
                ov82_0223F2F8(app, state, STATE_SELECT_BATTLE);
            } else if (ov80_0223792C(app->challengeType) == 1) {
                ov82_0223F2F8(app, state, STATE_SYNC_BEFORE_EXIT);
            } else {
                ov82_0223F2F8(app, state, STATE_FADE_OUT);
            }
        }
        break;
    case STATE_SYNC_BEFORE_EXIT:
        if (ov82_0223E7E8(app) == 1) {
            ov82_0223F2F8(app, state, STATE_FADE_OUT);
        }
        break;
    case STATE_FADE_OUT:
        if (ov82_0223E820(app) == 1) {
            return 1;
        }
        break;
    case STATE_CLOSE_MESSAGE:
        if (ov82_0223E888(app) == 1) {
            ov82_0223F2F8(app, state, STATE_SYNC_BEFORE_EXIT);
        }
        break;
    }

    SpriteList_RenderAndAnimateSprites(app->spriteMan.spriteList);
    return 0;
}
