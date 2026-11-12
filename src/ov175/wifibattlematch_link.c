// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov260/wifibattlematch_link.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// wifibattlematch_link.c, in overlay 260 (a descriptive name: the ROM names only the next file,
// wifibattlematch_net.c): the proc of the Wi-Fi link that overlay 290 (wifibattlematch_sys.c) chains. It loads
// overlay 262, which the link's screens are made of, and runs the proc of the random matchups of the field or the
// one of the Wi-Fi competition, which the title screen's menu starts

#include "app/wifibattlematch_link.h"
#include "types.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"

// Where overlay 290 started the link from (WifiBattleMatchParam.unk8)
enum {
    START_TITLE,
    START_FIELD,
};

static BOOL func_ov175_021d4240(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov175_021d429c(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov175_021d42e0(GameProc *proc, u32 *state, void *param, void *work);

// pokebw2's WIFIBATTLEMATCH_LINK_PROC_FUNCTIONS
const GameProcFunctions data_ov175_021e2c68 = {
    func_ov175_021d4240,
    func_ov175_021d42e0,
    func_ov175_021d429c,
};

// pokebw2's WifiBattleMatchLink_Init
static BOOL func_ov175_021d4240(GameProc *proc, u32 *state, void *param, void *work) {
    WifiBattleMatchLinkParam *linkParam = param;

    GFL_OvlLoad(OVERLAY_ID(177));
    switch (linkParam->param->unk8) {
    case START_FIELD:
        return data_ov175_021e2c74.init(proc, state, param, work);
    case START_TITLE:
        return data_ov175_021e2c80.init(proc, state, param, work);
    default:
        GFL_ASSERT(0);
        return FALSE;
    }
}

// pokebw2's WifiBattleMatchLink_Exit
static BOOL func_ov175_021d429c(GameProc *proc, u32 *state, void *param, void *work) {
    WifiBattleMatchLinkParam *linkParam = param;
    BOOL result;

    switch (linkParam->param->unk8) {
    case START_FIELD:
        result = data_ov175_021e2c74.exit(proc, state, param, work);
        break;
    case START_TITLE:
        result = data_ov175_021e2c80.exit(proc, state, param, work);
        break;
    default:
        GFL_ASSERT(0);
        break;
    }
    GFL_SndStop();
    GFL_OvlUnload(OVERLAY_ID(177));
    return result;
}

// pokebw2's WifiBattleMatchLink_Main
static BOOL func_ov175_021d42e0(GameProc *proc, u32 *state, void *param, void *work) {
    WifiBattleMatchLinkParam *linkParam = param;

    switch (linkParam->param->unk8) {
    case START_FIELD:
        return data_ov175_021e2c74.main(proc, state, param, work);
    case START_TITLE:
        return data_ov175_021e2c80.main(proc, state, param, work);
    default:
        GFL_ASSERT(0);
        return FALSE;
    }
}
