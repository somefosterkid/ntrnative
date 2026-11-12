// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov287/b_plist_anm.c, which is the same code in
// Black 2/White 2's overlay 287. The names are Black's dsd names; pokebw2's guesses are in the comments.
#include "battle/b_plist_anm.h"
#include "battle/b_plist_main.h"
#include "gfl/bg_sys.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "system/palanm.h"

// The battle party list's buttons (b_plist_anm.c, named by the ROM's embedded string): the pieces of each button in
// each of its states, cut from the list's tilemaps, drawn onto the sub screen's BG 2, and the press animation that
// draws a button pressed and then released

// Where a button is on the screen and its size, in tiles
typedef struct {
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} BPlistButtonRect;

static void func_ov198_0220b460(u16 *dst, const u16 *src, u8 x, u8 y, u8 width, u8 height);
static u16 *func_ov198_0220b4b0(BPlistWork *work, u8 button, u8 state, u8 mode);
static void func_ov198_0220b5ac(BPlistWork *work, u16 *buf, u8 button, u8 state, u8 mode);
static void func_ov198_0220b6d0(BPlistWork *work, u8 button, u8 state, u8 mode);
static u8 func_ov198_0220bbb4(BPlistWork *work);

static const BPlistButtonRect data_ov198_02211a74[32] = {
#include "asset/b_plist_anm_data_ov198_02211a74.inc"
};

// Cuts every button's pieces out of the buttons' tilemap (BPlistAnm_CutButtonScrn)
void func_ov198_0220b120(BPlistWork *work, const void *scrn) {
    func_ov198_0220b460(work->plateScrn[0][0], scrn, 0, 0, 16, 6);
    func_ov198_0220b460(work->plateScrn[0][1], scrn, 0, 6, 16, 6);
    func_ov198_0220b460(work->plateScrn[0][2], scrn, 0, 12, 16, 6);
    func_ov198_0220b460(work->plateScrn[0][3], scrn, 0, 18, 16, 6);
    func_ov198_0220b460(work->plateScrn[1][0], scrn, 16, 0, 16, 6);
    func_ov198_0220b460(work->plateScrn[1][1], scrn, 16, 6, 16, 6);
    func_ov198_0220b460(work->plateScrn[1][2], scrn, 16, 12, 16, 6);
    func_ov198_0220b460(work->plateScrn[1][3], scrn, 16, 18, 16, 6);
    func_ov198_0220b460(work->moveButtonScrn[0], scrn, 0, 39, 13, 5);
    func_ov198_0220b460(work->moveButtonScrn[1], scrn, 0, 44, 13, 5);
    func_ov198_0220b460(work->moveButtonScrn[2], scrn, 13, 39, 13, 5);
    func_ov198_0220b460(work->moveButtonScrn[3], scrn, 13, 44, 13, 5);
    func_ov198_0220b460(work->button12Scrn[0], scrn, 0, 49, 5, 5);
    func_ov198_0220b460(work->button12Scrn[1], scrn, 5, 49, 5, 5);
    func_ov198_0220b460(work->button12Scrn[2], scrn, 10, 49, 5, 5);
    func_ov198_0220b460(work->button12Scrn[3], scrn, 15, 49, 5, 5);
    func_ov198_0220b460(work->button13Scrn[0], scrn, 0, 54, 5, 5);
    func_ov198_0220b460(work->button13Scrn[1], scrn, 5, 54, 5, 5);
    func_ov198_0220b460(work->button13Scrn[2], scrn, 10, 54, 5, 5);
    func_ov198_0220b460(work->button13Scrn[3], scrn, 15, 54, 5, 5);
    func_ov198_0220b460(work->button6Scrn[0], scrn, 26, 24, 5, 5);
    func_ov198_0220b460(work->button6Scrn[1], scrn, 26, 29, 5, 5);
    func_ov198_0220b460(work->button6Scrn[2], scrn, 26, 34, 5, 5);
    func_ov198_0220b460(work->button6Scrn[3], scrn, 26, 39, 5, 5);
    func_ov198_0220b460(work->button27Scrn[0], scrn, 0, 24, 26, 5);
    func_ov198_0220b460(work->button27Scrn[1], scrn, 0, 29, 26, 5);
    func_ov198_0220b460(work->button27Scrn[2], scrn, 0, 34, 26, 5);
    func_ov198_0220b460(work->button28Scrn[0], scrn, 0, 59, 5, 2);
    func_ov198_0220b460(work->button28Scrn[1], scrn, 5, 59, 5, 2);
    func_ov198_0220b460(work->button28Scrn[2], scrn, 10, 59, 5, 2);
}

