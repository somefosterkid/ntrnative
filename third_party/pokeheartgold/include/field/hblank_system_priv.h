#ifndef POKEHEARTGOLD_FIELD_HBLANK_SYSTEM_PRIV_H
#define POKEHEARTGOLD_FIELD_HBLANK_SYSTEM_PRIV_H

#include "global.h"

#include "constants/heap.h"

#include "field/hblank_system.h"

typedef struct HBlankTask HBlankTask;

typedef void (*HBlankCallback)(HBlankTask *task, void *param);

struct HBlankTask {
    BOOL used;
    void *param;
    HBlankCallback callback;
};

struct HBlankSystem {
    BOOL active;
    HBlankTask tasks[2];
};

void ov01_021FB55C(HBlankSystem *hBlankSystem);
HBlankTask *ov01_021FB5B4(HBlankSystem *hBlankSystem);

#endif // POKEHEARTGOLD_FIELD_HBLANK_SYSTEM_PRIV_H
