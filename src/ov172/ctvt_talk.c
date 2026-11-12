// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov257/ctvt_talk.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/comm_tvt/ctvt_talk.h"
#include "types.h"
#include "app/comm_tvt/comm_tvt_sys.h"
#include "app/comm_tvt/ctvt_camera.h"
#include "app/comm_tvt/ctvt_comm.h"
#include "app/comm_tvt/ctvt_game.h"
#include "app/comm_tvt/ctvt_mic.h"
#include "app/comm_tvt/ima_adpcm.h"
#include "constants/sound.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/net_handle.h"
#include "gfl/random.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "nitro/memorymap.h"
#include "system/app_taskmenu.h"
#include "system/bmp_winframe.h"
#include "system/printsys.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "nitro/os.h"
#include "nnsys/gfd.h"

// The Xtransceiver's talk mode: the video chat itself. The member who holds the talk button records their voice,
// which is sent in chunks and played on every machine, with its waveform drawn on the bottom screen. The parent can
// also invite the others to a minigame, or end the call

#define CTVT_TALK_SPEEDS 9
#define CTVT_TALK_SPEED_DEFAULT 4
#define CTVT_TALK_COOLDOWN 8

// The keys that hold the talk button
#define CTVT_TALK_KEYS (PAD_BUTTON_A | PAD_BUTTON_R)

// The steps of the talk mode
enum {
    TALK_STATE_FADE_IN,
    TALK_STATE_WAIT_FADE_IN,
    TALK_STATE_FADE_OUT_TO_GAME,
    TALK_STATE_FADE_OUT,
    TALK_STATE_WAIT_FADE_OUT,
    TALK_STATE_MAIN,
    TALK_STATE_REQUEST_TALK,
    TALK_STATE_WAIT_TALKER,
    TALK_STATE_RECORD,
    TALK_STATE_ASK_EXIT,
    TALK_STATE_EXIT_MENU,
    TALK_STATE_SEND_EXIT,
    TALK_STATE_WAIT_EXIT,
    TALK_STATE_DISCONNECTED,
    TALK_STATE_WAIT_DISCONNECTED,
    TALK_STATE_SEND_PARENT_EXIT,
    TALK_STATE_PARENT_LEFT,
    TALK_STATE_WAIT_PARENT_LEFT,
    TALK_STATE_SYNC_EXIT,
    TALK_STATE_WAIT_SYNC_EXIT,
    TALK_STATE_ALONE,
    TALK_STATE_WAIT_ALONE,
};

// What the talk mode goes on to, in next
enum {
    TALK_NEXT_DRAW = 6,
    TALK_NEXT_EXIT,
};

// The steps of recording, in next while recording
enum {
    RECORD_START,
    RECORD_RECORDING,
    RECORD_SENDING,
    RECORD_SEND_DONE,
    RECORD_WAIT_PLAY,
    RECORD_WAIT_PLAY_END,
};

struct CtvtTalk {
    int state;
    int next;
    BOOL waitSound;
    u8 requestCooldown;
    u8 speed;
    u8 speedTouch;
    u8 speedRepeat;
    CtvtMic *mic;
    BOOL chunkPending;
    u8 chunk;
    void *voiceAlloc;
    CtvtVoicePacket *voicePacket;
    u8 *voiceData;
    BOOL messagePending;
    BmpWin *waveWindow;
    u8 waveX;
    u8 waveY;
    BOOL flashing;
    u8 flashFrame;
    u16 flashPalette[16];
    u16 normalPalette[16];
    BmpWin *messageWindow;
    AppTaskMenu *exitMenu;
    BOOL notifyExit;
    u16 waitFrames;
    BOOL waveShown;
    ClActor *speedCursor;
    ClActor *zoomButton;
    ClActor *talkButton;
    ClActor *exitButton;
    ClActor *voiceIcon;
    int voiceIconState;
    int shownVoiceIconState;
};

void func_ov172_021b9580(CommTvtWork *sys, CtvtTalk *talk);
void func_ov172_021b97ac(CommTvtWork *sys, CtvtTalk *talk);
void func_ov172_021b99b4(CommTvtWork *sys, CtvtTalk *talk);
void func_ov172_021b99f4(CommTvtWork *sys, CtvtTalk *talk);
void func_ov172_021b9ab8(CommTvtWork *sys, CtvtTalk *talk, u8 *pixels, u8 x0, u8 y0, u8 x1, u8 y1, u8 color);
void func_ov172_021b9b88(u8 *pixels, u8 x, u8 y, u8 color);
void func_ov172_021b9c1c(CommTvtWork *sys, CtvtTalk *talk, u32 msgId);
void func_ov172_021b9c9c(CommTvtWork *sys, CtvtTalk *talk);
void func_ov172_021b9d04(CommTvtWork *sys, CtvtTalk *talk);
BOOL func_ov172_021b9df8(CommTvtWork *sys, CtvtTalk *talk);
void func_ov172_021b9e3c(CommTvtWork *sys, CtvtTalk *talk);
void func_ov172_021b9e5c(CommTvtWork *sys, CtvtTalk *talk, const s16 *samples, u8 color, long pos, u16 size);
BOOL func_ov172_021b9f4c(CommTvtWork *sys, CtvtTalk *talk, BOOL held);
u8 func_ov172_021b8b5c(CommTvtWork *sys, CtvtComm *comm);
u8 func_ov172_021b8b68(CommTvtWork *sys, CtvtComm *comm);
BOOL func_ov172_021b77b4(CommTvtWork *sys, CtvtCamera *work);
void *func_ov172_021b8b84(CommTvtWork *sys, CtvtComm *comm);

