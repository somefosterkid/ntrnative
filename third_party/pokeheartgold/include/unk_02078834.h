#ifndef POKEHEARTGOLD_UNK_02078834_H
#define POKEHEARTGOLD_UNK_02078834_H

#include "task.h"

struct SaveData;

typedef struct UnkAppArgs3C {
    u8 filler0[0x34];
    struct SaveData *unk34;
    s8 unk38;
    s8 unk39;
} UnkAppArgs3C;

BOOL sub_02078D24(s32 a0);
void sub_02078B58(TaskManager *taskManager);
void sub_02078B78(TaskManager *taskManager, u16 *var_p);

#endif // POKEHEARTGOLD_UNK_02078834_H
