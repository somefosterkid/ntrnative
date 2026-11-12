// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_mapchange.h, the lines in use
// so far.
#ifndef BWPORT_FIELD_EVENT_MAPCHANGE_H
#define BWPORT_FIELD_EVENT_MAPCHANGE_H

#include "types.h"
#include "struct_decls.h"
#include "nitro/fx/fx.h"

GameEvent *EventMapChangeDiveOut_Create(GameSystem *gsys);
GameEvent *EventMapChangeDiveIn_Create(GameSystem *gsys, u16 zoneId);
// Black 2 takes the unused zone ID as a u32
GameEvent *EventMapChange_CreateForFly(GameSystem *gsys, Field *field, u16 unused, ZoneSpawnInfo *spawn, u16 warpDir);

GameEvent *EventMapChange_CreateGridDefault(GameSystem *gsys, Field *field, u16 zoneId, VecFx32 *pos, u16 dir);

GameEvent *CreateGameEntryPointEvent(GameSystem *gsys, GameSystemProcData *procData);
GameEvent *EventMapChangeBlackout_Create(GameSystem *gsys);

void FieldMapControl_DeleteAllActors(GameSystem *gsys);
u16 ConvWarpDirToAngle(s32 warpDir);
void SetupZoneChangeSpawn(EventData *eventData, ZoneSpawnInfo *next, ZoneSpawnInfo *spawn);
void CallSpawnAllZoneNPCs(GameData *gameData, const ZoneSpawnInfo *spawn);
void Field_SetPlayerHidden(Field *field, BOOL hidden);
void GameData_UpdateFlashStatus(GameData *gameData, u16 zoneId);
void GameData_AdjustPlayerStateOnDiveOut(GameData *gameData);
// pokebw2's AdjustEscapeRopeSpawn
void func_ov020_02185f18(GameData *gameData, ZoneSpawnInfo *spawn);
// pokebw2's GameData_UpdatePartyForTimeOfDay
void func_ov020_0218601c(GameData *gameData);
// pokebw2's CityState_IsSet
BOOL func_ov020_0218625c(CityState *state);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
GameEvent *EventGameOpening_Create(GameSystem *gsys, GameSystemProcData *procData);
GameEvent *EventFieldFirst_Create(GameSystem *gsys, GameSystemProcData *procData);
GameEvent *EventFieldContinue_Create(GameSystem *gsys, GameSystemProcData *procData);
GameEvent *EventMapChangeWarp_CreateGrid(GameSystem *gsys, Field *field, u16 zoneId, const VecFx32 *pos, u16 dir,
                                         BOOL unk40);
GameEvent *EventMapChangeQuicksand_Create(GameSystem *gsys, Field *field, const VecFx32 *effectPos, u16 zoneId,
                                          VecFx32 *pos);
GameEvent *EventMapChangeDig_Create(GameSystem *gsys);
GameEvent *EventMapChangeTeleport_Create(GameSystem *gsys);
GameEvent *EventUnionRoomWarp_Create(GameSystem *gsys, Field *field);
GameEvent *EventEntralinkWarpIn_Create(GameSystem *gsys, u16 zoneId, const VecFx32 *pos, u32 a3);
GameEvent *func_ov018_0217cdbc(GameSystem *gsys);
GameEvent *func_ov018_0217ce08(GameSystem *gsys, Field *field, u16 zoneId, u32 param);
GameEvent *func_ov018_0217fa24(GameSystem *gsys);
GameEvent *EventMapChange_CreateGrid(GameSystem *gsys, Field *field, u8 mode, u16 zoneId, VecFx32 *pos, u16 dir);
void FieldMapControl_LoadBlackoutZone(GameSystem *gsys);
GameEvent *EventMapChangeWarpPad_Create(GameSystem *gsys, Field *field, u16 zoneId, const VecFx32 *pos, u16 dir);
GameEvent *EventMapChangeFakeWarp_Create(GameSystem *gsys, Field *field, u16 zoneId, const VecFx32 *pos, u16 dir);
GameEvent *EventEntralinkWarpIn_CreateCore(GameSystem *gsys, Field *field, ZoneSpawnInfo *spawn, u32 a3, u32 a4);
GameEvent *EventEntralinkWarp_Create(GameSystem *gsys, Field *field, ZoneSpawnInfo *spawn);
GameEvent *EventMapChangeEnding_Create(GameSystem *gsys, u16 zoneId, const VecFx32 *pos);
void FieldMapControl_LoadZone(GameSystem *gsys, u16 zoneId);
void FieldMapControl_InitSpawn(GameSystem *gsys, ZoneSpawnInfo *next);
void GameData_DeleteAllActors(GameData *gameData, const ZoneSpawnInfo *spawn);
void GameData_SetGimmickByZone(GameData *gameData, s32 zoneId);
// pokebw2's GameData_UpdateJoinAvenueForZone
void func_ov020_021859f0(GameData *gameData);
// pokebw2's GameData_UpdateZoneChangeFlag
void func_ov020_021859fc(GameData *gameData, const ZoneSpawnInfo *spawn);
// pokebw2's GameData_UpdateEscapeRopeZone
void func_ov020_02185eb0(GameData *gameData, const ZoneSpawnInfo *spawn);

// Overlay 21: the place name window of a zone change
void func_ov021_021bc550(PlaceName *placeName, s32 zoneId);
GameEvent *func_ov021_021bfdd0(GameEvent *event, GameSystem *gsys, Field *field, BOOL flag);
GameEvent *func_ov021_021bf61c(GameEvent *event, GameSystem *gsys, Field *field);
GameEvent *func_ov021_021bfe00(GameEvent *event, GameSystem *gsys, Field *field);
GameEvent *func_ov021_021bf5e8(GameEvent *event, GameSystem *gsys, Field *field, BOOL seasonChanged, int prevSeason, int season);
GameEvent *func_ov021_021bf66c(GameEvent *event, GameSystem *gsys, Field *field);
GameEvent *func_ov021_021bfd70(GameEvent *event, GameSystem *gsys, Field *field, BOOL seasonChanged);
GameEvent *func_ov021_021bf5b4(GameEvent *event, GameSystem *gsys, Field *field, BOOL seasonChanged, int prevSeason, int season);
void func_ov020_0218609c(CityState *city);
void func_ov020_021860b8(CityState *city);
GameEvent *func_ov020_02185360(GameSystem *gsys, Field *field, ZoneWarp *warp, u32 dir);
GameEvent *func_ov020_02185410(GameSystem *gsys, Field *field, ZoneWarp *warp, u32 dir);
GameEvent *func_ov020_021850fc(GameSystem *gsys, Field *field);
GameEvent *func_ov020_02185214(GameSystem *gsys);

#endif // BWPORT_FIELD_EVENT_MAPCHANGE_H
