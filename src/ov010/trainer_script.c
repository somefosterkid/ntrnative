// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/trainer_script.c. Black's Trainer flags
// start at 0x58c, and Black's file ends before Black 2's resetRebattleTrainers.
#include "types.h"
#include "battle/trainer_data.h"
#include "field/field_script.h"
#include "field/trainer_script.h"
#include "save/event_work.h"

void SetupTrainerClashSlot(GameEvent *event, int index, const TrainerClashData *data) {
    ScriptWork *work = EventScriptCall_GetWork(event);
    TrainerClashSlot *dst = ScriptWork_GetTrainerState(work, index);

    dst->data = *data;
    dst->eye = NULL;
}

u16 GetNPCTrainerIDFromSCRID(u32 scriptId) {
    if (scriptId < 0x1388) {
        return scriptId - 0xbb8;
    }
    return scriptId - 0x1388;
}

u16 GetNormalSCRIDFromTrainerID(u32 trainerId) {
    return trainerId + 0xbb8;
}

u16 GetPairMember2SCRIDFromTrainerID(u32 trainerId) {
    return trainerId + 0x1388;
}

BOOL isTrainerScrIdDoublePair2(u32 scriptId) {
    return scriptId >= 0x1388;
}

BOOL isDoubleBattle(u32 trainerId) {
    return TrainerData_GetParam(trainerId, 2) == 1;
}

u8 getBattleType(u16 trainerId) {
    return TrainerData_GetParam(trainerId, 2);
}

BOOL TrainerFlagGet(EventWork *eventWork, u16 trainerId) {
    return EventWork_FlagGet(eventWork, (trainerId + 0x58c));
}

void setTrainerBattleFlag(EventWork *eventWork, u16 trainerId) {
    EventWork_FlagSet(eventWork, (trainerId + 0x58c));
}

void clearTrainerBattleFlag(EventWork *eventWork, u16 trainerId) {
    EventWork_FlagReset(eventWork, (trainerId + 0x58c));
}
