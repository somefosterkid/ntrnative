#include "global.h"

#include "overlay_82.h"
#include "system.h"
#include "bg_window.h"
#include "filesystem.h"
#include "list_menu_2d.h"
#include "options.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "unk_0200A090.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "pokemon_summary_app.h"
#include "overlay_82_battle_hall_app.h"
#include "message_printer.h"


struct OverlayManager;
struct BattleHallAppSprite;
struct MessagePrinter;
struct Menu;
struct ListMenu2DTemplate;
struct MsgData;
struct PokemonSummary;
struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
typedef struct BattleHallAppSpriteManager {
    SpriteList *spriteList;
    G2dRenderer renderer;
    SpriteResourceCollection *resourceCollection[4];
    SpriteResource *resources[2][4];
} BattleHallAppSpriteManager;
typedef struct BattleHallAppSprite BattleHallAppSprite;
typedef struct Menu Menu;
u16 ov82_0223F558(BattleHallApp *app);
u16 ov82_0223F570(BattleHallApp *app);

s32 ov82_0223F53C(void *arg0) {
    if (gSystem.newKeys & 1) {
        *((s8 *)&((BattleHallApp *)arg0)->pad_90[0]) = 0;
        return 1;
    }
    return 0;
}

u16 ov82_0223F558(BattleHallApp *app) {
    return (app->cursorPos % 4) * 64 + 32;
}

u16 ov82_0223F570(BattleHallApp *app) {
    return (app->cursorPos / 4) * 36 + 16;
}
