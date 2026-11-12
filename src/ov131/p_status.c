// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov207/p_status.c, which is the same code in
// Black 2/White 2's overlay 207. The names are Black's dsd names; pokebw2's guesses are in the comments.
#include "app/p_status.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/proc.h"
#include "gfl/random.h"
#include "gfl/std.h"
#include "gfl/ui.h"
#include "p_status_local.h"
#include "pml/poke_party.h"
#include "system/game_data.h"

// The summary screen's process (p_status.c, named by the ROM's embedded string). Started without a parameter, as from
// a debug menu, it makes one of its own: a party of five test Pokémon, or with X held a box of thirty, some of them
// empty

#define DEBUG_PARTY_COUNT 5
#define DEBUG_BOX_COUNT 30

static BOOL func_ov131_021d6324(GameProc *proc, u32 *state, void *data, void *work);
static BOOL func_ov131_021d657c(GameProc *proc, u32 *state, void *data, void *work);
static BOOL func_ov131_021d65bc(GameProc *proc, u32 *state, void *data, void *work);

// pokebw2's PSTATUS_PROC_FUNCTIONS
GameProcFunctions data_ov131_021dc800 = { func_ov131_021d6324, func_ov131_021d65bc, func_ov131_021d657c };

// pokebw2's PStatus_ProcInit
static BOOL func_ov131_021d6324(GameProc *proc, u32 *state, void *data, void *work) {
    PStatusParam *param = data;
    PStatusWork *wk;
    PokeParty *party;
    PartyPkm *partyPkm;
    BoxPkm *boxPkm;
    u8 i;

    switch (*state) {
    case 0:
        GFL_HeapCreateChild(HEAPID_USER, HEAPID_P_STATUS, 0x50000);
        wk = GFL_ProcInitSubsystem(proc, sizeof(PStatusWork), HEAPID_P_STATUS);
        MI_CpuFill8(wk, 0, sizeof(PStatusWork));
        wk->heapId = HEAPID_P_STATUS;
        if (param == NULL) {
            param = GFL_HeapAllocate(HEAPID_USER, sizeof(PStatusParam), FALSE, "p_status.c", 77);
            if (!(GCTX_HIDGetHeldKeys() & PAD_BUTTON_X)) {
                party = PokeParty_Create(HEAPID_USER);
                PokeParty_InitCore(party, 6);
                for (i = 0; i < DEBUG_PARTY_COUNT; i++) {
                    // The trainer ID 0xffffffff, not PKM_ID_RANDOM
                    partyPkm = PokeParty_NewTempPkm(i + 3, 10, 0xffffffff, HEAPID_USER);
                    {
                        // The version's name
                        u16 otName[5] = { 0x30d6, 0x30e9, 0x30c3, 0x30af, 0xffff };

                        PokeParty_SetParam(partyPkm, PKM_PARAM_OT_NAME_RAW, (uintptr_t)otName);
                    }
                    PokeParty_SetParam(partyPkm, PKM_PARAM_OT_GENDER, 0);
                    PokeParty_AddPkm(party, partyPkm);
                    GFL_HeapFree(partyPkm);
                }
                param->partyIndex = 0;
                param->party = party;
                param->dataType = PSTATUS_DATA_PARTY;
                param->partyCount = DEBUG_PARTY_COUNT;
            } else {
                param->party =
                    GFL_HeapAllocate(HEAPID_USER, PML_GetPkmRawSize() * DEBUG_BOX_COUNT, TRUE, "p_status.c", 129);
                param->dataType = PSTATUS_DATA_BOX;
                param->partyCount = DEBUG_BOX_COUNT;
                param->partyIndex = 0xff;
                for (i = 0; i < param->partyCount; i++) {
                    boxPkm = (BoxPkm *)((u8 *)param->party + PML_GetPkmRawSize() * i);
                    PML_PkmInit(boxPkm);
                    if (GFL_RandomLCAlt(3) != 0) {
                        PML_CreateTempPkm(boxPkm, i + 1, 50, PKM_ID_RANDOM);
                        {
                            // The version's name
                            u16 otName[5] = { 0x30d6, 0x30e9, 0x30c3, 0x30af, 0xffff };

                            PML_PkmSetParam(boxPkm, PKM_PARAM_OT_NAME_RAW, (uintptr_t)otName);
                        }
                        PML_PkmSetParam(boxPkm, PKM_PARAM_OT_GENDER, 0);
                        if (param->partyIndex == 0xff) {
                            param->partyIndex = i;
                        }
                    }
                }
            }
            param->mode = PSTATUS_MODE_NORMAL;
            param->page = PSTATUS_PAGE_INFO;
            param->fromFieldMenu = TRUE;
            param->isNationalDex = TRUE;
            param->gameData = GameData_Create(HEAPID_USER);
            if (GCTX_HIDGetHeldKeys() & PAD_BUTTON_X) {
                param->mode = PSTATUS_MODE_FORGET_MOVE;
                param->fromFieldMenu = FALSE;
                param->move = MOVE_SCRATCH;
            }
            if (GCTX_HIDGetHeldKeys() & PAD_BUTTON_R) {
                param->mode = PSTATUS_MODE_1;
                param->fromFieldMenu = FALSE;
            }
            func_020355d8(TRUE);
        }
        wk->param = param;
        *state = 1;
        break;
    case 1:
        if (func_ov131_021d4240(work) == TRUE) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's PStatus_ProcExit
static BOOL func_ov131_021d657c(GameProc *proc, u32 *state, void *data, void *work) {
    PStatusWork *wk = work;

    if (func_ov131_021d4418(wk) == FALSE) {
        return FALSE;
    }
    if (data == NULL) {
        GameData_Free(wk->param->gameData);
        GFL_HeapFree(wk->param->party);
        GFL_HeapFree(wk->param);
    }
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_P_STATUS);
    return TRUE;
}

// pokebw2's PStatus_ProcMain
static BOOL func_ov131_021d65bc(GameProc *proc, u32 *state, void *data, void *work) {
    int result = func_ov131_021d44c8(work);

    if (result == PSTATUS_RESULT_BACK) {
        return TRUE;
    }
    if (result == PSTATUS_RESULT_CLOSE) {
        return TRUE;
    }
    return FALSE;
}
