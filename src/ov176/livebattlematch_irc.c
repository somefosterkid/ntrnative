// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov261/livebattlematch_irc.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/version.h"
#include "app/livebattlematch_irc.h"
#include "field/delivery_irc.h"
#include "field/net_devirc.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/std.h"
#include "pml/poke_party.h"
#include "system/version.h"

// The commands of the link, the game's command base 0x1f and a number
#define LIVEBATTLEMATCH_IRC_COMMAND_PLAYER_DATA 0x1f00
#define LIVEBATTLEMATCH_IRC_COMMAND_PARTY 0x1f01
#define LIVEBATTLEMATCH_IRC_COMMAND_UNUSED 0x1f02
#define LIVEBATTLEMATCH_IRC_COMMAND_READY 0x1f03
#define LIVEBATTLEMATCH_IRC_COMMAND_NUM 4

#define LIVEBATTLEMATCH_IRC_BUFFER_SIZE 0x1000
// The code and size of the p_delivery's data: the competition's data of a Download Station
#define LIVEBATTLEMATCH_IRC_DELIVERY_CODE 0x30
#define LIVEBATTLEMATCH_IRC_DELIVERY_SIZE 0x160
// The version bit of the delivery's data
#define LIVEBATTLEMATCH_VERSION_MASK (1 << GAME_VERSION)

typedef struct LiveBattleMatchIrc {
    // The steps of whichever function is stepping the link
    u32 state;
    HeapID heapId;
    // Where the other machine's data arrives
    u8 buffer[LIVEBATTLEMATCH_IRC_BUFFER_SIZE];
    // Whether the data of each command came, until it is taken
    BOOL playerDataArrived;
    BOOL partyArrived;
    BOOL unusedArrived;
    BOOL readyArrived;
    void *p_delivery; // the ROM's name, from its assert
    int deliveryResult;
} LiveBattleMatchIrc;

