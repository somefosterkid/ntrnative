#include "global.h"

#include "constants/heap.h"

#include "bag.h"
#include "bag_types_def.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021EEA7C(Ov112PokewalkerApp *app);

void ov112_021EEA7C(Ov112PokewalkerApp *app) {
    s32 i;
    for (i = 0; i < 3; i++) {
        if (app->unk_9DAC[i].unk_0 != 0) {
            Bag_AddItem(app->bag, app->unk_9DAC[i].unk_0, 1, HEAP_ID_154);
        }
    }
    for (i = 0; i < 10; i++) {
        if (app->unk_9DB8[i].unk_0 != 0) {
            Bag_AddItem(app->bag, app->unk_9DB8[i].unk_0, 1, HEAP_ID_154);
        }
    }
    if ((((u32)(app->unk_AABC << 0x19)) >> 0x1f) != 0) {
        Bag_AddItem(app->bag, app->unk_B002, 1, HEAP_ID_154);
    }
    return;
}
