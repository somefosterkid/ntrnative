// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov268/br_codein.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Battle Recorder's code input, where the player types a battle video's number on the lower screen: its work,
// and its parameter. The ROM names this file. The states and the input are in br_codein_pv.c and the graphics in
// br_codein_disp.c, as in overlay 205's code input screen, which this is a copy of. Our names

#include "types.h"
#include "app/battle_recorder/br_codein.h"
#include "app/battle_recorder/br_codein_pv.h"
#include "app/battle_recorder/br_res.h"
#include "app/battle_recorder/br_util.h"
#include "gfl/arc.h"
#include "gfl/button_man.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"

// The archive of the code input screen's graphics, as in overlay 129's codein.c
#define ARCID_CODEIN 59

void func_ov183_021e083c(TCB *tcb, void *data);

// pokebw2's BrCodeIn_Init
BrCodeInWork *func_ov183_021e06d0(const BrCodeInParam *param, HeapID heapId) {
    BrCodeInWork *wk = GFL_HeapAllocate(heapId, sizeof(BrCodeInWork), FALSE, "br_codein.c", 56);

    MI_CpuFill8(wk, 0, sizeof(BrCodeInWork));
    wk->param = *param;
    wk->heapId = heapId;
    wk->unit = param->unit;
    wk->font = func_ov186_021fc20c(param->res);
    wk->arc = GFL_ArcSysCreateFileHandle(ARCID_CODEIN, wk->heapId);
    func_ov183_021e1244(wk);
    func_ov183_021e1910(wk);
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov183_021e083c, wk, 16);
    return wk;
}

// pokebw2's BrCodeIn_Main
void func_ov183_021e0754(BrCodeInWork *wk) {
    func_ov183_021e16e4(wk);
}

// pokebw2's BrCodeIn_GetResult
int func_ov183_021e075c(const BrCodeInWork *wk) {
    return wk->result;
}

// pokebw2's BrCodeIn_GetNumber
u64 func_ov183_021e0764(const BrCodeInWork *wk) {
    return wk->number;
}

// pokebw2's BrCodeIn_Exit
void func_ov183_021e0774(BrCodeInWork *wk) {
    GFL_TCBRemove(wk->vblankTask);
    func_ov183_021e08e4(wk);
    GFL_ArcToolFree(wk->arc);
    func_02051c6c(wk->buttons);
    GFL_HeapFree(wk);
}

BrCodeInParam *func_ov183_021e07a4(u32 heapId, u32 length, ClActUnit *unit, BrRes *res, BrBallEffect *ballEff,
                                    const int *blocks) {
    int i = 0;
    BrCodeInParam *param = GFL_HeapAllocate(heapId, sizeof(BrCodeInParam), FALSE, "br_codein.c", 192);

    param->unit = unit;
    param->res = res;
    param->length = length;
    param->strbuf = GFL_StrBufCreate(length + 1, heapId);
    param->ballEff = ballEff;
    for (; i < BR_CODEIN_BLOCK_COUNT; i++) {
        param->blocks[i] = blocks[i];
    }
    param->blocks[i] = blocks[i - 1];
    return param;
}

// The parameter and its string keep the original's names, which the assertions' texts hold
// pokebw2's BrCodeInParam_Free
void func_ov183_021e0808(BrCodeInParam *codein_param) {
    GFL_ASSERT(codein_param->strbuf != NULL);
    GFL_ASSERT(codein_param != NULL);
    GFL_StrBufFree(codein_param->strbuf);
    GFL_HeapFree(codein_param);
}

// pokebw2's BrCodeIn_VBlank
void func_ov183_021e083c(TCB *tcb, void *data) {
}
