#include "global.h"

#include "sprite_system.h"
#include "unk_struct_ov96_0220b148.h"
#include "overlay_96_app_0220a87c.h"

s32 ov96_0220B354(void *);

void ov96_0220A87C(Ov96App_0220A87C *arg0);

void ov96_0220A87C(Ov96App_0220A87C *arg0) {
    s32 var_r4;
    UnkStruct_ov96_0220B148 *temp_r0;

    ManagedSprite_SetAnimateFlag(arg0->unk_10, 0);
    ManagedSprite_SetAnimateFlag(arg0->unk_C, 0);
    var_r4 = 0;
    do {
        temp_r0 = arg0->unk_154[var_r4];
        if ((((u32)(temp_r0->unk_10 * 0x10)) >> 0x1F) != 0) {
            ManagedSprite_SetAnimateFlag(temp_r0->unk_0, 0);
            ManagedSprite_SetAnimateFlag(arg0->unk_154[var_r4]->unk_4, 0);
        }
        var_r4 += 1;
    } while (var_r4 < 0xA);
    ov96_0220B354(arg0->unk_14);
}
