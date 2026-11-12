// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov257/ctvt_call.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/comm_tvt/ctvt_call.h"
#include "types.h"
#include "app/comm_tvt.h"
#include "app/comm_tvt/comm_tvt_sys.h"
#include "app/comm_tvt/ctvt_camera.h"
#include "app/comm_tvt/ctvt_comm.h"
#include "constants/sound.h"
#include "constants/version.h"
#include "field/zone.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/net_whpipe.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "save/player_info.h"
#include "save/wifi_list.h"
#include "system/app_taskmenu.h"
#include "system/bmp_winframe.h"
#include "system/game_beacon.h"
#include "system/ctvt_beacon.h"
#include "system/game_data.h"
#include "system/printsys.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "nitro/os.h"

// The Xtransceiver's calls. The bottom screen lists the machines nearby whose players are friends: Xtransceivers in a
// call, which can be joined while it has room, and games in the field, up to three of which can be called. Then it
// waits for the call to connect, ringing while it calls

#define CTVT_CALL_ROWS 15
#define CTVT_CALL_ENTRIES 15
#define CTVT_CALL_INVITES 3
#define CTVT_CALL_NONE 0xff

// The rows the list shows at once, and their height in pixels
#define CTVT_CALL_VISIBLE_ROWS 5
#define CTVT_CALL_ROW_HEIGHT 32
// How far the scroll bar moves, in pixels
#define CTVT_CALL_SCROLL_BAR_RANGE 120

// The members a call can have
#define CTVT_CALL_MEMBERS_MAX 4

// The GFL game service IDs of the beacons listed: a game in the field, and an Xtransceiver
#define CTVT_CALL_SERVICE_FIELD 3
#define CTVT_CALL_SERVICE_CTVT 0x20

// How long to wait for a call to connect, and how often it rings, in frames
#define CTVT_CALL_WAIT_FRAMES 1200
#define CTVT_CALL_RING_FRAMES 120 // Black's
// How long the notice about the photos stays up
#define CTVT_CALL_PHOTO_NOTICE_FRAMES 300

enum {
    CALL_STATE_FADE_IN,
    CALL_STATE_WAIT_FADE_IN,
    // The call connected: on to the talk
    CALL_STATE_FADE_OUT,
    // Leaving, once the camera's sound is done
    CALL_STATE_LEAVE,
    CALL_STATE_WAIT_FADE_OUT,
    CALL_STATE_LIST,
    // A button was pressed, which flashes before it does anything
    CALL_STATE_DECIDED,
    CALL_STATE_JOINING,
    CALL_STATE_WAITING,
    CALL_STATE_CALLING,
    // The call failed: a message, until the screen is tapped
    CALL_STATE_FAILED,
    CALL_STATE_PHOTO_NOTICE,
    CALL_STATE_WAIT_PHOTO_NOTICE,
};

// What the button of the bottom screen does
enum {
    CALL_MENU_NONE,
    CALL_MENU_CALL,
    CALL_MENU_JOIN,
    // Waiting to be called, with no button
    CALL_MENU_WAIT,
    // Back to the list, whose button isn't made yet
    CALL_MENU_RESET,
    // The return button was pressed
    CALL_MENU_BACK,
};

// What the list's check box shows
enum {
    CALL_CHECK_OFF = 9,
    CALL_CHECK_ON,
    CALL_CHECK_DISABLED,
};

// The beacon of a game in the field: a header, then the game's beacon. Its owner isn't decompiled; the header's fields
// are from their use here
typedef struct {
    u8 unk00[2];
    u8 unk02_0 : 1;
    // The list shows the game only when this is 2
    u8 kind : 5;
    u8 cameraBlocked : 1;
    u8 cameraEnabled : 1;
    u8 unk03;
    u32 trainerId;
    GameBeacon beacon;
} CtvtCallFieldBeacon;

// A machine found by scanning
typedef struct {
    BOOL active;
    // Set before each scan and cleared when the machine is found again, so that the machines gone can be dropped
    BOOL stale;
    // To be joined or called
    BOOL selected;
    BOOL cameraEnabled;
    // Makes its row flash
    BOOL changed;
    // A CtvtCommBeacon or a CtvtCallFieldBeacon, by gameServiceId
    void *beacon;
    u8 gameServiceId;
    u8 mac[6];
    u8 memberCount;
    u8 row;
} CtvtCallEntry;

// A row of the list, on BG 6
typedef struct {
    BOOL needsDraw;
    BOOL printPending;
    ClActor *frame;
    ClActor *check;
    ClActor *camera;
    BmpWin *window;
    // An index into the entries, or CTVT_CALL_NONE when the row is empty
    u8 entry;
} CtvtCallRow;

struct CtvtCall {
    // The entries to call, CTVT_CALL_NONE where free
    u8 invites[CTVT_CALL_INVITES];
    // The entry to join, or CTVT_CALL_NONE
    u8 joinTarget;
    u16 scrollY;
    u16 scrollBarY;
    int state;
    BOOL listDirty;
    BOOL scrollBarHeld;
    u8 rowCount;
    u8 prevRowCount;
    u32 menuMode;
    AppTaskMenuWin *menu;
    BOOL msgWinPending;
    BOOL infoWinPending;
    BOOL recordingStarted;
    // The bottom screen's message and the top screen's
    BmpWin *msgWin;
    BmpWin *infoWin;
    u16 waitTimer;
    u16 ringTimer;
    // Whether to show the top screen's message once it is printed
    BOOL showInfo;
    u32 unk3C;
    CtvtCallRow rows[CTVT_CALL_ROWS];
    CtvtCallEntry entries[CTVT_CALL_ENTRIES];
    ClActor *returnButton;
    ClActor *scrollBar;
};

void func_ov172_021bc5e8(CommTvtWork *sys, CtvtCall *call);
void func_ov172_021bc7fc(CommTvtWork *sys, CtvtCall *call);
void func_ov172_021bcb88(CommTvtWork *sys, CtvtCall *call);
static void func_ov172_021bcc40(CommTvtWork *sys, CtvtCall *call, CtvtCallRow *row, u8 index);
void func_ov172_021bce2c(CommTvtWork *sys, CtvtCall *call, CtvtCallRow *row, u8 index);
void func_ov172_021bcef8(CommTvtWork *sys, CtvtCall *call);
void func_ov172_021bd038(CommTvtWork *sys, CtvtCall *call);
BOOL func_ov172_021bd0a4(CommTvtWork *sys, CtvtCall *call, const u16 *name, u32 id, u32 gender);
static void func_ov172_021bd0c4(CommTvtWork *sys, CtvtCall *call, u32 msgId);
static void func_ov172_021bd134(CommTvtWork *sys, CtvtCall *call, u32 msgId);

