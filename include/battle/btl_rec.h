// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_rec.h: the lines in use so far.
#ifndef BWPORT_BATTLE_BTL_REC_H
#define BWPORT_BATTLE_BTL_REC_H

#include "types.h"
#include "battle/btl_action.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// The actions of a turn, as a chunk of the recording: the chunk's header in data[0] and data[1], then the clients in
// clientFlags, each with a byte (client ID in bits 5 to 7, action count in bits 0 to 4) and its actions
struct BtlRecTool {
    u8 size;
    u8 clientFlags;
    u8 numClients;
    // The chunk's type and whether it starts a chapter, for its header
    u8 type : 6;
    u8 chapter : 1;
    u8 overflow : 1;
    u8 data[0x3c];
};

// Black 2's; twinned code confirms the offsets of data, size, error and pos in Black
// The reader of a recorded battle's actions, which the main module holds
struct BtlRecReader {
    const u8 *data;
    u32 size : 31;
    // Set when the recording ran out or was corrupt; every client gets a blank action from then on
    u32 error : 1;
    // Each client's read position
    u32 pos[4];
    // Each client's actions of the last chunk read
    BattleAction actions[4][16];
};

u8 func_ov093_021ee638(const void *data);
void *func_ov093_021ee63c(BtlRecorder *recorder, u32 *size);
void func_ov093_021ee644(BtlRecReader *reader, const void *data, u32 size);
void func_ov093_021ee674(BtlRecReader *reader);
BOOL func_ov093_021ee688(BtlRecReader *reader, u8 clientId);
u32 func_ov093_021ee830(BtlRecReader *reader);
BOOL func_ov093_021ee894(BtlRecReader *reader, u8 clientId);
void func_ov093_021ee8b4(BtlRecTool *tool, BOOL chapter);
void *func_ov093_021ee8d8(BtlRecTool *tool, u32 *size, BOOL chapter);
void func_ov093_021ee8f4(BtlRecTool *tool, u8 clientId, const void *actions, u8 count);
void *func_ov093_021ee96c(BtlRecTool *tool, u8 value, u32 *size);
void *func_ov093_021ee9a4(BtlRecTool *tool, u32 *size);
void func_ov093_021ee9b4(BtlRecTool *tool, const void *data, u32 size);
BOOL func_ov093_021ee9e4(BtlRecTool *tool, u32 *pos, u8 *clientId, u8 *count, void *actions);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BtlRecorder *func_ov093_021ee5e4(HeapID heapId);
void func_ov093_021ee600(BtlRecorder *recorder);
void func_ov093_021ee608(BtlRecorder *recorder, const u8 *data, u32 size);
BattleAction *func_ov093_021ee6b8(BtlRecReader *reader, u8 clientId, u8 *count, u8 *chapter);

#endif // BWPORT_BATTLE_BTL_REC_H
