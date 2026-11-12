// Black's code at main 0x0202e818-0x0202e848. No name is known: the module and address.

#include "types.h"
#include "field/intrude_work.h"
#include "save/player_info.h"
#include "struct_decls.h"
#include "system/comm_player_support.h"
#include "system/game_system.h"
#include "system/season.h"

struct CommPlayerSupport {
    u32 type;
    u32 usedType;
    u32 lastType;
    PlayerInfo supporter;
    PlayerInfo usedSupporter;
};

BOOL func_0202e818(CommPlayerSupport *support) {
    if ((support->usedType != 0) || (support->lastType != 0)) {
        return 1;
    }
    return 0;
}

PlayerInfo *func_0202e82c(CommPlayerSupport *support) {
    if (support->usedType != 0) {
        return &support->usedSupporter;
    }
    return &support->supporter;
}

u8 GameSystem_GetSeason(GameSystem *gsys) {
    return getSeasonFromPlayerData(GSYS_GetGameCommSystem(gsys));
}