static const TouchRect data_ov172_021c6f8f[] = {
    { 0x08, 0x28, 0x10, 0x48 },
    { 0x20, 0x4d, 0x00, 0x40 },
    { 0x4d, 0xa0, 0x00, 0x30 },
    { TOUCH_RECT_END },
};

// The masks that keep the other pixels of a byte of the waveform's 4bpp bitmap, and the pixel of each position
static const u16 data_ov172_021c6f66[] = { 0xfff0, 0xff0f, 0xf0ff, 0x0fff };

// How far the waveform may swing at the window's ends, so that it fits the rounded corners
// pokebw2's sWaveEdges
static const u8 data_ov172_021c6f6e[] = { 3, 7, 11, 14, 17, 21, 24, 32, 32 };

// pokebw2's CtvtTalk_Create
CtvtTalk *func_ov172_021b8c3c(CommTvtWork *sys, HeapID heapId) {
    static char sFile[] = "ctvt_talk.c";
    u8 i;
    CtvtTalk *talk = GFL_HeapAllocate(heapId, sizeof(CtvtTalk), TRUE, sFile, 215);

    talk->mic = func_ov172_021b9f9c(heapId);
    talk->voiceAlloc = GFL_HeapAllocate(heapId, 0x808, TRUE, sFile, 218);
    talk->voicePacket = talk->voiceAlloc;
    talk->voiceData = (u8 *)talk->voicePacket->data;
    talk->speed = CTVT_TALK_SPEED_DEFAULT;
    MIi_CpuCopy16((u16 *)HW_DB_BG_PLTT + 16, talk->normalPalette, sizeof(talk->normalPalette));
    for (i = 0; i < 16; i++) {
        talk->flashPalette[i] = 0x7fff;
    }
    return talk;
}

// pokebw2's CtvtTalk_Delete
void func_ov172_021b8cb0(CommTvtWork *sys, CtvtTalk *talk) {
    GFL_HeapFree(talk->voicePacket);
    func_ov172_021ba01c(talk->mic);
    GFL_HeapFree(talk);
}

// The resources of the actors are the system's: characters, palette and cells
static inline void CtvtTalk_CreateActor(CommTvtWork *sys, ClActor **actor, int chars, int palette, int cells,
                                        const ClActorSetup *setup, HeapID heapId) {
    *actor = func_0204b294(func_ov172_021be024(sys), func_ov172_021be018(sys, chars),
                            func_ov172_021be018(sys, palette), func_ov172_021be018(sys, cells), setup, 1, heapId);
}

void func_ov172_021b8cc8(CommTvtWork *sys, CtvtTalk *talk) {
    HeapID heapId = func_ov172_021be00c(sys);
    ArcTool *arc = func_ov172_021be010(sys);
    ClActorSetup setup;
    u8 talker;

    GFL_BGSysLoadArcNCGRStatic(arc, 0xb, 6, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 0x11, 6, 0, 0, FALSE, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 0xb, 5, 0, 0x5000, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 0xf, 5, 0, 0, FALSE, heapId);
    G2S_SetBlendAlpha(2, 0x14, 5, 0xf);
    GFL_BGSysLoadScr(6);
    GFL_BGSysLoadScr(5);

    setup.x = 0x80;
    setup.y = 0x60;
    setup.sequence = talk->speed + 0x19;
    setup.priority = 0;
    setup.bgPriority = 2;
    CtvtTalk_CreateActor(sys, &talk->speedCursor, 5, 1, 9, &setup, heapId);
    func_0204b3dc(talk->speedCursor, TRUE);
    if (func_ov172_021be0b4(sys) == TRUE) {
        setup.sequence = 0x15;
    } else {
        setup.sequence = 0x16;
    }
    CtvtTalk_CreateActor(sys, &talk->zoomButton, 5, 1, 9, &setup, heapId);
    func_0204b3dc(talk->zoomButton, TRUE);
    setup.sequence = 0x14;
    CtvtTalk_CreateActor(sys, &talk->exitButton, 5, 1, 9, &setup, heapId);
    func_0204b3dc(talk->exitButton, TRUE);
    setup.sequence = 0x18;
    CtvtTalk_CreateActor(sys, &talk->voiceIcon, 5, 1, 9, &setup, heapId);
    func_0204b3dc(talk->voiceIcon, TRUE);
    setup.x = 0xe8;
    setup.y = 0xa8;
    setup.sequence = 1;
    setup.priority = 0;
    setup.bgPriority = 0;
    CtvtTalk_CreateActor(sys, &talk->talkButton, 6, 2, 10, &setup, heapId);
    func_0204b3dc(talk->talkButton, TRUE);
    func_0204b92c(talk->talkButton, TRUE);

    talk->state = 0;
    talk->waitSound = FALSE;
    talk->voiceIconState = 0;
    talk->shownVoiceIconState = 3;
    talk->waveWindow = BmpWin_CreateDynamic(4, 8, 3, 0x10, 8, 10, TRUE);
    talk->messageWindow = BmpWin_CreateDynamic(4, 1, 1, 0x1e, 4, 10, TRUE);
    GFL_BitmapFill(BmpWin_GetBitmap(talk->waveWindow), 0);
    BmpWin_FlushMap(talk->waveWindow);
    BmpWin_FlushChar(talk->waveWindow);
    talk->exitMenu = NULL;
    talk->notifyExit = FALSE;
    talk->messagePending = FALSE;
    GFL_BGSysLoadScr(4);
    talk->flashFrame = 0;
    talk->flashing = FALSE;
    talk->waveShown = FALSE;
    talk->requestCooldown = 0;

    talker = func_ov172_021b8b68(sys, func_ov172_021bdfe4(sys));
    if (talker != 0xff) {
        func_ov172_021be210(sys, talker);
        talk->voiceIconState = 2;
    }
    if (!func_ov172_021ba270(talk->mic)) {
        talk->voiceIconState = 2;
    }
}


