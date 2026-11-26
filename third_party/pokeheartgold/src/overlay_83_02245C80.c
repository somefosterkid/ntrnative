#include "global.h"

#include "filesystem.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "unk_0200A090.h"
#include "overlay_83_battle_castle_opponent_app.h"
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
u8 ov80_02237B58(u8 challengeType, BOOL includeBothOpponents);
void ov83_02245CA8(BattleCastleOpponentApp *arg0, s32 slot);

void ov83_02245C80(BattleCastleOpponentApp *app);

void ov83_02245C80(BattleCastleOpponentApp *app) {
    u8 numMons = ov80_02237B58(app->challengeType, 1);

    for (int i = 0; i < numMons; i++) {
        ov83_02245CA8(app, (u8)i);
    }
}
