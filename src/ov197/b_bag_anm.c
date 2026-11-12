// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov286/b_bag_anm.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "battle/b_bag_anm.h"
#include "battle/b_bag_item.h"
#include "battle/b_bag_main.h"
#include "gfl/arc.h"
#include "gfl/heap.h"
#include "system/bgwinfrm.h"

// The battle bag's buttons (b_bag_anm.c, our guess after the ROM's b_bag_main.c and Diamond and Pearl's
// b_bag_anm.c): the 14 window frames on BG 6, from members of archive 98, which pages show which, their palette rows
// for normal, pressed and disabled, and the press animation that draws a button pressed and then released

// A button's member of archive 98 and its size in tiles
typedef struct {
    u16 member;
    u8 width;
    u8 height;
} BBagButton;

// The buttons: 0-3 the pockets, 4-9 the item slots, 10 back, 11 and 12 the previous and next page, 13 the wide bar
static const BBagButton data_ov197_0220db44[14] = {
    { 4, 16, 8 }, { 5, 16, 8 }, { 6, 16, 8 }, { 7, 16, 8 }, { 8, 16, 6 }, { 8, 16, 6 }, { 8, 16, 6 },
    { 8, 16, 6 }, { 8, 16, 6 }, { 8, 16, 6 }, { 11, 5, 5 }, { 9, 5, 5 }, { 10, 5, 5 }, { 12, 25, 5 },
};

// pokebw2's BBagAnm_CreateButtons
void func_ov197_0220bbe4(BBagWork *work) {
    ArcTool *arc;
    u32 i;

    work->buttons = func_0202c9b4(2, 14, work->param->heapId);
    arc = GFL_ArcSysCreateFileHandle(99, HEAPID_TAIL(work->param->heapId));
    for (i = 0; i < 14; i++) {
        func_0202ca3c(work->buttons, i, 6, data_ov197_0220db44[i].width, data_ov197_0220db44[i].height);
        func_0202cfec(work->buttons, i, 0, 64, 0, 64);
        func_0202cb18(work->buttons, i, arc, data_ov197_0220db44[i].member, TRUE);
    }
    GFL_ArcToolFree(arc);

    func_0202cb4c(work->buttons, 0, 0, 1);
    func_0202cb4c(work->buttons, 1, 0, 10);
    func_0202cb4c(work->buttons, 2, 16, 1);
    func_0202cb4c(work->buttons, 3, 16, 10);
    func_0202cb4c(work->buttons, 4, 32, 1);
    func_0202cb4c(work->buttons, 5, 48, 1);
    func_0202cb4c(work->buttons, 6, 32, 7);
    func_0202cb4c(work->buttons, 7, 48, 7);
    func_0202cb4c(work->buttons, 8, 32, 13);
    func_0202cb4c(work->buttons, 9, 48, 13);
    func_0202cb4c(work->buttons, 11, 32, 19);
    func_0202cb4c(work->buttons, 12, 37, 19);
}

// pokebw2's BBagAnm_DeleteButtons
void func_ov197_0220bd14(BBagWork *work) {
    func_0202ca0c(work->buttons);
}

// Shows the buttons of a page, each in its palette row: 1 or 4 normal, 3 or 6 disabled
// pokebw2's BBagAnm_PutPageButtons
void func_ov197_0220bd24(BBagWork *work, u8 page) {
    u32 i;

    for (i = 0; i < 14; i++) {
        func_0202cc70(work->buttons, i);
    }

    switch (page) {
    case 0:
        func_0202cb4c(work->buttons, 10, 27, 19);
        func_0202cb4c(work->buttons, 13, 1, 19);
        if (work->lastItem == 0) {
            func_0202ce50(work->buttons, 13, 0, 0, 25, 5, 3);
        } else {
            func_0202ce50(work->buttons, 13, 0, 0, 25, 5, 1);
        }
        func_0202cc58(work->buttons, 0);
        func_0202cc58(work->buttons, 1);
        func_0202cc58(work->buttons, 2);
        func_0202cc58(work->buttons, 3);
        func_0202cc58(work->buttons, 10);
        func_0202cc58(work->buttons, 13);
        break;
    case 1:
        func_0202cb4c(work->buttons, 10, 59, 19);
        for (i = 0; i < 6; i++) {
            if (func_ov197_0220bb98(work, i) == 0) {
                func_0202ce50(work->buttons, i + 4, 0, 0, data_ov197_0220db44[i + 4].width,
                                      data_ov197_0220db44[i + 4].height, 6);
            } else {
                func_0202ce50(work->buttons, i + 4, 0, 0, data_ov197_0220db44[i + 4].width,
                                      data_ov197_0220db44[i + 4].height, 4);
            }
        }
        if (work->lastPage[work->pocket] == 0) {
            func_0202ce50(work->buttons, 11, 0, 0, 5, 5, 3);
            func_0202ce50(work->buttons, 12, 0, 0, 5, 5, 3);
        } else {
            func_0202ce50(work->buttons, 11, 0, 0, 5, 5, 1);
            func_0202ce50(work->buttons, 12, 0, 0, 5, 5, 1);
        }
        func_0202cc58(work->buttons, 4);
        func_0202cc58(work->buttons, 5);
        func_0202cc58(work->buttons, 6);
        func_0202cc58(work->buttons, 7);
        func_0202cc58(work->buttons, 8);
        func_0202cc58(work->buttons, 9);
        func_0202cc58(work->buttons, 11);
        func_0202cc58(work->buttons, 12);
        func_0202cc58(work->buttons, 10);
        break;
    case 2:
        func_0202ce50(work->buttons, 13, 0, 0, 25, 5, 1);
        func_0202cb4c(work->buttons, 10, 27, 51);
        func_0202cb4c(work->buttons, 13, 1, 51);
        func_0202cc58(work->buttons, 10);
        func_0202cc58(work->buttons, 13);
        break;
    }
}

// pokebw2's BBagAnm_StartButtonAnm
void func_ov197_0220bf20(BBagWork *work, u8 button) {
    work->animSeq = 0;
    work->animCount = 0;
    work->animButton = button;
    work->animActive = TRUE;
}

// Draws the pressed button in the pressed palette row (2, or 5 for the pockets and items) for 5 frames, then in the
// normal one for 2
// pokebw2's BBagAnm_MainButtonAnm
void func_ov197_0220bf40(BBagWork *work) {
    u8 palette;

    if (work->animActive == FALSE) {
        return;
    }

    switch (work->animSeq) {
    case 0:
        if (work->animButton >= 10) {
            palette = 2;
        } else {
            palette = 5;
        }
        func_0202ce50(work->buttons, work->animButton, 0, 0, data_ov197_0220db44[work->animButton].width,
                              data_ov197_0220db44[work->animButton].height, palette);
        func_0202cc58(work->buttons, work->animButton);
        work->animSeq++;
        break;
    case 1:
        if (work->animCount == 4) {
            if (work->animButton >= 10) {
                palette = 1;
            } else {
                palette = 4;
            }
            func_0202ce50(work->buttons, work->animButton, 0, 0, data_ov197_0220db44[work->animButton].width,
                                  data_ov197_0220db44[work->animButton].height, palette);
            func_0202cc58(work->buttons, work->animButton);
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
