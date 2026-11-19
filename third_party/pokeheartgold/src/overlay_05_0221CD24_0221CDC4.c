#include "global.h"

#include "overlay_05.h"
#include "sprite_system.h"
#include "overlay_05_unk_struct_ov10_0221_fb28_t.h"

void ov05_0221CDC4(UnkStruct_ov10_0221FB28 *arg0, void *arg1) {
    ManagedSpriteTemplate unkTemplate;

    unkTemplate.x = 0;
    unkTemplate.y = 0;
    unkTemplate.z = 0;
    unkTemplate.animation = 0;
    unkTemplate.drawPriority = *(s32 *)((u8 *)arg1 + 0x14);
    unkTemplate.pal = *(s32 *)((u8 *)arg1 + 0x18);
    unkTemplate.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    unkTemplate.resIdList[0] = *(s32 *)arg1;
    unkTemplate.resIdList[1] = *(s32 *)((u8 *)arg1 + 4);
    unkTemplate.resIdList[2] = *(s32 *)((u8 *)arg1 + 8);
    unkTemplate.resIdList[3] = *(s32 *)((u8 *)arg1 + 0xC);
    unkTemplate.bgPriority = *(s32 *)((u8 *)arg1 + 0x10);
    unkTemplate.vramTransfer = 0;
    SpriteSystem_NewSprite(arg0->unk_190, arg0->unk_194, &unkTemplate);
}
