// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov084/beacon_view.c and beacon_view_setup.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// Named by the ROM's "beacon_view.c", the only file-name string of the overlay; the file starts at the view's creation
#include "types.h"
#include "constants/sound.h"
#include "field/beacon_view.h"
#include "field/beacon_view_common.h"
#include "field/fest_mission_data.h"
#include "field/field_disp_control.h"
#include "field/game_beacon_set.h"
#include "field/intrude_work.h"
#include "gfl/clact.h"
#include "gfl/net.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "save/high_link.h"
#include "save/trainer_game_info.h"
#include "system/game_beacon.h"
#include "system/game_system.h"
#include "system/palanm.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "text/system/game_beacon.h"
#include "constants/items.h"
#include "constants/version.h"
#include "field/beacon_view_setup.h"
#include "field/festival.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "nitro/mi.h"
#include "save/bag.h"
#include "save/event_work.h"
#include "save/trainer_card.h"
#include "system/aeabi.h"
#include "system/bmp_oam.h"
#include "system/fest_mission.h"
#include "system/game_data.h"
#include "system/scroll_bar.h"
#include "field/field.h"
#include "constants/arc.h"
#include "constants/text_banks.h"
#include "field/event_fest_mission.h"
#include "field/event_phrase_input.h"
#include "field/field_menu.h"
#include "field/field_pass_power.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"
#include "gfl/tcb.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/os.h"
#include "nnsys/g2d.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "save/trainer_data_savedata.h"
#include "system/app_keycursor.h"
#include "system/app_taskmenu.h"
#include "system/game_event.h"
#include "system/gf_font.h"

// The beacon view's work: what it takes from the game, its graphics, windows and actors, and the events that leave
// the view for another screen

// The view's graphics, and the trainers' sprites
#define ARCID_BEACON_VIEW 171
#define ARCID_BEACON_VIEW_TRAINER 32

// What the event waits for before it hands over
#define EVENT_WAIT_BUTTON 0
#define EVENT_WAIT_TASKS 1
#define EVENT_WAIT_MENU_BUTTON 2
#define EVENT_WAIT_MEDAL 3

typedef struct {
    u32 seq;
    GameSystem *gsys;
    Field *field;
    BeaconViewWork *work;
    GameEvent *next;
    u32 wait;
    // The button or actor to wait for, or, for EVENT_WAIT_MENU_BUTTON, the AppTaskMenuWin *
    uintptr_t param;
} BeaconViewEvent;


// The view's sequences
#define SEQ_MAIN 0
#define SEQ_NEW_BEACON 1
#define SEQ_WAIT_TASKS 2
#define SEQ_SCROLL 3
#define SEQ_BAR 4
#define SEQ_THANKS 5

// Black's symbols, not yet declared in any header
GameEventReturnCode func_ov032_021f2588(GameEvent *event, u32 *state, void *data);
BOOL func_ov032_021f25d4(BeaconViewWork *work, BeaconViewEvent *event);
BOOL func_ov032_021f2608(BeaconViewWork *work, BeaconViewEvent *event);
BOOL func_ov032_021f2624(BeaconViewWork *work, BeaconViewEvent *event);
GameEvent *func_ov032_021f254c(BeaconViewWork *work, GameEvent *next, u32 wait, uintptr_t param);
GameEvent *func_ov010_02164b38(GameSystem *, Field *, u8);
void func_ov032_021f2544(BeaconViewWork *work);
int func_ov032_021f2674(BeaconViewWork *work);
int func_ov032_021f26d8(BeaconViewWork *work);
int func_ov032_021f26f0(BeaconViewWork *work);
int func_ov032_021f2714(BeaconViewWork *work);
int func_ov032_021f27b4(BeaconViewWork *work);
int func_ov032_021f27cc(BeaconViewWork *work);

BeaconViewWork *func_ov032_021f2234(GameSystem *gsys, void *parent) {
    BeaconViewWork *work = GFL_HeapAllocate(0x70, sizeof(BeaconViewWork), TRUE, "beacon_view.c", 141);

    work->gsys = gsys;
    work->gameData = GSYS_GetGameData(gsys);
    work->field = GSYS_GetField(gsys);
    work->dispControl = FieldG2D_GetDispControl(work->field);
    work->parent = parent;
    work->heapId = 0x70;
    work->resHeapId = 0x15;
    work->tmpHeapId = HEAPID_TAIL(0x70);
    work->arc = GFL_ArcSysCreateFileHandle(0xad, work->heapId);
    func_ov032_021f27d4(work);
    func_ov032_021f2908(work);
    func_ov032_021f2a8c(work, work->arc);
    func_ov032_021f2c1c(work);
    func_ov032_021f2d20(work, work->arc);
    func_ov032_021f2f44(work, work->arc);
    func_ov032_021f3058(work);
    work->printNow = TRUE;
    func_ov032_021efec0(work);
    work->printNow = FALSE;
    work->active = FALSE;
    func_ov032_021f0208(work, TRUE);
    return work;
}

