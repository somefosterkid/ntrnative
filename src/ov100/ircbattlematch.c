// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov174/ircbattlematch.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/ircbattlematch.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "constants/version.h"
#include "field/delivery_irc.h"
#include "field/net_devirc.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/button_man.h"
#include "gfl/clact.h"
#include "gfl/fade.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/net_system.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "nitro/gx.h"
#include "nitro/memorymap.h"
#include "nitro/os.h"
#include "pml/poke_party.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "save/wifi_list.h"
#include "system/app_taskmenu.h"
#include "system/bmp_winframe.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/palanm.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "gfl/bmp.h"
#include "gfl/graphics.h"
#include "nitro/math.h"

// Overlay 174's ircbattlematch.c, named after its string: the infrared match screen of the infrared menu's battles,
// trades and friend codes, the musical and the Battle Subway. The players make an infrared connection, which shows
// the mode they chose; the screen then trades their modes, trainer data and parties, checks that they chose the same
// thing, and ends with the agreed mode in the parameter's result.

// The game's commands, from the high byte 0xc
enum {
    IRCMATCH_CMD_MODE = 0xc00,
    IRCMATCH_CMD_PLAYER_INFO,
    IRCMATCH_CMD_PARTY,
    IRCMATCH_CMD_FRIEND_DATA,
    IRCMATCH_CMD_MAC,
    IRCMATCH_CMD_ORDER,
    IRCMATCH_CMD_COUNT = 6,
};

// The numbers the machines synchronize on, in the order they come
enum {
    IRCMATCH_TIMING_COMMANDS = 0x78,
    IRCMATCH_TIMING_FRIEND_DATA,
    IRCMATCH_TIMING_PARTY,
    IRCMATCH_TIMING_PLAYER_INFO,
    IRCMATCH_TIMING_MODE,
    IRCMATCH_TIMING_MAC,
    IRCMATCH_TIMING_ORDER,
    IRCMATCH_TIMING_RESULT,
    IRCMATCH_TIMING_JOIN_AVENUE,
};
#define IRCMATCH_TIMING_GROUP 12

// The archive of the screen's own graphics (not named: our name would be ours alone)
#define IRCMATCH_ARCID 28

// The messages of TEXT_BANK_STARTMENU_2 (the infrared menu's, bank 62 of the system messages) that the screen shows
enum {
    IRCMATCH_MSG_BACK = 2,
    IRCMATCH_MSG_CONNECT_OPPONENTS = 8,
    IRCMATCH_MSG_CONNECT_PARTNER = 9,
    IRCMATCH_MSG_CONNECT_LEADER = 11,
    IRCMATCH_MSG_WAIT = 12,
    IRCMATCH_MSG_QUIT_CONFIRM = 15,
    IRCMATCH_MSG_CONNECT_TRADE_PARTNER = 16,
    IRCMATCH_MSG_WAIT_MOMENT = 23,
    IRCMATCH_MSG_MISMATCH = 24,
    IRCMATCH_MSG_YES = 26,
    IRCMATCH_MSG_NO = 27,
    IRCMATCH_MSG_TRADE_STARTING = 29,
    IRCMATCH_MSG_BATTLE_STARTING = 30,
    IRCMATCH_MSG_CONNECT_FRIEND_CODE = 32,
    IRCMATCH_MSG_FRIEND_ROSTER_FULL = 33,
    IRCMATCH_MSG_FRIEND_COUNT = 34,
    IRCMATCH_MSG_CONFIRM = 35,
    IRCMATCH_MSG_MEMBERS_OK = 36,
    IRCMATCH_MSG_CONNECT_ALL = 37,
    IRCMATCH_MSG_CONNECT_SHOW_LEADER = 38,
    IRCMATCH_MSG_MEMBER_COUNT = 39,
    IRCMATCH_MSG_FRIEND_REGISTERED = 42,
    IRCMATCH_MSG_FRIEND_UPDATED = 43,
    IRCMATCH_MSG_CONNECT_PLAY_FRIEND = 46,
};

// The sprites' slots
enum {
    IRCMATCH_ACTOR_0,
    IRCMATCH_ACTOR_1,
    IRCMATCH_ACTOR_2,
    IRCMATCH_ACTOR_3,
    IRCMATCH_ACTOR_4,
    IRCMATCH_ACTOR_5,
    IRCMATCH_ACTOR_6,
    IRCMATCH_ACTOR_7,
    IRCMATCH_ACTOR_COUNT,
};

typedef struct IrcBattleMatch IrcBattleMatch;
typedef void (*IrcBattleMatchState)(IrcBattleMatch *work);

// A message of the bank, which is passed by address
typedef struct {
    u32 msgId;
} IrcBattleMatchMsg;

struct IrcBattleMatch {
    // The colors of BG 0's palette, then the same colors backwards, which the screen cycles through
    u16 palettes[18][16];
    u16 paletteFrame;
    HeapID paletteHeapId;
    IrcBattleMatchParam *param;
    IrcBattleMatchState state;
    s32 mode;
    // The mode each player sent, by net ID
    s32 modes[4];
    // The MAC addresses of the infrared players, by net ID
    u8 macs[4][6];
    HeapID heapId;
    BmpWin *messageWindow;
    BmpWin *countWindow;
    u32 unk284;
    MsgData *msgData;
    WordSet *wordSet;
    Font *font;
    StrBuf *strbuf;
    StrBuf *strbufFormatted;
    u32 unk29C[5];
    TCB *vblankTcb;
    ClActUnit *actUnit;
    u32 actorPalette;
    u32 actorChars;
    u32 actorCells;
    u32 unk2C4;
    ClActor *actors[IRCMATCH_ACTOR_COUNT];
    // The players that the network accepted
    u32 acceptedPlayers;
    BOOL enoughPlayers;
    u32 unk2F0[3];
    u32 bgChars;
    u32 unk300;
    u32 windowFrameChars;
    u32 menuBgChars;
    TCB *unk30C;
    u32 unk310;
    u8 unk314[0xc7c - 0x314];
    // Whether the link is up, as the network's callback says
    BOOL linked;
    BOOL messagePending;
    BOOL bannerActive;
    s32 bannerFrames;
    BOOL linkChanged;
    u32 friendResult;
    BOOL linkChangePending;
    s32 linkChangeFrames;
    s32 scrollY;
    BOOL connectDone;
    ButtonMan *buttonMan;
    u32 unkCA8;
    s32 counter;
    u32 currentMsgId;
    void *paletteBackup;
    PrintQueue *printQueue;
    // Whether the menu has only the Back button
    BOOL backOnly;
    AppTaskMenu *menu;
    AppTaskMenuRes *menuRes;
    AppTaskMenuItem menuItems[2];
    AppTaskMenuWin *taskWin0;
    AppTaskMenuWin *taskWin1;
    // The member count that the window shows
    s32 shownCount;
    BOOL palettePulse;
};

