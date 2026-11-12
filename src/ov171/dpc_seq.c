// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov256/dpc_seq.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/dpc_seq.h"
#include "app/dpc_bmp.h"
#include "app/dpc_main.h"
#include "app/dpc_obj.h"
#include "app/dpc_ui.h"
#include "constants/sound.h"
#include "gfl/gx_layers.h"
#include "gfl/graphics.h"
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/sound.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/math.h"
#include "system/printsys.h"
#include "system/wipe.h"
#include "app/dendou_pc.h"

// The Hall of Fame PC's sequences: the pages of teams, and a page's Pokémon turning on their ring to bring the
// chosen one to the front. The ROM doesn't name this file; the name is a guess, after box2_seq.c

enum {
    DPCSEQ_INIT,
    DPCSEQ_RELEASE,
    DPCSEQ_WIPE,
    DPCSEQ_BUTTON_ANIME,
    DPCSEQ_PAGE_MAIN,
    DPCSEQ_PAGE_CHANGE,
    DPCSEQ_POKE_IN,
    DPCSEQ_POKE_MAIN,
    DPCSEQ_POKE_OUT,
    DPCSEQ_POKE_TURN,
    DPCSEQ_END_SET,
    DPCSEQ_END,
};

typedef u32 (*DpcSeqFunc)(DendouPCWork *wk);

static u32 func_ov171_021dcea4(DendouPCWork *wk);
static u32 func_ov171_021dcf38(DendouPCWork *wk);
static u32 func_ov171_021dcfa8(DendouPCWork *wk);
static u32 func_ov171_021dcfd4(DendouPCWork *wk);
static u32 func_ov171_021dcff4(DendouPCWork *wk);
static u32 func_ov171_021dd0e8(DendouPCWork *wk);
static u32 func_ov171_021dd10c(DendouPCWork *wk);
static u32 func_ov171_021dd218(DendouPCWork *wk);
static u32 func_ov171_021dd38c(DendouPCWork *wk);
static u32 func_ov171_021dd3f4(DendouPCWork *wk);
static u32 func_ov171_021dd498(DendouPCWork *wk);
u32 func_ov171_021dd4a4(DendouPCWork *wk, u32 next);
static u32 func_ov171_021dd4d0(DendouPCWork *wk, u32 next);
static u32 func_ov171_021dd500(DendouPCWork *wk, u32 id, u32 sequence, u32 next);
static BOOL func_ov171_021dd51c(DendouPCWork *wk, s8 move);
static BOOL func_ov171_021dd558(DendouPCWork *wk, s8 move);
static BOOL func_ov171_021dd5a8(DendouPCWork *wk, s32 pos);
static u8 func_ov171_021dd5d4(DendouPCWork *wk, s8 from, s8 to, s8 direction);
static void func_ov171_021dd620(DendouPCWork *wk, s8 move);
static BOOL func_ov171_021dd774(DendouPCWork *wk);

// How far apart the ring's Pokémon are, in degrees, and how many degrees the ring turns in a frame, by their number
// pokebw2's sDpcSeqTurnAngle
static const u8 data_ov171_021deb56[HALL_OF_FAME_TEAM_SIZE] = { 0, 180, 120, 90, 72, 60 };
// pokebw2's sDpcSeqTurnSpeed
static const u8 data_ov171_021deb50[HALL_OF_FAME_TEAM_SIZE] = { 5, 5, 5, 5, 4, 4 };

// pokebw2's sDpcSeqFuncs
static const DpcSeqFunc data_ov171_021deb5c[] = {
    func_ov171_021dcea4,   func_ov171_021dcf38,  func_ov171_021dcfa8,    func_ov171_021dcfd4, func_ov171_021dcff4, func_ov171_021dd0e8,
    func_ov171_021dd10c, func_ov171_021dd218, func_ov171_021dd38c, func_ov171_021dd3f4,    func_ov171_021dd498,
};

