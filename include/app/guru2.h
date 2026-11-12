// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/guru2.h: the lines in use so far.
#ifndef BWPORT_APP_GURU2_H
#define BWPORT_APP_GURU2_H

#include "types.h"
#include "field/union_app.h"
#include "struct_decls.h"

#define HEAPID_GURU2 0x72
#define GURU2_MEMBER_MAX UNION_APP_MEMBER_MAX

// The base of the net commands, and the commands. The receipt screen sends the first five
#define GURU2_NET_CMD_BASE 0x2800

// Black 2's; twinned code confirms the values of GURU2_NET_CMD_JOIN, GURU2_NET_CMD_SIGNAL, GURU2_NET_CMD_CANCEL,
// GURU2_NET_CMD_START, GURU2_NET_CMD_5, GURU2_NET_CMD_6, GURU2_NET_CMD_7, GURU2_NET_CMD_8, GURU2_NET_CMD_9,
// GURU2_NET_CMD_10, GURU2_NET_CMD_11, GURU2_NET_CMD_12, GURU2_NET_CMD_13, GURU2_NET_CMD_14, GURU2_NET_CMD_BLOCK and
// GURU2_NET_CMD_END in Black
enum {
    // A member joined
    GURU2_NET_CMD_JOIN = GURU2_NET_CMD_BASE,
    // A Guru2CommSignal, to the parent or from it
    GURU2_NET_CMD_SIGNAL,
    // The parent cancelled
    GURU2_NET_CMD_CANCEL,
    GURU2_NET_CMD_3,
    // The parent closed the group, and the trade starts
    GURU2_NET_CMD_START,
    GURU2_NET_CMD_5,
    GURU2_NET_CMD_6,
    GURU2_NET_CMD_7,
    GURU2_NET_CMD_8,
    GURU2_NET_CMD_9,
    GURU2_NET_CMD_10,
    GURU2_NET_CMD_11,
    GURU2_NET_CMD_12,
    GURU2_NET_CMD_13,
    GURU2_NET_CMD_14,
    // A member's block of GURU2_COMM_BLOCK_SIZE bytes, sent in chunks
    GURU2_NET_CMD_BLOCK,
    GURU2_NET_CMD_END,
};

// The size of what each member sends with GURU2_NET_CMD_BLOCK
#define GURU2_COMM_BLOCK_SIZE 0x594

// Black 2's; matching code confirms the offsets of app, gameData, records and started in Black
// The proc's param, which is union_main.c's UnionProcParam
typedef struct {
    UnionApp *app;
    GameData *gameData;
    u8 unk08[8];
    GameRecords *records;
    // Set by the main screen once the game has started
    BOOL started;
} Guru2Param;

typedef struct Guru2ProcWork Guru2ProcWork;
typedef struct Guru2CommWork Guru2CommWork;
typedef struct Guru2ReceiptWork Guru2ReceiptWork;
typedef struct Guru2MainWork Guru2MainWork;

// Black 2's; twinned code confirms the offsets of partyIndex, memberCount, memberBits, receiptDone, param, g2c, g2m and
// g2r, and its size in Black
// The work the three parts share, which guru2.c allocates and gives the receipt and main screens as their param
struct Guru2ProcWork {
    int state;
    // The party slot picked on the party list
    int partyIndex;
    // The members when the receipt screen ended: how many, and their net ID bits
    u32 memberCount;
    u32 memberBits;
    // Whether the receipt screen ended with the group closed, rather than cancelled
    BOOL receiptDone;
    Guru2Param *param;
    Guru2CommWork *g2c;
    Guru2MainWork *g2m;
    Guru2ReceiptWork *g2r;
};

// Black 2's; twinned code confirms the offsets of data in Black
// What Guru2Comm_Send sends: the data follows four bytes that go out with it
typedef struct {
    u32 unk0;
    u8 data[0x18];
} Guru2CommSendBuffer;

