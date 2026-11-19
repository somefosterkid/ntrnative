#include "global.h"

#include "constants/heap.h"

#include "battle/battle.h"
#include "battle/battle_cursor.h"

#include "bg_window.h"
#include "filesystem.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02020A0C.h"
#include "wild_encounter.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "battle/battle_party_context.h"


void ov08_02221294(BattleParty *battleParty);

void ov08_02221328(BattleParty *battleParty);

void ov08_022213C8(BattleParty *battleParty);

void ov08_022213C8(BattleParty *battleParty) {
    if (battleParty->displayingContestStats == FALSE) {
        ov08_02221294(battleParty);
        return;
    }
    ov08_02221328(battleParty);
}