// Cuts the page buttons' pieces out of the pages' tilemap (BPlistAnm_CutPageScrn)
void func_ov198_0220b3b4(BPlistWork *work, const void *scrn) {
    func_ov198_0220b460(work->button7Scrn[0], scrn, 0, 0, 30, 17);
    func_ov198_0220b460(work->button7Scrn[1], scrn, 0, 17, 30, 17);
    func_ov198_0220b460(work->button7Scrn[2], scrn, 0, 34, 30, 17);
    func_ov198_0220b460(work->button14Scrn[0], scrn, 0, 51, 16, 6);
    func_ov198_0220b460(work->button14Scrn[1], scrn, 16, 51, 16, 6);
    func_ov198_0220b460(work->button14Scrn[2], scrn, 0, 57, 16, 6);
    func_ov198_0220b460(work->button14Scrn[3], scrn, 16, 57, 16, 6);
}

// Copies a rectangle of tiles, width by height at (x, y), out of a screen 32 tiles wide into a packed buffer
// (BPlistAnm_CopyScrnRect)
static void func_ov198_0220b460(u16 *dst, const u16 *src, u8 x, u8 y, u8 width, u8 height) {
    u16 i, j;

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            dst[i * width + j] = src[(y + i) * 32 + x + j];
        }
    }
}

// A button's piece in a state (BPlistAnm_GetButtonScrn)
static u16 *func_ov198_0220b4b0(BPlistWork *work, u8 button, u8 state, u8 mode) {
    switch (button) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        if (mode == 0) {
            return work->plateScrn[0][state];
        }
        return work->plateScrn[1][state];
    case 6:
        return work->button6Scrn[state];
    case 7:
        return work->button7Scrn[state];
    case 8:
    case 9:
    case 10:
    case 11:
        return work->moveButtonScrn[state];
    case 12:
        return work->button12Scrn[state];
    case 13:
        return work->button13Scrn[state];
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
        return work->button14Scrn[state];
    case 27:
        return work->button27Scrn[state];
    case 28:
    case 29:
    case 30:
    case 31:
        return work->button28Scrn[state];
    }
    return NULL;
}

// Fills buf with a button's tiles in a state; a party plate changes for an egg, a fainted Pokémon or a partner's
// (BPlistAnm_MakeButtonScrn)
static void func_ov198_0220b5ac(BPlistWork *work, u16 *buf, u8 button, u8 state, u8 mode) {
    u16 *src;
    u8 width;
    int size;
    u8 idx;
    u16 tiles[2];
    u8 i, j;

    src = func_ov198_0220b4b0(work, button, state, mode);
    width = data_ov198_02211a74[button].width;
    size = width * data_ov198_02211a74[button].height;
    MI_CpuCopy8(src, buf, size * 2);

    switch (button) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        idx = func_ov198_022110d4(work, button);
        if (work->pokemon[idx].species == 0) {
            break;
        }
        if (work->pokemon[idx].isEgg) {
            tiles[0] = buf[width * 2 + 5];
            tiles[1] = buf[width * 3 + 5];
            for (i = 0; i < 2; i++) {
                for (j = 0; j < 9; j++) {
                    buf[(i + 2) * width + j + 6] = tiles[i];
                }
            }
        } else if (work->pokemon[idx].hp == 0) {
            for (i = 0; i < size; i++) {
                buf[i] = (buf[i] & 0xfff) | (((state & 1) + 7) << 12);
            }
        } else if (func_ov198_02210f04(work, idx) == TRUE) {
            for (i = 0; i < size; i++) {
                buf[i] = (buf[i] & 0xfff) | (((state & 1) + 5) << 12);
            }
        }
        break;
    }
}

