#include "global.h"

#include "message_format.h"
#include "pokemon_types_def.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

BoxPokemon *ov14_021E60C0(UnkStruct_ov14_boxapp *, u32, u32);
void ov14_021F6698(UnkStruct_ov14_boxapp *, u32, u32);

void ov14_021F67B0(UnkStruct_ov14_boxapp *param_1, u32 param_2, u32 param_3) {
    BoxPokemon *pBVar1;
    u32 unaff_r5;

    switch (param_2) {
    case 0:
        unaff_r5 = 2;
        break;
    case 1:
        unaff_r5 = 3;
        pBVar1 = (BoxPokemon *)
            ov14_021E60C0(param_1, (param_1)->unk1F, (param_1)->unk21);
        BufferBoxMonNickname((param_1)->sys->unk_24, 0, pBVar1);
        break;
    case 2:
        unaff_r5 = 4;
        pBVar1 = (BoxPokemon *)
            ov14_021E60C0(param_1, (param_1)->unk1F, (param_1)->unk21);
        BufferBoxMonNickname((param_1)->sys->unk_24, 0, pBVar1);
        break;
    case 3:
        unaff_r5 = 0x1f;
        break;
    case 4:
        unaff_r5 = 0x20;
        pBVar1 = (BoxPokemon *)
            ov14_021E60C0(param_1, (param_1)->unk1F, (param_1)->unk21);
        BufferBoxMonNickname((param_1)->sys->unk_24, 0, pBVar1);
        break;
    case 5:
        unaff_r5 = 0x21;
        break;
    case 6:
        unaff_r5 = 6;
    }
    ov14_021F6698(param_1, unaff_r5, param_3);
    return;
}
