// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov214/worldtrade_sublcd.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/sound.h"
#include "gfl/arc_util.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/sound.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "nitro/gx.h"
#include "nitro/os.h"
#include "worldtrade_local.h"
#include "dpw/dpw_tr.h"

// The Global Trade Station's lower screen, the trade room: the player, who walks in and out, and the people that
// stand for the trainers a search found. The names are ours, guessed

// The player's walk into or out of the trade room
typedef struct {
    int seq;
    int y;
    WorldTradeWork *wk;
} SubLcdHeroDemo;

void func_ov135_021ff914(SubLcdHeroDemo *demo, int seq);
void func_ov135_021ff92c(TCB *tcb, void *data);
void func_ov135_021ffa3c(TCB *tcb, void *data);
static void func_ov135_021ffaec(ClActor *act, int x, int y);
int func_ov135_021ffbc0(int index);
void func_ov135_021ffc18(WorldTradeWork *wk);
void func_ov135_021ffc8c(NNSG2dCharacterData **charData, NNSG2dPaletteData *pltt, int index, int trainerType,
                                 int gender);

// Where the characters of each person go in OBJ VRAM
// pokebw2's sFieldObjCharOffsets
static const u16 data_ov135_02203884[7] = { 0x200, 0x400, 0x600, 0x800, 0xa00, 0xc00, 0xe00 };

// The palette of each trainer type's person
// pokebw2's sFieldObjPalettes
static const u8 data_ov135_02203892[16] = { 2, 3, 6, 5, 4, 5, 2, 0, 1, 3, 6, 5, 4, 7, 7, 0 };

// Where the people stand
static const struct {
    u16 x;
    u16 y;
// pokebw2's sSubLcdObjPos
} data_ov135_022038a2[7] = {
    { 128, 54 }, { 96, 55 }, { 160, 55 }, { 64, 70 }, { 192, 70 }, { 48, 102 }, { 208, 102 },
};

// pokebw2's sSubLcdObjTouchRects
static const TouchRect data_ov135_022038be[] = {
    { 70, 102, 112, 144 },  { 71, 103, 80, 112 },  { 71, 103, 144, 176 },   { 86, 118, 48, 80 },
    { 86, 118, 176, 208 }, { 118, 150, 32, 64 }, { 118, 150, 192, 224 }, { TOUCH_RECT_END, 0, 0, 0 },
};

// pokebw2's SubLcd_ActPos

// Corrected WorldTradeWork: fields carved out of unkC8 at the offsets this file reads.

void func_ov135_021ff6a4(WorldTradeWork *wk);

void func_ov135_021ff6a4(WorldTradeWork *wk) {
    ClActorSetup setup;
    ClActorPos pos;
    int i;

    func_ov135_021ffc18(wk);

    setup.x = 128;
    setup.y = 130;
    setup.sequence = 0;
    setup.priority = 0;
    setup.bgPriority = 0;
    wk->subAct[0] = func_0204b294(wk->clactUnit, wk->clactRes[WT_CLACT_RES_HERO][WT_CLACT_RES_CHAR],
                                  wk->clactRes[WT_CLACT_RES_HERO][WT_CLACT_RES_PLTT],
                                  wk->clactRes[WT_CLACT_RES_HERO][WT_CLACT_RES_CELL], &setup, 1, HEAPID_WORLDTRADE);
    func_0204b92c(wk->subAct[0], TRUE);
    func_0204b84c(wk->subAct[0], 2);
    func_0204b878(wk->subAct[0], 3);
    func_0204b3dc(wk->subAct[0], TRUE);

    for (i = 0; i < 7; i++) {
        wk->subAct[1 + i] =
            func_0204b294(wk->clactUnit, wk->clactRes[WT_CLACT_RES_SUB][WT_CLACT_RES_CHAR],
                          wk->clactRes[WT_CLACT_RES_SUB][WT_CLACT_RES_PLTT],
                          wk->clactRes[WT_CLACT_RES_SUB][WT_CLACT_RES_CELL], &setup, 1, HEAPID_WORLDTRADE);
        func_0204b92c(wk->subAct[1 + i], TRUE);
        func_0204b878(wk->subAct[1 + i], i * 4 + 14);
        func_0204b3dc(wk->subAct[1 + i], FALSE);
        func_ov135_021ffaec(wk->subAct[1 + i], data_ov135_022038a2[i].x, data_ov135_022038a2[i].y);
        func_0204b84c(wk->subAct[1 + i], 2);
        func_0204b6f4(wk->subAct[1 + i], (u8)(i + 2), 1);
    }

    wk->partnerCursorAct =
        func_0204b294(wk->clactUnit, wk->clactRes[WT_CLACT_RES_SUB][WT_CLACT_RES_CHAR],
                      wk->clactRes[WT_CLACT_RES_SUB][WT_CLACT_RES_PLTT],
                      wk->clactRes[WT_CLACT_RES_SUB][WT_CLACT_RES_CELL], &setup, 1, HEAPID_WORLDTRADE);
    func_0204b92c(wk->partnerCursorAct, TRUE);
    func_0204b878(wk->partnerCursorAct, 43);
    func_0204b3dc(wk->partnerCursorAct, FALSE);
    func_ov135_021ffaec(wk->partnerCursorAct, 128, 86);
    func_0204b84c(wk->partnerCursorAct, 1);

    wk->promptDsAct = func_0204b294(wk->clactUnit, wk->clactRes[WT_CLACT_RES_SUB][WT_CLACT_RES_CHAR],
                                    wk->clactRes[WT_CLACT_RES_SUB][WT_CLACT_RES_PLTT],
                                    wk->clactRes[WT_CLACT_RES_SUB][WT_CLACT_RES_CELL], &setup, 1, HEAPID_WORLDTRADE);
    func_0204b92c(wk->promptDsAct, TRUE);
    func_0204b878(wk->promptDsAct, 42);
    func_ov135_021f4ea0(wk->promptDsAct, 55, wk->drawOffset + 168);
    func_0204b3dc(wk->promptDsAct, FALSE);

    for (i = 0; i < 8; i++) {
        func_0204b454(wk->subAct[i], &pos, 1);
        wk->subActPos[i].x = pos.x;
        wk->subActPos[i].y = pos.y;
    }
}

