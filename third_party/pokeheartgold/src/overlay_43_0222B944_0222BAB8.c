#include "global.h"

#include "bg_window.h"
#include "list_menu_items.h"
#include "overlay_42.h"
#include "pm_string.h"
#include "sprite.h"
#include "overlay_43_unk_struct_ov64_0222_f0_c4.h"
#include "overlay_43_palpad.h"

s32 ov43_0222C7EC(void *, void *);

void ov43_0222BAB8(void *arg0, void *arg1, void *arg2);

void ov43_0222BAB8(void *arg0, void *arg1, void *arg2) {
    s32 v1;
    s32 v3;
    void *v2;

    Sprite_Delete(((PalPadRoster *)arg0)->deleteEffect);
    RemoveWindow(arg0 + 0xC8);
    String_Delete(((PalPadRoster *)arg0)->msgText);
    v3 = 0;
    do {
        ListMenuItems_Delete(((ListMenuItem *)((PalPadRoster *)arg0)->menuItems[v3]));
        v3 += 1;
    } while (v3 < 2);
    RemoveWindow(arg0 + 0xB8);
    ov42_02227274(((UnkStruct_ov42_02227060 *)((PalPadRoster *)arg0)->trainerIcons));
    ov42_02227114(((UnkStruct_ov42_02227060 *)((PalPadRoster *)arg0)->trainerIcons));
    v1 = 0;
    v2 = arg0 + 8;
    do {
        ov43_0222C7EC(v2, arg2);
        v1 += 1;
        v2 += 0x38;
    } while (v1 < 3);
}
