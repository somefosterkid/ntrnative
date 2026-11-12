// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/musical_stage_sys.c, ported by tools/port_file.py.
#include "types.h"
#include "constants/species.h"
#include "field/musical.h"
#include "field/musical_program.h"
#include "field/musical_stage_sys.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "app/musical/musical_system.h"

#define HEAPID_MUSICAL        0x2e
#define HEAPID_MUSICAL_EVENT  0x2c

extern u32 OVERLAY_209_ID[];

typedef struct {
    MusicalStageParam *param;
    // Overlay 209's stage
    void *stage;
    u32 unk8;
} MusicalStageWork;

static BOOL func_ov010_02156ce0(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov010_02156edc(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov010_02156f24(GameProc *proc, u32 *state, void *param, void *work);

GameProcFunctions data_ov010_02172748 = {
    func_ov010_02156ce0,
    func_ov010_02156f24,
    func_ov010_02156edc,
};

static BOOL func_ov010_02156ce0(GameProc *proc, u32 *state, void *param, void *work) {
    MusicalStageWork *wk;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_MUSICAL, 0xa8000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(MusicalStageWork), HEAPID_MUSICAL);
    if (param == NULL) {
        GFL_OvlLoad(OVERLAY_ID(112));
        // A test show of four Spinda
        wk->param = func_ov010_02156f68(HEAPID_MUSICAL, NULL);
        func_ov010_02156fd0(wk->param, 0, SPECIES_SPINDA, 0, 0xff000000, 60, HEAPID_MUSICAL);
        func_ov010_02156fd0(wk->param, 1, SPECIES_SPINDA, 0, 0xff0000, 120, HEAPID_MUSICAL);
        func_ov010_02156fd0(wk->param, 2, SPECIES_SPINDA, 0, 0xff00, 0xfffd, HEAPID_MUSICAL);
        func_ov010_02156fd0(wk->param, 3, SPECIES_SPINDA, 0, 0xff, 0xffff, HEAPID_MUSICAL);
        func_ov010_0215700c(wk->param, 0, 7, 19, 0, 0);
        func_ov010_0215700c(wk->param, 0, 8, 19, 0, 1);
        func_ov010_0215700c(wk->param, 0, 2, 16, 0, 2);
        func_ov010_0215700c(wk->param, 1, 1, 7, 0, 0);
        func_ov010_0215700c(wk->param, 1, 5, 9, 0, 1);
        func_ov010_0215700c(wk->param, 1, 7, 24, 0, 2);
        func_ov010_0215700c(wk->param, 1, 8, 43, 0, 3);
        func_ov010_0215700c(wk->param, 2, 7, 31, 0, 0);
        func_ov010_0215700c(wk->param, 2, 8, 30, 0, 1);
        func_ov010_0215700c(wk->param, 2, 2, 15, 0, 2);
        func_ov010_0215700c(wk->param, 3, 7, 30, 0, 0);
        func_ov010_0215700c(wk->param, 3, 8, 30, 0, 1);
        func_ov010_0215700c(wk->param, 3, 2, 21, 0, 2);
        wk->param->pokes[1]->owner = 0;
        GFL_HeapCreateChild(HEAPID_USER, HEAPID_TAIL(HEAPID_MUSICAL_EVENT), 0x80000);
        wk->param->ov210 = func_ov112_021f685c(HEAPID_USER);
        func_ov112_021f68bc(wk->param->ov210, NULL, NULL, 0, HEAPID_MUSICAL_EVENT);
        wk->param->program = func_ov010_02157028(HEAPID_MUSICAL, wk->param->ov210, 0);
        func_ov010_02157174(HEAPID_MUSICAL, wk->param->program, wk->param);
        wk->param->pokes[0]->points = 75;
        wk->param->pokes[1]->points = 45;
        wk->param->pokes[2]->points = 30;
        wk->param->pokes[3]->points = 0;
    } else {
        wk->param = param;
    }
    return TRUE;
}

BOOL func_ov010_02156edc(GameProc *proc, u32 *state, void *param, void *work) {
    MusicalStageWork *wk = work;

    if (param == NULL) {
        func_ov010_0215716c(wk->param->program);
        func_ov112_021f6888(wk->param->ov210);
        GFL_HeapDelete(HEAPID_MUSICAL_EVENT);
        wk->param->pokes[1]->owner = 2;
        func_ov010_02156f98(wk->param);
        GFL_OvlUnload(OVERLAY_ID(112));
    }
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_MUSICAL);
    return TRUE;
}

BOOL func_ov010_02156f24(GameProc *proc, u32 *state, void *param, void *work) {
    MusicalStageWork *wk = work;

    switch (*state) {
    case 0:
        wk->stage = func_ov132_021f8480(wk->param, HEAPID_MUSICAL);
        *state = 1;
        break;
    case 1:
        if (func_ov132_021f879c(wk->stage) == TRUE) {
            *state = 2;
        }
        break;
    case 2:
        func_ov132_021f8640(wk->stage);
        return TRUE;
    }
    return FALSE;
}


MusicalStageParam *func_ov010_02156f68(HeapID heapId, void *comm) {
    MusicalStageParam *param;
    u8 i;

    param = GFL_HeapAllocate(heapId, sizeof(MusicalStageParam), FALSE, "musical_stage_sys.c", 190);
    param->comm = comm;
    for (i = 0; i < 4; i++) {
        param->pokes[i] = NULL;
    }
    return param;
}

void func_ov010_02156f98(MusicalStageParam *param) {
    u8 i;

    for (i = 0; i < 4; i++) {
        if (param->pokes[i] != NULL && param->pokes[i]->owner == 2) {
            GFL_HeapFree(param->pokes[i]);
        }
    }
    GFL_HeapFree(param);
}

void func_ov010_02156fc4(MusicalStageParam *param, u8 pos, MusicalPoke *poke) {
    param->pokes[pos] = poke;
    param->pokes[pos]->owner = 0;
}

void func_ov010_02156fd0(MusicalStageParam *param, u8 pos, u16 species, u8 form, u32 personality, u16 a5, HeapID heapId) {
    param->pokes[pos] = func_ov112_021f67e0(species, form, 0, 0, personality, heapId);
    param->pokes[pos]->owner = 2;
    param->pokes[pos]->unk78 = a5;
}


void func_ov010_02157000(MusicalStageParam *param, u8 pos, MusicalPoke *poke) {
    param->pokes[pos] = poke;
    param->pokes[pos]->owner = 1;
}

void func_ov010_0215700c(MusicalStageParam *param, u8 pos, u8 slot, u16 itemId, s16 a4, u8 a5) {
    MusicalPoke *poke = param->pokes[pos];

    poke->equips[slot].itemId = itemId;
    poke->equips[slot].unk2 = a4;
    poke->equips[slot].slot = a5;
}
