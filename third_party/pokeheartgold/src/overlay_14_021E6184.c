#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "party.h"
#include "pokemon.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"
#include "overlay_14_move_owner.h"

s32 ov14_021E6100(void *, u32, s32);
s32 ov14_021F4A20(void *, u32);

BoxPokemon *ov14_021E60C0(void *, u32, s32);
s32 ov14_021F4958(void *, u32);

void ov14_021E6184(UnkStruct_ov14_boxapp *arg0, MoveSet *arg1) {
    CopyBoxPokemonToPokemon(ov14_021E60C0(arg0, arg0->unk1F, arg1->e[0].idx), arg1->e[0].mon);
    Party_AddMon(arg0->party, arg1->e[0].mon);
    ov14_021E6100(arg0, arg0->unk1F, arg1->e[0].idx);
    ov14_021F4958(arg0, arg0->unk1F);
    ov14_021F4A20(arg0, arg0->unk1F);
}
