#include "global.h"

#include "constants/heap.h"
#include "overlay_01_021FB4C0.h"

#include "field/hblank_system_priv.h"
struct HBlankTask;

void ov01_021FB584(HBlankTask *task);

void ov01_021FB5B0(struct HBlankTask *arg0, void *arg1);

void ov01_021FB55C(HBlankSystem *hBlankSystem) {
    memset(hBlankSystem, 0, sizeof(HBlankSystem));
    hBlankSystem->active = 0;

    for (int i = 0; i < 2; i++) {
        ov01_021FB584(&hBlankSystem->tasks[i]);
    }
}

void ov01_021FB584(HBlankTask *task) {
    task->used = 0;
    task->callback = ov01_021FB5B0;
    task->param = 0L;
}

void ov01_021FB594(void *param) {
    HBlankSystem *hBlankSystem = param;

    for (int i = 0; i < 2; i++) {
        hBlankSystem->tasks[i].callback(&hBlankSystem->tasks[i], hBlankSystem->tasks[i].param);
    }
}