// pokebw2's sReturnButton
static const TouchRect data_ov172_021c7040[] = {
    { 0xa8, 0xc0, 0xe0, 0xf8 },
    { TOUCH_RECT_END },
};

// pokebw2's CtvtCall_Create
CtvtCall *func_ov172_021bba6c(CommTvtWork *sys, HeapID heapId) {
    return GFL_HeapAllocate(heapId, sizeof(CtvtCall), TRUE, "ctvt_call.c", 216);
}

// pokebw2's CtvtCall_Delete
void func_ov172_021bba88(CommTvtWork *sys, CtvtCall *call) {
    GFL_HeapFree(call);
}

static inline ClActor *CtvtCall_NewActor(CommTvtWork *sys, ClActorSetup *setup, u32 charsId, u32 paletteId, u32 cellId, HeapID heapId) {
    ClActUnit *unit = func_ov172_021be024(sys);
    u32 chars = func_ov172_021be018(sys, charsId);
    u32 palette = func_ov172_021be018(sys, paletteId);

    return func_0204b294(unit, chars, palette, func_ov172_021be018(sys, cellId), setup, 1, heapId);
}

static inline void CtvtCall_CreateActor(CommTvtWork *sys, ClActor **actor, int chars, int palette, int cells,
                                        const ClActorSetup *setup, HeapID heapId) {
    *actor = func_0204b294(func_ov172_021be024(sys), func_ov172_021be018(sys, chars),
                            func_ov172_021be018(sys, palette), func_ov172_021be018(sys, cells), setup, 1, heapId);
}
void func_ov172_021bba94(CommTvtWork *sys, CtvtCall *call) {
    HeapID heapId;
    ArcTool *arc;
    CommTvtParam *param;
    ClActorSetup setup;
    u8 i;
    u8 j;

    heapId = func_ov172_021be00c(sys);
    arc = func_ov172_021be010(sys);
    param = func_ov172_021be004(sys);
    if (param->unk4 != 2 && param->unk4 != 3 && func_ov172_021be0ec(sys) == 0) {
        GFL_BGSysLoadArcNCGRStatic(arc, 0xb, 4, 0, 0x2800, 0, heapId);
        loadBGScrToVramByFileNoReserveNegAlign(arc, 0x12, 4, 0, 0, 0, heapId);
        GFL_BGSysLoadScr(4);
    }

    setup.bgPriority = 3;
    for (i = 0; i < CTVT_CALL_ROWS; i++) {
        setup.x = 0x80;
        setup.y = i * 0x20 + 0x10;
        setup.sequence = 8;
        setup.priority = 0x20;
        CtvtCall_CreateActor(sys, &call->rows[i].frame, 5, 1, 9, &setup, heapId);
        func_0204b3dc(call->rows[i].frame, 0);
        func_0204b92c(call->rows[i].frame, 1);

        setup.x = 0x12;
        setup.sequence = 9;
        setup.priority = 0x10;
        call->rows[i].check = CtvtCall_NewActor(sys, &setup, 5, 1, 9, heapId);
        func_0204b3dc(call->rows[i].check, 0);

        setup.x = 0xc8;
        setup.sequence = 0x12;
        setup.priority = 0x10;
        call->rows[i].camera = CtvtCall_NewActor(sys, &setup, 5, 1, 9, heapId);
        func_0204b3dc(call->rows[i].camera, 0);
    }

    setup.x = 0xe0;
    setup.y = 0xa8;
    if (func_ov172_021be0ec(sys) == 0) {
        setup.sequence = 1;
    } else {
        setup.sequence = 0xf;
    }
    setup.priority = 0;
    setup.bgPriority = 0;
    call->returnButton = CtvtCall_NewActor(sys, &setup, 6, 2, 10, heapId);
    func_0204b3dc(call->returnButton, 1);
    func_0204b92c(call->returnButton, 1);

    setup.x = 0xf8;
    setup.y = 0x18;
    setup.sequence = 0xc;
    call->scrollBar = CtvtCall_NewActor(sys, &setup, 5, 1, 9, heapId);
    func_0204b3dc(call->scrollBar, 0);

    for (j = 0; j < CTVT_CALL_ENTRIES; j++) {
        call->entries[j].active = FALSE;
        call->entries[j].stale = FALSE;
        call->entries[j].selected = FALSE;
        call->entries[j].changed = FALSE;
        call->entries[j].row = CTVT_CALL_NONE;
        call->entries[j].beacon = GFL_HeapAllocate(heapId, 0x64, TRUE, "ctvt_call.c", 330);
        call->entries[j].memberCount = 0;
    }

    for (i = 0; i < CTVT_CALL_ROWS; i++) {
        call->rows[i].needsDraw = FALSE;
        call->rows[i].printPending = FALSE;
        call->rows[i].entry = CTVT_CALL_NONE;
        call->rows[i].window = BmpWin_CreateDynamic(6, 3, i * 4 + 1, 0x19, 2, 10, 1);
    }
    call->msgWin = BmpWin_CreateDynamic(4, 1, 0x15, 0x15, 2, 10, 1);
    call->infoWin = BmpWin_CreateDynamic(4, 2, 9, 0x1c, 2, 10, 1);
    call->showInfo = FALSE;
    call->ringTimer = 0;
    for (i = 0; i < CTVT_CALL_INVITES; i++) {
        call->invites[i] = CTVT_CALL_NONE;
    }
    call->joinTarget = CTVT_CALL_NONE;
    call->scrollY = 0;
    call->scrollBarY = 0;
    call->rowCount = 0;
    call->prevRowCount = CTVT_CALL_NONE;
    call->menuMode = 0xff;
    call->listDirty = FALSE;
    call->scrollBarHeld = FALSE;
    call->msgWinPending = FALSE;
    call->infoWinPending = FALSE;
    call->recordingStarted = FALSE;
    call->menu = NULL;
    call->state = 0;
    call->waitTimer = 0;

    GFL_BGSysMoveBGReq(6, 3, 0);
    if ((param->unk4 == 2 || param->unk4 == 3) && func_ov172_021be0ec(sys) == 0) {
        func_0204b878(call->returnButton, 0xf);
        func_ov172_021bd134(sys, call, 0x18);
        call->showInfo = TRUE;
        call->menuMode = 3;
    }
}


