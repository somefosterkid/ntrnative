#include "global.h"

#include "pokemon_storage_system.h"
#include "pokewalker.h"
#include "save.h"
#include "save_arrays.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021EC134(Ov112PokewalkerApp *arg0) {
    void *new_var;
    void *new_var2;
    s32 sp8;
    s32 sp4;
    u16 sp0[2];
    PCStorage *temp_r4;
    sp8 = -1;
    sp4 = 0;
    temp_r4 = SaveArray_PCStorage_Get(arg0->saveData);
    new_var2 = arg0->unk_1F2E8;
    new_var = new_var2;
    sub_02032688(arg0->pokewalker, &sp0[1], &sp0[0]);
    if (Pokewalker_TryGetBoxMon(arg0->pokewalker, new_var2) != 0) {
        sp8 = (s32)sp0[0];
        PCStorage_FindFirstEmptySlot(temp_r4, &sp8, &sp4);
        PCStorage_PlaceMonInBoxByIndexPair(temp_r4, (u32)sp8, (u32)sp4, new_var);
        arg0->unk_1F370 = (s16)sp8;
        return;
    }
    arg0->unk_1F370 = -1;
}