// Black 2's; twinned code confirms the offsets of g2p, send, startCount, started, unk28, joinCount, leavingBits, unk34,
// signalCount, unk3C, unk40, unk42, blockBits, unk48, unk4A, blockRecvBuffer and blocks, and its size in Black
struct Guru2CommWork {
    Guru2ProcWork *g2p;
    Guru2CommSendBuffer send;
    // How many members there were when GURU2_NET_CMD_START came
    int startCount;
    BOOL started;
    u32 unk28;
    // How many members the parent counts, and those it has let leave and that are still there (net ID bits)
    int joinCount;
    u32 leavingBits;
    u32 unk34;
    // The count of the parent's answer to this machine's request to leave
    u16 signalCount;
    u8 unk3A[2];
    // Tested by the main screen when it ends after an error
    u32 unk3C;
    // Net ID bits that the main screen's commands set
    u16 unk40;
    u16 unk42;
    u8 unk44[2];
    // The members whose block has arrived
    s16 blockBits;
    u16 unk48;
    u16 unk4A;
    // GURU2_COMM_BLOCK_SIZE bytes per member: where the chunks arrive, and the blocks that have arrived whole
    u8 *blockRecvBuffer;
    u8 *blocks;
    u8 unk54[4];
};

// pokebw2's Guru2Comm_GetBlock
// A member's block
void *func_ov137_021dce00(Guru2CommWork *g2c, int netId);
// pokebw2's Guru2Main_Exit
BOOL func_ov137_021de960(GameProc *proc, u32 *state, void *param, void *work);
// pokebw2's Guru2Main_Main
BOOL func_ov137_021dea0c(GameProc *proc, u32 *state, void *param, void *work);
// pokebw2's Guru2Main_OnButton
// What the net commands 7 to 12 call with their data
void func_ov137_021e168c(Guru2MainWork *g2m, u32 value);
// pokebw2's Guru2Main_OnGameData
void func_ov137_021e1694(Guru2MainWork *g2m, const void *data);
// pokebw2's Guru2Main_OnPosition
void func_ov137_021e172c(Guru2MainWork *g2m, const void *data);
// pokebw2's Guru2Main_OnPlayerCount
void func_ov137_021e1758(Guru2MainWork *g2m, u32 value);
// pokebw2's Guru2Main_OnPartyIndex
void func_ov137_021e180c(Guru2MainWork *g2m, int netId, u32 value);
// pokebw2's Guru2Main_OnResult
void func_ov137_021e1814(Guru2MainWork *g2m, const void *data);

// What GURU2_NET_CMD_SIGNAL carries
typedef struct {
    u8 netId;
    // The parent's answer to a request to leave: how many members there are
    u8 count;
    // GURU2_SIGNAL_*
    u8 type;
    // The parent's answer to a request to leave: whether the member may
    u8 ok;
} Guru2CommSignal;

enum {
    // A member asks the parent to leave, and the parent answers
    GURU2_SIGNAL_LEAVE_REQUEST,
    // The member leaves, which the parent tells everyone
    GURU2_SIGNAL_LEAVE,
};

