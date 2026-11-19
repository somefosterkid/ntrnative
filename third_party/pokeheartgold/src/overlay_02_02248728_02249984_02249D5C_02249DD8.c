#include "global.h"

#include "heap.h"
#include "sys_task_api.h"
#include "unk_0200A090.h"
#include "overlay_02_hmcut_in.h"

void ov02_02249DD8(SysTask *task, void *hmCutInPtr);

void ov02_02249DD8(SysTask *task, void *hmCutInPtr) {
    int i;
    HMCutIn *cutIn = hmCutInPtr;

    if (cutIn->resourcesLoaded == TRUE) {
        for (i = 0; i < 4; i++) {
            if (cutIn->charSource[i] != NULL) {
                sub_0200A740(cutIn->charSource[i]);
            }
        }
        for (i = 0; i < 3; i++) {
            if (cutIn->paletteSource[i] != NULL) {
                sub_0200A740(cutIn->paletteSource[i]);
            }
        }
        if (cutIn->pokemonCharSource != NULL) {
            Heap_Free(cutIn->pokemonCharSource);
            cutIn->pokemonCharSource = NULL;
        }
        if (cutIn->pokemonPaletteSource != NULL) {
            Heap_Free(cutIn->pokemonPaletteSource);
            cutIn->pokemonPaletteSource = NULL;
        }
        cutIn->resourceSourcesFreed = TRUE;
        SysTask_Destroy(task);
    }
}
