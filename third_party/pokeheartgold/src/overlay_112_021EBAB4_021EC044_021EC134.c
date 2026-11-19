#include "global.h"

#include "pokemon_storage_system.h"
#include "pokewalker.h"
#include "save.h"
#include "save_arrays.h"

void ov112_021EC134(void *arg0);

void ov112_021EC134(void *arg0) {
    void *new_var;
    void *new_var2;
    s32 sp8;
    s32 sp4;
    u16 sp0[2];
    PCStorage *temp_r4;
    sp8 = -1;
    sp4 = 0;
    temp_r4 = SaveArray_PCStorage_Get(*((SaveData **)(((u8 *)arg0) + 0x20)));
    new_var2 = arg0 + 0x1F2E8;
    new_var = new_var2;
    sub_02032688(*((POKEWALKER **)(((u8 *)arg0) + 0x1E440)), &sp0[1], &sp0[0]);
    if (Pokewalker_TryGetBoxMon(*((POKEWALKER **)(((u8 *)arg0) + 0x1E440)), new_var2) != 0) {
        sp8 = (s32)sp0[0];
        PCStorage_FindFirstEmptySlot(temp_r4, &sp8, &sp4);
        PCStorage_PlaceMonInBoxByIndexPair(temp_r4, (u32)sp8, (u32)sp4, new_var);
        *((s16 *)(((u8 *)arg0) + 0x1F370)) = (s16)sp8;
        return;
    }
    *((s16 *)(((u8 *)arg0) + 0x1F370)) = -1;
}