static inline void CtvtTalk_ClearWindow(BmpWin *window) {
    func_020456f8(window);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
}

// pokebw2's CtvtTalk_Leave
void func_ov172_021b8fb8(CommTvtWork *sys, CtvtTalk *talk) {
    if (talk->exitMenu != NULL) {
        func_02027858(talk->exitMenu);
    }
    CtvtTalk_ClearWindow(talk->waveWindow);
    CtvtTalk_ClearWindow(talk->messageWindow);
    BmpWin_Free(talk->waveWindow);
    BmpWin_Free(talk->messageWindow);
    func_0204b3b4(talk->talkButton);
    func_0204b3b4(talk->voiceIcon);
    func_0204b3b4(talk->exitButton);
    func_0204b3b4(talk->zoomButton);
    func_0204b3b4(talk->speedCursor);
    GFL_BGSysClearScr(6);
    GFL_BGSysLoadScr(6);
    reg_G2S_DB_BLDCNT = 0;
}

static const TouchRect data_ov172_021c6f77[3] = { { 0xa8, 0xc0, 0xe8, 0x00 }, { 0x68, 0xa0, 0xd8, 0xff }, { 0xff, 0x00, 0x00, 0x00 } };
static const TouchRect data_ov172_021c6f83[3] = { { 0x08, 0x58, 0xb8, 0xf0 }, { 0x28, 0x68, 0xd0, 0xff }, { 0xff, 0x00, 0x00, 0x00 } };