static inline void CtvtCall_ClearWindow(BmpWin *window) {
    func_020456f8(window);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
}

static inline void CtvtCall_ClearWindowQueued(BmpWin *window) {
    func_020456f8(window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
}

// pokebw2's CtvtCall_Leave
void func_ov172_021bbe5c(CommTvtWork *sys, CtvtCall *call) {
    u8 i;

    if (call->menu != NULL) {
        func_02028148(call->menu);
        call->menu = NULL;
    }
    CtvtCall_ClearWindow(call->msgWin);
    BmpWin_Free(call->msgWin);
    CtvtCall_ClearWindow(call->infoWin);
    BmpWin_Free(call->infoWin);
    func_0204b3b4(call->scrollBar);
    func_0204b3b4(call->returnButton);
    for (i = 0; i < CTVT_CALL_ROWS; i++) {
        BmpWin_Free(call->rows[i].window);
        func_0204b3b4(call->rows[i].camera);
        func_0204b3b4(call->rows[i].check);
        func_0204b3b4(call->rows[i].frame);
    }
    for (i = 0; i < CTVT_CALL_ENTRIES; i++) {
        GFL_HeapFree(call->entries[i].beacon);
    }
    GFL_BGSysClearScr(6);
    GFL_BGSysLoadScr(6);
    GFL_BGSysMoveBGReq(6, 3, 0);
    GFL_BGSysClearScr(4);
    GFL_BGSysLoadScr(4);
    GFL_BGSysMoveBGReq(4, 3, 0);
}

int func_ov172_021bbf2c(CommTvtWork *sys, CtvtCall *call) {
    HeapID heapId = func_ov172_021be00c(sys);
    CommTvtParam *param = func_ov172_021be004(sys);

    switch (call->state) {
    case CALL_STATE_FADE_IN:
        if (func_ov172_021be02c(sys) == TRUE) {
            GFL_WipeSet(0, 1, 1, 0, 6, 1, heapId);
        } else {
            GFL_WipeSet(4, 1, 1, 0, 6, 1, heapId);
        }
        call->state = CALL_STATE_WAIT_FADE_IN;
        break;
    case CALL_STATE_WAIT_FADE_IN:
        if (GFL_WipeIsFinished() == TRUE) {
            CommTvtParam *param = func_ov172_021be004(sys);

            if (func_ov172_021be0ec(sys) == TRUE) {
                call->state = CALL_STATE_PHOTO_NOTICE;
            } else if (param->unk4 == 2 || param->unk4 == 3) {
                call->state = CALL_STATE_WAITING;
            } else {
                call->state = CALL_STATE_LIST;
            }
        }
        break;
    case CALL_STATE_FADE_OUT:
        GFL_WipeSet(4, 0, 0, 0, 6, 1, heapId);
        func_ov172_021be030(sys, FALSE);
        call->state = CALL_STATE_WAIT_FADE_OUT;
        break;
    case CALL_STATE_LEAVE:
        if (func_ov172_021b7590(sys, func_ov172_021bdfdc(sys)) == TRUE) {
            GFL_WipeSet(0, 0, 0, 0, 6, 1, heapId);
            func_ov172_021be030(sys, FALSE);
            call->state = CALL_STATE_WAIT_FADE_OUT;
        }
        break;
    case CALL_STATE_WAIT_FADE_OUT:
        if (GFL_WipeIsFinished() == TRUE) {
            if (call->menuMode == CALL_MENU_NONE || call->menuMode == CALL_MENU_BACK) {
                return COMM_TVT_MODE_GAME;
            }
            return COMM_TVT_MODE_TALK;
        }
        break;
    case CALL_STATE_LIST:
        func_ov172_021bc5e8(sys, call);
        func_ov172_021bd038(sys, call);
        break;
    case CALL_STATE_DECIDED:
        if (call->menuMode == CALL_MENU_NONE || call->menuMode == CALL_MENU_BACK) {
            if (func_0204b98c(call->returnButton) == FALSE) {
                func_ov172_021b7578(sys, func_ov172_021bdfdc(sys));
                call->menuMode = CALL_MENU_NONE;
                call->state = CALL_STATE_LEAVE;
            }
        } else if (func_02028230(call->menu) == TRUE) {
            if (call->menuMode == CALL_MENU_JOIN) {
                CtvtComm *comm = func_ov172_021bdfe4(sys);

                func_ov172_021b80dc(sys, comm, CTVT_CONNECT_MAC);
                func_ov172_021b80ec(sys, comm, call->entries[call->joinTarget].mac);
                func_ov172_021bd134(sys, call, 24);
                call->showInfo = TRUE;
                call->state = CALL_STATE_JOINING;
                call->waitTimer = 0;
                call->ringTimer = 0;
                func_02028148(call->menu);
                call->menu = NULL;
            } else {
                CtvtComm *comm = func_ov172_021bdfe4(sys);
                CtvtCommBeacon *beacon;
                u8 i, j;

                call->state = CALL_STATE_CALLING;
                func_ov172_021bd134(sys, call, 20);
                call->showInfo = TRUE;
                call->ringTimer = 0;
                func_ov172_021b80dc(sys, comm, CTVT_CONNECT_SCAN);
                func_02028148(call->menu);
                call->menu = NULL;
                beacon = func_ov172_021b8b7c(sys, comm);
                for (i = 0; i < CTVT_CALL_INVITES; i++) {
                    if (call->invites[i] == CTVT_CALL_NONE) {
                        for (j = 0; j < 6; j++) {
                            beacon->inviteMacs[i][j] = 0xff;
                        }
                    } else {
                        for (j = 0; j < 6; j++) {
                            beacon->inviteMacs[i][j] = call->entries[call->invites[i]].mac[j];
                        }
                    }
                }
                beacon->inviteOnly = TRUE;
                func_ov172_021b8c14(sys, comm);
            }
            func_0204b878(call->returnButton, 15);
        }
        break;
    case CALL_STATE_JOINING:
    case CALL_STATE_WAITING:
    case CALL_STATE_CALLING:
        func_ov172_021bdfe4(sys);
        if (func_ov172_021be054(sys) >= 2) {
            if (call->recordingStarted == FALSE) {
                CtvtCamera *camera = func_ov172_021bdfdc(sys);

                GFL_SndStop();
                GFL_SndSEPlay(SEQ_SE_SYS_72);
                func_ov172_021b7b64(sys, camera);
            }
            call->state = CALL_STATE_FADE_OUT;
        }
        if (call->state == CALL_STATE_CALLING) {
            call->ringTimer++;
            if (call->ringTimer >= CTVT_CALL_RING_FRAMES) {
                call->ringTimer = 0;
                GFL_SEPlayKeepVol(SEQ_SE_SYS_71, 2);
            }
        }
        if (call->state == CALL_STATE_CALLING) {
            if (func_ov172_021b8c20(sys, func_ov172_021bdfe4(sys)) == TRUE) {
                func_ov172_021b80dc(sys, func_ov172_021bdfe4(sys), CTVT_CONNECT_PARENT);
                func_ov172_021bd134(sys, call, 39);
                call->state = CALL_STATE_FAILED;
            }
        } else if (call->state == CALL_STATE_JOINING || call->state == CALL_STATE_WAITING) {
            call->waitTimer++;
            if (call->waitTimer >= CTVT_CALL_WAIT_FRAMES) {
                func_ov172_021b80dc(sys, func_ov172_021bdfe4(sys), CTVT_CONNECT_PARENT);
                func_ov172_021bd134(sys, call, 40);
                call->state = CALL_STATE_FAILED;
            }
        }
        break;
    case CALL_STATE_FAILED:
        if (func_02035c60() == TRUE) {
            if (func_ov172_021be004(sys)->unk4 == 2) {
                func_ov172_021b7578(sys, func_ov172_021bdfdc(sys));
                call->menuMode = CALL_MENU_NONE;
                call->state = CALL_STATE_LEAVE;
            } else {
                u8 i;

                for (i = 0; i < CTVT_CALL_INVITES; i++) {
                    call->invites[i] = CTVT_CALL_NONE;
                }
                call->state = CALL_STATE_LIST;
                call->joinTarget = CTVT_CALL_NONE;
                func_0201f7a8(call->infoWin, 1);
                CtvtCall_ClearWindow(call->infoWin);
                func_ov172_021be34c(sys);
                func_0204b878(call->returnButton, 1);
                call->menuMode = CALL_MENU_RESET;
            }
        }
        break;
    case CALL_STATE_PHOTO_NOTICE:
        BmpWin_Free(call->msgWin);
        call->msgWin = BmpWin_CreateDynamic(4, 2, 5, 28, 14, 10, TRUE);
        func_ov172_021bd0c4(sys, call, 41);
        call->state = CALL_STATE_WAIT_PHOTO_NOTICE;
        break;
    case CALL_STATE_WAIT_PHOTO_NOTICE:
        if (func_ov172_021be054(sys) >= 2 && call->recordingStarted == FALSE) {
            GFL_SndStop();
            call->recordingStarted = TRUE;
            GFL_SndSEPlay(SEQ_SE_SYS_72);
        }
        if (func_02035c60() == TRUE || func_ov172_021be0d0(sys) == TRUE) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_0201f7a8(call->msgWin, 1);
            CtvtCall_ClearWindow(call->msgWin);
            BmpWin_Free(call->msgWin);
            call->msgWin = BmpWin_CreateDynamic(4, 1, 21, 21, 2, 10, TRUE);
            func_ov172_021be0f0(sys);
            if (param->unk4 == 2 || param->unk4 == 3) {
                func_0204b878(call->returnButton, 15);
                if (func_ov172_021be054(sys) < 2) {
                    func_ov172_021bd134(sys, call, 24);
                }
                call->showInfo = TRUE;
                call->menuMode = CALL_MENU_WAIT;
                call->state = CALL_STATE_WAITING;
            } else {
                HeapID heapId;
                ArcTool *arc;

                call->state = CALL_STATE_LIST;
                call->menuMode = CALL_MENU_RESET;
                func_0204b878(call->returnButton, 1);
                heapId = func_ov172_021be00c(sys);
                arc = func_ov172_021be010(sys);
                GFL_BGSysLoadArcNCGRStatic(arc, 11, 4, 0, 0x2800, FALSE, heapId);
                loadBGScrToVramByFileNoReserveNegAlign(arc, 18, 4, 0, 0, FALSE, heapId);
                GFL_BGSysLoadScr(4);
            }
        }
        break;
    }

    if (param->unk4 != 3 && func_ov172_021be0ec(sys) == FALSE) {
        u8 i;

        func_ov172_021bc7fc(sys, call);
        for (i = 0; i < CTVT_CALL_ROWS; i++) {
            if (call->rows[i].printPending == TRUE &&
                !func_0201c6c4(func_ov172_021be044(sys), BmpWin_GetBitmap(call->rows[i].window))) {
                BmpWin *window = call->rows[i].window;

                BmpWin_FlushChar(window);
                BmpWin_FlushMap(window);
                GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
                call->rows[i].printPending = FALSE;
            }
        }
        if (call->listDirty == TRUE) {
            u8 inviteCount = 0;

            for (i = 0; i < CTVT_CALL_ROWS; i++) {
                func_ov172_021bce2c(sys, call, &call->rows[i], i);
            }
            GFL_BGSysMoveBGReq(6, 3, call->scrollY);
            for (i = 0; i < CTVT_CALL_INVITES; i++) {
                if (call->invites[i] != CTVT_CALL_NONE) {
                    inviteCount++;
                }
            }
            for (i = 0; i < CTVT_CALL_ENTRIES; i++) {
                if (call->entries[i].active == TRUE) {
                    CtvtCallRow *row = &call->rows[call->entries[i].row];

                    if (call->entries[i].selected == TRUE && call->entries[i].memberCount < CTVT_CALL_MEMBERS_MAX &&
                        (call->joinTarget == CTVT_CALL_NONE || call->joinTarget == i)) {
                        func_0204b878(row->check, CALL_CHECK_ON);
                    } else if (inviteCount == CTVT_CALL_INVITES || call->joinTarget != CTVT_CALL_NONE ||
                               call->entries[i].memberCount == CTVT_CALL_MEMBERS_MAX) {
                        func_0204b878(row->check, CALL_CHECK_DISABLED);
                    } else {
                        func_0204b878(row->check, CALL_CHECK_OFF);
                    }
                    if (call->entries[i].changed == TRUE) {
                        call->entries[i].changed = FALSE;
                        func_0204b878(row->frame, 34);
                    }
                }
            }
            call->listDirty = FALSE;
        }
    }

    if (call->msgWinPending == TRUE && !func_0201c6c4(func_ov172_021be044(sys), BmpWin_GetBitmap(call->msgWin))) {
        BmpWin_FlushChar(call->msgWin);
        BmpWin_FlushMap(call->msgWin);
        GFL_BGSysLoadScr(4);
        call->msgWinPending = FALSE;
    }
    if (call->infoWinPending == TRUE && !func_0201c6c4(func_ov172_021be044(sys), BmpWin_GetBitmap(call->infoWin))) {
        BmpWin_FlushChar(call->infoWin);
        BmpWin_FlushMap(call->infoWin);
        GFL_BGSysLoadScr(4);
        call->infoWinPending = FALSE;
        if (call->showInfo == TRUE) {
            call->showInfo = FALSE;
            func_ov172_021be318(sys, call->infoWin);
        }
    }
    if (call->menu != NULL) {
        func_02028178(call->menu);
    }
    return COMM_TVT_MODE_CALL;
}

