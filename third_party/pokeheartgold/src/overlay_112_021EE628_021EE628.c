#include "global.h"

#include "pokemon_storage_system.h"
#include "pokewalker.h"
#include "save_arrays.h"
#include "overlay_112_pokewalker_app.h"

s32 ov112_021EECF0(void *);

s32 ov112_021EE628(Ov112PokewalkerApp *arg0);

s32 ov112_021EE628(Ov112PokewalkerApp *arg0) {
    PCStorage *temp_r7;
    u32 temp_r6;
    u32 temp_r4;
    temp_r7 = SaveArray_PCStorage_Get(arg0->saveData);
    temp_r4 = arg0->link.unk_C;
    temp_r6 = arg0->link.unk_10;
    sub_020326A4(arg0->pokewalker, 2U, temp_r4);
    sub_02032720(arg0->pokewalker, arg0->unk_1D798);
    Pokewalker_SetBoxMon(arg0->pokewalker, PCStorage_GetMonByIndexPair(temp_r7, temp_r4, temp_r6));
    PCStorage_DeleteBoxMonByIndexPair(temp_r7, temp_r4, temp_r6);
    ov112_021EECF0(arg0);
    return 2;
}
