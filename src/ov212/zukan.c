// Black's code at ov212 0x021c8e80-0x021c9244. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "gfl/heap.h"
#include "gfl/proc.h"
#include "struct_decls.h"
#include "gfl/overlay.h"
#include "app/zukan_open.h"
#include "app/zukan_common.h"
#include "app/zukanlist.h"
#include "app/zukansearch.h"
#include "save/pokedex.h"
#include "system/game_data.h"
#include "nitro/mi.h"

extern u32 OVERLAY_208_ID[];
extern u32 OVERLAY_209_ID[];
extern const struct GameProcFunctions data_ov208_021c8108;
extern u32 OVERLAY_210_ID[];
extern u32 OVERLAY_211_ID[];
extern const struct GameProcFunctions data_ov209_021be794;
extern const struct GameProcFunctions data_ov210_021c0714;

void func_ov212_021c8f7c(ZukanCommonWork *wk);
u32 func_ov212_021c8f98(ZukanCommonWork *wk);
u32 func_ov212_021c8fa8(ZukanCommonWork *wk);
u32 func_ov212_021c8ff4(ZukanCommonWork *wk);
u32 func_ov212_021c9024(ZukanCommonWork *wk);
u32 func_ov212_021c9098(ZukanCommonWork *wk);
u32 func_ov212_021c9130(ZukanCommonWork *wk);
u32 func_ov212_021c9184(ZukanCommonWork *wk);
u32 func_ov212_021c91c4(ZukanCommonWork *wk);

u32 func_ov212_021c8e80(GameProc *proc, u32 param_2, ZukanCommonParam *param);
BOOL func_ov212_021c8f2c(GameProc *proc, u32 param_2, u32 param_3, ZukanCommonWork *wk);
u32 func_ov212_021c8f58(GameProc *proc, u32 param_2, u32 param_3, ZukanCommonWork *wk);

// The Pokedex's proc, which the field's proc links run (pokebw2's ZUKAN_PROC_FUNCTIONS)
const GameProcFunctions data_ov212_021c9938 = {
    (GameProcFunc)func_ov212_021c8e80,
    (GameProcFunc)func_ov212_021c8f2c,
    (GameProcFunc)func_ov212_021c8f58,
};

u32 (*const data_ov212_021c9944[9])(ZukanCommonWork *wk) = {
    func_ov212_021c8f98,
    func_ov212_021c8fa8,
    func_ov212_021c8ff4,
    func_ov212_021c9024,
    func_ov212_021c9098,
    func_ov212_021c91c4,
    func_ov212_021c9244,
    func_ov212_021c9130,
    func_ov212_021c9184,
};

u32 func_ov212_021c8e80(GameProc *proc, u32 param_2, ZukanCommonParam *param) {
    ZukanCommonWork *wk;

    GFL_HeapCreateChild(1, 0x69, 0x8000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(ZukanCommonWork), 0x69);
    MI_CpuFill8(wk, 0, sizeof(ZukanCommonWork));
    wk->param = param;
    wk->procMgr = CreateGameProcManager(0x69);
    wk->count = func_ov212_021c9288(wk->param->pokedex, &wk->list, 0x69);
    wk->searched = 0;
    func_ov212_021c92ec(wk->param->pokedex, wk->list, wk->count);
    func_ov212_021c9380(wk->param->pokedex, &wk->criteria);
    if (wk->param->mode == 0) {
        wk->state = 1;
    } else if (wk->param->mode == 1) {
        wk->state = 3;
    } else if (wk->param->mode == 2) {
        wk->state = 5;
        wk->unk_24 = 1;
    } else if (wk->param->mode == 3) {
        wk->state = 5;
        wk->unk_24 = 2;
    } else if (wk->param->mode == 4) {
        wk->state = 5;
        wk->unk_24 = 3;
    } else if (wk->param->mode == 5) {
        wk->state = 5;
        wk->unk_24 = 4;
    } else {
        wk->state = 7;
    }
    return 1;
}

BOOL func_ov212_021c8f2c(GameProc *proc, u32 param_2, u32 param_3, ZukanCommonWork *wk) {
    if (wk->state == 9) {
        return 1;
    }
    wk->running = GFL_ProcMgrUpdate(wk->procMgr);
    wk->state = data_ov212_021c9944[wk->state](wk);
    return 0;
}

u32 func_ov212_021c8f58(GameProc *proc, u32 param_2, u32 param_3, ZukanCommonWork *wk) {
    func_ov212_021c8f7c(wk);
    FreeGameProcManager(wk->procMgr);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(0x69);
    return 1;
}

void func_ov212_021c8f7c(ZukanCommonWork *wk) {
    if (wk->list != NULL) {
        GFL_HeapFree(wk->list);
        wk->list = NULL;
    }
}

u32 func_ov212_021c8f90(ZukanCommonWork *wk, u32 next) {
    wk->nextState = next;
    return 0;
}

u32 func_ov212_021c8f98(ZukanCommonWork *wk) {
    if (wk->running != 1) {
        return wk->nextState;
    }
    return 0;
}

