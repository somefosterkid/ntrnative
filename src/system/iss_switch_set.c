// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/iss_switch_set.c, ported by tools/port_file.py.
#include "types.h"
#include "constants/arc.h"
#include "gfl/arc.h"
#include "gfl/heap.h"
#include "gfl/sound.h"
#include "nitro/math.h"
#include "system/iss_switch.h"
#include "system/iss_switch_set.h"

// A BGM's switch set in the interactive sound system: the switches of the BGM's tracks, the zones the set plays in and
// the master volume, which fades. Names and layout from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0),
// except ISSSwitchSet_ResetState and ISSSwitchSet_ApplyVolume. swan names ISSSwitchSys_GetZoneIDCount and
// ISSSwitchSys_GetZoneID after the switch system, though they read the set

#define ISS_SWITCH_SET_ZONE_MAX 9

struct ISSSwitchSet {
    // The BGM's id, the first word of the set
    u32 bgmId;
    // The zones the set plays in
    u16 zoneIds[ISS_SWITCH_SET_ZONE_MAX];
    u8 zoneIdCount;
    ISSSwitch *switches[ISS_SWITCH_COUNT];
};

static void ISSSwitchSet_LoadArcDataCore(ISSSwitchSet *set, u32 datId);
static void ISSSwitchSet_UpdateCore(ISSSwitchSet *set);
static void ISSSwitchSet_ReqSwitchFadeInCore(ISSSwitchSet *set, ISSSwitchIndex switchIndex);
static void ISSSwitchSet_ReqSwitchFadeOutCore(ISSSwitchSet *set, ISSSwitchIndex switchIndex);
static u32 ISSSwitchSet_GetBGMIdCore(ISSSwitchSet *set);
static u8 ISSSwitchSys_GetZoneIDCount(ISSSwitchSet *set);
static u16 ISSSwitchSys_GetZoneID(ISSSwitchSet *set, int index);
static BOOL ISSSwitchSet_CheckHasZoneCore(ISSSwitchSet *set, u16 zoneId);
static BOOL ISSSwitchSet_IsSwitchOnCore(ISSSwitchSet *set, ISSSwitchIndex switchIndex);
static ISSSwitch *ISSSwitchSet_GetSwitch(ISSSwitchSet *set, ISSSwitchIndex switchIndex);
static void ISSSwitchSet_UpdateFadeIn(ISSSwitchSet *set);
static void ISSSwitchSet_UpdateFadeOut(ISSSwitchSet *set);
static void ISSSwitchSet_AdvanceFade(ISSSwitchSet *set);
static BOOL ISSSwitchSet_IsFadeOver(ISSSwitchSet *set);

ISSSwitchSet *func_0202c384(HeapID heapId);
void func_0202c360(ISSSwitchSet *set);
void func_0202c3a8(ISSSwitchSet *set, HeapID heapId);
ISSSwitchSet *ISSSwitchSet_Create(HeapID heapId) {
    ISSSwitchSet *set = func_0202c384(heapId);

    func_0202c360(set);
    func_0202c3a8(set, heapId);
    return set;
}

static void func_0202c3c4(ISSSwitchSet *set);
void func_0202c3a0(void *ptr);
void ISSSwitchSet_Free(ISSSwitchSet *set) {
    func_0202c3c4(set);
    func_0202c3a0(set);
}

ISSSwitch *func_0202c510(ISSSwitchSet *set, int switchIndex);
void func_0202c3e0(ISSSwitchSet *set, u32 datId);
void func_0202c318(ISSSwitchSet *set, u32 datId, HeapID heapId) {
    func_0202c3e0(set, datId);
}


void func_0202c470(ISSSwitchSet *set);
void ISSSwitchSet_Update(ISSSwitchSet *set) {
    func_0202c470(set);
}

void func_0202c48c(ISSSwitchSet *set, ISSSwitchIndex switchIndex);
void func_0202c328(ISSSwitchSet *set, ISSSwitchIndex switchIndex) {
    func_0202c48c(set, switchIndex);
}

void func_0202c498(ISSSwitchSet *set, ISSSwitchIndex switchIndex);
void func_0202c330(ISSSwitchSet *set, ISSSwitchIndex switchIndex) {
    func_0202c498(set, switchIndex);
}

void func_0202c4a4(ISSSwitchSet *set);
void func_0202c338(ISSSwitchSet *set) {
    func_0202c4a4(set);
}





void ISSSwitchSet_ResetState(ISSSwitchSet *set);

u32 func_0202c4c8(ISSSwitchSet *set);
u32 ISSSwitchSet_GetBGMId(ISSSwitchSet *set) {
    return func_0202c4c8(set);
}

BOOL ISSSwitchSet_CheckHasZone(ISSSwitchSet *set, u16 zoneId) {
    return ISSSwitchSet_CheckHasZoneCore(set, zoneId);
}

BOOL func_0202c504(ISSSwitchSet *set, ISSSwitchIndex switchIndex);
BOOL ISSSwitchSet_IsSwitchOn(ISSSwitchSet *set, ISSSwitchIndex switchIndex) {
    return func_0202c504(set, switchIndex);
}

void func_0202c518(ISSSwitchSet *set);
void func_0202c358(ISSSwitchSet *set) {
    func_0202c518(set);
}

void func_0202c360(ISSSwitchSet *set) {
    int i;

    set->bgmId = 0;
    set->zoneIdCount = 0;
    for (i = 0; i < ISS_SWITCH_SET_ZONE_MAX; i++) {
        set->zoneIds[i] = 0;
    }
    for (i = 0; i < ISS_SWITCH_COUNT; i++) {
        set->switches[i] = NULL;
    }
}