void func_ov172_021bc5e8(CommTvtWork *sys, CtvtCall *call) {
    u32 touchX;
    u32 touchY;
    u32 touchX2;
    u32 touchY2;
    BOOL touched;
    BOOL touched2;
    s32 row;
    ClActorPos pos;
    s32 barY;

    touched = func_02035d20(&touchX, &touchY);
    touched2 = func_02035cb8(&touchX2, &touchY2);
    func_ov172_021bdfe4(sys);
    if (touched == TRUE) {
        if (touchX > 8 && touchX < 0xe0) {
            row = (s32)((touchY + call->scrollY) << 11) >> 16;
            if (row >= 0 && row < CTVT_CALL_ROWS) {
                u8 entryIdx = call->rows[row].entry;
                CtvtCallEntry *entry = &call->entries[entryIdx];
                BOOL handled = FALSE;

                if (entry->active == TRUE && entry->memberCount < CTVT_CALL_MEMBERS_MAX) {
                    if (entry->gameServiceId == CTVT_CALL_SERVICE_CTVT) {
                        if (entry->selected == FALSE) {
                            call->joinTarget = entryIdx;
                            handled = TRUE;
                            entry->selected = TRUE;
                            entry->changed = TRUE;
                            call->listDirty = TRUE;
                            GFL_SndSEPlay(0x73b);
                        } else {
                            call->joinTarget = CTVT_CALL_NONE;
                            entry->selected = FALSE;
                            handled = TRUE;
                            entry->changed = TRUE;
                            call->listDirty = TRUE;
                            GFL_SndSEPlay(0x73c);
                        }
                    } else if (call->joinTarget == CTVT_CALL_NONE) {
                        if (entry->selected == FALSE) {
                            u8 i;

                            for (i = 0; i < CTVT_CALL_INVITES; i++) {
                                if (call->invites[i] == CTVT_CALL_NONE) {
                                    call->invites[i] = entryIdx;
                                    break;
                                }
                            }
                            if (i < CTVT_CALL_INVITES) {
                                handled = TRUE;
                                entry->selected = TRUE;
                                entry->changed = TRUE;
                                call->listDirty = TRUE;
                                GFL_SndSEPlay(0x73b);
                            }
                        } else {
                            u8 i;

                            for (i = 0; i < CTVT_CALL_INVITES; i++) {
                                if (entryIdx == call->invites[i]) {
                                    call->invites[i] = CTVT_CALL_NONE;
                                    break;
                                }
                            }
                            entry->selected = FALSE;
                            handled = TRUE;
                            entry->changed = TRUE;
                            call->listDirty = TRUE;
                            GFL_SndSEPlay(0x73c);
                        }
                    }
                }
                if (handled == TRUE) {
                    func_ov172_021bcef8(sys, call);
                }
            }
        } else {
            if (touchX >= 0xec && touchX <= 0x104 && touchY >= call->scrollBarY + 0xc && touchY <= call->scrollBarY + 0x24 && call->rowCount > CTVT_CALL_VISIBLE_ROWS) {
                call->scrollBarHeld = TRUE;
                func_0204b878(call->scrollBar, 0xd);
                GFL_SndSEPlay(0x66c);
            }
        }
    }

    if (touched2 == TRUE && call->scrollBarHeld == TRUE && call->rowCount > CTVT_CALL_VISIBLE_ROWS) {
        if (touchX2 >= 0xec && touchX2 <= 0x104 && touchY2 >= 0xc && touchY2 <= 0x9c) {
            u32 range = (call->rowCount - CTVT_CALL_VISIBLE_ROWS) * CTVT_CALL_ROW_HEIGHT;

            barY = (s32)touchY2 - 0x18;
            if (barY < 0) {
                barY = 0;
            } else if (barY > CTVT_CALL_SCROLL_BAR_RANGE) {
                barY = CTVT_CALL_SCROLL_BAR_RANGE;
            }
            call->scrollBarY = barY;
            pos.x = 0xf8;
            pos.y = call->scrollBarY + 0x18;
            func_0204b404(call->scrollBar, &pos, 1);
            call->scrollY = call->scrollBarY * range / CTVT_CALL_SCROLL_BAR_RANGE;
            call->listDirty = TRUE;
        }
    } else {
        call->scrollBarHeld = FALSE;
        func_0204b878(call->scrollBar, 0xc);
    }

    if (call->menu != NULL && func_0202824c(call->menu) == 1) {
        func_0202822c(call->menu, TRUE);
        call->state = 6;
        GFL_SndSEPlay(0x54c);
        if (call->menuMode == 1) {
            GFL_SEPlayKeepVol(0x73d, 2);
        }
    }
    if (func_02035c08(data_ov172_021c7040) == 0 || (GCTX_HIDGetPressedKeys() & 2) != 0) {
        call->state = 6;
        call->menuMode = 5;
        func_0204b878(call->returnButton, 9);
        GFL_SndSEPlay(0x551);
    }
}