void func_ov135_021ff8b0(WorldTradeWork *wk) {
    SubLcdHeroDemo *demo;

    func_ov135_021ff6a4(wk);
    wk->heroDemoWork = GFL_HeapAllocate(0x48, sizeof(SubLcdHeroDemo), FALSE, "worldtrade_sublcd.c", 212);
    wk->demoTask = GFL_TCBMgrAddTask(wk->tcbManager, func_ov135_021ff92c, wk->heroDemoWork, 5);
    demo = GFL_TCBGetData(wk->demoTask);
    demo->seq = 0;
    demo->y = -40;
    demo->wk = wk;
    wk->demoEnd = 0;
    func_ov135_021ff914(demo, 0);
    GFL_SndSEPlay(0x790);
}

void func_ov135_021ff914(SubLcdHeroDemo *demo, int seq) {
    func_0204b878(demo->wk->subAct[0], (u16)seq);
}

void func_ov135_021ff92c(TCB *tcb, void *data) {
    SubLcdHeroDemo *demo = data;
    WorldTradeWork *wk = demo->wk;

    switch (demo->seq) {
    case 0:
        if (demo->y > 160) {
            demo->y = 160;
            demo->seq = 1;
            func_ov135_021ff914(demo, 1);
        }
        demo->y += 2;
        func_ov135_021ffaec(wk->subAct[0], 128, demo->y);
        break;
    case 1:
        if (!func_0204b98c(wk->subAct[0])) {
            func_ov135_021ff914(demo, 2);
            demo->seq = 2;
        }
        break;
    case 2:
        if (demo->y <= 138) {
            demo->y = 138;
            demo->seq = 3;
            func_ov135_021ff914(demo, 3);
        } else {
            demo->y--;
        }
        func_ov135_021ffaec(wk->subAct[0], 128, demo->y);
        break;
    case 3:
        GFL_SndSEPlay(0x55c);
        demo->wk->demoEnd = 1;
        GFL_HeapFree(demo);
        GFL_TCBRemove(tcb);
        break;
    }
}

void func_ov135_021ff9e0(WorldTradeWork *wk) {
    SubLcdHeroDemo *demo;

    wk->heroDemoWork = GFL_HeapAllocate(0x48, sizeof(SubLcdHeroDemo), FALSE, "worldtrade_sublcd.c", 336);
    wk->demoTask = GFL_TCBMgrAddTask(wk->tcbManager, func_ov135_021ffa3c, wk->heroDemoWork, 5);
    demo = GFL_TCBGetData(wk->demoTask);
    demo->seq = 0;
    demo->y = 138;
    demo->wk = wk;
    func_ov135_021ff914(demo, 5);
    GFL_SndSEPlay(0x55d);
}

void func_ov135_021ffa3c(TCB *tcb, void *data) {
    SubLcdHeroDemo *demo = data;
    WorldTradeWork *wk = demo->wk;

    switch (demo->seq) {
    case 0:
        if (demo->y > 160) {
            demo->y = 160;
            demo->seq = 1;
            func_ov135_021ff914(demo, 6);
        }
        demo->y += 1;
        func_ov135_021ffaec(wk->subAct[0], 128, demo->y);
        break;
    case 1:
        if (!func_0204b98c(wk->subAct[0])) {
            func_ov135_021ff914(demo, 0);
            demo->seq = 2;
            GFL_SndSEPlay(0x791);
        }
        break;
    case 2:
        if (demo->y < -20) {
            demo->seq = 3;
            func_ov135_021ff914(demo, 3);
        }
        demo->y -= 2;
        func_ov135_021ffaec(wk->subAct[0], 128, demo->y);
        break;
    case 3:
        wk->demoEnd = 1;
        GFL_HeapFree(demo);
        GFL_TCBRemove(tcb);
        break;
    }
}