int func_ov172_021b9044(CommTvtWork *sys, CtvtTalk *talk) {
    // The fades' steps and frames per step: emitted although every read folds (the bytes 06 01 at the head
    // of the file's .rodata, which nothing points to)
    static const u8 wipeDivision = 6;
    static const u8 wipeSync = 1;
    HeapID heapId = func_ov172_021be00c(sys);
    CtvtCamera *camera;
    CtvtComm *comm;
    u8 talker;
    u8 self;
    s16 *samples;
    int pos;
    u32 size;
    u32 quotient;

    switch (talk->state) {
    case 0:
        if (func_ov172_021be02c(sys) == TRUE) {
            GFL_WipeSet(0, 1, 1, 0, wipeDivision, wipeSync, heapId);
        } else {
            GFL_WipeSet(4, 1, 1, 0, wipeDivision, wipeSync, heapId);
        }
        talk->state = 1;
        break;
    case 1:
        if (GFL_WipeIsFinished() == TRUE) {
            talk->state = 5;
        }
        break;
    case 2:
        GFL_WipeSet(4, 0, 0, 0, wipeDivision, wipeSync, heapId);
        func_ov172_021be030(sys, FALSE);
        talk->state = 4;
        break;
    case 3:
        if (talk->next != 6 || func_0204b98c(talk->exitButton) != TRUE) {
            camera = func_ov172_021bdfdc(sys);
            if (talk->waitSound == FALSE || func_ov172_021b7590(sys, camera) == TRUE) {
                GFL_WipeSet(0, 0, 0, 0, wipeDivision, wipeSync, heapId);
                func_ov172_021be030(sys, TRUE);
                talk->state = 4;
            }
        }
        break;
    case 4:
        if (GFL_WipeIsFinished() == TRUE) {
            if (talk->next == 6) {
                return 3;
            }
            return 4;
        }
        break;
    case 5:
        func_ov172_021b9580(sys, talk);
        break;
    case 6:
        if (func_ov172_021b9df8(sys, talk) == FALSE) {
            if (func_ov172_021b9f4c(sys, talk, TRUE) == TRUE || (GCTX_HIDGetHeldKeys() & 0x101) != 0) {
                comm = func_ov172_021bdfe4(sys);
                if (func_ov172_021b8740(sys, comm, 6, 0) == TRUE) {
                    talk->state = 7;
                }
            } else {
                talk->state = 5;
            }
        }
        break;
    case 7:
        if (func_ov172_021b9df8(sys, talk) == FALSE) {
            comm = func_ov172_021bdfe4(sys);
            talker = func_ov172_021b8b68(sys, comm);
            self = func_ov172_021b8b5c(sys, comm);
            if (func_ov172_021b9f4c(sys, talk, TRUE) == TRUE || (GCTX_HIDGetHeldKeys() & 0x101) != 0) {
                if (talker != 0xff) {
                    if (talker == self) {
                        talk->state = 8;
                        talk->next = 0;
                    } else {
                        talk->state = 5;
                    }
                }
            } else {
                talk->state = 5;
            }
        }
        break;
    case 8:
        func_ov172_021b97ac(sys, talk);
        break;
    case 9:
        func_ov172_021b9c9c(sys, talk);
        break;
    case 10:
        func_ov172_021b9d04(sys, talk);
        break;
    case 11:
        comm = func_ov172_021bdfe4(sys);
        if (func_ov172_021b8740(sys, comm, 10, 0) == TRUE) {
            talk->state = 12;
        }
        /* fall through */
    case 12:
        if (func_ov172_021be054(sys) <= 1) {
            camera = func_ov172_021bdfdc(sys);
            talk->next = 7;
            talk->state = 3;
            func_ov172_021be0b0(sys, TRUE);
            func_ov172_021b7578(sys, camera);
            func_ov172_021b7b80(sys, camera);
            talk->waitSound = TRUE;
        }
        break;
    case 13:
        comm = func_ov172_021bdfe4(sys);
        func_ov172_021b8084(sys, comm);
        func_ov172_021be0b0(sys, TRUE);
        func_ov172_021b9c1c(sys, talk, 36);
        talk->state = 14;
        camera = func_ov172_021bdfdc(sys);
        func_ov172_021b7b80(sys, camera);
        talk->waitSound = TRUE;
        break;
    case 14:
        if (func_02035c60() == TRUE || (GCTX_HIDGetPressedKeys() & 3) != 0) {
            talk->next = 7;
            talk->state = 3;
            camera = func_ov172_021bdfdc(sys);
            func_ov172_021b7578(sys, camera);
            talk->waitSound = TRUE;
        }
        break;
    case 15:
        comm = func_ov172_021bdfe4(sys);
        if (func_ov172_021b8740(sys, comm, 10, 0) == TRUE) {
            camera = func_ov172_021bdfdc(sys);
            func_ov172_021be0b0(sys, TRUE);
            func_ov172_021b7b80(sys, camera);
            talk->state = 18;
        }
        break;
    case 16:
        comm = func_ov172_021bdfe4(sys);
        func_ov172_021b8084(sys, comm);
        func_ov172_021b9c1c(sys, talk, 37);
        talk->state = 17;
        talk->waitFrames = 0;
        func_ov172_021be0b0(sys, TRUE);
        camera = func_ov172_021bdfdc(sys);
        func_ov172_021b7b80(sys, camera);
        talk->waitSound = TRUE;
        break;
    case 17:
        talk->waitFrames++;
        if (func_02035c60() == TRUE || (GCTX_HIDGetPressedKeys() & 3) != 0 || talk->waitFrames >= 300) {
            talk->state = 18;
        }
        break;
    case 18:
        comm = func_ov172_021bdfe4(sys);
        func_ov172_021b8b34(sys, comm, 47);
        talk->state = 19;
        break;
    case 19:
        comm = func_ov172_021bdfe4(sys);
        if (func_ov172_021b8b48(sys, comm, 47) == TRUE) {
            talk->next = 7;
            talk->state = 3;
            func_ov172_021be34c(sys);
            func_ov172_021be0b0(sys, TRUE);
            camera = func_ov172_021bdfdc(sys);
            func_ov172_021b7578(sys, camera);
            talk->waitSound = TRUE;
        }
        break;
    case 20:
        comm = func_ov172_021bdfe4(sys);
        func_ov172_021b8084(sys, comm);
        func_ov172_021b9c1c(sys, talk, 38);
        talk->state = 21;
        func_ov172_021be0b0(sys, TRUE);
        camera = func_ov172_021bdfdc(sys);
        func_ov172_021b7b80(sys, camera);
        talk->waitSound = TRUE;
        break;
    case 21:
        if (func_02035c60() == TRUE || (GCTX_HIDGetPressedKeys() & 3) != 0) {
            talk->next = 7;
            talk->state = 3;
            func_ov172_021be0b0(sys, TRUE);
            camera = func_ov172_021bdfdc(sys);
            func_ov172_021b7578(sys, camera);
            talk->waitSound = TRUE;
        }
        break;
    }

    func_ov172_021b99b4(sys, talk);
    func_ov172_021ba050(talk->mic);
    if (talk->messagePending == TRUE) {
        PrintQueue *queue = func_ov172_021be044(sys);
        GFLBitmap *bitmap = BmpWin_GetBitmap(talk->messageWindow);

        if (func_0201c6c4(queue, bitmap) == FALSE) {
            BmpWin_FlushChar(talk->messageWindow);
            BmpWin_FlushMap(talk->messageWindow);
            GFL_BGSysLoadScr(4);
            talk->messagePending = FALSE;
            if (talk->notifyExit == TRUE) {
                talk->notifyExit = FALSE;
                func_ov172_021be318(sys, talk->messageWindow);
            }
        }
    }
    if (talk->flashing == TRUE) {
        talk->flashFrame++;
        if (talk->flashFrame > 6) {
            talk->flashing = FALSE;
            NNS_GfdRegisterNewVramTransferTask(0x1f, 0x20, talk->normalPalette, 0x20);
        } else if (talk->flashFrame % 12 < 6) {
            NNS_GfdRegisterNewVramTransferTask(0x1f, 0x20, talk->flashPalette, 0x20);
        } else {
            NNS_GfdRegisterNewVramTransferTask(0x1f, 0x20, talk->normalPalette, 0x20);
        }
    }
    if (talk->waveShown == FALSE && talk->state != 8 && talk->state < 11 && func_ov172_021ba244(talk->mic) == TRUE) {
        func_ov172_021b9e3c(sys, talk);
        talk->waveShown = TRUE;
    }
    if (talk->waveShown == TRUE) {
        if (func_ov172_021ba244(talk->mic) == FALSE) {
            talk->waveShown = FALSE;
            GFL_BitmapFill(BmpWin_GetBitmap(talk->waveWindow), 0);
            BmpWin_FlushChar(talk->waveWindow);
        } else {
            u16 total;

            comm = func_ov172_021bdfe4(sys);
            samples = func_ov172_021b8b84(sys, comm);
            pos = func_ov172_021ba24c(talk->mic);
            total = func_ov172_021ba254(talk->mic);
            size = func_ov172_021ba248(talk->mic);
            if (pos > total) {
                pos = total;
            }
            quotient = (size * pos) / total;
            samples = (s16 *)((u8 *)samples + CTVT_MIC_SKIPPED_SIZE);
            func_ov172_021b9e5c(sys, talk, samples, 3, quotient, size);
        }
    }
    if (func_0204b98c(talk->talkButton) == FALSE) {
        if (talk->state == 10) {
            func_0204b878(talk->talkButton, 15);
        } else {
            func_0204b878(talk->talkButton, 1);
        }
    }
    return 1;
}

