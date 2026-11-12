// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov084/beacon_view.c and beacon_view_sub.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The ROM has no name for this file: it ends where beacon_view.c, the one holding the ROM's only file-name string, begins
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
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_cp.h"
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
#include "nitro/os.h"
#include "nnsys/g2d.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "save/trainer_data_savedata.h"
#include "system/app_keycursor.h"
#include "system/app_taskmenu.h"
#include "system/game_event.h"
#include "system/gf_font.h"

// The beacon view's sequences: what the field's touch screen calls, and what the view does from frame to frame

// The view's sequences
#define SEQ_MAIN 0
#define SEQ_NEW_BEACON 1
#define SEQ_WAIT_TASKS 2
#define SEQ_SCROLL 3
#define SEQ_BAR 4
#define SEQ_THANKS 5

// What SEQ_THANKS does
#define THANKS_START 0
#define THANKS_WAIT_BUTTON 1
#define THANKS_INPUT 2
#define THANKS_SCROLL_END 3
#define THANKS_SEND 4
#define THANKS_SENT 5
#define THANKS_END 6
#define THANKS_EXIT 7
// The bar is touched: not a state, but what the input gives
#define THANKS_BAR 8

// How long the view leaves a message alone
#define MESSAGE_WAIT 90

int func_ov032_021f0050(BeaconViewWork *work);
BOOL func_ov032_021f0078(BeaconViewWork *work);
// Black's symbols, not yet declared in any header
void func_ov032_021f1a18(BeaconViewWork *work, u32 passPower);
void func_ov032_021f1268(BeaconViewWork *work);
BOOL func_ov010_02165b5c(const GameBeacon *beacon);
BOOL func_ov032_021f180c(BeaconViewWork *work, const GameBeacon *beacon, BOOL gift);
void func_ov032_021f0dd8(u32 se);
int func_ov032_021f055c(BeaconViewWork *work);
GameEvent *func_ov032_021f254c(BeaconViewWork *work, GameEvent *next, u32 wait, uintptr_t param);
void func_ov032_021f2544(BeaconViewWork *work);

void func_ov032_021efec0(BeaconViewWork *work) {
    int i;

    func_02021ee4(work->paletteFade, TRUE);
    func_ov032_021f0ac0(work);
    func_ov032_021f0b10(work, 1, NULL);
    for (i = 0; i < work->viewCount; i++) {
        u8 top = work->top;
        BeaconViewPanel *panel;
        u32 dataIndex;

        func_ov010_02165ac4(work->log, work->beacon, &work->time, top + i);
        dataIndex = func_ov010_021659f0(work->log, top + i);
        panel = func_ov032_021f0f44(work, 0xff);
        func_ov032_021f0f30(panel, dataIndex, i + 1);
        func_ov032_021f1144(work, panel, work->beacon, 2, FALSE, NULL);
        func_ov032_021f0ea4(panel, TRUE);
    }
    func_ov032_021f0e24(work);
    func_ov032_021f0e7c(work);
    if (work->unk_28 == 0x30) {
        func_ov032_021f0df4(work->actors[BEACON_VIEW_ACTOR_PASS_POWER], 3);
    }
    func_0203d34c(FALSE, work->tmpHeapId);
}

int func_ov032_021eff7c(BeaconViewWork *work) {
    BeaconViewTouch touch;
    int button;
    u32 index;
    int arrow;

    if (!func_02035d20((u32 *)&touch.x, (u32 *)&touch.y)) {
        return SEQ_MAIN;
    }
    button = func_ov032_021f0a18(work, &touch);
    if (button != -1) {
        switch (button) {
        case 0:
            return SEQ_BAR;
        case 1:
            func_ov032_021f253c(work, BEACON_VIEW_EVENT_PASS_POWER_LIST);
            break;
        case 2:
            return SEQ_THANKS;
        case 3:
            func_ov032_021f253c(work, BEACON_VIEW_EVENT_MENU);
            break;
        }
        return SEQ_MAIN;
    }
    index = func_ov032_021f08e4(work, &touch);
    if (index != -1) {
        func_ov010_02165ac4(work->log, work->beacon, &work->time, work->top + index);
        if (func_ov010_02165c10(work->beacon)) {
            return SEQ_MAIN;
        }
        work->selected = index;
        func_ov032_021f1f00(work, index);
        func_ov032_021f253c(work, BEACON_VIEW_EVENT_DETAIL);
        return SEQ_MAIN;
    }
    arrow = func_ov032_021f096c(work, &touch);
    if (arrow != -1) {
        func_ov032_021f1340(work, arrow, 0);
        return SEQ_SCROLL;
    }
    return SEQ_MAIN;
}


int func_ov032_021f0050(BeaconViewWork *work) {
    BeaconViewTouch touch;

    if (func_02035cb8((u32 *)&touch.x, (u32 *)&touch.y)) {
        return func_ov032_021f096c(work, &touch);
    }
    return TOUCH_RECT_NONE;
}

