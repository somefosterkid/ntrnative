#include "global.h"

#include "list_menu.h"
#include "sprite.h"
#include "unk_02005D10.h"
#include "unk_0200A090.h"
#include "bg_window.h"
#include "render_window.h"
#include "overlay_49_unk_struct_ov70_0225_ec20.h"

struct ListMenuItem;

void ov49_0225B070(UnkStruct_ov70_0225EC20 *param0);

u32 ov49_0225AFD8(UnkStruct_ov70_0225EC20 *param0);

void ov49_0225B014(UnkStruct_ov70_0225EC20 *arg0, u16 *arg1, u16 *arg2);

u32 ov49_0225AFD8(UnkStruct_ov70_0225EC20 *param0) {
    u32 v0;

    if (param0->unk_30 == 0L) {
        return 0xfffffffe;
    }

    v0 = ListMenu_ProcessInput(param0->unk_30);

    switch (v0) {
    case 0xffffffff:
    case 0xfffffffe:
        ov49_0225B070(param0);
        break;
    default:
        PlaySE(1500);
        break;
    }

    return v0;
}

void ov49_0225B014(UnkStruct_ov70_0225EC20 *arg0, u16 *cursorPos, u16 *itemsAbove) {
    ListMenu *list;
    s32 v0;
    UnkStruct_ov70_0225EC20 *v1;

    v1 = arg0;
    list = v1->unk_30;
    if (list != NULL) {
        DestroyListMenu(list, cursorPos, itemsAbove);
        v1->unk_30 = NULL;
        ClearWindowPixelBuffer(&v1->unk_20, 1);
        ClearWindowTilemapAndScheduleTransfer(&v1->unk_20);
        RemoveWindow(&v1->unk_20);
        v0 = 0;
        v1->unk_3C = 0;
        do {
            Sprite_SetDrawFlag(arg0->unk_50[v0], 0);
            v0 += 1;
        } while (v0 < 2);
    }
}