// pokebw2's CtvtTalk_Draw
void func_ov172_021b9574(CommTvtWork *sys, CtvtTalk *talk) {
    func_ov172_021ba094(talk->mic);
}

void func_ov172_021b9580(CommTvtWork *sys, CtvtTalk *talk) {
    CtvtComm *comm = func_ov172_021bdfe4(sys);
    u8 kind = func_ov172_021be054(sys);
    CtvtCamera *camera;
    u32 pressed;
    s32 hit;

    if (func_ov172_021b9df8(sys, talk) == TRUE) {
        return;
    }
    if (talk->state <= 8 && func_ov172_021be0b4(sys) == FALSE) {
        camera = func_ov172_021bdfdc(sys);
        if (GCTX_HIDGetPressedKeys() & PAD_KEY_UP) {
            func_ov172_021b7b3c(sys, camera, CAMERA_SIZE_DSI_VGA);
        }
        if (GCTX_HIDGetPressedKeys() & PAD_KEY_RIGHT) {
            func_ov172_021b7b3c(sys, camera, CAMERA_SIZE_DS_LCD);
        }
        if (GCTX_HIDGetPressedKeys() & PAD_KEY_DOWN) {
            func_ov172_021b7b3c(sys, camera, CAMERA_SIZE_QVGA);
        }
        if (GCTX_HIDGetPressedKeys() & PAD_KEY_LEFT) {
            func_ov172_021b7b3c(sys, camera, CAMERA_SIZE_CIF);
        }
    }

    if (kind == 1 || !func_ov172_021ba270(talk->mic)) {
        talk->voiceIconState = 2;
    } else {
        u8 self = func_ov172_021b8b5c(sys, comm);
        u8 talker = func_ov172_021b8b68(sys, comm);

        if (talker != 0xff) {
            if (talker == self) {
                if (talk->requestCooldown != 0) {
                    talk->requestCooldown--;
                } else if (func_ov172_021b8740(sys, comm, 9, 0) == TRUE) {
                    talk->requestCooldown = CTVT_TALK_COOLDOWN;
                }
            }
            talk->voiceIconState = 2;
        } else {
            BOOL touched = func_ov172_021b9f4c(sys, talk, FALSE);

            pressed = GCTX_HIDGetPressedKeys();
            if ((pressed & CTVT_TALK_KEYS) || touched == TRUE) {
                talk->state = 6;
                GFL_SndSEPlay(SEQ_SE_SYS_45);
                talk->flashing = TRUE;
                talk->flashFrame = 0;
            }
            talk->voiceIconState = 0;
        }
    }

    if (talk->state == 5) {
        if ((GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) || (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) ||
            func_02035c08(data_ov172_021c6f83) != TOUCH_RECT_NONE) {
            talk->state = 3;
            talk->next = 6;
            func_0204b878(talk->exitButton, 20);
            func_0204b92c(talk->exitButton, TRUE);
            GFL_SndSEPlay(SEQ_SE_SYS_45);
        }
    }

    if (talk->state == 5) {
        hit = func_02035c08(data_ov172_021c6f77);
        if (hit == 0 || (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B)) {
            if (hit == 0) {
                func_020355d8(TRUE);
            } else {
                func_020355d8(FALSE);
            }
            talk->state = 9;
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            func_0204b878(talk->talkButton, 9);
        } else if (hit == 1 || (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_SELECT | PAD_BUTTON_START))) {
            func_ov172_021be0b8(sys);
            GFL_SndSEPlay(SEQ_SE_SYS_48);
            if (func_ov172_021be0b4(sys) == TRUE) {
                func_0204b878(talk->zoomButton, 21);
            } else {
                func_0204b878(talk->zoomButton, 22);
            }
        }
    }

    if (func_ov172_021be0a4(sys) == 0 && (GCTX_HIDGetPressedKeys() & PAD_BUTTON_L)) {
        camera = func_ov172_021bdfdc(sys);
        if (func_ov172_021b77b4(sys, camera) == FALSE) {
            func_ov172_021be08c(sys);
            GFL_SndSEPlay(SEQ_SE_SYS_45);
        }
    }
    func_ov172_021b99f4(sys, talk);
}