// Black 2's; twinned code confirms the values of GURU2_RECEIPT_SEQ_INIT, GURU2_RECEIPT_SEQ_1, GURU2_RECEIPT_SEQ_2,
// GURU2_RECEIPT_SEQ_MAIN, GURU2_RECEIPT_SEQ_CANCEL_YESNO_INIT, GURU2_RECEIPT_SEQ_CANCEL_YESNO,
// GURU2_RECEIPT_SEQ_CANCEL_SIGNAL_WAIT, GURU2_RECEIPT_SEQ_CANCEL_OK, GURU2_RECEIPT_SEQ_CANCEL_REFUSED,
// GURU2_RECEIPT_SEQ_END_MSG, GURU2_RECEIPT_SEQ_END_WAIT, GURU2_RECEIPT_SEQ_PARENT_CANCEL_YESNO_INIT,
// GURU2_RECEIPT_SEQ_PARENT_CANCEL_YESNO, GURU2_RECEIPT_SEQ_PARENT_CANCEL_MSG, GURU2_RECEIPT_SEQ_PARENT_CANCEL_WAIT,
// GURU2_RECEIPT_SEQ_PARENT_CANCEL_SYNC, GURU2_RECEIPT_SEQ_PARENT_CANCEL_SYNC_WAIT, GURU2_RECEIPT_SEQ_NO_MEMBERS_END,
// GURU2_RECEIPT_SEQ_MEMBER_LEFT, GURU2_RECEIPT_SEQ_MEMBER_LEFT_2, GURU2_RECEIPT_SEQ_MEMBER_LEFT_WAIT,
// GURU2_RECEIPT_SEQ_START_YESNO_INIT, GURU2_RECEIPT_SEQ_START_YESNO, GURU2_RECEIPT_SEQ_START_SEND,
// GURU2_RECEIPT_SEQ_START_WAIT, GURU2_RECEIPT_SEQ_MSG_WAIT and GURU2_RECEIPT_SEQ_START in Black
// The receipt screen's sequences, which the net commands set
enum {
    GURU2_RECEIPT_SEQ_INIT,
    GURU2_RECEIPT_SEQ_1,
    GURU2_RECEIPT_SEQ_2,
    GURU2_RECEIPT_SEQ_MAIN,
    GURU2_RECEIPT_SEQ_CANCEL_YESNO_INIT,
    GURU2_RECEIPT_SEQ_CANCEL_YESNO,
    GURU2_RECEIPT_SEQ_CANCEL_SIGNAL_WAIT,
    GURU2_RECEIPT_SEQ_CANCEL_OK,
    GURU2_RECEIPT_SEQ_CANCEL_REFUSED,
    GURU2_RECEIPT_SEQ_END_MSG,
    GURU2_RECEIPT_SEQ_END_WAIT,
    GURU2_RECEIPT_SEQ_PARENT_CANCEL_YESNO_INIT,
    GURU2_RECEIPT_SEQ_PARENT_CANCEL_YESNO,
    GURU2_RECEIPT_SEQ_PARENT_CANCEL_MSG,
    GURU2_RECEIPT_SEQ_PARENT_CANCEL_WAIT,
    GURU2_RECEIPT_SEQ_PARENT_CANCEL_SYNC,
    GURU2_RECEIPT_SEQ_PARENT_CANCEL_SYNC_WAIT,
    GURU2_RECEIPT_SEQ_NO_MEMBERS_MSG_WAIT,
    GURU2_RECEIPT_SEQ_NO_MEMBERS_END,
    GURU2_RECEIPT_SEQ_MEMBER_LEFT,
    GURU2_RECEIPT_SEQ_MEMBER_LEFT_2,
    GURU2_RECEIPT_SEQ_MEMBER_LEFT_WAIT,
    GURU2_RECEIPT_SEQ_START_YESNO_INIT,
    GURU2_RECEIPT_SEQ_START_YESNO,
    GURU2_RECEIPT_SEQ_START_SEND,
    GURU2_RECEIPT_SEQ_START_WAIT,
    GURU2_RECEIPT_SEQ_MSG_WAIT,
    GURU2_RECEIPT_SEQ_START,
    GURU2_RECEIPT_SEQ_COUNT,
};

// pokebw2's Guru2Comm_AddCommands
// Adds the net commands
void func_ov137_021dcb78(Guru2CommWork *g2c);
// pokebw2's Guru2Receipt_Main
BOOL func_ov137_021dcf80(GameProc *proc, u32 *state, void *param, void *work);
// pokebw2's Guru2Receipt_Exit
BOOL func_ov137_021dd0b8(GameProc *proc, u32 *state, void *param, void *work);
// pokebw2's Guru2Receipt_SetSeq
// Called by the net commands: sets the screen's sequence
void func_ov137_021de08c(Guru2ReceiptWork *g2r, int seq, u8 netId);
// pokebw2's Guru2Receipt_SetSeqOnJoin
void func_ov137_021de17c(Guru2ReceiptWork *g2r, int seq, u8 netId);

#define GURU2_NET_CMD_COUNT (GURU2_NET_CMD_END - GURU2_NET_CMD_BASE)

// pokebw2's Guru2Comm_Create
// guru2_comm.c
Guru2CommWork *func_ov137_021dcb04(Guru2ProcWork *g2p, u32 heapId);
// pokebw2's Guru2Comm_Free
void func_ov137_021dcb60(Guru2CommWork *g2c);

// Black 2's, not checked against Black
// What guru2.c's Guru2ProcWork.state says the Spin Trade's screen was left for
enum {
    GURU2_PROC_STATE_POKE_LIST = 1,
    GURU2_PROC_STATE_MAIN = 3,
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's Guru2Comm_Send
// Sends a GURU2_NET_CMD_*, returning whether it could
BOOL func_ov137_021dcbc4(Guru2CommWork *g2c, u32 command, const void *data, u32 size);
// pokebw2's Guru2Main_Init
// guru2_main.c, the screen with the disc
BOOL func_ov137_021de7fc(GameProc *proc, u32 *state, void *param, void *work);
// pokebw2's Guru2Receipt_Init
// guru2_receipt.c, the screen where the group gathers
BOOL func_ov137_021dce10(GameProc *proc, u32 *state, void *param, void *work);
// pokebw2's Guru2Comm_End
void func_ov137_021dcb90(Guru2CommWork *g2c);

// The proc, which the Union Room runs for the other activities
extern const GameProcFunctions data_ov137_021e1a2c;

#endif // BWPORT_APP_GURU2_H