BOOL func_ov100_021b95ac(int a, int b);
static void *func_ov100_021b95a0(void *work);
int func_ov100_021b95a8(void *work);
static void func_ov100_021b95b8(IrcBattleMatch *work, IrcBattleMatchState state);
static void func_ov100_021b95c0(IrcBattleMatch *work);
static void func_ov100_021b95d8(IrcBattleMatch *work);
static void func_ov100_021b95f0(IrcBattleMatch *work, u16 buffer, u32 count);
static void func_ov100_021b967c(IrcBattleMatch *work, BOOL dim);
static void func_ov100_021b96b8(IrcBattleMatch *work);
void func_ov100_021b96ec(IrcBattleMatch *work);
void func_ov100_021b974c(IrcBattleMatch *work, s16 x, s16 y, int slot, int sequence);
void func_ov100_021b97bc(IrcBattleMatch *work, int dx, int dy, int slot);
void func_ov100_021b9800(IrcBattleMatch *work, s16 x, s16 y, int slot);
void func_ov100_021b9824(void *work_, int netId);
void func_ov100_021b9848(void *work_);
void func_ov100_021b9888(int netId, int size, const void *data, void *work_, NetHandle *handle);
static void func_ov100_021b98a8(int netId, int size, const void *data, void *work_, NetHandle *handle);
void func_ov100_021b98f0(int netId, int size, const void *data, void *work_, NetHandle *handle);
void *func_ov100_021b98f4(int netId, void *work_, int size);
void func_ov100_021b9910(int netId, int size, const void *data, void *work_, NetHandle *handle);
static void func_ov100_021b9950(int netId, int size, const void *data, void *work_, NetHandle *handle);
void func_ov100_021b99a8(int netId, int size, const void *data, void *work_, NetHandle *handle);
void func_ov100_021b9a1c(IrcBattleMatch *work);
void func_ov100_021b9a54(IrcBattleMatch *work);
int func_ov100_021b9ab4(IrcBattleMatch *work);
static void func_ov100_021b9ad8(IrcBattleMatch *work);
void func_ov100_021b9b7c(IrcBattleMatch *work);
void func_ov100_021b9be4(IrcBattleMatch *work);
void func_ov100_021b9c50(IrcBattleMatch *work);
static void func_ov100_021b9cc4(IrcBattleMatch *work);
static void func_ov100_021b9d6c(IrcBattleMatch *work);
static void func_ov100_021b9db8(IrcBattleMatch *work);
static void func_ov100_021b9e14(IrcBattleMatch *work);
static void func_ov100_021b9ef8(IrcBattleMatch *work);
void func_ov100_021b9f80(IrcBattleMatch *work);
void func_ov100_021b95c0(IrcBattleMatch *work);
void func_ov100_021b99f4(IrcBattleMatch *work);
void func_ov100_021b99d4(IrcBattleMatch *work);
static void func_ov100_021b9fbc(void *work_);
void func_ov100_021b9ff4(void *work_, BOOL linked);
static void func_ov100_021ba028(IrcBattleMatch *work);
void func_ov100_021ba1c8(TCB *tcb, void *data);
static void func_ov100_021ba1d0(IrcBattleMatch *work);
static void func_ov100_021ba290(IrcBattleMatch *work, BOOL withConfirm);
static void func_ov100_021ba3a4(IrcBattleMatchMsg *msg, IrcBattleMatch *work);
static void func_ov100_021ba454(u32 msgId, IrcBattleMatch *work);
static void func_ov100_021ba530(u32 msgId, IrcBattleMatch *work);
static void func_ov100_021ba5f4(IrcBattleMatch *work);
void func_ov100_021ba624(IrcBattleMatch *work);
void func_ov100_021ba640(IrcBattleMatch *work);
void func_ov100_021ba64c(IrcBattleMatch *work, int slot);
static void func_ov100_021ba668(IrcBattleMatch *work);
static void func_ov100_021ba780(IrcBattleMatch *work);
void func_ov100_021baa9c(IrcBattleMatch *work);
BOOL func_ov100_021baaec(int a, int b);
static void func_ov100_021bab08(IrcBattleMatch *work);
static void func_ov100_021bac08(IrcBattleMatch *work);
void func_ov100_021bac80(IrcBattleMatch *work);
void func_ov100_021bacc8(IrcBattleMatch *work);
static void func_ov100_021bad1c(IrcBattleMatch *work);
void func_ov100_021bad78(IrcBattleMatch *work);
static void func_ov100_021badb0(IrcBattleMatch *work);
void func_ov100_021badfc(IrcBattleMatch *work);
void func_ov100_021bae14(IrcBattleMatch *work);
static void func_ov100_021bae5c(IrcBattleMatch *work);
void func_ov100_021baeb4(IrcBattleMatch *work);
void func_ov100_021baee0(IrcBattleMatch *work);
void func_ov100_021baf4c(IrcBattleMatch *work);
void func_ov100_021bafa0(IrcBattleMatch *work);
void func_ov100_021bafcc(IrcBattleMatch *work);
static void func_ov100_021baffc(IrcBattleMatch *work);
void func_ov100_021bb058(IrcBattleMatch *work);
void func_ov100_021bb0c4(uintptr_t param, fx32 frame);
void func_ov100_021bb10c(uintptr_t param, fx32 frame);
void func_ov100_021bb154(IrcBattleMatch *work);
static void func_ov100_021bb180(IrcBattleMatch *work);
static void func_ov100_021bb270(IrcBattleMatch *work);
BOOL func_ov100_021bb414(GameProc *proc, u32 *state, void *param, void *work_);
static BOOL func_ov100_021bb498(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov100_021bb584(GameProc *proc, u32 *state, void *param, void *work_);

// What the players' beacons say, which is the same for every mode
// pokebw2's sIrcBattleMatchBeaconData
static u32 data_ov100_021bb800 = 11;

// Black keeps the message number here as a word; the value is 0xf
static const u32 data_ov100_021bb65c = 0xf;

static const NetCommand data_ov100_021bb6c0[IRCMATCH_CMD_COUNT] = {
    { func_ov100_021b9888, NULL },
    { func_ov100_021b98a8, NULL },
    { func_ov100_021b98f0, func_ov100_021b98f4 },
    { func_ov100_021b9910, NULL },
    { func_ov100_021b9950, NULL },
    { func_ov100_021b99a8, NULL },
};

// pokebw2's sIrcBattleMatchNetInit
static GFLNetInitData data_ov100_021bb83c = {
    NULL,
    0,
    NULL,
    func_ov100_021b9824,
    NULL,
    NULL,
    func_ov100_021b95a0,
    func_ov100_021b95a8,
    func_ov100_021b95ac,
    NULL,
    NULL,
    NULL,
    NULL,
    { 0 },
    NULL,
    NULL,
    0,
    { 1, 0, 0, 0, 0x80, 0x13, 0, 0 },
    HEAPID_USER,
    0xd,
    0xf,
    0x10,
    0xf0,
    0,
    2,
    0x64,
    0x10,
    1,
    0,
    3,
    1,
    0xb,
    { 0xfe, 0xff, 0, 0 },
    0,
    0,
};

// pokebw2's IrcBattleMatch_GetBeaconData
static void *func_ov100_021b95a0(void *work) {
    return &data_ov100_021bb800;
}

// pokebw2's IrcBattleMatch_GetBeaconDataSize
int func_ov100_021b95a8(void *work) {
    return sizeof(data_ov100_021bb800);
}

// pokebw2's IrcBattleMatch_IsSameGame
BOOL func_ov100_021b95ac(int a, int b) {
    return a == b;
}

// pokebw2's IrcBattleMatch_SetState
static void func_ov100_021b95b8(IrcBattleMatch *work, IrcBattleMatchState state) {
    work->state = state;
}

// Ends the screen as quit, and the network with it
// pokebw2's IrcBattleMatch_Abort
static void func_ov100_021b95c0(IrcBattleMatch *work) {
    work->param->result = IRCBATTLEMATCH_RESULT_QUIT;
    func_0203d0d0();
    func_0203ceac(NULL);
}

// pokebw2's IrcBattleMatch_StateWaitWipe
static void func_ov100_021b95d8(IrcBattleMatch *work) {
    if (GFL_WipeIsFinished()) {
        func_ov100_021b95b8(work, NULL);
    }
}

// Dims count palettes of a palette memory of the sub screen
// pokebw2's IrcBattleMatch_FadeSubPalettes
static void func_ov100_021b95f0(IrcBattleMatch *work, u16 buffer, u32 count) {
    PaletteFade *fade = func_02021518(work->heapId);
    u32 size;
    u16 *faded;

    func_0202155c(fade, buffer, 0x200, work->heapId);
    size = count * 32;
    func_020216d4(fade, buffer, 0, size);
    func_02021fd8(fade, buffer, 0, count * 16, 6, 0);
    faded = func_02021730(fade, buffer);
    DC_FlushRange(faded, size);
    switch (buffer) {
    case PALFADE_BUFFER_SUB_OBJ:
        GXS_LoadOBJPltt(faded, 0, size);
        break;
    case PALFADE_BUFFER_SUB_BG:
        GXS_LoadBGPltt(faded, 0, size);
        break;
    }
    func_020215a0(fade, buffer);
    func_02021540(fade);
}

// Dims the sub screen's BG palette behind a menu, or brings it back
// pokebw2's IrcBattleMatch_DimBackground
static void func_ov100_021b967c(IrcBattleMatch *work, BOOL dim) {
    if (dim) {
        MI_CpuCopy8((void *)HW_DB_BG_PLTT, work->paletteBackup, 0x200);
        func_ov100_021b95f0(work, PALFADE_BUFFER_SUB_BG, 9);
    } else {
        GXS_LoadBGPltt(work->paletteBackup, 0, 0x120);
    }
}

// pokebw2's IrcBattleMatch_StartWipeOut
static void func_ov100_021b96b8(IrcBattleMatch *work) {
    GFL_WipeSet(0, 0, 0, 0, 6, 1, work->heapId);
    func_ov100_021b95b8(work, func_ov100_021b95d8);
}

// pokebw2's IrcBattleMatch_LoadActorResources
void func_ov100_021b96ec(IrcBattleMatch *work) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(IRCMATCH_ARCID, work->heapId);

    work->actorChars = func_0204a6c8(arc, 7, FALSE, CLACT_VRAM_MAIN, work->heapId);
    work->actorPalette = func_0204ac18(arc, 3, CLACT_VRAM_MAIN, 0, 0, 3, work->heapId);
    work->actorCells = func_0204af28(arc, 9, 15, work->heapId);
    GFL_ArcToolFree(arc);
}

// pokebw2's IrcBattleMatch_CreateActor
void func_ov100_021b974c(IrcBattleMatch *work, s16 x, s16 y, int slot, int sequence) {
    ClActorSetup setup;

    if (work->actors[slot] == NULL) {
        setup.x = x;
        setup.y = y;
        setup.sequence = sequence;
        setup.priority = slot;
        setup.bgPriority = 2;
        work->actors[slot] = func_0204b294(work->actUnit, work->actorChars, work->actorPalette, work->actorCells,
                                           &setup, CLACT_SURFACE_MAIN, work->heapId);
        func_0204b92c(work->actors[slot], TRUE);
        func_0204b3dc(work->actors[slot], TRUE);
        func_0204b570(work->actors[slot], 2);
    }
}

// pokebw2's IrcBattleMatch_MoveActor
void func_ov100_021b97bc(IrcBattleMatch *work, int dx, int dy, int slot) {
    ClActorPos pos;

    if (work->actors[slot] != NULL) {
        func_0204b454(work->actors[slot], &pos, CLACT_SURFACE_MAIN);
        pos.x += (s16)dx;
        pos.y += (s16)dy;
        func_0204b404(work->actors[slot], &pos, CLACT_SURFACE_MAIN);
    }
}

// pokebw2's IrcBattleMatch_SetActorPos
void func_ov100_021b9800(IrcBattleMatch *work, s16 x, s16 y, int slot) {
    ClActorPos pos;

    if (work->actors[slot] != NULL) {
        pos.x = x;
        pos.y = y;
        func_0204b404(work->actors[slot], &pos, CLACT_SURFACE_MAIN);
    }
}

