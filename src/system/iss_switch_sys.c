// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/iss_switch_sys.c, ported by tools/port_file.py.
#include "types.h"
#include "constants/arc.h"
#include "gfl/arc.h"
#include "gfl/heap.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "system/iss_switch_set.h"
#include "system/iss_switch_sys.h"

// The interactive sound system's BGM switches. Each BGM with switches has a switch set, loaded from ARCID_ISS_SWITCH;
// the system updates the set of the BGM that is playing, and keeps requests to mute switches or fade the master volume
// until their BGM plays. Names and layouts from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0), except
// ISSSwitchSys_ResetSwitches, ISSSwitchSys_ResetMuteStateChangeRequests and
// ISSSwitchSys_ResetMasterVolumeChangeRequest

#define ISS_SWITCH_MUTE_REQUEST_MAX 4

typedef struct {
    BOOL enabled;
    BOOL isUnmute;
    u32 bgmId;
    ISSSwitchIndex switchIndex;
} ISSSwitchMuteStateChangeRequest;

typedef struct {
    BOOL enabled;
    u32 bgmId;
    int targetVolume;
    // The fade's length, in frames
    u16 interval;
} ISSSwitchMasterVolumeChangeRequest;

struct ISSSwitchSys {
    BOOL isEnabled;
    u8 setCount;
    ISSSwitchSet **switchSets;
    // The switch set of the BGM that is playing
    ISSSwitchSet *nowSwitchSet;
};

static void ISSSwitchSys_FreeCore(ISSSwitchSys *switchSys);
static void func_0202c148(ISSSwitchSys *switchSys, HeapID heapId);
void ISSSwitchSys_FreeSwitchSets(ISSSwitchSys *switchSys);
void ISSSwitchSys_UpdateSwitchSet(ISSSwitchSys *switchSys);
void ISSSwitchSys_EnableCore(ISSSwitchSys *switchSys);
void ISSSwitchSys_DisableCore(ISSSwitchSys *switchSys);
void ISSSwitchSys_ChangeZoneCore(ISSSwitchSys *switchSys, u16 zoneId);
u8 ISSSwitchSys_GetSwitchSetCount(ISSSwitchSys *switchSys);
ISSSwitchSet *ISSSwitchSys_GetSwitchSet(ISSSwitchSys *switchSys, u8 index);
static ISSSwitchSet *ISSSwitchSys_GetNowSwitchSet(ISSSwitchSys *switchSys);
BOOL ISSSwitchSys_IsZoneIDNoNeedSetup(ISSSwitchSys *switchSys, u16 zoneId);
ISSSwitchSet *ISSSwitchSys_GetSwitchSetForBGM(ISSSwitchSys *switchSys, u32 bgm);
BOOL ISSSwitchSys_ChangeSwitchSetForBGM(ISSSwitchSys *switchSys, u32 bgm);
static void ISSSwitchSys_CommitVolume(ISSSwitchSys *switchSys);
u32 ISSSwitchSys_GetBGMID(void);
BOOL ISSSwitchSys_IsEnabled(ISSSwitchSys *switchSys);
static void ISSSwitchSys_ResetSwitchSetState(ISSSwitchSys *switchSys);
static void ISSSwitch_ReqSwitchMuteStateChangeCore(ISSSwitchSys *switchSys, ISSSwitchIndex switchIndex, BOOL unmute,
                                                   u32 bgm);
static void ISSSwitchMuteStateChangeRequest_Set(ISSSwitchMuteStateChangeRequest *request, ISSSwitchIndex switchIndex,
                                                BOOL unmute, u32 bgm);
static void ISSSwitchSys_ProcessMuteStateChangeRequest(ISSSwitchMuteStateChangeRequest *request,
                                                       ISSSwitchSys *switchSys);
static BOOL ISSSwitchMuteStateChangeRequest_IsEnabled(ISSSwitchMuteStateChangeRequest *request);
static void ISSSwitchSys_ReqMasterVolumeChangeCore(ISSSwitchSys *switchSys, int volume, u16 frames, u32 bgm);
static void ISSSwitchMasterVolumeChangeRequest_Set(ISSSwitchMasterVolumeChangeRequest *request, int volume, u16 frames,
                                                   u32 bgm);
static void ISSSwitchSys_ProcessMasterVolumeChangeRequest(ISSSwitchMasterVolumeChangeRequest *request,
                                                          ISSSwitchSys *switchSys);
static BOOL ISSSwitchMasterVolumeChangeRequest_IsEnabled(ISSSwitchMasterVolumeChangeRequest *request);

