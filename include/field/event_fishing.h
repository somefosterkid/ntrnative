// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_fishing.h.
#ifndef BWPORT_FIELD_EVENT_FISHING_H
#define BWPORT_FIELD_EVENT_FISHING_H

#include "types.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "system/game_event.h"

struct FishingEventWork {
    GameSystem *gsys;
    GameData *gameData;
    Field *field;
    EncountSystem *encountSystem;
    GameRecords *records;
    MMSys *actorSystem;
    FieldPlayer *player;
    FieldActor *actor;
    s16 gridX;
    s16 gridY;
    s16 gridZ;
    u16 unk26;
    u32 playerExState;
    void *effect2C;
    void *effect30;
    VecFx32 playerPos;
    VecFx32 fishingPos;
    u8 unk4C[12];
    u8 faceDirection;
    u8 isPhenomenon;
    // A u8 in Black 2, followed by a flag for the rod's animation
    u16 noFishing;
    s32 timer;
    u32 elapsed;
    BtlSetup *battleSetup;
};

// Where the player would surf to, the water in front of them. Returns FALSE if there is none
BOOL CreateSurfPos(void *context, Field *field, VecFx32 *position);
GameEvent *EventFieldFishing_Create(Field *field, GameSystem *gsys);
GameEventReturnCode EventFieldFishing_Callback(GameEvent *event, u32 *state, void *data);

u32 func_ov021_021c5fa8(FishingEventWork *work, u32 value);
u32 func_ov021_021c5fc0(FishingEventWork *work, u32 value);
void func_ov021_021c5fec(FishingEventWork *work);
void func_ov021_021c6018(FishingEventWork *work);
void func_ov021_021c602c(FishingEventWork *work);
void func_ov021_021c6054(FishingEventWork *work);

#endif // BWPORT_FIELD_EVENT_FISHING_H
