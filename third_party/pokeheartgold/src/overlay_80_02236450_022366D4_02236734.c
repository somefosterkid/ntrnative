#include "global.h"

#include "constants/heap.h"

#include "filesystem_files_def.h"
#include "msgdata.h"
#include "pm_version.h"
#include "pokemon.h"
#include "scrcmd_9.h"
#include "unk_0204A3F4.h"

typedef enum HeapID HeapID;

u32 ov80_02236AF0(u8);

extern const u16 ov80_0223C048[];

void ov80_02229EF4(void *param_1, s32 param_2, NarcId param_3);

u32 ov80_02236734(u8 *battleTower, FrontierMon *dto, u16 setID, u32 otID, u32 givenPersonality, u8 ivs, u8 partyIndex, int giveReservedItem, u32 heapID);

u32 ov80_02236734(u8 *battleTower, FrontierMon *dto, u16 setID, u32 otID, u32 givenPersonality, u8 ivs, u8 partyIndex, int giveReservedItem, u32 heapID) {
    u16 monData[8];
    u32 friendship;
    u32 personality;
    u32 value;
    int i;
    int evs;
    int count;
    int ability;

    MI_CpuFill8(dto, 0, sizeof(FrontierMon));
    ov80_02229EF4(monData, setID, (NarcId)(ov80_02236AF0(battleTower[0xf])));

    dto->species = monData[0];
    dto->form = monData[7];
    if (giveReservedItem != 0) {
        dto->item = ov80_0223C048[partyIndex];
    } else {
        dto->item = monData[6];
    }

    friendship = 0xff;
    for (i = 0; i < 4; i++) {
        dto->moves[i] = monData[1 + i];
        if (monData[1 + i] == 0xda) {
            friendship = 0;
        }
    }

    dto->otId = otID;

    if (givenPersonality == 0) {
        do {
            u32 lo = FrontierFieldSystem_0204B510((FrontierFieldSystem *)(battleTower));
            u32 hi = FrontierFieldSystem_0204B510((FrontierFieldSystem *)(battleTower));
            personality = lo | (hi << 16);
        } while (((u8 *)monData)[0xb] != GetNatureFromPersonality(personality) || CalcShininessByOtIdAndPersonality(otID, personality) == 1);
        dto->pid = personality;
    } else {
        personality = givenPersonality;
        dto->pid = personality;
    }

    dto->hpIv = ivs;
    dto->atkIv = ivs;
    dto->defIv = ivs;
    dto->spdIv = ivs;
    dto->spAtkIv = ivs;
    dto->spDefIv = ivs;

    count = 0;
    for (i = 0; i < 6; i++) {
        value = MaskOfFlagNo(i);
        if (value & ((u8 *)monData)[0xa]) {
            count++;
        }
    }
    evs = 0x1fe / count;
    if (evs > 0xff) {
        evs = 0xff;
    }
    for (i = 0; i < 6; i++) {
        value = MaskOfFlagNo(i);
        if (value & ((u8 *)monData)[0xa]) {
            dto->evs[i] = evs;
        }
    }

    dto->ppUp = 0;
    dto->language = gGameLanguage;

    ability = GetMonBaseStat(dto->species, 0x19);
    if (ability != 0) {
        if (dto->pid & 1) {
            dto->ability = ability;
        } else {
            dto->ability = GetMonBaseStat(dto->species, 0x18);
        }
    } else {
        dto->ability = GetMonBaseStat(dto->species, 0x18);
    }

    dto->friendship = friendship;
    GetSpeciesNameIntoArray(dto->species, (HeapID)(heapID), dto->nickname);
    return personality;
}
