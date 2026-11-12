// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/field_actor_savedata.h: the lines in
// use so far.
#ifndef BWPORT_SAVE_FIELD_ACTOR_SAVEDATA_H
#define BWPORT_SAVE_FIELD_ACTOR_SAVEDATA_H

// Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"

#define FIELD_ACTOR_SAVE_COUNT 64

// Black 2's; twinned code confirms the offsets of flags, collisionWidth, collisionHeight, uid, moveCode, areaW, areaH,
// defaultDir, faceDir, motionDir, zoneId, modelId, evType, spawnFlag, scrId, param0, param1, param2, defaultGPosX,
// defaultGPosY, defaultGPosZ, gPosX, gPosY, gPosZ, wPosY, railPos and unk40 in Black
// What is kept of an actor, with the fields of FieldActor narrowed
typedef struct {
    u32 flags;
    u8 collisionWidth;
    u8 collisionHeight;
    u16 padding6;
    u8 uid;
    u8 moveCode;
    s8 areaW;
    s8 areaH;
    s8 defaultDir;
    s8 faceDir;
    s8 motionDir;
    u16 zoneId;
    u16 modelId;
    u16 evType;
    u16 spawnFlag;
    u16 scrId;
    u16 param0;
    u16 param1;
    u16 param2;
    s16 defaultGPosX;
    s16 defaultGPosY;
    s16 defaultGPosZ;
    s16 gPosX;
    s16 gPosY;
    s16 gPosZ;
    fx32 wPosY;
    // The actor's rail position and the word after it
    u8 railPos[0x10];
    u8 unk40[0x10];
} FieldActorSaveEntry;

struct FieldActorSave {
    FieldActorSaveEntry actors[FIELD_ACTOR_SAVE_COUNT];
};

u32 ActorSave_GetBlockSize(void);
void ActorSave_Save(MMSys *system, FieldActorSave *save);
void ActorSave_Load(MMSys *system, FieldActorSave *save);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void ActorSave_Init(FieldActorSave *save);

#endif // BWPORT_SAVE_FIELD_ACTOR_SAVEDATA_H
