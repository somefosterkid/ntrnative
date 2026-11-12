// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov287/b_plist_main.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "battle/b_plist_main.h"
#include "battle/b_app_tool.h"
#include "battle/b_plist_anm.h"
#include "battle/b_plist_bmp.h"
#include "battle/b_plist_cursor.h"
#include "battle/b_plist_obj.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "pml/hm_check.h"
#include "pml/item.h"
#include "pml/personal.h"
#include "pml/poke_party.h"
#include "pml/waza.h"
#include "system/app_menu_common.h"
#include "system/bgwinfrm.h"
#include "system/bmp_winframe.h"
#include "system/cursor_move.h"
#include "system/gf_font.h"
#include "system/hp_gauge.h"
#include "system/palanm.h"
#include "system/printsys.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "text/system/egg_demo.h"
#include "gfl/tcb.h"

// The battle party list's entry, task and states (b_plist_main.c, named by the ROM's embedded string): the list
// copies the party's data once, then runs one state function a frame, each returning the next state, until state
// 0x21 frees everything and sets the parameter's done. The pages are 0 the list, 1 the chosen Pokémon, 2 its
// summary, 3 its moves, 4 one move, 5 the move to restore PP to, 6 the move to forget, 7 the confirmation and 8 the
// position to fill.

void func_ov198_0220f478(TCB *tcb, void *data);
int func_ov198_0220f4e0(BPlistWork *work);
int func_ov198_0220f65c(BPlistWork *work);
static int func_ov198_0220f698(BPlistWork *work);
int func_ov198_0220f6c0(BPlistWork *work);
int func_ov198_0220f7c4(BPlistWork *work);
int func_ov198_0220f88c(BPlistWork *work);
int func_ov198_0220f9f8(BPlistWork *work);
int func_ov198_0220fb44(BPlistWork *work);
int func_ov198_0220fce0(BPlistWork *work);
static int func_ov198_0220fdd4(BPlistWork *work);
int func_ov198_0220fed0(BPlistWork *work);
int func_ov198_0220ffcc(BPlistWork *work);
int func_ov198_022100a0(BPlistWork *work);
int func_ov198_022100bc(BPlistWork *work);
static int func_ov198_022101ac(BPlistWork *work);
static int func_ov198_022101c0(BPlistWork *work);
static int func_ov198_022101d4(BPlistWork *work);
static int func_ov198_022101e8(BPlistWork *work);
int func_ov198_022101fc(BPlistWork *work);
static int func_ov198_02210220(BPlistWork *work);
static int func_ov198_02210234(BPlistWork *work);
static int func_ov198_02210248(BPlistWork *work);
int func_ov198_0221025c(BPlistWork *work);
int func_ov198_02210290(BPlistWork *work);
int func_ov198_022102a8(BPlistWork *work);
int func_ov198_022102bc(BPlistWork *work);
int func_ov198_0221034c(BPlistWork *work);
int func_ov198_0221039c(BPlistWork *work);
static int func_ov198_022103b8(BPlistWork *work);
static int func_ov198_02210408(BPlistWork *work);
BOOL func_ov198_02210420(TCB *tcb, BPlistWork *work);
BAppCursor *func_ov196_0220af40(HeapID heapId);
void func_ov198_022104ec(BPlistWork *work);
static void func_ov198_022105e4(void);
void func_ov198_02210608(BPlistWork *work);
void func_ov198_02210710(BPlistWork *work);
void func_ov198_02210774(BPlistWork *work);
void func_ov198_022107a8(BPlistWork *work, PartyPkm *pkm, BPlistPokemon *row);
void func_ov198_0221099c(BPlistWork *work);
BOOL func_ov198_02210a4c(BPlistWork *work);
u8 func_ov198_02210b30(BPlistWork *work, int pos, int dir);
static void func_ov198_02210bfc(BPlistWork *work, u8 page);
BOOL func_ov198_02210c88(BPlistWork *work, u8 page);
void func_ov198_02210d4c(BPlistWork *work, u8 page);
BOOL func_ov198_02210e20(BPlistWork *work);
static u8 func_ov198_02210ed4(BPlistWork *work);
static BOOL func_ov198_02210efc(BPlistWork *work);
static BOOL func_ov198_02210f1c(BPlistWork *work);
static BOOL func_ov198_02210f4c(BPlistWork *work, int pos);
BOOL func_ov198_02210f84(BPlistWork *work, u8 pos);
static BOOL func_ov198_02210f98(BPlistWork *work, u8 pos);
void func_ov198_02210fcc(BPlistWork *work, u8 page);
void func_ov198_02211034(BPlistWork *work);
void func_ov198_022110a4(BPlistWork *work, u8 pos1, u8 pos2);
static BOOL func_ov198_022110b8(BPlistWork *work, u8 slot);
void func_ov198_022110e0(BPlistWork *work, u8 idx, u8 pos);
void func_ov198_022110f8(BPlistWork *work, u16 pos1, u16 pos2);
int func_ov198_02211168(BPlistWork *work);
BOOL func_ov198_02211190(BPlistWork *work);
int func_ov198_022112ac(BPlistWork *work);
int func_ov198_02211378(BPlistWork *work);
void func_ov198_0221139c(u8 *x, u8 *y, u8 pos);
void func_ov198_022113b8(u16 *buf, u8 bg, u8 pos);
void func_ov198_022113fc(BPlistWork *work, u8 pos, BOOL back);
static BOOL func_ov198_02211420(BPlistWork *work);
static int func_ov198_02211810(BPlistWork *work);
BOOL func_ov198_02211844(BPlistWork *work);
static BOOL func_ov198_0221189c(BPlistWork *work);
void func_ov198_022118f4(BPlistWork *work);
static void func_ov198_02211944(BPlistWork *work);
static BOOL func_ov198_02211958(BPlistWork *work);
static void func_ov198_02211970(BPlistWork *work, u32 se);
void func_ov198_02211984(BPlistWork *work, u32 se);

// The NSCR files of BGs 6 and 7, by page
static const u32 data_ov198_022121d4[9][2] = {
#include "asset/b_plist_main_data_ov198_022121d4.inc"
};

// The list position of each party slot in a multi battle, by the player's column
static const u8 data_ov198_02212138[2][6] = {
    { 0, 3, 1, 4, 2, 5 },
    { 3, 0, 4, 1, 5, 2 },
};

static int (*const data_ov198_0221221c[])(BPlistWork *work) = {
    func_ov198_0220f4e0,
    func_ov198_0220f65c,
    func_ov198_0220f698,
    func_ov198_0220f6c0,
    func_ov198_0220f88c,
    func_ov198_0220f9f8,
    func_ov198_0220fb44,
    func_ov198_0220fce0,
    func_ov198_022112ac,
    func_ov198_02211810,
    func_ov198_02211378,
    func_ov198_022101ac,
    func_ov198_022101c0,
    func_ov198_022101d4,
    func_ov198_022101e8,
    func_ov198_022101fc,
    func_ov198_02210220,
    func_ov198_02210234,
    func_ov198_02210248,
    func_ov198_02211168,
    func_ov198_0221025c,
    func_ov198_02210290,
    func_ov198_022102a8,
    func_ov198_022102bc,
    func_ov198_0221034c,
    func_ov198_0220fed0,
    func_ov198_0220ffcc,
    func_ov198_022100a0,
    func_ov198_022100bc,
    func_ov198_0221039c,
    func_ov198_0220fdd4,
    func_ov198_022103b8,
    func_ov198_02210408,
};

// pokebw2's BPlistMain_StateWaitWipe



void func_ov198_0220f3e8(BPlistParam *param) {
    BPlistWork *work;

    if (param->partyIndex > 5) {
        param->partyIndex = 0;
    }
    work = GFL_HeapAllocate(param->heapId, sizeof(BPlistWork), TRUE, "b_plist_main.c", 304);
    GFL_TCBMgrAddTask(param->tcbManager, func_ov198_0220f478, work, 100);
    work->param = param;
    if (param->unk1F != 5) {
        param->slot = 0;
    } else if (func_ov198_02210efc(work) == TRUE && param->unk1C == 1) {
        param->partyIndex++;
    }
    work->paletteFade = param->paletteFade;
    work->unk_2178 = 0;
    work->unk_2176 = param->partyIndex;
    work->unk_2540 = FALSE;
}

void func_ov198_0220f478(TCB *tcb, void *data) {
    BPlistWork *work = data;

    if (work->param->unk34 == 1) {
        func_ov198_02211944(work);
        work->unk_2178 = 0x21;
    }
    if (work->unk_2178 != 0x21) {
        work->unk_2178 = data_ov198_0221221c[work->unk_2178](work);
    }
    if (work->unk_2178 == 0x21 && func_ov198_02210420(tcb, work) == TRUE) {
        return;
    }
    GFL_TCBExMgrUpdate(work->tcbExMgr);
    func_ov198_0220f1a4(work);
    func_ov198_0220b7ac(work);
    func_ov198_0220e22c(work);
}

