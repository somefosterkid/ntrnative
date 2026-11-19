#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "overlay_96_unk_struct_ov96_021_f5018.h"

void ov96_021F6E68(UnkStruct_ov96_021F5018 *param0, int param1);

void ov96_021F6E68(UnkStruct_ov96_021F5018 *param0, int param1) {
    u8 v2[3];
    u8 v0[3];
    u8 v1;
    u8 i;

    v1 = param1 / 100;
    v2[0] = (v1 % 5) * 4;
    v0[0] = (v1 / 5) * 8;
    v1 = (param1 - v1 * 100) / 10;
    v2[1] = (v1 % 5) * 4;
    v0[1] = (v1 / 5) * 8;
    v1 = param1 % 10;
    v2[2] = (v1 % 5) * 4;
    v0[2] = (v1 / 5) * 8;

    for (i = 0; i < 3; i++) {
        CopyRectToBgTilemapRect(param0->bgConfig, 4, i * 4 + 1, 0, 4, 8, param0->unk_5C->rawData, v2[i], v0[i], 20, 16);
    }

    ScheduleBgTilemapBufferTransfer(param0->bgConfig, 4);
}
