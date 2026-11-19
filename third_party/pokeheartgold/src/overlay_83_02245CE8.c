#include "global.h"

#include "filesystem.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "unk_0200A090.h"
#include "message_format.h"
#include "msgdata.h"
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

typedef struct UnkStruct_ov83_02245D08 {
    u8 unk_0[0x24];
    MessageFormat *unk_24;
    String *unk_28;
} UnkStruct_ov83_02245D08;

struct BattleCastlePersistentSave;
struct BattleCastleSave;
struct FrontierSave;
struct ListMenuCursor;
struct MessagePrinter;
struct ListMenu2D;
struct MsgData;
struct MessageFormat;
typedef struct ListMenuCursor ListMenuCursor;
typedef struct ListMenu2D ListMenu2D;
BOOL ov80_02237D8C(u8 challengeType);
void ov83_02245CE8(BattleCastleOpponentApp *app, u32 *x, u32 *y);

s32 ov83_02247998(Window *, String *, s32, s32, s32, s32, s32);
void ov83_02245D08(UnkStruct_ov83_02245D08 *arg0, void *arg1, MsgData *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);

void ov83_02245CE8(BattleCastleOpponentApp *app, u32 *x, u32 *y) {
    if (ov80_02237D8C(app->challengeType) == 1) {
        *x = 64;
    } else {
        *x = 96;
    }

    *y = 60;
}

void ov83_02245D08(UnkStruct_ov83_02245D08 *arg0, void *arg1, MsgData *msgData, s32 strno, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    String *string;

    string = NewString_ReadMsgData(msgData, strno);
    StringExpandPlaceholders(arg0->unk_24, arg0->unk_28, string);
    ov83_02247998(arg1, arg0->unk_28, arg4, arg5, arg6, arg7, arg8);
    String_Delete(string);
}