void func_ov172_021bc7fc(CommTvtWork *sys, CtvtCall *call) {
    BOOL listChanged = FALSE;
    BOOL menuChanged = FALSE;
    int i;

    if (func_ov172_021b8108(sys, func_ov172_021bdfe4(sys)) == FALSE) {
        return;
    }

    for (i = 0; i < CTVT_CALL_ENTRIES; i++) {
        if (call->entries[i].active == TRUE) {
            call->entries[i].stale = TRUE;
        }
    }

    if (call->state != CALL_STATE_JOINING && call->state != CALL_STATE_CALLING && call->state != CALL_STATE_FAILED &&
        call->state >= CALL_STATE_LIST) {
        for (i = 0; i < 16; i++) {
            BOOL found = FALSE;
            BOOL replace = FALSE;
            void *beacon = func_0203cf10(i);

            if (beacon != NULL) {
                int slot = CTVT_CALL_NONE;
                u8 *mac = func_0203cf3c(i);
                u8 service = func_ov014_02177b80(i);
                BOOL isFriend;
                u8 j, k;

                if (service == CTVT_CALL_SERVICE_FIELD && ((CtvtCallFieldBeacon *)beacon)->kind != 2) {
                    continue;
                }
                isFriend = FALSE;
                if (service == CTVT_CALL_SERVICE_CTVT) {
                    const u16 *name = GetPlayerName(beacon);
                    u32 gender = getTrainerGender(beacon);
                    u32 id = getIDAsUInt(beacon);
                    u8 selfMac[6];

                    OS_GetMacAddress(selfMac);
                    if (((CtvtCommBeacon *)beacon)->inviteOnly == FALSE ||
                        func_ov172_021b85d0(beacon, selfMac) == TRUE) {
                        isFriend = func_ov172_021bd0a4(sys, call, name, id, gender);
                    }
                } else {
                    CtvtCallFieldBeacon *fieldBeacon = beacon;

                    if (IsZoneEntralinkHub(fieldBeacon->beacon.zoneId) == FALSE &&
                        GetZoneIsEntreeForest(fieldBeacon->beacon.zoneId) == FALSE) {
                        isFriend = func_ov172_021bd0a4(sys, call, fieldBeacon->beacon.name, fieldBeacon->trainerId,
                                                       fieldBeacon->beacon.gender);
                    }
                }
                if (isFriend == FALSE) {
                    continue;
                }

                for (j = 0; j < CTVT_CALL_ENTRIES; j++) {
                    if (call->entries[j].active == TRUE) {
                        BOOL same = TRUE;

                        for (k = 0; k < 6; k++) {
                            if (mac[k] != call->entries[j].mac[k]) {
                                same = FALSE;
                                break;
                            }
                        }
                        if (same == TRUE) {
                            call->entries[j].stale = FALSE;
                            found = TRUE;
                            if (service != call->entries[j].gameServiceId) {
                                found = FALSE;
                                replace = TRUE;
                                slot = j;
                                for (k = 0; k < CTVT_CALL_INVITES; k++) {
                                    if (j == call->invites[k]) {
                                        call->invites[k] = CTVT_CALL_NONE;
                                        menuChanged = TRUE;
                                    }
                                }
                                if (call->joinTarget == j) {
                                    call->joinTarget = CTVT_CALL_NONE;
                                    menuChanged = TRUE;
                                }
                            }
                            if (service == CTVT_CALL_SERVICE_CTVT) {
                                CtvtCommBeacon *entryBeacon = call->entries[j].beacon;

                                if (entryBeacon->memberCount != ((CtvtCommBeacon *)beacon)->memberCount) {
                                    found = FALSE;
                                    replace = TRUE;
                                    slot = j;
                                }
                            }
                            break;
                        }
                    } else if (slot == CTVT_CALL_NONE) {
                        slot = j;
                    }
                }

                if (found == FALSE && slot != CTVT_CALL_NONE) {
                    call->entries[slot].active = TRUE;
                    call->entries[slot].stale = FALSE;
                    call->entries[slot].selected = FALSE;
                    for (k = 0; k < 6; k++) {
                        call->entries[slot].mac[k] = mac[k];
                    }
                    if (service == CTVT_CALL_SERVICE_CTVT) {
                        call->entries[slot].gameServiceId = CTVT_CALL_SERVICE_CTVT;
                        MI_CpuCopy8(beacon, call->entries[slot].beacon, sizeof(CtvtCommBeacon));
                    } else {
                        call->entries[slot].gameServiceId = CTVT_CALL_SERVICE_FIELD;
                        MI_CpuCopy8(beacon, call->entries[slot].beacon, sizeof(CtvtCallFieldBeacon));
                    }
                    if (replace == FALSE) {
                        for (k = 0; k < CTVT_CALL_ROWS; k++) {
                            if (call->rows[k].entry == CTVT_CALL_NONE) {
                                call->rows[k].entry = slot;
                                call->entries[slot].row = k;
                                call->rows[k].needsDraw = TRUE;
                                break;
                            }
                        }
                    } else {
                        call->rows[call->entries[slot].row].needsDraw = TRUE;
                    }
                    listChanged = TRUE;
                }
            }
        }
    }

    for (i = 0; i < CTVT_CALL_ENTRIES; i++) {
        if (call->entries[i].active == TRUE && call->entries[i].stale == TRUE) {
            u8 row;
            u8 k;

            for (row = call->entries[i].row; row < CTVT_CALL_ROWS - 1; row++) {
                if (call->rows[row + 1].entry == CTVT_CALL_NONE) {
                    break;
                }
                call->rows[row].entry = call->rows[row + 1].entry;
                call->entries[call->rows[row].entry].row = row;
                call->rows[row].needsDraw = TRUE;
            }
            call->rows[row].entry = CTVT_CALL_NONE;
            call->rows[row].needsDraw = TRUE;
            call->entries[i].row = CTVT_CALL_NONE;
            call->entries[i].active = FALSE;
            for (k = 0; k < CTVT_CALL_INVITES; k++) {
                if (i == call->invites[k]) {
                    call->invites[k] = CTVT_CALL_NONE;
                    menuChanged = TRUE;
                }
            }
            if (call->joinTarget == i) {
                call->joinTarget = CTVT_CALL_NONE;
                menuChanged = TRUE;
            }
            i--;
            listChanged = TRUE;
        }
    }

    if (listChanged == TRUE) {
        func_ov172_021bcb88(sys, call);
        menuChanged = TRUE;
        call->listDirty = TRUE;
    }
    if (menuChanged == TRUE && call->state == CALL_STATE_LIST) {
        func_ov172_021bcef8(sys, call);
    }
}


