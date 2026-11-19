#include "global.h"

#include "bg_window.h"
#include "field_types_def.h"
#include "sprite.h"
#include "start_menu.h"
#include "overlay_27_ov27task.h"

void ov27_0225BC84(Ov27Task *arg0);

void ov27_0225BC84(Ov27Task *arg0) {
    s32 sp4[8];
    enum StartMenuIcon var_r4;
    s32 *var_r4_2;
    Window *var_r5_2;
    s32 *var_r5;
    s32 temp_r0;
    s32 var_r6;
    s32 var_r7;
    var_r7 = 0;
    var_r4 = START_MENU_ICON_POKEDEX;
    var_r5 = sp4;
    do {
        temp_r0 = FieldSystem_ShouldDrawStartMenuIcon(arg0->fieldSystem, var_r4);
        var_r4 += 1;
        *var_r5 = temp_r0;
        var_r5 += 1;
    } while (var_r4 < 8);
    var_r6 = 0;
    var_r4_2 = sp4;
    var_r5_2 = arg0->unk_3F0;
    do {
        if ((*var_r4_2) != 0) {
            CopyWindowToVram(var_r5_2);
            var_r7 += 1;
        }
        var_r4_2 += 1;
        var_r6 += 1;
        var_r5_2++;
    } while (var_r6 < 8);
    if (var_r7) {
        Sprite_SetDrawFlag(arg0->unk_390[14], 1);
        CopyWindowToVram(&arg0->unk_3E0);
    }
}
