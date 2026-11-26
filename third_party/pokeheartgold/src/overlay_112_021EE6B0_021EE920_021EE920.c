#include "global.h"

#include "constants/heap.h"

#include "bag.h"
#include "bag_types_def.h"
#include "pokewalker.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021EE920(Ov112PokewalkerApp *param_1) {
    u32 uVar1;
    param_1->unk_1F376 = 0;
    if ((((u32)((param_1->unk_9D78) << 0x1f)) >> 0x1f) && (uVar1 = sub_02032764(param_1->pokewalker), uVar1 == 0)) {
        sub_0203276C(param_1->pokewalker);
        Bag_AddItem(param_1->bag, 0xcf, 1, HEAP_ID_154);
        param_1->unk_1F376 = 1;
    }
    return;
}
