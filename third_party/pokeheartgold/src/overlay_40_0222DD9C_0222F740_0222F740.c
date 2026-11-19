#include "global.h"
struct UnkStruct_ov62_02233310;
struct UnkStruct_0208C06C;

#include "sprite_system.h"
#include "overlay_40_unk_struct_0208_c06_c.h"
#include "overlay_40_unk_struct_ov62_02233310.h"

s32 ov40_0222F878(void *);

void ov40_0222F740(struct UnkStruct_ov62_02233310 *arg0, struct UnkStruct_0208C06C *arg1, enum NNS_G2D_VRAM_TYPE arg2);

void ov40_0222F740(UnkStruct_ov62_02233310 *arg0, UnkStruct_0208C06C *arg1, enum NNS_G2D_VRAM_TYPE arg2) {
    ManagedSpriteTemplate sp14;
    NARC *sp10;
    SpriteSystem *spC;
    ManagedSprite *temp_r0_2;
    SpriteSystem *temp_r0;
    SpriteManager *temp_r7;
    temp_r7 = arg1->unk_14.unk_08;
    sp10 = arg1->unk_14.unk_00;
    temp_r0 = arg1->unk_14.unk_04;
    spC = temp_r0;
    SpriteSystem_LoadCharResObjFromOpenNarc(temp_r0, temp_r7, sp10, 0x7F, 0, arg2, 0x30D40);
    SpriteSystem_LoadCellResObjFromOpenNarc(temp_r0, temp_r7, sp10, 0x7E, 0, 0x30D40);
    SpriteSystem_LoadAnimResObjFromOpenNarc(temp_r0, temp_r7, sp10, 0x7D, 0, 0x30D40);
    sp14.x = 0x80;
    sp14.y = 0x60;
    sp14.z = 0;
    sp14.animation = 0;
    sp14.drawPriority = 1;
    sp14.bgPriority = 1;
    sp14.resIdList[4] = -1;
    sp14.resIdList[5] = -1;
    sp14.pal = 0;
    sp14.vram = arg2;
    sp14.vramTransfer = 0;
    sp14.resIdList[0] = 0x30D40;
    sp14.resIdList[2] = 0x30D40;
    sp14.resIdList[3] = 0x30D40;
    if (arg2 == NNS_G2D_VRAM_TYPE_2DMAIN) {
        sp14.resIdList[1] = 0x270F;
    } else {
        sp14.resIdList[1] = 0x2710;
    }
    arg0->unk_2C[0] = SpriteSystem_NewSprite(arg1->unk_14.unk_04, arg1->unk_14.unk_08, &sp14);
    arg0->unk_2C[1] = SpriteSystem_NewSprite(arg1->unk_14.unk_04, arg1->unk_14.unk_08, &sp14);
    ManagedSprite_SetPaletteOverride(arg0->unk_2C[0], 2);
    ManagedSprite_SetPaletteOverride(arg0->unk_2C[1], 2);
    ManagedSprite_SetAnim(arg0->unk_2C[0], 0);
    ManagedSprite_SetAnim(arg0->unk_2C[1], 0);
    ManagedSprite_TickFrame(arg0->unk_2C[0]);
    ManagedSprite_TickFrame(arg0->unk_2C[1]);
    ManagedSprite_SetFlipMode(arg0->unk_2C[1], 2);
    temp_r0_2 = arg0->unk_2C[0];
    if (arg2 == NNS_G2D_VRAM_TYPE_2DMAIN) {
        ManagedSprite_SetPositionXY(arg0->unk_2C[0], 0x80, 0x18);
        ManagedSprite_SetPositionXY(arg0->unk_2C[1], 0x80, 0x78);
    } else {
        ManagedSprite_SetPositionXY(arg0->unk_2C[0], 0x80, 0x58);
        ManagedSprite_SetPositionXY(arg0->unk_2C[1], 0x80, 0xB8);
    }
    ov40_0222F878(arg0);
}