// Leaves the view for the menu when something else needs the touch screen
// pokebw2's BeaconView_CheckLeave
BOOL func_ov032_021f0078(BeaconViewWork *work) {
    u32 state = func_ov010_021585e0(work->commSys);

    if (func_02012204(work->gsys)) {
        func_ov032_021f253c(work, BEACON_VIEW_EVENT_MENU);
        return TRUE;
    }
    if (state == 2) {
        func_ov032_021f253c(work, BEACON_VIEW_EVENT_MENU_COMM);
        return TRUE;
    }
    if (func_0201058c(work->wifiList) & 8) {
        func_ov032_021f253c(work, BEACON_VIEW_EVENT_MENU_WIFI);
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov032_021f00c8(BeaconViewWork *work) {
    u16 *count = func_020275d0(work->status);

    if (*count == 0) {
        return FALSE;
    }
    *count = 0;
    StrBuf *strbuf = func_020275c4(work->status);
    func_0201ee0c(work->wordSet, 0, strbuf, work->gender, 1, 2);
    func_ov032_021f18c4(work, 0xb);
    return TRUE;
}

BOOL func_ov032_021f0114(BeaconViewWork *work) {
    u32 passPower = func_ov032_021f07d4(work);
    if (passPower == 0x30) {
        return FALSE;
    }
    if (passPower == PassPower_GetUsedIDByPassPower(passPower, work->passPowerData)) {
        return FALSE;
    }
    func_ov032_021f1a18(work, passPower);
    return TRUE;
}

BOOL func_ov032_021f0140(BeaconViewWork *work) {
    if (!func_ov010_021658d4(work->newTable, work->newBeacon, &work->newTime)) {
        func_ov032_021f1268(work);
        return FALSE;
    }
    if (func_ov010_02165b5c(work->newBeacon)) {
        return FALSE;
    }
    work->newOffset = func_ov010_02165a10(work->log, work->newBeacon, work->newTime, &work->isNew);
    work->newIndex = func_ov010_021659f0(work->log, work->newOffset);
    if (!work->isNew) {
        work->newPanel = func_ov032_021f0f44(work, work->newIndex);
    } else {
        work->newPanel = NULL;
        work->passerbyCount = func_0200c1e0(work->trainerCard);
    }
    if (work->newPanel != NULL && work->newPanel->printTask != NULL) {
        work->subSeq = 0;
    } else {
        work->subSeq = 1;
    }
    work->oldCount = work->logCount;
    if (work->logCount < GAME_BEACON_LOG_MAX) {
        work->logCount += (u8)work->isNew;
        if (work->logCount <= BEACON_VIEW_LINE_MAX) {
            work->viewCount = work->logCount;
        }
    }
    return TRUE;
}


void func_ov032_021f0208(BeaconViewWork *work, BOOL dimmed) {
    if (dimmed) {
        func_02021fd8(work->paletteFade, 1, 0, 0xe0, 6, 0);
        func_02021fd8(work->paletteFade, 3, 0, 0xe0, 6, 0);
        FieldDispControl_ReqSetAlphaB(work->dispControl, 5, 0x1e, 10, 6);
    } else {
        func_02021fd8(work->paletteFade, 1, 0, 0xe0, 0, 0);
        func_02021fd8(work->paletteFade, 3, 0, 0xe0, 0, 0);
        FieldDispControl_ReqSetAlphaB(work->dispControl, 4, 0x1e, 13, 3);
    }
}


void func_ov032_021f028c(BeaconViewWork *work) {
    func_ov032_021f12b8(work, work->scrollDir);
}

void func_ov032_021f029c(BeaconViewWork *work, BOOL slow) {
    if (slow) {
        func_0201c4dc(work->printQueue, 2000);
    } else {
        func_0201c4dc(work->printQueue, 500);
    }
}

BOOL func_ov032_021f02c4(BeaconViewWork *work) {
    switch (work->subSeq) {
    case 0:
        if (work->newPanel->printTask != NULL) {
            break;
        }
        work->subSeq++;
        break;
    case 1: {
        BOOL message = func_ov032_021f180c(work, work->newBeacon, work->isNew);

        if (!work->isNew) {
            if (message) {
                func_ov032_021f05e8(work, BEACON_VIEW_BUTTON_ALL, 2);
                func_ov032_021f0dd8(0x549);
            }
            if (work->newPanel != NULL) {
                func_ov032_021f1144(work, work->newPanel, work->newBeacon, 0, message ? FALSE : TRUE, NULL);
                func_ov032_021f0ea4(work->newPanel, TRUE);
            }
            return TRUE;
        }
        func_ov032_021f05e8(work, BEACON_VIEW_BUTTON_ALL, 2);
        work->subSeq++;
        break;
    }
    case 2:
        func_ov032_021f0ac0(work);
        func_ov032_021f0b10(work, 1, &work->pending);
        func_ov032_021f06e0(work, work->newBeacon, work->oldCount);
        work->subSeq++;
        break;
    case 3:
        if (work->top != 0) {
            work->newOffset = work->top;
            func_ov010_02165ac4(work->log, work->newBeacon, &work->newTime, work->newOffset);
            work->newIndex = func_ov010_021659f0(work->log, work->newOffset);
        }
        func_ov032_021f13a0(work, work->newBeacon, work->newIndex, 1, (work->top != 0 ? 1 : 0) + 1);
        work->top = work->newOffset;
        func_ov032_021f0dd8(0x64a);
        return TRUE;
    }
    return FALSE;
}


BOOL func_ov032_021f03ec(BeaconViewWork *work) {
    switch (work->subSeq) {
    case 0:
        func_ov032_021f05e8(work, 0, TRUE);
        func_ov032_021f029c(work, TRUE);
        func_ov032_021f1a08(work);
        work->subSeq++;
        break;
    case 1:
        if (work->pending == 0) {
            func_ov032_021f029c(work, FALSE);
            work->subSeq = 0;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

int func_ov032_021f0438(BeaconViewWork *work) {
    if (work->pending != 0) {
        return SEQ_THANKS;
    }
    switch (work->subSeq) {
    case THANKS_START:
        func_ov032_021f05e8(work, BEACON_VIEW_ACTOR_THANKS, 1);
        func_ov032_021f0b10(work, 2, &work->pending);
        work->subSeq++;
        break;
    case THANKS_WAIT_BUTTON:
        if (func_ov032_021f0668(work, BEACON_VIEW_ACTOR_THANKS)) {
            break;
        }
        func_ov032_021f05e8(work, BEACON_VIEW_ACTOR_PASS_POWER, 2);
        func_ov032_021f05e8(work, BEACON_VIEW_ACTOR_GREETING, 2);
        func_ov032_021f05e8(work, BEACON_VIEW_ACTOR_RETURN, 2);
        work->subSeq++;
        break;
    case THANKS_INPUT:
        work->subSeq = func_ov032_021f055c(work);
        if (work->subSeq == THANKS_SCROLL_END) {
            return SEQ_SCROLL;
        }
        break;
    case THANKS_SEND: {
        u8 dataIndex = func_ov010_021659f0(work->log, work->selected + work->top);
        BeaconViewPanel *panel = func_ov032_021f0f44(work, dataIndex);

        func_0201ee0c(work->wordSet, 0, panel->name, panel->gender, 1, 2);
        func_ov032_021f029c(work, TRUE);
        func_ov032_021f18c4(work, 3);
        func_ov010_021657e8(work->gameData, panel->trainerId);
        work->subSeq = THANKS_SENT;
        break;
    }
    case THANKS_SENT:
        func_ov032_021f029c(work, FALSE);
        work->subSeq = THANKS_END;
        break;
    case THANKS_SCROLL_END:
        func_ov032_021f05e8(work, BEACON_VIEW_ACTOR_THANKS, 0);
        work->subSeq = THANKS_INPUT;
        break;
    case THANKS_END:
        func_ov032_021f05e8(work, BEACON_VIEW_BUTTON_ALL, 0);
        func_ov032_021f0b10(work, 1, &work->pending);
        work->subSeq = THANKS_EXIT;
        break;
    case THANKS_EXIT:
        work->subSeq = 0;
        return SEQ_MAIN;
    }
    return SEQ_THANKS;
}


int func_ov032_021f055c(BeaconViewWork *work) {
    BeaconViewTouch touch;
    int hit;

    if (func_02012204(work->gsys)) {
        func_ov032_021f253c(work, 1);
        return THANKS_END;
    }
    if (!func_02035d20((u32 *)&touch.x, (u32 *)&touch.y)) {
        return THANKS_INPUT;
    }
    if (func_ov032_021f0a18(work, &touch) == 2) {
        return THANKS_END;
    }
    hit = func_ov032_021f08e4(work, &touch);
    if (hit != -1) {
        work->selected = hit;
        func_ov032_021f1f00(work, hit);
        return THANKS_SEND;
    }
    hit = func_ov032_021f096c(work, &touch);
    if (hit != -1) {
        func_ov032_021f1340(work, hit, SEQ_THANKS);
        return THANKS_SCROLL_END;
    }
    return THANKS_INPUT;
}

void func_ov032_021f05e8(BeaconViewWork *work, int button, int state) {
    u8 buttons;

    if (button == BEACON_VIEW_BUTTON_ALL) {
        buttons = 0xf;
    } else {
        buttons = 1 << button;
    }
    if ((buttons & (1 << BEACON_VIEW_ACTOR_PASS_POWER)) && work->unk_28 != 0x30) {
        func_ov032_021f0df4(work->actors[BEACON_VIEW_ACTOR_PASS_POWER], (u8)(state + 1));
    }
    if (buttons & (1 << BEACON_VIEW_ACTOR_GREETING)) {
        func_ov032_021f0df4(work->actors[BEACON_VIEW_ACTOR_GREETING], (u8)(state + 4));
    }
    if ((buttons & (1 << BEACON_VIEW_ACTOR_THANKS)) && work->logCount != 0) {
        func_ov032_021f0df4(work->actors[BEACON_VIEW_ACTOR_THANKS], (u8)(state + 7));
    }
    if (buttons & (1 << BEACON_VIEW_ACTOR_RETURN)) {
        func_ov032_021f0df4(work->actors[BEACON_VIEW_ACTOR_RETURN], (u8)(state + 10));
    }
}


BOOL func_ov032_021f0668(BeaconViewWork *work, int button) {
    return func_0204b98c(work->actors[button]);
}


void func_ov032_021f067c(BeaconViewWork *work) {
    func_ov032_021f0e24(work);
}

void func_ov032_021f0684(BeaconViewWork *work, u8 *otherGender, u8 *otherVersion) {
    u16 time[2];
    int i;

    *otherGender = 0;
    *otherVersion = 0;
    for (i = 0; i < work->logCount; i++) {
        func_ov010_02165ac4(work->log, work->countBeacon, time, i);
        if (work->gender != func_ov010_02165d08(work->countBeacon)) {
            (*otherGender)++;
        }
        if (work->unk_32 != func_ov010_02165c48(work->countBeacon)) {
            (*otherVersion)++;
        }
    }
}


// The beacon view's parts: where the screen is touched, its text, the log's lines and how they move, and the tasks
// that show a message, a Pass Power or a Funfest mission over the log

#define PANEL_X 8
#define PANEL_HEIGHT 40
// A line's y: line 1 is the top of the screen
#define PANEL_LINE_Y(line) (-PANEL_HEIGHT * (1 - (line)))
// The plates are slanted: each line sits further right
#define PANEL_SLANT 13
#define PANEL_LINE_X(line) (-PANEL_SLANT * (1 - (line)))

#define BAR_X 232
#define BAR_TOP 44
#define BAR_BOTTOM 132

// How long a beacon's name stays in the fresh color, in seconds
#define BEACON_FRESH_SECONDS 10800

typedef struct {
    u8 seq;
    u8 dir;
    u8 frames;
    int mode;
    BeaconViewPanel *panel;
    BeaconViewWork *work;
    int printing;
    int *pending;
} BeaconViewAddLineTask;

typedef struct {
    int seq;
    BeaconViewWork *work;
    BeaconViewPanel *panel;
    u8 dir;
    u8 delay;
    u8 frames;
    u8 frame;
    s16 destX;
    s16 destY;
    fx32 x;
    fx32 y;
    fx32 stepX;
    fx32 stepY;
    int *pending;
} BeaconViewMoveTask;

typedef struct {
    u8 dir;
    u8 frames;
    u8 frame;
    s8 pos;
    s8 step;
    int *pending;
} BeaconViewBgScrollTask;

typedef struct {
    BeaconViewWork *work;
    BeaconViewPanel *panels[BEACON_VIEW_LINE_MAX];
    int seq;
    u8 top;
    u8 count;
    int printing;
    int *pending;
} BeaconViewJumpTask;

typedef struct {
    u8 seq;
    u8 timer;
    int printing;
    BeaconViewWork *work;
    int *pending;
} BeaconViewMessageTask;

typedef struct {
    u8 seq;
    u8 unk1;
    u8 timer;
    u8 record;
    u8 unk4;
    u8 button;
    int printing;
    u32 passPower;
    u16 itemCount;
    u16 price;
    // The time left of the Pass Power in use, in seconds, and as minutes and seconds
    u16 time;
    u16 minutes;
    u16 seconds;
    BeaconViewWork *work;
    int *pending;
} BeaconViewPassPowerTask;

typedef struct {
    u8 seq;
    u8 unk1;
    u8 timer;
    u8 unk3[2];
    u8 button;
    int printing;
    FestMissionConfig config;
    u32 unk38[4];
    BeaconViewWork *work;
    int *pending;
} BeaconViewMissionTask;

typedef struct {
    u8 seq;
    u8 timer;
    u32 unk4;
    BeaconViewPanel *panel;
    int type;
    BeaconViewWork *work;
    u32 unk14;
} BeaconViewIconTask;

typedef struct {
    u8 seq;
    u8 frame;
    u8 frames;
    u8 offset;
    u32 unk4;
    BeaconViewPanel *panel;
    BeaconViewWork *work;
    int *pending;
} BeaconViewFlashTask;

// Sends a window's characters to VRAM once its text is printed
typedef struct {
    u32 unk0;
    BeaconViewWin *win;
    BeaconViewWork *work;
    int *pending;
} BeaconViewFlushTask;

typedef struct {
    BeaconViewPanel *panel;
    BeaconViewPanelOam *oam;
    BeaconViewWork *work;
    int *pending;
} BeaconViewPrintTask;

BOOL func_ov032_021f0820(u32 number);
static void func_ov032_021f0a98(BeaconViewWork *work, u8 messageId);
// The arrows' outlines, each cut into three triangles by the corners they take
static const u8 data_ov032_021f3538[3][3] = { { 0, 1, 4 }, { 1, 2, 4 }, { 2, 3, 4 } };
static const BeaconViewTouch data_ov032_021f356c[2][5] = {
#include "asset/beacon_view_sub_data_ov032_021f356c.inc"
};
static const u8 data_ov032_021f3541[19] = {
#include "asset/beacon_view_sub_data_ov032_021f3541.inc"
};
static const u32 data_ov032_021f3554[6] = { 0x21, 0x23, 0x22, 0x24, 0x25, 0x26 };
void func_ov032_021f0fc8(BeaconViewWork *work, BeaconViewPanel *panel, int view);
u32 func_ov032_021f0fe8(const GameBeacon *beacon);
void func_ov032_021f10d8(BeaconViewWork *work, BeaconViewPanel *panel, const GameBeacon *beacon);
static void func_ov032_021f1704(TCBEx *task, void *data);
static void func_ov032_021f178c(BeaconViewWork *work, u8 dir, int *pending);
static void func_ov032_021f17d8(TCBEx *task, void *data);
static void func_ov032_021f1888(BeaconViewWork *work, const GameBeacon *beacon);
void func_ov032_021f18e0(BeaconViewWork *work, const StrBuf *strbuf, int *pending);
void func_ov032_021f1930(TCBEx *task, void *data);
BOOL func_ov032_021f199c(BeaconViewWork *work, const GameBeacon *beacon, BOOL gift);
void func_ov032_021f1a28(BeaconViewWork *work, u32 passPower, u8 record, int *pending);
void func_ov032_021f1b94(TCBEx *task, void *data);
static void func_ov032_021f1cf0(TCBEx *task, void *data);
void func_ov032_021f1eac(TCBEx *task, void *data);
void func_ov032_021f1f34(BeaconViewWork *work, BeaconViewPanel *panel, int *pending);
void func_ov032_021f1f9c(TCBEx *task, void *data);
static void func_ov032_021f201c(BeaconViewWork *work, BeaconViewWin *win, const StrBuf *strbuf, s16 x, u8 y, u8 fill, u16 color, int *pending);
static void func_ov032_021f20a8(BeaconViewWork *work, BeaconViewWin *win, int *pending);
void func_ov032_021f20e4(TCBEx *task, void *data);
void func_ov032_021f2144(BeaconViewWork *work, BeaconViewPanel *panel, const StrBuf *strbuf, u16 color, int *pending);
void func_ov032_021f1418(TCBEx *task, void *data);
void func_ov032_021f1484(BeaconViewWork *work, u8 dir, BeaconViewPanel *newPanel, BeaconViewPanel *skip, int *pending);
void func_ov032_021f150c(BeaconViewWork *work, BeaconViewPanel *panel, int *pending);
static void func_ov032_021f21b0(BeaconViewWork *work, BeaconViewPanel *panel, int *pending);
void func_ov032_021f21f8(TCBEx *task, void *data);

// The message of the offer of a Pass Power, by what offers it
static const u8 data_ov032_021f352c[] = {
    GameBeacon_Text_004,
    GameBeacon_Text_005,
    6,
};

// pokebw2's sBeaconViewButtonSounds
static const u16 data_ov032_021f3530[] = {
    SEQ_SE_SYS_79,
    SEQ_SE_SYS_79,
    SEQ_SE_SYS_79,
    SEQ_SE_CANCEL1,
};


static const u16 sBeaconViewDigitDivisors[] = { 1, 10, 100, 1000 };

static const TouchRect sBeaconViewArrowRects[] = {
    { 24, 40, 232, 248 },
    { 144, 160, 232, 248 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

// The palette of each trainer sprite
static const u8 sBeaconViewTrainerPalettes[BEACON_VIEW_TRAINER_CHARS] = {
    2, 3, 6, 5, 4, 5, 2, 0, 1, 3, 6, 5, 4, 7, 7, 0,
};

// The Pass Power that each record gives
static const u32 sBeaconViewRecordPassPowers[] = { 0x21, 0x23, 0x22, 0x24, 0x25, 0x26 };

// Whether a number of three digits or more has the same digit all through
// pokebw2's BeaconView_IsRepdigit


void func_ov032_021f06e0(BeaconViewWork *work, const GameBeacon *beacon, u8 oldCount) {
    u8 versionCount = 0;
    u8 genderCount = 0;

    func_ov032_021f0684(work, &genderCount, &versionCount);
    if (func_ov010_02165c50(beacon) != 2) {
        func_ov032_021f079c(work, 1);
    }
    if (work->otherVersionCount != versionCount && versionCount == 20) {
        func_ov032_021f079c(work, 2);
    }
    if (work->passerbyCount % 100 == 0) {
        func_ov032_021f079c(work, 3);
    }
    if (oldCount == GAME_BEACON_LOG_MAX - 1 && work->logCount == GAME_BEACON_LOG_MAX) {
        func_ov032_021f079c(work, 4);
    }
    if (work->otherGenderCount != genderCount && genderCount == 20) {
        func_ov032_021f079c(work, 5);
    }
    if (func_ov032_021f0820(work->passerbyCount)) {
        func_ov032_021f079c(work, 6);
    }
    work->otherGenderCount = genderCount;
    work->otherVersionCount = versionCount;
}

void func_ov032_021f079c(BeaconViewWork *work, u8 record) {
    int i;

    for (i = 0; i < work->recordCount; i++) {
        if (record == work->records[i]) {
            return;
        }
    }
    work->records[work->recordCount++] = record;
}

u32 func_ov032_021f07d4(BeaconViewWork *work) {
    u8 record = work->records[0];
    u8 count;

    if (record == 0) {
        return 0x30;
    }
    count = work->recordCount;
    work->recordCount = count - 1;
    MI_CpuCopy8(&work->records[1], work->records, work->recordCount);
    work->records[work->recordCount] = 0;
    return data_ov032_021f3554[record - 1];
}

BOOL func_ov032_021f0820(u32 number) {
    u32 digit;

    if (number / 100 == 0) {
        return FALSE;
    }
    digit = number % 10;
    number /= 10;
    do {
        u8 next = number % 10;

        if (next != digit) {
            return FALSE;
        }
        number /= 10;
    } while (number != 0);
    return TRUE;
}

s32 func_ov032_021f086c(s32 *param_a, s32 *param_b) {
    return param_a[0] * param_b[1] - param_a[1] * param_b[0];
}

void func_ov032_021f0884(s32 *param_a, s32 *param_b, s32 *param_out) {
    param_out[0] = param_a[0] - param_b[0];
    param_out[1] = param_a[1] - param_b[1];
}

BOOL func_ov032_021f089c(s32 *point, s32 *polygon) {
    s32 i;
    s32 diff[2];
    s32 edge[2];

    for (i = 0; i < 3; i++) {
        func_ov032_021f0884(point, &polygon[i * 2], diff);
        func_ov032_021f0884(&polygon[((i + 1) % 3) * 2], &polygon[i * 2], edge);
        if (func_ov032_021f086c(diff, edge) < 0) {
            return FALSE;
        }
    }
    return TRUE;
}

u32 func_ov032_021f08e4(BeaconViewWork *work, BeaconViewTouch *touch) {
    s32 tri[6];
    int i;
    int x;
    int y;

    for (i = 0; i < work->viewCount; i++) {
        x = i * 13;
        y = i * 40;
        tri[0] = x;
        tri[1] = y + 5;
        tri[2] = x + 12;
        tri[3] = y + 43;
        tri[4] = x + 210;
        tri[5] = y + 43;
        if (func_ov032_021f089c((s32 *)touch, tri)) {
            return i;
        }
        tri[0] = x;
        tri[1] = y + 5;
        tri[2] = x + 210;
        tri[3] = y + 43;
        tri[4] = x + 198;
        tri[5] = y + 5;
        if (func_ov032_021f089c((s32 *)touch, tri)) {
            return i;
        }
    }
    return -1;
}

int func_ov032_021f096c(BeaconViewWork *work, BeaconViewTouch *touch) {
    u32 shown = 0;
    BeaconViewTouch tri[3];
    int i;
    int j;

    if (work->top != 0) {
        shown |= 1;
    }
    if (work->top + work->viewCount < work->logCount) {
        shown |= 2;
    }
    for (i = 0; i < 2; i++) {
        if (shown & (i + 1)) {
            for (j = 0; j < 3; j++) {
                tri[0] = data_ov032_021f356c[i][data_ov032_021f3538[j][0]];
                tri[1] = data_ov032_021f356c[i][data_ov032_021f3538[j][1]];
                tri[2] = data_ov032_021f356c[i][data_ov032_021f3538[j][2]];
                if (func_ov032_021f089c((s32 *)touch, (s32 *)tri)) {
                    return 1 - i;
                }
            }
        }
    }
    return -1;
}

int func_ov032_021f0a18(BeaconViewWork *work, BeaconViewTouch *touch) {
    int i;
    TouchRect rects[2];

    rects[1].top = TOUCH_RECT_END;
    for (i = 0; i < BEACON_VIEW_ACTOR_UP; i++) {
        u8 state = func_0204b898(work->actors[i]) - (i * 3 + 1);

        if (state % 3 < 2) {
            rects[0].top = 172;
            rects[0].bottom = rects[0].top + 24;
            rects[0].left = i * 32 + 132;
            rects[0].right = rects[0].left + 24;
            if (func_02035d40(rects, touch->x, touch->y) != TOUCH_RECT_NONE) {
                func_ov032_021f0dd8(data_ov032_021f3530[i]);
                return i;
            }
        }
    }
    return TOUCH_RECT_NONE;
}


// pokebw2's BeaconView_ExpandMessage
static void func_ov032_021f0a98(BeaconViewWork *work, u8 messageId) {
    GFL_MsgDataLoadStrbuf(work->msgData, messageId, work->strTemplate);
    GFL_WordSetFormatStrbuf(work->wordSet, work->strExpand, work->strTemplate);
}

void func_ov032_021f0ac0(BeaconViewWork *work) {
    WordSetNumber(work->wordSet, 0, work->logCount, 2, 1, TRUE);
    func_ov032_021f0a98(work, 0);
    func_ov032_021f201c(work, &work->wins[2].win, work->strExpand, 4, 0, 1, 0x3dc1, NULL);
}


void func_ov032_021f0b10(BeaconViewWork *work, u8 messageId, int *pending) {
    WordSetNumber(work->wordSet, 0, work->passerbyCount, 5, 0, TRUE);
    func_ov032_021f0a98(work, messageId);
    func_ov032_021f201c(work, &work->wins[1].win, work->strExpand, 0, 4, 0, 0x3c40, pending);
}


void func_ov032_021f0b64(BeaconViewWork *work) {
    GFL_BitmapFill(work->wins[1].win.bitmap, 0x40);
    BmpWin_FlushChar(work->wins[1].win.window);
}

void func_ov032_021f0b80(BeaconViewWork *work, const StrBuf *strbuf, int *pending) {
    func_ov032_021f201c(work, &work->wins[0].win, strbuf, 0, 0, 9, 0x449, pending);
}

u32 func_ov032_021f0ba8(u8 *counter) {
    u16 colors[1];
    u32 px;
    u32 py;
    u8 n;

    colors[0] = 0xfffe;
    n = *counter;
    *counter = n - 1;
    if (n == 0) {
        *counter = 0;
        return TRUE;
    }
    if (func_02035d20(&px, &py) != FALSE) {
        if (func_02041ff4(6, px, py + 0x40, colors) != FALSE) {
            *counter = 0;
            return TRUE;
        }
    }
    return FALSE;
}

void func_ov032_021f0c04(BeaconViewWork *work, int type) {
    int count;
    int x;
    int i;
    BeaconViewButton *buttons;

    if (type == 0) {
        buttons = work->buttons;
        count = 3;
        x = 8;
    } else {
        buttons = work->buttons2;
        count = 2;
        x = 16;
    }
    for (i = 0; i < count; i++) {
        buttons[i].window = func_0202800c(work->taskMenuRes, &buttons[i].item, x + i * 8, 0x15, 8, 3, 1, 0, work->heapId);
    }
}

int func_ov032_021f0c70(BeaconViewWork *work, int type) {
    int result = -1;
    BeaconViewButton *buttons;
    int count;
    int i;

    if (type == 0) {
        buttons = work->buttons;
        count = 3;
    } else {
        buttons = work->buttons2;
        count = 2;
    }
    for (i = 0; i < count; i++) {
        if (func_0202846c(buttons[i].window) == TRUE) {
            func_02028178(buttons[i].window);
            return -1;
        }
    }
    for (i = 0; i < count; i++) {
        if (func_0202824c(buttons[i].window)) {
            if (i < count - 1) {
                func_ov032_021f0dd8(0x54c);
            } else {
                // The last button stays lit
                func_0202822c(buttons[i].window, TRUE);
                func_ov032_021f0dd8(0x54c);
            }
            result = i;
        }
        func_02028178(buttons[i].window);
    }
    return result;
}


BOOL func_ov032_021f0cf8(BeaconViewWork *work, u8 index) {
    int i;

    func_02028178(work->buttons[index].window);
    if (func_02028230(work->buttons[index].window)) {
        for (i = 0; i < 3; i++) {
            func_02028148(work->buttons[i].window);
            work->buttons[i].window = NULL;
        }
        return TRUE;
    }
    return FALSE;
}


void func_ov032_021f0d38(BeaconViewWork *work) {
    int i;

    for (i = 0; i < 3; i++) {
        func_02028148(work->buttons[i].window);
        work->buttons[i].window = NULL;
    }
}

BOOL func_ov032_021f0d5c(BeaconViewWork *work, u8 index) {
    int i;

    func_02028178(work->buttons2[index].window);
    if (func_02028230(work->buttons2[index].window)) {
        for (i = 0; i < 2; i++) {
            func_02028148(work->buttons2[i].window);
            work->buttons2[i].window = NULL;
        }
        return TRUE;
    }
    return FALSE;
}


void func_ov032_021f0d9c(BeaconViewWork *work) {
    int i;

    for (i = 0; i < 2; i++) {
        func_02028148(work->buttons2[i].window);
        work->buttons2[i].window = NULL;
    }
}


s32 func_ov032_021f0dc0(GameBeacon *beacon) {
    s32 n = func_ov010_02165e24(beacon);

    if (n == 0 || n >= 0x33) {
        return 0x12;
    }
    return n + 0x11;
}


void func_ov032_021f0dd8(u32 se) {
    GFL_SndSEPlay(se);
}

void func_ov032_021f0de0(ClActor *actor, s16 x, s16 y) {
    ClActorPos pos;

    pos.x = x;
    pos.y = y;
    func_0204b404(actor, &pos, 0);
}

void func_ov032_021f0df4(ClActor *actor, u16 anim) {
    func_0204b878(actor, anim);
    func_0204b9a0(actor);
}

void func_ov032_021f0e04(BeaconViewWork *work, BOOL visible) {
    int i;

    for (i = 0; i <= 3; i++) {
        func_0204b3dc(work->actors[i], visible);
    }
}

void func_ov032_021f0e24(BeaconViewWork *work) {
    if (work->top == 0) {
        func_0204b878(work->actors[BEACON_VIEW_ACTOR_UP], 0x10);
    } else {
        func_0204b878(work->actors[BEACON_VIEW_ACTOR_UP], 0xd);
    }
    if (work->top + work->viewCount < work->logCount) {
        func_0204b878(work->actors[BEACON_VIEW_ACTOR_DOWN], 0x11);
        return;
    }
    func_0204b878(work->actors[BEACON_VIEW_ACTOR_DOWN], 0x14);
}


void func_ov032_021f0e7c(BeaconViewWork *work) {
    if (work->logCount == 0) {
        func_0204b878(work->actors[2], 9);
    } else {
        func_0204b878(work->actors[2], 7);
    }
}


static void func_ov032_021f0ed0(BeaconViewPanel *panel, BOOL visible);

void func_ov032_021f0ea4(BeaconViewPanel *panel, BOOL visible) {
    func_02025774(panel->oam.actor, visible);
    func_0204b3dc(panel->trainer, visible);
    func_0204b3dc(panel->base, visible);
    if (visible == FALSE) {
        func_ov032_021f0ed0(panel, visible);
    }
}


void func_ov032_021f0ed0(BeaconViewPanel *panel, BOOL visible) {
    func_0204b3dc(panel->icon, visible);
}

void func_ov032_021f0edc(BeaconViewPanel *panel) {
    func_ov032_021f0ea4(panel, FALSE);
    func_ov032_021f0ed0(panel, FALSE);
    GFL_BitmapFill(panel->oam.bitmap, 0);
    func_020257d0(panel->oam.actor);
    GFL_StrBufClear(panel->name);
    GFL_StrBufClear(panel->greeting);
    panel->dataIndex = 0xff;
    panel->line = 0;
    panel->rank = 0;
    if (panel->printTask != NULL) {
        GFL_TCBExRequestEnd(panel->printTask);
        panel->printTask = NULL;
    }
    if (panel->iconTask != NULL) {
        GFL_TCBExRequestEnd(panel->iconTask);
        panel->iconTask = NULL;
    }
}


// pokebw2's BeaconViewPanel_Assign
void func_ov032_021f0f30(BeaconViewPanel *panel, u8 dataIndex, u8 line) {
    func_ov032_021f0edc(panel);
    panel->dataIndex = dataIndex;
    panel->line = line;
}

// pokebw2's BeaconView_FindPanel
BeaconViewPanel *func_ov032_021f0f44(BeaconViewWork *work, u8 dataIndex) {
    int i;

    for (i = 0; i < BEACON_VIEW_PANEL_MAX; i++) {
        if (work->panels[i].dataIndex == dataIndex) {
            return &work->panels[i];
        }
    }
    return NULL;
}

void func_ov032_021f0f74(BeaconViewWork *work, BeaconViewPanel *panel, const GameBeacon *beacon) {
    u8 view = func_ov010_02165ce8(beacon);

    func_0204a9d0(work->res[BEACON_VIEW_RES_TRAINER].lists[BEACON_VIEW_RES_CHARS].ids[panel->id],
                  work->trainerChars[view].data);
    func_020215bc(work->paletteFade, &work->trainerPalette.colors[(u8)(data_ov032_021f3541[view] * 16)], 3,
                  (panel->id + 9) * 16, 0x20);
}


// pokebw2's BeaconViewPanel_SetTrainer
void func_ov032_021f0fc8(BeaconViewWork *work, BeaconViewPanel *panel, int view) {
    func_0204a9d0(work->res[BEACON_VIEW_RES_ICON].lists[BEACON_VIEW_RES_CHARS].ids[panel->id],
                  work->iconChars[view].data);
}

// pokebw2's BeaconViewPanel_SetIcon
u32 func_ov032_021f0fe8(const GameBeacon *beacon) {
    switch (func_ov010_02165c48(beacon)) {
    case VERSION_BLACK:
        return 0;
    case VERSION_WHITE:
        return 1;
    default:
        return 2;
    }
}

void func_ov032_021f1004(BeaconViewWork *work, BeaconViewPanel *panel, const GameBeacon *beacon) {
    u16 colors[5];
    u32 set = func_ov032_021f0fe8(beacon) * 16;

    colors[0] = work->commonPalette.colors[set];
    colors[1] = work->commonPalette.colors[set + 1];
    colors[2] = work->commonPalette.colors[set + 2];
    func_ov010_02165cc0(&colors[3], beacon);
    func_02021f64(&colors[3], &colors[4], 1, 3, 0);
    func_020215bc(work->paletteFade, colors, 3, (panel->id + 4) * 16 + 1, sizeof(colors));
}


void func_ov032_021f1070(BeaconViewWork *work, BeaconViewPanel *panel, const StrBuf *strbuf, int *pending) {
    func_ov032_021f2144(work, panel, strbuf, 0x3c60, pending);
}

void func_ov032_021f1080(BeaconViewWork *work, BeaconViewPanel *panel, BOOL force, int *pending) {
    u8 unk = func_02026d48(panel->trainerId) >= 0x2a30;

    if (unk == panel->unk5 && force == 0) {
        return;
    }
    panel->unk5 = unk;
    if (unk != 0) {
        func_ov032_021f2144(work, panel, panel->name, 0xca0, pending);
    } else {
        func_ov032_021f2144(work, panel, panel->name, 0x3c60, pending);
    }
}

void func_ov032_021f10d8(BeaconViewWork *work, BeaconViewPanel *panel, const GameBeacon *beacon) {
    u8 rank;
    u16 litOffset;

    panel->rank = func_ov010_02165cb4(beacon);
    rank = panel->rank;
    if (rank == 0) {
        litOffset = 0x30;
        rank = 5;
    } else {
        litOffset = 0x35;
    }
    func_020215bc(work->paletteFade, &work->commonPalette.colors[0x30], 3, (panel->id + 4) * 16 + 8, 5 * sizeof(u16));
    func_020215bc(work->paletteFade, &work->commonPalette.colors[litOffset], 3, (panel->id + 4) * 16 + 8,
                  rank * sizeof(u16));
}

void func_ov032_021f1144(BeaconViewWork *work, BeaconViewPanel *panel, const GameBeacon *beacon, int mode, BOOL quiet,
                         int *pending) {
    panel->trainerId = func_ov010_02165ce4(beacon);
    panel->gender = func_ov010_02165d08(beacon);
    func_ov010_02165c34(beacon, panel->name);
    GFL_StrBufClear(panel->greeting);
    func_ov010_02165d50(beacon, panel->greeting);
    func_ov032_021f10d8(work, panel, beacon);
    func_ov032_021f0f74(work, panel, beacon);
    func_ov032_021f1004(work, panel, beacon);
    if (quiet) {
        return;
    }
    switch (mode) {
    case 0:
        switch (func_ov010_02165e24(beacon)) {
        case 24:
            func_ov010_02165d6c(beacon, work->strTemplate);
            func_ov032_021f1070(work, panel, work->strTemplate, pending);
            break;
        case 49:
            func_ov010_02165d8c(beacon, work->strTemplate);
            func_ov032_021f1070(work, panel, work->strTemplate, pending);
            break;
        }
        func_ov032_021f1e40(work, panel, beacon, FALSE);
        break;
    case 1:
        func_ov032_021f1070(work, panel, panel->greeting, pending);
        func_ov032_021f1e40(work, panel, beacon, TRUE);
        break;
    case 2:
        func_ov032_021f1080(work, panel, TRUE, pending);
        break;
    }
}


void func_ov032_021f1218(BeaconViewPanel *panel, s16 x, s16 y) {
    panel->x = x;
    panel->y = y;
    func_ov032_021f0de0(panel->base, x, y);
    func_ov032_021f0de0(panel->trainer, x + 0x20, y + 0x18);
    func_ov032_021f0de0(panel->icon, x + 0x3c, y + 0x18);
    func_0202590c(panel->oam.actor, x + 0x48, y + 0x10);
}

void func_ov032_021f1268(BeaconViewWork *work) {
    int i;

    for (i = 0; i < work->viewCount; i++) {
        BeaconViewPanel *panel = func_ov032_021f0f44(work, func_ov010_021659f0(work->log, work->top + i));

        if (panel->iconTask == NULL || panel->printTask != NULL) {
            func_ov032_021f1080(work, panel, 0, 0);
        }
    }
}

void func_ov032_021f12b8(BeaconViewWork *work, int dir) {
    u8 offset;
    u8 dataIndex;

    func_ov032_021f0dd8(SEQ_SE_SELECT4);
    if (dir == 0) {
        offset = work->top + work->viewCount;
        work->top++;
    } else {
        offset = work->top - 1;
        work->top--;
    }
    func_020275b8(work->status, work->top);
    func_ov010_02165ac4(work->log, work->beacon, &work->time, offset);
    dataIndex = func_ov010_021659f0(work->log, offset);
    func_ov032_021f13a0(work, work->beacon, dataIndex, dir, 2);
}

void func_ov032_021f1340(BeaconViewWork *work, int dir, u8 returnSeq) {
    work->scrolling = TRUE;
    work->scrollEnd = FALSE;
    work->scrollDir = dir;
    work->scrollReturnSeq = returnSeq;
    work->scrollFrames = 0;
    func_ov032_021f029c(work, TRUE);
    if (work->scrollDir == 0) {
        func_ov032_021f0df4(work->actors[BEACON_VIEW_ACTOR_DOWN], 19);
    } else {
        func_ov032_021f0df4(work->actors[BEACON_VIEW_ACTOR_UP], 15);
    }
    func_ov032_021f05e8(work, BEACON_VIEW_BUTTON_ALL, 2);
    func_ov032_021f12b8(work, dir);
}


void func_ov032_021f13a0(BeaconViewWork *work, const GameBeacon *beacon, u8 dataIndex, u8 dir, int mode) {
    BeaconViewPanel *panel = func_ov032_021f0f44(work, 0xff);
    BeaconViewAddLineTask *data;

    if (panel == NULL) {
        return;
    }
    data = GFL_TCBExGetData(
        GFL_TCBExMgrAddTask(work->tcbManager, func_ov032_021f1418, sizeof(BeaconViewAddLineTask), 0));
    MI_CpuFill8(data, 0, sizeof(BeaconViewAddLineTask));
    data->work = work;
    data->dir = dir;
    data->panel = panel;
    data->mode = mode;
    data->pending = &work->pending;
    work->pending++;
    func_ov032_021f0f30(panel, dataIndex, 0);
    func_ov032_021f1144(work, panel, beacon, mode, FALSE, &data->printing);
}

void func_ov032_021f1418(TCBEx *task, void *data) {
    BeaconViewAddLineTask *add = data;
    BeaconViewWork *work = add->work;

    if (add->printing > 0) {
        return;
    }
    if (add->seq == 0) {
        func_ov032_021f0ea4(add->panel, TRUE);
        if (add->mode == 1) {
            func_ov032_021f150c(work, add->panel, &add->printing);
            func_ov032_021f0ed0(add->panel, TRUE);
        } else {
            func_ov032_021f1484(work, add->dir, add->panel, NULL, &add->printing);
        }
        add->seq++;
        return;
    }
    if (add->pending != NULL) {
        (*add->pending)--;
    }
    GFL_TCBExRequestEnd(task);
}

void func_ov032_021f1484(BeaconViewWork *work, u8 dir, BeaconViewPanel *newPanel, BeaconViewPanel *skip,
                                 int *pending) {
    int i;

    if (newPanel != NULL) {
        if (dir == 0) {
            newPanel->line = BEACON_VIEW_LINE_MAX + 1;
        } else {
            newPanel->line = 0;
        }
        func_ov032_021f1218(newPanel, PANEL_LINE_X(newPanel->line), PANEL_LINE_Y(newPanel->line));
        func_ov032_021f0ea4(newPanel, TRUE);
    }
    for (i = 0; i < BEACON_VIEW_PANEL_MAX; i++) {
        BeaconViewPanel *panel = &work->panels[i];

        if (panel->dataIndex == 0xff) {
            continue;
        }
        if (skip != NULL && panel->id == skip->id) {
            continue;
        }
        func_ov032_021f155c(work, panel, dir, 0, pending);
    }
}

// Slides a new line in from the left onto the first line, the others making room
void func_ov032_021f150c(BeaconViewWork *work, BeaconViewPanel *panel, int *pending) {
    s8 delay = 0;

    panel->line = 1;
    func_ov032_021f1218(panel, PANEL_LINE_X(1) - 208, 0);
    func_ov032_021f0ea4(panel, TRUE);
    if (work->logCount != 0) {
        delay = 8;
    }
    func_ov032_021f155c(work, panel, 2, delay, pending);
    func_ov032_021f1484(work, 1, NULL, panel, pending);
}


// Brings a log entry in on the free line: mode 1 slides it in from the left as the newest, pushing the others down;
// otherwise it comes in from below for dir 0 and from above for dir 1, as the others move by a line
// pokebw2's BeaconView_AddLine
void func_ov032_021f155c(BeaconViewWork *work, BeaconViewPanel *panel, u8 dir, u8 delay, int *pending) {
    BeaconViewMoveTask *data = GFL_TCBExGetData(
        GFL_TCBExMgrAddTask(work->tcbManager, func_ov032_021f1704, sizeof(BeaconViewMoveTask), 0));

    MI_CpuFill8(data, 0, sizeof(BeaconViewMoveTask));
    data->dir = dir;
    if (data->dir == 0) {
        panel->line--;
    } else if (data->dir == 1) {
        panel->line++;
    }
    data->work = work;
    data->panel = panel;
    data->destX = PANEL_LINE_X(panel->line);
    data->destY = PANEL_LINE_Y(panel->line);
    data->x = FX32_CONST(panel->x);
    data->y = FX32_CONST(panel->y);
    data->frames = 8;
    data->delay = delay;
    data->stepX = FX_Div(FX32_CONST(data->destX - panel->x), FX32_CONST(data->frames));
    data->stepY = FX_Div(FX32_CONST(data->destY - panel->y), FX32_CONST(data->frames));
    data->pending = pending;
    (*data->pending)++;
}

// pokebw2's BeaconView_AddLineTask
static void func_ov032_021f1704(TCBEx *task, void *data) {
    BeaconViewMoveTask *move = data;

    switch (move->seq) {
    case 0:
        if (move->delay != 0) {
            move->delay--;
            return;
        }
        move->frame++;
        if (move->frame < move->frames) {
            move->x += move->stepX;
            move->y += move->stepY;
            func_ov032_021f1218(move->panel, move->x >> FX32_SHIFT, move->y >> FX32_SHIFT);
            return;
        }
        func_ov032_021f1218(move->panel, move->destX, move->destY);
        if (move->panel->line == 0 || move->panel->line == BEACON_VIEW_LINE_MAX + 1) {
            move->seq++;
            return;
        }
        break;
    case 1:
    default:
        if (move->panel->printTask != NULL) {
            return;
        }
        func_ov032_021f0edc(move->panel);
        break;
    }
    (*move->pending)--;
    GFL_TCBExRequestEnd(task);
}

// Scrolls the top screen's background in or out over a few frames
static void func_ov032_021f178c(BeaconViewWork *work, u8 dir, int *pending) {
    BeaconViewBgScrollTask *data = GFL_TCBExGetData(
        GFL_TCBExMgrAddTask(work->tcbManager, func_ov032_021f17d8, sizeof(BeaconViewBgScrollTask), 0));

    MI_CpuFill8(data, 0, sizeof(BeaconViewBgScrollTask));
    data->dir = dir;
    if (data->dir == 0) {
        data->pos = 0;
        data->step = 16;
    } else {
        data->pos = 64;
        data->step = -16;
    }
    data->frames = 4;
    data->pending = pending;
    (*pending)++;
}

static void func_ov032_021f17d8(TCBEx *task, void *data) {
    BeaconViewBgScrollTask *scroll = data;

    if (scroll->frame++ < scroll->frames) {
        scroll->pos += scroll->step;
        GFL_BGSysMoveBG(6, BG_MOVE_SET_Y, scroll->pos);
        return;
    }
    (*scroll->pending)--;
    GFL_TCBExRequestEnd(task);
}

// Moves a line to where it belongs over some frames, after going up a line for dir 0 or down a line for dir 1. A line
// that leaves the screen is freed
BOOL func_ov032_021f180c(BeaconViewWork *work, const GameBeacon *beacon, BOOL gift) {
    u16 type = func_ov010_02165e24(beacon);

    if (gift) {
        func_ov010_02165fcc(beacon, work->wordSet, work->tmpHeapId);
        func_ov032_021f18c4(work, 0x12);
        return TRUE;
    }
    switch (type) {
    case 1:
        return FALSE;
    case 0x12:
        return func_ov032_021f199c(work, beacon, FALSE);
    case 0x32:
        return func_ov032_021f199c(work, beacon, TRUE);
    case 0x18:
        func_ov010_02165374(func_0200c1c8(work->trainerCard));
        break;
    }
    func_ov032_021f1888(work, beacon);
    return TRUE;
}

static void func_ov032_021f1888(BeaconViewWork *work, const GameBeacon *beacon) {
    func_ov010_02165fcc(beacon, work->wordSet, work->tmpHeapId);
    func_ov032_021f0a98(work, func_ov032_021f0dc0(beacon));
    func_ov032_021f18e0(work, work->strExpand, &work->pending);
}

void func_ov032_021f18c4(BeaconViewWork *work, u8 top) {
    func_ov032_021f0a98(work, top);
    func_ov032_021f18e0(work, work->strExpand, &work->pending);
}

void func_ov032_021f18e0(BeaconViewWork *work, const StrBuf *strbuf, int *pending) {
    BeaconViewMessageTask *data = GFL_TCBExGetData(
        GFL_TCBExMgrAddTask(work->tcbManager, func_ov032_021f1930, sizeof(BeaconViewMessageTask), 0));

    MI_CpuFill8(data, 0, sizeof(BeaconViewMessageTask));
    GFL_StrBufCopy(work->strMessage, strbuf);
    data->work = work;
    data->timer = 90;
    func_ov032_021f05e8(work, BEACON_VIEW_BUTTON_ALL, 2);
    data->pending = pending;
    (*pending)++;
}

void func_ov032_021f1930(TCBEx *task, void *data) {
    BeaconViewMessageTask *message = data;

    if (message->printing != 0) {
        return;
    }
    switch (message->seq) {
    case 0:
        func_ov032_021f0b80(message->work, message->work->strMessage, &message->printing);
        message->seq++;
        break;
    case 1:
        func_ov032_021f178c(message->work, 0, &message->printing);
        message->seq++;
        break;
    case 2:
        if (func_ov032_021f0ba8(&message->timer)) {
            func_ov032_021f178c(message->work, 1, &message->printing);
            message->seq++;
        }
        break;
    default:
        (*message->pending)--;
        GFL_TCBExRequestEnd(task);
        break;
    }
}

BOOL func_ov032_021f199c(BeaconViewWork *work, const GameBeacon *beacon, BOOL gift) {
    int passPower;

    if (gift) {
        passPower = func_ov010_02165ebc(beacon);
    } else {
        passPower = func_ov010_02165d20(beacon);
    }
    if (passPower == 0x30 || passPower > 0x2d) {
        return FALSE;
    }
    if (passPower == PassPower_GetUsedIDByPassPower(passPower, work->passPowerData)) {
        return FALSE;
    }
    func_ov010_02165fcc(beacon, work->wordSet, work->tmpHeapId);
    GFL_WordSetFormatStrbuf(work->wordSet, work->strExpand, work->strTemplate);
    func_ov032_021f1a28(work, passPower, 1, &work->pending);
    return TRUE;
}


// pokebw2's BeaconView_ShowMessage
void func_ov032_021f1a08(BeaconViewWork *work) {
    func_ov032_021f1a28(work, work->unk_28, 0, &work->pending);
}

void func_ov032_021f1a18(BeaconViewWork *work, u32 passPower) {
    func_ov032_021f1a28(work, passPower, 2, &work->pending);
}

void func_ov032_021f1a28(BeaconViewWork *work, u32 passPower, u8 record, int *pending) {
    TCBEx *task = GFL_TCBExMgrAddTask(work->tcbManager, func_ov032_021f1b94, sizeof(BeaconViewPassPowerTask), 0);
    BOOL noRecord = FALSE;
    BeaconViewPassPowerTask *data = GFL_TCBExGetData(task);

    MI_CpuFill8(data, 0, sizeof(BeaconViewPassPowerTask));
    work->selectedPanel = 0x30;
    if (record == 0) {
        noRecord = TRUE;
    }
    work->barDragging = noRecord;
    data->work = work;
    data->timer = 90;
    data->passPower = passPower;
    data->record = record;
    loadPassPowerToStrbuf(work->wordSet, 1, passPower);
    if (data->record == 0) {
        data->price = PassPowerData_GetPrice(work->passPowerData, passPower);
        data->itemCount = BagSave_GetItemCountByID(work->bag, ITEM_PASS_ORB, work->tmpHeapId);
        WordSetNumber(work->wordSet, 2, data->price, 3, 0, TRUE);
        WordSetNumber(work->wordSet, 3, data->itemCount, 3, 0, TRUE);
        if (passPower == PassPower_GetUsedIDByPassPower(passPower, work->passPowerData)) {
            data->time = func_02010eec();
            if (data->time != 0) {
                data->minutes = data->time / 60;
                data->seconds = data->time % 60;
                WordSetNumber(work->wordSet, 4, data->minutes, 3, 0, TRUE);
                WordSetNumber(work->wordSet, 5, data->seconds, 2, 0, TRUE);
            }
        }
    }
    if (data->record) {
        func_ov032_021f05e8(work, 0, 2);
    }
    func_ov032_021f05e8(work, 1, 2);
    func_ov032_021f05e8(work, 2, 2);
    func_ov032_021f05e8(work, 3, 2);
    if (data->time != 0 || data->itemCount < data->price) {
        func_02031288(task, func_ov032_021f1cf0);
    }
    data->pending = pending;
    (*pending)++;
}

void func_ov032_021f1b94(TCBEx *task, void *data) {
    BeaconViewPassPowerTask *offer = data;
    BeaconViewWork *work = offer->work;
    int button;

    if (offer->printing > 0) {
        return;
    }
    switch (offer->seq) {
    case 0:
        func_ov032_021f0a98(work, data_ov032_021f352c[offer->record]);
        func_ov032_021f0b80(work, work->strExpand, &offer->printing);
        offer->seq++;
        break;
    case 1:
        func_ov032_021f178c(work, 0, &offer->printing);
        offer->seq++;
        break;
    case 2:
        func_ov032_021f029c(work, TRUE);
        func_ov032_021f0c04(work, 0);
        func_ov032_021f0b64(work);
        func_ov032_021f0e04(work, FALSE);
        offer->seq++;
        break;
    case 3:
        button = func_ov032_021f0c70(work, 0);
        if (button < 0) {
            return;
        }
        offer->button = button;
        switch (offer->button) {
        case 0:
            func_ov032_021f253c(work, BEACON_VIEW_EVENT_PASS_POWER);
            offer->seq = 3;
            break;
        case 1:
            work->selectedPanel = offer->passPower;
            func_ov032_021f253c(work, BEACON_VIEW_EVENT_MEDAL);
            offer->seq = 4;
            break;
        case 2:
            offer->seq = 5;
            break;
        }
        break;
    case 4:
        if (work->selectedPanel != 0x30) {
            work->selectedPanel = 0x30;
            offer->seq = 3;
        } else {
            func_ov032_021f0d38(work);
            offer->seq = 6;
        }
        break;
    case 5:
        if (func_ov032_021f0cf8(work, offer->button)) {
            offer->seq++;
        }
        break;
    case 6:
        func_ov032_021f0b10(work, 1, &offer->printing);
        func_ov032_021f0e04(work, TRUE);
        func_ov032_021f178c(work, 1, &offer->printing);
        offer->seq++;
        break;
    default:
        func_ov032_021f05e8(work, BEACON_VIEW_BUTTON_ALL, 0);
        func_ov032_021f029c(work, FALSE);
        (*offer->pending)--;
        GFL_TCBExRequestEnd(task);
        break;
    }
}


// pokebw2's BeaconView_OfferRecordPassPower
static void func_ov032_021f1cf0(TCBEx *task, void *data) {
    BeaconViewPassPowerTask *offer = data;
    BeaconViewWork *work = offer->work;
    int button;

    if (offer->printing > 0) {
        return;
    }
    switch (offer->seq) {
    case 0:
        if (offer->time != 0) {
            func_ov032_021f0a98(work, 9 + (offer->minutes != 0));
        } else if (offer->itemCount < offer->price) {
            func_ov032_021f0a98(work, 8);
        }
        func_ov032_021f0b80(work, work->strExpand, &offer->printing);
        offer->seq++;
        break;
    case 1:
        func_ov032_021f178c(work, 0, &offer->printing);
        offer->seq++;
        break;
    case 2:
        func_ov032_021f0e04(work, FALSE);
        offer->seq++;
        break;
    case 3:
        func_ov032_021f0c04(work, 1);
        func_ov032_021f0b64(work);
        offer->seq++;
        break;
    case 4:
        button = func_ov032_021f0c70(work, 1);
        if (button < 0) {
            return;
        }
        offer->button = button;
        if (offer->button == 0) {
            func_ov032_021f253c(work, BEACON_VIEW_EVENT_PASS_POWER_USE);
            work->eventWait = TRUE;
            offer->seq = 5;
        } else {
            offer->seq = 6;
        }
        break;
    case 5:
        if (work->eventWait) {
            offer->seq = 4;
        } else {
            func_ov032_021f0d9c(work);
            offer->seq = 7;
        }
        break;
    case 6:
        if (func_ov032_021f0d5c(work, offer->button)) {
            offer->seq++;
        }
        break;
    case 7:
        func_ov032_021f0b10(work, 1, &offer->printing);
        func_ov032_021f0e04(work, TRUE);
        func_ov032_021f178c(work, 1, &offer->printing);
        offer->seq++;
        break;
    default:
        func_ov032_021f05e8(work, BEACON_VIEW_BUTTON_ALL, 0);
        (*offer->pending)--;
        GFL_TCBExRequestEnd(task);
        break;
    }
}

void func_ov032_021f1e40(BeaconViewWork *work, BeaconViewPanel *panel, const GameBeacon *beacon, BOOL greeting) {
    TCBEx *task = GFL_TCBExMgrAddTask(work->tcbManager, func_ov032_021f1eac, sizeof(BeaconViewIconTask), 0);
    BeaconViewIconTask *data = GFL_TCBExGetData(task);

    MI_CpuFill8(data, 0, sizeof(BeaconViewIconTask));
    data->work = work;
    data->panel = panel;
    data->timer = 90;
    panel->iconTask = task;
    if (greeting) {
        data->type = 1;
    } else {
        data->type = func_ov010_02165e24(beacon);
    }
    func_ov032_021f0fc8(work, panel, func_ov010_02165fc0(data->type));
    func_ov032_021f0ed0(panel, greeting ? FALSE : TRUE);
}


void func_ov032_021f1eac(TCBEx *task, void *data) {
    BeaconViewIconTask *icon = data;

    if (icon->seq == 0) {
        if (icon->timer-- != 0) {
            return;
        }
        if (icon->type == 1 || icon->type == 24 || icon->type == 49) {
            func_ov032_021f1080(icon->work, icon->panel, TRUE, NULL);
            icon->seq++;
            return;
        }
    } else if (icon->panel->printTask != NULL) {
        return;
    }
    icon->panel->iconTask = NULL;
    func_ov032_021f0ed0(icon->panel, FALSE);
    GFL_TCBExRequestEnd(task);
}


void func_ov032_021f1f00(BeaconViewWork *work, u8 dataIndex) {
    u8 kind = func_ov010_021659f0(work->log, dataIndex + work->top);
    BeaconViewPanel *panel = func_ov032_021f0f44(work, kind);

    func_ov032_021f1f34(work, panel, &work->pending);
    func_ov032_021f0dd8(0x54c);
}

void func_ov032_021f1f34(BeaconViewWork *work, BeaconViewPanel *panel, int *pending) {
    BeaconViewFlashTask *data = GFL_TCBExGetData(
        GFL_TCBExMgrAddTask(work->tcbManager, func_ov032_021f1f9c, sizeof(BeaconViewFlashTask), 0));

    MI_CpuFill8(data, 0, sizeof(BeaconViewFlashTask));
    data->work = work;
    data->panel = panel;
    data->frames = 2;
    data->offset = (panel->id + 4) * 16;
    func_02021fd8(data->work->paletteFade, 3, data->offset, 16, 4, 0x7fff);
    if (pending != NULL) {
        data->pending = pending;
        (*data->pending)++;
    }
}

void func_ov032_021f1f9c(TCBEx *task, void *data) {
    BeaconViewFlashTask *flash = data;

    if (flash->seq < 2) {
        if (flash->frame++ < flash->frames) {
            return;
        }
        func_02021fd8(flash->work->paletteFade, 3, flash->offset, 16, (flash->seq % 2) * 4, 0x7fff);
        flash->seq++;
        flash->frame = 0;
        return;
    }
    func_02021fd8(flash->work->paletteFade, 3, flash->offset, 16, 0, 0x7fff);
    if (flash->pending != NULL) {
        (*flash->pending)--;
    }
    GFL_TCBExRequestEnd(task);
}


static void func_ov032_021f201c(BeaconViewWork *work, BeaconViewWin *win, const StrBuf *strbuf, s16 x, u8 y, u8 fill,
                                 u16 color, int *pending) {
    GFL_BitmapFill(win->bitmap, fill);
    if (work->printNow) {
        BmpWin *window;

        GFL_TextRendererDrawToBitmapEx(win->bitmap, x, y, strbuf, work->font, color);
        window = win->window;
        BmpWin_FlushChar(window);
        BmpWin_FlushMap(window);
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
    } else {
        PrintWindow_PrintColor(&win->printWindow, work->printQueue, x, y, strbuf, work->font, color);
        func_ov032_021f20a8(work, win, pending);
    }
}


static void func_ov032_021f20a8(BeaconViewWork *work, BeaconViewWin *win, int *pending) {
    BeaconViewFlushTask *data = GFL_TCBExGetData(
        GFL_TCBExMgrAddTask(work->tcbManager, func_ov032_021f20e4, sizeof(BeaconViewFlushTask), 0));

    MI_CpuFill8(data, 0, sizeof(BeaconViewFlushTask));
    data->work = work;
    data->win = win;
    if (pending != NULL) {
        data->pending = pending;
        (*pending)++;
    }
}

void func_ov032_021f20e4(TCBEx *task, void *data) {
    BeaconViewFlushTask *flush = data;
    PrintQueue *queue = flush->work->printQueue;
    PrintWindow *printWindow = &flush->win->printWindow;

    PrintWindow_Flush(printWindow, queue);
    if (PrintWindow_IsPrinted(printWindow)) {
        BmpWin_FlushMap(flush->win->window);
        GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(flush->win->window));
        if (flush->pending != NULL) {
            (*flush->pending)--;
        }
        GFL_TCBExRequestEnd(task);
    }
}


// pokebw2's BeaconViewPanel_StartFlash
void func_ov032_021f2144(BeaconViewWork *work, BeaconViewPanel *panel, const StrBuf *strbuf, u16 color, int *pending) {
    BeaconViewPanelOam *oam = &panel->oam;

    GFL_BitmapFill(oam->bitmap, 0);
    if (work->printNow) {
        GFL_TextRendererDrawToBitmapEx(oam->bitmap, 0, 0, strbuf, work->font, color);
        func_020257d0(oam->actor);
    } else {
        func_0201c724(work->printQueue, oam->bitmap, 0, 0, strbuf, work->font, color);
        func_ov032_021f21b0(work, panel, pending);
    }
}

// pokebw2's BeaconViewPanel_FlashTask
static void func_ov032_021f21b0(BeaconViewWork *work, BeaconViewPanel *panel, int *pending) {
    TCBEx *task = GFL_TCBExMgrAddTask(work->tcbManager, func_ov032_021f21f8, sizeof(BeaconViewPrintTask), 0);
    BeaconViewPrintTask *data = GFL_TCBExGetData(task);

    MI_CpuFill8(data, 0, sizeof(BeaconViewPrintTask));
    data->work = work;
    data->panel = panel;
    data->oam = &panel->oam;
    panel->printTask = task;
    if (pending != NULL) {
        data->pending = pending;
        (*pending)++;
    }
}

void func_ov032_021f21f8(TCBEx *task, void *data) {
    BeaconViewPrintTask *print = data;

    if (!func_0201c6c4(print->work->printQueue, print->oam->bitmap)) {
        func_020257d0(print->oam->actor);
        if (print->pending != NULL) {
            (*print->pending)--;
        }
        print->panel->printTask = NULL;
        GFL_TCBExRequestEnd(task);
    }
}
