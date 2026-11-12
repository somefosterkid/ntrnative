// The party list's proc (pokebw2's src/ov165/pokelist.c): Black's ov091 .text 0x021b6240-0x021b660c, its proc table
// and the "pokelist.c" of its allocations in .data 0x021bed00-0x021bed18, the timer's frame counter in .bss
// 0x021bed80 (then the module's padding to 0x021beda0). Black's init builds a debug party for a list opened without
// a param.

#include "types.h"
#include "app/pokelist.h"
#include "gfl/heap.h"
#include "gfl/proc.h"
#include "struct_decls.h"

struct GameProc {
    const GameProcFunctions *functions;
    s32 overlayId;
    u32 state;
    u32 subState;
    void *param;
    void *work;
    GameProc *parent;
    GameProc *child;
};

static BOOL func_ov091_021b6240(GameProc *proc, u32 *state, void *procParam, void *work);
static BOOL func_ov091_021b6534(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov091_021b65b8(GameProc *proc, u32 *state, void *param, void *work);

// Frames since the time left was last counted down (pokebw2's sTimerFrames)
static u8 data_ov091_021bed80;

// The party list's proc (pokebw2's POKELIST_PROC_FUNCTIONS): init, main and exit
GameProcFunctions data_ov091_021bed00 = {
    func_ov091_021b6240,
    func_ov091_021b65b8,
    func_ov091_021b6534,
};


#include "gfl/random.h"
#include "gfl/key.h"
#include "gfl/ui.h"
#include "pml/poke_party.h"
#include "battle/regulation.h"
#include "constants/pokemon.h"

static const u16 data_ov091_021bea90[6];
static const u16 data_ov091_021bea9c[6];

static BOOL func_ov091_021b6240(GameProc *proc, u32 *state, void *procParam, void *work) {
    PokeListWork *wk = work;
    PokeListParam *param = procParam;

    switch (*state) {
    case 0:
        GFL_HeapCreateChild(1, 0x17, 0x30000);
        wk = GFL_ProcInitSubsystem(proc, sizeof(PokeListWork), 0x17);
        wk->heapId = 0x17;
        if (param == NULL) {
            u8 i;
            param = GFL_HeapAllocate(0x17, sizeof(PokeListParam), TRUE, "pokelist.c", 0x52);
            param->party = PokeParty_Create(0x17);
            param->index = 0;
            PokeParty_InitCore(param->party, 6);
            for (i = 0; i < 6; i++) {
                PartyPkm *pkm = PokeParty_NewTempPkm(data_ov091_021bea90[i], data_ov091_021bea9c[i], 0xFFFFFFFF, 0x17);
                {
                    // The version's name
                    u16 otName[5] = { 0x30d6, 0x30e9, 0x30c3, 0x30af, 0xffff };

                    PokeParty_SetParam(pkm, PKM_PARAM_OT_NAME_RAW, (uintptr_t)otName);
                }
                PokeParty_SetParam(pkm, PKM_PARAM_OT_GENDER, 0);
                PokeParty_AddPkm(param->party, pkm);
                GFL_HeapFree(pkm);
            }
            param->mode = 0;
            if (GCTX_HIDGetHeldKeys() & 0x100) {
                Regulation *regulation = func_0201a354(0xf, 0x17);
                u8 i;
                u8 j;
                param->regulation = regulation;
                regulation->unk2 = 1;
                regulation->unk3 = 6;
                regulation->unk4 = 1;
                regulation->unk5 = 0;
                regulation->unkA8 = 0;
                regulation->unk6 = 200;
                for (i = 0; i < 6; i++) {
                    param->picked[i] = 0;
                }
                param->mode = 0x16;
                for (j = 0; j < 3; j++) {
                    u8 k;
                    u16 *partnerName;
                    param->partners[j].party = PokeParty_Create(0x17);
                    for (k = 0; k < 6; k++) {
                        PartyPkm *pkm = PokeParty_NewTempPkm(GFL_RandomLCAlt(250), 100, 0xFFFFFFFF, 0x17);
                        {
                            // The version's name
                            u16 otName[5] = { 0x30d6, 0x30e9, 0x30c3, 0x30af, 0xffff };

                            PokeParty_SetParam(pkm, PKM_PARAM_OT_NAME_RAW, (uintptr_t)otName);
                        }
                        PokeParty_SetParam(pkm, PKM_PARAM_OT_GENDER, 0);
                        PokeParty_SetParam(pkm, PKM_PARAM_ITEM, GFL_RandomLCAlt(2));
                        PokeParty_AddPkm(param->partners[j].party, pkm);
                        GFL_HeapFree(pkm);
                    }
                    partnerName = GFL_HeapAllocate(0x17, 0xc, FALSE, "pokelist.c", 0xae);
                    partnerName[0] = 0x3077;
                    partnerName[1] = 0x308c;
                    partnerName[2] = 0x3044;
                    partnerName[3] = 0x3084;
                    partnerName[4] = 0xff11 + j;
                    partnerName[5] = 0xffff;
                    param->partners[j].name = partnerName;
                    param->partners[j].gender = GFL_RandomLCAlt(2);
                }
                param->partnerCount = 2;
                param->showPartners = TRUE;
                param->timerEnabled = TRUE;
                param->timeLeft = 0xff;
                param->battleMsg = 0;
                func_0201a59c(regulation, param->party, NULL);
            } else if (GCTX_HIDGetHeldKeys() & 0x400) {
                param->mode = 5;
                param->item = 0x2c;
            } else if (GCTX_HIDGetHeldKeys() & 0x800) {
                param->mode = 0x19;
            }
            func_020355d8(TRUE);
        }
        wk->param = param;
        *state = 1;
        // fall through
    case 1:
        if (func_ov091_021b660c(wk) == TRUE) {
            return TRUE;
        }
        break;
    default:
        break;
    }
    return FALSE;
}

static const u16 data_ov091_021bea90[6] = { 0x1e7, 0x87, 0x20, 2, 3, 1 };

static const u16 data_ov091_021bea9c[6] = { 0x64, 0x63, 0x33, 0x32, 1, 1 };

static BOOL func_ov091_021b6534(GameProc *proc, u32 *state, void *param, void *work) {
    PokeListWork *wk = work;
    u8 i;

    if (func_ov091_021b6818(wk) == FALSE) {
        return FALSE;
    }
    if (param == NULL) {
        for (i = 0; i < 3; i++) {
            if (wk->param->partners[i].party != NULL) {
                GFL_HeapFree(wk->param->partners[i].party);
            }
            if (wk->param->partners[i].name != NULL) {
                GFL_HeapFree(wk->param->partners[i].name);
            }
        }
        if (wk->param->regulation != NULL) {
            GFL_HeapFree(wk->param->regulation);
        }
        GFL_HeapFree(wk->param->party);
        GFL_HeapFree(wk->param);
    }
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(0x17);
    return TRUE;
}

static BOOL func_ov091_021b65b8(GameProc *proc, u32 *state, void *param, void *work) {
    PokeListWork *wk = work;

    if (param == NULL && wk->param->timerEnabled == TRUE && wk->param->timeLeft != 0) {
        data_ov091_021bed80++;
        if (data_ov091_021bed80 > 60) {
            data_ov091_021bed80 = 0;
            wk->param->timeLeft--;
        }
    }
    if (func_ov091_021b68f0(wk) == TRUE) {
        return TRUE;
    }
    return FALSE;
}