// pokebw2's DpcSeq_Main
BOOL func_ov171_021dce70(DendouPCWork *wk) {
    wk->seq = data_ov171_021deb5c[wk->seq](wk);
    if (wk->seq == DPCSEQ_END) {
        return FALSE;
    }
    func_ov171_021de030(wk);
    func_ov171_021dd8dc(wk);
    return TRUE;
}

static u32 func_ov171_021dcea4(DendouPCWork *wk) {
    GFL_OvlLoad(OVERLAY_ID(67));

    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    G2_BlendNone();
    G2S_BlendNone();
    GFL_BGSysSetDisplayLayout(GX_DISP_SELECT_SUB_MAIN);
    GX_SetMasterBrightness(-16);
    GXS_SetMasterBrightness(-16);

    func_ov171_021dccc8(wk);
    func_ov171_021dc910();
    func_ov171_021dc92c();
    func_ov171_021dca7c();
    func_ov171_021dcb70(wk);
    func_ov171_021dcc24(wk);
    func_ov171_021dd874(wk);
    func_ov171_021ddfe8(wk);
    func_ov171_021dd0e8(wk);
    func_ov171_021dcc0c();
    func_0203d34c(TRUE, HEAPID_DENDOU_PC);
    func_ov171_021dc8b8(wk);

    return func_ov171_021dd4a4(wk, DPCSEQ_PAGE_MAIN);
}


static u32 func_ov171_021dcf38(DendouPCWork *wk) {
    func_ov171_021dc8d4(wk);
    func_ov171_021de014(wk);
    func_ov171_021dd8bc(wk);
    func_ov171_021dcc88(wk);
    func_ov171_021dcb8c(wk);
    func_ov171_021dca40();
    func_ov171_021dce28(wk);

    GX_SetMasterBrightness(-16);
    GXS_SetMasterBrightness(-16);
    G2_BlendNone();
    G2S_BlendNone();
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);

    GFL_OvlUnload(OVERLAY_ID(67));
    return DPCSEQ_END;
}


// pokebw2's DpcSeq_Wipe
static u32 func_ov171_021dcfa8(DendouPCWork *wk) {
    if (func_0201c6b4(wk->printQueue) == TRUE && GFL_WipeIsFinished() == TRUE) {
        return wk->wipeNext;
    }
    return DPCSEQ_WIPE;
}

// pokebw2's DpcSeq_ButtonAnime
static u32 func_ov171_021dcfd4(DendouPCWork *wk) {
    if (func_ov171_021de0ac(wk, wk->animeObj) == FALSE) {
        return wk->animeNext;
    }
    return DPCSEQ_BUTTON_ANIME;
}

// pokebw2's DpcSeq_PageMain
static u32 func_ov171_021dcff4(DendouPCWork *wk) {
    u32 ret;

    if (func_0201c6b4(wk->printQueue) == FALSE) {
        return DPCSEQ_PAGE_MAIN;
    }

    ret = func_ov171_021de8a8(wk);
    switch (ret) {
    case DPCUI_LEFT:
        if (func_ov171_021dd51c(wk, -1) == TRUE) {
            GFL_SndSEPlay(SEQ_SE_SELECT4);
            return func_ov171_021dd500(wk, DPCOBJ_ARROW_LEFT, 12, DPCSEQ_PAGE_CHANGE);
        }
        break;
    case DPCUI_RIGHT:
        if (func_ov171_021dd51c(wk, 1) == TRUE) {
            GFL_SndSEPlay(SEQ_SE_SELECT4);
            return func_ov171_021dd500(wk, DPCOBJ_ARROW_RIGHT, 13, DPCSEQ_PAGE_CHANGE);
        }
        break;
    case DPCUI_CLOSE:
        GFL_SndSEPlay(SEQ_SE_CLOSE1);
        wk->param->result = DENDOU_PC_RESULT_CLOSE;
        return func_ov171_021dd500(wk, DPCOBJ_CLOSE, 8, DPCSEQ_END_SET);
    case DPCUI_RETURN:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        wk->param->result = DENDOU_PC_RESULT_RETURN;
        return func_ov171_021dd500(wk, DPCOBJ_RETURN, 9, DPCSEQ_END_SET);
    case DPCUI_DECIDE:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        wk->pokeNext = wk->pokePos;
        return DPCSEQ_POKE_IN;
    case DPCUI_POKE:
    case DPCUI_POKE + 1:
    case DPCUI_POKE + 2:
    case DPCUI_POKE + 3:
    case DPCUI_POKE + 4:
    case DPCUI_POKE + 5:
        wk->pokeNext = ret - DPCUI_POKE;
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        return DPCSEQ_POKE_IN;
    }
    return DPCSEQ_PAGE_MAIN;
}