int func_ov198_0220f4e0(BPlistWork *work) {
    G2S_BlendNone();
    GXS_SetMasterBrightness(-16);
    work->tcbExMgr = GFL_TCBExMgrCreate(work->param->heapId, work->param->heapId, 1, 4);
    if (work->param->unk1F == 4) {
        work->unk_2180 = 6;
    } else if (work->param->unk1F == 5) {
        work->unk_2180 = 4;
    } else {
        work->unk_2180 = 0;
    }
    work->cursor = func_ov196_0220af40(work->param->heapId);
    func_ov198_02211034(work);
    func_ov198_0221099c(work);
    func_ov198_022104ec(work);
    func_ov198_02210608(work);
    func_ov198_02210710(work);
    func_ov198_02210d4c(work, work->unk_2180);
    func_ov198_0220b848(work, work->unk_2180);
    func_ov198_0220bb94(work, work->unk_2180);
    func_ov198_0220e2b4(work);
    if (work->param->unk1F == 5) {
        func_ov198_0220f060(work);
    }
    func_ov198_0220ea68(work, work->unk_2180);
    func_ov198_0220bc24(work);
    func_ov198_0220be2c(work, work->unk_2180);
    func_ov198_0220e25c(work);
    if (*work->param->usingKeys == TRUE) {
        work->cursorVisible = TRUE;
    } else {
        work->cursorVisible = FALSE;
    }
    func_ov196_0220afdc(work->cursor, work->cursorVisible);
    if (work->unk_2180 == 0) {
        if (func_ov198_02210f04(work, func_ov198_022110d4(work, 0)) == TRUE) {
            work->param->partyIndex = 1;
        }
        func_ov198_02211998(work, work->unk_2180, work->param->partyIndex);
    } else if (work->param->unk1F == 5) {
        func_ov198_02211998(work, work->unk_2180, work->param->slot);
    } else {
        func_ov198_02211998(work, work->unk_2180, 0);
    }
    func_ov198_02210bfc(work, work->unk_2180);
    func_0202173c(work->paletteFade, 10, 0xffff, 0, 16, 0, 0, work->param->tcbManager);
    work->unk_2540 = TRUE;
    return 1;
}


int func_ov198_0220f65c(BPlistWork *work) {
    if (func_0201c6b4(work->printQueue) == TRUE) {
        GFL_WipeSet(WIPE_MODE_SUB, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, WIPE_COLOR_BLACK, 6, 1, work->param->heapId);
        return 2;
    }
    return 1;
}

static int func_ov198_0220f698(BPlistWork *work) {
    if (GFL_WipeIsFinished() == TRUE) {
        if (work->param->unk1F == 4) {
            return 0x19;
        }
        if (work->param->unk1F == 5) {
            return 0x1e;
        }
        return 3;
    }
    return 2;
}

int func_ov198_0220f6c0(BPlistWork *work) {
    u8 pos1;
    u8 pos2;

    if (func_02021ed8(work->paletteFade) != 0) {
        return 3;
    }
    if (func_ov198_02211958(work) == TRUE) {
        return 0x1f;
    }
    if (func_ov198_02210a4c(work) == TRUE) {
        if (work->param->partyIndex == 6) {
            if (work->param->unk1F == 2) {
                if (func_ov198_02211128(work, &pos1, &pos2, TRUE) == TRUE) {
                    if (pos1 < pos2) {
                        work->param->unk48[pos1] = 0xff;
                    } else {
                        work->param->unk48[pos2] = 0xff;
                    }
                    work->unk_2499[0] = pos1;
                    work->unk_2499[1] = pos2;
                    work->animSeq = 0;
                    func_ov198_02211970(work, SEQ_SE_CANCEL2);
                    return 9;
                }
            } else if (work->param->unk1F != 1) {
                func_ov198_02211970(work, SEQ_SE_CANCEL2);
                func_ov198_0220b760(work, 6);
                func_ov198_02211944(work);
                return 0x1f;
            }
        } else {
            func_ov198_02211970(work, SEQ_SE_DECIDE2);
            func_ov198_0220b760(work, work->param->partyIndex);
            if (work->param->unk1F == 3) {
                return func_ov198_0220f7c4(work);
            }
            work->unk_217c = 0xc;
            return 0x1d;
        }
    }
    return 3;
}


int func_ov198_0220f7c4(BPlistWork *work) {
    BPlistParam *param = work->param;

    if (work->pokemon[func_ov198_022110d4(work, param->partyIndex)].isEgg) {
        GFL_MsgDataLoadStrbuf(work->msgData, EggDemo_Text_091, work->strBuf);
        func_ov198_0220e13c(work);
        work->param->partyIndex = 6;
        work->unk_217c = 0x1f;
        func_ov198_02211944(work);
        return 0x17;
    }
    if (param->unk38[func_ov198_022110d4(work, param->partyIndex)] != 0) {
        func_ov198_0220e1d4(work);
        func_ov198_0220e13c(work);
        work->param->partyIndex = 6;
        work->unk_217c = 0x1f;
        func_ov198_02211944(work);
        return 0x17;
    }
    if (GetItemParam(param->item, 0x27, param->heapId) != 0 && GetItemParam(param->item, 0x28, param->heapId) == 0) {
        work->unk_217c = 0x12;
        return 0x1d;
    }
    param->unk48[0] = func_ov198_022110d4(work, param->partyIndex);
    return 0x1f;
}

int func_ov198_0220f88c(BPlistWork *work) {
    u32 ret;

    if (func_ov198_02211958(work) == TRUE) {
        return 0x1f;
    }
    ret = func_02025de8(work->cursorMove);
    switch (ret) {
    case 0:
        func_ov198_02211970(work, SEQ_SE_DECIDE2);
        func_ov198_0220b760(work, 7);
        if (func_ov198_02210e20(work) == TRUE) {
            if (work->param->unk1F != 2) {
                work->param->unk48[work->param->unk21] = func_ov198_022110d4(work, work->param->partyIndex);
                return 0x1f;
            }
            if (func_ov198_02211844(work) == FALSE) {
                func_ov198_022118f4(work);
                return 0x1f;
            }
            work->unk_217c = 0x13;
            return 0x1d;
        }
        work->unk_217c = 0x15;
        return 0x1d;
    case 1:
        if (func_ov198_02210ed4(work) == TRUE) {
            break;
        }
        func_ov198_02211970(work, SEQ_SE_DECIDE2);
        func_ov198_0220b760(work, 8);
        work->unk_217c = 0xd;
        return 0x1d;
    case 2:
        if (func_ov198_02210ed4(work) == TRUE) {
            break;
        }
        func_ov198_02211970(work, SEQ_SE_DECIDE2);
        func_ov198_0220b760(work, 10);
        work->unk_217c = 0xe;
        return 0x1d;
    case 3:
        func_ov198_02211970(work, SEQ_SE_CANCEL2);
        func_ov198_0220b760(work, 6);
        work->unk_217c = 0xb;
        return 0x1d;
    case CURSOR_MOVE_CANCEL:
        func_ov198_02211970(work, SEQ_SE_CANCEL2);
        func_ov198_0220b760(work, 6);
        work->unk_217c = 0xb;
        return 0x1d;
    case CURSOR_MOVE_MOVED:
    case CURSOR_MOVE_CURSOR_ON:
        func_ov198_02211970(work, SEQ_SE_SELECT1);
        break;
    case CURSOR_MOVE_NONE:
    case CURSOR_MOVE_EDGE_RIGHT:
    case CURSOR_MOVE_EDGE_LEFT:
    case CURSOR_MOVE_EDGE_DOWN:
    case CURSOR_MOVE_EDGE_UP:
        break;
    }
    return 4;
}

