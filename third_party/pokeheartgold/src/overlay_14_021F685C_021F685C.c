#include "global.h"

#include "message_format.h"
#include "pokemon_types_def.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

BoxPokemon *ov14_021E60C0(UnkStruct_ov14_boxapp *, u32, u32);
void ov14_021F6698(UnkStruct_ov14_boxapp *, u32, u32);
void ov14_021F685C(UnkStruct_ov14_boxapp *param_1, u32 param_2, u32 param_3, u32 param_4);

void ov14_021F685C(UnkStruct_ov14_boxapp *param_1, u32 param_2, u32 param_3, u32 param_4) {
    BoxPokemon *boxmon;
    u32 unaff_r4;

    switch (param_3) {
    case 0:
        unaff_r4 = 0x12;
        break;
    case 1:
        unaff_r4 = 0;
        boxmon = (BoxPokemon *)
            ov14_021E60C0(param_1, (param_1)->unk1F, (param_1)->unk21);
        BufferBoxMonNickname((param_1)->sys->unk_24, 0, boxmon);
        break;
    case 2:
        unaff_r4 = 5;
        break;
    case 3:
        unaff_r4 = 0x13;
        break;
    case 4:
        unaff_r4 = 0xd;
        break;
    case 5:
        unaff_r4 = 0x1d;
        break;
    case 6:
        unaff_r4 = 0x1e;
    }
    ov14_021F6698(param_1, unaff_r4, param_4);
    return;
}
