// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/battle_recorder/br_util.h, the lines in
// use so far.
#ifndef BWPORT_APP_BATTLE_RECORDER_BR_UTIL_H
#define BWPORT_APP_BATTLE_RECORDER_BR_UTIL_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "system/printsys.h"

// The Battle Recorder's utilities (br_util.c): message windows, lists, the profile of a video's player, sequences
// and the ball effect. pokebw2's guessed names are in the comments

// A position on the screen
typedef struct {
    s32 x;
    s32 y;
} BrPoint;

// pokebw2's BrText_Init, BrText_Exit, BrText_Print and BrText_Main
BrMsgWin *func_ov186_021fd12c(BrRes *res, PrintQueue *que, HeapID heapId);
void func_ov186_021fd15c(BrMsgWin *p_wk, BrRes *res);
void func_ov186_021fd170(BrMsgWin *p_wk, BrRes *res, u32 msgID);
BOOL func_ov186_021fd1bc(BrMsgWin *p_wk);

// A step of a sequence, which runs every frame until it sets the next one
typedef void (*BrSeqFunc)(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);

// pokebw2's BrSeq_Init, BrSeq_Exit, BrSeq_Main, BrSeq_IsEnd, BrSeq_SetNext and BrSeq_End
BrSeq *func_ov186_021fd6d8(void *p_wk_adrs, BrSeqFunc seq_function, HeapID heapId);
void func_ov186_021fd714(BrSeq *p_wk);
void func_ov186_021fd71c(BrSeq *p_wk);
BOOL func_ov186_021fd72c(const BrSeq *cp_wk);
void func_ov186_021fd73c(BrSeq *p_wk, BrSeqFunc seq_function);
void func_ov186_021fd744(BrSeq *p_wk);

// The balls that circle a touch or a cursor
enum {
    BR_BALL_EFFECT_NONE,
    BR_BALL_EFFECT_SPREAD,
    BR_BALL_EFFECT_LINE,
    BR_BALL_EFFECT_LINE_ROT,
    BR_BALL_EFFECT_ROT,
    BR_BALL_EFFECT_ROT_SMALL,
    BR_BALL_EFFECT_STOP,
    BR_BALL_EFFECT_TOUCH,
    BR_BALL_EFFECT_RING,
};

// pokebw2's BrBallEff_Init, BrBallEff_Exit, BrBallEff_Main and BrBallEff_Start
BrBallEffect *func_ov186_021fd760(ClActUnit *unit, BrRes *res, u32 display, HeapID heapId);
void func_ov186_021fd820(BrBallEffect *p_wk);
void func_ov186_021fd854(BrBallEffect *p_wk);
void func_ov186_021fd860(BrBallEffect *p_wk, u32 type, const BrPoint *pos);

// pokebw2's BrText_PrintBuf
void func_ov186_021fd19c(BrMsgWin *p_wk, BrRes *res, const StrBuf *str);

// pokebw2's BrMsgWin_Main
// TRUE once the text is printed
BOOL func_ov186_021fc830(BrMsgWin *p_wk);
// pokebw2's BrMsgWin_Init
BrMsgWin *func_ov186_021fc6a4(u16 frame, u8 x, u8 y, u8 w, u8 h, u8 plt, PrintQueue *que, HeapID heapId);
// pokebw2's BrMsgWin_Print
void func_ov186_021fc754(BrMsgWin *p_wk, MsgData *msg, u32 strID, Font *font, u16 color);
// pokebw2's BrMsgWin_Exit
void func_ov186_021fc734(BrMsgWin *p_wk);

// pokebw2's BrBallEff_SetAnmSeq
void func_ov186_021fd940(BrBallEffect *p_wk, u32 seq);
BOOL func_ov186_021fd938(const BrBallEffect *cp_wk);

// How BrMsgWin_SetPos places the text
enum {
    BR_MSGWIN_POS_ABSOLUTE,
    BR_MSGWIN_POS_WH_CENTER,
};

