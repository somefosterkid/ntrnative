// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov271/br_btn.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Battle Recorder's menu buttons: the buttons of the current menu, the stack of the buttons pressed to reach it,
// and the sequences that move them between menus. Also the button objects, a cell actor with its label in a bitmap
// OAM, that the screens use for their own buttons. The name is the ROM's string, from GFL_HeapAllocate's asserts

#include "types.h"
#include "app/battle_recorder/br_btn.h"
#include "app/battle_recorder/br_btn_data.h"
#include "app/battle_recorder/br_res.h"
#include "app/battle_recorder/br_util.h"
#include "gfl/bmp.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "system/bmp_oam.h"
#include "system/printsys.h"

// The parameters of a menu button, which func_ov186_021faf24 reads from its data
enum {
    BR_BTN_PARAM_TYPE,
    BR_BTN_PARAM_DATA1,
    BR_BTN_PARAM_DATA2,
    BR_BTN_PARAM_VALID,
    BR_BTN_PARAM_MENUID,
    BR_BTN_PARAM_UNVALID_TYPE,
    BR_BTN_PARAM_UNVALID_DATA,
    BR_BTN_PARAM_VALID_TYPE,
    BR_BTN_PARAM_X,
    BR_BTN_PARAM_Y,
};

// How func_ov186_021fad18 moves a button
enum {
    // Up or down to the target
    BR_BTN_MOVE_TARGET,
    // Down to its place in the menu
    BR_BTN_MOVE_RETURN,
    // Off the top of the sub screen, then from the top of the main screen to the target
    BR_BTN_MOVE_TO_MAIN,
    // Off the top of the main screen, then from the top of the sub screen to the target
    BR_BTN_MOVE_TO_SUB,
    // To the target, more slowly
    BR_BTN_MOVE_TARGET_SLOW,
};

// A button of the menus
typedef struct BrBtnWork {
    BOOL is_use;
    u32 display;
    BrBtn *btn;
    const BrBtnData *data;
    BOOL (*moveFunc)(struct BrBtnWork *p_wk);
    BrPoint target;
    u32 seq;
    s16 start;
    s16 end;
    s16 count;
    s16 max;
    u32 objID;
    // Kept at the button's position while it moves, for the ball effect
    BrPoint *follow;
    BrRes *res;
    HeapID heapId;
    ClActUnit *unit;
    BmpOamSys *bmpoam;
    StrBuf *str;
} BrBtnWork;

struct BrBtnSys {
    HeapID heapId;
    BrRes *res;
    ClActUnit *unit;
    BrSeq *seq;
    u32 objID;
    // FALSE while the menu waits for a button
    BOOL isBusy;
    u32 input;
    u32 btnType;
    BOOL isValid;
    u32 data1;
    u32 data2;
    u32 trgIdx;
    BrPoint trgPos;
    u8 btn_max;
    u8 btn_num;
    u8 btn_stack_max;
    u8 btn_stack_num;
    BrBtnWork *btn_stack;
    BrBtnWork *btn;
    BrBtnDataSys *btnData;
    BmpOamSys *bmpoam;
    PrintQueue *que;
    BrMsgWin *text;
    u32 unk54;
    BrBallEffect *ballEff;
    BrBtnRecovery *recovery;
};

// The buttons of the menus behind the current one, the top first
// pokebw2's sc_stack_pos
static const BrPoint data_ov186_02200358[] = {
    { 42, 169 },
    { 42, 153 },
    { 42, 137 },
    { 42, 121 },
};

