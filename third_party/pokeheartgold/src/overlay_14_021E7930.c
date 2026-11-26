#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "pokemon_storage_system.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

u32 ov14_021E7930(struct UnkStruct_ov14_boxapp *param_1, u32 param_2);

u32 ov14_021E7930(UnkStruct_ov14_boxapp *param_1, u32 param_2) {
    u8 bVar1;
    PCStorage **new_var;
    u32 uVar2;
    new_var = &(param_1->storage);
    bVar1 = PCStorage_GetBoxWallpaper(*new_var, param_2);
    uVar2 = (u32)bVar1;
    if ((0xf + 1) <= uVar2) {
        uVar2 = uVar2 - 0x10;
    }
    return uVar2;
}

s32 ov14_021E7940(s32 arg0, s32 arg1, void *arg2) {
    if ((arg0 >= (s32) * (u8 *)arg2) && (arg0 <= (s32) * (u8 *)((u8 *)arg2 + 1)) && (arg1 >= (s32) * (u8 *)((u8 *)arg2 + 2)) && (arg1 < (s32) * (u8 *)((u8 *)arg2 + 3))) {
        return 1;
    }
    return 0;
}
