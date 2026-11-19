#include "global.h"

#include "party.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "pokemon_types_def.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021E6100(UnkStruct_ov14_boxapp *param_1, u32 param_2, u32 param_3);
void ov14_021F4A20(UnkStruct_ov14_boxapp *param_1, s32 param_2);

void ov14_021F4958(UnkStruct_ov14_boxapp *param_1, u32 param_2);
void ov14_021E61BC(UnkStruct_ov14_boxapp *param_1, u8 *param_2);

void ov14_021E61BC(UnkStruct_ov14_boxapp *param_1, u8 *param_2) {
    Pokemon *mon;
    BoxPokemon *boxMon;
    u8 auStack_18;

    mon = Party_GetMonByIndex((param_1)->party, *(s32 *)(param_2 + 4) + -0x1e);
    auStack_18 = 0;
    SetMonData(mon, 0xbb, &auStack_18);
    boxMon = Mon_GetBoxMon(mon);
    PCStorage_PlaceMonInBoxByIndexPair(((PCStorage *)(param_1)->storage), (u32) (param_1)->unk1F, *(u32 *)(param_2 + 8), boxMon);
    ov14_021E6100(param_1, (u32) (param_1)->unk1F, *(u32 *)(param_2 + 4));
    ov14_021F4958(param_1, (u32) (param_1)->unk1F);
    ov14_021F4A20(param_1, (u32) (param_1)->unk1F);
    return;
}