void func_0202c118(ISSSwitchSys *switchSys);
void *func_0202c124(HeapID heapId);
void ISSSwitchSys_Free(ISSSwitchSys *switchSys);
void ISSSwitchSys_Update(ISSSwitchSys *switchSys);
void ISSSwitchSys_Enable(ISSSwitchSys *switchSys);
void ISSSwitchSys_Disable(ISSSwitchSys *switchSys);
void ISSSwitchSys_ChangeZone(ISSSwitchSys *switchSys, u16 zoneId);
void ISSSwitchSys_ReqSwitchFadeIn(ISSSwitchSys *switchSys, ISSSwitchIndex switchIndex);
void ISSSwitchSys_ReqSwitchFadeOut(ISSSwitchSys *switchSys, ISSSwitchIndex switchIndex);
void func_0202c110(ISSSwitchSys *switchSys);

ISSSwitchSys *ISSSwitchSys_Create(HeapID heapId) {
    ISSSwitchSys *switchSys = func_0202c124(heapId);

    func_0202c118(switchSys);
    func_0202c148(switchSys, heapId);
    return switchSys;
}

static void func_0202c140(ISSSwitchSys *switchSys);
void ISSSwitchSys_Free(ISSSwitchSys *switchSys) {
    ISSSwitchSys_FreeSwitchSets(switchSys);
    func_0202c140(switchSys);
}

void ISSSwitchSys_Update(ISSSwitchSys *switchSys) {
    if (ISSSwitchSys_IsEnabled(switchSys)) {
        ISSSwitchSys_UpdateSwitchSet(switchSys);
        ISSSwitchSys_CommitVolume(switchSys);
    }
}

void ISSSwitchSys_Enable(ISSSwitchSys *switchSys) {
    ISSSwitchSys_EnableCore(switchSys);
}

void ISSSwitchSys_Disable(ISSSwitchSys *switchSys) {
    ISSSwitchSys_DisableCore(switchSys);
}

void ISSSwitchSys_ChangeZone(ISSSwitchSys *switchSys, u16 zoneId) {
    ISSSwitchSys_ChangeZoneCore(switchSys, zoneId);
}

void func_0202c328(ISSSwitchSet *set, ISSSwitchIndex switchIndex);
void ISSSwitchSys_ReqSwitchFadeIn(ISSSwitchSys *switchSys, ISSSwitchIndex switchIndex) {
    ISSSwitchSet *set = ISSSwitchSys_GetNowSwitchSet(switchSys);
    if (set != NULL) {
        func_0202c328(set, switchIndex);
    }
}

void func_0202c330(ISSSwitchSet *set, ISSSwitchIndex switchIndex);
void ISSSwitchSys_ReqSwitchFadeOut(ISSSwitchSys *switchSys, ISSSwitchIndex switchIndex) {
    ISSSwitchSet *set = ISSSwitchSys_GetNowSwitchSet(switchSys);
    if (set != NULL) {
        func_0202c330(set, switchIndex);
    }
}








BOOL ISSSwitchSys_IsSwitchOn(ISSSwitchSys *switchSys, ISSSwitchIndex switchIndex) {
    ISSSwitchSet *set = ISSSwitchSys_GetNowSwitchSet(switchSys);

    if (set != NULL) {
        return ISSSwitchSet_IsSwitchOn(set, switchIndex);
    }
    return FALSE;
}

void func_0202c110(ISSSwitchSys *switchSys) {
    ISSSwitchSys_ResetSwitchSetState(switchSys);
}

void func_0202c118(ISSSwitchSys *switchSys) {
    switchSys->isEnabled = FALSE;
    switchSys->setCount = 0;
    switchSys->switchSets = NULL;
    switchSys->nowSwitchSet = NULL;
}

void *func_0202c124(HeapID heapId) {
    return GFL_HeapAllocate(heapId, sizeof(ISSSwitchSys), FALSE, "iss_switch_sys.c", 266);
}

static void func_0202c140(ISSSwitchSys *switchSys) {
    GFL_HeapFree(switchSys);
}

static void func_0202c148(ISSSwitchSys *switchSys, HeapID heapId) {
    int i;
    int count = GFL_ArcSysGetDataMax(ARCID_ISS_SWITCH);

    switchSys->setCount = count;
    switchSys->switchSets = GFL_HeapAllocate(heapId, count * sizeof(ISSSwitchSet *), FALSE, "iss_switch_sys.c", 314);
    for (i = 0; i < count; i++) {
        ISSSwitchSet *set = ISSSwitchSet_Create(heapId);

        func_0202c318(set, i, heapId);
        switchSys->switchSets[i] = set;
    }
}





void ISSSwitchSys_ResetSwitches(ISSSwitchSys *switchSys);

void ISSSwitchSys_ResetMuteStateChangeRequests(ISSSwitchSys *switchSys);

void ISSSwitchSys_ResetMasterVolumeChangeRequest(ISSSwitchSys *switchSys);

