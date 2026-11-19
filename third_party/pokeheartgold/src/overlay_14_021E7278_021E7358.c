#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "overlay_14_box_funcs.h"

UnkStruct_ov14_mon *ov14_021E7358(BoxPokemon *arg0) {
    u16 temp_r0;
    u32 var_r4;
    UnkStruct_ov14_mon *var_r6;
    long long new_var;
    if (GetBoxMonData(arg0, MON_DATA_SPECIES_EXISTS, NULL) != 0) {
        var_r6 = Heap_Alloc(HEAP_ID_10, sizeof(UnkStruct_ov14_mon));
        var_r6->boxmon = arg0;
        var_r6->species = (u16)GetBoxMonData(arg0, MON_DATA_SPECIES, NULL);
        var_r6->item = (s16)GetBoxMonData(arg0, MON_DATA_HELD_ITEM, NULL);
        var_r6->personality = GetBoxMonData(arg0, MON_DATA_PERSONALITY, NULL);
        var_r6->type1 = (s8)GetBoxMonData(arg0, MON_DATA_TYPE_1, NULL);
        var_r6->type2 = (s8)GetBoxMonData(arg0, MON_DATA_TYPE_2, NULL);
        new_var = GetBoxMonData(arg0, MON_DATA_ABILITY, NULL);
        var_r6->ability = (s8)new_var;
        var_r6->nature = GetBoxMonNature(arg0);
        var_r6->unk_10 = (s16)GetBoxMonData(arg0, MON_DATA_MARKINGS, NULL);
        var_r6->levelEgg = (u8)((0x7F & ((u8)GetBoxMonData(arg0, MON_DATA_LEVEL, NULL))) | (var_r6->levelEgg & (~0x7F)));
        var_r6->levelEgg = (u8)(((u8)(((u8)GetBoxMonData(arg0, MON_DATA_IS_EGG, NULL)) << 7)) | (var_r6->levelEgg & (~0x80)));
        var_r6->gender = GetBoxMonGender(arg0);
        temp_r0 = var_r6->species;
        if (((temp_r0 != SPECIES_NIDORAN_F) && (temp_r0 != SPECIES_NIDORAN_M)) && (var_r6->isEgg == 0)) {
            var_r6->hasGender = 1;
        } else {
            var_r6->hasGender = 0;
        }
        for (var_r4 = 0; var_r4 < 4U; var_r4++) {
            var_r6->msgIds[var_r4] = (s16)GetBoxMonData(arg0, var_r4 + MON_DATA_MOVE1, NULL);
        }
    } else {
        var_r6 = NULL;
    }
    return var_r6;
}
