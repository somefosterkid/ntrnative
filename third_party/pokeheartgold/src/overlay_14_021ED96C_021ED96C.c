#include "global.h"

#include "pokemon_storage_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021E78AC(UnkStruct_ov14_boxapp *, s32);
u32 ov14_021ED96C(UnkStruct_ov14_boxapp *param_1);

void ov14_021F4530(UnkStruct_ov14_boxapp *);
void ov14_021F4958(UnkStruct_ov14_boxapp *, u32);

u32 ov14_021ED96C(UnkStruct_ov14_boxapp *param_1) {
    u8 wallpaperno;

    ov14_021E78AC(param_1, *((u8 *)&(param_1)->sys->unk_44D));
    wallpaperno = *((u8 *)&(param_1)->sys->unk_44D);
    if (wallpaperno < 0x10) {
        PCStorage_SetBoxWallpaper(((PCStorage *)(param_1)->storage), (u32) (param_1)->unk1F, wallpaperno);
    } else {
        PCStorage_SetBoxWallpaper(((PCStorage *)(param_1)->storage), (u32) (param_1)->unk1F, wallpaperno + 0x10);
    }
    ov14_021F4530(param_1);
    ov14_021F4958(param_1, (param_1)->unk1F);
    return 0x48;
}