// Draws a button in a state on the sub screen's BG 2 (BPlistAnm_PutButton)
static void func_ov198_0220b6d0(BPlistWork *work, u8 button, u8 state, u8 mode) {
    u16 *buf = GFL_HeapAllocate(HEAPID_TAIL(work->param->heapId),
                                data_ov198_02211a74[button].width * data_ov198_02211a74[button].height * 2, FALSE,
                                "b_plist_anm.c", 517);

    func_ov198_0220b5ac(work, buf, button, state, mode);
    GFL_BGSysLoadScrAreaAll(6, buf, data_ov198_02211a74[button].x, data_ov198_02211a74[button].y,
                            data_ov198_02211a74[button].width, data_ov198_02211a74[button].height);
    GFL_BGSysQueueScrLoad(6);
    GFL_HeapFree(buf);
}

// Starts a button's press animation (BPlistAnm_StartButtonAnm)
void func_ov198_0220b760(BPlistWork *work, u8 button) {
    work->animMode = 0;
    if (button <= 5 && func_ov198_02210b00(work, button) == 2) {
        work->animMode = 1;
    }
    work->animSeq = 0;
    work->animCount = 0;
    work->animButton = button;
    work->animActive = TRUE;
}

// Runs the press animation: the button drawn pressed, then released a few frames later (BPlistAnm_MainButtonAnm)
void func_ov198_0220b7ac(BPlistWork *work) {
    if (!work->animActive) {
        return;
    }

    switch (work->animSeq) {
    case 0:
        func_ov198_0220b6d0(work, work->animButton, 1, work->animMode);
        work->animSeq++;
        break;
    case 1:
        if (work->animCount == 4) {
            func_ov198_0220b6d0(work, work->animButton, 0, work->animMode);
            work->animCount = 0;
            work->animSeq++;
        } else {
            work->animCount++;
        }
        break;
    case 2:
        if (work->animCount == 1) {
            work->animCount = 0;
            work->animSeq = 0;
            work->animActive = FALSE;
        } else {
            work->animCount++;
        }
        break;
    }
}