// Called when the network accepts a player
// pokebw2's IrcBattleMatch_OnNegotiated
void func_ov100_021b9824(void *work_, int netId) {
    IrcBattleMatch *work = work_;

    work->acceptedPlayers |= 1 << netId;
    if (MATH_CountPopulation(work->acceptedPlayers) >= 2) {
        work->enoughPlayers = TRUE;
    }
}

// Called when the connection is complete, which starts the banner of the starting message
// pokebw2's IrcBattleMatch_OnConnectDone
void func_ov100_021b9848(void *work_) {
    IrcBattleMatch *work = work_;

    if (work->mode != IRCBATTLEMATCH_MODE_MULTI) {
        work->messagePending = TRUE;
        work->bannerActive = TRUE;
        work->connectDone = TRUE;
    } else if (work->linked != 0 && work->linkChangeFrames != 0) {
        work->messagePending = TRUE;
        work->bannerActive = TRUE;
    }
}

// pokebw2's IrcBattleMatch_RecvMode
void func_ov100_021b9888(int netId, int size, const void *data, void *work_, NetHandle *handle) {
    IrcBattleMatch *work = work_;

    if (handle == func_020399a8()) {
        work->modes[netId] = *(const u32 *)data;
    }
}

// pokebw2's IrcBattleMatch_RecvPlayerInfo
static void func_ov100_021b98a8(int netId, int size, const void *data, void *work_, NetHandle *handle) {
    IrcBattleMatch *work = work_;
    PlayerInfo *info;
    RivalDataSave *rival;

    if (handle == func_020399a8()) {
        info = func_02012950(work->param->gameData, netId);
        func_02008494(data, info);
        rival = getHollow_RivalData(GameData_GetSaveControl(work->param->gameData));
        func_0200ed54(rival, getIDAsUInt(info));
    }
}

// The party goes to the buffer that func_ov100_021b98f4 gave
// pokebw2's IrcBattleMatch_RecvParty
void func_ov100_021b98f0(int netId, int size, const void *data, void *work_, NetHandle *handle) {
}

// pokebw2's IrcBattleMatch_GetPartyBuffer
void *func_ov100_021b98f4(int netId, void *work_, int size) {
    IrcBattleMatch *work = work_;

    if (netId >= 0 && netId < 4) {
        return work->param->parties[netId];
    }
    return NULL;
}

// pokebw2's IrcBattleMatch_RecvFriendData
void func_ov100_021b9910(int netId, int size, const void *data, void *work_, NetHandle *handle) {
    IrcBattleMatch *work = work_;
    GameData *gameData = work->param->gameData;

    if (handle == func_020399a8()) {
        if (netId != func_02039338()) {
            PlayerInfo *info = func_02012950(gameData, netId);

            work->friendResult = func_0200f5ec(gameData, info, data, work->heapId);
        }
    }
}

// Finds which machine the MAC address is, to put its player in order
// pokebw2's IrcBattleMatch_RecvMac
static void func_ov100_021b9950(int netId, int size, const void *data, void *work_, NetHandle *handle) {
    IrcBattleMatch *work = work_;
    int i;

    if (handle == func_020399a8()) {
        if (func_0203d378()) {
            for (i = 0; i < 4; i++) {
                if (GFL_STD_MemCmp(work->macs[i], data, 6) == 0) {
                    work->param->order[netId] = i;
                    return;
                }
            }
        }
    }
}

// pokebw2's IrcBattleMatch_RecvOrder
void func_ov100_021b99a8(int netId, int size, const void *data, void *work_, NetHandle *handle) {
    IrcBattleMatch *work = work_;

    if (handle == func_020399a8()) {
        if (netId != func_02039338()) {
            MI_CpuCopy8(data, work->param->order, sizeof(work->param->order));
        }
    }
}

void func_ov100_021b99d4(IrcBattleMatch *work) {
    if (func_02035c60()) {
        func_ov100_021b95c0(work);
        func_ov100_021b95b8(work, func_ov100_021b96b8);
    }
}

void func_ov100_021b99f4(IrcBattleMatch *work) {
    work->counter++;
    if (work->counter > 60) {
        func_0203ceac(0);
        func_ov100_021b95b8(work, func_ov100_021b99d4);
    }
}


// pokebw2's IrcBattleMatch_StateWaitResultSync
void func_ov100_021b9a1c(IrcBattleMatch *work) {
    if (func_02039cb4(func_020399a8(), 0x7f, 0xc)) {
        func_0203d7b0(0);
        func_0203d7bc(0);
        work->counter = 0;
        func_ov100_021b95b8(work, func_ov100_021b99f4);
    }
}

// Shows whether the friend was registered or updated
// pokebw2's IrcBattleMatch_StateShowFriendResult
void func_ov100_021b9a54(IrcBattleMatch *work) {
    IrcBattleMatchMsg *msg;

    if (work->friendResult != 0) {
        IrcBattleMatchMsg registered = { IRCMATCH_MSG_FRIEND_REGISTERED };

        msg = &registered;
    } else {
        IrcBattleMatchMsg updated = { IRCMATCH_MSG_FRIEND_UPDATED };

        msg = &updated;
    }
    func_ov100_021ba3a4(msg, work);
    func_ov100_021ba454(IRCMATCH_MSG_FRIEND_COUNT, work);
    GFL_SndSEPlay(SEQ_SE_FLD_124);
    func_02039c68(func_020399a8(), IRCMATCH_TIMING_RESULT, IRCMATCH_TIMING_GROUP);
    func_ov100_021b95b8(work, func_ov100_021b9a1c);
}

// How many Pokémon the smaller of the first two parties can battle with
// pokebw2's IrcBattleMatch_GetUsablePartySize
int func_ov100_021b9ab4(IrcBattleMatch *work) {
    int a = howManyPokesAreAbleToFight(work->param->parties[0]);
    int b = howManyPokesAreAbleToFight(work->param->parties[1]);

    if (a > b) {
        return b;
    }
    return a;
}

// pokebw2's IrcBattleMatch_StateWaitPartySync
static void func_ov100_021b9ad8(IrcBattleMatch *work) {
    if (func_02039cb4(func_020399a8(), IRCMATCH_TIMING_PARTY, IRCMATCH_TIMING_GROUP)) {
        switch (work->mode) {
        case IRCBATTLEMATCH_MODE_FRIEND:
            func_ov100_021b95b8(work, func_ov100_021b9a54);
            return;
        case IRCBATTLEMATCH_MODE_SINGLE:
        case IRCBATTLEMATCH_MODE_DOUBLE:
        case IRCBATTLEMATCH_MODE_TRIPLE:
        case IRCBATTLEMATCH_MODE_ROTATION:
            PokeParty_RecoverAll(work->param->parties[0]);
            PokeParty_RecoverAll(work->param->parties[1]);
            switch (func_ov100_021b9ab4(work)) {
            case 0:
                break;
            case 1:
                work->param->result = IRCBATTLEMATCH_MODE_SINGLE;
                break;
            case 2:
                if (work->param->result > IRCBATTLEMATCH_MODE_DOUBLE) {
                    work->param->result = IRCBATTLEMATCH_MODE_DOUBLE;
                }
                break;
            }
            func_ov100_021b95b8(work, func_ov100_021b96b8);
            break;
        }
        func_ov100_021b95b8(work, func_ov100_021b96b8);
    }
}

// pokebw2's IrcBattleMatch_StateSendParty
void func_ov100_021b9b7c(IrcBattleMatch *work) {
    if (func_02039cb4(func_020399a8(), IRCMATCH_TIMING_ORDER, IRCMATCH_TIMING_GROUP)) {
        NetHandle *handle = func_020399a8();

        if (func_0203d3f8(handle, 0xff, IRCMATCH_CMD_PARTY, PokeParty_GetSaveDataSize(), work->param->party, 0, FALSE,
                          TRUE)) {
            func_02039c68(func_020399a8(), IRCMATCH_TIMING_PARTY, IRCMATCH_TIMING_GROUP);
            func_ov100_021b95b8(work, func_ov100_021b9ad8);
        }
    }
}

// pokebw2's IrcBattleMatch_StateSendOrder
void func_ov100_021b9be4(IrcBattleMatch *work) {
    if (func_02039cb4(func_020399a8(), IRCMATCH_TIMING_MAC, IRCMATCH_TIMING_GROUP)) {
        if (func_0203d378()) {
            if (func_0203d3a8(func_020399a8(), IRCMATCH_CMD_ORDER, sizeof(work->param->order), work->param->order)) {
                func_02039c68(func_020399a8(), IRCMATCH_TIMING_ORDER, IRCMATCH_TIMING_GROUP);
                func_ov100_021b95b8(work, func_ov100_021b9b7c);
            }
        } else {
            func_02039c68(func_020399a8(), IRCMATCH_TIMING_ORDER, IRCMATCH_TIMING_GROUP);
            func_ov100_021b95b8(work, func_ov100_021b9b7c);
        }
    }
}

// pokebw2's IrcBattleMatch_StateSendMac
void func_ov100_021b9c50(IrcBattleMatch *work) {
    u8 mac[6];

    if (func_02039cb4(func_020399a8(), IRCMATCH_TIMING_FRIEND_DATA, IRCMATCH_TIMING_GROUP)) {
        switch (work->mode) {
        case IRCBATTLEMATCH_MODE_MUSICAL_ARG0:
        case IRCBATTLEMATCH_MODE_MUSICAL_ARG1:
        case IRCBATTLEMATCH_MODE_BSUBWAY:
            func_ov100_021b95b8(work, func_ov100_021b96b8);
            return;
        }
        OS_GetMacAddress(mac);
        if (func_0203d3a8(func_020399a8(), IRCMATCH_CMD_MAC, sizeof(mac), mac)) {
            func_02039c68(func_020399a8(), IRCMATCH_TIMING_MAC, IRCMATCH_TIMING_GROUP);
            func_ov100_021b95b8(work, func_ov100_021b9be4);
        }
    }
}