ISSSwitchSet *func_0202c384(HeapID heapId) {
    return GFL_HeapAllocate(heapId, sizeof(ISSSwitchSet), FALSE, "iss_switch_set.c", 298);
}

void func_0202c3a0(void *ptr) {
    GFL_HeapFree(ptr);
}

ISSSwitch *ISSSwitch_Create(HeapID heapId);
void func_0202c3a8(ISSSwitchSet *set, HeapID heapId) {
    int i;

    for (i = 0; i < ISS_SWITCH_COUNT; i++) {
        set->switches[i] = ISSSwitch_Create(heapId);
    }
}

static void func_0202c3c4(ISSSwitchSet *set) {
    int i;

    for (i = 0; i < ISS_SWITCH_COUNT; i++) {
        ISSSwitch_Free(set->switches[i]);
        set->switches[i] = NULL;
    }
}

void func_0202c3e0(ISSSwitchSet *set, u32 datId) {
    int i;
    u32 offset = 0;
    u16 fadeFrames;
    u16 trackMasks[ISS_SWITCH_COUNT];

    GFL_ArcSysReadRange(set, ARCID_ISS_SWITCH, datId, offset, 4);
    offset += 4;
    for (i = 0; i < ISS_SWITCH_COUNT; i++, offset += 2) {
        GFL_ArcSysReadRange(&trackMasks[i], ARCID_ISS_SWITCH, datId, offset, 2);
    }
    GFL_ArcSysReadRange(&fadeFrames, ARCID_ISS_SWITCH, datId, offset, 2);
    offset += 2;
    GFL_ArcSysReadRange(&set->zoneIdCount, ARCID_ISS_SWITCH, datId, offset, 1);
    offset += 1;
    GFL_ArcSysReadRange(set->zoneIds, ARCID_ISS_SWITCH, datId, offset, set->zoneIdCount * 2);
    for (i = 0; i < ISS_SWITCH_COUNT; i++) {
        ISSSwitch_Set(func_0202c510(set, i), fadeFrames, trackMasks[i]);
    }
}



ISSSwitch *func_0202c510(ISSSwitchSet *set, int switchIndex);
void ISSSwitch_Update(ISSSwitch *sw);
void func_0202c470(ISSSwitchSet *set) {
    int i;

    for (i = 0; i < ISS_SWITCH_COUNT; i++) {
        ISSSwitch_Update(func_0202c510(set, i));
    }
}

void func_0202c48c(ISSSwitchSet *set, ISSSwitchIndex switchIndex) {
    ISSSwitch_ReqFadeIn(func_0202c510(set, switchIndex));
}

void func_0202c498(ISSSwitchSet *set, ISSSwitchIndex switchIndex) {
    ISSSwitch_ReqFadeOut(func_0202c510(set, switchIndex));
}

void ISSSwitch_Unmute(ISSSwitch *sw);
void ISSSwitch_Mute(ISSSwitch *sw);
void func_0202c4a4(ISSSwitchSet *set) {
    int i;

    ISSSwitch_Unmute(func_0202c510(set, 0));
    for (i = 1; i < ISS_SWITCH_COUNT; i++) {
        ISSSwitch_Mute(func_0202c510(set, i));
    }
}

u32 func_0202c4c8(ISSSwitchSet *set) {
    return set->bgmId;
}









void ISSSwitchSet_ApplyVolume(ISSSwitchSet *set);

static inline u32 ISSSwitchSet_GetBGMIdCore(ISSSwitchSet *set) {
    return set->bgmId;
}

typedef struct {
    u32 pad_0;
    u32 pad_4;
    u16 zoneIds[7];
    u8 zoneIdCount;
} ISSSwitchSetZone;
static u8 ISSSwitchSys_GetZoneIDCount(ISSSwitchSet *set) {
    return ((ISSSwitchSetZone *)set)->zoneIdCount;
}

typedef struct {
    u32 unk_0;
    u16 zoneIds[ISS_SWITCH_SET_ZONE_MAX];
} ISSSwitchSetZoneData;
static u16 ISSSwitchSys_GetZoneID(ISSSwitchSet *set, int index) {
    return ((ISSSwitchSetZoneData *)set)->zoneIds[index];
}

static BOOL ISSSwitchSet_CheckHasZoneCore(ISSSwitchSet *set, u16 zoneId) {
    int count = ISSSwitchSys_GetZoneIDCount(set);
    int i;

    for (i = 0; i < count; i++) {
        if (zoneId == ISSSwitchSys_GetZoneID(set, i)) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL ISSSwitch_IsOn(ISSSwitch *sw);
BOOL func_0202c504(ISSSwitchSet *set, ISSSwitchIndex switchIndex) {
    return ISSSwitch_IsOn(func_0202c510(set, switchIndex));
}

ISSSwitch *func_0202c510(ISSSwitchSet *set, int switchIndex) {
    return set->switches[switchIndex];
}

void ISSSwitch_CommitVolume(ISSSwitch *sw, int volume);
void func_0202c518(ISSSwitchSet *set) {
    int i;
    for (i = 0; i < ISS_SWITCH_COUNT; i++) {
        ISSSwitch_CommitVolume(func_0202c510(set, i), SND_VOLUME_MAX);
    }
}




static inline BOOL ISSSwitchSet_IsSwitchOnCore(ISSSwitchSet *set, ISSSwitchIndex switchIndex) {
    return ISSSwitch_IsOn(ISSSwitchSet_GetSwitch(set, switchIndex));
}

static inline ISSSwitch *ISSSwitchSet_GetSwitch(ISSSwitchSet *set, ISSSwitchIndex switchIndex) {
    return set->switches[switchIndex];
}