int func_ov198_0220f9f8(BPlistWork *work) {
    u8 pos;

    if (func_ov198_02211958(work) == TRUE) {
        return 0x1f;
    }
    switch (func_02025de8(work->cursorMove)) {
    case 0:
        pos = func_ov198_02210b30(work, work->param->partyIndex, -1);
        if (pos == 0xff) {
            break;
        }
        work->param->partyIndex = pos;
        func_ov198_02211970(work, SEQ_SE_DECIDE2);
        func_ov198_0220b760(work, 12);
        work->unk_217c = 0x14;
        return 0x1d;
    case 1:
        pos = func_ov198_02210b30(work, work->param->partyIndex, 1);
        if (pos == 0xff) {
            break;
        }
        work->param->partyIndex = pos;
        func_ov198_02211970(work, SEQ_SE_DECIDE2);
        func_ov198_0220b760(work, 13);
        work->unk_217c = 0x14;
        return 0x1d;
    case 2:
        func_ov198_02211970(work, SEQ_SE_DECIDE2);
        func_ov198_0220b760(work, 11);
        work->unk_217c = 0xe;
        return 0x1d;
    case 3:
        func_ov198_02211970(work, SEQ_SE_CANCEL2);
        func_ov198_0220b760(work, 6);
        work->unk_2190[0] = 1;
        work->unk_217c = 0xc;
        return 0x1d;
    case CURSOR_MOVE_CANCEL:
        func_ov198_02211970(work, SEQ_SE_CANCEL2);
        func_ov198_0220b760(work, 6);
        work->unk_2190[0] = 1;
        work->unk_217c = 0xc;
        return 0x1d;
    case CURSOR_MOVE_MOVED:
    case CURSOR_MOVE_CURSOR_ON:
        func_ov198_02211970(work, SEQ_SE_SELECT1);
        break;
    case CURSOR_MOVE_NONE:
    case CURSOR_MOVE_EDGE_RIGHT:
    case CURSOR_MOVE_EDGE_LEFT:
    case CURSOR_MOVE_EDGE_DOWN:
    case CURSOR_MOVE_EDGE_UP:
        break;
    }
    return 5;
}

int func_ov198_0220fb44(BPlistWork *work) {
    u32 ret;
    u8 pos;

    if (func_ov198_02211958(work) == TRUE) {
        return 0x1f;
    }
    ret = func_02025de8(work->cursorMove);
    switch (ret) {
    case 0:
    case 1:
    case 2:
    case 3:
        if (work->pokemon[func_ov198_022110d4(work, work->param->partyIndex)].moves[ret].move == 0) {
            break;
        }
        func_ov198_02211970(work, SEQ_SE_DECIDE2);
        func_ov198_0220b760(work, ret + 14);
        work->param->slot = ret;
        work->unk_217c = 0xf;
        return 0x1d;
    case 4:
        pos = func_ov198_02210b30(work, work->param->partyIndex, -1);
        if (pos == 0xff) {
            break;
        }
        work->param->partyIndex = pos;
        func_ov198_02211970(work, SEQ_SE_DECIDE2);
        func_ov198_0220b760(work, 12);
        work->unk_217c = 0x14;
        return 0x1d;
    case 5:
        pos = func_ov198_02210b30(work, work->param->partyIndex, 1);
        if (pos == 0xff) {
            break;
        }
        work->param->partyIndex = pos;
        func_ov198_02211970(work, SEQ_SE_DECIDE2);
        func_ov198_0220b760(work, 13);
        work->unk_217c = 0x14;
        return 0x1d;
    case 6:
        func_ov198_02211970(work, SEQ_SE_DECIDE2);
        func_ov198_0220b760(work, 9);
        work->unk_217c = 0xd;
        return 0x1d;
    case 7:
        func_ov198_02211970(work, SEQ_SE_CANCEL2);
        func_ov198_0220b760(work, 6);
        work->unk_2190[0] = 2;
        work->unk_217c = 0xc;
        return 0x1d;
    case CURSOR_MOVE_CANCEL:
        func_ov198_02211970(work, SEQ_SE_CANCEL2);
        func_ov198_0220b760(work, 6);
        work->unk_2190[0] = 2;
        work->unk_217c = 0xc;
        return 0x1d;
    case CURSOR_MOVE_MOVED:
    case CURSOR_MOVE_CURSOR_ON:
        func_ov198_02211970(work, SEQ_SE_SELECT1);
        break;
    case CURSOR_MOVE_NONE:
    case CURSOR_MOVE_EDGE_RIGHT:
    case CURSOR_MOVE_EDGE_LEFT:
    case CURSOR_MOVE_EDGE_DOWN:
    case CURSOR_MOVE_EDGE_UP:
        break;
    }
    return 6;
}




int func_ov198_0220fce0(BPlistWork *work) {
    u32 ret;

    if (func_ov198_02211958(work) == TRUE) {
        return 0x1f;
    }
    ret = func_02025de8(work->cursorMove);
    switch (ret) {
    case 0:
    case 1:
    case 2:
    case 3:
        if (work->param->slot == ret) {
            break;
        }
        if (work->pokemon[func_ov198_022110d4(work, work->param->partyIndex)].moves[ret].move == 0) {
            break;
        }
        func_ov198_02211970(work, SEQ_SE_DECIDE2);
        work->param->slot = ret;
        return 0xf;
    case 4:
        func_ov198_02211970(work, SEQ_SE_CANCEL2);
        func_ov198_0220b760(work, 6);
        work->unk_217c = 0xe;
        return 0x1d;
    case CURSOR_MOVE_CANCEL:
        func_ov198_02211970(work, SEQ_SE_CANCEL2);
        func_ov198_0220b760(work, 6);
        work->unk_217c = 0xe;
        return 0x1d;
    case CURSOR_MOVE_MOVED:
    case CURSOR_MOVE_CURSOR_ON:
        func_ov198_02211970(work, SEQ_SE_SELECT1);
        break;
    case CURSOR_MOVE_NONE:
    case CURSOR_MOVE_EDGE_RIGHT:
    case CURSOR_MOVE_EDGE_LEFT:
    case CURSOR_MOVE_EDGE_DOWN:
    case CURSOR_MOVE_EDGE_UP:
        break;
    }
    return 0x7;
}



// pokebw2's BPlistMain_StateMoveInfoOnly
static int func_ov198_0220fdd4(BPlistWork *work) {
    u32 ret;

    if (func_02021ed8(work->paletteFade) != 0) {
        return 0x1e;
    }
    if (func_ov198_02211958(work) == TRUE) {
        return 0x1f;
    }
    ret = func_02025de8(work->cursorMove);
    switch (ret) {
    case 0:
    case 1:
    case 2:
    case 3:
        if (work->param->slot == ret) {
            break;
        }
        if (work->pokemon[func_ov198_022110d4(work, work->param->partyIndex)].moves[ret].move == 0) {
            break;
        }
        func_ov198_02211970(work, SEQ_SE_DECIDE2);
        work->param->slot = ret;
        return 0xf;
    case 4:
        func_ov198_02211970(work, SEQ_SE_CANCEL2);
        func_ov198_0220b760(work, 6);
        return 0x1f;
    case CURSOR_MOVE_CANCEL:
        func_ov198_02211970(work, SEQ_SE_CANCEL2);
        func_ov198_0220b760(work, 6);
        return 0x1f;
    case CURSOR_MOVE_MOVED:
    case CURSOR_MOVE_CURSOR_ON:
        func_ov198_02211970(work, SEQ_SE_SELECT1);
        break;
    case CURSOR_MOVE_NONE:
    case CURSOR_MOVE_EDGE_RIGHT:
    case CURSOR_MOVE_EDGE_LEFT:
    case CURSOR_MOVE_EDGE_DOWN:
    case CURSOR_MOVE_EDGE_UP:
        break;
    }
    return 0x1e;
}

int func_ov198_0220fed0(BPlistWork *work) {
    u32 ret;

    if (func_02021ed8(work->paletteFade) != 0) {
        return 0x19;
    }
    ret = func_02025de8(work->cursorMove);
    switch (ret) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        work->param->slot = ret;
        work->unk_2190[1] = ret;
        func_ov198_02211970(work, SEQ_SE_DECIDE2);
        func_ov198_0220b760(work, ret + 22);
        work->unk_217c = 0x11;
        return 0x1d;
    case 5:
        func_ov198_02211970(work, SEQ_SE_CANCEL2);
        work->param->slot = 4;
        func_ov198_0220b760(work, 6);
        return 0x1f;
    case CURSOR_MOVE_CANCEL:
        func_ov198_02211970(work, SEQ_SE_CANCEL2);
        work->param->slot = 4;
        func_ov198_0220b760(work, 6);
        return 0x1f;
    case CURSOR_MOVE_MOVED:
    case CURSOR_MOVE_CURSOR_ON:
        func_ov198_02211970(work, SEQ_SE_SELECT1);
        break;
    case CURSOR_MOVE_NONE:
    case CURSOR_MOVE_EDGE_RIGHT:
    case CURSOR_MOVE_EDGE_LEFT:
    case CURSOR_MOVE_EDGE_DOWN:
    case CURSOR_MOVE_EDGE_UP:
        break;
    }
    return 0x19;
}