void func_ov032_021f22fc(BeaconViewWork *work) {
    GFL_TCBExMgrFreeTasks(work->tcbManager);
    func_ov032_021f30b4(work);
    func_ov032_021f2fcc(work);
    func_ov032_021f2edc(work);
    func_ov032_021f2cc4(work);
    func_ov032_021f2bd4(work);
    func_ov032_021f2a04(work);
    func_ov032_021f28dc(work);
    GFL_ArcToolFree(work->arc);
    GFL_HeapFree(work);
}

void func_ov032_021f2344(BeaconViewWork *work, BOOL active) {
    if (work->active != active) {
        BOOL busy;

        if (work->eventRunning && !active) {
            busy = TRUE;
        } else {
            busy = FALSE;
        }
        if (!busy) {
            work->active = active;
            func_ov032_021f0208(work, active ? FALSE : TRUE);
        }
    }
    if (work->eventRequest != BEACON_VIEW_EVENT_NONE) {
        if (!active) {
            func_ov032_021f2544(work);
        }
        return;
    }
    if (!active) {
        return;
    }
    GFL_TCBExMgrUpdate(work->tcbManager);
    switch (work->seq) {
    case SEQ_MAIN:
        work->seq = func_ov032_021f2674(work);
        break;
    case SEQ_NEW_BEACON:
        work->seq = func_ov032_021f26d8(work);
        break;
    case SEQ_WAIT_TASKS:
        work->seq = func_ov032_021f26f0(work);
        break;
    case SEQ_SCROLL:
        work->seq = func_ov032_021f2714(work);
        break;
    case SEQ_BAR:
        work->seq = func_ov032_021f27b4(work);
        break;
    case SEQ_THANKS:
        work->seq = func_ov032_021f27cc(work);
        break;
    case 6:
    case 7:
        break;
    }
    func_ov010_02165888(work->newTable);
}


void func_ov032_021f23f8(BeaconViewWork *work) {
    func_0201c4e4(work->printQueue);
}

GameEvent *func_ov032_021f2404(BeaconViewWork *work, BOOL active) {
    GameEvent *event;

    if (!active) {
        return NULL;
    }
    switch (work->eventRequest) {
    case BEACON_VIEW_EVENT_MENU_COMM:
        func_ov010_02158614(work->commSys);
    case BEACON_VIEW_EVENT_MENU:
    case BEACON_VIEW_EVENT_MENU_WIFI:
        func_020275b8(work->status, 0);
        event = func_ov032_021f254c(work, EventFieldMenuReturn_Create(work->gsys, work->field, 0), 0, 3);
        break;
    case BEACON_VIEW_EVENT_DETAIL:
        event = func_ov032_021f254c(work, func_ov010_02164b38(work->gsys, work->field, work->selected + work->top), 1, 0);
        break;
    case BEACON_VIEW_EVENT_MEDAL: {
        u32 args[2];

        args[0] = work->selectedPanel;
        args[1] = work->barDragging;
        event = func_ov032_021f254c(
            work, GameEvent_CreateOverlayDelegate(work->gsys, OVERLAY_ID(83), func_ov083_021f5260, args), 2,
            (uintptr_t)work->buttons[1].window);
        work->selectedPanel = 0x30;
        break;
    }
    case BEACON_VIEW_EVENT_PASS_POWER:
        event = func_ov032_021f254c(
            work, GameEvent_CreateOverlayDelegate(work->gsys, OVERLAY_ID(83), func_ov083_021f5368, work->field), 2,
            (uintptr_t)work->buttons[0].window);
        break;
    case BEACON_VIEW_EVENT_PASS_POWER_USE:
        event = func_ov032_021f254c(
            work, GameEvent_CreateOverlayDelegate(work->gsys, OVERLAY_ID(83), func_ov083_021f5368, work->field), 2,
            (uintptr_t)work->buttons2[0].window);
        work->eventWait = FALSE;
        break;
    case BEACON_VIEW_EVENT_PASS_POWER_LIST:
        event = func_ov032_021f254c(
            work, EventPhraseInput_Create(work->gsys, work->field, NULL, 7, func_020275d0(work->status)), 0, 1);
        break;
    default:
        return NULL;
    }
    func_ov032_021f2544(work);
    return event;
}


void func_ov032_021f253c(BeaconViewWork *work, u16 request) {
    work->eventRequest = request;
}

void func_ov032_021f2544(BeaconViewWork *work) {
    work->eventRequest = 0;
}