void func_ov186_021fa020(BrBtnSys *p_wk, const BrBtnWork *cp_btn, u32 display);
BOOL func_ov186_021fa0a8(BrBtnSys *p_wk, BrBtnWork *p_btn);
void func_ov186_021fa0fc(BrBtnSys *p_wk, u32 menuID, const BrPoint *cp_pos);
void func_ov186_021fa1dc(BrBtnRecovery *p_wk, u16 menuID, u16 btnID);
void func_ov186_021fa210(BrBtnRecovery *p_wk);
void func_ov186_021fa21c(BrBtnSys *p_wk, u32 mode);
void func_ov186_021fa274(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov186_021fa284(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov186_021fa444(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov186_021fa500(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov186_021fa6a8(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov186_021fa758(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov186_021fa7fc(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov186_021fa920(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov186_021faa34(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov186_021faa74(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov186_021faad0(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov186_021fab7c(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
void func_ov186_021fac20(BrBtnWork *p_wk, const BrBtnData *cp_data, ClActUnit *unit, BmpOamSys *bmpoam, BrRes *res,
                           const StrBuf *str, u32 objID, HeapID heapId);
void func_ov186_021face0(BrBtnWork *p_wk);
BOOL func_ov186_021fad04(const BrBtnWork *cp_wk, u32 x, u32 y);
void func_ov186_021fad18(BrBtnWork *p_wk, u32 type, const BrPoint *cp_pos);
BOOL func_ov186_021fad74(BrBtnWork *p_wk);
void func_ov186_021fadb8(BrBtnWork *p_wk, u32 display);
void func_ov186_021fae70(const BrBtnSys *cp_sys, BrBtnWork *p_dst, const BrBtnWork *cp_src, u32 display);
void func_ov186_021faec0(BrBtnWork *p_wk, u16 idx);
void func_ov186_021faee4(const BrBtnWork *cp_wk, BrPoint *p_pos);
void func_ov186_021faf08(BrBtnWork *p_wk, s16 y);
u32 func_ov186_021faf24(const BrBtnWork *cp_wk, u32 paramID);
void func_ov186_021fafa0(BrBtnWork *p_wk, u16 priority);
void func_ov186_021fafb0(BrBtnWork *p_wk, u32 priority);
void func_ov186_021fafc0(BrBtnWork *p_wk, u32 plt);
void func_ov186_021fafcc(BrBtnWork *p_wk, BrPoint *p_pos);
BOOL func_ov186_021fafd0(BrBtnWork *p_wk);
BOOL func_ov186_021fb058(BrBtnWork *p_wk);
BOOL func_ov186_021fb0e8(BrBtnWork *p_wk);
BOOL func_ov186_021fb1ec(BrBtnWork *p_wk);
BOOL func_ov186_021fb2f0(BrBtnWork *p_wk);
void func_ov186_021fb514(BrBtn *p_wk, s16 x, s16 y);
void func_ov186_021fb548(const BrBtn *cp_wk, s16 *p_x, s16 *p_y);
void func_ov186_021fb578(BrBtn *p_wk, u8 priority);
u8 func_ov186_021fb594(const BrBtn *cp_wk);
void func_ov186_021fb5a0(BrBtn *p_wk, u8 priority);
u8 func_ov186_021fb5b8(const BrBtn *cp_wk);
void func_ov186_021fb5c4(BrBtn *p_wk, u32 plt);

// pokebw2's BrBtnSys_Exit

BrBtnSys *func_ov186_021f9cb8(int menuID, ClActUnit *unit, BrRes *res, BrRecordInfo *recordInfo, BrBtnRecovery *recovery,
                              BrBallEffect *ballEff, HeapID heapId) {
    BrBtnSys *p_wk;
    int i;

    GFL_ASSERT(menuID < BR_MENUID_MAX);
    p_wk = GFL_HeapAllocate(heapId, sizeof(BrBtnSys), FALSE, "br_btn.c", 319);
    MI_CpuFill8(p_wk, 0, sizeof(BrBtnSys));
    p_wk->res = res;
    p_wk->unit = unit;
    p_wk->heapId = heapId;
    p_wk->recovery = recovery;
    p_wk->bmpoam = func_02025538(heapId, unit);
    p_wk->que = func_0201c440(heapId);
    p_wk->ballEff = ballEff;
    p_wk->isBusy = TRUE;
    {
        BrBtnDataSetup setup;

        MI_CpuFill8(&setup, 0, sizeof(BrBtnDataSetup));
        setup.recordInfo = recordInfo;
        p_wk->btnData = func_ov186_021fb5e4(&setup, heapId);
    }

    if (menuID >= BR_MENUID_MUSICAL_TOP) {
        p_wk->objID = BR_RES_OBJ_MUSICAL_BTN_M;
    } else {
        p_wk->objID = BR_RES_OBJ_BROWSE_BTN_M;
    }
    func_ov186_021fbe54(p_wk->res, p_wk->objID, heapId);
    func_ov186_021fbe54(p_wk->res, p_wk->objID + 1, heapId);

    p_wk->btn_max = func_ov186_021fb788(p_wk->btnData);
    {
        u32 size = sizeof(BrBtnWork) * p_wk->btn_max;

        p_wk->btn = GFL_HeapAllocate(heapId, size, FALSE, "br_btn.c", 359);
        MI_CpuFill8(p_wk->btn, 0, size);
    }
    p_wk->btn_stack_max = BR_BTN_SYS_STACK_MAX;
    p_wk->btn_stack = GFL_HeapAllocate(heapId, sizeof(BrBtnWork) * BR_BTN_SYS_STACK_MAX, FALSE, "br_btn.c", 369);
    MI_CpuFill8(p_wk->btn_stack, 0, sizeof(BrBtnWork) * BR_BTN_SYS_STACK_MAX);
    p_wk->seq = func_ov186_021fd6d8(p_wk, func_ov186_021fa274, heapId);

    if (p_wk->recovery->stack_num != 0) {
        MsgData *msg = func_ov186_021fc214(res);

        for (i = 0; i < p_wk->recovery->stack_num; i++) {
            const BrBtnData *cp_data = func_ov186_021fb704(p_wk->btnData, p_wk->recovery->stack[i].menuID, p_wk->recovery->stack[i].btnID);
            StrBuf *str = func_ov186_021fb7d0(p_wk->btnData, cp_data, msg, HEAPID_TAIL(heapId));
            BrBtnWork btn;

            MI_CpuFill8(&btn, 0, sizeof(BrBtnWork));
            func_ov186_021fac20(&btn, cp_data, unit, p_wk->bmpoam, res, str, p_wk->objID, heapId);
            func_ov186_021fa020(p_wk, &btn, CLACT_SURFACE_MAIN);
            GFL_StrBufFree(str);
        }
        for (i = 0; i < p_wk->btn_stack_num; i++) {
            func_ov186_021faec0(&p_wk->btn_stack[p_wk->btn_stack_num - i - 1], i);
        }
        func_ov186_021fd73c(p_wk->seq, func_ov186_021fa920);
        p_wk->btnType = BR_BTN_TYPE_RETURN;
    } else {
        MsgData *msg = func_ov186_021fc214(res);

        p_wk->btn_num = func_ov186_021fb760(p_wk->btnData, menuID);
        for (i = 0; i < p_wk->btn_num; i++) {
            const BrBtnData *cp_data = func_ov186_021fb704(p_wk->btnData, menuID, i);
            StrBuf *str = func_ov186_021fb7d0(p_wk->btnData, cp_data, msg, HEAPID_TAIL(heapId));

            func_ov186_021fac20(&p_wk->btn[i], cp_data, unit, p_wk->bmpoam, res, str, p_wk->objID, heapId);
            GFL_StrBufFree(str);
        }
        for (i = 0; i < p_wk->btn_num; i++) {
            func_ov186_021faf08(&p_wk->btn[i], 224);
        }
        func_ov186_021fd73c(p_wk->seq, func_ov186_021faad0);
    }
    return p_wk;
}

void func_ov186_021f9f48(BrBtnSys *p_wk) {
    int i;

    func_ov186_021fd714(p_wk->seq);
    if (p_wk->text != NULL) {
        func_ov186_021fd15c(p_wk->text, p_wk->res);
        p_wk->text = NULL;
    }
    for (i = 0; i < p_wk->btn_stack_num; i++) {
        func_ov186_021face0(&p_wk->btn_stack[i]);
    }
    GFL_HeapFree(p_wk->btn_stack);
    for (i = 0; i < p_wk->btn_num; i++) {
        func_ov186_021face0(&p_wk->btn[i]);
    }
    GFL_HeapFree(p_wk->btn);
    func_ov186_021fc0c8(p_wk->res, p_wk->objID);
    func_ov186_021fc0c8(p_wk->res, p_wk->objID + 1);
    func_ov186_021fb63c(p_wk->btnData);
    func_02025588(p_wk->bmpoam);
    func_0201c4c0(p_wk->que);
    GFL_HeapFree(p_wk);
}

// pokebw2's BrBtnSys_Main
void func_ov186_021f9fe8(BrBtnSys *p_wk) {
    func_ov186_021fd71c(p_wk->seq);
    func_0201c4e4(p_wk->que);
    if (p_wk->text != NULL) {
        func_ov186_021fd1bc(p_wk->text);
    }
}

// pokebw2's BrBtnSys_GetInput
u32 func_ov186_021fa004(const BrBtnSys *cp_wk, u32 *p_data1, u32 *p_data2) {
    if (p_data1 != NULL) {
        *p_data1 = cp_wk->data1;
    }
    if (p_data2 != NULL) {
        *p_data2 = cp_wk->data2;
    }
    return cp_wk->input;
}

// pokebw2's BrBtnSys_IsBusy
BOOL func_ov186_021fa018(const BrBtnSys *cp_wk) {
    return cp_wk->isBusy;
}

// pokebw2's BrBtnSys_GetUnk54
u32 func_ov186_021fa01c(const BrBtnSys *cp_wk) {
    return cp_wk->unk54;
}

// pokebw2's BrBtnSys_PushStack
void func_ov186_021fa020(BrBtnSys *p_wk, const BrBtnWork *cp_btn, u32 display) {
    int i;

    GFL_ASSERT(p_wk->btn_stack_num < p_wk->btn_stack_max);
    func_ov186_021fae70(p_wk, &p_wk->btn_stack[p_wk->btn_stack_num], cp_btn, display);
    p_wk->btn_stack_num++;
    for (i = 0; i < p_wk->btn_stack_num; i++) {
        func_ov186_021fafa0(&p_wk->btn_stack[i], (p_wk->btn_stack_num - i) * 2 + 1);
        func_ov186_021fafb0(&p_wk->btn_stack[i], 1);
    }
}

// pokebw2's BrBtnSys_PopStack
BOOL func_ov186_021fa0a8(BrBtnSys *p_wk, BrBtnWork *p_btn) {
    if (p_wk->btn_stack_num != 0) {
        p_wk->btn_stack_num--;
        *p_btn = p_wk->btn_stack[p_wk->btn_stack_num];
        MI_CpuFill8(&p_wk->btn_stack[p_wk->btn_stack_num], 0, sizeof(BrBtnWork));
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BrBtnSys_LoadMenu
void func_ov186_021fa0fc(BrBtnSys *p_wk, u32 menuID, const BrPoint *cp_pos) {
    int i;
    MsgData *msg;

    for (i = 0; i < p_wk->btn_max; i++) {
        func_ov186_021face0(&p_wk->btn[i]);
    }

    msg = func_ov186_021fc214(p_wk->res);
    p_wk->btn_num = func_ov186_021fb760(p_wk->btnData, menuID);
    for (i = 0; i < p_wk->btn_num; i++) {
        const BrBtnData *cp_data = func_ov186_021fb704(p_wk->btnData, menuID, i);
        StrBuf *str = func_ov186_021fb7d0(p_wk->btnData, cp_data, msg, HEAPID_TAIL(p_wk->heapId));

        func_ov186_021fac20(&p_wk->btn[i], cp_data, p_wk->unit, p_wk->bmpoam, p_wk->res, str, p_wk->objID, p_wk->heapId);
        GFL_StrBufFree(str);
        func_ov186_021fafa0(&p_wk->btn[i], 3);
        if (cp_pos != NULL) {
            func_ov186_021fb514(p_wk->btn[i].btn, cp_pos->x, cp_pos->y);
        }
    }
}

// pokebw2's BrBtnRecovery_Push
void func_ov186_021fa1dc(BrBtnRecovery *p_wk, u16 menuID, u16 btnID) {
    GFL_ASSERT(p_wk->stack_num < BR_BTN_SYS_STACK_MAX);
    p_wk->stack[p_wk->stack_num].menuID = menuID;
    p_wk->stack[p_wk->stack_num].btnID = btnID;
    p_wk->stack_num++;
}

// pokebw2's BrBtnRecovery_Pop
void func_ov186_021fa210(BrBtnRecovery *p_wk) {
    if (p_wk->stack_num != 0) {
        p_wk->stack_num--;
    }
}

// Lights the buttons of the stack that can be pressed to return: the top one, or after a return the top two
// pokebw2's BrBtnSys_SetStackPalette
void func_ov186_021fa21c(BrBtnSys *p_wk, u32 mode) {
    int i;
    BOOL isActive;

    for (i = 0; i < p_wk->btn_stack_num; i++) {
        switch (mode) {
        case 0:
            isActive = i == p_wk->btn_stack_num - 1;
            break;
        case 1:
            isActive = i >= p_wk->btn_stack_num - 2;
            break;
        }
        if (isActive) {
            func_ov186_021fafc0(&p_wk->btn_stack[i], 0);
        } else {
            func_ov186_021fafc0(&p_wk->btn_stack[i], 1);
        }
    }
}

// pokebw2's BrBtnSys_Seq_Start
void func_ov186_021fa274(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    func_ov186_021fd73c(p_seqwk, func_ov186_021fa284);
}

// pokebw2's BrBtnSys_Seq_Main
void func_ov186_021fa284(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrBtnSys *p_wk = p_wk_adrs;
    BOOL isTrg = FALSE;
    u32 x, y;
    int i;

    p_wk->input = BR_BTN_INPUT_NONE;
    p_wk->isBusy = FALSE;

    if (func_02035d20(&x, &y)) {
        for (i = 0; i < p_wk->btn_max; i++) {
            if (func_ov186_021fad04(&p_wk->btn[i], x, y)) {
                if (func_ov186_021faf24(&p_wk->btn[i], BR_BTN_PARAM_VALID)) {
                    if (p_wk->text != NULL) {
                        func_ov186_021fd15c(p_wk->text, p_wk->res);
                        p_wk->text = NULL;
                    }
                    GFL_SndSEPlay(0x703);
                    p_wk->trgIdx = i;
                    isTrg = TRUE;
                    p_wk->trgPos.x = x;
                    p_wk->trgPos.y = y;
                    func_ov186_021fd860(p_wk->ballEff, BR_BALL_EFFECT_TOUCH, &p_wk->trgPos);
                    func_ov186_021fafcc(&p_wk->btn[p_wk->trgIdx], &p_wk->trgPos);
                    p_wk->unk54 = 0;
                } else {
                    u32 type = func_ov186_021faf24(&p_wk->btn[i], BR_BTN_PARAM_UNVALID_TYPE);
                    // Never read, as in pokebw2's twin
                    u32 data = func_ov186_021faf24(&p_wk->btn[i], BR_BTN_PARAM_UNVALID_DATA);

                    GFL_SndSEPlay(0x704);
                    if (type == 1) {
                        p_wk->trgIdx = i;
                        func_ov186_021fd73c(p_seqwk, func_ov186_021faa74);
                    }
                }
                break;
            }
        }
    }

    if (isTrg) {
        p_wk->isBusy = TRUE;
        p_wk->btnType = func_ov186_021faf24(&p_wk->btn[p_wk->trgIdx], BR_BTN_PARAM_TYPE);
        p_wk->isValid = func_ov186_021faf24(&p_wk->btn[p_wk->trgIdx], BR_BTN_PARAM_VALID);
        p_wk->data1 = func_ov186_021faf24(&p_wk->btn[p_wk->trgIdx], BR_BTN_PARAM_DATA1);
        p_wk->data2 = func_ov186_021faf24(&p_wk->btn[p_wk->trgIdx], BR_BTN_PARAM_DATA2);
        for (i = 0; i < p_wk->btn_num; i++) {
            if (i == p_wk->trgIdx) {
                func_ov186_021fafa0(&p_wk->btn[i], 1);
            } else {
                func_ov186_021fafa0(&p_wk->btn[i], 3);
            }
        }

        switch (p_wk->btnType) {
        case BR_BTN_TYPE_RETURN:
            func_ov186_021fd73c(p_seqwk, func_ov186_021fa758);
            break;
        case BR_BTN_TYPE_SELECT:
        case BR_BTN_TYPE_MENU:
        case BR_BTN_TYPE_CHANGE_DISPLAY:
            func_ov186_021fd73c(p_seqwk, func_ov186_021fa444);
            break;
        case BR_BTN_TYPE_EXIT:
            func_ov186_021fd73c(p_seqwk, func_ov186_021fab7c);
            break;
        default:
            func_ov186_021fd73c(p_seqwk, func_ov186_021faa34);
            break;
        }
    }
}

// pokebw2's BrBtnSys_Seq_Hide
void func_ov186_021fa444(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrBtnSys *p_wk = p_wk_adrs;
    int i;

    switch (*p_seq) {
    case 0: {
        BrPoint pos;

        for (i = 0; i < p_wk->btn_num; i++) {
            if (p_wk->trgIdx != i) {
                func_ov186_021faee4(&p_wk->btn[p_wk->trgIdx], &pos);
                func_ov186_021fad18(&p_wk->btn[i], BR_BTN_MOVE_TARGET, &pos);
            }
        }
        *p_seq = 1;
        break;
    }
    case 1: {
        BOOL isEnd = TRUE;

        for (i = 0; i < p_wk->btn_num; i++) {
            isEnd &= func_ov186_021fad74(&p_wk->btn[i]);
        }
        if (isEnd) {
            *p_seq = 2;
        }
        break;
    }
    case 2:
        // Both branches start the same sequence
        if (p_wk->btnType == BR_BTN_TYPE_CHANGE_DISPLAY) {
            func_ov186_021fd73c(p_seqwk, func_ov186_021fa500);
            return;
        } else {
            func_ov186_021fd73c(p_seqwk, func_ov186_021fa500);
        }
        break;
    }
}

// pokebw2's BrBtnSys_Seq_Change
void func_ov186_021fa500(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrBtnSys *p_wk = p_wk_adrs;
    int i;

    if (p_wk->btnType <= BR_BTN_TYPE_MENU) {
        BrPoint pos;
        u32 nextMenuID = p_wk->data1;

        func_ov186_021faee4(&p_wk->btn[p_wk->trgIdx], &pos);
        func_ov186_021fa020(p_wk, &p_wk->btn[p_wk->trgIdx], CLACT_SURFACE_SUB);
        func_ov186_021fa1dc(p_wk->recovery, func_ov186_021faf24(&p_wk->btn[p_wk->trgIdx], BR_BTN_PARAM_MENUID),
                           p_wk->trgIdx);
        MI_CpuFill8(&p_wk->btn[p_wk->trgIdx], 0, sizeof(BrBtnWork));
        pos.y = -32;
        func_ov186_021fa0fc(p_wk, nextMenuID, &pos);
        func_ov186_021fd73c(p_seqwk, func_ov186_021fa7fc);
    } else if (p_wk->btnType == BR_BTN_TYPE_RETURN) {
        BrBtnWork btn;

        func_ov186_021fa210(p_wk->recovery);
        if (func_ov186_021fa0a8(p_wk, &btn)) {
            u32 menuID = func_ov186_021faf24(&btn, BR_BTN_PARAM_MENUID);
            BrPoint pos;
            u32 y;

            GFL_ASSERT(btn.is_use);
            pos.x = func_ov186_021fb7b0(btn.data, BR_BTN_DATA_PARAM_X);
            pos.y = func_ov186_021fb7b0(btn.data, BR_BTN_DATA_PARAM_Y);
            func_ov186_021fa0fc(p_wk, menuID, &pos);

            y = func_ov186_021fb7b0(btn.data, BR_BTN_DATA_PARAM_Y);
            for (i = 0; i < p_wk->btn_num; i++) {
                if (y == func_ov186_021fb7b0(p_wk->btn[i].data, BR_BTN_DATA_PARAM_Y)) {
                    func_ov186_021fafa0(&p_wk->btn[i], 1);
                }
            }
            func_ov186_021face0(&btn);
            func_ov186_021fd73c(p_seqwk, func_ov186_021fa6a8);
        }
    } else {
        func_ov186_021fa020(p_wk, &p_wk->btn[p_wk->trgIdx], CLACT_SURFACE_SUB);
        func_ov186_021fa1dc(p_wk->recovery, func_ov186_021faf24(&p_wk->btn[p_wk->trgIdx], BR_BTN_PARAM_MENUID),
                           p_wk->trgIdx);
        MI_CpuFill8(&p_wk->btn[p_wk->trgIdx], 0, sizeof(BrBtnWork));
        for (i = 0; i < p_wk->btn_num; i++) {
            func_ov186_021face0(&p_wk->btn[i]);
        }
        func_ov186_021fd73c(p_seqwk, func_ov186_021fa7fc);
    }
}

// pokebw2's BrBtnSys_Seq_Show
void func_ov186_021fa6a8(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrBtnSys *p_wk = p_wk_adrs;
    int i;

    switch (*p_seq) {
    case 0:
        for (i = 0; i < p_wk->btn_num; i++) {
            func_ov186_021fad18(&p_wk->btn[i], BR_BTN_MOVE_RETURN, NULL);
        }
        *p_seq = 1;
        break;
    case 1: {
        BOOL isEnd = TRUE;

        for (i = 0; i < p_wk->btn_num; i++) {
            isEnd &= func_ov186_021fad74(&p_wk->btn[i]);
        }
        if (isEnd) {
            *p_seq = 2;
        }
        break;
    }
    case 2:
        switch (p_wk->btnType) {
        case BR_BTN_TYPE_SELECT:
        case BR_BTN_TYPE_MENU:
            func_ov186_021fd73c(p_seqwk, func_ov186_021faa34);
            break;
        case BR_BTN_TYPE_CHANGE_DISPLAY:
            func_ov186_021fd73c(p_seqwk, func_ov186_021fa7fc);
            break;
        default:
            func_ov186_021fd73c(p_seqwk, func_ov186_021faa34);
            break;
        }
        break;
    }
}

// pokebw2's BrBtnSys_Seq_Return
void func_ov186_021fa758(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrBtnSys *p_wk = p_wk_adrs;
    int i;

    switch (*p_seq) {
    case 0:
        for (i = 0; i < p_wk->btn_num; i++) {
            BrPoint pos;

            func_ov186_021faee4(&p_wk->btn[i], &pos);
            pos.y = 224;
            func_ov186_021fad18(&p_wk->btn[i], BR_BTN_MOVE_TARGET_SLOW, &pos);
        }
        *p_seq = 1;
        break;
    case 1: {
        BOOL isEnd = TRUE;

        for (i = 0; i < p_wk->btn_num; i++) {
            isEnd &= func_ov186_021fad74(&p_wk->btn[i]);
        }
        if (isEnd) {
            *p_seq = 2;
        }
        break;
    }
    case 2:
        func_ov186_021fd73c(p_seqwk, func_ov186_021fa920);
        break;
    }
}

// pokebw2's BrBtnSys_Seq_Stack
void func_ov186_021fa7fc(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrBtnSys *p_wk = p_wk_adrs;
    int i;

    switch (*p_seq) {
    case 0:
        for (i = 0; i < p_wk->btn_stack_num; i++) {
            if (i == p_wk->btn_stack_num - 1) {
                func_ov186_021fad18(&p_wk->btn_stack[i], BR_BTN_MOVE_TO_MAIN, &data_ov186_02200358[0]);
            } else {
                func_ov186_021fad18(&p_wk->btn_stack[i], BR_BTN_MOVE_TARGET_SLOW,
                                    &data_ov186_02200358[p_wk->btn_stack_num - i - 1]);
            }
        }
        *p_seq = 1;
        break;
    case 1: {
        BOOL isEnd = TRUE;

        for (i = 0; i < p_wk->btn_stack_num; i++) {
            isEnd &= func_ov186_021fad74(&p_wk->btn_stack[i]);
        }
        if (isEnd) {
            func_ov186_021fa21c(p_wk, 0);
            *p_seq = 2;
        }
        break;
    }
    case 2: {
        BrBtnWork *p_top = &p_wk->btn_stack[p_wk->btn_stack_num - 1];
        u32 type = func_ov186_021faf24(p_top, BR_BTN_PARAM_TYPE);
        u16 msgID = func_ov186_021faf24(p_top, BR_BTN_PARAM_DATA2);

        if (type == BR_BTN_TYPE_MENU) {
            if (p_wk->text == NULL) {
                p_wk->text = func_ov186_021fd12c(p_wk->res, p_wk->que, p_wk->heapId);
            }
            func_ov186_021fd170(p_wk->text, p_wk->res, msgID);
        }
        *p_seq = 3;
        break;
    }
    case 3:
        switch (p_wk->btnType) {
        case BR_BTN_TYPE_SELECT:
        case BR_BTN_TYPE_MENU:
            func_ov186_021fd73c(p_seqwk, func_ov186_021fa6a8);
            break;
        default:
            func_ov186_021fd73c(p_seqwk, func_ov186_021faa34);
        }
        break;
    }
}

// pokebw2's BrBtnSys_Seq_Unstack
void func_ov186_021fa920(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrBtnSys *p_wk = p_wk_adrs;
    int i;

    switch (*p_seq) {
    case 0:
        for (i = 0; i < p_wk->btn_stack_num; i++) {
            if (i == p_wk->btn_stack_num - 1) {
                BrPoint pos;

                pos.x = func_ov186_021fb7b0(p_wk->btn_stack[i].data, BR_BTN_DATA_PARAM_X);
                pos.y = func_ov186_021fb7b0(p_wk->btn_stack[i].data, BR_BTN_DATA_PARAM_Y);
                func_ov186_021fad18(&p_wk->btn_stack[i], BR_BTN_MOVE_TO_SUB, &pos);
                func_ov186_021fafa0(&p_wk->btn_stack[i], 1);
            } else {
                func_ov186_021fad18(&p_wk->btn_stack[i], BR_BTN_MOVE_TARGET_SLOW,
                                    &data_ov186_02200358[p_wk->btn_stack_num - i - 2]);
            }
        }
        for (i = 0; i < p_wk->btn_num; i++) {
            func_ov186_021fafa0(&p_wk->btn[i], 3);
        }
        *p_seq = 1;
        break;
    case 1: {
        BOOL isEnd = TRUE;

        for (i = 0; i < p_wk->btn_stack_num; i++) {
            isEnd &= func_ov186_021fad74(&p_wk->btn_stack[i]);
        }
        if (isEnd) {
            func_ov186_021fa21c(p_wk, 1);
            *p_seq = 2;
        }
        break;
    }
    case 2:
        func_ov186_021fd73c(p_seqwk, func_ov186_021fa500);
        break;
    }
}

// pokebw2's BrBtnSys_Seq_End
void func_ov186_021faa34(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrBtnSys *p_wk = p_wk_adrs;

    switch (*p_seq) {
    case 0:
        if (p_wk->isValid) {
            switch (p_wk->btnType) {
            case BR_BTN_TYPE_SELECT:
            case BR_BTN_TYPE_MENU:
            case BR_BTN_TYPE_RETURN:
                break;
            case BR_BTN_TYPE_EXIT:
                p_wk->input = BR_BTN_INPUT_EXIT;
                break;
            case BR_BTN_TYPE_CHANGE_DISPLAY:
                p_wk->input = BR_BTN_INPUT_SELECT;
                break;
            }
        }
        func_ov186_021fd73c(p_seqwk, func_ov186_021fa284);
        break;
    }
}

// pokebw2's BrBtnSys_Seq_Info
void func_ov186_021faa74(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrBtnSys *p_wk = p_wk_adrs;

    switch (*p_seq) {
    case 0: {
        u32 msgID = func_ov186_021faf24(&p_wk->btn[p_wk->trgIdx], BR_BTN_PARAM_UNVALID_DATA);

        if (p_wk->text == NULL) {
            p_wk->text = func_ov186_021fd12c(p_wk->res, p_wk->que, p_wk->heapId);
        }
        func_ov186_021fd170(p_wk->text, p_wk->res, msgID);
        *p_seq = 1;
        break;
    }
    case 1:
        *p_seq = 2;
        break;
    case 2:
        func_ov186_021fd73c(p_seqwk, func_ov186_021fa284);
        break;
    }
}

// pokebw2's BrBtnSys_Seq_Open
void func_ov186_021faad0(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrBtnSys *p_wk = p_wk_adrs;
    int i;

    switch (*p_seq) {
    case 0:
        for (i = 0; i < p_wk->btn_num; i++) {
            BrPoint pos;

            pos.x = func_ov186_021faf24(&p_wk->btn[i], BR_BTN_PARAM_X);
            pos.y = func_ov186_021faf24(&p_wk->btn[i], BR_BTN_PARAM_Y);
            func_ov186_021fad18(&p_wk->btn[i], BR_BTN_MOVE_TARGET_SLOW, &pos);
        }
        *p_seq = 1;
        break;
    case 1: {
        BOOL isEnd = TRUE;

        for (i = 0; i < p_wk->btn_num; i++) {
            isEnd &= func_ov186_021fad74(&p_wk->btn[i]);
        }
        if (isEnd) {
            *p_seq = 2;
        }
        break;
    }
    case 2:
        func_ov186_021fd73c(p_seqwk, func_ov186_021fa274);
        break;
    }
}

// pokebw2's BrBtnSys_Seq_Close
void func_ov186_021fab7c(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrBtnSys *p_wk = p_wk_adrs;
    int i;

    switch (*p_seq) {
    case 0:
        for (i = 0; i < p_wk->btn_num; i++) {
            BrPoint pos;

            func_ov186_021faee4(&p_wk->btn[i], &pos);
            pos.y = 224;
            func_ov186_021fad18(&p_wk->btn[i], BR_BTN_MOVE_TARGET_SLOW, &pos);
        }
        *p_seq = 1;
        break;
    case 1: {
        BOOL isEnd = TRUE;

        for (i = 0; i < p_wk->btn_num; i++) {
            isEnd &= func_ov186_021fad74(&p_wk->btn[i]);
        }
        if (isEnd) {
            *p_seq = 2;
        }
        break;
    }
    case 2:
        func_ov186_021fd73c(p_seqwk, func_ov186_021faa34);
        break;
    }
}

void func_ov186_021fac20(BrBtnWork *p_wk, const BrBtnData *cp_data, ClActUnit *unit, BmpOamSys *bmpoam, BrRes *res,
                           const StrBuf *str, u32 objID, HeapID heapId) {
    GFL_ASSERT(p_wk->is_use == FALSE);
    MI_CpuFill8(p_wk, 0, sizeof(BrBtnWork));
    p_wk->is_use = TRUE;
    p_wk->display = CLACT_SURFACE_SUB;
    p_wk->data = cp_data;
    p_wk->objID = objID;
    p_wk->res = res;
    p_wk->heapId = heapId;
    p_wk->unit = unit;
    p_wk->bmpoam = bmpoam;
    p_wk->str = GFL_StrBufClone(str, heapId);
    {
        ClActorSetup setup;
        BrResObjData obj;

        MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
        setup.x = func_ov186_021fb7b0(p_wk->data, BR_BTN_DATA_PARAM_X);
        setup.y = func_ov186_021fb7b0(p_wk->data, BR_BTN_DATA_PARAM_Y);
        setup.sequence = func_ov186_021fb7b0(p_wk->data, BR_BTN_DATA_PARAM_ANMSEQ);
        setup.priority = 1;
        func_ov186_021fc124(res, p_wk->objID + 1, &obj);
        p_wk->btn = func_ov186_021fb3bc(&setup, p_wk->str, 160, p_wk->display, unit, bmpoam, func_ov186_021fc20c(res), &obj,
                               HEAPID_TAIL(heapId));
    }
}

// pokebw2's BrBtnWork_Exit
void func_ov186_021face0(BrBtnWork *p_wk) {
    if (p_wk->is_use) {
        GFL_StrBufFree(p_wk->str);
        func_ov186_021fb494(p_wk->btn);
        MI_CpuFill8(p_wk, 0, sizeof(BrBtnWork));
    }
}

// pokebw2's BrBtnWork_GetTrg
BOOL func_ov186_021fad04(const BrBtnWork *cp_wk, u32 x, u32 y) {
    if (cp_wk->is_use) {
        return func_ov186_021fb4cc(cp_wk->btn, x, y);
    }
    return FALSE;
}

// pokebw2's BrBtnWork_StartMove
void func_ov186_021fad18(BrBtnWork *p_wk, u32 type, const BrPoint *cp_pos) {
    p_wk->seq = 0;
    if (cp_pos != NULL) {
        p_wk->target = *cp_pos;
    }
    switch (type) {
    case BR_BTN_MOVE_TARGET:
        p_wk->moveFunc = func_ov186_021fafd0;
        break;
    case BR_BTN_MOVE_RETURN:
        p_wk->moveFunc = func_ov186_021fb058;
        break;
    case BR_BTN_MOVE_TO_MAIN:
        p_wk->moveFunc = func_ov186_021fb0e8;
        break;
    case BR_BTN_MOVE_TO_SUB:
        p_wk->moveFunc = func_ov186_021fb1ec;
        break;
    case BR_BTN_MOVE_TARGET_SLOW:
        p_wk->moveFunc = func_ov186_021fb2f0;
        break;
    }
}

// pokebw2's BrBtnWork_MainMove
BOOL func_ov186_021fad74(BrBtnWork *p_wk) {
    if (p_wk->moveFunc != NULL) {
        BOOL ret = p_wk->moveFunc(p_wk);

        if (ret) {
            p_wk->moveFunc = NULL;
        }
        if (p_wk->follow != NULL) {
            s16 x, y;

            func_ov186_021fb548(p_wk->btn, &x, &y);
            p_wk->follow->x = x + 80;
            p_wk->follow->y = y;
        }
        return ret;
    }
    return TRUE;
}

// pokebw2's BrBtnWork_ChangeDisplay
void func_ov186_021fadb8(BrBtnWork *p_wk, u32 display) {
    if (p_wk->display != display) {
        s16 x, y;
        u8 softPriority, bgPriority;
        s32 ofs;
        ClActorSetup setup;
        BrResObjData obj;
        Font *font;

        p_wk->display = display;
        func_ov186_021fb548(p_wk->btn, &x, &y);
        softPriority = func_ov186_021fb594(p_wk->btn);
        bgPriority = func_ov186_021fb5b8(p_wk->btn);
        func_ov186_021fb494(p_wk->btn);
        p_wk->follow = NULL;

        if (display == CLACT_SURFACE_MAIN) {
            ofs = 512;
        } else {
            ofs = -512;
        }
        MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
        setup.x = x;
        setup.y = y + ofs;
        setup.sequence = func_ov186_021fb7b0(p_wk->data, BR_BTN_DATA_PARAM_ANMSEQ);
        setup.priority = softPriority;
        setup.bgPriority = bgPriority;
        func_ov186_021fc124(p_wk->res, p_wk->objID + display, &obj);
        font = func_ov186_021fc20c(p_wk->res);
        // The message data and the label's message ID are read and left unused: the label is kept
        func_ov186_021fc214(p_wk->res);
        func_ov186_021fb7b0(p_wk->data, BR_BTN_DATA_PARAM_MSGID);
        p_wk->btn = func_ov186_021fb3bc(&setup, p_wk->str, 160, display, p_wk->unit, p_wk->bmpoam, font, &obj, p_wk->heapId);
    }
}

// pokebw2's BrBtnWork_Copy
void func_ov186_021fae70(const BrBtnSys *cp_sys, BrBtnWork *p_dst, const BrBtnWork *cp_src, u32 display) {
    GFL_ASSERT(p_dst->is_use == FALSE);
    GFL_ASSERT(cp_src->is_use == TRUE);
    *p_dst = *cp_src;
    if (cp_src->display != display) {
        func_ov186_021fadb8(p_dst, display);
    }
}

// pokebw2's BrBtnWork_SetStackPos
void func_ov186_021faec0(BrBtnWork *p_wk, u16 idx) {
    s16 x = func_ov186_021fb7b0(p_wk->data, BR_BTN_DATA_PARAM_X);

    func_ov186_021fb514(p_wk->btn, x, 169 - idx * 16);
}

// pokebw2's BrBtnWork_GetPos
void func_ov186_021faee4(const BrBtnWork *cp_wk, BrPoint *p_pos) {
    s16 x, y;

    func_ov186_021fb548(cp_wk->btn, &x, &y);
    p_pos->x = x;
    p_pos->y = y;
}

// pokebw2's BrBtnWork_SetPosY
void func_ov186_021faf08(BrBtnWork *p_wk, s16 y) {
    s16 x = func_ov186_021fb7b0(p_wk->data, BR_BTN_DATA_PARAM_X);

    func_ov186_021fb514(p_wk->btn, x, y);
}

// pokebw2's BrBtnWork_GetParam
u32 func_ov186_021faf24(const BrBtnWork *cp_wk, u32 paramID) {
    u32 ret;

    switch (paramID) {
    case BR_BTN_PARAM_TYPE:
        ret = func_ov186_021fb7b0(cp_wk->data, BR_BTN_DATA_PARAM_TYPE);
        break;
    case BR_BTN_PARAM_DATA1:
        ret = func_ov186_021fb7b0(cp_wk->data, BR_BTN_DATA_PARAM_DATA1);
        break;
    case BR_BTN_PARAM_DATA2:
        ret = func_ov186_021fb7b0(cp_wk->data, BR_BTN_DATA_PARAM_DATA2);
        break;
    case BR_BTN_PARAM_UNVALID_TYPE:
        ret = func_ov186_021fb7b0(cp_wk->data, BR_BTN_DATA_PARAM_UNVALID_TYPE);
        break;
    case BR_BTN_PARAM_UNVALID_DATA:
        ret = func_ov186_021fb7b0(cp_wk->data, BR_BTN_DATA_PARAM_UNVALID_DATA);
        break;
    case BR_BTN_PARAM_VALID:
        ret = func_ov186_021fb7b0(cp_wk->data, BR_BTN_DATA_PARAM_VALID);
        break;
    case BR_BTN_PARAM_MENUID:
        ret = func_ov186_021fb7b0(cp_wk->data, BR_BTN_DATA_PARAM_MENUID);
        break;
    case BR_BTN_PARAM_VALID_TYPE:
        ret = func_ov186_021fb7b0(cp_wk->data, BR_BTN_DATA_PARAM_VALID_TYPE);
        break;
    case BR_BTN_PARAM_X:
        ret = func_ov186_021fb7b0(cp_wk->data, BR_BTN_DATA_PARAM_X);
        break;
    case BR_BTN_PARAM_Y:
        ret = func_ov186_021fb7b0(cp_wk->data, BR_BTN_DATA_PARAM_Y);
        break;
    default:
        GFL_ASSERT(0);
        ret = 0;
    }
    return ret;
}

// pokebw2's BrBtnWork_SetSoftPriority
void func_ov186_021fafa0(BrBtnWork *p_wk, u16 priority) {
    func_ov186_021fb578(p_wk->btn, priority);
}

// pokebw2's BrBtnWork_SetBgPriority
void func_ov186_021fafb0(BrBtnWork *p_wk, u32 priority) {
    func_ov186_021fb5a0(p_wk->btn, priority);
}

// pokebw2's BrBtnWork_SetPalette
void func_ov186_021fafc0(BrBtnWork *p_wk, u32 plt) {
    func_ov186_021fb5c4(p_wk->btn, plt);
}

// pokebw2's BrBtnWork_SetFollow
void func_ov186_021fafcc(BrBtnWork *p_wk, BrPoint *p_pos) {
    p_wk->follow = p_pos;
}

// pokebw2's BrBtnWork_Move_Target
BOOL func_ov186_021fafd0(BrBtnWork *p_wk) {
    switch (p_wk->seq) {
    case 0: {
        BrPoint pos;

        func_ov186_021faee4(p_wk, &pos);
        p_wk->start = pos.y;
        p_wk->end = p_wk->target.y;
        p_wk->count = 0;
        p_wk->max = 10;
        p_wk->seq = 1;
        break;
    }
    case 1: {
        s16 y = p_wk->start + (p_wk->end - p_wk->start) * p_wk->count / p_wk->max;
        s16 x;

        func_ov186_021fb548(p_wk->btn, &x, NULL);
        func_ov186_021fb514(p_wk->btn, x, y);
        if (p_wk->count++ >= p_wk->max) {
            p_wk->seq = 2;
        }
        break;
    }
    case 2:
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BrBtnWork_Move_Return
BOOL func_ov186_021fb058(BrBtnWork *p_wk) {
    switch (p_wk->seq) {
    case 0: {
        BrPoint pos;
        s16 end;

        func_ov186_021faee4(p_wk, &pos);
        end = func_ov186_021fb7b0(p_wk->data, BR_BTN_DATA_PARAM_Y);
        p_wk->start = pos.y;
        p_wk->end = end;
        p_wk->count = 0;
        p_wk->max = 16;
        p_wk->seq = 1;
        break;
    }
    case 1: {
        s16 y = p_wk->start + (p_wk->end - p_wk->start) * p_wk->count / p_wk->max;
        s16 x;

        func_ov186_021fb548(p_wk->btn, &x, NULL);
        func_ov186_021fb514(p_wk->btn, x, y);
        if (p_wk->count++ >= p_wk->max) {
            p_wk->seq = 2;
        }
        break;
    }
    case 2:
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BrBtnWork_Move_ToMain
BOOL func_ov186_021fb0e8(BrBtnWork *p_wk) {
    switch (p_wk->seq) {
    case 0: {
        BrPoint pos;

        func_ov186_021faee4(p_wk, &pos);
        p_wk->start = pos.y;
        p_wk->end = -32;
        p_wk->count = 0;
        p_wk->max = 14;
        p_wk->seq = 1;
        break;
    }
    case 1: {
        s16 y = p_wk->start + (p_wk->end - p_wk->start) * p_wk->count / p_wk->max;
        s16 x;

        func_ov186_021fb548(p_wk->btn, &x, NULL);
        func_ov186_021fb514(p_wk->btn, x, y);
        if (p_wk->count++ >= p_wk->max) {
            p_wk->seq = 2;
        }
        break;
    }
    case 2:
        func_ov186_021fadb8(p_wk, CLACT_SURFACE_MAIN);
        p_wk->seq = 3;
        break;
    case 3: {
        BrPoint pos;

        func_ov186_021faee4(p_wk, &pos);
        p_wk->start = pos.y;
        p_wk->end = p_wk->target.y;
        p_wk->count = 0;
        p_wk->max = 14;
        p_wk->seq = 4;
        break;
    }
    case 4: {
        s16 y = p_wk->start + (p_wk->end - p_wk->start) * p_wk->count / p_wk->max;
        s16 x;

        func_ov186_021fb548(p_wk->btn, &x, NULL);
        func_ov186_021fb514(p_wk->btn, x, y);
        if (p_wk->count++ >= p_wk->max) {
            p_wk->seq = 5;
        }
        break;
    }
    case 5:
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BrBtnWork_Move_ToSub
BOOL func_ov186_021fb1ec(BrBtnWork *p_wk) {
    switch (p_wk->seq) {
    case 0: {
        BrPoint pos;

        func_ov186_021faee4(p_wk, &pos);
        p_wk->start = pos.y;
        p_wk->end = 224;
        p_wk->count = 0;
        p_wk->max = 14;
        p_wk->seq = 1;
        break;
    }
    case 1: {
        s16 y = p_wk->start + (p_wk->end - p_wk->start) * p_wk->count / p_wk->max;
        s16 x;

        func_ov186_021fb548(p_wk->btn, &x, NULL);
        func_ov186_021fb514(p_wk->btn, x, y);
        if (p_wk->count++ >= p_wk->max) {
            p_wk->seq = 2;
        }
        break;
    }
    case 2:
        func_ov186_021fadb8(p_wk, CLACT_SURFACE_SUB);
        p_wk->seq = 3;
        break;
    case 3: {
        BrPoint pos;

        func_ov186_021faee4(p_wk, &pos);
        p_wk->start = pos.y;
        p_wk->end = p_wk->target.y;
        p_wk->count = 0;
        p_wk->max = 14;
        p_wk->seq = 4;
        break;
    }
    case 4: {
        s16 y = p_wk->start + (p_wk->end - p_wk->start) * p_wk->count / p_wk->max;
        s16 x;

        func_ov186_021fb548(p_wk->btn, &x, NULL);
        func_ov186_021fb514(p_wk->btn, x, y);
        if (p_wk->count++ >= p_wk->max) {
            p_wk->seq = 5;
        }
        break;
    }
    case 5:
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BrBtnWork_Move_TargetSlow
BOOL func_ov186_021fb2f0(BrBtnWork *p_wk) {
    switch (p_wk->seq) {
    case 0: {
        BrPoint pos;

        func_ov186_021faee4(p_wk, &pos);
        p_wk->start = pos.y;
        p_wk->end = p_wk->target.y;
        p_wk->count = 0;
        p_wk->max = 26;
        p_wk->seq = 1;
        break;
    }
    case 1: {
        s16 y = p_wk->start + (p_wk->end - p_wk->start) * p_wk->count / p_wk->max;
        s16 x;

        func_ov186_021fb548(p_wk->btn, &x, NULL);
        func_ov186_021fb514(p_wk->btn, x, y);
        if (p_wk->count++ >= p_wk->max) {
            p_wk->seq = 2;
        }
        break;
    }
    case 2:
        return TRUE;
    }
    return FALSE;
}

BrBtn *func_ov186_021fb378(const ClActorSetup *setup, u16 msgID, u16 width, u32 display, ClActUnit *unit, BmpOamSys *bmpoam,
                    Font *font, MsgData *msg, const BrResObjData *obj, HeapID heapId) {
    StrBuf *str = GFL_MsgDataLoadStrbufNew(msg, msgID);
    BrBtn *p_wk = func_ov186_021fb3bc(setup, str, width, display, unit, bmpoam, font, obj, heapId);

    GFL_StrBufFree(str);
    return p_wk;
}

BrBtn *func_ov186_021fb3bc(const ClActorSetup *setup, const StrBuf *str, u16 width, u32 display, ClActUnit *unit,
                  BmpOamSys *bmpoam, Font *font, const BrResObjData *obj, HeapID heapId) {
    BrBtn *p_wk = GFL_HeapAllocate(heapId, sizeof(BrBtn), FALSE, "br_btn.c", 2619);
    BmpOamActorSetup actorSetup;

    MI_CpuFill8(p_wk, 0, sizeof(BrBtn));
    p_wk->width = width;
    p_wk->display = display;
    p_wk->clwk = func_0204b294(unit, obj->chr, obj->plt, obj->cell, setup, display, heapId);
    func_0204b688(p_wk->clwk, TRUE);
    p_wk->bmp = GFL_BitmapCreate(p_wk->width / 8, 2, 0x20, heapId);
    GFL_TextRendererDrawToBitmapEx(p_wk->bmp, 1, 0, str, font, 0x3da0);

    MI_CpuFill8(&actorSetup, 0, sizeof(BmpOamActorSetup));
    actorSetup.bitmap = p_wk->bmp;
    actorSetup.x = setup->x + 32;
    actorSetup.y = setup->y - 8;
    actorSetup.palette = obj->plt;
    actorSetup.priority = setup->priority - 1;
    actorSetup.surface = display;
    actorSetup.vramType = display;
    actorSetup.bgPriority = setup->bgPriority;
    actorSetup.paletteOffset = 1;
    p_wk->oam = func_020255a0(bmpoam, &actorSetup);
    func_020257d0(p_wk->oam);
    func_0202597c(p_wk->oam, GX_OAM_MODE_XLU);
    return p_wk;
}

// pokebw2's BrBtn_Exit
void func_ov186_021fb494(BrBtn *p_wk) {
    func_0202570c(p_wk->oam);
    GFL_BitmapFree(p_wk->bmp);
    func_0204b3b4(p_wk->clwk);
    GFL_HeapFree(p_wk);
}

// pokebw2's BrBtn_GetTrg
BOOL func_ov186_021fb4b4(const BrBtn *cp_wk, u32 x, u32 y) {
    BOOL ret = func_ov186_021fb4cc(cp_wk, x, y);

    if (ret) {
        GFL_SndSEPlay(0x703);
    }
    return ret;
}

// Whether the point is on the button, each range tested as one unsigned comparison
// pokebw2's BrBtn_GetHit
BOOL func_ov186_021fb4cc(const BrBtn *cp_wk, u32 x, u32 y) {
    ClActorPos pos;
    s32 left, right, top, bottom;

    func_0204b454(cp_wk->clwk, &pos, cp_wk->display);
    left = pos.x;
    right = pos.x + cp_wk->width;
    top = pos.y - 16;
    bottom = pos.y + 16;
    return ((u32)(x - left) <= (u32)(right - left)) & ((u32)(y - top) <= (u32)(bottom - top));
}

// pokebw2's BrBtn_SetPos
void func_ov186_021fb514(BrBtn *p_wk, s16 x, s16 y) {
    ClActorPos pos;
    pos.x = x;
    pos.y = y;
    func_0204b404(p_wk->clwk, &pos, p_wk->display);
    func_0202590c(p_wk->oam, pos.x + 0x20, pos.y - 8);
}

// pokebw2's BrBtn_GetPos
void func_ov186_021fb548(const BrBtn *cp_wk, s16 *p_x, s16 *p_y) {
    ClActorPos pos;
    func_0204b454(cp_wk->clwk, &pos, cp_wk->display);
    if (p_x != NULL) {
        *p_x = pos.x;
    }
    if (p_y != NULL) {
        *p_y = pos.y;
    }
}

// pokebw2's BrBtn_SetSoftPriority
void func_ov186_021fb578(BrBtn *p_wk, u8 priority) {
    func_0204b800(p_wk->clwk, priority);
    func_020259c4(p_wk->oam, priority - 1);
}

// pokebw2's BrBtn_GetSoftPriority
u8 func_ov186_021fb594(const BrBtn *cp_wk) {
    return func_0204b838(cp_wk->clwk);
}

// pokebw2's BrBtn_SetBgPriority
void func_ov186_021fb5a0(BrBtn *p_wk, u8 priority) {
    func_0204b84c(p_wk->clwk, priority);
    func_02025a0c(p_wk->oam, priority);
}

// pokebw2's BrBtn_GetBgPriority
u8 func_ov186_021fb5b8(const BrBtn *cp_wk) {
    return func_0204b864(cp_wk->clwk);
}

// pokebw2's BrBtn_SetPalette
void func_ov186_021fb5c4(BrBtn *p_wk, u32 plt) {
    func_0204b6f4(p_wk->clwk, (u8)plt, 0);
    func_02025a54(p_wk->oam, (u8)(plt + 1));
}