// Draws a page's buttons (BPlistAnm_PutPageButtons)
void func_ov198_0220b848(BPlistWork *work, u8 page) {
    u16 i;
    int state;

    switch (page) {
    case 0:
        for (i = 0; i < 6; i++) {
            state = func_ov198_02210b00(work, i);
            if (state == 0) {
                func_ov198_0220b6d0(work, i, 3, 1);
            } else if (state == 1) {
                func_ov198_0220b6d0(work, i, 0, 0);
            } else if (state == 2) {
                func_ov198_0220b6d0(work, i, 0, 1);
            }
        }
        if (work->param->unk1F == 1 || work->param->unk1F == 2) {
            func_ov198_0220bbf0(work);
        } else {
            func_ov198_0220b6d0(work, 6, 0, 0);
        }
        break;
    case 8:
        for (i = 0; i < 6; i++) {
            state = func_ov198_02210b00(work, i);
            if (state == 0) {
                func_ov198_0220b6d0(work, i, 3, 1);
            } else if (state == 1) {
                func_ov198_0220b6d0(work, i, 0, 0);
            } else if (state == 2) {
                func_ov198_0220b6d0(work, i, 0, 1);
            }
        }
        func_ov198_0220b6d0(work, 6, 0, 0);
        break;
    case 1:
        func_ov198_0220b6d0(work, 6, 0, 0);
        func_ov198_0220b6d0(work, 7, 0, 0);
        if (work->pokemon[func_ov198_022110d4(work, work->param->partyIndex)].isEgg) {
            func_ov198_0220b6d0(work, 8, 3, 0);
            func_ov198_0220b6d0(work, 10, 3, 0);
        } else {
            func_ov198_0220b6d0(work, 8, 0, 0);
            func_ov198_0220b6d0(work, 10, 0, 0);
        }
        break;
    case 2:
        if (func_ov198_0220bbb4(work) == TRUE) {
            func_ov198_0220b6d0(work, 12, 0, 0);
            func_ov198_0220b6d0(work, 13, 0, 0);
        } else {
            func_ov198_0220b6d0(work, 12, 3, 0);
            func_ov198_0220b6d0(work, 13, 3, 0);
        }
        func_ov198_0220b6d0(work, 11, 0, 0);
        func_ov198_0220b6d0(work, 6, 0, 0);
        break;
    case 3:
        if (func_ov198_0220bbb4(work) == TRUE) {
            func_ov198_0220b6d0(work, 12, 0, 0);
            func_ov198_0220b6d0(work, 13, 0, 0);
        } else {
            func_ov198_0220b6d0(work, 12, 3, 0);
            func_ov198_0220b6d0(work, 13, 3, 0);
        }
        for (i = 0; i < 4; i++) {
            if (work->pokemon[func_ov198_022110d4(work, work->param->partyIndex)].moves[i].move != 0) {
                func_ov198_0220b6d0(work, i + 14, 0, 0);
            } else {
                func_ov198_0220b6d0(work, i + 14, 3, 0);
            }
        }
        func_ov198_0220b6d0(work, 9, 0, 0);
        func_ov198_0220b6d0(work, 6, 0, 0);
        break;
    case 4:
        func_ov198_0220b6d0(work, 6, 0, 0);
        for (i = 0; i < 4; i++) {
            if (work->param->slot == i) {
                func_ov198_0220b6d0(work, i + 28, 2, 0);
            } else {
                func_ov198_0220b6d0(work, i + 28, 0, 0);
            }
        }
        break;
    case 5:
        for (i = 0; i < 4; i++) {
            if (work->pokemon[func_ov198_022110d4(work, work->param->partyIndex)].moves[i].move != 0) {
                func_ov198_0220b6d0(work, i + 18, 0, 0);
            } else {
                func_ov198_0220b6d0(work, i + 18, 3, 0);
            }
        }
        func_ov198_0220b6d0(work, 6, 0, 0);
        break;
    case 6:
        func_ov198_0220b6d0(work, 22, 0, 0);
        func_ov198_0220b6d0(work, 23, 0, 0);
        func_ov198_0220b6d0(work, 24, 0, 0);
        func_ov198_0220b6d0(work, 25, 0, 0);
        func_ov198_0220b6d0(work, 26, 0, 0);
        func_ov198_0220b6d0(work, 6, 0, 0);
        break;
    case 7:
        func_ov198_0220b6d0(work, 27, 0, 0);
        func_ov198_0220b6d0(work, 6, 0, 0);
        break;
    }
}

// Restores the sub screen's saved palette. The caller passes the page, which this does not use
// (BPlistAnm_RestorePalette)
void func_ov198_0220bb94(BPlistWork *work, u8 page) {
    func_020215bc(work->paletteFade, work->savedPalette, PALFADE_BUFFER_SUB_BG, 0xc0, sizeof(work->savedPalette));
}

// Whether at least two of the party can fight (BPlistAnm_CanSwitch)
static u8 func_ov198_0220bbb4(BPlistWork *work) {
    u16 i;
    u16 count = 0;

    for (i = 0; i < 6; i++) {
        if (work->pokemon[i].species != 0 && !work->pokemon[i].isEgg) {
            count++;
        }
    }
    return count >= 2;
}

// Draws the return button, disabled (state 3) when func_ov198_02211128 finds no swap (BPlistAnm_PutReturnButton)
void func_ov198_0220bbf0(BPlistWork *work) {
    u8 pos1, pos2;

    if (func_ov198_02211128(work, &pos1, &pos2, FALSE) == TRUE) {
        func_ov198_0220b6d0(work, 6, 0, 0);
    } else {
        func_ov198_0220b6d0(work, 6, 3, 0);
    }
}