GameEvent *func_ov032_021f254c(BeaconViewWork *work, GameEvent *next, u32 wait, uintptr_t param) {
    GameEvent *event = GameEvent_Create(work->gsys, NULL, func_ov032_021f2588, sizeof(BeaconViewEvent));
    BeaconViewEvent *data = GameEvent_GetData(event);
    data->work = work;
    data->gsys = work->gsys;
    data->field = work->field;
    data->next = next;
    data->wait = wait;
    data->param = param;
    work->eventRunning = TRUE;
    return event;
}

GameEventReturnCode func_ov032_021f2588(GameEvent *event, u32 *state, void *data) {
    BeaconViewEvent *args = data;
    BeaconViewWork *work = args->work;
    BOOL done = FALSE;
    switch (args->wait) {
    case EVENT_WAIT_BUTTON:
        done = func_ov032_021f25d4(work, args);
        break;
    case EVENT_WAIT_TASKS:
        done = func_ov032_021f2608(work, args);
        break;
    case EVENT_WAIT_MENU_BUTTON:
        done = func_ov032_021f2624(work, args);
        break;
    }
    if (done) {
        work->eventRunning = FALSE;
        GameEvent_Replace(event, args->next);
    }
    return GAMEEVENT_CONTINUE;
}


// The files of a set of OAM resources: how many of each kind, and the first
typedef struct {
    u16 count;
    u16 fileId;
} BeaconViewResFiles;

typedef struct {
    BeaconViewResFiles lists[BEACON_VIEW_RES_KINDS];
} BeaconViewResSetup;

typedef struct {
    u8 x;
    u8 y;
    u8 sequence : 5;
    u8 bgPriority : 3;
} BeaconViewActorSetup;

void func_ov032_021f2bd4(BeaconViewWork *work);
void func_ov032_021f2c1c(BeaconViewWork *work);
void func_ov032_021f2cc4(BeaconViewWork *work);
void func_ov032_021f2d20(BeaconViewWork *work, ArcTool *arc);
void func_ov032_021f2edc(BeaconViewWork *work);
void func_ov032_021f2f44(BeaconViewWork *work, ArcTool *arc);
void func_ov032_021f2fcc(BeaconViewWork *work);
void func_ov032_021f3010(BeaconViewWinSlot *slot, u8 bg, u8 palette, u8 x, u8 y, u8 width, u8 height);
void func_ov032_021f3058(BeaconViewWork *work);
void func_ov032_021f30b4(BeaconViewWork *work);
void func_ov032_021f30d4(BeaconViewWork *work, BeaconViewRes *res, const BeaconViewResSetup *setup, ArcTool *arc);
void func_ov032_021f31a8(BeaconViewWork *work, BeaconViewRes *res);
ClActor *func_ov032_021f3214(BeaconViewWork *work, u32 chars, u32 palette, u32 cellAnims, s16 x, s16 y,
                                    u8 sequence, u8 bgPriority, u8 priority);
void func_ov032_021f3264(BeaconViewWork *work);
void func_ov032_021f32d4(BeaconViewWork *work);
void func_ov032_021f32f8(BeaconViewWork *work, u8 id);
void func_ov032_021f34c8(BeaconViewWork *work, u8 id);

