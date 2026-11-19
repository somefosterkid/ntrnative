#include "global.h"

#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F42EC(UnkStruct_ov14_boxsys *arg0) {
    u32 v0;

    v0 = 0;
    do {
        SpriteSystem_LoadCharResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_1_9, 0x4A, 0, 1, v0 + 0xC129);
        v0 += 1;
    } while (v0 < 4U);
    SpriteSystem_LoadPlttResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_1_9, 0x4B, 0, 2, 1, 0xC100);
    SpriteSystem_LoadCellResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_1_9, 0x4C, 1, 0xC0FE);
    SpriteSystem_LoadAnimResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_1_9, 0x4D, 1, 0xC0FE);
}
