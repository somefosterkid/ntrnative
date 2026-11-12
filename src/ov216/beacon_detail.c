// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov308/beacon_detail.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/beacon_detail.h"
#include "app/beacon_detail_gra.h"
#include "app/beacon_detail_local.h"
#include "app/townmap.h"
#include "app/townmap/townmap_data.h"
#include "app/ui/touchbar.h"
#include "app/ui/ui_scene.h"
#include "constants/arc.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/fade.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcbl.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nnsys/g2d.h"
#include "pml/poke_graphic.h"
#include "system/game_beacon.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/pms_draw.h"
#include "system/printsys.h"
#include "system/wordset.h"

// The screen of a beacon's details: the application that the C-Gear's log of passersby opens for one of its
// beacons. The upper screen shows the sender's profile, the touch screen where the sender is on the map and what the
// sender was doing, and the touch bar turns to the log's other beacons. The ROM has no name for this file;
// "beacon_detail.c" is a guess from its graphic file's, "beacon_detail_gra.c"

// The screen's graphics, the town map's and the trainers that the Union Room shows players as
#define ARCID_BEACON_DETAIL 173
#define ARCID_TOWNMAP_GRA 85
#define ARCID_UNION_VIEW 32

// How many beacons the log's list shows at once
#define LOG_LIST_ROWS 4

#define TRAINER_PALETTE 8

// The files of a set of cell actor graphics, and where they go
typedef struct {
    u8 vramType;
    u8 paletteOffset;
    u8 paletteCount;
    u16 paletteFile;
    u16 charFile;
    // The animations follow it
    u16 cellFile;
} BeaconDetailObjResFiles;

static BOOL func_ov216_021bbd80(GameProc *proc, u32 *state, void *param, void *data);
BOOL func_ov216_021bbe18(GameProc *proc, u32 *state, void *param, void *data);
static BOOL func_ov216_021bbe7c(GameProc *proc, u32 *state, void *param, void *data);
static u32 func_ov216_021bbf0c(BeaconDetailWork *work);
static u32 func_ov216_021bbf14(BeaconDetailWork *work);
static u32 func_ov216_021bbf28(BeaconDetailWork *work);
static u32 func_ov216_021bbf60(BeaconDetailWork *work);
static u32 func_ov216_021bbf98(BeaconDetailWork *work);
static void func_ov216_021bbfec(BeaconDetailWork *work);
void func_ov216_021bc0b4(BeaconDetailWork *work);
void func_ov216_021bc0c8(BeaconDetailWork *work);
void func_ov216_021bc178(BeaconDetailWork *work);
void func_ov216_021bc1e4(BeaconDetailWork *work, HeapID heapId);
void func_ov216_021bc378(BeaconDetailWork *work);
TouchBar *func_ov216_021bc398(ClActUnit *unit, HeapID heapId);
void func_ov216_021bc3d4(TouchBar *touchBar);
static void func_ov216_021bc3dc(TouchBar *touchBar);
void func_ov216_021bc3e4(BeaconDetailWork *work, BeaconDetailObjRes *res,
                                    const BeaconDetailObjResFiles *files, ArcTool *arc);
void func_ov216_021bc43c(BeaconDetailWork *work, BeaconDetailObjRes *res);
void func_ov216_021bc454(BeaconDetailWork *work, ArcTool *arc);
void func_ov216_021bc56c(BeaconDetailWork *work);
void func_ov216_021bc5c4(BeaconDetailWindow *window, u8 bg, u8 palette, u8 x, u8 y, u8 width, u8 height);
ClActor *func_ov216_021bc614(BeaconDetailWork *work, ClActUnit *unit, BeaconDetailObjRes *res, s16 x, s16 y,
                                      u8 anim, u8 priority, u8 bgPriority, u16 surface);
void func_ov216_021bc650(BeaconDetailWork *work);
void func_ov216_021bc6b4(BeaconDetailWork *work);
void func_ov216_021bc6d8(BeaconDetailWork *work);
void func_ov216_021bc6fc(BeaconDetailWork *work);
void func_ov216_021bc70c(BeaconDetailWork *work);
void func_ov216_021bc8a8(BeaconDetailWork *work);
static void func_ov216_021bc920(BeaconDetailWork *work);
void *func_ov010_02165b10(HeapID heapId);
void *func_02012a54(GameData *gameData);
BOOL func_ov010_02165b5c(const GameBeacon *beacon);
s32 func_02012ffc(void);