int func_ov198_0220ffcc(BPlistWork *work) {
    switch (func_02025de8(work->cursorMove)) {
    case 0:
        func_ov198_02211970(work, SEQ_SE_DECIDE2);
        func_ov198_0220b760(work, 27);
        if (func_ov198_02210f1c(work) == FALSE) {
            work->unk_217c = 0x1f;
        } else {
            work->unk_217c = 0x1b;
        }
        return 0x1d;
    case 1:
        func_ov198_02211970(work, SEQ_SE_CANCEL2);
        func_ov198_0220b760(work, 6);
        work->unk_2190[2] = 0;
        work->unk_217c = 0x10;
        return 0x1d;
    case CURSOR_MOVE_CANCEL:
        func_ov198_02211970(work, SEQ_SE_CANCEL2);
        func_ov198_0220b760(work, 6);
        work->unk_2190[2] = 0;
        work->unk_217c = 0x10;
        return 0x1d;
    case CURSOR_MOVE_MOVED:
    case CURSOR_MOVE_CURSOR_ON:
        func_ov198_02211970(work, SEQ_SE_SELECT1);
        break;
    case CURSOR_MOVE_NONE:
    case CURSOR_MOVE_EDGE_RIGHT:
    case CURSOR_MOVE_EDGE_LEFT:
    case CURSOR_MOVE_EDGE_DOWN:
    case CURSOR_MOVE_EDGE_UP:
        break;
    }
    return 0x1a;
}


int func_ov198_022100a0(BPlistWork *work) {
    func_ov198_0220d6ac(work);
    work->unk_2193 = 1;
    work->unk_217c = 0x1a;
    return 0x17;
}

int func_ov198_022100bc(BPlistWork *work) {
    u32 ret;
    u8 slot;

    ret = func_02025de8(work->cursorMove);
    switch (ret) {
    case 0:
    case 1:
    case 2:
    case 3:
        slot = func_ov198_022110d4(work, work->param->partyIndex);
        if (work->pokemon[slot].moves[ret].move == 0) {
            break;
        }
        work->param->slot = ret;
        work->param->unk48[0] = slot;
        func_ov198_02211970(work, SEQ_SE_DECIDE2);
        func_ov198_0220b760(work, ret + 18);
        return 0x1f;
    case 4:
        func_ov198_02211970(work, SEQ_SE_CANCEL2);
        func_ov198_0220b760(work, 6);
        work->unk_217c = 0xb;
        return 0x1d;
    case CURSOR_MOVE_CANCEL:
        func_ov198_02211970(work, SEQ_SE_CANCEL2);
        func_ov198_0220b760(work, 6);
        work->unk_217c = 0xb;
        return 0x1d;
    case CURSOR_MOVE_MOVED:
    case CURSOR_MOVE_CURSOR_ON:
        func_ov198_02211970(work, SEQ_SE_SELECT1);
        break;
    case CURSOR_MOVE_NONE:
    case CURSOR_MOVE_EDGE_RIGHT:
    case CURSOR_MOVE_EDGE_LEFT:
    case CURSOR_MOVE_EDGE_DOWN:
    case CURSOR_MOVE_EDGE_UP:
        break;
    }
    return 0x1c;
}



// pokebw2's BPlistMain_StateToPage0
static int func_ov198_022101ac(BPlistWork *work) {
    if (func_ov198_02210c88(work, 0) == FALSE) {
        return 0xb;
    }
    return 3;
}

// pokebw2's BPlistMain_StateToPage1
static int func_ov198_022101c0(BPlistWork *work) {
    if (func_ov198_02210c88(work, 1) == FALSE) {
        return 0xc;
    }
    return 4;
}

// pokebw2's BPlistMain_StateToPage2
static int func_ov198_022101d4(BPlistWork *work) {
    if (func_ov198_02210c88(work, 2) == FALSE) {
        return 0xd;
    }
    return 5;
}

// pokebw2's BPlistMain_StateToPage3
static int func_ov198_022101e8(BPlistWork *work) {
    if (func_ov198_02210c88(work, 3) == FALSE) {
        return 0xe;
    }
    return 6;
}

// pokebw2's BPlistMain_StateToPage4
int func_ov198_022101fc(BPlistWork *work) {
    if (func_ov198_02210c88(work, 4) == FALSE) {
        return 0xf;
    }
    if (work->param->unk1F == 5) {
        return 0x1e;
    }
    return 7;
}

// pokebw2's BPlistMain_StateToPage6
static int func_ov198_02210220(BPlistWork *work) {
    if (func_ov198_02210c88(work, 6) == FALSE) {
        return 0x10;
    }
    return 0x19;
}

// pokebw2's BPlistMain_StateToPage7
static int func_ov198_02210234(BPlistWork *work) {
    if (func_ov198_02210c88(work, 7) == FALSE) {
        return 0x11;
    }
    return 0x1a;
}

// pokebw2's BPlistMain_StateToPage5
static int func_ov198_02210248(BPlistWork *work) {
    if (func_ov198_02210c88(work, 5) == FALSE) {
        return 0x12;
    }
    return 0x1c;
}

int func_ov198_0221025c(BPlistWork *work) {
    func_ov198_0220ea68(work, work->unk_2180);
    func_ov198_0220be2c(work, work->unk_2180);
    func_ov198_0220b848(work, work->unk_2180);
    func_ov198_02210bfc(work, work->unk_2180);
    if (work->unk_2180 == 2) {
        return 5;
    }
    return 6;
}


int func_ov198_02210290(BPlistWork *work) {
    func_ov198_0220e13c(work);
    work->unk_217c = 0x16;
    return 0x17;
}


int func_ov198_022102a8(BPlistWork *work) {
    func_0201f7a8(work->msgWins[1].window, 0);
    return 4;
}

int func_ov198_022102bc(BPlistWork *work) {
    switch (func_0201ce30(work->printStream)) {
    case 0:
        if (func_02035c60() == TRUE || (GCTX_HIDGetHeldKeys() & (PAD_BUTTON_A | PAD_BUTTON_B))) {
            func_0201ce5c(work->printStream, 0);
        }
        work->unk_1eb4 = FALSE;
        break;
    case 1:
        if (work->unk_1eb4 == FALSE) {
            if (func_02035c60() == TRUE || (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B))) {
                func_ov198_02211984(work, SEQ_SE_MESSAGE);
                func_0201ce38(work->printStream);
                work->unk_1eb4 = TRUE;
            }
        }
        break;
    case 2:
        func_0201ce48(work->printStream);
        work->printStream = NULL;
        work->unk_1eb4 = FALSE;
        return 0x18;
    }
    return 0x17;
}


int func_ov198_0221034c(BPlistWork *work) {
    if (func_ov198_02211958(work) == TRUE) {
        return 0x1f;
    }
    if ((GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) || func_02035c60() == TRUE) {
        if (work->unk_2193 == TRUE) {
            func_0201f7a8(work->msgWins[1].window, 0);
            work->unk_2193 = FALSE;
        }
        return work->unk_217c;
    }
    return 0x18;
}



int func_ov198_0221039c(BPlistWork *work) {
    if (work->animActive == FALSE) {
        return work->unk_217c;
    }
    return 0x1d;
}


// pokebw2's BPlistMain_StateStartExit
static int func_ov198_022103b8(BPlistWork *work) {
    if (func_ov198_02210efc(work) == TRUE && work->param->partyIndex != 6) {
        work->param->partyIndex /= 2;
    }
    func_0202173c(work->paletteFade, 0xa, 0xffff, 0, 0, 16, 0, work->param->tcbManager);
    return 0x20;
}

// pokebw2's BPlistMain_StateWaitFadeOut
static int func_ov198_02210408(BPlistWork *work) {
    if (func_02021ed8(work->paletteFade) == 0) {
        return 0x21;
    }
    return 0x20;
}

BOOL func_ov198_02210420(TCB *tcb, BPlistWork *work) {
    if (work->unk_2540 == TRUE) {
        if (func_0201c6b4(work->printQueue) == FALSE) {
            if (work->param->unk34 == 1) {
                func_0201c6ec(work->printQueue);
            } else {
                return FALSE;
            }
        }
        if (GFL_WipeIsFinished() == FALSE) {
            if (work->param->unk34 == 1) {
                func_020221b8();
            } else {
                return FALSE;
            }
        }
        if (work->printStream != NULL) {
            func_0201ce48(work->printStream);
        }
        if (work->unk_2194 != NULL) {
            func_0202ca0c(work->unk_2194);
        }
        func_ov198_02210774(work);
        func_ov198_0220e74c(work);
        func_ov198_0220be04(work);
        func_ov198_022105e4();
        GFL_TCBExMgrFree(work->tcbExMgr);
        func_ov198_022119e8(work);
        func_ov196_0220af64(work->cursor);
        if (work->cursorVisible == TRUE) {
            *work->param->usingKeys = TRUE;
        } else {
            *work->param->usingKeys = FALSE;
        }
    }
    work->param->done = TRUE;
    GFL_TCBRemove(tcb);
    GFL_HeapFree(work);
    return TRUE;
}