void ISSSwitchSys_FreeSwitchSets(ISSSwitchSys *switchSys) {
    int count = switchSys->setCount;
    int i;

    for (i = 0; i < count; i++) {
        ISSSwitchSet_Free(switchSys->switchSets[i]);
    }
    GFL_HeapFree(switchSys->switchSets);
    switchSys->setCount = 0;
    switchSys->switchSets = NULL;
    switchSys->nowSwitchSet = NULL;
}

void ISSSwitchSys_UpdateSwitchSet(ISSSwitchSys *switchSys) {
    ISSSwitchSet *set = ISSSwitchSys_GetNowSwitchSet(switchSys);

    if (set != NULL) {
        ISSSwitchSet_Update(set);
    }
}

void ISSSwitchSys_EnableCore(ISSSwitchSys *switchSys) {
    if (ISSSwitchSys_IsEnabled(switchSys) == TRUE) {
        return;
    }
    switchSys->isEnabled = TRUE;
    if (ISSSwitchSys_ChangeSwitchSetForBGM(switchSys, ISSSwitchSys_GetBGMID()) == TRUE) {
        ISSSwitchSys_ResetSwitchSetState(switchSys);
        ISSSwitchSys_CommitVolume(switchSys);
    }
}

void ISSSwitchSys_DisableCore(ISSSwitchSys *switchSys) {
    if (ISSSwitchSys_IsEnabled(switchSys)) {
        switchSys->isEnabled = FALSE;
    }
}

void ISSSwitchSys_ChangeZoneCore(ISSSwitchSys *switchSys, u16 zoneId) {
    if (ISSSwitchSys_IsEnabled(switchSys) && ISSSwitchSys_IsZoneIDNoNeedSetup(switchSys, zoneId) == FALSE) {
        ISSSwitchSys_ResetSwitchSetState(switchSys);
        ISSSwitchSys_DisableCore(switchSys);
    }
}

u8 ISSSwitchSys_GetSwitchSetCount(ISSSwitchSys *switchSys) {
    return switchSys->setCount;
}

ISSSwitchSet *ISSSwitchSys_GetSwitchSet(ISSSwitchSys *switchSys, u8 index) {
    return switchSys->switchSets[index];
}

static ISSSwitchSet *ISSSwitchSys_GetNowSwitchSet(ISSSwitchSys *switchSys) {
    return switchSys->nowSwitchSet;
}

// Whether the BGM's switch set also plays in the zone
BOOL ISSSwitchSys_IsZoneIDNoNeedSetup(ISSSwitchSys *switchSys, u16 zoneId) {
    ISSSwitchSet *set = ISSSwitchSys_GetNowSwitchSet(switchSys);

    if (set == NULL) {
        return FALSE;
    }
    return ISSSwitchSet_CheckHasZone(set, zoneId);
}

ISSSwitchSet *ISSSwitchSys_GetSwitchSetForBGM(ISSSwitchSys *switchSys, u32 bgm) {
    int count = ISSSwitchSys_GetSwitchSetCount(switchSys);
    int i;

    for (i = 0; i < count; i++) {
        ISSSwitchSet *set = ISSSwitchSys_GetSwitchSet(switchSys, i);

        if (bgm == ISSSwitchSet_GetBGMId(set)) {
            return set;
        }
    }
    return NULL;
}

// Returns whether the switch set changed
BOOL ISSSwitchSys_ChangeSwitchSetForBGM(ISSSwitchSys *switchSys, u32 bgm) {
    ISSSwitchSet *nowSet = ISSSwitchSys_GetNowSwitchSet(switchSys);
    ISSSwitchSet *set = ISSSwitchSys_GetSwitchSetForBGM(switchSys, bgm);

    if (nowSet == set) {
        return FALSE;
    }
    switchSys->nowSwitchSet = set;
    return TRUE;
}

void func_0202c358(ISSSwitchSet *set);
static void ISSSwitchSys_CommitVolume(ISSSwitchSys *switchSys) {
    ISSSwitchSet *set = ISSSwitchSys_GetNowSwitchSet(switchSys);

    if (set != NULL) {
        func_0202c358(set);
    }
}


u32 ISSSwitchSys_GetBGMID(void) {
    return GFL_SndBGMGetID();
}

BOOL ISSSwitchSys_IsEnabled(ISSSwitchSys *switchSys) {
    return switchSys->isEnabled;
}

void func_0202c338(ISSSwitchSet *set);
static void ISSSwitchSys_ResetSwitchSetState(ISSSwitchSys *switchSys) {
    ISSSwitchSet *set = ISSSwitchSys_GetNowSwitchSet(switchSys);

    if (set != NULL) {
        func_0202c338(set);
    }
}