const GameProcFunctions data_ov216_021bdb40 = {
    func_ov216_021bbd80,
    func_ov216_021bbe7c,
    func_ov216_021bbe18,
};

// The widths of the windows of a panel's name, job, hobby and play time
static const u8 sBeaconDetailWindowWidths[] = { 19, 19, 19, 11 };

static const TouchBarItem data_ov216_021bdb4c[] = {
    { BEACON_DETAIL_ICON_RETURN, { 232, 168 } },
    { BEACON_DETAIL_ICON_NEXT, { 200, 168 } },
    { BEACON_DETAIL_ICON_PREV, { 168, 168 } },
};

// pokebw2's BeaconDetail_Main


static BOOL func_ov216_021bbd80(GameProc *proc, u32 *state, void *param, void *data) {
    BeaconDetailWork *work;

    GFL_OvlLoad(OVERLAY_ID(67));
    GFL_OvlLoad(OVERLAY_ID(72));
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_BEACON_DETAIL, 0x30000);
    work = GFL_ProcInitSubsystem(proc, sizeof(BeaconDetailWork), HEAPID_BEACON_DETAIL);
    MI_CpuFill8(work, 0, sizeof(BeaconDetailWork));
    work->param = param;
    work->heapId = HEAPID_BEACON_DETAIL;
    work->tailHeapId = HEAPID_TAIL(HEAPID_BEACON_DETAIL);
    work->graphic = func_ov216_021bc930(0, work->heapId);
    func_ov216_021bbfec(work);
    func_ov216_021bc0c8(work);
    func_ov216_021bc1e4(work, work->heapId);
    func_ov216_021bc6d8(work);
    func_ov216_021bc454(work, work->arc);
    func_ov216_021bc650(work);
    func_ov216_021bc70c(work);
    work->touchBar = func_ov216_021bc398(func_ov216_021bca38(work->graphic), work->heapId);
    return TRUE;
}

BOOL func_ov216_021bbe18(GameProc *proc, u32 *state, void *param, void *data) {
    BeaconDetailWork *work = data;

    func_ov216_021bc3d4(work->touchBar);
    func_ov216_021bc8a8(work);
    func_ov216_021bc6b4(work);
    func_ov216_021bc56c(work);
    func_ov216_021bc6fc(work);
    func_ov216_021bc378(work);
    func_ov216_021bc178(work);
    func_ov216_021bc0b4(work);
    func_ov216_021bc9d0(work->graphic);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_BEACON_DETAIL);
    GFL_OvlUnload(OVERLAY_ID(72));
    GFL_OvlUnload(OVERLAY_ID(67));
    return TRUE;
}

static BOOL func_ov216_021bbe7c(GameProc *proc, u32 *state, void *param, void *data) {
    BeaconDetailWork *work = data;

    if (work->empty) {
        return TRUE;
    }
    if ((int)*state >= BEACON_DETAIL_SEQ_INPUT && (int)*state <= BEACON_DETAIL_SEQ_WAIT) {
        func_ov216_021bc3dc(work->touchBar);
    }
    switch (*state) {
    case BEACON_DETAIL_SEQ_INIT:
        *state = func_ov216_021bbf28(work);
        break;
    case BEACON_DETAIL_SEQ_FADE_IN:
        *state = func_ov216_021bbf60(work);
        break;
    case BEACON_DETAIL_SEQ_INPUT:
        *state = func_ov216_021bbf0c(work);
        break;
    case BEACON_DETAIL_SEQ_WAIT:
        *state = func_ov216_021bbf14(work);
        break;
    case BEACON_DETAIL_SEQ_FADE_OUT:
        *state = func_ov216_021bbf98(work);
        break;
    case BEACON_DETAIL_SEQ_END:
        return TRUE;
    }
    GFL_TCBExMgrUpdate(work->tcbManager);
    func_0201c4e4(work->printQueue);
    func_ov216_021bc920(work);
    func_ov216_021bca24(work->graphic);
    return FALSE;
}

// pokebw2's BeaconDetail_SeqInput
static u32 func_ov216_021bbf0c(BeaconDetailWork *work) {
    return func_ov216_021bcc10(work);
}

// pokebw2's BeaconDetail_SeqWait
static u32 func_ov216_021bbf14(BeaconDetailWork *work) {
    if (work->taskCount) {
        return BEACON_DETAIL_SEQ_WAIT;
    }
    func_ov216_021bccbc(work);
    return BEACON_DETAIL_SEQ_INPUT;
}

