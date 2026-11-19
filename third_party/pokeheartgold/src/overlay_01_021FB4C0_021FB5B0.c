#include "global.h"
#include "constants/heap.h"

#include "field/hblank_system_priv.h"

void ov01_021FB5B0(HBlankTask *task, void *param);

void ov01_021FB5B0(HBlankTask *task, void *param) {
}

HBlankTask *ov01_021FB5B4(HBlankSystem *hBlankSystem) {
    for (int i = 0; i < 2; i++) {
        if (hBlankSystem->tasks[i].used == 0) {
            return &hBlankSystem->tasks[i];
        }
    }

    return 0L;
}