// pokebw2's DpcSeq_PageChange
static u32 func_ov171_021dd0e8(DendouPCWork *wk) {
    wk->pokePos = 0;
    func_ov171_021de388(wk);
    func_ov171_021dd964(wk);
    func_ov171_021ddaf0(wk);
    return DPCSEQ_PAGE_MAIN;
}

// pokebw2's DpcSeq_PokeIn
static u32 func_ov171_021dd10c(DendouPCWork *wk) {
    s8 pos;

    switch (wk->subSeq) {
    case 0:
        func_ov171_021de080(wk, DPCOBJ_ARROW_LEFT, 18);
        func_ov171_021de080(wk, DPCOBJ_ARROW_RIGHT, 19);
        func_ov171_021de080(wk, DPCOBJ_CLOSE, 14);
        GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG1, TRUE);
        func_ov171_021de76c(wk, TRUE);
        func_ov171_021dcba4(wk);
        wk->subSeq++;
        break;
    case 1:
        if (func_ov171_021dcbe8(wk) == FALSE) {
            wk->subSeq = 0;
            pos = wk->pokePos;
            if (wk->pokeNext == pos) {
                wk->pokePos = wk->pokeNext;
                func_ov171_021ddc28(wk);
                return DPCSEQ_POKE_MAIN;
            }
            wk->pokePos = wk->pokeNext;
            if (func_ov171_021dd5a8(wk, wk->pokeNext) == TRUE) {
                wk->speed = -data_ov171_021deb50[wk->teams[wk->page].count - 1];
                func_ov171_021dd620(wk, -func_ov171_021dd5d4(wk, pos, wk->pokeNext, 1));
            } else {
                wk->speed = data_ov171_021deb50[wk->teams[wk->page].count - 1];
                func_ov171_021dd620(wk, func_ov171_021dd5d4(wk, pos, wk->pokeNext, -1));
            }
            GFL_SndSEPlay(SEQ_SE_TRIPLE_S);
            return DPCSEQ_POKE_TURN;
        }
        break;
    }
    return DPCSEQ_POKE_IN;
}

// pokebw2's DpcSeq_PokeMain
static u32 func_ov171_021dd218(DendouPCWork *wk) {
    u32 ret;
    s8 pos;

    if (func_0201c6b4(wk->printQueue) == FALSE) {
        return DPCSEQ_POKE_MAIN;
    }

    ret = func_ov171_021de914(wk);
    switch (ret) {
    case DPCUI_LEFT:
        if (func_ov171_021dd558(wk, 1) == TRUE) {
            wk->speed = -data_ov171_021deb50[wk->teams[wk->page].count - 1];
            func_ov171_021dd620(wk, -1);
            GFL_SndSEPlay(SEQ_SE_TRIPLE_S);
            return DPCSEQ_POKE_TURN;
        }
        break;
    case DPCUI_RIGHT:
        if (func_ov171_021dd558(wk, -1) == TRUE) {
            wk->speed = data_ov171_021deb50[wk->teams[wk->page].count - 1];
            func_ov171_021dd620(wk, 1);
            GFL_SndSEPlay(SEQ_SE_TRIPLE_S);
            return DPCSEQ_POKE_TURN;
        }
        break;
    case DPCUI_RETURN:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        return func_ov171_021dd500(wk, DPCOBJ_RETURN, 9, DPCSEQ_POKE_OUT);
    case DPCUI_POKE:
    case DPCUI_POKE + 1:
    case DPCUI_POKE + 2:
    case DPCUI_POKE + 3:
    case DPCUI_POKE + 4:
    case DPCUI_POKE + 5:
        pos = wk->pokePos;
        if (ret - DPCUI_POKE != pos) {
            wk->pokePos = ret - DPCUI_POKE;
            if (func_ov171_021dd5a8(wk, ret - DPCUI_POKE) == TRUE) {
                wk->speed = -data_ov171_021deb50[wk->teams[wk->page].count - 1];
                func_ov171_021dd620(wk, -func_ov171_021dd5d4(wk, pos, ret - DPCUI_POKE, 1));
            } else {
                wk->speed = data_ov171_021deb50[wk->teams[wk->page].count - 1];
                func_ov171_021dd620(wk, func_ov171_021dd5d4(wk, pos, ret - DPCUI_POKE, -1));
            }
            GFL_SndSEPlay(SEQ_SE_TRIPLE_S);
            return DPCSEQ_POKE_TURN;
        }
        break;
    case DPCUI_CLOSE:
    case DPCUI_DECIDE:
        break;
    }
    return DPCSEQ_POKE_MAIN;
}