// pokebw2's BeaconDetail_SeqInit
static u32 func_ov216_021bbf28(BeaconDetailWork *work) {
    switch (work->seq) {
    case 0:
        func_ov216_021bcba4(work);
        work->seq++;
        break;
    case 1:
        if (func_0201c6b4(work->printQueue)) {
            work->seq = 0;
            return BEACON_DETAIL_SEQ_FADE_IN;
        }
        break;
    }
    return BEACON_DETAIL_SEQ_INIT;
}

// pokebw2's BeaconDetail_SeqFadeIn
static u32 func_ov216_021bbf60(BeaconDetailWork *work) {
    switch (work->seq) {
    case 0:
        GFL_FadeSet(3, 16, 0, 0);
        work->seq++;
        break;
    case 1:
        if (!GFL_FadeIsRunning()) {
            work->seq = 0;
            return BEACON_DETAIL_SEQ_INPUT;
        }
        break;
    }
    return BEACON_DETAIL_SEQ_FADE_IN;
}

// pokebw2's BeaconDetail_SeqFadeOut
static u32 func_ov216_021bbf98(BeaconDetailWork *work) {
    switch (work->seq) {
    case 0:
        // The return button's animation ends first, unless the game itself ends the screen
        if (!work->param->unk09) {
            func_ov216_021bc3dc(work->touchBar);
            if (func_ov067_021b6448(work->touchBar) != BEACON_DETAIL_ICON_RETURN) {
                break;
            }
        }
        GFL_FadeSet(3, 0, 16, 0);
        work->seq++;
        break;
    case 1:
        if (!GFL_FadeIsRunning()) {
            work->seq = 0;
            return BEACON_DETAIL_SEQ_END;
        }
        break;
    }
    return BEACON_DETAIL_SEQ_FADE_OUT;
}

static void func_ov216_021bbfec(BeaconDetailWork *work) {
    int i;
    int count;

    work->beacon = func_ov010_02165b10(work->heapId);
    work->beaconStatus = func_02012a54(work->param->gameData);
    work->infoTable = func_020275a8(work->beaconStatus);
    count = func_ov010_02165b04(work->infoTable);
    work->index = 0xff;
    for (i = 0; i < count; i++) {
        func_ov010_02165ac4(work->infoTable, work->beacon, &work->time, i);
        if (func_ov010_02165b5c(work->beacon) || func_ov010_02165c10(work->beacon)) {
            continue;
        }
        if (work->param->subscreen == i) {
            work->index = work->count;
        }
        work->logIndices[work->count++] = i;
    }
    if (work->index == 0xff || work->index >= work->count) {
        work->index = 0;
    }
    if (work->count == 0) {
        work->empty = TRUE;
    }
    work->unk_4f = func_02012ffc();
    work->townMapData = func_ov072_021bbb24(work->heapId);
}


void func_ov216_021bc0b4(BeaconDetailWork *work) {
    func_ov072_021bbb34(work->townMapData);
    GFL_HeapFree(work->beacon);
}

void func_ov216_021bc0c8(BeaconDetailWork *work) {
    work->tcbManager = GFL_TCBExMgrCreate(work->heapId, work->heapId, 64, 128);
    work->arc = GFL_ArcSysCreateFileHandle(ARCID_BEACON_DETAIL, work->heapId);
    GFL_TextRndUpdateColorIndexLUT(7, 8, 15);
    work->printQueue = func_0201c450(0x800, work->heapId);
    work->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, work->heapId);
    work->wordSet = GFL_WordSetSystemCreate(8, 64, work->heapId);
    work->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_STARTMENU, work->heapId);
    work->countryMsgData =
        GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_EVENT_MAPCHANGE_LOAD_COUNTRY_TO_STRBUF, work->heapId);
    work->formatBuf = GFL_StrBufCreate(109, work->heapId);
    work->textBuf = GFL_StrBufCreate(109, work->heapId);
    work->unk144 = GFL_StrBufCreate(109, work->heapId);
}



// pokebw2's BeaconDetail_FreeSystems
void func_ov216_021bc178(BeaconDetailWork *work) {
    GFL_StrBufFree(work->unk144);
    GFL_StrBufFree(work->textBuf);
    GFL_StrBufFree(work->formatBuf);
    GFL_MsgDataFree(work->countryMsgData);
    GFL_MsgDataFree(work->msgData);
    GFL_WordSetSystemFree(work->wordSet);
    GFL_FontFree(work->font);
    func_0201c6ec(work->printQueue);
    func_0201c4c0(work->printQueue);
    func_0201dd68();
    GFL_TCBExMgrFree(work->tcbManager);
    GFL_ArcToolFree(work->arc);
}

