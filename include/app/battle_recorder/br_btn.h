// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/battle_recorder/br_btn.h, the lines in
// use so far.
#ifndef BWPORT_APP_BATTLE_RECORDER_BR_BTN_H
#define BWPORT_APP_BATTLE_RECORDER_BR_BTN_H

#include "types.h"
#include "app/battle_recorder/br_res.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// The Battle Recorder's menu buttons (br_btn.c), and the button objects its screens use. pokebw2's guessed names are
// in the comments

// pokebw2's BrBtn_InitEx
BrBtn *func_ov186_021fb378(const ClActorSetup *setup, u16 msgID, u16 width, u32 display, ClActUnit *unit,
                           BmpOamSys *bmpoam, Font *font, MsgData *msg, const BrResObjData *obj, HeapID heapId);
// pokebw2's BrBtn_Exit
void func_ov186_021fb494(BrBtn *p_wk);
// pokebw2's BrBtn_GetTrg
BOOL func_ov186_021fb4b4(const BrBtn *cp_wk, u32 x, u32 y);

// pokebw2's BrBtnSys_Exit
void func_ov186_021f9f48(BrBtnSys *p_wk);
// pokebw2's BrBtnSys_Main
void func_ov186_021f9fe8(BrBtnSys *p_wk);
// pokebw2's BrBtnSys_GetInput
// BR_BTN_INPUT_*, with the pressed button's BR_BTN_DATA_PARAM_DATA1 and DATA2
u32 func_ov186_021fa004(const BrBtnSys *cp_wk, u32 *p_data1, u32 *p_data2);

// Black 2's, not checked against Black
#define BR_BTN_SYS_STACK_MAX 6

// Black 2's; twinned code confirms the offsets of menuID in Black
struct BrBtnStack {
    u16 menuID;
    u16 btnID;
};

// The menus that were open, kept while another screen runs
struct BrBtnRecovery {
    BrBtnStack stack[BR_BTN_SYS_STACK_MAX];
    u32 stack_num;
};

// What BrBtnSys_GetInput returns once a button has been pressed
enum {
    BR_BTN_INPUT_NONE,
    // A button that starts a screen
    BR_BTN_INPUT_SELECT,
    // The button that leaves the Battle Recorder
    BR_BTN_INPUT_EXIT,
};

// Black 2's; twinned code confirms the offsets of clwk, oam, bmp, display and width in Black
// A button: a cell actor with its label in a bitmap OAM
struct BrBtn {
    ClActor *clwk;
    BmpOamActor *oam;
    GFLBitmap *bmp;
    u32 unkC;
    u32 display;
    u32 width;
};

// pokebw2's BrBtn_GetHit
BOOL func_ov186_021fb4cc(const BrBtn *cp_wk, u32 x, u32 y);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BrBtnSys_Init
BrBtnSys *func_ov186_021f9cb8(int menuID, ClActUnit *unit, BrRes *res, BrRecordInfo *recordInfo, BrBtnRecovery *recovery,
                        BrBallEffect *ballEff, HeapID heapId);
// pokebw2's BrBtnSys_IsBusy
BOOL func_ov186_021fa018(const BrBtnSys *cp_wk);
// pokebw2's BrBtnSys_GetUnk54
u32 func_ov186_021fa01c(const BrBtnSys *cp_wk);
// pokebw2's BrBtn_Init
BrBtn *func_ov186_021fb3bc(const ClActorSetup *setup, const StrBuf *str, u16 width, u32 display, ClActUnit *unit,
                  BmpOamSys *bmpoam, Font *font, const BrResObjData *obj, HeapID heapId);

#endif // BWPORT_APP_BATTLE_RECORDER_BR_BTN_H
