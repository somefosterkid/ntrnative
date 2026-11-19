#ifndef POKEHEARTGOLD_UNK_020551B8_H
#define POKEHEARTGOLD_UNK_020551B8_H

#include "global.h"
#include "task.h"

// the environment of the encounter-effect task (sub_020551B8): state, the cut-in effect's done flag, effect id, battle BGM
typedef struct EncounterEffectTaskData {
    int taskState;
    BOOL done;
    int encEffectID;
    int battleBGM;
} EncounterEffectTaskData;

void sub_02055218(TaskManager *man, int effect, int bgm);

#endif
