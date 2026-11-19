#include "global.h"

#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F41E4(UnkStruct_ov14_boxsys *arg0) {
    u32 v0;

    v0 = 0;
    do {
        SpriteSystem_LoadCharResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_1_9, 0x46, 0, 1, v0 + 0xC123);
        v0 += 1;
    } while (v0 < 6U);
    SpriteSystem_LoadPlttResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_1_9, 0x47, 0, 1, 1, 0xC0FF);
    SpriteSystem_LoadCellResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_1_9, 0x48, 1, 0xC0FD);
    SpriteSystem_LoadAnimResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_1_9, 0x49, 1, 0xC0FD);
}