// pokebw2's CtvtTalk_UpdateRecord
void func_ov172_021b97ac(CommTvtWork *sys, CtvtTalk *talk) {
    BOOL touched = func_ov172_021b9f4c(sys, talk, TRUE);
    CtvtComm *comm;
    s16 *samples;
    u32 pos;

    if (func_ov172_021b9df8(sys, talk) == TRUE) {
        return;
    }
    switch (talk->next) {
    case RECORD_START:
        if (func_ov172_021ba0bc(talk->mic) == TRUE) {
            talk->voiceIconState = 1;
            talk->next = RECORD_RECORDING;
            talk->chunk = 0;
            talk->chunkPending = FALSE;
            func_ov172_021b69e0();
            func_ov172_021b9e3c(sys, talk);
        }
        break;
    case RECORD_RECORDING:
        if (touched != TRUE && !(GCTX_HIDGetHeldKeys() & CTVT_TALK_KEYS)) {
            func_ov172_021ba120(talk->mic);
        }
        if (func_ov172_021ba158(talk->mic) == FALSE) {
            talk->next = RECORD_SENDING;
            talk->voiceIconState = 2;
        }
        samples = func_ov172_021ba170(talk->mic);
        pos = func_ov172_021ba15c(talk->mic);
        samples = (s16 *)((u8 *)samples + CTVT_MIC_SKIPPED_SIZE);
        func_ov172_021b9e5c(sys, talk, samples, 9, pos, 0xc000);
        // fallthrough
    case RECORD_SENDING: {
        BOOL busy;
        u32 size;

        comm = func_ov172_021bdfe4(sys);
        if (func_ov172_021b8c34(sys, comm) != TRUE) {
            break;
        }
        busy = func_ov172_021b8b74(sys, comm);
        size = func_ov172_021ba15c(talk->mic);
        if (talk->chunkPending == FALSE && busy == FALSE &&
            (size >= (talk->chunk + 1) * 0x800 || talk->next == RECORD_SENDING)) {
            u8 *buffer = func_ov172_021ba170(talk->mic);

            u32 encoded =
                func_ov172_021ba174(talk->mic, (const s16 *)(buffer + talk->chunk * 0x800), talk->voiceData, 0x800);

            talk->voicePacket->chunk = talk->chunk;
            talk->voicePacket->size = encoded;
            talk->voicePacket->playSize = size;
            talk->voicePacket->speed = talk->speed;
            if (talk->next == RECORD_SENDING && size <= (talk->chunk + 1) * 0x800) {
                talk->voicePacket->isLast = TRUE;
            } else {
                talk->voicePacket->isLast = FALSE;
            }
            talk->chunkPending = TRUE;
        }
        if (talk->chunkPending == TRUE && func_ov172_021b891c(sys, comm, talk->voicePacket) == TRUE) {
            talk->chunk++;
            talk->chunkPending = FALSE;
            if (talk->voicePacket->isLast == TRUE) {
                talk->next = RECORD_SEND_DONE;
            }
        }
        break;
    }
    case RECORD_SEND_DONE:
        comm = func_ov172_021bdfe4(sys);
        if (func_ov172_021b8b74(sys, comm) == FALSE &&
            func_ov172_021b8740(sys, comm, CTVT_PACKET_PLAY_VOICE, 0) == TRUE) {
            talk->next = RECORD_WAIT_PLAY;
            GFL_BitmapFill(BmpWin_GetBitmap(talk->waveWindow), 0);
            BmpWin_FlushChar(talk->waveWindow);
        }
        break;
    case RECORD_WAIT_PLAY:
        if (talk->voicePacket->playSize > 32) {
            if (func_ov172_021ba244(func_ov172_021bdfec(sys)) == TRUE) {
                talk->next = RECORD_WAIT_PLAY_END;
            }
        } else {
            talk->state = TALK_STATE_MAIN;
        }
        break;
    case RECORD_WAIT_PLAY_END:
        comm = func_ov172_021bdfe4(sys);
        if (func_ov172_021ba244(func_ov172_021bdfec(sys)) == FALSE &&
            func_ov172_021b8740(sys, comm, CTVT_PACKET_CANCEL_TALK, 0) == TRUE) {
            talk->state = TALK_STATE_MAIN;
        }
        break;
    }
}

// pokebw2's CtvtTalk_UpdateVoiceIcon
void func_ov172_021b99b4(CommTvtWork *sys, CtvtTalk *talk) {
    if (talk->voiceIconState != talk->shownVoiceIconState) {
        if (talk->voiceIconState == 1) {
            func_0204b878(talk->voiceIcon, 23);
        } else if (talk->voiceIconState == 2) {
            func_0204b878(talk->voiceIcon, 35);
        } else {
            func_0204b878(talk->voiceIcon, 24);
        }
    }
}