// pokebw2's DpcSeq_PokeOut
static u32 func_ov171_021dd38c(DendouPCWork *wk) {
    switch (wk->subSeq) {
    case 0:
        func_ov171_021de76c(wk, FALSE);
        func_ov171_021dcba4(wk);
        GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG1, FALSE);
        wk->subSeq++;
        break;
    case 1:
        if (func_ov171_021dcbe8(wk) == FALSE) {
            func_ov171_021de080(wk, DPCOBJ_ARROW_LEFT, 4);
            func_ov171_021de080(wk, DPCOBJ_ARROW_RIGHT, 5);
            func_ov171_021de080(wk, DPCOBJ_CLOSE, 0);
            func_ov171_021ddfbc(wk);
            wk->subSeq = 0;
            return DPCSEQ_PAGE_MAIN;
        }
        break;
    }
    return DPCSEQ_POKE_OUT;
}

// pokebw2's DpcSeq_PokeTurn
static u32 func_ov171_021dd3f4(DendouPCWork *wk) {
    u32 i;

    if (func_ov171_021dd774(wk) == FALSE) {
        for (i = 0; i < wk->teams[wk->page].count; i++) {
            wk->fade[i] = wk->fadeNext[i];
        }
        func_ov171_021dcbe8(wk);
        func_ov171_021ddc28(wk);
        return DPCSEQ_POKE_MAIN;
    }

    for (i = 0; i < wk->teams[wk->page].count; i++) {
        if (wk->fadeNow[i] > wk->fadeNext[i]) {
            wk->fadeNow[i]--;
        } else if (wk->fadeNow[i] < wk->fadeNext[i]) {
            wk->fadeNow[i]++;
        }
    }
    func_ov171_021dcba4(wk);
    return DPCSEQ_POKE_TURN;
}

// pokebw2's DpcSeq_EndSet
static u32 func_ov171_021dd498(DendouPCWork *wk) {
    return func_ov171_021dd4d0(wk, DPCSEQ_RELEASE);
}

// pokebw2's DpcSeq_SetFadeIn
u32 func_ov171_021dd4a4(DendouPCWork *wk, u32 next) {
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, WIPE_COLOR_BLACK, 6, 1, HEAPID_DENDOU_PC);
    wk->wipeNext = next;
    return DPCSEQ_WIPE;
}

// pokebw2's DpcSeq_SetFadeOut
static u32 func_ov171_021dd4d0(DendouPCWork *wk, u32 next) {
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 6, 1, HEAPID_DENDOU_PC);
    wk->wipeNext = next;
    return DPCSEQ_WIPE;
}

// pokebw2's DpcSeq_SetButtonAnime
static u32 func_ov171_021dd500(DendouPCWork *wk, u32 id, u32 sequence, u32 next) {
    func_ov171_021de080(wk, id, sequence);
    wk->animeObj = id;
    wk->animeNext = next;
    return DPCSEQ_BUTTON_ANIME;
}