// pokebw2's BPlistMain_InitBG
void func_ov198_022104ec(BPlistWork *work) {
    {
        BGSysLCDConfig lcdConfig = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D };

        GFL_BGSysSetLCDConfigForEngine(&lcdConfig, BGSYS_ENGINE_SUB);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf800,
            GX_BG_CHARBASE_0x00000,
            0x8000,
            GX_BG_EXTPLTT_23,
            3,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(7, &setup, BGMODE_TEXT);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf000,
            GX_BG_CHARBASE_0x00000,
            0x8000,
            GX_BG_EXTPLTT_23,
            2,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(6, &setup, BGMODE_TEXT);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe800,
            GX_BG_CHARBASE_0x10000,
            0x8000,
            GX_BG_EXTPLTT_01,
            1,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(5, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(5);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe000,
            GX_BG_CHARBASE_0x18000,
            0x8000,
            GX_BG_EXTPLTT_01,
            0,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(4, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(4);
    }
    GFL_BGSysClearCharCore(5, BGSYS_TILE_SIZE_16, 0, work->param->heapId);
    GFL_BGSysClearCharCore(4, BGSYS_TILE_SIZE_16, 0, work->param->heapId);
    GFL_BGSysQueueScrLoad(5);
    GFL_BGSysQueueScrLoad(4);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, TRUE);
}

// pokebw2's BPlistMain_ExitBG
static void func_ov198_022105e4(void) {
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ,
                           FALSE);
    GFL_BGSysReleaseBG(4);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(6);
    GFL_BGSysReleaseBG(7);
}

// pokebw2's BPlistMain_LoadGraphics
void func_ov198_02210608(BPlistWork *work) {
    ArcTool *arc;
    NNSG2dScreenData *scrn;
    void *buf;

    arc = GFL_ArcSysCreateFileHandle(100, HEAPID_TAIL(work->param->heapId));
    GFL_BGSysLoadArcNCGRStatic(arc, 18, 7, 0, 0, TRUE, work->param->heapId);
    buf = GFL_G2DIOReadNSCRArc(arc, 16, TRUE, &scrn, work->param->heapId);
    func_ov198_0220b120(work, scrn->rawData);
    GFL_HeapFree(buf);
    buf = GFL_G2DIOReadNSCRArc(arc, 17, TRUE, &scrn, work->param->heapId);
    func_ov198_0220b3b4(work, scrn->rawData);
    GFL_HeapFree(buf);
#ifdef BLACK2
    func_02021640(work->paletteFade, 100, 20, work->param->heapId, PALFADE_BUFFER_SUB_BG, 0x1e0, 0);
#else
    func_02021640(work->paletteFade, 100, 19, work->param->heapId, PALFADE_BUFFER_SUB_BG, 0x1e0, 0);
#endif
    GFL_ArcToolFree(arc);
    MI_CpuCopy8(func_02021728(work->paletteFade, PALFADE_BUFFER_SUB_BG) + 0xc0, work->savedPalette,
               sizeof(work->savedPalette));
    LoadSysMsgBox(4, 1, 14, 0, work->param->heapId);
    func_020216d4(work->paletteFade, PALFADE_VRAM_SUB_BG, 0xe0, 0x20);
    func_02021640(work->paletteFade, ARCID_FONT, 5, work->param->heapId, PALFADE_BUFFER_SUB_BG, 0x20, 0xd0);
}

void func_ov198_02210710(BPlistWork *work) {
    work->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_EGG_DEMO, work->param->heapId);
    work->smallFont = GFL_FontCreate(ARCID_FONT, 3, 0, FALSE, work->param->heapId);
    work->wordSet = GFL_WordSetSystemCreateDefault(work->param->heapId);
    work->printQueue = func_0201c450(0x800, work->param->heapId);
    work->strBuf = GFL_StrBufCreate(0x200, work->param->heapId);
}

void func_ov198_02210774(BPlistWork *work) {
    GFL_MsgDataFree(work->msgData);
    GFL_FontFree(work->smallFont);
    GFL_WordSetSystemFree(work->wordSet);
    func_0201c4c0(work->printQueue);
    GFL_StrBufFree(work->strBuf);
}



// NONMATCHING: narrows PokeParty_GetSex's result before the sex bit field, which the original doesn't (it
// matches once PokeParty_GetSex returns u8)
// pokebw2's BPlistMain_SetPokemon
void func_ov198_022107a8(BPlistWork *work, PartyPkm *pkm, BPlistPokemon *row) {
    BPlistMove *move;
    u32 i;

    row->pkm = pkm;
    if (pkm == NULL) {
        row->species = 0;
        return;
    }
    row->species = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
    if (row->species == 0) {
        return;
    }
    row->attack = PokeParty_GetParam(row->pkm, PKM_PARAM_ATTACK, NULL);
    row->defense = PokeParty_GetParam(row->pkm, PKM_PARAM_DEFENSE, NULL);
    row->speed = PokeParty_GetParam(row->pkm, PKM_PARAM_SPEED, NULL);
    row->spAttack = PokeParty_GetParam(row->pkm, PKM_PARAM_SP_ATTACK, NULL);
    row->spDefense = PokeParty_GetParam(row->pkm, PKM_PARAM_SP_DEFENSE, NULL);
    row->hp = PokeParty_GetParam(row->pkm, PKM_PARAM_HP, NULL);
    row->maxHp = PokeParty_GetParam(row->pkm, PKM_PARAM_MAX_HP, NULL);
    row->type1 = PokeParty_GetParam(row->pkm, PKM_PARAM_TYPE1, NULL);
    row->type2 = PokeParty_GetParam(row->pkm, PKM_PARAM_TYPE2, NULL);
    row->level = PokeParty_GetParam(row->pkm, PKM_PARAM_LEVEL, NULL);
    if (PokeParty_GetParam(row->pkm, PKM_PARAM_SHOW_SEX, NULL) == TRUE) {
        row->hideSex = FALSE;
    } else {
        row->hideSex = TRUE;
    }
    row->sex = PokeParty_GetSex(row->pkm);
    row->status = (u8)func_020276c4(row->pkm);
    row->isEgg = (u8)PokeParty_GetParam(row->pkm, PKM_PARAM_IS_EGG, NULL);
    row->ability = PokeParty_GetParam(row->pkm, PKM_PARAM_ABILITY, NULL);
    row->item = PokeParty_GetParam(row->pkm, PKM_PARAM_ITEM, NULL);
    row->form = PokeParty_GetParam(row->pkm, PKM_PARAM_FORM, NULL);
    row->exp = PokeParty_GetParam(row->pkm, PKM_PARAM_EXP, NULL);
    row->levelExp = getExpForPkm_Wrapper(row->pkm);
    if (row->level == 100) {
        row->nextLevelExp = row->levelExp;
    } else {
        row->nextLevelExp = PML_UtilGetPkmLvExp(row->species, row->form, row->level + 1);
    }
    for (i = 0; i < 4; i++) {
        move = &row->moves[i];
        move->move = PokeParty_GetParam(row->pkm, PKM_PARAM_MOVE1 + i, NULL);
        if (move->move == 0) {
            continue;
        }
        move->pp = PokeParty_GetParam(row->pkm, PKM_PARAM_MOVE1_PP + i, NULL);
        move->maxPp = PokeParty_GetParam(row->pkm, PKM_PARAM_MOVE1_PP_UP + i, NULL);
        move->maxPp = PML_MoveGetMaxPP(move->move, move->maxPp);
        move->type = PML_MoveGetParam(move->move, MOVE_PARAM_TYPE);
        move->category = PML_MoveGetParam(move->move, MOVE_PARAM_CATEGORY);
        if (PML_MoveIsAlwaysHit(move->move) == TRUE) {
            move->accuracy = 0;
        } else {
            move->accuracy = PML_MoveGetParam(move->move, MOVE_PARAM_ACCURACY);
        }
        move->power = PML_MoveGetParam(move->move, MOVE_PARAM_POWER);
    }
}