// pokebw2's IrcBattleMatch_StateCheckVersions
void func_ov100_021b9cc4(IrcBattleMatch *work) {
    if (func_02039cb4(func_020399a8(), 0x7b, 0xc)) {
        if (work->mode == 7) {
            WifiList *wifiList = func_0201283c(work->param->gameData);
            DWCFriendData token;

            func_02009ca8(&wifiList->myUserData, &token);
            if (func_0203d3a8(func_020399a8(), 0xc03, 0xc, &token)) {
                func_02039c68(func_020399a8(), 0x79, 0xc);
                func_ov100_021b95b8(work, func_ov100_021b9c50);
                if (work->menu != NULL) {
                    func_02027858(work->menu);
                    work->menu = NULL;
                }
            }
        } else {
            func_02039c68(func_020399a8(), 0x79, 0xc);
            if (work->menu != NULL) {
                func_02027858(work->menu);
                work->menu = NULL;
            }
            func_ov100_021b95b8(work, func_ov100_021b9c50);
        }
    }
}


// pokebw2's IrcBattleMatch_StateSendPlayerInfo
static void func_ov100_021b9d6c(IrcBattleMatch *work) {
    NetHandle *handle = func_020399a8();
    u32 size = func_02008468();

    if (func_0203d3a8(handle, IRCMATCH_CMD_PLAYER_INFO, size, GetGameDataPlayerInfo(work->param->gameData))) {
        func_02039c68(func_020399a8(), IRCMATCH_TIMING_PLAYER_INFO, IRCMATCH_TIMING_GROUP);
        func_ov100_021b95b8(work, func_ov100_021b9cc4);
    }
}

// The battles go on with the lowest mode of the two players
// pokebw2's IrcBattleMatch_StateWaitModeSync
static void func_ov100_021b9db8(IrcBattleMatch *work) {
    u16 netId;

    if (func_02039cb4(func_020399a8(), IRCMATCH_TIMING_MODE, IRCMATCH_TIMING_GROUP)) {
        netId = func_02039338();
        switch (work->mode) {
        case IRCBATTLEMATCH_MODE_SINGLE:
        case IRCBATTLEMATCH_MODE_DOUBLE:
        case IRCBATTLEMATCH_MODE_TRIPLE:
        case IRCBATTLEMATCH_MODE_ROTATION:
            if (work->mode > work->modes[1 - netId]) {
                work->param->result = work->modes[1 - netId];
            }
            break;
        }
        func_ov100_021b95b8(work, func_ov100_021b9d6c);
    }
}

// Shows the message that was on the screen again, with a Back button
// pokebw2's IrcBattleMatch_StateReshowMessage
static void func_ov100_021b9e14(IrcBattleMatch *work) {
    IrcBattleMatchMsg msg;

    func_ov100_021ba5f4(work);
    msg.msgId = work->currentMsgId;
    func_ov100_021ba3a4(&msg, work);
    func_ov100_021ba290(work, FALSE);
    func_ov100_021b95b8(work, func_ov100_021b9ef8);
}

void func_ov100_021b9e48(IrcBattleMatch *work) {
    if (func_020279e8(work->menu)) {
        u8 pos = func_02027a04(work->menu);

        func_02027858(work->menu);
        work->menu = NULL;
        GFL_BGSysQueueScrLoad(6);
        if (pos == 0) {
            func_ov100_021ba5f4(work);
            func_ov100_021b95c0(work);
            func_ov100_021b95b8(work, func_ov100_021b96b8);
        } else {
            func_ov100_021b95b8(work, func_ov100_021b9e14);
        }
        func_ov100_021b967c(work, FALSE);
    }
}

void func_ov100_021b9ea4(IrcBattleMatch *work) {
    IrcBattleMatchMsg msg = { data_ov100_021bb65c };
    u32 currentMsgId = work->currentMsgId;

    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 4, 0x180, 0x20, work->heapId);
    func_ov100_021ba3a4(&msg, work);
    work->currentMsgId = currentMsgId;
    func_ov100_021ba1d0(work);
    func_0201dd68();
    func_ov100_021b95b8(work, func_ov100_021b9e48);
}

// pokebw2's IrcBattleMatch_StateWaitStart
static void func_ov100_021b9ef8(IrcBattleMatch *work) {
    if (work->menu != NULL) {
        if (func_020279e8(work->menu)) {
            func_02027a04(work->menu);
            func_02027858(work->menu);
            work->menu = NULL;
            GFL_BGSysQueueScrLoad(6);
            func_ov100_021b95b8(work, func_ov100_021b9ea4);
            return;
        }
    } else {
        func_ov100_021ba290(work, FALSE);
        return;
    }
    if (func_02039cb4(func_020399a8(), IRCMATCH_TIMING_COMMANDS, IRCMATCH_TIMING_GROUP)) {
        if (func_0203d3a8(func_020399a8(), IRCMATCH_CMD_MODE, sizeof(work->mode), &work->mode)) {
            func_02039c68(func_020399a8(), IRCMATCH_TIMING_MODE, IRCMATCH_TIMING_GROUP);
            func_ov100_021b95b8(work, func_ov100_021b9db8);
        }
    }
}

// pokebw2's IrcBattleMatch_StateRegisterCommands
void func_ov100_021b9f80(IrcBattleMatch *work) {
    func_0203a574(IRCMATCH_CMD_MODE, data_ov100_021bb6c0, 6, work);
    func_02039c68(func_020399a8(), IRCMATCH_TIMING_COMMANDS, IRCMATCH_TIMING_GROUP);
    func_0203d7b0(1);
    func_0203d7bc(1);
    func_ov100_021b95b8(work, func_ov100_021b9ef8);
}

// Called when the link is up: collects the MAC addresses of the players
// pokebw2's IrcBattleMatch_OnMacsKnown
static void func_ov100_021b9fbc(void *work_) {
    IrcBattleMatch *work = work_;
    int i;

    if (func_0203d378()) {
        for (i = 0; i < 4; i++) {
            func_0203d0b8(work->macs[i], i);
        }
    }
    func_ov100_021b95b8(work, func_ov100_021b9f80);
}

// Called when the link goes up or down
// pokebw2's IrcBattleMatch_OnLinkChanged
void func_ov100_021b9ff4(void *work_, BOOL linked) {
    IrcBattleMatch *work = work_;

    work->linked = linked;
    work->linkChanged = TRUE;
    work->linkChangePending = TRUE;
    if (work->mode == IRCBATTLEMATCH_MODE_MULTI && linked == FALSE) {
        work->messagePending = TRUE;
        work->bannerActive = TRUE;
    }
}

static const BGSysVRAMConfig data_ov100_021bb6f0 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

static const BGSysLCDConfig data_ov100_021bb6b0 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

// clang-format off
// pokebw2's IrcBattleMatch_SetupBGs
static void func_ov100_021ba028(IrcBattleMatch *work) {
    GFL_BGSysSetVRAMBanks(&data_ov100_021bb6f0);
    GFL_BGSysSetLCDConfig(&data_ov100_021bb6b0);
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    {
        BGSetup setup = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xd000,
                          GX_BG_CHARBASE_0x10000, 0x8000, GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE };

        GFL_BGSysCreateBG(0, &setup, BGMODE_TEXT);
        GFL_BGSysFillScrArea(0, 0, 0, 0, 32, 32, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(0);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xd800,
                          GX_BG_CHARBASE_0x10000, 0x8000, GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE };

        GFL_BGSysCreateBG(1, &setup, BGMODE_TEXT);
        GFL_BGSysFillScrArea(1, 0, 0, 0, 32, 32, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(1);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xe000,
                          GX_BG_CHARBASE_0x00000, 0x8000, GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE };

        GFL_BGSysCreateBG(4, &setup, BGMODE_TEXT);
        GFL_BGSysFillScrArea(4, 0, 0, 0, 32, 32, BGSYS_FILL_TILE_PALETTE);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xe800,
                          GX_BG_CHARBASE_0x08000, 0x8000, GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE };

        GFL_BGSysCreateBG(5, &setup, BGMODE_TEXT);
        GFL_BGSysFillScrArea(5, 0, 0, 0, 32, 32, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(5);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf000,
                          GX_BG_CHARBASE_0x08000, 0x8000, GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE };

        GFL_BGSysCreateBG(6, &setup, BGMODE_TEXT);
        GFL_BGSysFillChar(6, 0, 1, 0);
        GFL_BGSysFillScrArea(6, 0, 0, 0, 32, 32, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(6);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800,
                          GX_BG_CHARBASE_0x10000, 0x8000, GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE };

        GFL_BGSysCreateBG(7, &setup, BGMODE_TEXT);
        GFL_BGSysFillScrArea(7, 0, 0, 0, 32, 32, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(7);
    }
}
// clang-format on

// pokebw2's IrcBattleMatch_VBlank
void func_ov100_021ba1c8(TCB *tcb, void *data) {
    func_0204a648();
}

