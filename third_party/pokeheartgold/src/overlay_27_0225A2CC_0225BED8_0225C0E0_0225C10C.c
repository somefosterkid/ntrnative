#include "global.h"

#include "field_types_def.h"
#include "start_menu.h"
#include "overlay_27_ov27task.h"

extern void *ov27_0225CF94;

extern s32 ov27_0225CFC8;

void ov27_0225C10C(Ov27Task *arg0);

void ov27_0225C10C(Ov27Task *arg0) {
    s32 var_r4;
    u8 temp_r1;
    u8 temp_r1_2;

    var_r4 = 0;
    do {
        temp_r1 = *(u8 *)(var_r4 + (((u8 *)&ov27_0225CFC8) + ((arg0->unk_51C_1) * 8)));
        if (temp_r1 != 0xD) {
            if ((s32)temp_r1 < 7) {
                arg0->unk_514[var_r4] = FieldSystem_ShouldDrawStartMenuIcon(arg0->fieldSystem, (enum StartMenuIcon)temp_r1);
            } else {
                arg0->unk_514[var_r4] = ((u32)(*(u16 *)((u8 *)(((u8 *)&ov27_0225CF94) + (temp_r1 * 4)) + 2) << 0x10) >> 0x1F);
            }
        }
        temp_r1_2 = arg0->unk_514[var_r4];
        arg0->unk_470[var_r4].unk_0 = temp_r1_2;
        var_r4 += 1;
    } while (var_r4 < 7);
}
