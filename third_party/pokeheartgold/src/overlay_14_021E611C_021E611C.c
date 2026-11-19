#include "global.h"

#include "party.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "pokemon_types_def.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021F4A20(UnkStruct_ov14_boxapp *param_1, s32 param_2);

BoxPokemon *ov14_021E60C0(UnkStruct_ov14_boxapp *param_1, u32 param_2, u32 param_3);
void ov14_021F4958(UnkStruct_ov14_boxapp *param_1, u32 param_2);
void ov14_021E611C(UnkStruct_ov14_boxapp *param_1, MoveEntry *param_2, MoveEntry *param_3);

// Swaps the box mon in param_2's slot with the party mon in param_3's slot (param_2/param_3 are MoveSet entries).
void ov14_021E611C(UnkStruct_ov14_boxapp *param_1, MoveEntry *param_2, MoveEntry *param_3) {
    BoxPokemon *pBVar1;
    Pokemon *src;
    u8 uStack_18;
    s32 iVar2;
    pBVar1 = ov14_021E60C0(param_1, (u32)((param_1)->unk1F), param_2->idx);
    CopyBoxPokemonToPokemon(pBVar1, param_2->mon);
    iVar2 = param_3->idx;
    src = Party_GetMonByIndex((param_1)->party, iVar2 + (-0x1e));
    CopyPokemonToPokemon(src, param_3->mon);
    uStack_18 = 0;
    SetMonData(param_3->mon, 0xbb, &uStack_18);
    Party_SafeCopyMonToSlot_ResetAprijuiceModifiers((param_1)->party, iVar2 + (-0x1e), param_2->mon);
    pBVar1 = Mon_GetBoxMon(param_3->mon);
    PCStorage_PlaceMonInBoxByIndexPair(((PCStorage *)(param_1)->storage), (u32)((param_1)->unk1F), param_3->arg, pBVar1);
    ov14_021F4958(param_1, (u32)((param_1)->unk1F));
    ov14_021F4A20(param_1, (u32)((param_1)->unk1F));
}