// A Yes and No menu
// pokebw2's IrcBattleMatch_CreateYesNoMenu
static void func_ov100_021ba1d0(IrcBattleMatch *work) {
    AppTaskMenuInit init;

    init.heapId = work->heapId;
    init.itemCount = 2;
    init.items = work->menuItems;
    init.posType = APP_TASKMENU_POS_BOTTOM_RIGHT;
    init.x = 32;
    init.y = 13;
    init.width = 13;
    init.height = 3;
    work->menuItems[0].str = GFL_StrBufCreate(100, work->heapId);
    GFL_MsgDataLoadStrbuf(work->msgData, IRCMATCH_MSG_YES, work->menuItems[0].str);
    work->menuItems[0].color = 0x39e3;
    work->menuItems[1].str = GFL_StrBufCreate(100, work->heapId);
    GFL_MsgDataLoadStrbuf(work->msgData, IRCMATCH_MSG_NO, work->menuItems[1].str);
    work->menuItems[1].color = 0x39e3;
    work->menuItems[0].type = 0;
    work->menuItems[1].type = 0;
    work->menu = func_02027778(&init, work->menuRes);
    if (work->mode != IRCBATTLEMATCH_MODE_MUSICAL_ARG0 && work->mode != IRCBATTLEMATCH_MODE_MUSICAL_ARG1 &&
        work->mode != IRCBATTLEMATCH_MODE_BSUBWAY) {
        func_02027cd8(work->menu, TRUE);
    }
    GFL_StrBufFree(work->menuItems[0].str);
    GFL_StrBufFree(work->menuItems[1].str);
    func_ov100_021b967c(work, TRUE);
}

// A Back button, with a Confirm button above it
// pokebw2's IrcBattleMatch_CreateBackMenu
static void func_ov100_021ba290(IrcBattleMatch *work, BOOL withConfirm) {
    AppTaskMenuInit init;

    init.heapId = work->heapId;
    init.posType = APP_TASKMENU_POS_BOTTOM_RIGHT;
    init.x = 32;
    init.y = 24;
    init.width = 13;
    init.height = 3;
    if (withConfirm) {
        init.itemCount = 2;
        init.items = work->menuItems;
        work->menuItems[0].str = GFL_StrBufCreate(100, work->heapId);
        GFL_MsgDataLoadStrbuf(work->msgData, IRCMATCH_MSG_CONFIRM, work->menuItems[0].str);
        work->menuItems[0].color = 0x39e3;
        work->menuItems[0].type = 0;
        work->menuItems[1].str = GFL_StrBufCreate(100, work->heapId);
        GFL_MsgDataLoadStrbuf(work->msgData, IRCMATCH_MSG_BACK, work->menuItems[1].str);
        work->menuItems[1].color = 0x39e3;
        work->menuItems[1].type = APP_TASKMENU_ITEM_RETURN;
        work->menu = func_02027778(&init, work->menuRes);
        GFL_StrBufFree(work->menuItems[0].str);
        GFL_StrBufFree(work->menuItems[1].str);
        work->backOnly = FALSE;
    } else {
        init.itemCount = 1;
        init.items = work->menuItems;
        work->menuItems[0].str = GFL_StrBufCreate(100, work->heapId);
        GFL_MsgDataLoadStrbuf(work->msgData, IRCMATCH_MSG_BACK, work->menuItems[0].str);
        work->menuItems[0].color = 0x39e3;
        work->menuItems[0].type = APP_TASKMENU_ITEM_RETURN;
        work->menu = func_02027778(&init, work->menuRes);
        GFL_StrBufFree(work->menuItems[0].str);
        work->backOnly = TRUE;
    }
    if (work->mode != IRCBATTLEMATCH_MODE_MUSICAL_ARG0 && work->mode != IRCBATTLEMATCH_MODE_MUSICAL_ARG1 &&
        work->mode != IRCBATTLEMATCH_MODE_BSUBWAY) {
        func_02027cd8(work->menu, TRUE);
    }
}

// Shows a message in the window at the top, which it makes if it has none
// pokebw2's IrcBattleMatch_ShowMessage
static void func_ov100_021ba3a4(IrcBattleMatchMsg *msg, IrcBattleMatch *work) {
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 4, 0x1a0, 0x20, work->heapId);
    if (work->messageWindow == NULL) {
        work->messageWindow = BmpWin_CreateDynamic(5, 1, 1, 30, 5, 13, 0);
        BmpWin_FlushMap(work->messageWindow);
        BmpWin_DrawFrame(work->messageWindow, 0, CHAR_POS(work->windowFrameChars), 14);
    }
    GFL_BitmapFill(BmpWin_GetBitmap(work->messageWindow), 0xff);
    work->currentMsgId = msg->msgId;
    GFL_MsgDataLoadStrbuf(work->msgData, msg->msgId, work->strbufFormatted);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(work->messageWindow), 4, 4, work->strbufFormatted, work->font);
    BmpWin_FlushChar(work->messageWindow);
}

// Shows how many friends the Friend Roster holds in the window below the message
// pokebw2's IrcBattleMatch_ShowFriendCount
static void func_ov100_021ba454(u32 msgId, IrcBattleMatch *work) {
    int count;

    if (work->countWindow == NULL) {
        work->countWindow = BmpWin_CreateDynamic(5, 1, 8, 18, 4, 13, 0);
        BmpWin_FlushMap(work->countWindow);
        BmpWin_DrawFrame(work->countWindow, 0, CHAR_POS(work->windowFrameChars), 14);
    }
    GFL_BitmapFill(BmpWin_GetBitmap(work->countWindow), 0xff);
    GFL_MsgDataLoadStrbuf(work->msgData, msgId, work->strbuf);
    count = func_02009a34(func_0201283c(work->param->gameData));
    WordSetNumber(work->wordSet, 0, count, 2, 1, TRUE);
    WordSetNumber(work->wordSet, 1, 32, 2, 1, TRUE);
    GFL_WordSetFormatStrbuf(work->wordSet, work->strbufFormatted, work->strbuf);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(work->countWindow), 0, 2, work->strbufFormatted, work->font);
    BmpWin_FlushChar(work->countWindow);
}

// Shows how many players have joined
// pokebw2's IrcBattleMatch_ShowMemberCount
static void func_ov100_021ba530(u32 msgId, IrcBattleMatch *work) {
    if (work->countWindow == NULL) {
        work->countWindow = BmpWin_CreateDynamic(5, 1, 8, 23, 4, 13, 0);
    }
    GFL_BitmapFill(BmpWin_GetBitmap(work->countWindow), 0xff);
    BmpWin_FlushMap(work->countWindow);
    BmpWin_FlushChar(work->countWindow);
    BmpWin_DrawFrame(work->countWindow, 0, CHAR_POS(work->windowFrameChars), 14);
    GFL_MsgDataLoadStrbuf(work->msgData, msgId, work->strbuf);
    WordSetNumber(work->wordSet, 0, func_0203d170(), 2, 1, TRUE);
    GFL_WordSetFormatStrbuf(work->wordSet, work->strbufFormatted, work->strbuf);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(work->countWindow), 0, 2, work->strbufFormatted, work->font);
    BmpWin_FlushChar(work->countWindow);
}

// pokebw2's IrcBattleMatch_FreeWindows
static void func_ov100_021ba5f4(IrcBattleMatch *work) {
    if (work->messageWindow != NULL) {
        BmpWin_Free(work->messageWindow);
    }
    if (work->countWindow != NULL) {
        BmpWin_Free(work->countWindow);
    }
    work->messageWindow = NULL;
    work->countWindow = NULL;
}

// pokebw2's IrcBattleMatch_InitGraphics
void func_ov100_021ba624(IrcBattleMatch *work) {
    GFL_BGSysCreate(work->heapId);
    BmpWin_InitAllocator(work->heapId);
    func_0201dd60();
}

// pokebw2's IrcBattleMatch_FreeGraphics
void func_ov100_021ba640(IrcBattleMatch *work) {
    GFL_BGSysFree();
    BmpWin_FreeAllocator();
}

// pokebw2's IrcBattleMatch_FreeActor
void func_ov100_021ba64c(IrcBattleMatch *work, int slot) {
    if (work->actors[slot] != NULL) {
        func_0204b3b4(work->actors[slot]);
        work->actors[slot] = NULL;
    }
}

// Shows what to do for the mode
// pokebw2's IrcBattleMatch_ShowConnectMessage
static void func_ov100_021ba668(IrcBattleMatch *work) {
    if (work->connectDone == TRUE) {
        IrcBattleMatchMsg msg = { IRCMATCH_MSG_CONNECT_LEADER };

        func_ov100_021ba3a4(&msg, work);
    } else if (work->mode == IRCBATTLEMATCH_MODE_FRIEND) {
        if (func_02009a34(func_0201283c(work->param->gameData)) == 32) {
            IrcBattleMatchMsg msg = { IRCMATCH_MSG_FRIEND_ROSTER_FULL };

            func_ov100_021ba3a4(&msg, work);
        } else {
            IrcBattleMatchMsg msg = { IRCMATCH_MSG_CONNECT_FRIEND_CODE };

            func_ov100_021ba3a4(&msg, work);
            func_ov100_021ba454(IRCMATCH_MSG_FRIEND_COUNT, work);
        }
    } else if (work->mode == IRCBATTLEMATCH_MODE_TRADE) {
        IrcBattleMatchMsg msg = { IRCMATCH_MSG_CONNECT_TRADE_PARTNER };

        func_ov100_021ba3a4(&msg, work);
    } else if (work->mode == IRCBATTLEMATCH_MODE_MULTI) {
        IrcBattleMatchMsg msg = { IRCMATCH_MSG_CONNECT_PARTNER };

        func_ov100_021ba3a4(&msg, work);
    } else if (work->mode == IRCBATTLEMATCH_MODE_MUSICAL_ARG0) {
        IrcBattleMatchMsg msg = { IRCMATCH_MSG_CONNECT_ALL };

        func_ov100_021ba3a4(&msg, work);
        func_ov100_021ba530(IRCMATCH_MSG_MEMBER_COUNT, work);
    } else if (work->mode == IRCBATTLEMATCH_MODE_MUSICAL_ARG1) {
        IrcBattleMatchMsg msg = { IRCMATCH_MSG_CONNECT_SHOW_LEADER };

        func_ov100_021ba3a4(&msg, work);
    } else if (work->mode == IRCBATTLEMATCH_MODE_BSUBWAY) {
        IrcBattleMatchMsg msg = { IRCMATCH_MSG_CONNECT_PLAY_FRIEND };

        func_ov100_021ba3a4(&msg, work);
    } else {
        IrcBattleMatchMsg msg = { IRCMATCH_MSG_CONNECT_OPPONENTS };

        func_ov100_021ba3a4(&msg, work);
    }
}

