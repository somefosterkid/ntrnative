#ifndef POKEHEARTGOLD_APPROACHING_TRAINER_DATA_H
#define POKEHEARTGOLD_APPROACHING_TRAINER_DATA_H

#include "field_types_def.h"

// ApproachingTrainerData: merged by tools/hg/promote.py from 3 m2c local copies (1 variants, base
// src/unk_02062108_02062260_0206229C_0206229C.c) and 4 accesses; fields at every accessed offset, pads elsewhere.

struct FieldSystem;
struct OverworldAnimManager;
struct PlayerAvatar;

typedef struct ApproachingTrainerData {
    int state;
    BOOL done;
    int direction;
    int sightRange;
    int unused;
    int approachType;
    int approachNum;
    int delay;
    struct OverworldAnimManager *animMan;
    LocalMapObject *mapObj;
    struct PlayerAvatar *playerAvatar;
    struct FieldSystem *fieldSystem;
} ApproachingTrainerData;

// sizeof(ApproachingTrainerData) == 0x30

#endif // POKEHEARTGOLD_APPROACHING_TRAINER_DATA_H
