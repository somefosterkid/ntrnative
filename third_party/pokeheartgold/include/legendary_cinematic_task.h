#ifndef POKEHEARTGOLD_LEGENDARY_CINEMATIC_TASK_H
#define POKEHEARTGOLD_LEGENDARY_CINEMATIC_TASK_H

#include "global.h"

#include "field/model_attributes.h"

// Environment of the legendary cinematic launch task (sub_02097BE0 creates it, sub_02097C50 runs it). DS 0x54.
typedef struct LegendaryCinematicTask {
    ModelAttributes unk_00;    // 0x00
    int unk_48;                // 0x48
    int unk_4c;                // 0x4C
    int state;                 // 0x50
} LegendaryCinematicTask;

#endif // POKEHEARTGOLD_LEGENDARY_CINEMATIC_TASK_H