// pokebw2's IrcBattleMatch_InitScreen
static void func_ov100_021ba780(IrcBattleMatch *work) {
    ArcTool *arc;
    int i;
    void *paletteFile;
    NNSG2dPaletteData *palette;

    func_0201dd68();
    work->wordSet = GFL_WordSetSystemCreate(11, 200, work->heapId);
    func_ov100_021ba624(work);
    func_ov100_021ba028(work);
    work->strbufFormatted = GFL_StrBufCreate(200, work->heapId);
    work->strbuf = GFL_StrBufCreate(200, work->heapId);
    work->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, work->heapId);
    work->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_STARTMENU_2, work->heapId);
    arc = GFL_ArcSysCreateFileHandle(IRCMATCH_ARCID, work->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 1, 0, 0, 0, work->heapId);
    work->bgChars = GFL_BGSysLoadArcNCGRDynamic(arc, 6, 0, 0, FALSE, work->heapId);
    if (work->mode != IRCBATTLEMATCH_MODE_MULTI) {
        GFL_G2DIOLoadNSCRSync(arc, 13, 1, 0, CHAR_POS(work->bgChars), 0, FALSE, work->heapId);
    } else {
        GFL_G2DIOLoadNSCRSync(arc, 13, 0, 0, CHAR_POS(work->bgChars), 0, FALSE, work->heapId);
        GFL_G2DIOLoadNSCRSync(arc, 14, 1, 0, CHAR_POS(work->bgChars), 0, FALSE, work->heapId);
    }
    GFL_ArcToolFree(arc);
    arc = GFL_ArcSysCreateFileHandle(ARCID_GSYNC_MENU, work->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 1, 4, 0, 0, work->heapId);
    work->menuBgChars = GFL_BGSysLoadArcNCGRDynamic(arc, 4, 4, 0, FALSE, work->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 9, 4, 0, CHAR_POS(work->menuBgChars), 0, FALSE, work->heapId);
    GFL_ArcToolFree(arc);
    work->windowFrameChars = LoadCursorImageEndOfHeap(5, 14, 0, work->heapId);
    GFL_FadeSet(3, 16, 0, 2);
    func_ov100_021ba668(work);
    ClActSys_Create(&data_020a1448, &data_ov100_021bb6f0, work->heapId);
    work->actUnit = func_0204b100(40, 0, work->heapId);
    work->vblankTcb = GFL_VBlankTCBAdd(func_ov100_021ba1c8, work, 0);
    work->printQueue = func_0201c440(work->heapId);
    work->menuRes = func_02027f60(6, 9, work->font, work->printQueue, work->heapId);
    func_ov100_021b96ec(work);
    if (work->mode != IRCBATTLEMATCH_MODE_MULTI) {
        func_ov100_021b974c(work, 57, 154, 2, 10);
        func_ov100_021b974c(work, 128, 113, 4, 2);
        func_ov100_021b974c(work, 200, 38, 0, 11);
        func_ov100_021b974c(work, 128, 77, 5, 3);
    } else {
        func_ov100_021b974c(work, 57, 154, 0, 4);
        func_ov100_021b974c(work, 198, 154, 1, 5);
        func_ov100_021b974c(work, 200, 38, 2, 6);
        func_ov100_021b974c(work, 55, 38, 3, 7);
        func_ov100_021b974c(work, 152, 146, 6, 0);
        func_ov100_021b974c(work, 103, 146, 7, 1);
        func_ov100_021b974c(work, 152, 46, 4, 0);
        func_ov100_021b974c(work, 103, 46, 5, 1);
    }
    GFL_BGSysSetEnabledBGsA(0x13);
    GFL_BGSysSetEnabledBGsB(0x17);
    work->paletteHeapId = work->heapId;
    arc = GFL_ArcSysCreateFileHandle(IRCMATCH_ARCID, work->paletteHeapId);
    paletteFile = GFL_G2DIOReadNCLRArc(arc, 2, &palette, work->paletteHeapId);
    MI_CpuCopy8(palette->pRawData, work->palettes, 0x120);
    for (i = 0; i < 9; i++) {
        MI_CpuCopy8(work->palettes[8 - i], work->palettes[9 + i], 0x20);
    }
    GFL_HeapFree(paletteFile);
    GFL_ArcToolFree(arc);
    func_ov100_021b95b8(work, func_ov100_021baa9c);
}

// pokebw2's IrcBattleMatch_StateWaitFade
void func_ov100_021baa9c(IrcBattleMatch *work) {
    if (GFL_FadeIsRunning()) {
        if (work->mode == IRCBATTLEMATCH_MODE_FRIEND &&
            func_02009a34(func_0201283c(work->param->gameData)) == 32) {
            func_ov100_021b95b8(work, func_ov100_021bafa0);
            work->counter = 180;
            return;
        }
        func_ov100_021b95b8(work, func_ov100_021bab08);
    }
}

