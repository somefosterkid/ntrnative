#include "global.h"

#include "sprite.h"
#include "overlay_96_unk_struct_ov96_021_f7934.h"
#include "sprite_system.h"

void ov96_021F85A0(UnkStruct_ov96_021F7934 *arg0);

void ov96_021F85A0(UnkStruct_ov96_021F7934 *arg0) {
    ManagedSpriteTemplate sp0 = { 0 };
    ManagedSprite *temp_r0;

    sp0.resIdList[0] = 0x3E8;
    sp0.resIdList[1] = 0x3E8;
    sp0.resIdList[2] = 0x3E8;
    sp0.resIdList[3] = 0x3E8;
    sp0.vram = NNS_G2D_VRAM_TYPE_2DSUB;
    sp0.bgPriority = 1;
    sp0.x = 0xE0;
    sp0.y = 0x70;
    temp_r0 = SpriteSystem_NewSprite(arg0->spriteSystem, arg0->spriteManager, &sp0);
    arg0->sprites[0] = temp_r0;
}