// pokebw2's CtvtCall_DrawRows
void func_ov172_021bcb88(CommTvtWork *sys, CtvtCall *call) {
    u8 count = 0;
    u8 i;

    for (i = 0; i < CTVT_CALL_ROWS; i++) {
        func_ov172_021bcc40(sys, call, &call->rows[i], i);
    }
    for (i = 0; i < CTVT_CALL_ROWS; i++) {
        if (call->rows[i].entry != CTVT_CALL_NONE) {
            count++;
        }
    }
    if (call->rowCount != count) {
        if (count > CTVT_CALL_VISIBLE_ROWS) {
            u32 range = (count - CTVT_CALL_VISIBLE_ROWS) * CTVT_CALL_ROW_HEIGHT;

            func_0204b3dc(call->scrollBar, TRUE);
            if (call->scrollY > range) {
                call->scrollY = range;
            }
            call->scrollBarY = call->scrollY * CTVT_CALL_SCROLL_BAR_RANGE / range;
            if (call->scrollBarHeld == FALSE) {
                ClActorPos pos;

                pos.x = 248;
                pos.y = call->scrollBarY + 24;
                func_0204b404(call->scrollBar, &pos, 1);
            }
        } else {
            func_0204b3dc(call->scrollBar, FALSE);
            call->scrollY = 0;
            call->listDirty = TRUE;
        }
        call->rowCount = count;
    }
}

