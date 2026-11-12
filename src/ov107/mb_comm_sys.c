// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov181/mb_comm_sys.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/mb_parent/mb_comm_sys.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/net_lower_data.h"
#include "pml/poke_party.h"


// The connection between the DS Download Play parent and the child program once it has booted (mb_comm_sys.c): the
// parent sends the child a program, the child sends back Pokémon or, for the Memory Link, 0x500 bytes of its save
// data, and the two sides tell each other their progress with commands of one byte and a value. The names are ours,
// from how the parent uses them

// The game's commands, from 0x400 as the init data's gameCommandBase of 4 places them
enum {
    MB_COMM_NETCMD_PACKET = 0x400,
    MB_COMM_NETCMD_PARENT_INFO,
    MB_COMM_NETCMD_POKEMON,
    MB_COMM_NETCMD_DATA,
};

// The connection's states
enum {
    MB_COMM_SEQ_NONE,
    MB_COMM_SEQ_WAIT_CONNECT,
    MB_COMM_SEQ_WAIT_NEGOTIATION,
    MB_COMM_SEQ_WAIT_TIMING,
    MB_COMM_SEQ_WAIT_DISCONNECT_TIMING,
    MB_COMM_SEQ_DISCONNECTED,
    MB_COMM_SEQ_CONNECTED,
};

#define MB_COMM_POKEMON_MAX 6

// The Pokémon the child sends: how many, the score of the Poké Transfer game, then the Pokémon
typedef struct {
    u8 count;
    u16 score;
} MBCommPokemonHeader;

typedef struct {
    u32 value;
    u8 command;
} MBCommPacket;

struct MBCommSys {
    HeapID heapId;
    int seq;
    // The state to tell the other side, and the state last shared
    u32 pendingState;
    u32 state;
    MBCommPokemonHeader *pokemon;
    MBCommParentInfo *parentInfo;
    // Which commands have arrived
    u16 parentInfoReceived : 1;
    u16 pokemonReceived : 1;
    u16 ack : 1;
    u16 saveReady : 1;
    u16 saveStart : 1;
    u16 saveMid : 1;
    u16 saved : 1;
    u16 saveSync1 : 1;
    u16 saveSync2 : 1;
    u16 saveSync3 : 1;
    u16 saveSync4 : 1;
    u16 netStarted : 1;
    u16 boxSpaceReceived : 1;
    u16 answerReceived : 1;
    u16 resultReceived : 1;
    u16 more : 1;
    u16 itemReceived : 1;
    u16 hasItem : 1;
    u16 itemAnswerReceived : 1;
    u16 itemAnswer : 1;
    u16 finish : 1;
    u16 end : 1;
    u16 dataReceived : 1;
    u16 close : 1;
    u8 syncValue;
    u16 boxSpace;
    u8 answer;
    u8 unk_21[3];
    // The count of Pokémon to receive in the low 16 bits, then more counts and two flags
    u32 result;
    void *data;
};

