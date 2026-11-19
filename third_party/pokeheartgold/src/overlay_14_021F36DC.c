#include "global.h"

#include "gf_gfx_loader.h"
#include "heap.h"
#include "item.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F3844(UnkStruct_ov14_boxsys *arg0, u16 arg1);

void ov14_021F395C(void *param_1, s16 param_2, s16 param_3);

void ov14_021F38B0(void *arg0, u16 arg1);
void ov14_021F391C(UnkStruct_ov14_boxsys *arg0, s32 arg1);

void ov14_021F36DC(UnkStruct_ov14_boxapp *arg0, UnkStruct_ov14_mon *arg1, s32 arg2) {
    int new_var2;
    s32 v2;
    UnkStruct_ov14_boxsys *v0;
    UnkStruct_ov14_boxsys *v1;

    v0 = arg0->sys;
    v2 = arg2;
    v2 = v2 + v0->unk_88D0;
    ov14_021F3614(v0, arg1, v2);
    ManagedSprite_SetDrawFlag(arg0->sys->sprites[v2], 1);
    v1 = arg0->sys;
    new_var2 = 1;
    v1->unk_88D0 = (u16)(new_var2 ^ v1->unk_88D0);
}

void ov14_021F3714(UnkStruct_ov14_boxsys *arg0) {
    SpriteSystem_LoadCharResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_1_9, 0x4F, 1, 1, 0xC11F);
    SpriteSystem_LoadPlttResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_itemtool_itemdata_item_icon, GetItemIndexMapping(0U, 2), 0, 1, 1, 0xC0FC);
    SpriteSystem_LoadCharResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_1_9, 0x4F, 1, 2, 0xC120);
    SpriteSystem_LoadPlttResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_itemtool_itemdata_item_icon, GetItemIndexMapping(0U, 2), 0, 1, 2, 0xC0FD);
    SpriteSystem_LoadCellResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_1_9, 0x50, 1, 0xC0FB);
    SpriteSystem_LoadAnimResObj(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), NARC_a_0_1_9, 0x51, 1, 0xC0FB);
}

extern ManagedSpriteTemplate ov14_021F83E4;

extern ManagedSpriteTemplate ov14_021F8418;

void ov14_021F37F4(UnkStruct_ov14_boxsys *arg0) {
    arg0->sprites[11] = SpriteSystem_NewSprite(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), &ov14_021F83E4);
    ManagedSprite_SetDrawFlag(arg0->sprites[11], 0);
    arg0->sprites[12] = SpriteSystem_NewSpriteWithYOffset(arg0->unk_2F4, ((SpriteManager *)arg0->unk_2F8), &ov14_021F8418, 0x200000);
    ManagedSprite_SetDrawFlag(arg0->sprites[12], 0);
}

void ov14_021F3844(UnkStruct_ov14_boxsys *arg0, u16 itemId) {
    NNSG2dCharacterData *ppCharData;
    u32 palSlotOffset;
    void *ptr;

    ptr = GfGfxLoader_GetCharData(NARC_itemtool_itemdata_item_icon, GetItemIndexMapping(itemId, 1), 0, &ppCharData, HEAP_ID_10);
    ov14_021F2C1C(arg0, 0xB, ppCharData->pRawData, 0x200);
    Heap_Free(ptr);
    palSlotOffset = NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy(arg0->sprites[11]->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
    GfGfxLoader_GXLoadPal(NARC_itemtool_itemdata_item_icon, GetItemIndexMapping(itemId, 2), GF_PAL_LOCATION_MAIN_OBJ, (enum GFPalSlotOffset)palSlotOffset, 0x20U, HEAP_ID_10);
}

void ov14_021F38B0(void *arg0, u16 itemId) {
    NNSG2dCharacterData *ppCharData;
    u32 palSlotOffset;
    void *ptr;

    ptr = GfGfxLoader_GetCharData(NARC_itemtool_itemdata_item_icon, GetItemIndexMapping(itemId, 1), 0, &ppCharData, HEAP_ID_10);
    ov14_021F2C50(arg0, 0xC, ppCharData->pRawData, 0x200);
    Heap_Free(ptr);
    palSlotOffset = NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy(((UnkStruct_ov14_boxsys *)arg0)->sprites[12]->sprite), NNS_G2D_VRAM_TYPE_2DSUB);
    GfGfxLoader_GXLoadPal(NARC_itemtool_itemdata_item_icon, GetItemIndexMapping(itemId, 2), GF_PAL_LOCATION_SUB_OBJ, (enum GFPalSlotOffset)palSlotOffset, 0x20U, HEAP_ID_10);
}

void ov14_021F391C(UnkStruct_ov14_boxsys *arg0, s32 arg1) {
    if (arg1 == 1) {
        ManagedSprite_SetAffineOverwriteMode(arg0->sprites[11], 1U);
        ManagedSprite_SetAffineTranslation(arg0->sprites[11], 0xC, 0xC);
        return;
    }
    ManagedSprite_SetAffineTranslation(arg0->sprites[11], 0, 0);
    ManagedSprite_SetAffineOverwriteMode(arg0->sprites[11], 0U);
}

void ov14_021F395C(void *param_1, s16 param_2, s16 param_3) {
    ManagedSprite_SetPositionXY(((UnkStruct_ov14_boxsys *)param_1)->sprites[11], param_2, param_3);
    return;
}