// pokebw2's CtvtCall_DrawRow
static void func_ov172_021bcc40(CommTvtWork *sys, CtvtCall *call, CtvtCallRow *row, u8 index) {
    if (row->needsDraw != TRUE) {
        return;
    }
    if (row->entry != CTVT_CALL_NONE) {
        MsgData *msgData;
        PrintQueue *queue;
        StrBuf *name;
        StrBuf *str;
        CtvtCommBeacon *beacon;
        HeapID heapId;
        StrBuf *format;
        Font *font;
        CtvtCallEntry *entry;
        WordSet *wordSet;

        heapId = func_ov172_021be00c(sys);
        font = func_ov172_021be034(sys);
        msgData = func_ov172_021be03c(sys);
        queue = func_ov172_021be044(sys);
        entry = &call->entries[row->entry];
        wordSet = GFL_WordSetSystemCreateDefault(heapId);

        GFL_BitmapFill(BmpWin_GetBitmap(row->window), 0);
        name = GFL_StrBufCreate(32, heapId);
        if (entry->gameServiceId == CTVT_CALL_SERVICE_CTVT) {
            GFL_StrBufLoadFixedString(name, func_0202e930(entry->beacon), 8);
        } else {
            GFL_StrBufLoadFixedString(name, ((CtvtCallFieldBeacon *)entry->beacon)->beacon.name, 8);
        }
        func_0201c724(queue, BmpWin_GetBitmap(row->window), 6, 0, name, font, 0x3c40);
        GFL_StrBufFree(name);

        format = GFL_MsgDataLoadStrbufNew(msgData, 14);
        str = GFL_StrBufCreate(32, heapId);
        if (entry->gameServiceId == CTVT_CALL_SERVICE_CTVT) {
            WordSetNumber(wordSet, 0, func_0202e938(entry->beacon), 5, 2, TRUE);
        } else {
            WordSetNumber(wordSet, 0, ((CtvtCallFieldBeacon *)entry->beacon)->beacon.trainerId, 5, 2, TRUE);
        }
        GFL_WordSetFormatStrbuf(wordSet, str, format);
        GFL_StrBufFree(format);
        func_0201c724(queue, BmpWin_GetBitmap(row->window), 80, 0, str, font, 0x3c40);
        GFL_StrBufFree(str);

        if (entry->gameServiceId == CTVT_CALL_SERVICE_CTVT) {
            StrBuf *memberFormat;
            StrBuf *members;

            beacon = entry->beacon;
            entry->memberCount = beacon->memberCount;
            memberFormat = GFL_MsgDataLoadStrbufNew(msgData, 16);
            members = GFL_StrBufCreate(32, heapId);
            WordSetNumber(wordSet, 0, beacon->memberCount, 1, 0, TRUE);
            GFL_WordSetFormatStrbuf(wordSet, members, memberFormat);
            GFL_StrBufFree(memberFormat);
            func_0201c724(queue, BmpWin_GetBitmap(row->window), 144, 0, members, font, 0x3c40);
            GFL_StrBufFree(members);
        } else {
            entry->memberCount = 0;
        }

        if (entry->gameServiceId == CTVT_CALL_SERVICE_CTVT) {
            if (((CtvtCommBeacon *)entry->beacon)->cameraEnabled == TRUE) {
                entry->cameraEnabled = TRUE;
            } else {
                entry->cameraEnabled = FALSE;
            }
        } else {
            CtvtCallFieldBeacon *fieldBeacon = entry->beacon;

            if (fieldBeacon->cameraEnabled == TRUE && fieldBeacon->cameraBlocked == FALSE) {
                entry->cameraEnabled = TRUE;
            } else {
                entry->cameraEnabled = FALSE;
            }
        }
        GFL_WordSetSystemFree(wordSet);
        row->printPending = TRUE;
    } else {
        CtvtCall_ClearWindowQueued(row->window);
    }
    row->needsDraw = FALSE;
}

static inline void CtvtCall_HideRow(CtvtCallRow *row) {
    func_0204b3dc(row->frame, FALSE);
    func_0204b3dc(row->check, FALSE);
    func_0204b3dc(row->camera, FALSE);
}

