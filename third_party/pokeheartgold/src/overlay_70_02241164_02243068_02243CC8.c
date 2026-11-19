#include "global.h"

#include "list_menu_items.h"
#include "sprite.h"
#include "unk_struct_ov70_02242e58.h"

s32 ov70_02241DB4(void *, s32);

s16 ov70_02243CC8(UnkStruct_ov70_02242E58 *arg0);

s16 ov70_02243CC8(UnkStruct_ov70_02242E58 *arg0) {
    Sprite *sprite;

    ov70_02241DB4(arg0, 3);
    sprite = arg0->unk_18;
    if (sprite != NULL) {
        Sprite_SetDrawFlag(sprite, 1);
    }
    ListMenuItems_Delete(arg0->unk_34);
    return arg0->unk_42;
}
