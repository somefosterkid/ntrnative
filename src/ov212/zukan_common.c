// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov302/zukan_common.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/zukan_common.h"
#include "app/zukan_search_engine.h"
#include "constants/species.h"
#include "gfl/arc.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "save/event_work.h"
#include "save/pokedex.h"
#include "save/save_control.h"
#include "system/game_data.h"
#include "system/wipe.h"

// What the Pokédex's overlays share: the list of the species

u32 func_ov212_021c9244(ZukanCommonWork *wk) {
    ZukanCommonList *list;
    u32 result;
    PokeDexSave *pokedex;

    pokedex = GameData_GetPokedex(wk->param->gameData);
    list = wk->subParam;
    func_0200ca78(pokedex, list->species[list->index]);
    switch (list->state) {
    case 0:
        wk->param->result = 1;
        result = 9;
        break;
    case 1:
        wk->param->result = 0;
        result = 3;
        break;
    }
    GFL_HeapFree(wk->subParam);
    return result;
}

u32 func_ov212_021c9288(PokeDexSave *pokedex, u16 **list, HeapID heapId) {
    u16 *species;
    u32 last;
    u32 i;
    u32 count;

    if (func_0200ca70(pokedex) == 1) {
        count = func_ov212_021c976c(FALSE, heapId, &species);
    } else {
        count = func_ov212_021c976c(TRUE, heapId, &species);
    }
    last = 0;
    for (i = 0; i < count; i++) {
        if (PokeDex_IsCaught(pokedex, species[i]) == TRUE || PokeDex_IsSeen(pokedex, species[i]) == TRUE) {
            last = (u16)(i + 1);
        }
    }
    *list = species;
    return last;
}

// pokebw2's ZukanCommon_SelectSeen
void func_ov212_021c92ec(PokeDexSave *pokedex, const u16 *list, u16 count) {
    u16 first = 0xffff;
    u16 current = func_0200ca8c(pokedex);
    u16 i;

    for (i = 0; i < count; i++) {
        if (PokeDex_IsSeen(pokedex, list[i]) == 1) {
            if (current == list[i]) {
                return;
            }
            if (first == 0xffff) {
                first = list[i];
            }
        }
    }
    if (first != 0xffff) {
        func_0200ca78(pokedex, first);
    }
}

// pokebw2's ZukanCommon_IsFirstShown
BOOL func_ov212_021c9348(PokeDexSave *pokedex) {
    if (func_0200ca70(pokedex) == 1) {
        return TRUE;
    }
    if (PokeDex_IsCaught(pokedex, SPECIES_VICTINI) == 1 || PokeDex_IsSeen(pokedex, SPECIES_VICTINI) == 1) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's ZukanCommon_InitCriteria
void func_ov212_021c9380(PokeDexSave *pokedex, ZukanSearchCriteria *criteria) {
    if (func_0200ca70(pokedex) == 1) {
        criteria->regional = 0;
    } else {
        criteria->regional = 1;
    }
    criteria->order = 0;
    criteria->initial = 0xff;
    criteria->type1 = 0xff;
    criteria->type2 = 0xff;
    criteria->color = 0xff;
    criteria->shape = 0xff;
}

// pokebw2's ZukanCommon_WipeIn
void func_ov212_021c93a8(HeapID heapId) {
    GFL_WipeSet(0, 1, 1, 0, 6, 1, heapId);
}

// pokebw2's ZukanCommon_WipeOut
void func_ov212_021c93c4(HeapID heapId) {
    GFL_WipeSet(0, 0, 0, 0, 6, 1, heapId);
}

// pokebw2's ZukanCommon_ScrollBGs
void func_ov212_021c93e4(u8 bg1, u8 bg2, int *timer) {
    (*timer)++;
    if (*timer == 4) {
        GFL_BGSysMoveBGReq(bg1, BG_MOVE_RIGHT, 1);
        GFL_BGSysMoveBGReq(bg2, BG_MOVE_RIGHT, 1);
        *timer = 0;
    }
}

// pokebw2's ZukanCommon_CreateActor
ClActor *func_ov212_021c940c(ClActUnit *unit, const ZukanActorTemplate *template, HeapID heapId) {
    return func_0204b294(unit, template->chars, template->palette, template->cells, &template->setup, template->surface,
                         heapId);
}
