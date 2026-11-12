// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov294/intro.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "demo/intro.h"
#include "gfl/msg.h"
#include "gfl/std.h"
#include "system/gf_font.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"

// The intro's process. ov294 has no file name for this file, which is named after the others (intro_cmd.c and so on)

typedef struct {
    HeapID heapId;
    IntroParam *param;
    IntroGraphic *graphic;
    Font *font;
    MsgData *msgData;
    IntroCmd *cmd;
    IntroMcss *mcss;
    IntroG3d *g3d;
    IntroParticle *particle;
} IntroWork;

BOOL func_ov204_021bda40(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov204_021bdb30(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov204_021bdae4(GameProc *proc, u32 *state, void *param, void *work);

// pokebw2's INTRO_SOUNDS
const u32 data_ov204_021bfd30[] = {
    SEQ_SE_NAGERU, SEQ_SE_BOWA2, SEQ_SE_OPEN2, SEQ_SE_KON, SEQ_SE_TOUJOU_INTRO, SEQ_BGM_STARTING, SEQ_BGM_STARTING2,
};

// pokebw2's INTRO_PROC_FUNCTIONS
// pokebw2's IntroProc_Exit
// pokebw2's IntroProc_Init
// pokebw2's IntroProc_Main
const GameProcFunctions data_ov204_021bfd24 = { func_ov204_021bda40, func_ov204_021bdb30, func_ov204_021bdae4 };

// pokebw2's INTRO_SOUND_COUNT
const u32 data_ov204_021bfd20 = NELEMS(data_ov204_021bfd30);



BOOL func_ov204_021bda40(GameProc *proc, u32 *state, void *param, void *work) {
    IntroWork *wk;

    GFL_OvlLoad(OVERLAY_ID(67));
    GFL_HeapCreateChild(HEAPID_USER, 0x6f, 0x100000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(IntroWork), 0x6f);
    MI_CpuFill8(wk, 0, sizeof(IntroWork));
    wk->heapId = 0x6f;
    wk->param = param;
    wk->graphic = func_ov204_021bdc18(1, wk->param->mode, wk->heapId);
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, 0, wk->heapId);
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_INTRO_BTL_SERVER_FLOW, wk->heapId);
    wk->mcss = func_ov204_021bf594(wk->heapId, wk->param->mode);
    wk->g3d = func_ov204_021bf8dc(wk->graphic, wk->param->mode, wk->heapId);
    wk->particle = func_ov204_021bfc24(wk->graphic, wk->heapId);
    wk->cmd = func_ov204_021bef20(wk->g3d, wk->particle, wk->mcss, wk->param, wk->graphic, wk->heapId);
    return TRUE;
}

BOOL func_ov204_021bdae4(GameProc *proc, u32 *state, void *param, void *work) {
    IntroWork *wk = work;
    HeapID heapId;

    func_ov204_021bfcd0(wk->particle);
    func_ov204_021bf5e0(wk->mcss);
    func_ov204_021bf9d0(wk->g3d);
    GFL_MsgDataFree(wk->msgData);
    GFL_FontFree(wk->font);
    func_ov204_021bef74(wk->cmd);
    func_ov204_021bdcf0(wk->graphic);
    heapId = wk->heapId;
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(heapId);
    GFL_OvlUnload(OVERLAY_ID(67));
    return TRUE;
}

BOOL func_ov204_021bdb30(GameProc *proc, u32 *state, void *param, void *work) {
    IntroWork *wk = work;

    if (!func_ov204_021bef88(wk->cmd)) {
        return TRUE;
    }
    func_ov204_021bdd4c(wk->graphic);
    func_ov204_021bdd60(wk->graphic);
    func_ov204_021bf898(wk->mcss);
    func_ov204_021bf610(wk->mcss);
    func_ov204_021bfce0(wk->particle);
    func_ov204_021bf9fc(wk->g3d);
    func_ov204_021bdd6c(wk->graphic);
    return FALSE;
}