static void func_ov176_0217e45c(int netId, int size, const void *data, void *work, NetHandle *handle);
static void *func_ov176_0217e4fc(int netId, void *work, int size);
static void func_ov176_0217e484(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov176_0217e4ac(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov176_0217e4d4(int netId, int size, const void *data, void *work, NetHandle *handle);
static BOOL func_ov176_0217e524(int a, int b);

// pokebw2's sLiveBattleMatchIrcCommands
static const NetCommand data_ov176_0217e544[LIVEBATTLEMATCH_IRC_COMMAND_NUM] = {
    {func_ov176_0217e45c, func_ov176_0217e4fc},
    {func_ov176_0217e484, func_ov176_0217e4fc},
    {func_ov176_0217e4ac, func_ov176_0217e4fc},
    {func_ov176_0217e4d4, func_ov176_0217e4fc},
};

// pokebw2's sLiveBattleMatchIrcNetInit
static const GFLNetInitData data_ov176_0217e564 = {
    NULL,
    0,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    {0},
    NULL,
    NULL,
    0,
    {1, 0, 0, 0, 0x80, 0x13, 0, 0},
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
    0x1f,
    {0xff, 0xff, 0xff, 0xff},
    0,
    0,
};

// Whether the other machine is still linked, which the link reports
static int data_ov176_0217e630 = TRUE;

// pokebw2's LiveBattleMatchIrc_Create
LiveBattleMatchIrc *func_ov176_0217e00c(GameData *gameData, HeapID heapId) {
    LiveBattleMatchIrc *irc = GFL_HeapAllocate(heapId, sizeof(LiveBattleMatchIrc), FALSE, "livebattlematch_irc.c", 142);

    MI_CpuFill8(irc, 0, sizeof(LiveBattleMatchIrc));
    irc->heapId = heapId;
    if (func_0203cd98()) {
        func_0203a574(LIVEBATTLEMATCH_IRC_COMMAND_PLAYER_DATA, data_ov176_0217e544, LIVEBATTLEMATCH_IRC_COMMAND_NUM, irc);
    }
    return irc;
}

// pokebw2's LiveBattleMatchIrc_Delete
void func_ov176_0217e058(LiveBattleMatchIrc *irc) {
    if (func_0203cd98()) {
        func_0203a5e4(LIVEBATTLEMATCH_IRC_COMMAND_PLAYER_DATA);
    }
    GFL_HeapFree(irc);
}

// pokebw2's LiveBattleMatchIrc_Main
void func_ov176_0217e074(LiveBattleMatchIrc *irc) {
}

// pokebw2's LiveBattleMatchIrc_CheckError
BOOL func_ov176_0217e078(LiveBattleMatchIrc *irc) {
    if (func_0200fb24()) {
        if (irc->p_delivery != NULL) {
            func_ov010_02157e8c(irc->p_delivery);
            irc->p_delivery = NULL;
        }
        func_0200fbb0();
        func_0200f83c();
        return TRUE;
    }
    return FALSE;
}

// pokebw2's LiveBattleMatchIrc_ResetLink
BOOL func_ov176_0217e0a8(LiveBattleMatchIrc *irc) {
    irc->state = 0;
    return TRUE;
}

// pokebw2's LiveBattleMatchIrc_StepLink
int func_ov176_0217e0b0(LiveBattleMatchIrc *irc) {
    switch (irc->state) {
    case 0:
        func_0203cbac(&data_ov176_0217e564, NULL, NULL);
        data_ov176_0217e630 = TRUE;
        irc->state = 1;
        break;
    case 1:
        if (func_0203cd98()) {
            irc->state = 2;
        }
        break;
    case 2:
        func_0203a574(LIVEBATTLEMATCH_IRC_COMMAND_PLAYER_DATA, data_ov176_0217e544, LIVEBATTLEMATCH_IRC_COMMAND_NUM, irc);
        func_ov015_021799c0(func_ov176_0217e524);
        irc->state = 3;
        break;
    case 3:
        func_0203d0dc(NULL);
        irc->state = 4;
        break;
    case 4:
        if (func_0203d1b4() > 1) {
            irc->state = 5;
        }
        if (!data_ov176_0217e630) {
            irc->state = 6;
        }
        break;
    case 5:
        func_ov015_021799c0(NULL);
        return 1;
    case 6:
        func_ov015_021799c0(NULL);
        return 2;
    }
    return 0;
}

// pokebw2's LiveBattleMatchIrc_CancelLink
void func_ov176_0217e158(LiveBattleMatchIrc *irc) {
    if (irc->state >= 2) {
        func_0203a5e4(LIVEBATTLEMATCH_IRC_COMMAND_PLAYER_DATA);
    }
    func_ov015_021799c0(NULL);
    irc->state = 0;
}

// pokebw2's LiveBattleMatchIrc_StepCancel
BOOL func_ov176_0217e178(LiveBattleMatchIrc *irc) {
    switch (irc->state) {
    case 0:
        if (func_0203cd98()) {
            func_0203ceac(NULL);
            irc->state = 1;
        } else {
            irc->state = 2;
        }
        break;
    case 1:
        if (func_0203cdbc()) {
            irc->state = 2;
        }
        break;
    case 2:
        return TRUE;
    }
    return FALSE;
}

// pokebw2's LiveBattleMatchIrc_ResetEnd
BOOL func_ov176_0217e1b8(LiveBattleMatchIrc *irc) {
    irc->state = 0;
    return TRUE;
}

// pokebw2's LiveBattleMatchIrc_StepEnd
BOOL func_ov176_0217e1c0(LiveBattleMatchIrc *irc) {
    switch (irc->state) {
    case 0:
        func_0203a5e4(LIVEBATTLEMATCH_IRC_COMMAND_PLAYER_DATA);
        func_0203ceac(NULL);
        irc->state = 1;
        break;
    case 1:
        if (func_0203cdbc()) {
            irc->state = 2;
        }
        break;
    case 2:
        return TRUE;
    }
    return FALSE;
}

// pokebw2's LiveBattleMatchIrc_SendPlayerData
BOOL func_ov176_0217e1fc(LiveBattleMatchIrc *irc, void *data) {
    u8 partnerId = func_0203d1a4(func_020399a8()) ? 0 : 1;
    NetHandle *handle = func_020399a8();

    return func_0203d3f8(handle, partnerId, LIVEBATTLEMATCH_IRC_COMMAND_PLAYER_DATA,
                         PokeParty_GetSaveDataSize() + LIVEBATTLEMATCH_PLAYER_HEADER_SIZE, data, TRUE, TRUE, TRUE);
}

// pokebw2's LiveBattleMatchIrc_TakePlayerData
BOOL func_ov176_0217e23c(LiveBattleMatchIrc *irc, void **data) {
    BOOL arrived = irc->playerDataArrived;

    if (arrived) {
        irc->playerDataArrived = FALSE;
        *data = irc->buffer;
    }
    return arrived;
}

// pokebw2's LiveBattleMatchIrc_SendParty
BOOL func_ov176_0217e258(LiveBattleMatchIrc *irc, PokeParty *party) {
    u8 partnerId = func_0203d1a4(func_020399a8()) ? 0 : 1;
    NetHandle *handle = func_020399a8();

    return func_0203d3f8(handle, partnerId, LIVEBATTLEMATCH_IRC_COMMAND_PARTY, PokeParty_GetSaveDataSize(), party, TRUE,
                         TRUE, TRUE);
}

// pokebw2's LiveBattleMatchIrc_TakeParty
BOOL func_ov176_0217e298(LiveBattleMatchIrc *irc, PokeParty *party) {
    BOOL arrived = irc->partyArrived;

    if (arrived) {
        irc->partyArrived = FALSE;
        MI_CpuCopy8(irc->buffer, party, PokeParty_GetSaveDataSize());
    }
    return arrived;
}

// pokebw2's LiveBattleMatchIrc_SendReady
BOOL func_ov176_0217e2c0(LiveBattleMatchIrc *irc, const u32 *flag) {
    u8 partnerId = func_0203d1a4(func_020399a8()) ? 0 : 1;

    return func_0203d3f8(func_020399a8(), partnerId, LIVEBATTLEMATCH_IRC_COMMAND_READY, sizeof(u32), flag, TRUE, TRUE,
                         TRUE);
}

// pokebw2's LiveBattleMatchIrc_TakeReady
BOOL func_ov176_0217e2f8(LiveBattleMatchIrc *irc, u32 *flag) {
    BOOL arrived = irc->readyArrived;

    if (arrived) {
        irc->readyArrived = FALSE;
        MI_CpuCopy8(irc->buffer, flag, sizeof(u32));
    }
    return arrived;
}

// pokebw2's LiveBattleMatchIrc_StartDelivery
void func_ov176_0217e318(LiveBattleMatchIrc *irc, void *buffer) {
    DeliveryIrcInit init;

    MI_CpuFill8(&init, 0, sizeof(DeliveryIrcInit));
    init.code = LIVEBATTLEMATCH_IRC_DELIVERY_CODE;
    init.data[0].datasize = LIVEBATTLEMATCH_IRC_DELIVERY_SIZE;
    init.data[0].pData = buffer;
    init.unk04 = 0;
    init.heapId = irc->heapId;
    init.data[0].region = region;
    init.data[0].mask = LIVEBATTLEMATCH_VERSION_MASK;
    init.dataNum = 1;
    GFL_ASSERT(irc->p_delivery == NULL);
    irc->p_delivery = func_ov010_02157dd8(&init);
    irc->state = 0;
}

// pokebw2's LiveBattleMatchIrc_StepDelivery
int func_ov176_0217e380(LiveBattleMatchIrc *irc) {
    if (irc->p_delivery != NULL) {
        func_ov010_02157e78(irc->p_delivery);
    }
    switch (irc->state) {
    case 0:
        func_ov010_02157e34(irc->p_delivery);
        irc->state++;
        break;
    case 1:
        irc->deliveryResult = func_ov010_02157e6c(irc->p_delivery);
        if (irc->deliveryResult != 0) {
            irc->state++;
        }
        break;
    case 2:
        func_ov010_02157e8c(irc->p_delivery);
        irc->p_delivery = NULL;
        irc->state++;
        break;
    case 3:
        if (func_0203d210()) {
            switch (irc->deliveryResult) {
            case 1:
                return 1;
            case 3:
                return 3;
            case 2:
            default:
                return 2;
            }
        }
        break;
    }
    return 0;
}

// pokebw2's LiveBattleMatchIrc_ResetDeliveryEnd
void func_ov176_0217e40c(LiveBattleMatchIrc *irc) {
    irc->state = 0;
}

// pokebw2's LiveBattleMatchIrc_StepDeliveryEnd
BOOL func_ov176_0217e414(LiveBattleMatchIrc *irc) {
    if (irc->p_delivery != NULL) {
        func_ov010_02157e78(irc->p_delivery);
    }
    switch (irc->state) {
    case 0:
        if (irc->p_delivery != NULL) {
            func_ov010_02157e8c(irc->p_delivery);
            irc->p_delivery = NULL;
        }
        irc->state++;
        break;
    case 1:
        if (func_0203d210()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's LiveBattleMatchIrc_OnPlayerData
static void func_ov176_0217e45c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    LiveBattleMatchIrc *irc = work;

    if (handle == func_020399a8() && netId != func_0203d1a4(func_020399a8())) {
        irc->playerDataArrived = TRUE;
    }
}

// pokebw2's LiveBattleMatchIrc_OnParty
static void func_ov176_0217e484(int netId, int size, const void *data, void *work, NetHandle *handle) {
    LiveBattleMatchIrc *irc = work;

    if (handle == func_020399a8() && netId != func_0203d1a4(func_020399a8())) {
        irc->partyArrived = TRUE;
    }
}

// pokebw2's LiveBattleMatchIrc_OnUnused
static void func_ov176_0217e4ac(int netId, int size, const void *data, void *work, NetHandle *handle) {
    LiveBattleMatchIrc *irc = work;

    if (handle == func_020399a8() && netId != func_0203d1a4(func_020399a8())) {
        irc->unusedArrived = TRUE;
    }
}

// pokebw2's LiveBattleMatchIrc_OnReady
static void func_ov176_0217e4d4(int netId, int size, const void *data, void *work, NetHandle *handle) {
    LiveBattleMatchIrc *irc = work;

    if (handle == func_020399a8() && netId != func_0203d1a4(func_020399a8())) {
        irc->readyArrived = TRUE;
    }
}

// pokebw2's LiveBattleMatchIrc_GetBuffer
static void *func_ov176_0217e4fc(int netId, void *work, int size) {
    LiveBattleMatchIrc *irc = work;

    if (netId == func_0203d1a4(func_020399a8())) {
        GFL_ASSERT(0);
        return NULL;
    }
    return irc->buffer;
}

// pokebw2's LiveBattleMatchIrc_OnLinkChanged
static BOOL func_ov176_0217e524(int a, int b) {
    BOOL linked = a == b;

    data_ov176_0217e630 = linked;
    return linked;
}
