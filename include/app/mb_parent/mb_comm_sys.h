// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/mb_parent/mb_comm_sys.h: the lines in
// use so far.
#ifndef BWPORT_APP_MB_PARENT_MB_COMM_SYS_H
#define BWPORT_APP_MB_PARENT_MB_COMM_SYS_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Black 2's; twinned code confirms the values of MB_COMM_CMD_ACK, MB_COMM_CMD_ANSWER, MB_COMM_CMD_ITEM_ANSWER and
// MB_COMM_CMD_FINISH in Black
// The commands of the main packet, which carries a command and a value. Both sides send them; the names say what
// the child means by those the parent receives
enum {
    MB_COMM_CMD_ACK,
    MB_COMM_CMD_SAVE_SYNC_1,
    MB_COMM_CMD_SAVE_SYNC_2,
    MB_COMM_CMD_SAVE_SYNC_3,
    MB_COMM_CMD_SAVE_SYNC_4,
    MB_COMM_CMD_BOX_SPACE,
    MB_COMM_CMD_STATE = 10,
    MB_COMM_CMD_SAVE_READY,
    MB_COMM_CMD_SAVE_START,
    MB_COMM_CMD_SAVE_MID,
    MB_COMM_CMD_SAVED,
    MB_COMM_CMD_ANSWER,
    MB_COMM_CMD_ITEM_ANSWER,
    MB_COMM_CMD_FINISH,
    MB_COMM_CMD_END,
    MB_COMM_CMD_RESULT,
    MB_COMM_CMD_MORE,
    MB_COMM_CMD_ITEM,
    MB_COMM_CMD_CLOSE,
};

// pokebw2's MBComm_StartNet
void func_ov107_021f0efc(MBCommSys *comm);
// pokebw2's MBComm_EndNet
void func_ov107_021f0fe8(MBCommSys *comm);
// pokebw2's MBComm_Connect
void func_ov107_021f1018(MBCommSys *comm);
// pokebw2's MBComm_StartDisconnect
void func_ov107_021f1028(MBCommSys *comm);
// pokebw2's MBComm_IsDisconnected
BOOL func_ov107_021f1040(MBCommSys *comm);
// pokebw2's MBComm_IsConnected
BOOL func_ov107_021f1050(MBCommSys *comm);
// pokebw2's MBComm_IsSaveReady
BOOL func_ov107_021f10d4(MBCommSys *comm);
// pokebw2's MBComm_IsSaveStarted
BOOL func_ov107_021f10dc(MBCommSys *comm);
// pokebw2's MBComm_IsSaved
BOOL func_ov107_021f10ec(MBCommSys *comm);
// pokebw2's MBComm_IsSaveSync1
BOOL func_ov107_021f10f4(MBCommSys *comm);
// pokebw2's MBComm_IsSaveSync4
BOOL func_ov107_021f1104(MBCommSys *comm);
// pokebw2's MBComm_HasResult
BOOL func_ov107_021f1110(MBCommSys *comm);
// pokebw2's MBComm_GetResultCount
u16 func_ov107_021f1118(MBCommSys *comm);
// pokebw2's MBComm_GetResultMoreCount
u16 func_ov107_021f1120(MBCommSys *comm);
// pokebw2's MBComm_GetResultFlag1
// Bits 30 and 31 of the result
BOOL func_ov107_021f1134(MBCommSys *comm);
// pokebw2's MBComm_GetResultFlag2
BOOL func_ov107_021f1148(MBCommSys *comm);
// pokebw2's MBComm_HasMore
BOOL func_ov107_021f115c(MBCommSys *comm);
// pokebw2's MBComm_HasItemInfo
BOOL func_ov107_021f1164(MBCommSys *comm);
// pokebw2's MBComm_HasItem
BOOL func_ov107_021f116c(MBCommSys *comm);
// pokebw2's MBComm_ClearPokemon
void func_ov107_021f1184(MBCommSys *comm);
// pokebw2's MBComm_IsPokemonReceived
BOOL func_ov107_021f11bc(MBCommSys *comm);
// pokebw2's MBComm_GetPokemonCount
u8 func_ov107_021f11c4(MBCommSys *comm);
// pokebw2's MBComm_GetPokemon
BoxPkm *func_ov107_021f11cc(MBCommSys *comm, u8 index);
// pokebw2's MBComm_GetScore
u16 func_ov107_021f11e0(MBCommSys *comm);
// pokebw2's MBComm_IsAcked
BOOL func_ov107_021f11e8(MBCommSys *comm);
// pokebw2's MBComm_SendCommand
BOOL func_ov107_021f11f0(MBCommSys *comm, u8 command, u32 value);
// pokebw2's MBComm_SendParentInfo
BOOL func_ov107_021f1370(MBCommSys *comm, MBCommParentInfo *info);

struct MBCommParentInfo {
    u32 textSpeed;
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's MBComm_Create
MBCommSys *func_ov107_021f0de8(HeapID heapId);
// pokebw2's MBComm_Delete
void func_ov107_021f0e40(MBCommSys *comm);
// pokebw2's MBComm_Update
void func_ov107_021f0e6c(MBCommSys *comm);
// pokebw2's MBComm_IsNetReady
BOOL func_ov107_021f1008(MBCommSys *comm);
// pokebw2's MBComm_IsNetEnded
BOOL func_ov107_021f1010(MBCommSys *comm);
// pokebw2's MBComm_GetState
int func_ov107_021f1060(MBCommSys *comm);
// pokebw2's MBComm_ResetCommands
void func_ov107_021f1064(MBCommSys *comm);
// pokebw2's MBComm_IsSaveMidReached
BOOL func_ov107_021f10e4(MBCommSys *comm);
// pokebw2's MBComm_SendProgram
void func_ov107_021f1174(MBCommSys *comm, void *data, u32 size);

#endif // BWPORT_APP_MB_PARENT_MB_COMM_SYS_H
