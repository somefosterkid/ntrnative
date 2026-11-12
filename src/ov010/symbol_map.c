// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/symbol_map.c, ported by tools/port_file.py.
#include "types.h"
#include "field/symbol_map.h"
#include "field/symbol_save_field.h"
#include "field/intrude_comm.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "save/save_control.h"
#include "system/game_data.h"
#include "system/game_system.h"

#define SYMBOL_MAP_WIDTH 3
#define SYMBOL_MAP_SIZE 30

// The areas of the forest as it grows, by their IDs from 1, three to a row; 0 is no area. By how far the forest has
// grown and by the season of its trees
// pokebw2's SYMBOL_MAP_0
static const u8 data_ov010_02171ed6[SYMBOL_MAP_SIZE] = {
#include "asset/symbol_map_data_ov010_02171ed6.inc"
};

// pokebw2's SYMBOL_MAP_1
static const u8 data_ov010_02171eb8[SYMBOL_MAP_SIZE] = {
#include "asset/symbol_map_data_ov010_02171eb8.inc"
};

// pokebw2's SYMBOL_MAP_2
static const u8 data_ov010_02171e9a[SYMBOL_MAP_SIZE] = {
#include "asset/symbol_map_data_ov010_02171e9a.inc"
};

// pokebw2's SYMBOL_MAP_3
static const u8 data_ov010_02171e7c[SYMBOL_MAP_SIZE] = {
#include "asset/symbol_map_data_ov010_02171e7c.inc"
};

// pokebw2's SYMBOL_MAP_4
static const u8 data_ov010_02171e5e[SYMBOL_MAP_SIZE] = {
#include "asset/symbol_map_data_ov010_02171e5e.inc"
};

// pokebw2's SYMBOL_MAP_5
static const u8 data_ov010_02171e40[SYMBOL_MAP_SIZE] = {
#include "asset/symbol_map_data_ov010_02171e40.inc"
};

// pokebw2's SYMBOL_MAP_6
static const u8 data_ov010_02171e22[SYMBOL_MAP_SIZE] = {
#include "asset/symbol_map_data_ov010_02171e22.inc"
};

// pokebw2's SYMBOL_MAP_7
static const u8 data_ov010_02171e04[SYMBOL_MAP_SIZE] = {
#include "asset/symbol_map_data_ov010_02171e04.inc"
};

// pokebw2's SYMBOL_MAP_8
static const u8 data_ov010_02171de6[SYMBOL_MAP_SIZE] = {
#include "asset/symbol_map_data_ov010_02171de6.inc"
};

// pokebw2's SYMBOL_MAP_9
static const u8 data_ov010_02171dc8[SYMBOL_MAP_SIZE] = {
#include "asset/symbol_map_data_ov010_02171dc8.inc"
};

// pokebw2's SYMBOL_MAP_10
static const u8 data_ov010_02171daa[SYMBOL_MAP_SIZE] = {
#include "asset/symbol_map_data_ov010_02171daa.inc"
};

// pokebw2's SYMBOL_MAP_11
static const u8 data_ov010_02171d8c[SYMBOL_MAP_SIZE] = {
#include "asset/symbol_map_data_ov010_02171d8c.inc"
};

// pokebw2's SYMBOL_MAP_12
static const u8 data_ov010_02171d6e[SYMBOL_MAP_SIZE] = {
#include "asset/symbol_map_data_ov010_02171d6e.inc"
};

// pokebw2's SYMBOL_MAP_13
static const u8 data_ov010_02171d50[SYMBOL_MAP_SIZE] = {
#include "asset/symbol_map_data_ov010_02171d50.inc"
};

// pokebw2's SYMBOL_MAP_TABLE
static const u8 *const data_ov010_02171ef4[14] = {
    data_ov010_02171ed6,
    data_ov010_02171eb8,
    data_ov010_02171e9a,
    data_ov010_02171e7c,
    data_ov010_02171e5e,
    data_ov010_02171e40,
    data_ov010_02171e22,
    data_ov010_02171e04,
    data_ov010_02171de6,
    data_ov010_02171dc8,
    data_ov010_02171daa,
    data_ov010_02171d8c,
    data_ov010_02171d6e,
    data_ov010_02171d50,
};

static u8 func_ov010_0216630c(const u8 *map, u8 area);
static u16 func_ov010_02166324(const u8 *map, u8 area);
static u32 func_ov010_021663f4(const u8 *map, u8 area, u8 dir);
static BOOL func_ov010_02166434(GameSystem *gsys, u32 *a, u32 *b);
static const u8 *func_ov010_0216649c(GameSystem *gsys);

BOOL func_ov010_0216628c(AreaNPCSave *npcData, u32 index) {
    u32 kind;
    u32 slot;

    switch (func_0200e184(index)) {
    case 0:
        kind = 2;
        break;
    case 1:
        kind = 3;
        break;
    case 2:
        kind = 0;
        break;
    case 3:
        kind = 1;
        break;
    default:
        return FALSE;
    }
    slot = func_0200e164(npcData, kind);
    if (slot == 0xffff) {
        return FALSE;
    }
    func_0200e06c(npcData);
    npcData->pokemon[slot] = npcData->pokemon[index];
    if (kind == 3) {
        func_0200e27c(npcData, slot);
    }
    decryptNpcData(npcData);
    func_0200e18c(npcData, index);
    return TRUE;
}

