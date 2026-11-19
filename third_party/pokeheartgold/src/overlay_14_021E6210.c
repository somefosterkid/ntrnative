#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "party.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "pokemon_storage_system.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021E62C8(UnkStruct_ov14_boxapp *param_1, s32 param_2);

s32 ov14_021E6100(void *, u32, s32);
s32 ov14_021E6464(UnkStruct_ov14_boxapp *param_1, u32 param_2);
void ov14_021F4A20(UnkStruct_ov14_boxapp *param_1, s32 param_2);

void ov14_021E6210(struct UnkStruct_ov14_boxapp *param_1, u32 *param_2);
BoxPokemon *ov14_021E60C0(UnkStruct_ov14_boxapp *param_1, u32 param_2, u32 param_3);
void ov14_021F4958(UnkStruct_ov14_boxapp *param_1, u32 param_2);

void ov14_021E6210(UnkStruct_ov14_boxapp *param_1, u32 *param_2) {
    Pokemon *pPVar1;
    s32 slot;
    s32 slot_00;
    PartyExtraSub PStack_1b;
    PartyExtraSub PStack_20;

    slot_00 = param_2[1] + -0x1e;
    slot = param_2[2] + -0x1e;
    pPVar1 = Party_GetMonByIndex(param_1->party, slot_00);
    CopyPokemonToPokemon(pPVar1, (Pokemon *)*param_2);
    Party_GetMonAprijuiceModifiers(param_1->party, &PStack_1b, slot_00);
    Party_GetMonAprijuiceModifiers(param_1->party, &PStack_20, slot);
    pPVar1 = Party_GetMonByIndex(param_1->party, slot);
    Party_SafeCopyMonToSlot_ResetAprijuiceModifiers(param_1->party, slot_00, pPVar1);
    Party_SafeCopyMonToSlot_ResetAprijuiceModifiers(param_1->party, slot, (Pokemon *)*param_2);
    Party_SetMonAprijuiceModifiers(param_1->party, &PStack_20, slot_00);
    Party_SetMonAprijuiceModifiers(param_1->party, &PStack_1b, slot);
    return;
}

void ov14_021E627C(void *arg0, void *arg1) {
    u8 *new_var;
    PartyExtraSub v0;
    s32 slot;
    int new_var2;
    new_var = ((u8 *)arg0) + 0x1F;
    slot = (*((s32 *)(((u8 *)arg1) + 4))) - 0x1E;
    CopyPokemonToPokemon(Party_GetMonByIndex(((UnkStruct_ov14_boxapp *)arg0)->party, slot), *((Pokemon **)arg1));
    Party_GetMonAprijuiceModifiers(((UnkStruct_ov14_boxapp *)arg0)->party, &v0, slot);
    ov14_021E6100(arg0, *((u8 *)new_var), *((s32 *)(((u8 *)arg1) + 4)));
    Party_AddMon(((UnkStruct_ov14_boxapp *)arg0)->party, *((Pokemon **)arg1));
    new_var2 = Party_GetCount(((UnkStruct_ov14_boxapp *)arg0)->party) - 1;
    Party_SetMonAprijuiceModifiers(((UnkStruct_ov14_boxapp *)arg0)->party, &v0, new_var2);
}

void ov14_021E62C8(UnkStruct_ov14_boxapp *param_1, s32 param_2) {
    BoxPokemon *boxMon;
    u32 boxno;

    boxMon = ov14_021E60C0(param_1, (u32) (param_1)->unk1F, *(u32 *)(param_2 + 4));
    boxno = ov14_021E6464(param_1, *(u32 *)(param_2 + 8));
    PCStorage_PlaceMonInBoxFirstEmptySlot(((PCStorage *)(param_1)->storage), boxno, boxMon);
    PCStorage_DeleteBoxMonByIndexPair(((PCStorage *)(param_1)->storage), (u32) (param_1)->unk1F, *(u32 *)(param_2 + 4));
    ov14_021F4958(param_1, (u32) (param_1)->unk1F);
    ov14_021F4A20(param_1, (u32) (param_1)->unk1F);
    ov14_021F4958(param_1, boxno);
    ov14_021F4A20(param_1, boxno);
    return;
}