// pokebw2's CtvtCall_UpdateRowActors
void func_ov172_021bce2c(CommTvtWork *sys, CtvtCall *call, CtvtCallRow *row, u8 index) {
    s16 y;

    if (row->entry == CTVT_CALL_NONE) {
        CtvtCall_HideRow(row);
        return;
    }
    y = index * CTVT_CALL_ROW_HEIGHT - call->scrollY;
    if (y >= -CTVT_CALL_ROW_HEIGHT && y <= 160) {
        ClActorPos pos;

        pos.x = 128;
        pos.y = y + 16;
        func_0204b404(row->frame, &pos, 1);
        func_0204b3dc(row->frame, TRUE);
        pos.x = 18;
        func_0204b404(row->check, &pos, 1);
        func_0204b3dc(row->check, TRUE);
        pos.x = 200;
        func_0204b404(row->camera, &pos, 1);
        if (call->entries[row->entry].cameraEnabled == TRUE) {
            func_0204b878(call->rows[index].camera, 18);
            func_0204b3dc(row->camera, TRUE);
        } else {
            func_0204b3dc(row->camera, FALSE);
        }
    } else {
        CtvtCall_HideRow(row);
    }
}

// pokebw2's CtvtCall_UpdateMenu
void func_ov172_021bcef8(CommTvtWork *sys, CtvtCall *call) {
    HeapID heapId;
    MsgData *msgData;
    AppTaskMenuRes *res;
    u8 inviteCount = 0;
    u8 i;

    heapId = func_ov172_021be00c(sys);
    msgData = func_ov172_021be03c(sys);
    res = func_ov172_021be04c(sys);
    for (i = 0; i < CTVT_CALL_INVITES; i++) {
        if (call->invites[i] != CTVT_CALL_NONE) {
            inviteCount++;
        }
    }
    if (call->joinTarget != CTVT_CALL_NONE) {
        if (call->menuMode != 2) {
            AppTaskMenuItem item = { 0 };

            CtvtCall_ClearWindowQueued(call->msgWin);
            func_0201f7a8(call->msgWin, 1);
            if (call->menu != NULL) {
                func_02028148(call->menu);
                call->menu = NULL;
            }
            item.str = GFL_MsgDataLoadStrbufNew(msgData, 19);
            item.color = 0x39e3;
            item.type = 0;
            call->menu = func_02027fec(res, &item, 0, 21, 21, heapId);
            GFL_StrBufFree(item.str);
            call->menuMode = 2;
        }
    } else if (inviteCount != 0) {
        if (call->menuMode != 1) {
            AppTaskMenuItem item = { 0 };

            CtvtCall_ClearWindowQueued(call->msgWin);
            func_0201f7a8(call->msgWin, 1);
            if (call->menu != NULL) {
                func_02028148(call->menu);
                call->menu = NULL;
            }
            item.str = GFL_MsgDataLoadStrbufNew(msgData, 18);
            item.color = 0x39e3;
            item.type = 0;
            call->menu = func_02027fec(res, &item, 0, 21, 21, heapId);
            GFL_StrBufFree(item.str);
            call->menuMode = 1;
        }
    } else if (call->menuMode != 0) {
        if (call->menu != NULL) {
            func_02028148(call->menu);
            call->menu = NULL;
        }
        if (call->rowCount != 0) {
            func_ov172_021bd0c4(sys, call, 17);
        }
        call->menuMode = 0;
    }
}

// Shows the nobody-found message on the top screen when the list empties, and the list's message when it fills again
// pokebw2's CtvtCall_UpdateFoundMessage
void func_ov172_021bd038(CommTvtWork *sys, CtvtCall *call) {
    if (call->rowCount != call->prevRowCount) {
        if (call->rowCount == 0) {
            func_ov172_021bd134(sys, call, 28);
            call->showInfo = TRUE;
            CtvtCall_ClearWindowQueued(call->msgWin);
            func_0201f7a8(call->msgWin, 1);
        } else if (call->prevRowCount == 0 && call->rowCount != 0) {
            func_ov172_021bd0c4(sys, call, 17);
            func_ov172_021be34c(sys);
            func_0201f7a8(call->infoWin, 1);
            CtvtCall_ClearWindow(call->infoWin);
        }
        call->prevRowCount = call->rowCount;
    }
}

// pokebw2's CtvtCall_IsFriend
BOOL func_ov172_021bd0a4(CommTvtWork *sys, CtvtCall *call, const u16 *name, u32 id, u32 gender) {
    return func_02009d50(func_0201283c(func_ov172_021be004(sys)->gameData), name, id, gender, NULL);
}

// pokebw2's CtvtCall_PrintMessage
static void func_ov172_021bd0c4(CommTvtWork *sys, CtvtCall *call, u32 msgId) {
    Font *font = func_ov172_021be034(sys);
    MsgData *msgData = func_ov172_021be03c(sys);
    PrintQueue *queue = func_ov172_021be044(sys);
    StrBuf *str;

    GFL_BitmapFill(BmpWin_GetBitmap(call->msgWin), 15);
    str = GFL_MsgDataLoadStrbufNew(msgData, msgId);
    func_0201c724(queue, BmpWin_GetBitmap(call->msgWin), 0, 0, str, font, 0x440);
    GFL_StrBufFree(str);
    BmpWin_DrawFrame(call->msgWin, 2, 0x140, 9);
    call->msgWinPending = TRUE;
}

// pokebw2's CtvtCall_PrintInfo
static void func_ov172_021bd134(CommTvtWork *sys, CtvtCall *call, u32 msgId) {
    Font *font = func_ov172_021be034(sys);
    MsgData *msgData = func_ov172_021be03c(sys);
    PrintQueue *queue = func_ov172_021be044(sys);
    StrBuf *str;

    GFL_BitmapFill(BmpWin_GetBitmap(call->infoWin), 15);
    str = GFL_MsgDataLoadStrbufNew(msgData, msgId);
    func_0201c724(queue, BmpWin_GetBitmap(call->infoWin), 0, 0, str, font, 0x440);
    GFL_StrBufFree(str);
    BmpWin_DrawFrame(call->infoWin, 2, 0x140, 9);
    call->infoWinPending = TRUE;
    call->showInfo = FALSE;
    func_ov172_021be34c(sys);
}
