#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "font_types_def.h"
#include "gf_3d_loader.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "render_text.h"
#include "seal_case.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "text.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02020A0C.h"
#include "battle/battle.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "battle/battle_party_context.h"
#include "battle/battle_cursor.h"

struct BattleSystem;







u8 BattleSystem_GetTextFrameDelay(BattleSystem *battleSys);

enum BattlePartyMessageBox {
    BATTLE_PARTY_MESSAGE_BOX_SELECT_OPTION = 0,
    BATTLE_PARTY_MESSAGE_BOX_ERROR,
};

void ov08_022201E8(BattleParty *battleParty);

void ov08_022201E8(BattleParty *battleParty) {
    TextFlags_SetCanABSpeedUpPrint(TRUE);
    battleParty->textPrinterID = AddTextPrinterParameterized(&battleParty->messageBoxWindows[BATTLE_PARTY_MESSAGE_BOX_ERROR], FONT_MESSAGE, battleParty->string, 0, 0, BattleSystem_GetTextFrameDelay(battleParty->context->battleSys), NULL);
}
