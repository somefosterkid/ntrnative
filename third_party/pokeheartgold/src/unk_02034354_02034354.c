#include "global.h"

#include "heap.h"
#include "unk_02034354.h"
#include "unk_02034354_internal.h"

void sub_02034354(SaveData *saveData, const LinkBattleRuleset *regulation) {
    int netId;
    PlayerProfile *trainerInfo = Save_PlayerData_GetProfile(saveData);

    if (_021D4130) {
        return;
    }

    _021D4130 = Heap_Alloc(HEAP_ID_15, sizeof(CommunicationInformation));
    MI_CpuClear8(_021D4130, sizeof(CommunicationInformation));

    for (netId = 0; netId < 8; netId++) {
        _021D4130->trainerInfo[netId] = &_021D4130->playerInfo[netId].trainerInfoBuffer;
        sub_020346E8(netId);
    }

    _021D4130->dataFinishedReading = 0;
    _021D4130->dataRecvFlag = 0;
    _021D4130->curNetId = 0;
    _021D4130->saveData = saveData;
    _021D4130->regulation = regulation;

    PlayerProfile_Copy(trainerInfo, _021D4130->trainerInfo[0]);
}
