#ifndef POKEBW2_APP_BATTLE_RECORDER_BR_UTIL_H
#define POKEBW2_APP_BATTLE_RECORDER_BR_UTIL_H

#include "types.h"
#include "app/battle_recorder/br_res.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "save/gds_profile.h"
#include "struct_decls.h"
#include "system/printsys.h"

// The Battle Recorder's utilities (br_util.c): message windows, lists, the profile of a video's player, sequences
// and the ball effect

// A position on the screen
typedef struct {
    s32 x;
    s32 y;
} BrPoint;

// How BrMsgWin_SetPos places the text
enum {
    BR_MSGWIN_POS_ABSOLUTE,
    BR_MSGWIN_POS_WH_CENTER,
};

BrMsgWin *BrMsgWin_Init(u16 frame, u8 x, u8 y, u8 w, u8 h, u8 plt, PrintQueue *que, HeapID heapId);
void BrMsgWin_Exit(BrMsgWin *p_wk);
void BrMsgWin_Print(BrMsgWin *p_wk, MsgData *msg, u32 strID, Font *font, u16 color);
void BrMsgWin_PrintBuf(BrMsgWin *p_wk, const StrBuf *str, Font *font, u16 color);
// TRUE once the text is printed
BOOL BrMsgWin_Main(BrMsgWin *p_wk);
void BrMsgWin_SetPos(BrMsgWin *p_wk, s32 x, s32 y, u32 type);

// A list of strings in a window, scrolled by touch or with a cursor
enum {
    BR_LIST_TYPE_TOUCH,
    BR_LIST_TYPE_CURSOR,
};

#define BR_LIST_SELECT_NONE 0xffffffff

typedef struct {
    // NULL for an item whose bitmap BrList_SetBmp gives
    const StrBuf *str;
    u32 param;
} BrListItem;

// Where a list was scrolled to, kept to open it there again
typedef struct {
    u16 list;
    u16 cursor;
} BrListPos;

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

// What BrList_GetParam returns
enum {
    BR_LIST_PARAM_CURSOR,
    BR_LIST_PARAM_LIST,
    BR_LIST_PARAM_SLIDE,
    BR_LIST_PARAM_LISTMAX,
};

BrList *BrList_Init(const BrListParam *cp_param, HeapID heapId);
void BrList_Exit(BrList *p_wk);
void BrList_Main(BrList *p_wk);
BOOL BrList_IsMove(BrList *p_wk);
// The selected item's param, or BR_LIST_SELECT_NONE
u32 BrList_GetSelect(const BrList *cp_wk);
void BrList_SetBmp(BrList *p_wk, u32 idx, GFLBitmap *bmp);
GFLBitmap *BrList_GetBmp(const BrList *cp_wk, u32 idx);
u32 BrList_GetParam(const BrList *cp_wk, u32 paramID);
void BrList_Write(BrList *p_wk);

// The message window of the main screen's info text
BrMsgWin *BrText_Init(BrRes *res, PrintQueue *que, HeapID heapId);
void BrText_Exit(BrMsgWin *p_wk, BrRes *res);
void BrText_Print(BrMsgWin *p_wk, BrRes *res, u32 msgID);
void BrText_PrintBuf(BrMsgWin *p_wk, BrRes *res, const StrBuf *str);
BOOL BrText_Main(BrMsgWin *p_wk);

// The profile of a video's player: the name, birthday, home, introduction, Pokémon and trainer
enum {
    BR_PROFILE_TYPE_TRAINER,
    BR_PROFILE_TYPE_UNION,
};

BrProfile *BrProfile_Init(GdsProfile *cp_profile, BrRes *res, ClActUnit *unit, PrintQueue *que, u32 type,
                          HeapID heapId);
void BrProfile_Exit(BrProfile *p_wk);
// TRUE once everything is printed
BOOL BrProfile_Main(BrProfile *p_wk);

// A step of a sequence, which runs every frame until it sets the next one
typedef void (*BrSeqFunc)(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);

BrSeq *BrSeq_Init(void *p_wk_adrs, BrSeqFunc seq_function, HeapID heapId);
void BrSeq_Exit(BrSeq *p_wk);
void BrSeq_Main(BrSeq *p_wk);
BOOL BrSeq_IsEnd(const BrSeq *cp_wk);
void BrSeq_SetNext(BrSeq *p_wk, BrSeqFunc seq_function);
void BrSeq_End(BrSeq *p_wk);
BOOL BrSeq_IsComp(const BrSeq *cp_wk, BrSeqFunc seq_function);

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

BrBallEffect *BrBallEff_Init(ClActUnit *unit, BrRes *res, u32 display, HeapID heapId);
void BrBallEff_Exit(BrBallEffect *p_wk);
void BrBallEff_Main(BrBallEffect *p_wk);
// Starts an effect at pos, which the balls follow if the effect moves back to it
void BrBallEff_Start(BrBallEffect *p_wk, u32 type, const BrPoint *pos);
BOOL BrBallEff_IsEnd(const BrBallEffect *cp_wk);
void BrBallEff_SetAnmSeq(BrBallEffect *p_wk, u32 seq);

#endif // POKEBW2_APP_BATTLE_RECORDER_BR_UTIL_H
