#include "global.h"
#include "pokeathlon/pokeathlon.h"
#include "heap.h"

s32 sub_02036FD8(s32, void *, s32);

void ov96_021E88FC(Ov96Handlers *);

BOOL ov96_021E87EC(s32 cmd, void *data, s32 size, Ov96System *system);

BOOL ov96_021E87EC(s32 cmd, void *data, s32 size, Ov96System *system) {
    if (system->isLocal == 1) {
        return sub_02036FD8(cmd, data, size);
    }
    return ov96_021E8988(system->handlers, cmd, data, size);
}

void ov96_021E8810(Ov96System *ptr) {
    if (ptr->isLocal == 0) {
        ov96_021E88FC(ptr->handlers);
    }
    Heap_Free(ptr);
}