// pokebw2's BPlistMain_InitPokemon
void func_ov198_0221099c(BPlistWork *work) {
    unsigned long i;

    if (work->param->unk18 == FALSE) {
        for (i = 0; i < 6; i++) {
            if (i < PokeParty_GetPkmCount(work->param->party)) {
                func_ov198_022107a8(work, PokeParty_GetPkm(work->param->party, i), &work->pokemon[i]);
            } else {
                func_ov198_022107a8(work, NULL, &work->pokemon[i]);
            }
        }
    } else {
        for (i = 0; i < 3; i++) {
            if (i < PokeParty_GetPkmCount(work->param->party)) {
                func_ov198_022107a8(work, PokeParty_GetPkm(work->param->party, i), &work->pokemon[i]);
            } else {
                func_ov198_022107a8(work, NULL, &work->pokemon[i]);
            }
            if (i < PokeParty_GetPkmCount(work->param->unk8)) {
                func_ov198_022107a8(work, PokeParty_GetPkm(work->param->unk8, i), &work->pokemon[i + 3]);
            } else {
                func_ov198_022107a8(work, NULL, &work->pokemon[i + 3]);
            }
        }
    }
}

// pokebw2's BPlistMain_UpdatePartyCursor
BOOL func_ov198_02210a4c(BPlistWork *work) {
    u32 ret = func_02025de8(work->cursorMove);

    switch (ret) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        if (func_ov198_02210b00(work, ret) == 0) {
            break;
        }
        work->param->partyIndex = ret;
        return TRUE;
    case 6:
        work->param->partyIndex = 6;
        return TRUE;
    case CURSOR_MOVE_CANCEL:
        work->param->partyIndex = 6;
        return TRUE;
    case CURSOR_MOVE_MOVED:
    case CURSOR_MOVE_CURSOR_ON:
        func_ov198_02211970(work, SEQ_SE_SELECT1);
        break;
    case CURSOR_MOVE_NONE:
    case CURSOR_MOVE_EDGE_RIGHT:
    case CURSOR_MOVE_EDGE_LEFT:
    case CURSOR_MOVE_EDGE_DOWN:
    case CURSOR_MOVE_EDGE_UP:
        break;
    }
    return FALSE;
}

// pokebw2's BPlistMain_CheckPos
int func_ov198_02210b00(BPlistWork *work, int pos) {
    if (work->pokemon[func_ov198_022110d4(work, pos)].species == 0) {
        return 0;
    }
    if (func_ov198_02210f4c(work, pos) == TRUE) {
        return 1;
    }
    return 2;
}

// pokebw2's BPlistMain_GetNextPos
u8 func_ov198_02210b30(BPlistWork *work, int pos, int dir) {
    int start = pos;
    u8 next;

    if (func_ov198_02210efc(work) == TRUE) {
        u8 order[6] = { 0, 2, 4, 1, 3, 5 };

        for (pos = 0; pos < 6; pos++) {
            if (start == order[pos]) {
                break;
            }
        }
        while (TRUE) {
            pos += dir;
            if (pos < 0) {
                pos = 5;
            } else if (pos >= 6) {
                pos = 0;
            }
            next = order[pos];
            if (start == next) {
                break;
            }
            if (func_ov198_02210b00(work, next) != 0 && !work->pokemon[func_ov198_022110d4(work, next)].isEgg) {
                return next;
            }
        }
    } else {
        while (TRUE) {
            pos += dir;
            if (pos < 0) {
                pos = 5;
            } else if (pos >= 6) {
                pos = 0;
            }
            if (start == pos) {
                break;
            }
            if (func_ov198_02210b00(work, pos) != 0 && !work->pokemon[func_ov198_022110d4(work, pos)].isEgg) {
                return pos;
            }
        }
    }
    return 0xff;
}

// pokebw2's BPlistMain_DrawExpBar
static void func_ov198_02210bfc(BPlistWork *work, u8 page) {
    BPlistPokemon *row;
    u32 exp;
    u32 levelSpan;
    u8 fill;
    u16 tile;
    u8 i;

    if (page != 2) {
        return;
    }
    row = &work->pokemon[func_ov198_022110d4(work, work->param->partyIndex)];
    if (row->level < 100) {
        levelSpan = row->nextLevelExp - row->levelExp;
        exp = row->exp - row->levelExp;
    } else {
        levelSpan = 0;
        exp = 0;
    }
    fill = func_0202d01c(exp, levelSpan, 64);
    for (i = 0; i < 8; i++) {
        if (fill >= 8) {
            tile = 0x1e;
        } else {
            tile = fill + 0x16;
        }
        GFL_BGSysFillScrArea(7, tile, i + 10, 8, 1, 1, 16);
        if (fill < 8) {
            fill = 0;
        } else {
            fill -= 8;
        }
    }
    GFL_BGSysQueueScrLoad(7);
}

BOOL func_ov198_02210c88(BPlistWork *work, u8 page) {
    switch (work->pageChangeSeq) {
    case 0:
        if (func_0201c6b4(work->printQueue) == TRUE) {
            func_ov198_0220bdd4(work);
            func_ov198_0220bc70(work, page);
            func_ov198_0220be2c(work, page);
            work->pageChangeSeq++;
        }
        break;
    case 1:
        if (func_0201c6b4(work->printQueue) == TRUE) {
            func_ov198_02210d4c(work, page);
            GFL_BGSysFillScrAsync(4, 0);
            GFL_BGSysFillScrAsync(5, 0);
            func_ov198_0220e25c(work);
            func_ov198_02210bfc(work, page);
            func_ov198_0220ea68(work, page);
            func_ov198_02210fcc(work, page);
            func_ov198_0220b848(work, page);
            func_ov198_0220bb94(work, page);
            work->unk_2180 = page;
            work->pageChangeSeq = 0;
            return TRUE;
        }
        break;
    }
    return FALSE;
}


// pokebw2's BPlistMain_LoadPageScrn
void func_ov198_02210d4c(BPlistWork *work, u8 page) {
    NNSG2dScreenData *scrn;
    void *buf;
    u32 i;

    for (i = 0; i < 2; i++) {
        buf = GFL_G2DIOReadNSCR(100, data_ov198_022121d4[page][i], TRUE, &scrn, work->param->heapId);
        GFL_BGSysLoadScrAreaAll(i + 6, scrn->rawData, 0, 0, 32, 24);
        GFL_BGSysQueueScrLoad(i + 6);
        GFL_HeapFree(buf);
    }
}

// pokebw2's BPlistMain_GetSwitchError
int func_ov198_02210da8(BPlistWork *work) {
    u8 slot = func_ov198_022110d4(work, work->param->partyIndex);
    BPlistPokemon *row = &work->pokemon[slot];

    if (func_ov198_02210f04(work, slot) == TRUE) {
        return 1;
    }
    if (row->hp == 0) {
        return 2;
    }
    if (func_ov198_02210f4c(work, work->param->partyIndex) == TRUE) {
        return 3;
    }
    if (func_ov198_02210ed4(work) == TRUE) {
        return 5;
    }
    if (func_ov198_02210f98(work, work->param->partyIndex) == TRUE) {
        return 4;
    }
    if (work->param->move != 0) {
        return 6;
    }
    return 0;
}

BOOL func_ov198_02210e20(BPlistWork *work) {
    StrBuf *str;
    BPlistPokemon *row = &work->pokemon[func_ov198_022110d4(work, work->param->partyIndex)];

    switch (func_ov198_02210da8(work)) {
    case 1:
        str = GFL_MsgDataLoadStrbufNew(work->msgData, EggDemo_Text_090);
        break;
    case 2:
        str = GFL_MsgDataLoadStrbufNew(work->msgData, EggDemo_Text_087);
        loadPokemonNicknameToStrbuf(work->wordSet, 0, row->pkm);
        break;
    case 3:
        str = GFL_MsgDataLoadStrbufNew(work->msgData, EggDemo_Text_086);
        loadPokemonNicknameToStrbuf(work->wordSet, 0, row->pkm);
        break;
    case 4:
        str = GFL_MsgDataLoadStrbufNew(work->msgData, EggDemo_Text_103);
        loadPokemonNicknameToStrbuf(work->wordSet, 0, row->pkm);
        break;
    case 5:
        str = GFL_MsgDataLoadStrbufNew(work->msgData, EggDemo_Text_089);
        break;
    case 6:
        row = &work->pokemon[work->param->unk21];
        str = GFL_MsgDataLoadStrbufNew(work->msgData, EggDemo_Text_088);
        loadPokemonNicknameToStrbuf(work->wordSet, 0, row->pkm);
        break;
    case 0:
        return TRUE;
    }
    GFL_WordSetFormatStrbuf(work->wordSet, work->strBuf, str);
    GFL_StrBufFree(str);
    return FALSE;
}


// pokebw2's BPlistMain_IsEgg
static u8 func_ov198_02210ed4(BPlistWork *work) {
    return work->pokemon[func_ov198_022110d4(work, work->param->partyIndex)].isEgg ? TRUE : FALSE;
}