// pokebw2's BeaconView_WaitButton
BOOL func_ov032_021f25d4(BeaconViewWork *work, BeaconViewEvent *event) {
    switch (event->seq) {
    case 0:
        func_ov032_021f05e8(work, (int)event->param, 1);
        event->seq++;
        return FALSE;
    case 1:
        if (func_ov032_021f0668(work, (int)event->param)) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}

// pokebw2's BeaconView_WaitTasks
BOOL func_ov032_021f2608(BeaconViewWork *work, BeaconViewEvent *event) {
    GFL_TCBExMgrUpdate(work->tcbManager);
    if (work->pending == 0) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BeaconView_WaitMenuButton
BOOL func_ov032_021f2624(BeaconViewWork *work, BeaconViewEvent *event) {
    AppTaskMenuWin *win = (AppTaskMenuWin *)event->param;

    switch (event->seq) {
    case 0:
        func_0202822c(win, TRUE);
        event->seq++;
        break;
    case 1:
        func_02028178(win);
        if (func_02028230(win)) {
            func_020282a4(win);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void func_ov032_021f2664(TCB *tcb, void *data) {
    BeaconViewWork *work = data;

    if (work->paletteFade != NULL) {
        func_02021d50(work->paletteFade);
    }
}

int func_ov032_021f2674(BeaconViewWork *work) {
    int result;

    if (func_ov032_021f00c8(work)) {
        work->wait = 0x5a;
        return 2;
    }
    if (func_ov032_021f0078(work)) {
        return 0;
    }
    result = func_ov032_021eff7c(work);
    if (result == 0) {
        if (work->wait != 0) {
            work->wait--;
            return 0;
        }
        if (func_ov032_021f0114(work) == 1) {
            work->wait = 0x5a;
            return 2;
        }
        if (func_ov032_021f0140(work) == 0) {
            return 0;
        }
        work->wait = 0x5a;
        result = 1;
    }
    return result;
}

int func_ov032_021f26d8(BeaconViewWork *work) {
    if (func_ov032_021f02c4(work) == 0) {
        return 1;
    }
    work->subSeq = 0;
    return 2;
}

int func_ov032_021f26f0(BeaconViewWork *work) {
    if (work->pending != 0) {
        return 2;
    }
    func_ov032_021f05e8(work, 4, 0);
    func_ov032_021f067c(work);
    return 0;
}

int func_ov032_021f2714(BeaconViewWork *work) {
    if (work->scrollEnd == 0) {
        int pos = func_ov032_021f0050(work);

        if (pos != work->scrollDir || pos == -1) {
            work->scrollEnd = 1;
        }
        work->scrollFrames++;
    }
    if (work->scrollEnd != 0) {
        if (work->pending > 0) {
            return 3;
        }
        work->scrollEnd = 0;
        work->scrollFrames = 0;
        work->scrolling = 0;
        if (work->scrollReturnSeq == 0) {
            func_ov032_021f05e8(work, 4, 0);
        }
        func_ov032_021f067c(work);
        func_ov032_021f029c(work, 0);
        return work->scrollReturnSeq;
    }
    if (work->pending == 0) {
        func_ov032_021f028c(work);
    }
    return 3;
}

int func_ov032_021f27b4(BeaconViewWork *work) {
    if (func_ov032_021f03ec(work) == 0) {
        return 4;
    }
    work->subSeq = 0;
    return 0;
}

int func_ov032_021f27cc(BeaconViewWork *work) {
    return func_ov032_021f0438(work);
}

void func_ov032_021f27d4(BeaconViewWork *work) {
    SaveControl *save;
    PlayerInfo *info;

    work->status = func_02012a54(work->gameData);
    work->log = func_020275a8(work->status);
    work->logCount = func_ov010_02165b04(work->log);
    work->top = func_020275ac(work->status);
    if (work->logCount < 4) {
        work->viewCount = work->logCount;
    } else {
        work->viewCount = 4;
    }
    work->selectedPanel = 0x30;
    work->unk78 = func_02012ffc();
    save = GameData_GetSaveControl(work->gameData);
    work->unk_28 = func_0200bf10(getHighLinkBlockAddress(save));
    work->commSys = GSYS_GetGameCommSystem(work->gsys);
    work->wifiList = func_0201283c(work->gameData);
    work->bag = GameData_GetBag(work->gameData);
    work->trainerCard = getTrainerCardData_wrapper(save);
    work->passerbyCount = func_0200c1dc(work->trainerCard);
    info = func_02008730(save);
    work->trainerId = getIDAsUInt(info) & 0xffff;
    work->gender = getTrainerGender(info);
    work->version = func_02008554(info);
    work->unk_32 = GAME_VERSION;
    work->newTable = func_ov010_02165b2c(work->heapId);
    work->beacon = func_ov010_02165b10(work->heapId);
    work->countBeacon = func_ov010_02165b10(work->heapId);
    work->newBeacon = func_ov010_02165b10(work->heapId);
    work->passPowerData = PassPowerData_Create(work->heapId);
    func_ov032_021f0684(work, &work->otherGenderCount, &work->otherVersionCount);
}


void func_ov032_021f28dc(BeaconViewWork *work) {
    func_ov010_02165940(work->newTable->beacons);
    PassPowerData_Free(work->passPowerData);
    GFL_HeapFree(work->newBeacon);
    GFL_HeapFree(work->countBeacon);
    GFL_HeapFree(work->beacon);
    GFL_HeapFree(work->newTable);
}

void func_ov032_021f2908(BeaconViewWork *work) {
    work->tcbManager = GFL_TCBExMgrCreate(work->heapId, work->heapId, 0x10, 0x80);
    work->vblankTask = GFL_VBlankTCBAdd(func_ov032_021f2664, work, 0);
    work->paletteFade = func_02021518(work->heapId);
    func_0202155c(work->paletteFade, 1, 0x1c0, work->heapId);
    func_0202155c(work->paletteFade, 3, 0x200, work->heapId);
    work->printQueue = func_0201c440(work->heapId);
    func_0201c4c8(work->printQueue, 1);
    func_0201c4dc(work->printQueue, 500);
    work->font = func_ov021_021904ac(Field_GetMsgBGSys(work->field));
    func_0201dd68();
    work->wordSet = GFL_WordSetSystemCreateDefault(work->heapId);
    work->msgData = GFL_MsgSysLoadData(1, 2, 8, work->heapId);
    work->strTemplate = GFL_StrBufCreate(0xd9, work->heapId);
    work->strExpand = GFL_StrBufCreate(0xd9, work->heapId);
    work->strMessage = GFL_StrBufCreate(0xd9, work->heapId);
}

void func_ov032_021f2a04(BeaconViewWork *work) {
    GFL_StrBufFree(work->strMessage);
    GFL_StrBufFree(work->strExpand);
    GFL_StrBufFree(work->strTemplate);
    GFL_MsgDataFree(work->msgData);
    GFL_WordSetSystemFree(work->wordSet);
    func_0201c6ec(work->printQueue);
    func_0201c4c0(work->printQueue);
    func_0201dd68();
    func_020215a0(work->paletteFade, 3);
    func_020215a0(work->paletteFade, 1);
    func_02021540(work->paletteFade);
    work->paletteFade = NULL;
    GFL_TCBRemove(work->vblankTask);
    GFL_TCBExMgrFree(work->tcbManager);
}

static const BGSetup data_ov032_021f3608[4] = {
    {
        0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x7800, GX_BG_CHARBASE_0x00000, 0x6000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, 0,
    },
    {
        0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x7000, GX_BG_CHARBASE_0x00000, 0x6000,
        GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, 0,
    },
    {
        0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x6800, GX_BG_CHARBASE_0x00000, 0x6000,
        GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, 0,
    },
    {
        0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x6000, GX_BG_CHARBASE_0x00000, 0x6000,
        GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, 0,
    },
};

void func_ov032_021f2a8c(BeaconViewWork *work, ArcTool *arc) {
    GFL_BGSysCreateBG(4, &data_ov032_021f3608[0], 0);
    GFL_BGSysCreateBG(5, &data_ov032_021f3608[1], 0);
    GFL_BGSysCreateBG(6, &data_ov032_021f3608[2], 0);
    GFL_BGSysCreateBG(7, &data_ov032_021f3608[3], 0);
    MIi_CpuClear32(0, (void *)HW_DB_BG_VRAM, 0x20000);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    G2S_BlendNone();
    func_020216b4(work->paletteFade, arc, 0x13, work->tmpHeapId, 1, 0x1a0, 0);
    func_02021640(work->paletteFade, ARCID_FONT, 5, work->tmpHeapId, 1, 0x20, 0xd0);
    GFL_BGSysLoadArcNCGRStatic(arc, 0x14, 7, 0, 0, TRUE, work->tmpHeapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 0x15, 5, 0, 0x800, TRUE, work->tmpHeapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 0x16, 6, 0, 0x800, TRUE, work->tmpHeapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 0x17, 7, 0, 0x800, TRUE, work->tmpHeapId);
    G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2,
                      GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ,
                      13, 3);
    GFL_BGSysSetBGEnabled(4, 1);
    GFL_BGSysSetBGEnabled(5, 1);
    GFL_BGSysSetBGEnabled(6, 1);
    GFL_BGSysSetBGEnabled(7, 1);
}


// pokebw2's BeaconView_FreeBG
void func_ov032_021f2bd4(BeaconViewWork *work) {
    GFL_BGSysSetBGEnabled(4, FALSE);
    GFL_BGSysSetBGEnabled(5, FALSE);
    GFL_BGSysSetBGEnabled(6, FALSE);
    GFL_BGSysSetBGEnabled(7, FALSE);
    G2S_BlendNone();
    GFL_BGSysReleaseBG(4);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(6);
    GFL_BGSysReleaseBG(7);
}

void func_ov032_021f2c1c(BeaconViewWork *work) {
    int i;

    work->keyCursor = func_02028590(9, FALSE, TRUE, work->heapId);
    work->taskMenuRes = func_02027f60(4, 14, work->font, work->printQueue, work->heapId);
    for (i = 0; i < 3; i++) {
        work->buttons[i].item.str = GFL_MsgDataLoadStrbufNew(work->msgData, i + 12);
        work->buttons[i].item.color = 0x39e3;
        work->buttons[i].item.type = 0;
        work->buttons[i].window = NULL;
    }
    for (i = 0; i < 2; i++) {
        work->buttons2[i].item.str = GFL_MsgDataLoadStrbufNew(work->msgData, i + 15);
        work->buttons2[i].item.color = 0x39e3;
        work->buttons2[i].item.type = 0;
        work->buttons2[i].window = NULL;
    }
}


void func_ov032_021f2cc4(BeaconViewWork *work) {
    int i;

    for (i = 0; i < 2; i++) {
        if (work->buttons2[i].window != NULL) {
            func_02028148(work->buttons2[i].window);
        }
        GFL_StrBufFree(work->buttons2[i].item.str);
    }
    for (i = 0; i < 3; i++) {
        if (work->buttons[i].window != NULL) {
            func_02028148(work->buttons[i].window);
        }
        GFL_StrBufFree(work->buttons[i].item.str);
    }
    func_02027fd8(work->taskMenuRes);
    func_02028604(work->keyCursor);
}

void func_ov032_021f2d20(BeaconViewWork *work, ArcTool *arc) {
    int i;
    ArcTool *trainerArc = GFL_ArcSysCreateFileHandle(ARCID_BEACON_VIEW_TRAINER, work->tmpHeapId);

    {
        BeaconViewResSetup setup = { { { 1, 0 }, { 1, 1 }, { 1, 2 } } };

        func_ov032_021f30d4(work, &work->res[BEACON_VIEW_RES_COMMON], &setup, arc);
    }
    {
        BeaconViewResSetup setup = { { { 0, 0 }, { 5, 7 }, { 1, 5 } } };

        func_ov032_021f30d4(work, &work->res[BEACON_VIEW_RES_ICON], &setup, arc);
    }
    {
        BeaconViewResSetup setup = { { { 0, 0 }, { 5, 49 }, { 1, 65 } } };

        func_ov032_021f30d4(work, &work->res[BEACON_VIEW_RES_TRAINER], &setup, trainerArc);
    }
    func_020216b4(work->paletteFade, arc, 0, work->tmpHeapId, 3, 0x1c0, 0);
    func_02021640(work->paletteFade, 0, 1, work->tmpHeapId, 3, 0x20, 0xe0);
    work->commonPalette.file = GFL_ArcToolReadHeapNew(arc, 4, work->heapId);
    NNS_G2dGetUnpackedPaletteData(work->commonPalette.file, &work->commonPalette.data);
    work->commonPalette.colors = work->commonPalette.data->pRawData;
    work->trainerPalette.file = GFL_ArcToolReadHeapNew(trainerArc, 0, work->heapId);
    NNS_G2dGetUnpackedPaletteData(work->trainerPalette.file, &work->trainerPalette.data);
    work->trainerPalette.colors = work->trainerPalette.data->pRawData;
    for (i = 0; i < BEACON_VIEW_TRAINER_CHARS; i++) {
        work->trainerChars[i].file = GFL_ArcToolReadHeapNew(trainerArc, i + 49, work->heapId);
        NNS_G2dGetUnpackedCharacterData(work->trainerChars[i].file, &work->trainerChars[i].data);
    }
    for (i = 0; i < BEACON_VIEW_ICON_CHARS; i++) {
        work->iconChars[i].file = GFL_ArcToolReadHeapNew(arc, i + 7, work->heapId);
        NNS_G2dGetUnpackedCharacterData(work->iconChars[i].file, &work->iconChars[i].data);
    }
    DC_FlushAll();
    GFL_ArcToolFree(trainerArc);
}


// pokebw2's BeaconView_FreeResources
void func_ov032_021f2edc(BeaconViewWork *work) {
    int i;

    for (i = 0; i < BEACON_VIEW_ICON_CHARS; i++) {
        GFL_HeapFree(work->iconChars[i].file);
    }
    for (i = 0; i < BEACON_VIEW_TRAINER_CHARS; i++) {
        GFL_HeapFree(work->trainerChars[i].file);
    }
    GFL_HeapFree(work->commonPalette.file);
    GFL_HeapFree(work->trainerPalette.file);
    func_ov032_021f31a8(work, &work->res[BEACON_VIEW_RES_TRAINER]);
    func_ov032_021f31a8(work, &work->res[BEACON_VIEW_RES_ICON]);
    func_ov032_021f31a8(work, &work->res[BEACON_VIEW_RES_COMMON]);
}

// pokebw2's BeaconView_InitActors
void func_ov032_021f2f44(BeaconViewWork *work, ArcTool *arc) {
    int i;

    work->clUnit = func_0204b100(64, 0, work->resHeapId);
    {
        ClActSurfaceSetup surface = { 0, 512, 256, 192, 1, 1 };

        work->renderer = func_0204b034(&surface, 1, work->heapId);
    }
    func_0204b258(work->clUnit, work->renderer);
    work->bmpOam = func_02025538(work->heapId, work->clUnit);
    func_ov032_021f3264(work);
    for (i = 0; i < BEACON_VIEW_PANEL_MAX; i++) {
        func_ov032_021f32f8(work, i);
    }
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's BeaconView_FreeActors
void func_ov032_021f2fcc(BeaconViewWork *work) {
    int i;

    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, FALSE);
    for (i = 0; i < BEACON_VIEW_PANEL_MAX; i++) {
        func_ov032_021f34c8(work, i);
    }
    func_ov032_021f32d4(work);
    func_02025588(work->bmpOam);
    func_0204b084(work->renderer);
    func_0204b1cc(work->clUnit);
}

void func_ov032_021f3010(BeaconViewWinSlot *slot, u8 bg, u8 palette, u8 x, u8 y, u8 width, u8 height) {
    BmpWin *window;

    slot->win.window = BmpWin_CreateDynamic(bg, x, y, width, height, palette, TRUE);
    slot->win.bitmap = BmpWin_GetBitmap(slot->win.window);
    PrintWindow_Init(&slot->win.printWindow, slot->win.window);
    window = slot->win.window;
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
}

void func_ov032_021f3058(BeaconViewWork *work) {
    func_ov032_021f3010(&work->wins[BEACON_VIEW_WIN_MESSAGE], 6, 1, 2, 22, 28, 6);
    func_ov032_021f3010(&work->wins[BEACON_VIEW_WIN_TOTAL], 4, 13, 1, 21, 15, 3);
    func_ov032_021f3010(&work->wins[2], 7, 4, 0, 19, 6, 2);
}


void func_ov032_021f30b4(BeaconViewWork *work) {
    int i;

    for (i = 0; i < BEACON_VIEW_WIN_COUNT; i++) {
        BmpWin_Free(work->wins[i].win.window);
    }
}


void func_ov032_021f30d4(BeaconViewWork *work, BeaconViewRes *res, const BeaconViewResSetup *setup,
                               ArcTool *arc) {
    int i;

    MI_CpuFill8(res, 0, sizeof(BeaconViewRes));
    for (i = 0; i < BEACON_VIEW_RES_KINDS; i++) {
        res->lists[i].count = setup->lists[i].count;
        if (res->lists[i].count != 0) {
            res->lists[i].ids =
                GFL_HeapAllocate(work->resHeapId, res->lists[i].count * sizeof(u32), TRUE, "beacon_view.c", 1219);
        }
    }
    for (i = 0; i < res->lists[BEACON_VIEW_RES_PALETTE].count; i++) {
        res->lists[BEACON_VIEW_RES_PALETTE].ids[i] =
            func_0204abf0(arc, setup->lists[BEACON_VIEW_RES_PALETTE].fileId + i, CLACT_VRAM_SUB, 0, work->resHeapId);
    }
    for (i = 0; i < res->lists[BEACON_VIEW_RES_CHARS].count; i++) {
        res->lists[BEACON_VIEW_RES_CHARS].ids[i] =
            func_0204a6c8(arc, setup->lists[BEACON_VIEW_RES_CHARS].fileId + i, FALSE, CLACT_VRAM_SUB, work->resHeapId);
    }
    for (i = 0; i < res->lists[BEACON_VIEW_RES_CELLS].count; i++) {
        res->lists[BEACON_VIEW_RES_CELLS].ids[i] =
            func_0204af28(arc, setup->lists[BEACON_VIEW_RES_CELLS].fileId + i * 2,
                          setup->lists[BEACON_VIEW_RES_CELLS].fileId + 1 + i * 2, work->resHeapId);
    }
}

// pokebw2's BeaconView_FreeRes
void func_ov032_021f31a8(BeaconViewWork *work, BeaconViewRes *res) {
    int i;

    for (i = 0; i < res->lists[BEACON_VIEW_RES_CELLS].count; i++) {
        func_0204afd8(res->lists[BEACON_VIEW_RES_CELLS].ids[i]);
    }
    for (i = 0; i < res->lists[BEACON_VIEW_RES_CHARS].count; i++) {
        func_0204a8d4(res->lists[BEACON_VIEW_RES_CHARS].ids[i]);
    }
    for (i = 0; i < res->lists[BEACON_VIEW_RES_PALETTE].count; i++) {
        func_0204ada4(res->lists[BEACON_VIEW_RES_PALETTE].ids[i]);
    }
    for (i = 0; i < BEACON_VIEW_RES_KINDS; i++) {
        if (res->lists[i].ids != NULL) {
            GFL_HeapFree(res->lists[i].ids);
        }
    }
}

ClActor *func_ov032_021f3214(BeaconViewWork *work, u32 chars, u32 palette, u32 cellAnims, s16 x, s16 y,
                                    u8 sequence, u8 bgPriority, u8 priority) {
    ClActorSetup setup;
    ClActor *actor;

    setup.x = x;
    setup.y = y;
    setup.sequence = sequence;
    setup.bgPriority = bgPriority;
    setup.priority = priority;
    actor = func_0204b294(work->clUnit, chars, palette, cellAnims, &setup, 0, work->resHeapId);
    func_0204b92c(actor, TRUE);
    func_0204ba20(actor, FALSE);
    return actor;
}

const s16 sActorPositions[6][2] = {
    { 132, 172 }, { 164, 172 }, { 196, 172 }, { 228, 172 }, { 208, 8 }, { 216, 40 },
};

void func_ov032_021f3264(BeaconViewWork *work) {
    int i;

    for (i = 0; i < BEACON_VIEW_ACTOR_COUNT; i++) {
        work->actors[i] = func_ov032_021f3214(work,
            work->res[BEACON_VIEW_RES_COMMON].lists[BEACON_VIEW_RES_CHARS].ids[0],
            work->res[BEACON_VIEW_RES_COMMON].lists[BEACON_VIEW_RES_PALETTE].ids[0],
            work->res[BEACON_VIEW_RES_COMMON].lists[BEACON_VIEW_RES_CELLS].ids[0],
            sActorPositions[i][0], sActorPositions[i][1], i * 3 + 1, 1, 1);
        func_0204b3dc(work->actors[i], TRUE);
    }
}


// pokebw2's BeaconView_DeleteScreenActors
void func_ov032_021f32d4(BeaconViewWork *work) {
    int i;

    for (i = 0; i < BEACON_VIEW_ACTOR_COUNT; i++) {
        func_0204b3dc(work->actors[i], FALSE);
        func_0204b3b4(work->actors[i]);
    }
}

void func_ov032_021f32f8(BeaconViewWork *work, u8 id) {
    BeaconViewPanel *panel = &work->panels[id];
    BmpOamActorSetup setup;

    MI_CpuFill8(panel, 0, sizeof(BeaconViewPanel));
    panel->id = id;
    panel->dataIndex = 0xff;
    panel->line = 0;
    panel->queue = work->printQueue;
    panel->x = id * 13;
    panel->y = id * 40;
    panel->base = func_ov032_021f3214(work, work->res[BEACON_VIEW_RES_COMMON].lists[BEACON_VIEW_RES_CHARS].ids[0],
                                      work->res[BEACON_VIEW_RES_COMMON].lists[BEACON_VIEW_RES_PALETTE].ids[0],
                                      work->res[BEACON_VIEW_RES_COMMON].lists[BEACON_VIEW_RES_CELLS].ids[0], panel->x,
                                      panel->y, 0, 3, id + 20);
    func_0204b6f4(panel->base, id, 0);
    panel->trainer = func_ov032_021f3214(work, work->res[BEACON_VIEW_RES_TRAINER].lists[BEACON_VIEW_RES_CHARS].ids[id],
                                         work->res[BEACON_VIEW_RES_COMMON].lists[BEACON_VIEW_RES_PALETTE].ids[0],
                                         work->res[BEACON_VIEW_RES_TRAINER].lists[BEACON_VIEW_RES_CELLS].ids[0],
                                         panel->x + 32, panel->y + 24, 0, 3, id + 10);
    func_0204b6f4(panel->trainer, (u8)(id + 9), 1);
    panel->icon = func_ov032_021f3214(work, work->res[BEACON_VIEW_RES_ICON].lists[BEACON_VIEW_RES_CHARS].ids[id],
                                      work->res[BEACON_VIEW_RES_COMMON].lists[BEACON_VIEW_RES_PALETTE].ids[0],
                                      work->res[BEACON_VIEW_RES_ICON].lists[BEACON_VIEW_RES_CELLS].ids[0],
                                      panel->x + 60, panel->y + 24, 0, 3, id + 5);
    panel->oam.bitmap = GFL_BitmapCreate(12, 2, 0x20, work->heapId);
    setup.bitmap = panel->oam.bitmap;
    setup.x = panel->x + 72;
    setup.y = panel->y + 16;
    setup.palette = work->res[BEACON_VIEW_RES_COMMON].lists[BEACON_VIEW_RES_PALETTE].ids[0];
    setup.paletteOffset = 3;
    setup.priority = 0;
    setup.bgPriority = 3;
    setup.surface = 0;
    setup.vramType = CLACT_VRAM_SUB;
    panel->oam.actor = func_020255a0(work->bmpOam, &setup);
    panel->greeting = GFL_StrBufCreate(11, work->heapId);
    panel->name = GFL_StrBufCreate(8, work->heapId);
    GFL_BitmapFill(panel->oam.bitmap, 0);
    func_020257d0(panel->oam.actor);
    func_02025774(panel->oam.actor, FALSE);
    func_0204b3dc(panel->base, FALSE);
    func_0204b3dc(panel->trainer, FALSE);
    func_0204b3dc(panel->icon, FALSE);
}


void func_ov032_021f34c8(BeaconViewWork *work, u8 id) {
    BeaconViewPanel *panel = &work->panels[id];

    GFL_StrBufFree(panel->name);
    GFL_StrBufFree(panel->greeting);
    func_02025774(panel->oam.actor, FALSE);
    func_0202570c(panel->oam.actor);
    GFL_BitmapFree(panel->oam.bitmap);
    func_0204b3dc(panel->icon, FALSE);
    func_0204b3b4(panel->icon);
    func_0204b3dc(panel->trainer, FALSE);
    func_0204b3b4(panel->trainer);
    func_0204b3dc(panel->base, FALSE);
    func_0204b3b4(panel->base);
    MI_CpuFill8(panel, 0, sizeof(BeaconViewPanel));
}


