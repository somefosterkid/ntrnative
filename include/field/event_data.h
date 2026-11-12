// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_data.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_EVENT_DATA_H
#define BWPORT_FIELD_EVENT_DATA_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "field/encounter.h"
#include "gfl/heap.h"

void EventData_Clear(EventData *data);
// The quicksand trigger at the position, facing direction
ZoneTrigger *FindQuicksandTrigger(EventData *data, EventWork *eventWork, const VecFx32 *position, u32 direction);
void LoadZoneEntities(EventData *data, u16 zoneId, u8 season);
void *GetZoneInitScrPointer(EventData *data);

void EventData_Free(EventData *data);
void EventData_Reset(EventData *data);

EncData *GetEncountData(EventData *data);

// Black 2's; twinned code confirms the offsets of entityArc, encArc, otherArc, entityCount, npcCount, warpCount,
// triggerCount, entities, npcs, warps, triggers, prevEntityCount, prevNpcCount, prevWarpCount, prevTriggerCount,
// encLoaded, encData, initScript, initScriptOffset and cache in Black
struct EventData {
    u8 unk0[4];
    ArcTool *entityArc;
    ArcTool *encArc;
    ArcTool *otherArc;
    u16 zoneId;
    u16 entityCount;
    u16 npcCount;
    u16 warpCount;
    u16 triggerCount;
    u8 pad1A[2];
    ZoneBGEntity *entities;
    ZoneNPC *npcs;
    ZoneWarp *warps;
    ZoneTrigger *triggers;
    u16 prevEntityCount;
    u16 prevNpcCount;
    u16 prevWarpCount;
    u16 prevTriggerCount;
    u32 encLoaded;
    EncData encData;
    void *initScript;
    // The zone's entity file, read from here: the offset of its init script from the counts, then the counts of the
    // entities, NPCs, warps and triggers and the entities themselves
    u32 initScriptOffset;
    u8 cache[0x880];
    u8 rest[0x100];
};

void EventData_LoadZone(EventData *data, u16 zoneId, u8 season);
void EventData_LoadEntities(EventData *data, u16 zoneId, u8 season);
void EventData_LoadEncData(EventData *data, u16 zoneId, u8 season);
u32 IsEncountDataLoaded(EventData *data);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
EventData *EventData_Create(HeapID heapId);
u16 GetWarpAtPosition(EventData *data);
s32 GetWarpIDByPlayerPos(EventData *data, const VecFx32 *position);
s32 GetWarpIDByPlayerPosRail(EventData *data, const RailPosition *position);

#endif // BWPORT_FIELD_EVENT_DATA_H
