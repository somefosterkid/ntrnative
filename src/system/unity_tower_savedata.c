// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/unity_tower_savedata.c, ported by tools/port_file.py.
#include "types.h"
#include "field/unity_tower.h"
#include "system/country_region.h"
#include "nitro/mi.h"
#include "save/player_info.h"
#include "save/save_control.h"

// The Unity Tower's survey: the trainers met through trades and the GTS, and the countries and regions they came
// from, which open the tower's floors. The ROM has no string for the file, so the name is a guess (swan's
// getUnityTower_SurveySaveBlkAddrress). Block 0x1d.

#define SAVE_BLOCK_UNITY_TOWER_SURVEY 0x1d


u32 func_02009454(void) {
    return sizeof(UnityTowerSurveySave);
}

void func_0200945c(UnityTowerSurveySave *save) {
    MIi_CpuClear32(0, save, sizeof(UnityTowerSurveySave));
}

UnityTowerSurveySave *getUnityTower_SurveySaveBlkAddrress(SaveControl *save) {
    return SaveControl_GetBlockPtr(save, SAVE_BLOCK_UNITY_TOWER_SURVEY);
}

// Makes the player's own country and region, recorded as UNITY_TOWER_REGION_OWN
void func_0200947c(UnityTowerSurveySave *save, PlayerInfo *info, int country, int region) {
    func_02008574(info, country, region);
    func_020094d8(save, country, region, UNITY_TOWER_REGION_OWN);
}

u32 func_0200949c(UnityTowerSurveySave *save, int country, int region) {
    s32 r = region;
    if (country == COUNTRY_NONE) {
        return UNITY_TOWER_REGION_NONE;
    }
    return (save->regions[r / 4 + (country - 1) * 16] >> ((r % 4) * 2)) & 3;
}

void func_020094d8(UnityTowerSurveySave *save, int country, int region, u32 state) {
    s32 r = region;
    u8 *byte;
    u32 shift;
    u8 clear;
    u8 set;

    if (country == COUNTRY_NONE) {
        return;
    }
    byte = &save->regions[r / 4 + (country - 1) * 16];
    shift = (r % 4) * 2;
    clear = (3 << shift) ^ 0xff;
    set = state << shift;
    *byte &= clear;
    *byte |= set;
    if (country != COUNTRY_JAPAN) {
    }
}

u8 func_02009524(UnityTowerSurveySave *save) {
    return save->foreignRegionMet;
}

// A day has passed: every region recorded today becomes one recorded before
void func_0200952c(UnityTowerSurveySave *save) {
    s32 i;
    s32 shift;

    for (i = 0; i < UNITY_TOWER_REGION_BYTES; i++) {
        u8 byte = save->regions[i];
        for (shift = 0; shift < 8; shift += 2) {
            if (((byte >> shift) & 3) == UNITY_TOWER_REGION_TODAY) {
                byte &= (u8)((3 << shift) ^ 0xff);
                byte |= (u8)(UNITY_TOWER_REGION_BEFORE << shift);
            }
        }
        save->regions[i] = byte;
    }
}

u8 func_02009584(UnityTowerSurveySave *save) {
    return save->countryCount;
}

u8 func_02009590(UnityTowerSurveySave *save, PlayerInfo *info, BOOL withOwnCountry) {
    u8 count = func_02009584(save);

    if (!withOwnCountry) {
        u8 country = UnityTowerVisitor_GetCountry(info);
        if (country == COUNTRY_NONE) {
            count = 0;
        } else if (func_02009794(save, country) && count != 0) {
            count--;
        }
    }
    return count;
}

u8 func_020095c8(UnityTowerSurveySave *save) {
    s32 i;

    for (i = 0; i < UNITY_TOWER_VISITOR_MAX; i++) {
        if (!save->visitors[i].valid) {
            break;
        }
    }
    return i;
}

void func_020095e8(UnityTowerSurveySave *save) {
    if (save->countryCount < COUNTRY_CODE_COUNT) {
        save->countryCount++;
    }
}