// pokebw2's BPlistMain_IsMulti
static BOOL func_ov198_02210efc(BPlistWork *work) {
    return work->param->unk18;
}

// pokebw2's BPlistMain_IsPartnerSlot
BOOL func_ov198_02210f04(BPlistWork *work, u32 idx) {
    if (func_ov198_02210efc(work) == TRUE && idx >= 3) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BPlistMain_IsHMMove
static BOOL func_ov198_02210f1c(BPlistWork *work) {
    BPlistParam *param = work->param;

    if (param->slot == 4) {
        return FALSE;
    }
    return isPkmMoveHmMove(param->gameData, work->pokemon[param->partyIndex].moves[param->slot].move, param->heapId);
}

// pokebw2's BPlistMain_IsInBattle
static BOOL func_ov198_02210f4c(BPlistWork *work, int pos) {
    u8 slot = func_ov198_022110d4(work, pos);

    if (func_ov198_02210efc(work) == TRUE) {
        if (slot == 0 || slot == 3) {
            return TRUE;
        }
        return FALSE;
    }
    if (slot < work->param->unk22) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BPlistMain_IsBattlePos
BOOL func_ov198_02210f84(BPlistWork *work, u8 pos) {
    if (pos < work->param->unk22) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BPlistMain_IsChosen
static BOOL func_ov198_02210f98(BPlistWork *work, u8 pos) {
    u8 slot = func_ov198_022110d4(work, pos);
    BPlistParam *param = work->param;

    if (param->unk22 != 1) {
        if (slot == param->unk1D[0] || slot == param->unk1D[1]) {
            return TRUE;
        }
        return func_ov198_022110b8(work, slot);
    }
    return FALSE;
}

void func_ov198_02210fcc(BPlistWork *work, u8 page) {
    int pos;

    switch (page) {
    case 0:
    case 8:
        work->unk_2190[0] = 0;
        work->param->slot = 0;
        pos = work->param->partyIndex;
        break;
    case 1:
        work->param->slot = 0;
        pos = work->unk_2190[0];
        break;
    case 3:
    case 4:
        pos = work->param->slot;
        break;
    case 6:
        pos = work->unk_2190[1];
        break;
    case 7:
        pos = work->unk_2190[2];
        break;
    case 2:
    case 5:
        pos = 0;
        break;
    }
    func_ov198_022119f8(work, page, pos);
}

void func_ov198_02211034(BPlistWork *work) {
    u32 i;

    if (work->param->unk18 == TRUE) {
        for (i = 0; i < 6; i++) {
            work->partyOrder[i] = data_ov198_02212138[work->param->unk1C][i];
        }
    } else {
        for (i = 0; i < 6; i++) {
            work->partyOrder[i] = i;
        }
    }
    for (i = 0; i < 3; i++) {
        work->param->unk48[i] = 0xff;
    }
    for (i = 0; i < 2; i++) {
        work->swaps[i][0] = 0xff;
        work->swaps[i][1] = 0xff;
    }
}



// pokebw2's BPlistMain_SwapOrder
void func_ov198_022110a4(BPlistWork *work, u8 pos1, u8 pos2) {
    u8 tmp = work->partyOrder[pos1];

    work->partyOrder[pos1] = work->partyOrder[pos2];
    work->partyOrder[pos2] = tmp;
}

// pokebw2's BPlistMain_IsSlotChosen
static BOOL func_ov198_022110b8(BPlistWork *work, u8 slot) {
    u32 i;

    for (i = 0; i < 3; i++) {
        if (slot == work->param->unk48[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's BPlistMain_GetPartySlot
u8 func_ov198_022110d4(BPlistWork *work, int pos) {
    return work->partyOrder[pos];
}

// pokebw2's BPlistMain_SetChosenSlot
void func_ov198_022110e0(BPlistWork *work, u8 idx, u8 pos) {
    work->param->unk48[idx] = func_ov198_022110d4(work, pos);
}

void func_ov198_022110f8(BPlistWork *work, u16 pos1, u16 pos2) {
    u8 i;

    for (i = 0; i < 2; i++) {
        if (work->swaps[i][0] == 0xff) {
            work->swaps[i][0] = pos1;
            work->swaps[i][1] = pos2;
            return;
        }
    }
}

BOOL func_ov198_02211128(BPlistWork *work, u8 *pos1, u8 *pos2, BOOL clear) {
    int i;

    for (i = 1; i >= 0; i--) {
        if (work->swaps[i][0] != 0xff) {
            *pos1 = work->swaps[i][0];
            *pos2 = work->swaps[i][1];
            if (clear == TRUE) {
                work->swaps[i][0] = 0xff;
                work->swaps[i][1] = 0xff;
            }
            return TRUE;
        }
    }
    return FALSE;
}


int func_ov198_02211168(BPlistWork *work) {
    if (func_ov198_02210c88(work, 8) == FALSE) {
        work->unk_2498 = work->param->partyIndex;
        return 0x13;
    }
    return 8;
}

BOOL func_ov198_02211190(BPlistWork *work) {
    StrBuf *str;
    u8 slot = func_ov198_022110d4(work, work->param->partyIndex);
    BPlistPokemon *row = &work->pokemon[slot];

    if (func_ov198_02210f04(work, slot) == TRUE) {
        str = GFL_MsgDataLoadStrbufNew(work->msgData, EggDemo_Text_090);
        GFL_WordSetFormatStrbuf(work->wordSet, work->strBuf, str);
        GFL_StrBufFree(str);
        return FALSE;
    }
    if (func_ov198_02210f84(work, work->param->partyIndex) == FALSE) {
        if (func_ov198_02210f84(work, slot) == TRUE) {
            str = GFL_MsgDataLoadStrbufNew(work->msgData, EggDemo_Text_107);
        } else {
            str = GFL_MsgDataLoadStrbufNew(work->msgData, EggDemo_Text_106);
        }
        loadPokemonNicknameToStrbuf(work->wordSet, 0, row->pkm);
        GFL_WordSetFormatStrbuf(work->wordSet, work->strBuf, str);
        GFL_StrBufFree(str);
        return FALSE;
    }
    if (func_ov198_02210f98(work, work->param->partyIndex) == TRUE) {
        str = GFL_MsgDataLoadStrbufNew(work->msgData, EggDemo_Text_086);
        loadPokemonNicknameToStrbuf(work->wordSet, 0, row->pkm);
        GFL_WordSetFormatStrbuf(work->wordSet, work->strBuf, str);
        GFL_StrBufFree(str);
        return FALSE;
    }
    if (row->hp != 0) {
        str = GFL_MsgDataLoadStrbufNew(work->msgData, EggDemo_Text_086);
        loadPokemonNicknameToStrbuf(work->wordSet, 0, row->pkm);
        GFL_WordSetFormatStrbuf(work->wordSet, work->strBuf, str);
        GFL_StrBufFree(str);
        return FALSE;
    }
    return TRUE;
}


int func_ov198_022112ac(BPlistWork *work) {
    if (func_02021ed8(work->paletteFade) != 0) {
        return 8;
    }
    if (func_ov198_02211958(work) == TRUE) {
        return 0x1f;
    }
    if (func_ov198_02210a4c(work) == TRUE) {
        if (work->param->partyIndex == 6) {
            func_ov198_02211970(work, SEQ_SE_CANCEL2);
            func_ov198_0220b760(work, 6);
            work->param->partyIndex = work->unk_2498;
            work->unk_217c = 0xc;
            return 0x1d;
        }
        if (work->param->partyIndex == work->unk_2498) {
            return 0xc;
        }
        if (func_ov198_02211190(work) == TRUE) {
            func_ov198_022110f8(work, work->param->partyIndex, work->unk_2498);
            func_ov198_022110e0(work, work->param->partyIndex, work->unk_2498);
            work->unk_2499[0] = work->param->partyIndex;
            work->unk_2499[1] = work->unk_2498;
            work->animSeq = 0;
            return 9;
        }
        func_ov198_0220e13c(work);
        work->unk_217c = 0xa;
        return 0x17;
    }
    return 8;
}



int func_ov198_02211378(BPlistWork *work) {
    func_0201f7a8(work->msgWins[1].window, 0);
    func_ov198_0220c688(work);
    func_ov198_0220e27c(work);
    return 8;
}



// pokebw2's BPlistMain_GetPlatePos
void func_ov198_0221139c(u8 *x, u8 *y, u8 pos) {
    *x = (pos & 1) * 16;
    *y = (pos & 1) + pos / 2 * 6;
}

// pokebw2's BPlistMain_CopyPlateScrn
void func_ov198_022113b8(u16 *buf, u8 bg, u8 pos) {
    u16 *scrn = GFL_BGSysIsScrHeapExists(bg);
    u8 x;
    u8 y;
    u8 i;

    func_ov198_0221139c(&x, &y, pos);
    for (i = 0; i < 6; i++) {
        MIi_CpuCopy16(&scrn[(y + i) * 32 + x], &buf[i * 16], 16 * sizeof(u16));
    }
}

// pokebw2's BPlistMain_MovePlateIcons
void func_ov198_022113fc(BPlistWork *work, u8 pos, BOOL back) {
    s16 dx;

    if (pos & 1) {
        if (back == FALSE) {
            dx = 8;
        } else {
            dx = -8;
        }
    } else {
        if (back == FALSE) {
            dx = -8;
        } else {
            dx = 8;
        }
    }
    func_ov198_0220f22c(work, pos, dx);
}

static BOOL func_ov198_02211420(BPlistWork *work) {
    s8 x, y;
    s8 x1, y1, x2, y2;
    s8 x3, y3, x4, y4;
    u16 *buf;

    switch (work->animSeq) {
    case 0:
        work->unk_2194 = func_0202c9b4(BGWINFRAME_TRANSFER_VBLANK, 4, work->param->heapId);
        func_0202ca3c(work->unk_2194, 0, 5, 16, 6);
        func_0202ca3c(work->unk_2194, 1, 6, 16, 6);
        func_0202ca3c(work->unk_2194, 2, 5, 16, 6);
        func_0202ca3c(work->unk_2194, 3, 6, 16, 6);
        buf = GFL_HeapAllocate(HEAPID_TAIL(work->param->heapId), 16 * 6 * sizeof(u16), FALSE, "b_plist_main.c", 2953);
        func_ov198_022113b8(buf, 5, work->unk_2499[0]);
        func_0202cac4(work->unk_2194, 0, buf);
        func_ov198_022113b8(buf, 6, work->unk_2499[0]);
        func_0202cac4(work->unk_2194, 1, buf);
        func_ov198_022113b8(buf, 5, work->unk_2499[1]);
        func_0202cac4(work->unk_2194, 2, buf);
        func_ov198_022113b8(buf, 6, work->unk_2499[1]);
        func_0202cac4(work->unk_2194, 3, buf);
        GFL_HeapFree(buf);
        func_ov198_0221139c(&x, &y, work->unk_2499[0]);
        func_0202cb4c(work->unk_2194, 0, x, y);
        func_0202cb4c(work->unk_2194, 1, x, y);
        func_ov198_0221139c(&x, &y, work->unk_2499[1]);
        func_0202cb4c(work->unk_2194, 2, x, y);
        func_0202cb4c(work->unk_2194, 3, x, y);
        func_0202cd94(work->unk_2194);
        work->animSeq++;
        break;
    case 1:
        if (work->unk_2499[0] & 1) {
            func_0202cd64(work->unk_2194, 0, 1, 0, 16);
            func_0202cd64(work->unk_2194, 1, 1, 0, 16);
        } else {
            func_0202cd64(work->unk_2194, 0, -1, 0, 16);
            func_0202cd64(work->unk_2194, 1, -1, 0, 16);
        }
        if (work->unk_2499[1] & 1) {
            func_0202cd64(work->unk_2194, 2, 1, 0, 16);
            func_0202cd64(work->unk_2194, 3, 1, 0, 16);
        } else {
            func_0202cd64(work->unk_2194, 2, -1, 0, 16);
            func_0202cd64(work->unk_2194, 3, -1, 0, 16);
        }
        work->animSeq++;
        break;
    case 2:
        func_0202cd94(work->unk_2194);
        func_ov198_022113fc(work, work->unk_2499[0], FALSE);
        func_ov198_022113fc(work, work->unk_2499[1], FALSE);
        func_0202cf98(work->unk_2194, 0, &x1, &y1);
        func_0202cf98(work->unk_2194, 2, &x2, &y2);
        if (func_0202ce40(work->unk_2194, 0) != 0) {
            break;
        }
        func_ov198_022110a4(work, work->unk_2499[0], work->unk_2499[1]);
        func_0202cb4c(work->unk_2194, 0, x2, y2);
        func_0202cb4c(work->unk_2194, 1, x2, y2);
        func_0202cb4c(work->unk_2194, 2, x1, y1);
        func_0202cb4c(work->unk_2194, 3, x1, y1);
        func_ov198_0220f2dc(work, work->unk_2499[0], work->unk_2499[1]);
        work->animSeq++;
        break;
    case 3:
        func_0202cd94(work->unk_2194);
        if (work->unk_2499[1] & 1) {
            func_0202cd64(work->unk_2194, 0, -1, 0, 16);
            func_0202cd64(work->unk_2194, 1, -1, 0, 16);
        } else {
            func_0202cd64(work->unk_2194, 0, 1, 0, 16);
            func_0202cd64(work->unk_2194, 1, 1, 0, 16);
        }
        if (work->unk_2499[0] & 1) {
            func_0202cd64(work->unk_2194, 2, -1, 0, 16);
            func_0202cd64(work->unk_2194, 3, -1, 0, 16);
        } else {
            func_0202cd64(work->unk_2194, 2, 1, 0, 16);
            func_0202cd64(work->unk_2194, 3, 1, 0, 16);
        }
        work->animSeq++;
        break;
    case 4:
        func_0202cd94(work->unk_2194);
        func_ov198_022113fc(work, work->unk_2499[0], TRUE);
        func_ov198_022113fc(work, work->unk_2499[1], TRUE);
        func_0202cf98(work->unk_2194, 0, &x3, &y3);
        func_0202cf98(work->unk_2194, 2, &x4, &y4);
        if (func_0202ce40(work->unk_2194, 0) != 0) {
            break;
        }
        work->animSeq++;
        break;
    case 5:
        func_0202ca0c(work->unk_2194);
        work->unk_2194 = NULL;
        work->animSeq = 0;
        return FALSE;
    }
    return TRUE;
}

// pokebw2's BPlistMain_StateSwapAnm
static int func_ov198_02211810(BPlistWork *work) {
    if (func_ov198_02211420(work) == FALSE) {
        if (func_ov198_0221189c(work) == FALSE) {
            return 0x1f;
        }
        func_ov198_0220bbf0(work);
        func_ov198_0220c67c(work);
        func_ov198_0220e27c(work);
        return 3;
    }
    return 9;
}

// pokebw2's BPlistMain_HasTwoFainted
BOOL func_ov198_02211844(BPlistWork *work) {
    u8 count;
    u8 fainted;
    u8 i;

    if (func_ov198_02210efc(work) == TRUE) {
        return FALSE;
    }
    count = work->param->unk22;
    if (count == 1) {
        return FALSE;
    }
    fainted = 0;
    for (i = 0; i < count; i++) {
        if (work->pokemon[func_ov198_022110d4(work, i)].hp == 0) {
            fainted++;
            if (fainted == 2) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

// pokebw2's BPlistMain_HasReserve
static BOOL func_ov198_0221189c(BPlistWork *work) {
    u8 slot;
    u8 i;

    if (func_ov198_02210efc(work) == TRUE) {
        return FALSE;
    }
    i = work->param->unk22;
    if (i == 1) {
        return FALSE;
    }
    for (; i < 6; i++) {
        slot = func_ov198_022110d4(work, i);
        if (work->pokemon[slot].species != 0 && !work->pokemon[slot].isEgg && work->pokemon[slot].hp != 0) {
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's BPlistMain_ChooseFaintedPos
void func_ov198_022118f4(BPlistWork *work) {
    u32 i;

    if (func_ov198_02210efc(work) == TRUE) {
        work->param->unk48[0] = func_ov198_022110d4(work, work->param->partyIndex);
        return;
    }
    for (i = 0; i < 3; i++) {
        if (work->pokemon[func_ov198_022110d4(work, i)].hp == 0) {
            work->param->unk48[i] = work->param->partyIndex;
            return;
        }
    }
}

// pokebw2's BPlistMain_ClearChosen
static void func_ov198_02211944(BPlistWork *work) {
    u32 i;

    for (i = 0; i < 3; i++) {
        work->param->unk48[i] = 0xff;
    }
}

// pokebw2's BPlistMain_CheckQuit
static BOOL func_ov198_02211958(BPlistWork *work) {
    if (work->param->unk30 == 1) {
        func_ov198_02211944(work);
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BPlistMain_PlaySE
static void func_ov198_02211970(BPlistWork *work, u32 se) {
    if (work->param->unk40 == 1) {
        GFL_SndSEPlay(se);
    }
}

// pokebw2's BPlistMain_PlayMessageSE
void func_ov198_02211984(BPlistWork *work, u32 se) {
    if (work->param->unk40 == 1) {
        GFL_SndSEPlay(se);
    }
}