// The profile's panels on the upper screen's BGs, and the town map with the frame of the info window on the touch
// screen's
// pokebw2's BeaconDetail_LoadBGs
void func_ov216_021bc1e4(BeaconDetailWork *work, HeapID heapId) {
    ArcTool *townMapArc = GFL_ArcSysCreateFileHandle(ARCID_TOWNMAP_GRA, heapId);

    GFL_G2DIOLoadArcNCLRDefault(townMapArc, 0, 0, 0, 0x200, heapId);
    GFL_G2DIOLoadArcNCLRDefault(work->arc, 28, 4, 0, 0x200, heapId);
    GFL_BGSysLoadArcNCGRStatic(work->arc, 30, 4, 0, 0, TRUE, heapId);
    GFL_BGSysLoadArcNCGRStatic(work->arc, 30, 5, 0, 0, TRUE, heapId);
    GFL_BGSysLoadArcNCGRStatic(work->arc, 30, 7, 0, 0, TRUE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(work->arc, 32, 4, 0, 0, TRUE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(work->arc, 32, 5, 0, 0, TRUE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(work->arc, 33, 7, 0, 0, TRUE, heapId);
    GFL_BGSysSetScrPaletteNo(5, 0, 0, 32, 24, 1);
    GFL_BGSysLoadScr(5);
    GFL_BGSysLoadArcNCGRStatic(work->arc, 31, 1, 0, 0, TRUE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(work->arc, 34, 1, 0, 0x800, TRUE, heapId);
    GFL_BGSysLoadArcNCGRStatic(townMapArc, 5, 2, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(townMapArc, 17, 2, 0, 0x800, FALSE, heapId);
    GFL_BGSysLoadArcNCGRStatic(townMapArc, 7, 3, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(townMapArc, 16, 3, 0, 0x800, FALSE, heapId);
    work->versionPalette.file = GFL_ArcToolReadHeapNew(work->arc, 29, work->heapId);
    NNS_G2dGetUnpackedPaletteData(work->versionPalette.file, &work->versionPalette.data);
    work->versionPalette.raw = work->versionPalette.data->pRawData;
    GFL_ArcToolFree(townMapArc);
    G2_SetBlendAlpha(GX_PLANEMASK_BG1, GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ, 11,
                     5);
    G2S_SetBlendAlpha(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1, GX_PLANEMASK_ALL, 13, 3);
}

// pokebw2's BeaconDetail_FreeBGs
void func_ov216_021bc378(BeaconDetailWork *work) {
    GFL_HeapFree(work->versionPalette.file);
    G2S_BlendNone();
    G2_BlendNone();
}

TouchBar *func_ov216_021bc398(ClActUnit *unit, HeapID heapId) {
    TouchBarSetup setup;

    MI_CpuFill8(&setup, 0, sizeof(TouchBarSetup));
    setup.items = data_ov216_021bdb4c;
    setup.count = NELEMS(data_ov216_021bdb4c);
    setup.unit = unit;
    setup.bg = 0;
    setup.bgPalette = 13;
    setup.objPalette = 11;
    setup.vramType = CLACT_VRAM_MAIN;
    return func_ov067_021b62e0(&setup, heapId);
}


// pokebw2's BeaconDetail_FreeTouchBar
void func_ov216_021bc3d4(TouchBar *touchBar) {
    func_ov067_021b639c(touchBar);
}

// pokebw2's BeaconDetail_UpdateTouchBar
static void func_ov216_021bc3dc(TouchBar *touchBar) {
    func_ov067_021b63d0(touchBar);
}

void func_ov216_021bc3e4(BeaconDetailWork *work, BeaconDetailObjRes *res,
                                    const BeaconDetailObjResFiles *files, ArcTool *arc) {
    MI_CpuFill8(res, 0, sizeof(BeaconDetailObjRes));
    res->palette = func_0204ac18(arc, files->paletteFile, files->vramType, files->paletteOffset * 0x20, 0,
                                 files->paletteCount, work->heapId);
    res->chars = func_0204a6c8(arc, files->charFile, FALSE, files->vramType, work->heapId);
    res->cellAnims = func_0204af28(arc, files->cellFile, files->cellFile + 1, work->heapId);
}

// pokebw2's BeaconDetail_FreeObjRes
void func_ov216_021bc43c(BeaconDetailWork *work, BeaconDetailObjRes *res) {
    func_0204afd8(res->cellAnims);
    func_0204a8d4(res->chars);
    func_0204ada4(res->palette);
}

// The icons, the map's trainer with every view's characters and the palettes of the views, and a trainer picture
// for each panel
// pokebw2's BeaconDetail_LoadObjs
void func_ov216_021bc454(BeaconDetailWork *work, ArcTool *arc) {
    ArcTool *unionArc;
    int i;

    work->trainerArc = func_0202da04(work->heapId);
    {
        BeaconDetailObjResFiles files = { CLACT_VRAM_BOTH, 0, 3, 24, 25, 26 };

        func_ov216_021bc3e4(work, &work->iconRes, &files, arc);
    }
    {
        BeaconDetailObjResFiles files = { CLACT_VRAM_MAIN, 10, 1, 0, 49, 65 };

        unionArc = GFL_ArcSysCreateFileHandle(ARCID_UNION_VIEW, work->tailHeapId);
        func_ov216_021bc3e4(work, &work->unionRes, &files, unionArc);
    }
    work->unionPalette.file = GFL_ArcToolReadHeapNew(unionArc, 0, work->heapId);
    NNS_G2dGetUnpackedPaletteData(work->unionPalette.file, &work->unionPalette.data);
    work->unionPalette.raw = work->unionPalette.data->pRawData;
    for (i = 0; i < BEACON_DETAIL_UNION_VIEW_COUNT; i++) {
        work->unionChars[i].file = GFL_ArcToolReadHeapNew(unionArc, i + 49, work->heapId);
        NNS_G2dGetUnpackedCharacterData(work->unionChars[i].file, &work->unionChars[i].data);
    }
    GFL_ArcToolFree(unionArc);
    for (i = 0; i < BEACON_DETAIL_PANEL_COUNT; i++) {
        work->trainerRes[i].chars = func_0202da48(work->trainerArc, i + 2, CLACT_VRAM_SUB, work->heapId);
        work->trainerRes[i].palette =
            func_0202da18(work->trainerArc, i + 2, CLACT_VRAM_SUB, (i + TRAINER_PALETTE) * 0x20, work->heapId);
        work->trainerRes[i].cellAnims = func_0202da6c(i + 2, 0, CLACT_VRAM_SUB, work->heapId);
    }
}

// pokebw2's BeaconDetail_FreeObjs
void func_ov216_021bc56c(BeaconDetailWork *work) {
    int i;

    for (i = 0; i < BEACON_DETAIL_PANEL_COUNT; i++) {
        func_ov216_021bc43c(work, &work->trainerRes[i]);
    }
    for (i = 0; i < BEACON_DETAIL_UNION_VIEW_COUNT; i++) {
        GFL_HeapFree(work->unionChars[i].file);
    }
    GFL_HeapFree(work->unionPalette.file);
    func_ov216_021bc43c(work, &work->unionRes);
    func_ov216_021bc43c(work, &work->iconRes);
    GFL_ArcToolFree(work->trainerArc);
}

// pokebw2's BeaconDetail_InitWindow
void func_ov216_021bc5c4(BeaconDetailWindow *window, u8 bg, u8 palette, u8 x, u8 y, u8 width, u8 height) {
    window->window = BmpWin_CreateDynamic(bg, x, y, width, height, palette, TRUE);
    window->bitmap = BmpWin_GetBitmap(window->window);
    PrintWindow_Init(&window->print, window->window);
    GFL_BitmapFill(window->bitmap, 3);
    BmpWin_TransferNow(window->window);
}

ClActor *func_ov216_021bc614(BeaconDetailWork *work, ClActUnit *unit, BeaconDetailObjRes *res, s16 x, s16 y,
                                      u8 anim, u8 priority, u8 bgPriority, u16 surface) {
    ClActorSetup setup;

    setup.x = x;
    setup.y = y;
    setup.sequence = anim;
    setup.bgPriority = bgPriority;
    setup.priority = priority;
    return func_0204b294(unit, res->chars, res->palette, res->cellAnims, &setup, surface, work->heapId);
}

// pokebw2's BeaconDetail_CreateMapActors
void func_ov216_021bc650(BeaconDetailWork *work) {
    ClActUnit *unit = func_ov216_021bca38(work->graphic);

    work->actors[BEACON_DETAIL_ACTOR_TRAINER] =
        func_ov216_021bc614(work, unit, &work->unionRes, 128, 120, 0, 0, 2, CLACT_SURFACE_MAIN);
    func_0204b6f4(work->actors[BEACON_DETAIL_ACTOR_TRAINER], 0, 1);
    work->actors[BEACON_DETAIL_ACTOR_RECENT] =
        func_ov216_021bc614(work, unit, &work->iconRes, 96, 120, 0, 1, 2, CLACT_SURFACE_MAIN);
}

// pokebw2's BeaconDetail_FreeMapActors
void func_ov216_021bc6b4(BeaconDetailWork *work) {
    int i;

    for (i = 0; i < BEACON_DETAIL_ACTOR_COUNT; i++) {
        func_0204b3dc(work->actors[i], FALSE);
        func_0204b3b4(work->actors[i]);
    }
}

// pokebw2's BeaconDetail_CreateInfoWindow
void func_ov216_021bc6d8(BeaconDetailWork *work) {
    func_ov216_021bc5c4(&work->infoWindow, 1, 11, 2, 22, 28, 6);
}

// pokebw2's BeaconDetail_FreeInfoWindow
void func_ov216_021bc6fc(BeaconDetailWork *work) {
    BmpWin_Free(work->infoWindow.window);
}

void func_ov216_021bc70c(BeaconDetailWork *work) {
    ClActUnit *unit = func_ov216_021bca38(work->graphic);
    int i;
    int j;

    work->pmsDraw = func_0202db0c(unit, CLACT_VRAM_SUB, work->printQueue, work->font, 10, BEACON_DETAIL_PANEL_COUNT,
        work->heapId);
    func_0202de5c(work->pmsDraw, 4);
    func_0202de64(work->pmsDraw, PRINT_COLOR(15, 14, 4));
    func_0202de68(work->pmsDraw, TRUE);
    for (i = 0; i < BEACON_DETAIL_PANEL_COUNT; i++) {
        int bg = BGSYS_BG_SUB + i;
        BeaconDetailPanel *panel = &work->panels[i];

        panel->bg = bg;
        panel->recordWindow = BmpWin_CreateDynamic(bg, 2, 15, 28, 4, i, TRUE);
        for (j = 0; j < 4; j++) {
            func_ov216_021bc5c4(&panel->windows[j], bg, i, 2, j * 2 + 1, 19, 2);
        }
        func_ov216_021bc5c4(&panel->windows[BEACON_DETAIL_WINDOW_FROM], bg, i, 2, 9, 19, 2);
        func_ov216_021bc5c4(&panel->windows[BEACON_DETAIL_WINDOW_HOME], bg, i, 3, 11, 28, 4);
        func_ov216_021bc5c4(&panel->windows[BEACON_DETAIL_WINDOW_COUNTS], bg, i, 4, 19, 27, 4);
        panel->trainerActor =
            func_ov216_021bc614(work, unit, &work->trainerRes[i], 220, 44, 0, 1, i, CLACT_SURFACE_SUB);
        func_0204b688(panel->trainerActor, GX_OAM_MODE_NORMAL);
        func_0204b3dc(panel->trainerActor, FALSE);
        panel->rankActor = func_ov216_021bc614(work, unit, &work->iconRes, 200, 80, 6, 0, i, CLACT_SURFACE_SUB);
        func_0204b688(panel->rankActor, GX_OAM_MODE_XLU);
        func_0204b3dc(panel->rankActor, FALSE);
    }
}


// pokebw2's BeaconDetail_FreePanels
void func_ov216_021bc8a8(BeaconDetailWork *work) {
    int i;
    int j;

    for (i = 0; i < BEACON_DETAIL_PANEL_COUNT; i++) {
        BeaconDetailPanel *panel = &work->panels[i];

        func_0204b3dc(panel->trainerActor, FALSE);
        func_0204b3b4(panel->trainerActor);
        func_0204b3dc(panel->rankActor, FALSE);
        func_0204b3b4(panel->rankActor);
        BmpWin_Free(panel->windows[BEACON_DETAIL_WINDOW_COUNTS].window);
        for (j = 0; j < 2; j++) {
            BmpWin_Free(panel->windows[BEACON_DETAIL_WINDOW_FROM + j].window);
        }
        for (j = 0; j < 4; j++) {
            BmpWin_Free(panel->windows[j].window);
        }
        BmpWin_Free(panel->recordWindow);
    }
    func_0202dc08(work->pmsDraw);
}

// pokebw2's BeaconDetail_UpdatePMSDraw
static void func_ov216_021bc920(BeaconDetailWork *work) {
    func_0202dbc4(work->pmsDraw);
}
