// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/app_menu_common.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/battle.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "nnsys/g2d.h"
#include "pml/poke_party.h"
#include "system/app_menu_common.h"

// The graphics that the game's menus share: the archive's id, and the files of its icons. The file's name is a
// guess: the ROM has no string for it

// The menu bar's screen, and where it sits in a 32-tile-wide screen: its bottom three rows
#define BAR_SCREEN_FILE 0x1d
#define BAR_SCREEN_START (21 * 32)
#define BAR_SCREEN_TILES (3 * 32)

// The palette within the type icons' palette file of each type and contest category
// pokebw2's sTypeIconPalettes
static const u8 data_0209fcf8[] = {
    0, 0, 1, 1, 0, 0, 2, 1, 0, 0, 1, 2, 0, 1, 1, 2, 0, 0, 1, 1, 2, 0,
};

// pokebw2's sIconPalettes
static const u8 data_0209fcec[] = {
    0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1,
};

u32 func_020275e8(void) {
    return 0x21;
}

u8 func_020275ec(u8 type) {
    return data_0209fcf8[type];
}

u32 func_020275f8(u8 type) {
    return 0x22 + type;
}

u32 func_020275fc(u32 mapping) {
    return 0x3b + mapping;
}

u32 func_02027600(u32 mapping) {
    return 0x3e + mapping;
}

u8 func_02027604(u8 index) {
    return data_0209fcec[index];
}

u32 func_02027610(u8 index) {
    return 0x38 + index;
}

u32 func_02027614(void) {
    return 0x13;
}

u32 func_02027618(void) {
    return 0x14;
}

u32 func_0202761c(u32 mapping) {
    return 0x15 + mapping;
}

u32 func_02027620(u32 mapping) {
    return 0x18 + mapping;
}

u32 func_02027624(void) {
    return 0x1b;
}

u32 func_02027628(void) {
    return 0x1c;
}

u32 func_0202762c(void) {
    return BAR_SCREEN_FILE;
}

u32 func_02027630(void) {
    return 0x1e;
}

// pokebw2's AppMenuCommon_LoadBarScreen
void func_02027634(ArcTool *arc, u8 bg, HeapID heapId, u32 charBase, u32 palette) {
    u16 *dest = GFL_BGSysIsScrHeapExists(bg);
    NNSG2dScreenData *screen;
    void *file = GFL_G2DIOReadNSCRArc(arc, BAR_SCREEN_FILE, FALSE, &screen, heapId);
    int i;

    MI_CpuCopy8((u16 *)screen->rawData + BAR_SCREEN_START, dest + BAR_SCREEN_START, BAR_SCREEN_TILES * sizeof(u16));
    GFL_HeapFree(file);

    for (i = 0; i < BAR_SCREEN_TILES; i++) {
        dest[BAR_SCREEN_START + i] = (dest[BAR_SCREEN_START + i] & 0xfff) + charBase + (palette << 12);
    }
}

u32 func_02027694(void) {
    return 0x42;
}

u32 func_02027698(void) {
    return 0x41;
}

u32 func_0202769c(u32 mapping) {
    return 0x43 + mapping;
}

u32 func_020276a0(u32 mapping) {
    return 0x46 + mapping;
}

u32 func_020276a4(void) {
    return 0x49;
}

u32 func_020276a8(void) {
    return 0x4a;
}

u32 func_020276ac(u32 mapping) {
    return 0x4b + mapping;
}

u32 func_020276b0(u32 mapping) {
    return 0x4e + mapping;
}

u32 func_020276b4(void) {
    return 0xb;
}

u32 func_020276b8(void) {
    return 0xc;
}

u32 func_020276bc(u32 mapping) {
    return 0xd + mapping;
}

u32 func_020276c0(u32 mapping) {
    return 0x10 + mapping;
}

// pokebw2's AppMenuCommon_GetStatusIcon
u32 func_020276c4(PartyPkm *pkm) {
    u32 status;

    if (PokeParty_GetParam(pkm, PKM_PARAM_HP, NULL) == 0) {
        return APP_STATUS_ICON_FAINTED;
    }

    status = PokeParty_GetParam(pkm, PKM_PARAM_STATUS, NULL);
    if (status == CONDITION_PARALYSIS) {
        return APP_STATUS_ICON_PARALYSIS;
    } else if (status == CONDITION_SLEEP) {
        return APP_STATUS_ICON_SLEEP;
    } else if (status == CONDITION_FREEZE) {
        return APP_STATUS_ICON_FREEZE;
    } else if (status == CONDITION_BURN) {
        return APP_STATUS_ICON_BURN;
    } else if (status == CONDITION_POISON) {
        return APP_STATUS_ICON_POISON;
    }
    return APP_STATUS_ICON_NONE;
}

u32 func_02027710(u32 mapping) {
    return 0x51 + mapping;
}

u32 func_02027714(u32 mapping) {
    return 0x54 + mapping;
}

u32 func_02027718(u32 mapping) {
    return 0xa6 + mapping;
}

u32 func_0202771c(u32 mapping) {
    return 0xa9 + mapping;
}

u32 func_02027720(u32 ball) {
    if (ball == 0) {
        ball = 4;
    }
    return 0x56 + ball;
}

u32 func_0202772c(u32 ball) {
    if (ball == 0) {
        ball = 4;
    }
    return 0x6f + ball;
}

u32 func_02027738(u32 ball, u32 mapping) {
    return 0x89 + mapping;
}

u32 func_02027740(u32 ball, u32 mapping) {
    return 0x8c + mapping;
}

u32 func_02027748(void) {
    return 0x8f;
}

u32 func_0202774c(u32 mapping) {
    return 0x90 + mapping;
}

u32 func_02027750(u32 mapping) {
    return 0x93 + mapping;
}

u32 func_02027754(u32 mapping) {
    return 0x96 + mapping;
}

u32 func_02027758(void) {
    return 0;
}

u32 func_0202775c(u32 mapping) {
    return 2 + mapping;
}

u32 func_02027760(u32 mapping) {
    return 5 + mapping;
}

u32 func_02027764(u32 mapping) {
    return 8 + mapping;
}

u32 func_02027768(void) {
    return 0x99;
}

u32 func_0202776c(u32 mapping) {
    return 0x9a + mapping;
}

u32 func_02027770(u32 mapping) {
    return 0x9d + mapping;
}

u32 func_02027774(u32 mapping) {
    return 0xa0 + mapping;
}
