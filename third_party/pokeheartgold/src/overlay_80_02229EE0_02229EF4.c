#include "global.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "constants/heap.h"
#include "msgdata.h"
#include "pm_string.h"
#include "scrcmd_9.h"

void ov80_02229EF4(void *param_1, s32 param_2, NarcId param_3);

typedef struct FrontierTrainerBase FrontierTrainerBase;
struct FrontierTrainerBase {
    u16 trainerType;
    u16 numSets;
    u16 setIDs[];
};
FrontierTrainerBase *ov80_02229EE0(u16, enum HeapID, enum NarcId);
FrontierTrainerBase *ov80_02229F04(FrontierTrainer *trainer, s32 trainerID, enum HeapID heapID, enum NarcId narcID);

void ov80_02229EF4(void *param_1, s32 param_2, NarcId param_3) {
    ReadWholeNarcMemberByIdPair(param_1, param_3, param_2);
    return;
}

FrontierTrainerBase *ov80_02229F04(FrontierTrainer *trainer, s32 trainerID, enum HeapID heapID, enum NarcId narcID) {
    enum NarcId v0;
    FrontierTrainerBase *frontierTrainerBase;
    MsgData *msgData;
    String *string;

    v0 = narcID;
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x1B, heapID);
    MI_CpuFill8(trainer, 0U, 0x30U);
    frontierTrainerBase = ov80_02229EE0((u16)trainerID, heapID, v0);
    trainer->index = (u32)trainerID;
    trainer->introMessage.msg_bank = 0xFFFF;
    trainer->introMessage.msg_no = trainerID * 3;
    trainer->trainerClass = frontierTrainerBase->trainerType;
    string = NewString_ReadMsgData(msgData, trainerID);
    CopyStringToU16Array(string, trainer->name, 8U);
    String_Delete(string);
    DestroyMsgData(msgData);
    return frontierTrainerBase;
}