// pokebw2's CtvtTalk_UpdateSpeed
void func_ov172_021b99f4(CommTvtWork *sys, CtvtTalk *talk) {
    u32 trig = func_02035c08(data_ov172_021c6f8f);
    u32 cont = func_02035ba4(data_ov172_021c6f8f);
    u32 dir = TOUCH_RECT_NONE;
    BOOL changed = FALSE;

    if (trig != TOUCH_RECT_NONE) {
        talk->speedRepeat = 0;
        if (trig <= 1) {
            talk->speedTouch = 0;
            dir = 0;
        } else {
            talk->speedTouch = 1;
            dir = 1;
        }
    } else if (cont != TOUCH_RECT_NONE) {
        BOOL same = FALSE;

        if (cont <= 1) {
            if (talk->speedTouch == 0) {
                same = TRUE;
            }
        } else if (talk->speedTouch == 1) {
            same = TRUE;
        }
        if (same == TRUE) {
            talk->speedRepeat++;
            if (talk->speedRepeat >= 25) {
                dir = talk->speedTouch;
                talk->speedRepeat = 10;
            }
        } else {
            talk->speedTouch = 0xff;
        }
    } else {
        talk->speedTouch = 0xff;
    }

    if (dir == 0) {
        if (talk->speed < CTVT_TALK_SPEEDS - 1) {
            changed = TRUE;
            talk->speed++;
            func_0204b878(talk->speedCursor, talk->speed + 25);
        }
    } else if (dir == 1 && talk->speed != 0) {
        changed = TRUE;
        talk->speed--;
        func_0204b878(talk->speedCursor, talk->speed + 25);
    }
    if (changed == TRUE) {
        int pitch = (talk->speed - 4) * 64;

        GFL_SEPlayKeepVol(SEQ_SE_SYS_45, 1);
        GFL_SndPlayerSetParams(1, -1, pitch, -1);
    }
}

// pokebw2's CtvtTalk_DrawWaveLine
void func_ov172_021b9ab8(CommTvtWork *sys, CtvtTalk *talk, u8 *pixels, u8 x0, u8 y0, u8 x1, u8 y1, u8 color) {
    int dx = MATH_ABS(x0 - x1);
    int dy = MATH_ABS(y0 - y1);
    int steps;
    long stepX, stepY;
    long offsetX, offsetY;
    long i;

    steps = (dx > dy ? dx : dy) + 1;
    stepX = FX32_CONST(x1 - x0) / steps;
    stepY = FX32_CONST(y1 - y0) / steps;
    offsetX = 0;
    offsetY = 0;
    for (i = 0; i < steps; i++) {
        func_ov172_021b9b88(pixels, x0 + (offsetX >> FX32_SHIFT), y0 + (offsetY >> FX32_SHIFT), color);
        offsetX += stepX;
        offsetY += stepY;
    }
}

// The waveform's window is 16 x 8 tiles of 4bpp, two pixels to a byte
// pokebw2's CtvtTalk_PutWavePixel
void func_ov172_021b9b88(u8 *pixels, u8 x, u8 y, u8 color) {
    u8 px = x % 8;
    u8 pos = px % 4;
    u16 *tile = (u16 *)(pixels + (u16)(x / 8 + (y / 8) * 16) * 32);
    u8 py = y % 8;
    u16 *pixel = tile + (px / 4 + py * 2);
    u16 colorMasks[4] = { 0x0001, 0x0010, 0x0100, 0x1000 };

    if (pixel < (u16 *)(pixels + 0x1000) && pixel >= (u16 *)pixels) {
        *pixel = (*pixel & data_ov172_021c6f66[pos]) + colorMasks[pos] * color;
    }
}

// pokebw2's CtvtTalk_PrintMessage
void func_ov172_021b9c1c(CommTvtWork *sys, CtvtTalk *talk, u32 msgId) {
    Font *font = func_ov172_021be034(sys);
    MsgData *msgData = func_ov172_021be03c(sys);
    PrintQueue *queue = func_ov172_021be044(sys);
    StrBuf *str;

    func_0201c6ec(queue);
    GFL_BitmapFill(BmpWin_GetBitmap(talk->messageWindow), 15);
    str = GFL_MsgDataLoadStrbufNew(msgData, msgId);
    func_0201c724(queue, BmpWin_GetBitmap(talk->messageWindow), 0, 0, str, font, 0x440);
    GFL_StrBufFree(str);
    BmpWin_FlushChar(talk->messageWindow);
    BmpWin_FlushMap(talk->messageWindow);
    BmpWin_DrawFrame(talk->messageWindow, 1, 0x140, 9);
    talk->messagePending = TRUE;
}

// pokebw2's CtvtTalk_AskExit
void func_ov172_021b9c9c(CommTvtWork *sys, CtvtTalk *talk) {
    if (func_ov172_021b80e4(sys, func_ov172_021bdfe4(sys)) == CTVT_CONNECT_EXISTING) {
        func_ov172_021b9c1c(sys, talk, 32);
    } else if (func_ov172_021be0a4(sys) == 0 && func_ov172_021be054(sys) > 1) {
        func_ov172_021b9c1c(sys, talk, 33);
    } else {
        func_ov172_021b9c1c(sys, talk, 32);
    }
    talk->exitMenu = func_ov172_021be11c(sys);
    talk->state = TALK_STATE_EXIT_MENU;
    if (func_0204b98c(talk->talkButton) == FALSE) {
        func_0204b878(talk->talkButton, 15);
    }
}