// Moves to another page, around the ends, and returns whether the page changed
// pokebw2's DpcSeq_MovePage
static BOOL func_ov171_021dd51c(DendouPCWork *wk, s8 move) {
    s8 old = wk->page;

    wk->page += move;
    if (wk->page < 0) {
        wk->page = wk->pageMax - 1;
    } else if (wk->page >= wk->pageMax) {
        wk->page = 0;
    }
    if (wk->page != old) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's DpcSeq_MovePoke
static BOOL func_ov171_021dd558(DendouPCWork *wk, s8 move) {
    s8 old = wk->pokePos;

    wk->pokePos += move;
    if (wk->pokePos < 0) {
        wk->pokePos = wk->teams[wk->page].count - 1;
    } else if (wk->pokePos >= wk->teams[wk->page].count) {
        wk->pokePos = 0;
    }
    if (wk->pokePos != old) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's DpcSeq_IsPokeOnLeft
static BOOL func_ov171_021dd5a8(DendouPCWork *wk, s32 pos) {
    s16 x, y;

    func_ov171_021de0e4(wk, pos + func_ov171_021de5f4(wk), &x, &y);
    if (x < 128) {
        return TRUE;
    }
    return FALSE;
}

// How many places apart two Pokémon are, going around the ring in a direction
// pokebw2's DpcSeq_CountTurnSteps
static u8 func_ov171_021dd5d4(DendouPCWork *wk, s8 from, s8 to, s8 direction) {
    u8 i;

    for (i = 1; i < HALL_OF_FAME_TEAM_SIZE; i++) {
        from += direction;
        if (from < 0) {
            from = wk->teams[wk->page].count - 1;
        } else if (from >= wk->teams[wk->page].count) {
            from = 0;
        }
        if (from == to) {
            break;
        }
    }
    return i;
}

// Starts turning the ring by a number of places: each Pokémon goes to where the one that many places on is
// pokebw2's DpcSeq_SetTurn
static void func_ov171_021dd620(DendouPCWork *wk, s8 move) {
    u32 i;
    s16 pos = wk->pokePos;
    s16 next = pos + move;
    s32 speed, places, frames;

    if (next < 0) {
        next += wk->teams[wk->page].count;
    } else if (next >= wk->teams[wk->page].count) {
        next -= wk->teams[wk->page].count;
    }

    for (i = 0; i < wk->teams[wk->page].count; i++) {
        wk->angleNext[pos] = wk->angle[next];
        wk->fadeNext[pos] = wk->fade[next];

        pos += move / MATH_ABS(move);
        if (pos < 0) {
            pos = wk->teams[wk->page].count - 1;
        } else if (pos >= wk->teams[wk->page].count) {
            pos = 0;
        }
        next += move / MATH_ABS(move);
        if (next < 0) {
            next = wk->teams[wk->page].count - 1;
        } else if (next >= wk->teams[wk->page].count) {
            next = 0;
        }
    }

    // The frames a place takes, for each place
    speed = MATH_ABS(wk->speed);
    places = MATH_ABS(move);
    frames = data_ov171_021deb56[wk->teams[wk->page].count - 1] / speed;
    wk->stepCount = frames * places;
}

// Turns the ring a step, and returns whether it is still turning
// pokebw2's DpcSeq_Turn
static BOOL func_ov171_021dd774(DendouPCWork *wk) {
    u32 i;
    u32 base = func_ov171_021de5f4(wk);

    if (wk->stepCount == 0) {
        for (i = 0; i < wk->teams[wk->page].count; i++) {
            wk->angle[i] = wk->angleNext[i];
            func_ov171_021de608(wk, base + i, wk->angle[i]);
        }
        func_ov171_021de684(wk);
        return FALSE;
    }

    for (i = 0; i < wk->teams[wk->page].count; i++) {
        wk->angle[i] += wk->speed;
        if (wk->angle[i] < 0) {
            wk->angle[i] += 360;
        } else if (wk->angle[i] >= 360) {
            wk->angle[i] -= 360;
        }
        func_ov171_021de608(wk, base + i, wk->angle[i]);
    }
    func_ov171_021de684(wk);
    wk->stepCount--;
    return TRUE;
}
