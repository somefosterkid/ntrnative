#include "global.h"

#include "list_menu.h"
#include "sprite.h"
#include "overlay_49_unk_struct_ov70_0225_ec20.h"

struct ListMenuItem;

void ov49_0225B070(UnkStruct_ov70_0225EC20 *param0);

void ov49_0225B0D4(void);
void ov49_0225B124(Window *window);
void ov49_0225B0D8(Window *window);

void ov49_0225B070(UnkStruct_ov70_0225EC20 *param0) {
    u16 v0;

    if (param0->unk_3C == 0) {
        Sprite_SetDrawFlag(param0->unk_50[0], FALSE);
        Sprite_SetDrawFlag(param0->unk_50[1], FALSE);
        return;
    }

    ListMenuGetScrollAndRow(param0->unk_30, &v0, NULL);

    if (v0 <= 0) {
        Sprite_SetDrawFlag(param0->unk_50[0], FALSE);
    } else {
        Sprite_SetDrawFlag(param0->unk_50[0], TRUE);
    }

    if (v0 >= (param0->unk_3A - 7)) {
        Sprite_SetDrawFlag(param0->unk_50[1], FALSE);
    } else {
        Sprite_SetDrawFlag(param0->unk_50[1], TRUE);
    }
}

void ov49_0225B0D4(void) {
}

void ov49_0225B0D8(Window *window) {
    ov49_0225B124(window);
}