static void func_ov107_021f1230(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov107_021f13ac(int netId, int size, const void *data, void *work, NetHandle *handle);
void *func_ov107_021f13bc(int netId, void *work, int size);
static void func_ov107_021f13e8(int netId, int size, const void *data, void *work, NetHandle *handle);
static void *func_ov107_021f13f4(int netId, void *work, int size);
void *func_ov107_021f13f8(void *work);
static int func_ov107_021f1400(void *work);

// pokebw2's sMBCommCommands
static const NetCommand data_ov107_021f6c7c[] = {
    { func_ov107_021f1230, NULL },
    { func_ov107_021f13ac, func_ov107_021f13bc },
    { func_ov107_021f13e8, func_ov107_021f13f4 },
};

// pokebw2's sMBCommBeaconData
static u16 data_ov107_021f6ee8;

// pokebw2's MBComm_Create
MBCommSys *func_ov107_021f0de8(HeapID heapId) {
    MBCommSys *comm = GFL_HeapAllocate(heapId, 0x28, TRUE, "mb_comm_sys.c", 159);

    comm->heapId = heapId;
    comm->seq = MB_COMM_SEQ_NONE;
    comm->parentInfo = NULL;
    comm->netStarted = FALSE;
    comm->state = 0;
    comm->pendingState = 0;
    comm->pokemon =
        GFL_HeapAllocate(comm->heapId, PML_GetPkmRawSize() * MB_COMM_POKEMON_MAX + sizeof(MBCommPokemonHeader), TRUE,
                         "mb_comm_sys.c", 169);
    return comm;
}

// pokebw2's MBComm_Delete
void func_ov107_021f0e40(MBCommSys *comm) {
    if (comm->parentInfo != NULL) {
        GFL_HeapFree(comm->parentInfo);
    }
    if (comm->netStarted == TRUE) {
        func_0203e6c0();
    }
    GFL_HeapFree(comm->pokemon);
    GFL_HeapFree(comm);
}

// pokebw2's MBComm_Process
void func_ov107_021f0e6c(MBCommSys *comm) {
    switch (comm->seq) {
    case MB_COMM_SEQ_WAIT_CONNECT:
        if (func_02039acc() == TRUE) {
            comm->seq = MB_COMM_SEQ_WAIT_NEGOTIATION;
        }
        break;
    case MB_COMM_SEQ_WAIT_NEGOTIATION:
        if (func_020399b8(func_020399a8()) == TRUE) {
            comm->seq = MB_COMM_SEQ_WAIT_TIMING;
        }
        break;
    case MB_COMM_SEQ_WAIT_TIMING:
        if (func_0203d394(func_020399a8()) == TRUE) {
            comm->seq = MB_COMM_SEQ_CONNECTED;
            func_0203d7bc(TRUE);
        }
        break;
    case MB_COMM_SEQ_WAIT_DISCONNECT_TIMING:
        if (func_02039cb4(func_020399a8(), 0x40, 4) == TRUE) {
            comm->seq = MB_COMM_SEQ_DISCONNECTED;
            func_0203d7bc(FALSE);
        }
        break;
    case MB_COMM_SEQ_DISCONNECTED:
    case MB_COMM_SEQ_CONNECTED:
        break;
    }
    if (comm->state != comm->pendingState && comm->pendingState != 0) {
        if (func_ov107_021f11f0(comm, MB_COMM_CMD_STATE, comm->pendingState) == TRUE) {
            comm->state = comm->pendingState;
        }
    }
}


// pokebw2's MBComm_StartNet
void func_ov107_021f0efc(MBCommSys *comm) {
    GFLNetInitData init = {
        data_ov107_021f6c7c,
        NELEMS(data_ov107_021f6c7c),
        NULL,
        NULL,
        NULL,
        NULL,
        func_ov107_021f13f8,
        func_ov107_021f1400,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        { 0 },
        NULL,
        NULL,
        0,
        { 1, 0, 0, 0, 0x80, 0x13, 0, 0 },
        HEAPID_SYSTEM,
        0xd,
        0xf,
        0xf,
        0xf0,
        0,
        2,
        0x30,
        2,
        1,
        1,
        0,
        0,
        4,
        { 0x2c, 1, 0, 0 },
        0x1d0,
        0,
    };

    init.parentHeapId = comm->heapId;
    func_0203cbac(&init, NULL, comm);
    comm->result = 0;
    comm->parentInfoReceived = FALSE;
    comm->boxSpace = 0;
    comm->saveReady = FALSE;
    comm->syncValue = 0;
    comm->saveStart = FALSE;
    comm->saveMid = FALSE;
    comm->saved = FALSE;
    comm->saveSync1 = FALSE;
    comm->saveSync2 = FALSE;
    comm->saveSync3 = FALSE;
    comm->saveSync4 = FALSE;
    comm->netStarted = TRUE;
    comm->resultReceived = FALSE;
    comm->answerReceived = FALSE;
    comm->more = FALSE;
    comm->itemReceived = FALSE;
    comm->hasItem = FALSE;
    comm->itemAnswerReceived = FALSE;
    comm->itemAnswer = FALSE;
    comm->finish = FALSE;
    comm->end = FALSE;
    comm->answer = 0;
    func_0203e5f8(comm->heapId, FALSE);
}

// pokebw2's MBComm_EndNet
void func_ov107_021f0fe8(MBCommSys *comm) {
    func_0203ceac(NULL);
    func_0203e6c0();
    comm->netStarted = FALSE;
    comm->seq = MB_COMM_SEQ_NONE;
}

// pokebw2's MBComm_IsNetReady
BOOL func_ov107_021f1008(MBCommSys *comm) {
    return func_0203cd98();
}

// pokebw2's MBComm_IsNetEnded
BOOL func_ov107_021f1010(MBCommSys *comm) {
    return func_0203cdbc();
}

// pokebw2's MBComm_Connect
void func_ov107_021f1018(MBCommSys *comm) {
    func_0203d01c();
    comm->seq = MB_COMM_SEQ_WAIT_CONNECT;
}

// pokebw2's MBComm_StartDisconnect
void func_ov107_021f1028(MBCommSys *comm) {
    func_02039c68(func_020399a8(), 0x40, 4);
    comm->seq = MB_COMM_SEQ_WAIT_DISCONNECT_TIMING;
}

// pokebw2's MBComm_IsDisconnected
BOOL func_ov107_021f1040(MBCommSys *comm) {
    if (comm->seq == MB_COMM_SEQ_DISCONNECTED) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's MBComm_IsConnected
BOOL func_ov107_021f1050(MBCommSys *comm) {
    if (comm->seq >= MB_COMM_SEQ_CONNECTED) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's MBComm_GetState
int func_ov107_021f1060(MBCommSys *comm) {
    return comm->state;
}

// pokebw2's MBComm_ResetCommands
void func_ov107_021f1064(MBCommSys *comm) {
    comm->pokemonReceived = FALSE;
    comm->ack = FALSE;
    comm->saveReady = FALSE;
    comm->saveStart = FALSE;
    comm->saveMid = FALSE;
    comm->saved = FALSE;
    comm->saveSync1 = FALSE;
    comm->saveSync2 = FALSE;
    comm->saveSync3 = FALSE;
    comm->saveSync4 = FALSE;
    comm->boxSpaceReceived = FALSE;
    comm->syncValue = 0;
}

// pokebw2's MBComm_IsSaveReady
BOOL func_ov107_021f10d4(MBCommSys *comm) {
    return comm->saveReady;
}

// pokebw2's MBComm_IsSaveStarted
BOOL func_ov107_021f10dc(MBCommSys *comm) {
    return comm->saveStart;
}

// pokebw2's MBComm_IsSaveMidReached
BOOL func_ov107_021f10e4(MBCommSys *comm) {
    // BUG: This reads the flag of MB_COMM_CMD_SAVE_START again, and nothing reads that of MB_COMM_CMD_SAVE_MID, so the
    // parent goes on to the next step of the save without waiting for the child to reach the middle
#ifdef BUGFIX
    return comm->saveMid;
#else
    return comm->saveStart;
#endif
}

// pokebw2's MBComm_IsSaved
BOOL func_ov107_021f10ec(MBCommSys *comm) {
    return comm->saved;
}

// pokebw2's MBComm_IsSaveSync1
BOOL func_ov107_021f10f4(MBCommSys *comm) {
    return comm->saveSync1;
}

// pokebw2's MBComm_IsSaveSync2
BOOL func_ov107_021f10fc(MBCommSys *comm) {
    return comm->saveSync2;
}


// pokebw2's MBComm_IsSaveSync4
BOOL func_ov107_021f1104(MBCommSys *comm) {
    return comm->saveSync4;
}

u8 func_ov107_021f110c(struct MBCommSys *sys)
{
    return sys->syncValue;
}


// pokebw2's MBComm_HasResult
BOOL func_ov107_021f1110(MBCommSys *comm) {
    return comm->resultReceived;
}

// pokebw2's MBComm_GetResultCount
u16 func_ov107_021f1118(MBCommSys *comm) {
    return comm->result;
}

// pokebw2's MBComm_GetResultMoreCount
u16 func_ov107_021f1120(MBCommSys *comm) {
    return (comm->result & 0x3fff0000) >> 16;
}

// pokebw2's MBComm_GetResultFlag1
BOOL func_ov107_021f1134(MBCommSys *comm) {
    if (comm->result & (1 << 30)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's MBComm_GetResultFlag2
BOOL func_ov107_021f1148(MBCommSys *comm) {
    if (comm->result & 0x80000000) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's MBComm_HasMore
BOOL func_ov107_021f115c(MBCommSys *comm) {
    return comm->more;
}

// pokebw2's MBComm_HasItemInfo
BOOL func_ov107_021f1164(MBCommSys *comm) {
    return comm->itemReceived;
}

// pokebw2's MBComm_HasItem
BOOL func_ov107_021f116c(MBCommSys *comm) {
    return comm->hasItem;
}

// pokebw2's MBComm_SendProgram
void func_ov107_021f1174(MBCommSys *comm, void *data, u32 size) {
    func_0203e758(data, size, 2, TRUE);
}

// pokebw2's MBComm_ClearPokemon
void func_ov107_021f1184(MBCommSys *comm) {
    u8 i;
    MBCommPokemonHeader *header = comm->pokemon;

    header->count = 0;
    header->score = 0;
    for (i = 0; i < MB_COMM_POKEMON_MAX; i++) {
        PML_PkmInit((BoxPkm *)((u8 *)comm->pokemon + PML_GetPkmRawSize() * i + sizeof(MBCommPokemonHeader)));
    }
    comm->pokemonReceived = FALSE;
    comm->ack = FALSE;
}

// pokebw2's MBComm_IsPokemonReceived
BOOL func_ov107_021f11bc(MBCommSys *comm) {
    return comm->pokemonReceived;
}

// pokebw2's MBComm_GetPokemonCount
u8 func_ov107_021f11c4(MBCommSys *comm) {
    return comm->pokemon->count;
}

// pokebw2's MBComm_GetPokemon
BoxPkm *func_ov107_021f11cc(MBCommSys *comm, u8 index) {
    return (BoxPkm *)((u8 *)comm->pokemon + PML_GetPkmRawSize() * index + sizeof(MBCommPokemonHeader));
}

// pokebw2's MBComm_GetScore
u16 func_ov107_021f11e0(MBCommSys *comm) {
    return comm->pokemon->score;
}

// pokebw2's MBComm_IsAcked
BOOL func_ov107_021f11e8(MBCommSys *comm) {
    return comm->ack;
}

// pokebw2's MBComm_SendCommand
BOOL func_ov107_021f11f0(MBCommSys *comm, u8 command, u32 value) {
    NetHandle *handle;
    MBCommPacket packet;

    if (func_0203d378() == TRUE) {
        handle = func_0203996c(0xff);
    } else {
        handle = func_020399a8();
    }
    packet.command = command;
    packet.value = value;
    return func_0203d3f8(handle, 0xff, MB_COMM_NETCMD_PACKET, sizeof(MBCommPacket), &packet, TRUE, FALSE, FALSE);
}

// pokebw2's MBComm_RecvPacket
static void func_ov107_021f1230(int netId, int size, const void *data, void *work, NetHandle *handle) {
    const MBCommPacket *packet = data;
    MBCommSys *comm = work;

    switch (packet->command) {
    case MB_COMM_CMD_ACK:
        comm->ack = TRUE;
        break;
    case MB_COMM_CMD_SAVE_SYNC_1:
        comm->saveSync1 = TRUE;
        break;
    case MB_COMM_CMD_SAVE_SYNC_2:
        comm->saveSync2 = TRUE;
        comm->syncValue = packet->value;
        break;
    case MB_COMM_CMD_SAVE_SYNC_3:
        comm->saveSync3 = TRUE;
        comm->syncValue = packet->value;
        break;
    case MB_COMM_CMD_SAVE_SYNC_4:
        comm->saveSync4 = TRUE;
        break;
    case MB_COMM_CMD_BOX_SPACE:
        comm->boxSpace = packet->value;
        comm->boxSpaceReceived = TRUE;
        break;
    case MB_COMM_CMD_STATE:
        comm->state = packet->value;
        break;
    case MB_COMM_CMD_SAVE_READY:
        comm->saveReady = TRUE;
        break;
    case MB_COMM_CMD_SAVE_START:
        comm->saveStart = TRUE;
        break;
    case MB_COMM_CMD_SAVE_MID:
        comm->saveMid = TRUE;
        break;
    case MB_COMM_CMD_SAVED:
        comm->saved = TRUE;
        break;
    case MB_COMM_CMD_ANSWER:
        comm->answerReceived = TRUE;
        comm->answer = packet->value;
        break;
    case MB_COMM_CMD_ITEM_ANSWER:
        comm->itemAnswerReceived = TRUE;
        comm->itemAnswer = packet->value;
        break;
    case MB_COMM_CMD_FINISH:
        comm->finish = TRUE;
        break;
    case MB_COMM_CMD_END:
        comm->end = TRUE;
        break;
    case MB_COMM_CMD_RESULT:
        comm->resultReceived = TRUE;
        comm->result = packet->value;
        break;
    case MB_COMM_CMD_MORE:
        comm->more = TRUE;
        break;
    case MB_COMM_CMD_ITEM:
        comm->itemReceived = TRUE;
        comm->hasItem = packet->value;
        break;
    }
}

// pokebw2's MBComm_SendParentInfo
BOOL func_ov107_021f1370(MBCommSys *comm, MBCommParentInfo *info) {
    NetHandle *handle = func_0203996c(0xff);

    if (func_0203d1b4() < 2) {
        return FALSE;
    }
    return func_0203d3f8(handle, 1, MB_COMM_NETCMD_PARENT_INFO, 8, info, TRUE, FALSE, TRUE);
}

// pokebw2's MBComm_RecvParentInfo
static void func_ov107_021f13ac(int netId, int size, const void *data, void *work, NetHandle *handle) {
    MBCommSys *comm = work;

    comm->parentInfoReceived = TRUE;
}

void *func_ov107_021f13bc(int netId, void *work, int size) {
    MBCommSys *comm = work;

    if (comm->parentInfo == NULL) {
        comm->parentInfo = GFL_HeapAllocate(comm->heapId, 8, TRUE, "mb_comm_sys.c", 862);
    }
    return comm->parentInfo;
}

// pokebw2's MBComm_RecvPokemon
static void func_ov107_021f13e8(int netId, int size, const void *data, void *work, NetHandle *handle) {
    MBCommSys *comm = work;

    comm->pokemonReceived = TRUE;
}

// pokebw2's MBComm_GetPokemonBuffer
static void *func_ov107_021f13f4(int netId, void *work, int size) {
    MBCommSys *comm = work;

    return comm->pokemon;
}

void *func_ov107_021f13f8(void *work) {
    return &data_ov107_021f6ee8;
}


// pokebw2's MBComm_GetBeaconDataSize
static int func_ov107_021f1400(void *work) {
    return sizeof(data_ov107_021f6ee8);
}