// Whether the commands' base is that of the musical, as a player of the other musical mode has
// pokebw2's IrcBattleMatch_IsSameMusicalGame
BOOL func_ov100_021baaec(int a, int b) {
    if (a == 0x13 && b == 0x12) {
        return TRUE;
    }
    if (b == 0x13 && a == 0x12) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's IrcBattleMatch_StateStartNet
static void func_ov100_021bab08(IrcBattleMatch *work) {
    GFLNetInitData init = data_ov100_021bb83c;

    init.bNetType = 4;
    switch (work->mode) {
    case IRCBATTLEMATCH_MODE_SINGLE:
    case IRCBATTLEMATCH_MODE_DOUBLE:
    case IRCBATTLEMATCH_MODE_TRIPLE:
    case IRCBATTLEMATCH_MODE_ROTATION:
        init.gameCommandBase = 0xc;
        break;
    case IRCBATTLEMATCH_MODE_MULTI:
        init.gameCommandBase = 0x26;
        init.maxConnectNum = 4;
        break;
    case IRCBATTLEMATCH_MODE_FRIEND:
        init.gameCommandBase = 0x1d;
        break;
    case IRCBATTLEMATCH_MODE_TRADE:
        init.gameCommandBase = 0x1c;
        break;
    case IRCBATTLEMATCH_MODE_BSUBWAY:
        init.gameCommandBase = 0x2c;
        break;
    case IRCBATTLEMATCH_MODE_MUSICAL_ARG0:
        init.unk6E = 0x11;
        init.gameCommandBase = 0x13;
        init.bMPMode = TRUE;
        init.maxConnectNum = work->param->musicalNetInit->maxConnectNum;
        init.maxSendSize = work->param->musicalNetInit->maxSendSize;
        init.unk6C = work->param->musicalNetInit->unk6C;
        work->shownCount = -1;
        break;
    case IRCBATTLEMATCH_MODE_MUSICAL_ARG1:
        init.unk6E = 0x11;
        init.gameCommandBase = 0x12;
        init.bMPMode = TRUE;
        init.maxConnectNum = work->param->musicalNetInit->maxConnectNum;
        init.maxSendSize = work->param->musicalNetInit->maxSendSize;
        init.unk6C = work->param->musicalNetInit->unk6C;
        work->shownCount = -1;
        break;
    }
    func_0203cbac(&init, NULL, work);
    switch (work->mode) {
    case IRCBATTLEMATCH_MODE_MUSICAL_ARG1:
        func_0203d0c4(func_ov100_021baaec);
        func_0203d164(1);
        func_0203d0ac(0x11);
        break;
    case IRCBATTLEMATCH_MODE_MUSICAL_ARG0:
        func_0203d0c4(func_ov100_021baaec);
        func_0203d164(0);
        func_0203d0ac(0x11);
        break;
    case IRCBATTLEMATCH_MODE_MULTI:
        func_0203d0ac(0x25);
        break;
    }
    func_ov100_021ba290(work, FALSE);
    func_ov100_021b95b8(work, func_ov100_021bac80);
}

// pokebw2's IrcBattleMatch_FreeScreen
static void func_ov100_021bac08(IrcBattleMatch *work) {
    func_0201dd68();
    func_ov100_021ba5f4(work);
    GFL_BGSysFreeFilledChar(5, 1, 0);
    GFL_BGSysFreeCharMemory(5, CHAR_POS(work->windowFrameChars), CHAR_SIZE(work->windowFrameChars));
    GFL_BGSysReleaseBG(5);
    GFL_MsgDataFree(work->msgData);
    GFL_FontFree(work->font);
    GFL_StrBufFree(work->strbuf);
    GFL_StrBufFree(work->strbufFormatted);
    GFL_BGSysSetBGEnabled(5, FALSE);
    func_02027fd8(work->menuRes);
    func_ov100_021ba640(work);
}

// pokebw2's IrcBattleMatch_StateWaitNet
void func_ov100_021bac80(IrcBattleMatch *work) {
    if (func_0203cd98() == TRUE) {
        switch (work->mode) {
        case IRCBATTLEMATCH_MODE_MUSICAL_ARG0:
        case IRCBATTLEMATCH_MODE_MUSICAL_ARG1:
            func_ov015_021799c0(func_ov100_021baaec);
            break;
        }
        func_0203d060(func_ov100_021b9fbc, func_ov100_021b9ff4, func_ov100_021b9848);
        func_ov100_021b95b8(work, func_ov100_021bb270);
    }
}

// Shows the message that was on the screen again, after the quit prompt in the connecting screen
// pokebw2's IrcBattleMatch_StateRestoreConnect
void func_ov100_021bacc8(IrcBattleMatch *work) {
    IrcBattleMatchMsg msg;

    func_ov100_021ba5f4(work);
    msg.msgId = work->currentMsgId;
    func_ov100_021ba3a4(&msg, work);
    if (work->mode == IRCBATTLEMATCH_MODE_MUSICAL_ARG0 && work->shownCount > 0) {
        func_ov100_021ba290(work, TRUE);
        func_ov100_021b95b8(work, func_ov100_021bb270);
        return;
    }
    func_ov100_021ba290(work, FALSE);
    func_ov100_021b95b8(work, func_ov100_021bb270);
}

// pokebw2's IrcBattleMatch_StateQuitAnswerConnect
static void func_ov100_021bad1c(IrcBattleMatch *work) {
    if (func_020279e8(work->menu)) {
        u8 pos = func_02027a04(work->menu);

        func_02027858(work->menu);
        work->menu = NULL;
        GFL_BGSysQueueScrLoad(6);
        if (pos == 0) {
            func_ov100_021ba5f4(work);
            func_ov100_021b95c0(work);
            func_ov100_021b95b8(work, func_ov100_021b96b8);
        } else {
            func_ov100_021b95b8(work, func_ov100_021bacc8);
        }
        func_ov100_021b967c(work, FALSE);
    }
}

// pokebw2's IrcBattleMatch_StateWaitBackWaiting
void func_ov100_021bad78(IrcBattleMatch *work) {
    if (func_020279e8(work->menu)) {
        func_02027a04(work->menu);
        func_02027858(work->menu);
        work->menu = NULL;
        GFL_BGSysQueueScrLoad(6);
        func_ov100_021b95b8(work, func_ov100_021bb058);
    }
}

// pokebw2's IrcBattleMatch_StateWaitMembers
static void func_ov100_021badb0(IrcBattleMatch *work) {
    if (func_0203d158()) {
        func_ov100_021b95b8(work, func_ov100_021bad78);
        return;
    }
    if (func_020279e8(work->menu)) {
        func_02027a04(work->menu);
        func_02027858(work->menu);
        work->menu = NULL;
        GFL_BGSysQueueScrLoad(6);
        func_ov100_021b95b8(work, func_ov100_021bb058);
    }
}

// pokebw2's IrcBattleMatch_StateShowWaitMenu
void func_ov100_021badfc(IrcBattleMatch *work) {
    func_ov100_021ba290(work, FALSE);
    func_ov100_021b95b8(work, func_ov100_021badb0);
}

// pokebw2's IrcBattleMatch_StateRestoreAfterConfirm
void func_ov100_021bae14(IrcBattleMatch *work) {
    IrcBattleMatchMsg msg = { work->currentMsgId };

    func_ov100_021ba3a4(&msg, work);
    func_ov100_021b95b8(work, func_ov100_021bb270);
    if (work->mode == IRCBATTLEMATCH_MODE_MUSICAL_ARG0 && work->shownCount > 0) {
        func_ov100_021ba290(work, TRUE);
    } else {
        func_ov100_021ba290(work, FALSE);
    }
}

// pokebw2's IrcBattleMatch_StateConfirmAnswer
static void func_ov100_021bae5c(IrcBattleMatch *work) {
    if (func_020279e8(work->menu)) {
        u8 pos = func_02027a04(work->menu);

        func_02027858(work->menu);
        work->menu = NULL;
        if (pos == 0) {
            func_ov100_021ba5f4(work);
            GFL_BGSysQueueScrLoad(6);
            func_ov100_021b95b8(work, func_ov100_021badfc);
        } else {
            func_ov100_021ba5f4(work);
            func_ov100_021b95b8(work, func_ov100_021bae14);
        }
    }
}

// Waits for a touch, then goes back to the infrared menu
// pokebw2's IrcBattleMatch_StateWaitMismatchTouch
void func_ov100_021baeb4(IrcBattleMatch *work) {
    if (func_02035c60()) {
        func_ov100_021b95c0(work);
        work->param->result = IRCBATTLEMATCH_RESULT_BACK;
        func_ov100_021b95b8(work, func_ov100_021b96b8);
    }
}

// pokebw2's func_ov100_021b9ea4Connect
void func_ov100_021baee0(IrcBattleMatch *work) {
    IrcBattleMatchMsg msg = { IRCMATCH_MSG_QUIT_CONFIRM };
    u32 currentMsgId = work->currentMsgId;

    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 4, 0x180, 0x20, work->heapId);
    func_ov100_021ba3a4(&msg, work);
    work->currentMsgId = currentMsgId;
    func_ov100_021ba1d0(work);
    func_0201dd68();
    if (work->buttonMan != NULL) {
        func_02051c6c(work->buttonMan);
    }
    work->buttonMan = NULL;
    func_ov100_021b95b8(work, func_ov100_021bad1c);
}

// pokebw2's IrcBattleMatch_StateShowConfirmPrompt
void func_ov100_021baf4c(IrcBattleMatch *work) {
    IrcBattleMatchMsg msg = { IRCMATCH_MSG_MEMBERS_OK };
    u32 currentMsgId = work->currentMsgId;

    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 4, 0x180, 0x20, work->heapId);
    func_ov100_021ba3a4(&msg, work);
    work->currentMsgId = currentMsgId;
    func_ov100_021ba1d0(work);
    func_0201dd68();
    func_ov100_021b95b8(work, func_ov100_021bae5c);
}

// pokebw2's IrcBattleMatch_StateWaitFriendFull
void func_ov100_021bafa0(IrcBattleMatch *work) {
    work->counter--;
    if (work->counter == 0) {
        func_ov100_021ba5f4(work);
        func_ov100_021b95c0(work);
        func_ov100_021b95b8(work, func_ov100_021b96b8);
    }
}

// pokebw2's IrcBattleMatch_StateShowWaitMessage
void func_ov100_021bafcc(IrcBattleMatch *work) {
    IrcBattleMatchMsg msg;

    func_ov100_021ba5f4(work);
    msg.msgId = IRCMATCH_MSG_WAIT;
    func_ov100_021ba3a4(&msg, work);
    func_ov100_021ba290(work, FALSE);
    func_ov100_021b95b8(work, func_ov100_021bad78);
}

// pokebw2's IrcBattleMatch_StateQuitAnswerWaiting
static void func_ov100_021baffc(IrcBattleMatch *work) {
    if (func_020279e8(work->menu)) {
        u8 pos = func_02027a04(work->menu);

        func_02027858(work->menu);
        work->menu = NULL;
        GFL_BGSysQueueScrLoad(6);
        if (pos == 0) {
            func_ov100_021ba5f4(work);
            func_ov100_021b95c0(work);
            func_ov100_021b95b8(work, func_ov100_021b96b8);
        } else {
            func_ov100_021b95b8(work, func_ov100_021bafcc);
        }
        func_ov100_021b967c(work, FALSE);
    }
}

// pokebw2's func_ov100_021b9ea4Waiting
void func_ov100_021bb058(IrcBattleMatch *work) {
    IrcBattleMatchMsg msg = { IRCMATCH_MSG_QUIT_CONFIRM };
    u32 currentMsgId = work->currentMsgId;

    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 4, 0x180, 0x20, work->heapId);
    func_ov100_021ba3a4(&msg, work);
    work->currentMsgId = currentMsgId;
    func_ov100_021ba1d0(work);
    func_0201dd68();
    if (work->buttonMan != NULL) {
        func_02051c6c(work->buttonMan);
    }
    work->buttonMan = NULL;
    func_ov100_021b95b8(work, func_ov100_021baffc);
}

// pokebw2's IrcBattleMatch_OnLeaderAnimEnd
void func_ov100_021bb0c4(uintptr_t param, fx32 frame) {
    IrcBattleMatch *work = (IrcBattleMatch *)param;
    u16 sequence = func_0204b898(work->actors[0]);

    switch (sequence) {
    case 8:
        func_0204b878(work->actors[0], 11);
        func_ov100_021b9800(work, 200, 38, 0);
        func_ov100_021b974c(work, 128, 113, 4, 2);
        break;
    case 11:
        break;
    default:
        func_0204b878(work->actors[0], 8);
        break;
    }
}

// pokebw2's IrcBattleMatch_OnPartnerAnimEnd
void func_ov100_021bb10c(uintptr_t param, fx32 frame) {
    IrcBattleMatch *work = (IrcBattleMatch *)param;
    u16 sequence = func_0204b898(work->actors[2]);

    switch (sequence) {
    case 9:
        func_0204b878(work->actors[2], 10);
        func_ov100_021b9800(work, 57, 154, 2);
        func_ov100_021b974c(work, 128, 77, 5, 3);
        break;
    case 10:
        break;
    default:
        func_0204b878(work->actors[2], 9);
        break;
    }
}

