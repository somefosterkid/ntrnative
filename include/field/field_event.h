// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/field_event.h, the lines in use so
// far.
#ifndef BWPORT_FIELD_FIELD_EVENT_H
#define BWPORT_FIELD_FIELD_EVENT_H

#include "types.h"
#include "struct_decls.h"
#include "nitro/fx/fx.h"
#include "field/zone.h"
#include "field/event_season_banner.h"

GameEvent *CallFieldMapEntranceInTransition(GameSystem *gsys, Field *field, u32 type, u32 a3, u32 a4, u8 startSeason,
                                            u8 endSeason);
GameEvent *CallFieldMapEntranceOutTransitionDefault(GameSystem *gsys, Field *field, u32 type, u32 a3);
// The season banner's event between two seasons, which calls param_5 with param_6 (pokebw2's
// EventGameTransition_CreateSeasonFadeInEventWithCallback)
GameEvent *func_ov021_021be9a0(GameSystem *param_1, Field *param_2, u8 param_3, u8 param_4,
                               EventSeasonBannerCallback param_5, void *param_6);
GameEvent *CreateFieldCloseEvent(GameSystem *gsys, Field *field);
GameEvent *EventFieldOpen_CreateHeadless(GameSystem *gsys);
// pokebw2's func_ov036_021b5168 and func_ov036_021b50f4
void func_ov021_021bc5bc(PlaceName *placeName);
void func_ov021_021bc578(PlaceName *placeName, u32 zoneId);
GameEvent *EventFieldEffect_Create(GameSystem *gsys, void *g3dCi, u8 effectId);
// pokebw2's func_ov036_021b8890
GameEvent *func_ov021_021bea30(GameSystem *gsys, Field *field, u32 a2, u32 a3);

struct BtlFieldStatus {
    u32 bgId;
    u32 terrain;
    u8 weather;
    u8 season;
    u16 zoneId;
    u8 hour;
    u8 minute;
};

void SaveBtlFieldStatus(BtlFieldStatus *status, GameData *gameData, Field *field);

GameEvent *EventFieldCloseKeepSound_Create(GameSystem *gsys, Field *field);
// Runs the proc as a field subprocess
GameEvent *EventFieldSubprocessCall_Create(GameSystem *gsys, Field *field, s32 overlayId, const GameProcFunctions *functions,
                                           void *param);
// The name entry for the nickname of the party's Pokémon in slot (the first if there is none), which writes 1 to
// *result if the name was given
GameEvent *prepareForPokemonNameEntryData(GameSystem *gsys, u16 *result, int slot, BOOL transition);

// Moves the player to another zone with a transition, such as through a door
struct WarpSequence {
    GameEvent *parent;
    GameSystem *gsys;
    GameData *gameData;
    Field *field;
    u32 unk10;
    u32 transitionType;
    u16 zoneId;
    ZoneSpawnInfo spawn;
    u32 outTransition;
    u32 inTransition;
    BOOL seasonChanged;
    u8 startSeason;
    u8 endSeason;
    u32 unk48;
    u32 unk4C;
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
GameEvent *EventFieldOpen_Create(GameSystem *gsys);
GameEvent *EventPlayerSpinDown_Create(GameEvent *event, GameSystem *gsys, Field *field);
GameEvent *EventQuicksandArrive_Create(GameEvent *event, GameSystem *gsys, Field *field);
GameEvent *EventQuicksandDrawIn_Create(GameEvent *event, GameSystem *gsys, Field *field, const VecFx32 *pos);
GameEvent *EventWarpSequence_CreateIn(WarpSequence *warp);
GameEvent *EventWarpSequence_CreateOut(WarpSequence *warp);
GameEvent *EventFieldSubprocessTransition_Create(GameSystem *gsys, Field *field, s32 overlayId,
                                                 const GameProcFunctions *functions, void *param);
// pokebw2's EventFieldSubprocessCall_CreateWithCallback
// Runs the proc as a field subprocess, then calls callback with work if there is a callback, and frees work
GameEvent *func_02014840(GameSystem *gsys, Field *field, s32 overlayId,
                                                       const GameProcFunctions *functions, void *param,
                                                       void (*callback)(void *work), void *work);
GameEvent *EventDig_Create(GameEvent *event, GameSystem *gsys, Field *field, BOOL seasonChanged);
GameEvent *func_ov021_021bf5b4(GameEvent *event, GameSystem *gsys, Field *field, BOOL seasonChanged, int prevSeason,
                               int season);
GameEvent *func_ov021_021bfe00(GameEvent *event, GameSystem *gsys, Field *field);

#endif // BWPORT_FIELD_FIELD_EVENT_H
