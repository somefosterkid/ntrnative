#include "global.h"

#include "pokemon.h"
#include "pokemon_types_def.h"
#include "unk_0201956C.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

BoxPokemon *ov14_021E60C0(UnkStruct_ov14_boxapp *param_1, u32 param_2, u32 param_3);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
void ov14_021F027C(UnkStruct_ov14_boxapp *param_1, s8 param_2);
void ov14_021F2A18(struct UnkStruct_ov14_boxsys *param_1, s32 param_2, s32 param_3);
s32 ov14_021E9450(UnkStruct_ov14_boxapp *app);

void ov14_021F1EB8(UnkStruct_ov14_boxapp *param_1) {
    UnkStruct_ov14_boxsys *sys;
    BoxPokemon *boxMon;
    u32 uVar2;
    u32 uVar3;

    sys = param_1->sys;
    if (sys->unk_88C8 == 0) {
        ov14_021F2A18(sys, 9, 0);
        boxMon = ov14_021E60C0(param_1, (u32) param_1->unk1F, (u32) param_1->unk21);
        uVar2 = GetBoxMonData(boxMon, 0x4c, (void *)0x0);
        if (uVar2 == 0) {
            param_1->unk2C = 0x24;
            ov14_021F027C(param_1, 1);
            return;
        }
        uVar3 = 0x7c;
    } else {
        ov14_021F2A18(sys, 9, 0);
        uVar3 = 0x78;
    }
    ov14_021E84A4(param_1->sys->unk_2F0);
    ov14_021F0234(param_1, ov14_021E9450, uVar3);
    return;
}
