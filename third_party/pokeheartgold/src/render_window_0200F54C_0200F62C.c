#include "global.h"

#include "field/field_sprite_manager.h"

void sub_0200F62C(FieldSpriteManager *arg0);

void sub_0200F62C(FieldSpriteManager *fieldSpriteManager) {
    FieldSpriteManager_AddPlttRes(fieldSpriteManager, NARC_a_0_3_8, 0x32, 0, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 0x15CD5);
    FieldSpriteManager_AddCellRes(fieldSpriteManager, NARC_a_0_3_8, 0x30, 0, 0x15CD5);
    FieldSpriteManager_AddAnimRes(fieldSpriteManager, NARC_a_0_3_8, 0x2F, 0, 0x15CD5);
    FieldSpriteManager_AddCharRes(fieldSpriteManager, NARC_a_0_3_8, 0x31, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 0x15CD5);
}