// pokebw2's CtvtTalk_UpdateExitMenu
void func_ov172_021b9d04(CommTvtWork *sys, CtvtTalk *talk) {
    if (func_ov172_021b9df8(sys, talk) == TRUE) {
        func_02027858(talk->exitMenu);
        talk->exitMenu = NULL;
        return;
    }
    func_02027974(talk->exitMenu);
    if (func_020279e8(talk->exitMenu) != TRUE) {
        return;
    }
    if (func_02027a04(talk->exitMenu) == 0) {
        if (func_ov172_021b80e4(sys, func_ov172_021bdfe4(sys)) == CTVT_CONNECT_EXISTING) {
            talk->state = TALK_STATE_SEND_PARENT_EXIT;
            talk->notifyExit = TRUE;
            func_ov172_021b9c1c(sys, talk, 42);
        } else if (func_ov172_021be0a4(sys) == 0 && func_ov172_021be054(sys) > 1) {
            talk->state = TALK_STATE_SEND_EXIT;
        } else {
            CtvtCamera *camera;

            talk->next = TALK_NEXT_EXIT;
            talk->state = TALK_STATE_FADE_OUT;
            func_ov172_021be0b0(sys, TRUE);
            camera = func_ov172_021bdfdc(sys);
            func_ov172_021b7578(sys, camera);
            func_ov172_021b7b80(sys, camera);
            talk->waitSound = TRUE;
        }
    } else {
        talk->state = TALK_STATE_MAIN;
        func_0201f7a8(talk->messageWindow, 1);
        func_020456f8(talk->messageWindow);
        BmpWin_FlushMap(talk->waveWindow);
        GFL_BGSysLoadScr(4);
        func_0204b878(talk->talkButton, 1);
    }
    func_02027858(talk->exitMenu);
    talk->exitMenu = NULL;
}

BOOL func_ov172_021b9df8(CommTvtWork *sys, CtvtTalk *talk) {
    if (func_ov172_021be0d0(sys) == 1) {
        CtvtComm *comm = func_ov172_021bdfe4(sys);
        if (func_ov172_021b80e4(sys, comm) == CTVT_CONNECT_EXISTING) {
            talk->state = 0x10;
        } else {
            talk->state = 0xd;
        }
        return TRUE;
    }
    if (func_ov172_021be054(sys) <= 1) {
        talk->state = 0x14;
        return TRUE;
    }
    return FALSE;
}


// pokebw2's CtvtTalk_ResetWave
void func_ov172_021b9e3c(CommTvtWork *sys, CtvtTalk *talk) {
    talk->waveX = 3;
    talk->waveY = 32;
    GFL_BitmapFill(BmpWin_GetBitmap(talk->waveWindow), 0);
}

// pokebw2's CtvtTalk_DrawWave
void func_ov172_021b9e5c(CommTvtWork *sys, CtvtTalk *talk, const s16 *samples, u8 color, long pos, u16 size) {
    u32 end;
    u8 *pixels;
    BOOL drawn;
    u32 y;

    if (pos < 0) {
        return;
    }
    end = 123 * pos / size;
    pixels = GFL_BitmapGetPixelData(BmpWin_GetBitmap(talk->waveWindow));
    drawn = FALSE;
    if (talk->waveX < end) {
        drawn = TRUE;
        while (talk->waveX < end) {
            y = (samples[((talk->waveX + 1) * size / 128) / 2] + 0x8000) * 64 / 0x10000;
            if (talk->waveX < 11 && y < 32 - data_ov172_021c6f6e[talk->waveX - 3]) {
                y = 32 - data_ov172_021c6f6e[talk->waveX - 3];
            }
            if (talk->waveX > 114 && y < 32 - data_ov172_021c6f6e[(u8)(122 - talk->waveX)]) {
                y = 32 - data_ov172_021c6f6e[(u8)(122 - talk->waveX)];
            }
            func_ov172_021b9ab8(sys, talk, pixels, talk->waveX, talk->waveY, talk->waveX + 1, y, color);
            talk->waveX++;
            talk->waveY = y;
        }
    }
    if (drawn == TRUE) {
        BmpWin_FlushChar(talk->waveWindow);
    }
}

// pokebw2's CtvtTalk_IsTalkButtonTouched
BOOL func_ov172_021b9f4c(CommTvtWork *sys, CtvtTalk *talk, BOOL held) {
    u32 x, y;

    if (held == FALSE) {
        func_02035d20(&x, &y);
    } else {
        func_02035cb8(&x, &y);
    }
    if (y >= 104) {
        int dx = 128 - x;
        int dy = 104 - y;
        int dist = dx * dx + dy * dy;

        if (dist > 0x100 && dist < 0x1440) {
            return TRUE;
        }
    }
    return FALSE;
}

CtvtMic *func_ov172_021b9f98(CommTvtWork *sys, CtvtTalk *talk) {
    return talk->mic;
}