u32 func_ov212_021c8fa8(ZukanCommonWork *wk) {
    ZukanOpenParam *open;

    open = GFL_HeapAllocate(0x69, sizeof(ZukanOpenParam), FALSE, "zukan.c", 297);
    wk->subParam = open;
    open->gameData = wk->param->gameData;
    open->playerInfo = wk->param->playerInfo;
    QueueGameProc(wk->procMgr, OVERLAY_ID(211), &data_ov211_021bc8bc, wk->subParam);
    return func_ov212_021c8f90(wk, 2);
}

u32 func_ov212_021c8ff4(ZukanCommonWork *wk) {
    u32 next;

    switch (((ZukanOpenParam *)wk->subParam)->result) {
    case 0:
        wk->param->result = 0;
        next = 9;
        break;
    case 1:
        wk->param->result = 1;
        next = 9;
        break;
    case 2:
        next = 3;
        break;
    }
    GFL_HeapFree(wk->subParam);
    return next;
}

u32 func_ov212_021c9024(ZukanCommonWork *wk) {
    ZukanListParam *list;

    list = GFL_HeapAllocate(0x69, sizeof(ZukanListParam), FALSE, "zukan.c", 359);
    wk->subParam = list;
    list->gameData = wk->param->gameData;
    list->pokedex = wk->param->pokedex;
    list->searched = wk->searched;
    if (wk->searched == 0 && func_ov212_021c9348(wk->param->pokedex) == 0) {
        list->list = wk->list + 1;
        list->count = wk->count - 1;
    } else {
        list->list = wk->list;
        list->count = wk->count;
    }
    QueueGameProc(wk->procMgr, OVERLAY_ID(209), &data_ov209_021be794, wk->subParam);
    return func_ov212_021c8f90(wk, 4);
}

u32 func_ov212_021c9098(ZukanCommonWork *wk) {
    u32 next;

    switch (((ZukanListParam *)wk->subParam)->result) {
    case 0:
        if (wk->searched == 0) {
            wk->param->result = 0;
            next = 9;
            break;
        }
        func_ov212_021c8f7c(wk);
        wk->count = func_ov212_021c9288(wk->param->pokedex, &wk->list, 0x69);
        wk->searched = 0;
        next = 3;
        break;
    case 1:
        wk->param->result = 1;
        next = 9;
        break;
    case 2:
        func_ov212_021c8f7c(wk);
        wk->count = func_ov212_021c9288(wk->param->pokedex, &wk->list, 0x69);
        next = 3;
        break;
    case 3:
        next = 5;
        wk->unk_24 = 1;
        break;
    case 4:
        if (wk->searched == 1) {
            func_ov212_021c8f7c(wk);
            wk->count = func_ov212_021c9288(wk->param->pokedex, &wk->list, 0x69);
        }
        next = 7;
        break;
    default:
        wk->param->result = 0;
        next = 9;
        break;
    }
    GFL_HeapFree(wk->subParam);
    return next;
}

u32 func_ov212_021c9130(ZukanCommonWork *wk) {
    ZukanSearchParam *search;

    search = GFL_HeapAllocate(0x69, sizeof(ZukanSearchParam), FALSE, "zukan.c", 455);
    wk->subParam = search;
    search->gameData = wk->param->gameData;
    search->pokedex = wk->param->pokedex;
    search->criteria = &wk->criteria;
    search->list = NULL;
    search->count = 0;
    QueueGameProc(wk->procMgr, OVERLAY_ID(210), &data_ov210_021c0714, wk->subParam);
    return func_ov212_021c8f90(wk, 8);
}

u32 func_ov212_021c9184(ZukanCommonWork *wk) {
    ZukanSearchParam *search;
    u32 next;

    search = wk->subParam;
    switch (search->result) {
    case 0:
        func_ov212_021c8f7c(wk);
        wk->list = search->list;
        wk->count = search->count;
        wk->searched = 1;
        next = 3;
        break;
    case 1:
        wk->searched = 0;
        next = 3;
        break;
    case 2:
    default:
        wk->param->result = 1;
        next = 9;
        break;
    }
    GFL_HeapFree(wk->subParam);
    return next;
}

u32 func_ov212_021c91c4(ZukanCommonWork *wk) {
    ZukanCommonList *list;
    u16 current;
    u16 i;

    current = func_0200ca8c(GameData_GetPokedex(wk->param->gameData));
    list = GFL_HeapAllocate(0x69, sizeof(ZukanCommonList), FALSE, "zukan.c", 530);
    list->gameData = wk->param->gameData;
    list->unk_4 = wk->unk_24;
    list->species = wk->list;
    list->count = wk->count;
    list->index = 0;
    for (i = 0; i < list->count; i++) {
        if (current == list->species[i]) {
            list->index = i;
            break;
        }
    }
    QueueGameProc(wk->procMgr, OVERLAY_ID(208), &data_ov208_021c8108, list);
    wk->subParam = list;
    return func_ov212_021c8f90(wk, 6);
}
