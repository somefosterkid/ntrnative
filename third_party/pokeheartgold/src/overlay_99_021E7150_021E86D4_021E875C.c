#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "heap.h"
#include "overlay_99_pokeathlon_event_record.h"

extern u8 ov99_021EA59C;

void ov99_021E875C(u8 *param_1);

void ov99_021E875C(u8 *param_1) {
    u8 uVar1;
    NARC_Delete(((PokeathlonEventRecord *)param_1)->narc);
    uVar1 = 0;
    do {
        FreeBgTilemapBuffer(((PokeathlonEventRecord *)param_1)->bgConfig, (&ov99_021EA59C)[uVar1]);
        uVar1 = uVar1 + 1;
    } while (uVar1 < 6);
    Heap_Free(((void *)((PokeathlonEventRecord *)param_1)->bgConfig));
    return;
}