static u8 func_ov010_0216630c(const u8 *map, u8 area) {
    int i;

    for (i = 0; i < SYMBOL_MAP_SIZE; i++) {
        if (area == map[i]) {
            return i;
        }
    }
    return 1;
}

static u16 func_ov010_02166324(const u8 *map, u8 area) {
    int pos = func_ov010_0216630c(map, area);
    BOOL left = FALSE;
    BOOL right;
    BOOL up;
    BOOL down;

    if (pos % SYMBOL_MAP_WIDTH != 0 && pos != 1) {
        left = TRUE;
    }
    right = FALSE;
    if (pos % SYMBOL_MAP_WIDTH != 2 && pos != 1) {
        right = TRUE;
    }
    up = FALSE;
    if (pos - SYMBOL_MAP_WIDTH >= 0 && map[pos - SYMBOL_MAP_WIDTH] != 0) {
        up = TRUE;
    }
    down = FALSE;
    if (pos + SYMBOL_MAP_WIDTH < SYMBOL_MAP_SIZE && map[pos + SYMBOL_MAP_WIDTH] != 0) {
        down = TRUE;
    }
    if (area == 1) {
        return 0x119;
    }
    if (area == 5) {
        return 0x118;
    }
    if (left && right) {
        return 0x11d;
    }
    if (left && !right) {
        if (up && down) {
            return 0x11e;
        }
        if (up) {
            return 0x120;
        }
        if (down) {
            return 0x11b;
        }
    }
    if (!left && right) {
        if (up && down) {
            return 0x11c;
        }
        if (up) {
            return 0x11f;
        }
        if (down) {
            return 0x11a;
        }
    }
    return 0x119;
}

static u32 func_ov010_021663f4(const u8 *map, u8 area, u8 dir) {
    u8 pos = func_ov010_0216630c(map, area);

    switch (dir) {
    case 0:
        return map[pos - SYMBOL_MAP_WIDTH];
    case 1:
        return map[pos + SYMBOL_MAP_WIDTH];
    case 2:
        return map[pos - 1];
    case 3:
        return map[pos + 1];
    }
    return area;
}

static BOOL func_ov010_02166434(GameSystem *gsys, u32 *a, u32 *b) {
    GameData *gameData = GSYS_GetGameData(gsys);
    AreaNPCSave *npcData = getAreaNPCData(GameData_GetSaveControl(gameData));
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);

    if (func_ov011_021767a0(commSys, gameData) == 4) {
        *a = func_ov010_021673c4(npcData);
        *b = func_ov010_021673a8(npcData);
        return TRUE;
    }
    IntrudeComm *work = func_ov010_02158344(commSys);
    if (work == NULL) {
        *a = 0;
        *b = 0;
        return FALSE;
    }
    {
        SymbolMapList *ov011 = func_ov011_02176880(work);
        *a = ov011->unk50_6;
        *b = ov011->unk50_10;
    }
    return TRUE;
}


static const u8 *func_ov010_0216649c(GameSystem *gsys) {
    u32 a;
    u32 b;

    if (func_ov010_02166434(gsys, &a, &b) == FALSE) {
        return NULL;
    }
    return data_ov010_02171ef4[b * 7 + a];
}

SymbolMapList *func_ov010_021664cc(HeapID heapId, GameSystem *gsys, u32 *count) {
    GameData *gameData = GSYS_GetGameData(gsys);
    AreaNPCSave *npcData = getAreaNPCData(GameData_GetSaveControl(gameData));
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    SymbolMapList *list;
    u8 found;

    if (func_ov011_021767a0(commSys, gameData) == 4) {
        u32 season = GameData_Is30FPSMode(gameData);
        list = GFL_HeapAllocate(heapId, sizeof(SymbolMapList), TRUE, "symbol_map.c", 457);
        *count = func_ov010_0216749c(npcData, list->pokemon, 20, season, &found);
        list->count = found;
        list->unk50_6 = func_ov010_021673c4(npcData);
        list->unk50_10 = func_ov010_021673a8(npcData);
        list->unk50_14 = 4;
        list->unk50_18 = season;
        return list;
    }
    IntrudeComm *work = func_ov010_02158344(commSys);
    if (work == NULL) {
        return NULL;
    }
    list = GFL_HeapAllocate(heapId, sizeof(SymbolMapList), TRUE, "symbol_map.c", 476);
    MI_CpuCopy8(func_ov011_02176880(work), list, sizeof(SymbolMapList));
    *count = 530;
    return list;
}




u16 func_ov010_021665d0(GameSystem *gsys, u8 area) {
    const u8 *map = func_ov010_0216649c(gsys);

    if (map != NULL) {
        return func_ov010_02166324(map, area + 1);
    }
    return 0x118;
}

u32 func_ov010_021665f0(GameSystem *gsys, u8 area, u32 dir) {
    const u8 *map = func_ov010_0216649c(gsys);
    u8 next;

    if (map != NULL) {
        next = func_ov010_021663f4(map, area + 1, dir);
        return next - 1;
    }
    return 4;
}

BOOL func_ov010_02166618(u32 area) {
    u8 id = area + 1;

    if (id == 1) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov010_0216662c(u32 area) {
    u8 id = area + 1;

    if (id == 5) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov010_02166640(u32 area) {
    u8 id = area + 1;

    if (id == 2 || id == 3 || id == 4) {
        return TRUE;
    }
    return FALSE;
}