// pokebw2's IrcBattleMatch_StateShowMismatch
void func_ov100_021bb154(IrcBattleMatch *work) {
    IrcBattleMatchMsg msg = { IRCMATCH_MSG_MISMATCH };

    func_ov100_021ba5f4(work);
    func_ov100_021ba3a4(&msg, work);
    func_ov100_021b95b8(work, func_ov100_021baeb4);
}

// Updates the infrared multi battle's screen when the link goes up or down
// pokebw2's IrcBattleMatch_UpdateLinkChange
static void func_ov100_021bb180(IrcBattleMatch *work) {
    ClActorCallback callback;

    func_ov100_021ba5f4(work);
    if (work->linkChanged == TRUE) {
        if (work->linked != 0) {
            IrcBattleMatchMsg msg = { IRCMATCH_MSG_CONNECT_LEADER };

            func_ov100_021ba3a4(&msg, work);
        } else {
            IrcBattleMatchMsg msg = { IRCMATCH_MSG_WAIT };

            func_ov100_021ba3a4(&msg, work);
            func_ov100_021ba64c(work, 0);
            func_ov100_021ba64c(work, 2);
        }
        func_ov100_021ba64c(work, 6);
        func_ov100_021ba64c(work, 7);
        func_ov100_021ba64c(work, 4);
        func_ov100_021ba64c(work, 5);
        func_ov100_021ba64c(work, 1);
        func_ov100_021ba64c(work, 3);
        if (work->linked != 0) {
            callback.type = CLACT_CALLBACK_LAST_FRAME;
            callback.param = (uintptr_t)work;
            callback.func = func_ov100_021bb0c4;
            func_0204ba00(work->actors[0], &callback);
            callback.func = func_ov100_021bb10c;
            func_0204ba00(work->actors[2], &callback);
        }
        work->linkChanged = FALSE;
    }
    work->linkChangeFrames++;
    if (work->linkChangeFrames > 8 && work->palettePulse != 0) {
        work->linkChangePending = FALSE;
        GFL_BGSysSetBGEnabled(1, FALSE);
    }
}

// The message the multi battle and the others show once the link is lost, by mode
// pokebw2's sIrcBattleMatchWaitMessages
static IrcBattleMatchMsg data_ov100_021bb804[] = {
    { IRCMATCH_MSG_BATTLE_STARTING },
    { IRCMATCH_MSG_BATTLE_STARTING },
    { IRCMATCH_MSG_BATTLE_STARTING },
    { IRCMATCH_MSG_BATTLE_STARTING },
    { IRCMATCH_MSG_BATTLE_STARTING },
    { IRCMATCH_MSG_BATTLE_STARTING },
    { IRCMATCH_MSG_TRADE_STARTING },
    { IRCMATCH_MSG_TRADE_STARTING },
    { IRCMATCH_MSG_WAIT_MOMENT },
    { IRCMATCH_MSG_WAIT_MOMENT },
    { IRCMATCH_MSG_WAIT_MOMENT },
    { IRCMATCH_MSG_BATTLE_STARTING },
    { IRCMATCH_MSG_WAIT_MOMENT },
    { IRCMATCH_MSG_WAIT_MOMENT },
};

// pokebw2's IrcBattleMatch_StateConnectLoop
static void func_ov100_021bb270(IrcBattleMatch *work) {
    if (work->linkChangePending && work->mode == IRCBATTLEMATCH_MODE_MULTI) {
        func_ov100_021bb180(work);
    } else if (work->bannerActive == TRUE && work->mode != IRCBATTLEMATCH_MODE_MUSICAL_ARG0) {
        if (work->messagePending == TRUE) {
            func_ov100_021ba5f4(work);
            func_ov100_021ba3a4(&data_ov100_021bb804[work->mode], work);
            func_ov100_021ba64c(work, 4);
            func_ov100_021ba64c(work, 5);
            work->messagePending = FALSE;
            if (work->actors[0] != NULL) {
                func_0204b92c(work->actors[0], FALSE);
                func_0204b92c(work->actors[2], FALSE);
            }
        }
        func_ov100_021b97bc(work, 0, -2, 0);
        func_ov100_021b97bc(work, 0, 2, 2);
        work->bannerFrames++;
        if (work->bannerFrames > 30) {
            if (work->actors[0] != NULL) {
                func_0204b3dc(work->actors[0], FALSE);
                func_0204b3dc(work->actors[2], FALSE);
            }
            work->bannerActive = FALSE;
        }
    }
    if (func_0203cd98() && func_020399dc(func_020399a8()) == 7) {
        func_ov100_021b95b8(work, func_ov100_021bb154);
        return;
    }
    if (func_020279e8(work->menu)) {
        u8 pos = func_02027a04(work->menu);

        func_02027858(work->menu);
        work->menu = NULL;
        GFL_BGSysQueueScrLoad(6);
        if (work->backOnly) {
            func_ov100_021b95b8(work, func_ov100_021baee0);
        } else if (pos == 0) {
            func_ov100_021b95b8(work, func_ov100_021baf4c);
        } else {
            func_ov100_021b95b8(work, func_ov100_021baee0);
        }
    } else if (work->mode == IRCBATTLEMATCH_MODE_MUSICAL_ARG0) {
        if (work->shownCount != func_0203d170()) {
            func_ov100_021ba530(IRCMATCH_MSG_MEMBER_COUNT, work);
            work->shownCount = func_0203d170();
        }
        if (work->shownCount > 0 && work->backOnly == TRUE) {
            func_02027858(work->menu);
            work->menu = NULL;
            func_ov100_021ba290(work, TRUE);
        }
    }
}

BOOL func_ov100_021bb414(GameProc *proc, u32 *state, void *param, void *work_) {
    IrcBattleMatch *work;

    GFL_HeapCreateChild(1, 0x1f, 0x20000);
    work = GFL_ProcInitSubsystem(proc, sizeof(IrcBattleMatch), 0x1f);
    MI_CpuFill8(work, 0, sizeof(IrcBattleMatch));
    work->param = param;
    work->heapId = 0x1f;
    work->mode = work->param->result;
    work->paletteBackup = GFL_HeapAllocate(work->heapId, 0x200, FALSE, "ircbattlematch.c", 0xb07);
    func_ov100_021ba780(work);
    func_0203d0d0();
    G2_SetBlendAlpha(0x10, 3, 9, 0xe);
    return TRUE;
}

static inline BOOL IrcBattleMatch_CyclePalette(IrcBattleMatch *work) {
    if ((OS_GetVBlankCount() & 7) == 1) {
        GX_LoadBGPltt(work->palettes[work->paletteFrame], 0, 0x20);
        work->paletteFrame++;
        if (work->paletteFrame == 9) {
            return TRUE;
        }
        if (work->paletteFrame >= 18) {
            work->paletteFrame = 0;
        }
    }
    return FALSE;
}

// pokebw2's IrcBattleMatch_Main
static BOOL func_ov100_021bb498(GameProc *proc, u32 *state, void *param, void *work_) {
    IrcBattleMatch *work = work_;
    BOOL done = TRUE;
    IrcBattleMatchState stateFunc = work->state;

    work->palettePulse = IrcBattleMatch_CyclePalette(work);
    if (stateFunc != NULL) {
        stateFunc(work);
        if (work->state != NULL) {
            done = FALSE;
        }
    }
    if (work->menu != NULL) {
        func_02027974(work->menu);
    }
    if (work->taskWin0 != NULL) {
        func_02028178(work->taskWin0);
    }
    if (work->taskWin1 != NULL) {
        func_02028178(work->taskWin1);
    }
    GFL_BGSysMoveBG(4, 3, work->scrollY);
    work->scrollY--;
    func_0204a600();
    func_0201c4e4(work->printQueue);
    if (func_0200fb24()) {
        func_0200fb38();
        work->param->result = IRCBATTLEMATCH_RESULT_QUIT;
        done = TRUE;
        func_0202223c(0, 0);
        func_0202223c(1, 0);
    }
    return done;
}

BOOL func_ov100_021bb584(GameProc *proc, u32 *state, void *param, void *work_) {
    IrcBattleMatch *work = work_;

    if (!GFL_WipeIsFinished()) {
        return FALSE;
    }
    if (func_0203cd98()) {
        func_0203a5e4(IRCMATCH_CMD_MODE);
    }
    if (work->menu != NULL) {
        func_02027858(work->menu);
    }
    if (work->taskWin0 != NULL) {
        func_02028148(work->taskWin0);
    }
    if (work->taskWin1 != NULL) {
        func_02028148(work->taskWin1);
    }
    if (work->buttonMan != NULL) {
        func_02051c6c(work->buttonMan);
    }
    GFL_HeapFree(work->paletteBackup);
    work->buttonMan = NULL;
    GFL_WordSetSystemFree(work->wordSet);
    func_ov100_021bac08(work);
    GFL_TCBRemove(work->vblankTcb);
    func_0204b1cc(work->actUnit);
    func_0204a5ac();
    func_0201c6ec(work->printQueue);
    func_0201c4c0(work->printQueue);
    if (work->unk30C != NULL) {
        GFL_TCBRemove(work->unk30C);
        work->unk30C = NULL;
        work->unk310 = 0;
    }
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_IRC_BATTLE_MENU);
    return TRUE;
}


const GameProcFunctions data_ov100_021bb6a4 = {
    func_ov100_021bb414,
    func_ov100_021bb498,
    func_ov100_021bb584,
};