void func_020095fc(UnityTowerSurveySave *save, s32 value) {
    if (value < 5) {
        save->unk347 = value;
    }
}

u8 func_0200960c(UnityTowerSurveySave *save) {
    return save->unk347;
}

void setPlayerSurveys(UnityTowerSurveySave *save, s32 hobby) {
    if (hobby < 9) {
        save->hobby = hobby;
    }
}

u8 getPlayerSurveys(UnityTowerSurveySave *save) {
    return save->hobby;
}

u32 UnityTower_GetVisitorParam(UnityTowerSurveySave *save, u32 index, u32 param) {
    UnityTowerVisitor *visitor;

    if (index >= UNITY_TOWER_VISITOR_MAX) {
        return 0;
    }
    visitor = &save->visitors[index];
    switch (param) {
    case UNITY_TOWER_VISITOR_PARAM_SENT_SPECIES:
        return visitor->sentSpecies;
    case UNITY_TOWER_VISITOR_PARAM_RECEIVED_SPECIES:
        return visitor->receivedSpecies;
    case UNITY_TOWER_VISITOR_PARAM_HOBBY:
        return visitor->hobby;
    case UNITY_TOWER_VISITOR_PARAM_COUNTRY_COUNT:
        return visitor->countryCount;
    case UNITY_TOWER_VISITOR_PARAM_UNK26_0:
        return visitor->unk26_0;
    case UNITY_TOWER_VISITOR_PARAM_SCRIPT_FLAG:
        return visitor->scriptFlag;
    case UNITY_TOWER_VISITOR_PARAM_VALID:
        return visitor->valid;
    }
    return 0;
}

void func_02009698(UnityTowerSurveySave *save, int index, u32 param, u32 value) {
    UnityTowerVisitor *visitor;

    if (index >= UNITY_TOWER_VISITOR_MAX) {
        return;
    }
    visitor = &save->visitors[index];
    switch (param) {
    case UNITY_TOWER_VISITOR_PARAM_SENT_SPECIES:
    case UNITY_TOWER_VISITOR_PARAM_RECEIVED_SPECIES:
    case UNITY_TOWER_VISITOR_PARAM_HOBBY:
    case UNITY_TOWER_VISITOR_PARAM_COUNTRY_COUNT:
        break;
    case UNITY_TOWER_VISITOR_PARAM_UNK26_0:
        if (value < 5) {
            visitor->unk26_0 = value;
        }
        break;
    case UNITY_TOWER_VISITOR_PARAM_SCRIPT_FLAG:
        if (value != 0) {
            visitor->scriptFlag = 1;
        } else {
            visitor->scriptFlag = 0;
        }
        break;
    case UNITY_TOWER_VISITOR_PARAM_VALID:
        if (value != 0) {
            visitor->valid = 1;
        } else {
            visitor->valid = 0;
        }
        break;
    }
}

void *UnityTower_GetVisitor(UnityTowerSurveySave *save, s32 index) {
    if (index >= UNITY_TOWER_VISITOR_MAX) {
        return NULL;
    }
    return &save->visitors[index];
}

UnityTowerVisitor *func_0200974c(UnityTowerSurveySave *save) {
    return save->visitors;
}

BOOL func_02009750(UnityTowerSurveySave *save, u32 country) {
    BOOL isNew = FALSE;

    if (country == COUNTRY_NONE) {
        return FALSE;
    }
    if (country < COUNTRY_CODE_COUNT) {
        if (!func_02009794(save, country)) {
            isNew = TRUE;
        }
        save->countriesMet[country >> 3] |= (u8)(1 << (u8)(country & 7));
    }
    return isNew;
}

BOOL func_02009794(UnityTowerSurveySave *save, u32 country) {
    if (country < COUNTRY_CODE_COUNT) {
        if ((u8)((save->countriesMet[country >> 3] >> (u8)(country & 7)) & 1)) {
            return TRUE;
        }
    }
    return FALSE;
}
