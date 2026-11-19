#include "global.h"

#include "filesystem.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "unk_0200A090.h"
#include "overlay_83_battle_castle_self_app.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "battle_castle_app_sprite.h"
#include "bg_window.h"
#include "list_menu.h"
#include "struct_defs/resdat_narc_structs.h"
#include "overlay_manager.h"
#include "battle_castle_save.h"
#include "save_frontier.h"
#include "message_printer.h"

struct BattleCastlePersistentSave;
struct BattleCastleSave;
struct FrontierSave;
struct ListMenuCursor;
struct MessagePrinter;
struct ListMenu2D;
typedef struct ListMenuCursor ListMenuCursor;
typedef struct ListMenu2D ListMenu2D;
BOOL ov80_02237D8C(u8 challengeType);
void ov83_02244DA0(BattleCastleSelfApp *app, u32 *x, u32 *y, u8 slot);

void ov83_02244DA0(BattleCastleSelfApp *app, u32 *x, u32 *y, u8 slot) {
    if (ov80_02237D8C(app->challengeType) == 1) {
        if (slot == 0) {
            *x = 40;
        } else if (slot == 1) {
            *x = 104;
        } else if (slot == 2) {
            *x = 168;
        } else {
            *x = 232;
        }
    } else {
        if (slot == 0) {
            *x = 72;
        } else if (slot == 1) {
            *x = 136;
        } else {
            *x = 200;
        }
    }

    *y = 88;
}
