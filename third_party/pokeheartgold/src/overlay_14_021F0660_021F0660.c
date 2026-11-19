#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "item.h"
#include "party.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "pokemon_types_def.h"
#include "unk_0201956C.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

void ov14_021E8314(UnkStruct_0201956C *arg0);
s32 ov14_021E6480(void *, s32);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F08BC(void *);
s32 ov14_021F3190(void *, u32, s32);
s32 ov14_021F3488(void *, s32, s32);
s32 ov14_021F34C8(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F3F6C(void *);
s32 ov14_021F685C(void *, u32, s32, s32);
s32 ov14_021F6928(void *, s32, s32);
void ov14_021F0660(struct UnkStruct_ov14_boxapp *arg0, s32 arg1);

s32 ov14_021E9A24(UnkStruct_ov14_boxapp *app);

void ov14_021F0660(UnkStruct_ov14_boxapp *arg0, s32 arg1) {
    s32 sp0;
    int new_var;
    Pokemon *temp_r4;
    u32 var_r4;
    void *temp_r0;
    sp0 = arg1;
    arg0->unk21 = (u8)sp0;
    var_r4 = 0x1E;
    do {
        temp_r0 = *((void * *)&arg0->sys);
        if (var_r4 == (arg0->unk21)) {
            ov14_021F3190(*((void * *)&arg0->sys), var_r4, 0);
        } else {
            ov14_021F3190(*((void * *)&arg0->sys), var_r4, 1);
        }
        var_r4 += 1;
    } while (var_r4 < 0x24U);
    ov14_021F3F6C(arg0);
    ov14_021F3488(arg0, 2, 1);
    ov14_021F34C8(*((void * *)&arg0->sys), arg0->unk21, 0);
    ov14_021F685C(arg0, arg0->unk21, 1, 0x27);
    temp_r4 = Party_GetMonByIndex(arg0->party, sp0 - 0x1E);
    sp0 -= 0x1E;
    if (ov14_021E6480(arg0, sp0) == 0) {
        ov14_021F6928(arg0, 0x28, 8);
    } else if (ItemIdIsMail((u16)GetMonData(temp_r4, new_var = 6, 0L)) == 1) {
        ov14_021F6928(arg0, 0x28, new_var);
    } else if (GetMonData(temp_r4, 0xA2, 0L) != 0) {
        ov14_021F6928(arg0, 0x28, 7);
    } else if (PCStorage_CountEmptySpotsInBox(*((PCStorage * *)&arg0->storage), (u32)(arg0->unk1F)) == 0) {
        ov14_021F6928(arg0, 0x28, 2);
    } else {
        ov14_021F6928(arg0, 0x28, 0);
    }
    ov14_021F08BC(arg0);
    ov14_021E84A4(arg0->sys->unk_2F0);
    if (ov14_021E8544(arg0->sys->unk_2F0) == 0) {
        ov14_021E8314(arg0->sys->unk_2F0);
    }
    *((s8 *)&arg0->unk22) = 1;
    ov14_021F0234(arg0, ov14_021E9A24, 0x6E);
}