static void func_ov135_021ffaec(ClActor *act, int x, int y) {
    ClActorPos pos;

    pos.x = x;
    pos.y = y - 8;
    func_0204b404(act, &pos, 1);
}

// pokebw2's WorldTrade_SubLcdObjHitCheck
int func_ov135_021ffb00(int count) {
    int ret = func_02035c08(data_ov135_022038be);

    if (ret == TOUCH_RECT_NONE || ret >= count) {
        ret = TOUCH_RECT_NONE;
    }
    return ret;
}

void func_ov135_021ffb20(WorldTradeWork *wk, int count, int appear) {
    int i;

    if (count != 0 && appear == 1) {
        GFL_SndSEPlay(SEQ_SE_FLD_05);
    }
    for (i = 0; i < 7; i++) {
        if (i < count) {
            func_ov135_021ffc8c(wk->fieldObjCharaData, wk->fieldObjPalData, i, wk->downloadData[i].trainerType,
                                wk->downloadData[i].gender);
            if (appear) {
                func_0204b878(wk->subAct[1 + i], i * 4 + 14);
            } else {
                func_0204b878(wk->subAct[1 + i], i * 4 + 17);
            }
            func_0204b3dc(wk->subAct[1 + i], TRUE);
        } else {
            func_0204b3dc(wk->subAct[1 + i], FALSE);
        }
    }
}


// pokebw2's SubLcd_ObjAnimBase
int func_ov135_021ffbc0(int index) {
    return index * 4 + 14;
}

void func_ov135_021ffbc8(WorldTradeWork *wk) {
    int i;

    for (i = 0; i < 7; i++) {
        if (wk->subAct[1 + i] != NULL) {
            if (func_0204b898(wk->subAct[1 + i]) != func_ov135_021ffbc0(i) + 1 && func_0204b3f4(wk->subAct[1 + i])) {
                func_0204b878(wk->subAct[1 + i], func_ov135_021ffbc0(i) + 1);
            }
        }
    }
}

void func_ov135_021ffc18(WorldTradeWork *wk) {
    int i;

    wk->fieldObjPalBuf = GFL_G2DIOReadNCLR(32, 0, &wk->fieldObjPalData, HEAPID_WORLDTRADE);
    for (i = 0; i < 16; i++) {
        wk->fieldObjCharaBuf[i] =
            GFL_G2DIOReadOBJNCGR(32, 0x31 + i, FALSE, &wk->fieldObjCharaData[i], HEAPID_WORLDTRADE);
    }
    for (i = 0; i < 16; i++) {
        DC_FlushRange(wk->fieldObjCharaData[i]->pRawData, 0x200);
    }
}



void func_ov135_021ffc8c(NNSG2dCharacterData **charData, NNSG2dPaletteData *pltt, int index, int trainerType,
                                 int gender) {
    u8 *palette;

    if (trainerType > 16) {
        trainerType = 16;
    } else if (trainerType < 0) {
        trainerType = 0;
    }
    palette = pltt->pRawData;
    GXS_LoadOBJ(charData[trainerType]->pRawData, data_ov135_02203884[index], 0x200);
    GXS_LoadOBJPltt(palette + data_ov135_02203892[trainerType] * 32, (index + 2) * 32, 32);
}

void func_ov135_021ffcd4(WorldTradeWork *wk) {
    int i;

    if (wk->demoEnd) {
        if (wk->fieldObjPalBuf != NULL) {
            GFL_HeapFree(wk->fieldObjPalBuf);
            wk->fieldObjPalBuf = NULL;
        }
        for (i = 0; i < 16; i++) {
            if (wk->fieldObjCharaBuf[i] != NULL) {
                GFL_HeapFree(wk->fieldObjCharaBuf[i]);
                wk->fieldObjCharaBuf[i] = NULL;
            }
        }
    }
}

void func_ov135_021ffd1c(WorldTradeWork *wk, int index, int offsetY) {
    func_ov135_021ffaec(wk->partnerCursorAct, data_ov135_022038a2[index].x, offsetY + (data_ov135_022038a2[index].y + 32));
}

void func_ov135_021ffd44(WorldTradeWork *wk) {
    int i;

    for (i = 0; i < 8; i++) {
        if (wk->subAct[i] != NULL) {
            func_ov135_021f4ea0(wk->subAct[i], wk->subActPos[i].x, wk->subActPos[i].y);
        }
    }
}

void func_ov135_021ffd74(WorldTradeWork *wk) {
    int i;

    for (i = 0; i < 8; i++) {
        if (wk->subAct[i] != NULL) {
            func_ov135_021f4ea0(wk->subAct[i], wk->subActPos[i].x, wk->subActPos[i].y + 32);
        }
    }
}