// Black 2's, not checked against Black
// A list of strings in a window, scrolled by touch or with a cursor
enum {
    BR_LIST_TYPE_TOUCH,
    BR_LIST_TYPE_CURSOR,
};

// Where a list was scrolled to, kept to open it there again
struct BrListPos {
    u16 list;
    u16 cursor;
};

// Black 2's; twinned code confirms the offsets of cp_list, list_max, w, frame, str_line, res, unit, ballCursor,
// ballDecide and pos in Black
typedef struct {
    const BrListItem *cp_list;
    u32 list_max;
    u8 x;
    u8 y;
    u8 w;
    u8 h;
    u8 plt;
    u8 frame;
    // The height of an item, in characters
    u8 str_line;
    u32 type;
    BrRes *res;
    ClActUnit *unit;
    BrBallEffect *ballCursor;
    BrBallEffect *ballDecide;
    BrListPos *pos;
} BrListParam;

// Black 2's; twinned code confirms the values of BR_LIST_PARAM_LIST, BR_LIST_PARAM_SLIDE and BR_LIST_PARAM_LISTMAX in
// Black
// What BrList_GetParam returns
enum {
    BR_LIST_PARAM_CURSOR,
    BR_LIST_PARAM_LIST,
    BR_LIST_PARAM_SLIDE,
    BR_LIST_PARAM_LISTMAX,
};

// pokebw2's BrList_Write
void func_ov186_021fce50(BrList *p_wk);
// pokebw2's BrList_Exit
void func_ov186_021fcc80(BrList *p_wk);
// pokebw2's BrList_Main
void func_ov186_021fcd0c(BrList *p_wk);

// pokebw2's BrMsgWin_SetPos
void func_ov186_021fc868(BrMsgWin *p_wk, s32 x, s32 y, u32 type);
// pokebw2's BrMsgWin_PrintBuf
void func_ov186_021fc7c4(BrMsgWin *p_wk, const StrBuf *str, Font *font, u16 color);

#define BR_LIST_SELECT_NONE 0xffffffff
// The profile of a video's player: the name, birthday, home, introduction, Pokémon and trainer
enum {
    BR_PROFILE_TYPE_TRAINER,
    BR_PROFILE_TYPE_UNION,
};

// pokebw2's BrProfile_Main
// TRUE once everything is printed
BOOL func_ov186_021fd6b0(BrProfile *p_wk);
// pokebw2's BrProfile_Exit
void func_ov186_021fd640(BrProfile *p_wk);

struct BrListItem {
    // NULL for an item whose bitmap BrList_SetBmp gives
    const StrBuf *str;
    u32 param;
};

// pokebw2's BrList_IsMove
BOOL func_ov186_021fcda0(BrList *p_wk);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BrList_Init
BrList *func_ov186_021fc8c8(const BrListParam *cp_param, HeapID heapId);
// pokebw2's BrList_SetBmp
void func_ov186_021fcdac(BrList *p_wk, u32 idx, GFLBitmap *bmp);
// pokebw2's BrList_GetParam
u32 func_ov186_021fce0c(const BrList *cp_wk, u32 paramID);
// pokebw2's BrList_GetBmp
GFLBitmap *func_ov186_021fcde8(const BrList *cp_wk, u32 idx);
// pokebw2's BrList_GetSelect
// The selected item's param, or BR_LIST_SELECT_NONE
u32 func_ov186_021fcda8(const BrList *cp_wk);
// pokebw2's BrProfile_Init
BrProfile *func_ov186_021fd1c4(GdsProfile *cp_profile, BrRes *res, ClActUnit *unit, PrintQueue *que, u32 type,
                          HeapID heapId);
// pokebw2's BrSeq_IsComp
BOOL func_ov186_021fd750(const BrSeq *cp_wk, BrSeqFunc seq_function);

#endif // BWPORT_APP_BATTLE_RECORDER_BR_UTIL_H
