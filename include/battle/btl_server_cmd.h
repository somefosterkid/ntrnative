// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_server_cmd.h: the lines in use so
// far.
#ifndef BWPORT_BATTLE_BTL_SERVER_CMD_H
#define BWPORT_BATTLE_BTL_SERVER_CMD_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "gfl/std.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
#define BTL_SERVER_CMD_QUE_SIZE 3000

// The commands that the turn's flow writes, which the server then sends
struct BtlServerCmdQueue {
    u32 writePtr;
    u32 readPtr;
    u8 buffer[BTL_SERVER_CMD_QUE_SIZE];
};

u16 func_ov093_021cd5e0(BtlServerCmdQueue *que, u32 *args);

u16 SCQUE_RESERVE_Pos(BtlServerCmdQueue *que, u32 event);
void func_ov093_021cd64c(BtlServerCmdQueue *que, u8 event, ...);
void func_ov093_021cd568(BtlServerCmdQueue *que, u32 reserve, u32 event, ...);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u8 func_ov093_021cd644(BtlServerCmdQueue *que);
void func_ov093_021cd4b0(BtlServerCmdQueue *que, u32 event, ...);
void func_ov093_021cd63c(BtlServerCmdQueue *que, u8 value);
void func_ov093_021cd6ec(void);

// Black 2's, not checked against Black
// Fills the queue with a block of commands the server sent
static inline void BtlServerCmdQueue_Setup(BtlServerCmdQueue *queue, const void *data, u16 size) {
    MIi_CpuCopy32(data, queue->buffer, size);
    queue->writePtr = size;
    queue->readPtr = 0;
}

// Black 2's, not checked against Black
// Whether every command has been read
static inline BOOL BtlServerCmdQueue_IsEmpty(const BtlServerCmdQueue *queue) {
    return queue->readPtr == queue->writePtr;
}

// Black 2's, not checked against Black
static inline void BtlServerCmdQueue_Init(BtlServerCmdQueue *queue) {
    queue->writePtr = 0;
    queue->readPtr = 0;
}

// Black's: command 0x18, the outcome of the move a mon used (the same values func_ov093_021d6744 keeps in the mon);
// inlined in func_ov093_021bde98, which reads the result and the type before it calls GetMonID
static inline void ServerCmd_PutMoveResult(BtlServerCmdQueue *queue, u8 monId, u8 target, u8 result, u8 type,
                                           u16 actualMove, u16 move) {
    func_ov093_021cd4b0(queue, 0x18, monId, target, result, type, actualMove, move);
}

// Black's: command 0x54, the damage a move dealt to a mon; inlined in func_ov093_021c4104 (its arguments are
// narrowed and read last to first, before GetMonID is called)
static inline void ServerCmd_PutMoveDamage(BtlServerCmdQueue *queue, u8 monId, u8 effectiveness, u16 move) {
    func_ov093_021cd4b0(queue, 0x54, monId, effectiveness, move);
}

#endif // BWPORT_BATTLE_BTL_SERVER_CMD_H
