// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov271/br_util.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Battle Recorder's utilities: message windows, lists of strings, the profile of a video's player, the sequences
// its screens step through, and the ball effect that circles a touch. The name is the ROM's string, from
// GFL_HeapAllocate's asserts

#include "types.h"
#include "app/battle_recorder/br_util.h"
#include "gfl/arc.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/fx/fx_vec.h"
#include "nitro/math.h"
#include "pml/poke_party.h"
#include "system/pms_data.h"
#include "system/pms_draw.h"
#include "system/wordset.h"
#include "app/battle_recorder/br_res.h"
#include "save/gds_profile.h"

struct BrMsgWin {
    PrintWindow print;
    PrintQueue *que;
    BmpWin *bmpwin;
    StrBuf *str;
    u16 clear_color;
    u16 frame;
    u32 pos_type;
    BrPoint pos;
};

#define BR_LIST_HITTBL_MAX 10

typedef BOOL (*BrListMoveFunc)(BrList *p_wk, s32 dir);
typedef BOOL (*BrListIsMoveFunc)(BrList *p_wk);
typedef void (*BrListDecideFunc)(BrList *p_wk);

struct BrList {
    BrListParam param;
    // The scroll arrows, then the sidebar's arrows
    ClActor *clwk[4];
    BmpWin *bmpwin;
    u16 unk3C;
    u16 list_max;
    u16 cursor;
    u16 list;
    u16 line_max;
    u32 select;
    s8 slide;
    u8 is_slide;
    BrListMoveFunc move_func;
    BrListIsMoveFunc ismove_func;
    BrListDecideFunc decide_func;
    TouchRect hittbl[BR_LIST_HITTBL_MAX];
    BOOL is_hold;
    u16 hold_y;
    GFLBitmap *bmp[];
};

#define BR_PROFILE_MSGWIN_MAX 7

struct BrProfile {
    BrMsgWin *msgwin[BR_PROFILE_MSGWIN_MAX];
    BrRes *res;
    PMSDraw *pms;
    ClActor *pokeIcon;
    ClActor *trainer;
    u32 pokeIconPlt;
    u32 pokeIconChr;
    u32 pokeIconCel;
    u32 trainerPlt;
    u32 trainerChr;
    u32 trainerCel;
};

// A profile message window and the message it shows
typedef struct {
    u8 x;
    u8 y;
    u8 w;
    u8 h;
    u32 msgID;
} BrProfileMsgWinData;

struct BrSeq {
    BrSeqFunc seq_function;
    u32 seq;
    void *p_wk_adrs;
    u32 unkC;
};

// The motions of a ball: around a circle as its radius changes, around a circle, along a line, or after another
// point
typedef struct {
    BrPoint center;
    BrPoint now;
    u16 r_start;
    s16 r_end;
    u16 rot_start;
    u16 rot_end;
    s32 sync_max;
} BrBallCircle;

typedef struct {
    BrPoint center;
    BrPoint now;
    u16 r;
    u16 rot;
    s32 sync_max;
} BrBallRot;

typedef struct {
    BrPoint start;
    BrPoint end;
    BrPoint now;
    s32 sync_start;
    s32 sync_max;
} BrBallLine;

typedef struct {
    const BrPoint *target;
    fx32 x;
    fx32 y;
    fx32 z;
    fx32 speed;
} BrBallFollow;

typedef union {
    BrBallCircle circle;
    BrBallRot rot;
    BrBallLine line;
    BrBallFollow follow;
} BrBallMove;

#define BR_BALL_NUM 12

struct BrBallEffect {
    BrPoint pos;
    const BrPoint *p_follow;
    u32 display;
    BrSeq *seq;
    BrRes *res;
    BrBallMove move[BR_BALL_NUM];
    BrPoint ball[BR_BALL_NUM];
    ClActor *clwk[BR_BALL_NUM];
    s32 cnt;
    BOOL is_end;
    u32 unk230;
    BOOL is_move_end[BR_BALL_NUM];
};

static void func_ov186_021fc870(u32 type, const BrPoint *cp_pos, GFLBitmap *bmp, const StrBuf *str, Font *font,
                                 BrPoint *p_pos);
static void func_ov186_021fce60(BrList *p_wk, u16 list);
static s32 func_ov186_021fcedc(BrList *p_wk);
static void func_ov186_021fcf54(GFLBitmap *src, GFLBitmap *dst, u16 src_x, u16 src_y, u16 dst_x, u16 dst_y, u16 w, u16 h);
BOOL func_ov186_021fcf90(BrList *p_wk, s32 dir);
BOOL func_ov186_021fcfe0(BrList *p_wk, s32 dir);
BOOL func_ov186_021fd094(BrList *p_wk);
BOOL func_ov186_021fd0a8(BrList *p_wk);
void func_ov186_021fd0b8(BrList *p_wk);
void func_ov186_021fd110(BrList *p_wk);
static void func_ov186_021fd964(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
static void func_ov186_021fd9a0(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
static void func_ov186_021fda7c(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
static void func_ov186_021fdbf4(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
static void func_ov186_021fde88(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
static void func_ov186_021fdf40(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
static void func_ov186_021fdff0(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
static void func_ov186_021fdff4(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
static void func_ov186_021fe1c4(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs);
static void func_ov186_021fe2bc(BrBallCircle *p_wk, const BrPoint *cp_center, u16 r_start, u16 r_end, u16 rot_start,
                              u16 rot_end, s32 sync_max);
static BOOL func_ov186_021fe2f8(BrBallCircle *p_wk, BrPoint *p_now, s32 sync);
static void func_ov186_021fe3b4(BrBallRot *p_wk, const BrPoint *cp_center, u16 r, u16 rot, s32 sync_max);
static BOOL func_ov186_021fe3e4(BrBallRot *p_wk, BrPoint *p_now, s32 sync);
static void func_ov186_021fe444(BrBallLine *p_wk, const BrPoint *cp_start, const BrPoint *cp_end, s32 sync_start,
                            s32 sync_max);
static BOOL func_ov186_021fe478(BrBallLine *p_wk, BrPoint *p_now, s32 sync);
static void func_ov186_021fe4d8(BrBallFollow *p_wk, const BrPoint *cp_target, const BrPoint *cp_start, fx32 speed);
static BOOL func_ov186_021fe54c(BrBallFollow *p_wk, BrPoint *p_now);

// Where the balls of the effects start and end
// pokebw2's sc_ball_pos
static const BrPoint data_ov186_02200ad8 = { 128, 96 };
static const BrPoint data_ov186_02200ae0 = { 128, -16 };
static const BrPoint data_ov186_02200ad0 = { 128, 16 };
static const BrPoint data_ov186_02200ae8 = { 128, 216 };

// pokebw2's sc_slide_rect
static const TouchRect data_ov186_02200af0[] = {
    { 8, 128, 8, 32 },
    { 8, 128, 224, 248 },
    { TOUCH_RECT_END, TOUCH_RECT_END, TOUCH_RECT_END, TOUCH_RECT_END },
};

// The palette of each trainer appearance of the Union Room
// pokebw2's sc_union_plt
static const u8 data_ov186_02200afc[16] = { 2, 3, 6, 5, 4, 5, 2, 0, 1, 3, 6, 5, 4, 7, 7, 0 };

// Where the cursor's balls circle, for each row of a cursor list
// pokebw2's sc_cursor_pos
static const BrPoint data_ov186_02200b0c[] = {
    { 24, 76 }, { 24, 100 }, { 24, 124 }, { 24, 148 }, { 24, 172 },
};

// pokebw2's sc_profile_msgwin
static const BrProfileMsgWinData data_ov186_02200b34[BR_PROFILE_MSGWIN_MAX] = {
    { 7, 4, 18, 2, 14 },  { 17, 8, 11, 4, 16 }, { 4, 11, 12, 2, 17 }, { 3, 13, 25, 2, 23 },
    { 3, 15, 25, 2, 24 }, { 4, 17, 11, 2, 18 }, { 3, 19, 26, 4, 0 },
};


// Shows the scroll arrows of the directions the list can still scroll in
static inline void BrList_UpdateArrow(BrList *p_wk) {
    if (p_wk->is_slide) {
        if (p_wk->list == 0) {
            func_0204b3dc(p_wk->clwk[0], FALSE);
        } else {
            func_0204b3dc(p_wk->clwk[0], TRUE);
        }
        if (p_wk->list + p_wk->line_max == p_wk->param.list_max) {
            func_0204b3dc(p_wk->clwk[1], FALSE);
        } else {
            func_0204b3dc(p_wk->clwk[1], TRUE);
        }
    }
}

// pokebw2's BrMsgWin_Init
BrMsgWin *func_ov186_021fc6a4(u16 frame, u8 x, u8 y, u8 w, u8 h, u8 plt, PrintQueue *que, HeapID heapId) {
    BrMsgWin *p_wk = GFL_HeapAllocate(heapId, sizeof(BrMsgWin), FALSE, "br_util.c", 95);

    MI_CpuFill8(p_wk, 0, sizeof(BrMsgWin));
    p_wk->clear_color = 0;
    p_wk->que = que;
    p_wk->frame = frame;
    p_wk->str = GFL_StrBufCreate(255, heapId);
    p_wk->bmpwin = BmpWin_CreateDynamic(frame, x, y, w, h, plt, TRUE);
    PrintWindow_Init(&p_wk->print, p_wk->bmpwin);
    GFL_BitmapFill(BmpWin_GetBitmap(p_wk->bmpwin), p_wk->clear_color);
    BmpWin_TransferNow(p_wk->bmpwin);
    return p_wk;
}

// pokebw2's BrMsgWin_Exit
void func_ov186_021fc734(BrMsgWin *p_wk) {
    func_020456f8(p_wk->bmpwin);
    BmpWin_Free(p_wk->bmpwin);
    GFL_StrBufFree(p_wk->str);
    GFL_HeapFree(p_wk);
}

// pokebw2's BrMsgWin_Print
void func_ov186_021fc754(BrMsgWin *p_wk, MsgData *msg, u32 strID, Font *font, u16 color) {
    BrPoint pos;

    GFL_BitmapFill(BmpWin_GetBitmap(p_wk->bmpwin), p_wk->clear_color);
    GFL_MsgDataLoadStrbuf(msg, strID, p_wk->str);
    func_ov186_021fc870(p_wk->pos_type, &p_wk->pos, BmpWin_GetBitmap(p_wk->bmpwin), p_wk->str, font, &pos);
    PrintWindow_Print(&p_wk->print, p_wk->que, pos.x, pos.y, p_wk->str, font, color);
}

// pokebw2's BrMsgWin_PrintBuf
void func_ov186_021fc7c4(BrMsgWin *p_wk, const StrBuf *str, Font *font, u16 color) {
    BrPoint pos;

    GFL_BitmapFill(BmpWin_GetBitmap(p_wk->bmpwin), p_wk->clear_color);
    GFL_StrBufCopy(p_wk->str, str);
    func_ov186_021fc870(p_wk->pos_type, &p_wk->pos, BmpWin_GetBitmap(p_wk->bmpwin), p_wk->str, font, &pos);
    PrintWindow_Print(&p_wk->print, p_wk->que, pos.x, pos.y, p_wk->str, font, color);
}

// pokebw2's BrMsgWin_Main
BOOL func_ov186_021fc830(BrMsgWin *p_wk) {
    PrintWindow_Flush(&p_wk->print, p_wk->que);
    return p_wk->print.flushPending == FALSE;
}

// pokebw2's BrMsgWin_SetPos
void func_ov186_021fc868(BrMsgWin *p_wk, s32 x, s32 y, u32 type) {
    p_wk->pos_type = type;
    p_wk->pos.x = x;
    p_wk->pos.y = y;
}

static void func_ov186_021fc870(u32 type, const BrPoint *cp_pos, GFLBitmap *bmp, const StrBuf *str, Font *font,
                                 BrPoint *p_pos) {
    switch (type) {
    case BR_MSGWIN_POS_ABSOLUTE:
        *p_pos = *cp_pos;
        break;
    case BR_MSGWIN_POS_WH_CENTER: {
        u32 x = GFL_BitmapGetWidth(bmp) / 2;
        u32 y = GFL_BitmapGetHeight(bmp) / 2;

        x -= GFL_FontGetBlockWidth(str, font, 0) / 2;
        y -= GFL_FontGetBlockHeight(str, font) / 2;
        p_pos->x = x + cp_pos->x;
        p_pos->y = y + cp_pos->y;
        break;
    }
    }
}

BrList *func_ov186_021fc8c8(const BrListParam *cp_param, HeapID heapId) {
    BrList *p_wk;
    u32 size = sizeof(BrList) + sizeof(GFLBitmap *) * cp_param->list_max;
    int i;

    p_wk = GFL_HeapAllocate(heapId, size, FALSE, "br_util.c", 417);
    MI_CpuFill8(p_wk, 0, size);
    p_wk->param = *cp_param;
    p_wk->unk3C = 0;
    p_wk->list_max = cp_param->list_max;
    p_wk->line_max = cp_param->h / cp_param->str_line;
    p_wk->line_max = MATH_IMin(p_wk->line_max, cp_param->list_max);
    if (cp_param->pos != NULL) {
        p_wk->list = cp_param->pos->list;
        p_wk->cursor = cp_param->pos->cursor;
    }

    if (p_wk->param.type == BR_LIST_TYPE_TOUCH) {
        for (i = 0; i < p_wk->line_max; i++) {
            GFL_ASSERT(i < BR_LIST_HITTBL_MAX);
            p_wk->hittbl[i].left = p_wk->param.x * 8;
            p_wk->hittbl[i].top = (p_wk->param.y + i * p_wk->param.str_line) * 8;
            p_wk->hittbl[i].right = (p_wk->param.x + p_wk->param.w) * 8;
            p_wk->hittbl[i].bottom = (p_wk->param.y + (i + 1) * p_wk->param.str_line) * 8;
        }
        GFL_ASSERT(i < BR_LIST_HITTBL_MAX);
        p_wk->hittbl[i].left = TOUCH_RECT_END;
        p_wk->hittbl[i].top = TOUCH_RECT_END;
        p_wk->hittbl[i].right = TOUCH_RECT_END;
        p_wk->hittbl[i].bottom = TOUCH_RECT_END;
        p_wk->move_func = func_ov186_021fcf90;
        p_wk->ismove_func = func_ov186_021fd094;
        p_wk->decide_func = func_ov186_021fd0b8;
    } else if (p_wk->param.type == BR_LIST_TYPE_CURSOR) {
        p_wk->move_func = func_ov186_021fcfe0;
        p_wk->ismove_func = func_ov186_021fd0a8;
        p_wk->decide_func = func_ov186_021fd110;
        if (p_wk->param.ballCursor != NULL) {
            func_ov186_021fd860(p_wk->param.ballCursor, BR_BALL_EFFECT_ROT_SMALL, &data_ov186_02200b0c[p_wk->cursor]);
        }
    }

    p_wk->bmpwin = BmpWin_CreateDynamic(cp_param->frame, cp_param->x, cp_param->y, cp_param->w, cp_param->h, cp_param->plt, TRUE);
    BmpWin_FlushMap(p_wk->bmpwin);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(p_wk->bmpwin));

    {
        ClActorSetup setup;
        BrResObjData obj;
        u32 display;
        BOOL ret;

        MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
        if (cp_param->frame < BGSYS_BG_SUB) {
            func_ov186_021fbe54(p_wk->param.res, 7, heapId);
            ret = func_ov186_021fc124(p_wk->param.res, 7, &obj);
            GFL_ASSERT(ret);
            display = CLACT_SURFACE_MAIN;
        } else {
            func_ov186_021fbe54(p_wk->param.res, 8, heapId);
            ret = func_ov186_021fc124(p_wk->param.res, 8, &obj);
            GFL_ASSERT(ret);
            display = CLACT_SURFACE_SUB;
        }
        setup.x = 128;
        setup.priority = 0;
        for (i = 0; i < 2; i++) {
            if (cp_param->type == BR_LIST_TYPE_TOUCH) {
                setup.y = i == 0 ? 16 : 128;
            } else {
                setup.y = i == 0 ? 64 : 184;
            }
            setup.sequence = i;
            p_wk->clwk[i] = func_0204b294(cp_param->unit, obj.chr, obj.plt, obj.cell, &setup, display, heapId);
            func_0204b92c(p_wk->clwk[i], TRUE);
            func_0204b688(p_wk->clwk[i], TRUE);
        }
        func_0204b3dc(p_wk->clwk[0], FALSE);
    }
    {
        ClActorSetup setup;
        BrResObjData obj;
        BOOL ret;

        MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
        setup.y = 72;
        setup.sequence = 5;
        setup.priority = 0;
        ret = func_ov186_021fc124(p_wk->param.res, 6, &obj);
        GFL_ASSERT(ret);
        for (i = 2; i < 4; i++) {
            setup.x = i == 2 ? 24 : 232;
            p_wk->clwk[i] = func_0204b294(p_wk->param.unit, obj.chr, obj.plt, obj.cell, &setup, CLACT_SURFACE_SUB, heapId);
            func_0204b688(p_wk->clwk[i], TRUE);
        }
    }
    {
        Font *font = func_ov186_021fc20c(p_wk->param.res);

        for (i = 0; i < p_wk->param.list_max; i++) {
            const StrBuf *str = p_wk->param.cp_list[i].str;

            if (str != NULL) {
                p_wk->bmp[i] = GFL_BitmapCreate(p_wk->param.w, p_wk->param.str_line, 0x20, heapId);
                GFL_BitmapFill(p_wk->bmp[i], 0);
                GFL_TextRendererDrawToBitmapEx(p_wk->bmp[i], 0, 0, str, font, 0x3da0);
            }
        }
    }
    func_ov186_021fce60(p_wk, p_wk->list);

    if (cp_param->list_max < p_wk->line_max) {
        p_wk->is_slide = FALSE;
        func_0204b3dc(p_wk->clwk[0], FALSE);
        func_0204b3dc(p_wk->clwk[1], FALSE);
    } else {
        p_wk->is_slide = TRUE;
        BrList_UpdateArrow(p_wk);
    }

    if (func_ov186_021fcda0(p_wk)) {
        for (i = 2; i < 4; i++) {
            func_0204b92c(p_wk->clwk[i], TRUE);
        }
    }
    return p_wk;
}


// pokebw2's BrList_Exit
void func_ov186_021fcc80(BrList *p_wk) {
    int i;

    if (p_wk->param.ballCursor != NULL) {
        func_ov186_021fd860(p_wk->param.ballCursor, BR_BALL_EFFECT_NONE, NULL);
    }
    for (i = 0; i < 4; i++) {
        func_0204b3b4(p_wk->clwk[i]);
    }
    for (i = 0; i < p_wk->param.list_max; i++) {
        if (p_wk->bmp[i] != NULL && p_wk->param.cp_list[i].str != NULL) {
            GFL_BitmapFree(p_wk->bmp[i]);
        }
    }
    func_020456f8(p_wk->bmpwin);
    BmpWin_Free(p_wk->bmpwin);
    if (p_wk->param.frame < BGSYS_BG_SUB) {
        func_ov186_021fc0c8(p_wk->param.res, 7);
    } else {
        func_ov186_021fc0c8(p_wk->param.res, 8);
    }
    if (p_wk->param.pos != NULL) {
        p_wk->param.pos->list = p_wk->list;
        p_wk->param.pos->cursor = p_wk->cursor;
    }
    GFL_HeapFree(p_wk);
}

// pokebw2's BrList_Main
void func_ov186_021fcd0c(BrList *p_wk) {
    if (func_ov186_021fcda0(p_wk)) {
        p_wk->slide = func_ov186_021fcedc(p_wk);
        if (p_wk->slide != 0) {
            if (p_wk->move_func(p_wk, p_wk->slide)) {
                if (!GFL_SndIsPlaying(0x6fe)) {
                    GFL_SndSEPlay(0x6fe);
                }
                func_ov186_021fce60(p_wk, p_wk->list);
            }
            BrList_UpdateArrow(p_wk);
        }
    }
    p_wk->decide_func(p_wk);
}

// pokebw2's BrList_IsMove
BOOL func_ov186_021fcda0(BrList *p_wk) {
    return p_wk->ismove_func(p_wk);
}

// pokebw2's BrList_GetSelect
u32 func_ov186_021fcda8(const BrList *cp_wk) {
    return cp_wk->select;
}

// pokebw2's BrList_SetBmp
void func_ov186_021fcdac(BrList *p_wk, u32 idx, GFLBitmap *bmp) {
    GFL_ASSERT(idx < p_wk->param.list_max);
    GFL_ASSERT(p_wk->param.cp_list[idx].str == NULL);
    p_wk->bmp[idx] = bmp;
}

// pokebw2's BrList_GetBmp
GFLBitmap *func_ov186_021fcde8(const BrList *cp_wk, u32 idx) {
    GFL_ASSERT(idx < cp_wk->param.list_max);
    return cp_wk->bmp[idx];
}

// pokebw2's BrList_GetParam
u32 func_ov186_021fce0c(const BrList *cp_wk, u32 paramID) {
    switch (paramID) {
    case BR_LIST_PARAM_CURSOR:
        return cp_wk->cursor;
    case BR_LIST_PARAM_LIST:
        return cp_wk->list;
    case BR_LIST_PARAM_SLIDE:
        return cp_wk->slide;
    case BR_LIST_PARAM_LISTMAX:
        return cp_wk->param.list_max;
    default:
        GFL_ASSERT(0);
        return 0;
    }
}

// pokebw2's BrList_Write
void func_ov186_021fce50(BrList *p_wk) {
    func_ov186_021fce60(p_wk, p_wk->list);
}

// pokebw2's BrList_WriteCore
static void func_ov186_021fce60(BrList *p_wk, u16 list) {
    int i;

    GFL_BitmapFill(BmpWin_GetBitmap(p_wk->bmpwin), 0);
    for (i = list; i < list + p_wk->line_max; i++) {
        if (i < p_wk->param.list_max && p_wk->bmp[i] != NULL) {
            func_ov186_021fcf54(p_wk->bmp[i], BmpWin_GetBitmap(p_wk->bmpwin), 0, 0, 0, (i - list) * p_wk->param.str_line,
                           p_wk->param.w, p_wk->param.str_line);
        }
    }
    BmpWin_FlushChar(p_wk->bmpwin);
}

// The direction the list is dragged in, a step each time the touch moves 8 pixels
// pokebw2's BrList_GetSlide
static s32 func_ov186_021fcedc(BrList *p_wk) {
    s32 ret = 0;

    if (func_02035ba4(data_ov186_02200af0) != TOUCH_RECT_NONE) {
        u32 x, y;

        func_02035cb8(&x, &y);
        if (!p_wk->is_hold) {
            p_wk->is_hold = TRUE;
            p_wk->hold_y = y;
        } else {
            s32 dist = y - p_wk->hold_y;

            if (MATH_ABS(dist) > 8) {
                if (dist < 0) {
                    ret = -1;
                } else {
                    ret = 1;
                }
                p_wk->hold_y = y;
            }
        }
    } else if (p_wk->is_hold) {
        p_wk->is_hold = FALSE;
    }
    return ret;
}

// Copies whole rows of characters from one bitmap to another of the same width
// pokebw2's BrList_BlitBmp
static void func_ov186_021fcf54(GFLBitmap *src, GFLBitmap *dst, u16 src_x, u16 src_y, u16 dst_x, u16 dst_y, u16 w, u16 h) {
    u8 *p_src = GFL_BitmapGetPixelData(src);
    u8 *p_dst = GFL_BitmapGetPixelData(dst);

    MIi_CpuCopy32(p_src + (w * src_y + src_x) * 0x20, p_dst + (w * dst_y + dst_x) * 0x20, w * h * 0x20);
}

// pokebw2's BrList_Move_Touch
BOOL func_ov186_021fcf90(BrList *p_wk, s32 dir) {
    if (p_wk->list != 0 && dir == -1) {
        p_wk->list += dir;
        return TRUE;
    }
    if (p_wk->list + p_wk->line_max < p_wk->param.list_max && dir == 1) {
        p_wk->list += dir;
        return TRUE;
    }
    return FALSE;
}

// The cursor moves to the middle row, then the list scrolls under it
// pokebw2's BrList_Move_Cursor
BOOL func_ov186_021fcfe0(BrList *p_wk, s32 dir) {
    BOOL ret = FALSE;
    int line = p_wk->line_max;
    int center = (line - 1) / 2 + (line - 1) % 2;

    if (dir > 0) {
        if (center == p_wk->cursor && p_wk->list + line < p_wk->param.list_max) {
            p_wk->list += dir;
            ret = TRUE;
        } else if (p_wk->list + p_wk->cursor < p_wk->param.list_max - 1) {
            p_wk->cursor += dir;
            ret = TRUE;
        }
    } else if (dir < 0) {
        if (center == p_wk->cursor && p_wk->list != 0) {
            p_wk->list += dir;
            ret = TRUE;
        } else if (p_wk->cursor != 0) {
            p_wk->cursor += dir;
            ret = TRUE;
        }
    }

    if (ret && p_wk->param.ballCursor != NULL) {
        func_ov186_021fd860(p_wk->param.ballCursor, BR_BALL_EFFECT_ROT_SMALL, &data_ov186_02200b0c[p_wk->cursor]);
    }
    return ret;
}

// pokebw2's BrList_IsMove_Touch
BOOL func_ov186_021fd094(BrList *p_wk) {
    return p_wk->param.list_max >= p_wk->line_max;
}

// pokebw2's BrList_IsMove_Cursor
BOOL func_ov186_021fd0a8(BrList *p_wk) {
    return p_wk->param.list_max > 1;
}

// pokebw2's BrList_Decide_Touch
void func_ov186_021fd0b8(BrList *p_wk) {
    s32 trg;

    p_wk->select = BR_LIST_SELECT_NONE;
    trg = func_02035c08(p_wk->hittbl);
    if (trg != TOUCH_RECT_NONE) {
        if (p_wk->param.ballDecide != NULL) {
            u32 x, y;
            BrPoint pos;

            func_02035d20(&x, &y);
            pos.x = x;
            pos.y = y;
            func_ov186_021fd860(p_wk->param.ballDecide, BR_BALL_EFFECT_SPREAD, &pos);
        }
        GFL_SndSEPlay(0x703);
        p_wk->select = p_wk->param.cp_list[trg + p_wk->list].param;
    }
}

// pokebw2's BrList_Decide_Cursor
void func_ov186_021fd110(BrList *p_wk) {
    p_wk->select = p_wk->param.cp_list[p_wk->cursor + p_wk->list].param;
}

// pokebw2's BrText_Init
BrMsgWin *func_ov186_021fd12c(BrRes *res, PrintQueue *que, HeapID heapId) {
    BrMsgWin *p_wk;

    func_ov186_021fb964(res, 2, heapId);
    p_wk = func_ov186_021fc6a4(0, 1, 19, 30, 4, 2, que, heapId);
    p_wk->clear_color = 12;
    return p_wk;
}

// pokebw2's BrText_Exit
void func_ov186_021fd15c(BrMsgWin *p_wk, BrRes *res) {
    func_ov186_021fc734(p_wk);
    func_ov186_021fbda4(res, 2);
}

// pokebw2's BrText_Print
void func_ov186_021fd170(BrMsgWin *p_wk, BrRes *res, u32 msgID) {
    Font *font = func_ov186_021fc20c(res);

    func_ov186_021fc754(p_wk, func_ov186_021fc214(res), msgID, font, 0x3dac);
}

// pokebw2's BrText_PrintBuf
void func_ov186_021fd19c(BrMsgWin *p_wk, BrRes *res, const StrBuf *str) {
    func_ov186_021fc7c4(p_wk, str, func_ov186_021fc20c(res), 0x3dac);
}

// pokebw2's BrText_Main
BOOL func_ov186_021fd1bc(BrMsgWin *p_wk) {
    return func_ov186_021fc830(p_wk);
}

BrProfile *func_ov186_021fd1c4(GdsProfile *cp_profile, BrRes *res, ClActUnit *unit, PrintQueue *que, u32 type,
                          HeapID heapId) {
    BrProfile *p_wk;
    Font *font;
    int i;
    MsgData *msg;
    WordSet *wordset;

    p_wk = GFL_HeapAllocate(heapId, sizeof(BrProfile), FALSE, "br_util.c", 1345);
    MI_CpuFill8(p_wk, 0, sizeof(BrProfile));
    p_wk->res = res;
    font = func_ov186_021fc20c(res);
    p_wk->pms = func_0202db0c(unit, CLACT_VRAM_MAIN, que, font, 7, 1, heapId);
    func_0202dcfc(p_wk->pms, 0, TRUE);
    func_ov186_021fb964(p_wk->res, 8, heapId);
    msg = func_ov186_021fc214(res);
    wordset = func_ov186_021fc21c(res);

    for (i = 0; i < BR_PROFILE_MSGWIN_MAX; i++) {
        const BrProfileMsgWinData *cp_data = &data_ov186_02200b34[i];
        BOOL isPrint;
        StrBuf *str;
        StrBuf *src;

        p_wk->msgwin[i] = func_ov186_021fc6a4(1, cp_data->x, cp_data->y, cp_data->w, cp_data->h, 14, que, heapId);
        func_ov186_021fc868(p_wk->msgwin[i], 0, 1, BR_MSGWIN_POS_ABSOLUTE);
        isPrint = TRUE;

        switch (i) {
        case 0: {
            StrBuf *name;

            str = GFL_StrBufCreate(128, heapId);
            name = func_0200d6d0(cp_profile, heapId);
            src = GFL_MsgDataLoadStrbufNew(msg, cp_data->msgID);
            func_0201ee0c(wordset, 0, name, func_0200d6ec(cp_profile), TRUE, 2);
            GFL_WordSetFormatStrbuf(wordset, str, src);
            GFL_StrBufFree(name);
            GFL_StrBufFree(src);
            break;
        }
        case 1: {
            u32 month = func_0200d810(cp_profile);

            str = GFL_StrBufCreate(128, heapId);
            src = GFL_MsgDataLoadStrbufNew(msg, cp_data->msgID);
            loadMonthToStrbuf(wordset, 0, month);
            GFL_WordSetFormatStrbuf(wordset, str, src);
            GFL_StrBufFree(src);
            break;
        }
        case 3: {
            u32 country = func_0200d74c(cp_profile);

            str = GFL_StrBufCreate(128, heapId);
            src = GFL_MsgDataLoadStrbufNew(msg, cp_data->msgID);
            loadCountryToStrbuf(wordset, 0, country);
            GFL_WordSetFormatStrbuf(wordset, str, src);
            GFL_StrBufFree(src);
            func_ov186_021fc868(p_wk->msgwin[i], 8, 1, BR_MSGWIN_POS_ABSOLUTE);
            break;
        }
        case 4: {
            u32 country = func_0200d74c(cp_profile);
            u32 area = func_0200d760(cp_profile);

            str = GFL_StrBufCreate(128, heapId);
            src = GFL_MsgDataLoadStrbufNew(msg, cp_data->msgID);
            loadCountryAreaToStrbuf(wordset, 0, country, area);
            GFL_WordSetFormatStrbuf(wordset, str, src);
            GFL_StrBufFree(src);
            func_ov186_021fc868(p_wk->msgwin[i], 8, 1, BR_MSGWIN_POS_ABSOLUTE);
            break;
        }
        case 6: {
            PMSData pms;

            src = func_0200d774(cp_profile, &pms, heapId);
            if (src != NULL) {
                str = GFL_StrBufClone(src, heapId);
                GFL_StrBufFree(src);
                func_ov186_021fc868(p_wk->msgwin[i], 8, 1, BR_MSGWIN_POS_ABSOLUTE);
            } else {
                PMSDrawPos pos = { 8, 0 };

                func_0202de5c(p_wk->pms, 0);
                func_0202de64(p_wk->pms, 0x3da0);
                func_0202dc60(p_wk->pms, p_wk->msgwin[i]->bmpwin, &pms, 0, &pos);
                isPrint = FALSE;
            }
            break;
        }
        default:
            str = GFL_MsgDataLoadStrbufNew(msg, cp_data->msgID);
            break;
        }

        if (isPrint) {
            func_ov186_021fc7c4(p_wk->msgwin[i], str, font, 0x3da0);
            GFL_StrBufFree(str);
        }
    }

    {
        BOOL egg = func_0200d73c(cp_profile);
        int species = func_0200d6fc(cp_profile);
        int form = func_0200d70c(cp_profile);
        int gender = func_0200d72c(cp_profile);

        if (species != 0) {
            ArcTool *handle = GFL_ArcSysCreateFileHandle(7, HEAPID_TAIL(heapId));
            ClActorSetup setup;

            p_wk->pokeIconPlt = func_0204acec(handle, func_0201bc3c(), CLACT_VRAM_MAIN, 0x80, HEAPID_TAIL(heapId));
            p_wk->pokeIconCel = func_0204af28(handle, func_0201bc40(), getOBJTileMapping_MainEng(), HEAPID_TAIL(heapId));
            p_wk->pokeIconChr = func_0204a6c8(handle, PokeParty_GetIconIndex(species, form, gender, egg), FALSE,
                                              CLACT_VRAM_MAIN, HEAPID_TAIL(heapId));
            GFL_ArcToolFree(handle);

            MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
            setup.x = 75;
            setup.y = 64;
            setup.sequence = 1;
            p_wk->pokeIcon = func_0204b294(unit, p_wk->pokeIconChr, p_wk->pokeIconPlt, p_wk->pokeIconCel, &setup,
                                           CLACT_SURFACE_MAIN, heapId);
            func_0204b6f4(p_wk->pokeIcon, func_0201bb5c(species, form, gender, egg), 0);
            func_0204b688(p_wk->pokeIcon, TRUE);
        }
    }

    switch (type) {
    case BR_PROFILE_TYPE_TRAINER: {
        u32 cel, chr, plt, anm;
        ArcTool *handle;

        if (func_0200d6ec(cp_profile) == 0) {
            chr = 20;
            plt = 4;
            cel = 18;
            anm = 19;
        } else {
            chr = 23;
            plt = 6;
            cel = 21;
            anm = 22;
        }
        handle = GFL_ArcSysCreateFileHandle(31, HEAPID_TAIL(heapId));
        p_wk->trainerPlt = func_0204ac18(handle, plt, CLACT_VRAM_MAIN, 0x1a0, 0, 1, heapId);
        p_wk->trainerCel = func_0204af28(handle, cel, anm, heapId);
        p_wk->trainerChr = func_0204a6c8(handle, chr, FALSE, CLACT_VRAM_MAIN, heapId);
        GFL_ArcToolFree(handle);
        break;
    }
    case BR_PROFILE_TYPE_UNION: {
        u32 view = func_0200d820(cp_profile);
        ArcTool *handle = GFL_ArcSysCreateFileHandle(32, HEAPID_TAIL(heapId));

        p_wk->trainerPlt = func_0204ac18(handle, 0, CLACT_VRAM_MAIN, 0x1a0, data_ov186_02200afc[view], 1, heapId);
        p_wk->trainerCel = func_0204af28(handle, 65, 66, heapId);
        p_wk->trainerChr = func_0204a6c8(handle, view + 49, FALSE, CLACT_VRAM_MAIN, heapId);
        GFL_ArcToolFree(handle);
        break;
    }
    }

    {
        ClActorSetup setup;

        MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
        setup.x = 48;
        setup.y = 64;
        p_wk->trainer = func_0204b294(unit, p_wk->trainerChr, p_wk->trainerPlt, p_wk->trainerCel, &setup,
                                      CLACT_SURFACE_MAIN, heapId);
        func_0204b6f4(p_wk->trainer, 0, 1);
        func_0204b688(p_wk->trainer, TRUE);
    }
    return p_wk;
}

// pokebw2's BrProfile_Exit
void func_ov186_021fd640(BrProfile *p_wk) {
    int i;

    if (p_wk->trainer != NULL) {
        func_0204b3b4(p_wk->trainer);
        func_0204ada4(p_wk->trainerPlt);
        func_0204a8d4(p_wk->trainerChr);
        func_0204afd8(p_wk->trainerCel);
    }
    if (p_wk->pokeIcon != NULL) {
        func_0204b3b4(p_wk->pokeIcon);
        func_0204ada4(p_wk->pokeIconPlt);
        func_0204a8d4(p_wk->pokeIconChr);
        func_0204afd8(p_wk->pokeIconCel);
    }
    for (i = 0; i < BR_PROFILE_MSGWIN_MAX; i++) {
        if (p_wk->msgwin[i] != NULL) {
            func_ov186_021fc734(p_wk->msgwin[i]);
            p_wk->msgwin[i] = NULL;
        }
    }
    func_0202dc08(p_wk->pms);
    func_ov186_021fbda4(p_wk->res, 8);
    GFL_HeapFree(p_wk);
    GFL_BGSysLoadScr(1);
}

// pokebw2's BrProfile_Main
BOOL func_ov186_021fd6b0(BrProfile *p_wk) {
    BOOL isEnd = TRUE;
    int i;

    for (i = 0; i < BR_PROFILE_MSGWIN_MAX; i++) {
        if (p_wk->msgwin[i] != NULL) {
            isEnd &= func_ov186_021fc830(p_wk->msgwin[i]);
        }
    }
    func_0202dbc4(p_wk->pms);
    return isEnd;
}

// pokebw2's BrSeq_Init
BrSeq *func_ov186_021fd6d8(void *p_wk_adrs, BrSeqFunc seq_function, HeapID heapId) {
    BrSeq *p_wk = GFL_HeapAllocate(heapId, sizeof(BrSeq), FALSE, "br_util.c", 1698);

    MI_CpuFill8(p_wk, 0, sizeof(BrSeq));
    p_wk->p_wk_adrs = p_wk_adrs;
    func_ov186_021fd73c(p_wk, seq_function);
    return p_wk;
}

// pokebw2's BrSeq_Exit
void func_ov186_021fd714(BrSeq *p_wk) {
    GFL_HeapFree(p_wk);
}

// pokebw2's BrSeq_Main
void func_ov186_021fd71c(BrSeq *p_wk) {
    if (p_wk->seq_function != NULL) {
        p_wk->seq_function(p_wk, &p_wk->seq, p_wk->p_wk_adrs);
    }
}

// pokebw2's BrSeq_IsEnd
BOOL func_ov186_021fd72c(const BrSeq *cp_wk) {
    return cp_wk->seq_function == NULL;
}

// pokebw2's BrSeq_SetNext
void func_ov186_021fd73c(BrSeq *p_wk, BrSeqFunc seq_function) {
    p_wk->seq_function = seq_function;
    p_wk->seq = 0;
}

// pokebw2's BrSeq_End
void func_ov186_021fd744(BrSeq *p_wk) {
    func_ov186_021fd73c(p_wk, NULL);
}

// pokebw2's BrSeq_IsComp
BOOL func_ov186_021fd750(const BrSeq *cp_wk, BrSeqFunc seq_function) {
    return cp_wk->seq_function == seq_function;
}

BrBallEffect *func_ov186_021fd760(ClActUnit *unit, BrRes *res, u32 display, HeapID heapId) {
    BrBallEffect *p_wk = GFL_HeapAllocate(heapId, sizeof(BrBallEffect), FALSE, "br_util.c", 1994);
    int i;

    MI_CpuFill8(p_wk, 0, sizeof(BrBallEffect));
    p_wk->res = res;
    p_wk->display = display;
    p_wk->seq = func_ov186_021fd6d8(p_wk, func_ov186_021fd964, heapId);
    func_ov186_021fbe54(p_wk->res, display + 9, heapId);
    {
        ClActorSetup setup;
        BrResObjData obj;
        BOOL ret;

        MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
        ret = func_ov186_021fc124(p_wk->res, display + 9, &obj);
        GFL_ASSERT(ret);
        for (i = 0; i < BR_BALL_NUM; i++) {
            p_wk->clwk[i] = func_0204b294(unit, obj.chr, obj.plt, obj.cell, &setup, display, heapId);
            func_0204b3dc(p_wk->clwk[i], FALSE);
            func_0204b688(p_wk->clwk[i], TRUE);
        }
    }
    return p_wk;
}


// pokebw2's BrBallEff_Exit
void func_ov186_021fd820(BrBallEffect *p_wk) {
    int i;

    for (i = 0; i < BR_BALL_NUM; i++) {
        func_0204b3b4(p_wk->clwk[i]);
    }
    func_ov186_021fc0c8(p_wk->res, p_wk->display + 9);
    func_ov186_021fd714(p_wk->seq);
    GFL_HeapFree(p_wk);
}

// pokebw2's BrBallEff_Main
void func_ov186_021fd854(BrBallEffect *p_wk) {
    func_ov186_021fd71c(p_wk->seq);
}

// pokebw2's BrBallEff_Start
void func_ov186_021fd860(BrBallEffect *p_wk, u32 type, const BrPoint *pos) {
    if (pos != NULL) {
        p_wk->pos = *pos;
        p_wk->p_follow = pos;
    }
    p_wk->is_end = FALSE;
    if (type == BR_BALL_EFFECT_NONE && func_ov186_021fd750(p_wk->seq, func_ov186_021fde88)) {
        GFL_SndSEPlay(0x701);
    }
    switch (type) {
    case BR_BALL_EFFECT_NONE:
        func_ov186_021fd73c(p_wk->seq, func_ov186_021fd964);
        break;
    case BR_BALL_EFFECT_SPREAD:
        func_ov186_021fd73c(p_wk->seq, func_ov186_021fd9a0);
        break;
    case BR_BALL_EFFECT_LINE:
        func_ov186_021fd73c(p_wk->seq, func_ov186_021fda7c);
        break;
    case BR_BALL_EFFECT_LINE_ROT:
        func_ov186_021fd73c(p_wk->seq, func_ov186_021fdbf4);
        break;
    case BR_BALL_EFFECT_ROT:
        func_ov186_021fd73c(p_wk->seq, func_ov186_021fde88);
        break;
    case BR_BALL_EFFECT_ROT_SMALL:
        func_ov186_021fd73c(p_wk->seq, func_ov186_021fdf40);
        break;
    case BR_BALL_EFFECT_STOP:
        func_ov186_021fd73c(p_wk->seq, func_ov186_021fdff0);
        break;
    case BR_BALL_EFFECT_TOUCH:
        func_ov186_021fd73c(p_wk->seq, func_ov186_021fdff4);
        break;
    case BR_BALL_EFFECT_RING:
        func_ov186_021fd73c(p_wk->seq, func_ov186_021fe1c4);
        break;
    }
}

// pokebw2's BrBallEff_IsEnd
BOOL func_ov186_021fd938(const BrBallEffect *cp_wk) {
    return cp_wk->is_end;
}

// pokebw2's BrBallEff_SetAnmSeq
void func_ov186_021fd940(BrBallEffect *p_wk, u32 seq) {
    int i;

    for (i = 0; i < BR_BALL_NUM; i++) {
        func_0204b878(p_wk->clwk[i], seq);
    }
}

// pokebw2's BrBallEff_Seq_Wait
static void func_ov186_021fd964(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrBallEffect *p_wk = p_wk_adrs;
    int i;

    switch (*p_seq) {
    case 0:
        for (i = 0; i < BR_BALL_NUM; i++) {
            func_0204b3dc(p_wk->clwk[i], FALSE);
        }
        p_wk->is_end = TRUE;
        *p_seq = 1;
        break;
    case 1:
        break;
    }
}

// pokebw2's BrBallEff_Seq_Spread
static void func_ov186_021fd9a0(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrBallEffect *p_wk = p_wk_adrs;
    int i;

    switch (*p_seq) {
    case 0:
        p_wk->cnt = 0;
        for (i = 0; i < 6; i++) {
            func_ov186_021fe2bc(&p_wk->move[i].circle, &p_wk->pos, 8, 20, 0xffff * i / 6, 0xffff * i / 6 + 0x3333, 15);
            func_0204b3dc(p_wk->clwk[i], TRUE);
        }
        *p_seq = 1;
        // fallthrough
    case 1:
        for (i = 0; i < 6; i++) {
            BrPoint now;
            ClActorPos pos;

            func_ov186_021fe2f8(&p_wk->move[i].circle, &now, p_wk->cnt);
            pos.x = now.x;
            pos.y = now.y;
            func_0204b404(p_wk->clwk[i], &pos, p_wk->display);
        }
        if (p_wk->cnt++ > 15) {
            *p_seq = 2;
        }
        break;
    case 2:
        func_ov186_021fd73c(p_wk->seq, func_ov186_021fd964);
        break;
    }
}

// pokebw2's BrBallEff_Seq_Line
static void func_ov186_021fda7c(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrBallEffect *p_wk = p_wk_adrs;
    int i;
    ClActorPos pos;
    BrPoint start;
    BOOL isEnd;

    switch (*p_seq) {
    case 0:
        p_wk->cnt = 0;
        start = data_ov186_02200ad0;
        for (i = 0; i < BR_BALL_NUM; i++) {
            p_wk->ball[i] = start;
            func_0204b3dc(p_wk->clwk[i], TRUE);
        }
        func_ov186_021fe444(&p_wk->move[0].line, &p_wk->ball[0], &data_ov186_02200ae8, 0, 50);
        for (i = 1; i < BR_BALL_NUM; i++) {
            func_ov186_021fe4d8(&p_wk->move[i].follow, &p_wk->ball[i - 1], &p_wk->ball[i], FX32_CONST(4) - 0x666 * i / 2);
        }
        *p_seq = 1;
        // fallthrough
    case 1:
        isEnd = TRUE;
        p_wk->is_end = func_ov186_021fe478(&p_wk->move[0].line, &p_wk->ball[0], p_wk->cnt);
        isEnd &= p_wk->is_end;
        for (i = 1; i < BR_BALL_NUM; i++) {
            isEnd &= func_ov186_021fe54c(&p_wk->move[i].follow, &p_wk->ball[i]);
        }
        for (i = 0; i < BR_BALL_NUM; i++) {
            pos.x = p_wk->ball[i].x;
            pos.y = p_wk->ball[i].y;
            func_0204b404(p_wk->clwk[i], &pos, p_wk->display);
        }
        p_wk->cnt++;
        if (isEnd) {
            *p_seq = 2;
        }
        break;
    case 2:
        func_ov186_021fd73c(p_wk->seq, func_ov186_021fd964);
        break;
    }
}

// pokebw2's BrBallEff_Seq_LineRot
static void func_ov186_021fdbf4(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrBallEffect *p_wk = p_wk_adrs;
    int i;

    switch (*p_seq) {
    case 0:
        p_wk->cnt = 0;
        for (i = 0; i < 6; i++) {
            p_wk->ball[i] = data_ov186_02200ae0;
            func_0204b3dc(p_wk->clwk[i], TRUE);
        }
        func_ov186_021fe444(&p_wk->move[0].line, &p_wk->ball[0], &data_ov186_02200ad8, 0, 36);
        for (i = 1; i < 6; i++) {
            func_ov186_021fe4d8(&p_wk->move[i].follow, &p_wk->ball[i - 1], &p_wk->ball[i],
                              FX32_CONST(3.5) - 0xb33 * i / 2);
        }
        *p_seq = 1;
        // fallthrough
    case 1:
        p_wk->is_move_end[0] = func_ov186_021fe478(&p_wk->move[0].line, &p_wk->ball[0], p_wk->cnt);
        for (i = 1; i < 6; i++) {
            p_wk->is_move_end[i] = func_ov186_021fe54c(&p_wk->move[i].follow, &p_wk->ball[i]);
        }
        for (i = 0; i < 6; i++) {
            ClActorPos pos;

            pos.x = p_wk->ball[i].x;
            pos.y = p_wk->ball[i].y;
            func_0204b404(p_wk->clwk[i], &pos, p_wk->display);
        }
        p_wk->cnt++;
        if (p_wk->is_move_end[0]) {
            func_0204b3dc(p_wk->clwk[0], FALSE);
            *p_seq = 2;
        }
        break;
    case 2:
        p_wk->cnt = 0;
        for (i = 6; i < BR_BALL_NUM; i++) {
            func_ov186_021fe3b4(&p_wk->move[i].rot, &data_ov186_02200ad8, 15, 0xffff * (i - 6) / 6, 90);
            func_0204b3dc(p_wk->clwk[i], TRUE);
        }
        *p_seq = 3;
        // fallthrough
    case 3:
        for (i = 6; i < BR_BALL_NUM; i++) {
            func_ov186_021fe3e4(&p_wk->move[i].rot, &p_wk->ball[i], p_wk->cnt);
        }
        for (i = 1; i < 6; i++) {
            p_wk->is_move_end[i] = func_ov186_021fe54c(&p_wk->move[i].follow, &p_wk->ball[i]);
            if (p_wk->is_move_end[i]) {
                func_0204b3dc(p_wk->clwk[i], FALSE);
            }
        }
        for (i = 0; i < BR_BALL_NUM; i++) {
            ClActorPos pos;

            pos.x = p_wk->ball[i].x;
            pos.y = p_wk->ball[i].y;
            func_0204b404(p_wk->clwk[i], &pos, p_wk->display);
        }
        p_wk->is_end = TRUE;
        p_wk->cnt++;
        break;
    }
}

// pokebw2's BrBallEff_Seq_Rot
static void func_ov186_021fde88(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrBallEffect *p_wk = p_wk_adrs;
    int i;

    switch (*p_seq) {
    case 0:
        p_wk->cnt = 0;
        for (i = 0; i < 6; i++) {
            func_ov186_021fe3b4(&p_wk->move[i].rot, &p_wk->pos, 15, 0xffff * i / 6, 90);
            func_0204b3dc(p_wk->clwk[i], TRUE);
        }
        GFL_SndSEPlay(0x705);
        *p_seq = 1;
        // fallthrough
    case 1:
        for (i = 0; i < 6; i++) {
            BrPoint now;
            ClActorPos pos;

            func_ov186_021fe3e4(&p_wk->move[i].rot, &now, p_wk->cnt);
            pos.x = now.x;
            pos.y = now.y;
            func_0204b404(p_wk->clwk[i], &pos, p_wk->display);
        }
        p_wk->cnt++;
        break;
    }
}

// pokebw2's BrBallEff_Seq_RotSmall
static void func_ov186_021fdf40(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrBallEffect *p_wk = p_wk_adrs;
    int i;

    switch (*p_seq) {
    case 0:
        p_wk->cnt = 0;
        for (i = 0; i < 6; i++) {
            func_ov186_021fe3b4(&p_wk->move[i].rot, &p_wk->pos, 10, 0xffff * i / 6, 90);
            func_0204b3dc(p_wk->clwk[i], TRUE);
        }
        *p_seq = 1;
        // fallthrough
    case 1:
        for (i = 0; i < 6; i++) {
            BrPoint now;
            ClActorPos pos;

            func_ov186_021fe3e4(&p_wk->move[i].rot, &now, p_wk->cnt);
            pos.x = now.x;
            pos.y = now.y;
            func_0204b404(p_wk->clwk[i], &pos, p_wk->display);
        }
        p_wk->cnt++;
        break;
    }
}

// pokebw2's BrBallEff_Seq_Stop
static void func_ov186_021fdff0(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
}

// pokebw2's BrBallEff_Seq_Touch
static void func_ov186_021fdff4(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrBallEffect *p_wk = p_wk_adrs;
    int i;

    switch (*p_seq) {
    case 0:
        p_wk->cnt = 0;
        for (i = 0; i < 6; i++) {
            func_ov186_021fe2bc(&p_wk->move[i].circle, &p_wk->pos, 8, 20, 0xffff * i / 6, 0xffff * i / 6 + 0x3333, 16);
            func_0204b3dc(p_wk->clwk[i], TRUE);
        }
        *p_seq = 1;
        // fallthrough
    case 1:
        for (i = 0; i < 6; i++) {
            ClActorPos pos;

            func_ov186_021fe2f8(&p_wk->move[i].circle, &p_wk->ball[i], p_wk->cnt);
            pos.x = p_wk->ball[i].x;
            pos.y = p_wk->ball[i].y;
            func_0204b404(p_wk->clwk[i], &pos, p_wk->display);
        }
        if (p_wk->cnt++ > 16) {
            *p_seq = 2;
        }
        break;
    case 2:
        *p_seq = 3;
        break;
    case 3:
        for (i = 0; i < 6; i++) {
            func_ov186_021fe4d8(&p_wk->move[i].follow, p_wk->p_follow, &p_wk->ball[i], FX32_CONST(4) - 0x99a * i / 2);
        }
        *p_seq = 4;
        // fallthrough
    case 4: {
        BOOL isEnd = TRUE;

        for (i = 0; i < 6; i++) {
            ClActorPos pos;

            isEnd &= func_ov186_021fe54c(&p_wk->move[i].follow, &p_wk->ball[i]);
            pos.x = p_wk->ball[i].x;
            pos.y = p_wk->ball[i].y;
            func_0204b404(p_wk->clwk[i], &pos, p_wk->display);
        }
        if (isEnd) {
            *p_seq = 5;
        }
        break;
    }
    case 5:
        func_ov186_021fd73c(p_seqwk, func_ov186_021fd964);
        break;
    }
}

// pokebw2's BrBallEff_Seq_Ring
static void func_ov186_021fe1c4(BrSeq *p_seqwk, u32 *p_seq, void *p_wk_adrs) {
    BrBallEffect *p_wk = p_wk_adrs;
    int i;

    switch (*p_seq) {
    case 0:
        func_ov186_021fd940(p_wk, 0);
        for (i = 6; i < BR_BALL_NUM; i++) {
            // BUG: func_ov186_021fe3e4 turns by 0xffff * cnt / sync_max, so the ring starts from each ball's first angle
            // instead of where it had turned to, and the balls jump
#ifdef BUGFIX
            u16 rot = p_wk->move[i].rot.rot + 0xffff * p_wk->cnt / p_wk->move[i].rot.sync_max;
#else
            u16 rot = p_wk->move[i].rot.rot + p_wk->cnt / p_wk->move[i].rot.sync_max;
#endif

            func_ov186_021fe2bc(&p_wk->move[i].circle, &data_ov186_02200ad8, 15, 1, rot, rot + 0xf000, 30);
            func_0204b688(p_wk->clwk[i], FALSE);
        }
        p_wk->cnt = 0;
        *p_seq = 1;
        // fallthrough
    case 1:
        for (i = 6; i < BR_BALL_NUM; i++) {
            BrPoint now;
            ClActorPos pos;

            func_ov186_021fe2f8(&p_wk->move[i].circle, &now, p_wk->cnt);
            pos.x = now.x;
            pos.y = now.y;
            func_0204b404(p_wk->clwk[i], &pos, p_wk->display);
        }
        if (p_wk->cnt++ > 60) {
            *p_seq = 2;
        }
        break;
    case 2:
        func_ov186_021fd73c(p_seqwk, func_ov186_021fd964);
        break;
    }
}

static void func_ov186_021fe2bc(BrBallCircle *p_wk, const BrPoint *cp_center, u16 r_start, u16 r_end, u16 rot_start,
                              u16 rot_end, s32 sync_max) {
    MI_CpuFill8(p_wk, 0, sizeof(BrBallCircle));
    p_wk->r_start = r_start;
    p_wk->r_end = r_end;
    p_wk->rot_start = rot_start;
    p_wk->rot_end = rot_end;
    p_wk->sync_max = sync_max;
    p_wk->center = *cp_center;
    func_ov186_021fe2f8(p_wk, &p_wk->now, 0);
}

// pokebw2's BrBallCircle_Main
static BOOL func_ov186_021fe2f8(BrBallCircle *p_wk, BrPoint *p_now, s32 sync) {
    u16 rot_dist;
    s32 r_dist;
    s8 dir;
    u16 rot;
    s32 r;
    s32 x;
    s32 y;

    rot_dist = p_wk->rot_end - p_wk->rot_start;
    r_dist = (s16)(p_wk->r_end - (s16)p_wk->r_start);
    dir = r_dist / MATH_ABS(r_dist);
    rot = p_wk->rot_start + rot_dist * sync / p_wk->sync_max;
    r = p_wk->r_start + dir * (sync * MATH_ABS(r_dist) / p_wk->sync_max);
    if (r < 0) {
        r = 0;
    }
    x = (FX_CosIdx(rot) * r) >> FX32_SHIFT;
    y = (FX_SinIdx(rot) * r) >> FX32_SHIFT;
    x += p_wk->center.x;
    y += p_wk->center.y;
    p_wk->now.x = x;
    p_wk->now.y = y;
    if (p_now != NULL) {
        *p_now = p_wk->now;
    }
    return sync == p_wk->sync_max;
}

// pokebw2's BrBallRot_Init
static void func_ov186_021fe3b4(BrBallRot *p_wk, const BrPoint *cp_center, u16 r, u16 rot, s32 sync_max) {
    MI_CpuFill8(p_wk, 0, sizeof(BrBallRot));
    p_wk->center = *cp_center;
    p_wk->sync_max = sync_max;
    p_wk->r = r;
    p_wk->rot = rot;
    func_ov186_021fe3e4(p_wk, NULL, 0);
}

// pokebw2's BrBallRot_Main
static BOOL func_ov186_021fe3e4(BrBallRot *p_wk, BrPoint *p_now, s32 sync) {
    u16 rot = p_wk->rot + 0xffff * sync / p_wk->sync_max;

    p_wk->now.x = (FX_CosIdx(rot) * p_wk->r) >> FX32_SHIFT;
    p_wk->now.y = (FX_SinIdx(rot) * p_wk->r) >> FX32_SHIFT;
    p_wk->now.x += p_wk->center.x;
    p_wk->now.y += p_wk->center.y;
    if (p_now != NULL) {
        *p_now = p_wk->now;
    }
    return sync == p_wk->sync_max;
}

static void func_ov186_021fe444(BrBallLine *p_wk, const BrPoint *cp_start, const BrPoint *cp_end, s32 sync_start,
                            s32 sync_max) {
    MI_CpuFill8(p_wk, 0, sizeof(BrBallLine));
    p_wk->start = *cp_start;
    p_wk->end = *cp_end;
    p_wk->sync_max = sync_max;
    p_wk->sync_start = sync_start;
    func_ov186_021fe478(p_wk, NULL, 0);
}

// pokebw2's BrBallLine_Main
static BOOL func_ov186_021fe478(BrBallLine *p_wk, BrPoint *p_now, s32 sync) {
    BOOL ret = FALSE;
    s32 now = sync - p_wk->sync_start;

    if (now < 0) {
        now = 0;
    }
    p_wk->now.x = p_wk->start.x + (p_wk->end.x - p_wk->start.x) * now / p_wk->sync_max;
    p_wk->now.y = p_wk->start.y + (p_wk->end.y - p_wk->start.y) * now / p_wk->sync_max;
    if (now >= p_wk->sync_max) {
        p_wk->now = p_wk->end;
        ret = TRUE;
    }
    if (p_now != NULL) {
        *p_now = p_wk->now;
    }
    return ret;
}

// pokebw2's BrBallFollow_Init
static void func_ov186_021fe4d8(BrBallFollow *p_wk, const BrPoint *cp_target, const BrPoint *cp_start, fx32 speed) {
    MI_CpuFill8(p_wk, 0, sizeof(BrBallFollow));
    p_wk->target = cp_target;
    p_wk->x = FX32_CONST(cp_start->x);
    p_wk->y = FX32_CONST(cp_start->y);
    p_wk->z = 0;
    p_wk->speed = speed;
}

// Moves toward the target at the speed, and onto it once it is closer than a step. TRUE once it is on it
// pokebw2's BrBallFollow_Main
static BOOL func_ov186_021fe54c(BrBallFollow *p_wk, BrPoint *p_now) {
    VecFx32 dist;
    fx32 len;

    dist.x = FX32_CONST(p_wk->target->x) - p_wk->x;
    dist.y = FX32_CONST(p_wk->target->y) - p_wk->y;
    dist.z = 0;
    len = VEC_Mag(&dist);
    if (len != 0) {
        fx32 dx = FX_Div(FX_Mul(dist.x, p_wk->speed), len);
        fx32 dy = FX_Div(FX_Mul(dist.y, p_wk->speed), len);

        if (MATH_ABS(dist.x) > MATH_ABS(dx)) {
            p_wk->x += dx;
        } else {
            p_wk->x = FX32_CONST(p_wk->target->x);
        }
        if (MATH_ABS(dist.y) > MATH_ABS(dy)) {
            p_wk->y += dy;
        } else {
            p_wk->y = FX32_CONST(p_wk->target->y);
        }
    }
    if (p_now != NULL) {
        p_now->x = FX_Whole(p_wk->x);
        p_now->y = FX_Whole(p_wk->y);
    }
    return len == 0;
}
