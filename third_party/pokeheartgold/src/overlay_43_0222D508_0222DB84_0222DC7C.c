#include "global.h"

#include "bg_window.h"
#include "overlay_43_unk_struct_ov64_0222_e21_c.h"

void ov43_0222DC7C(void *param_1, void *param_2);

void ov43_0222DC7C(void *param_1, void *param_2) {
    u16 *puVar1;

    puVar1 = *((u16 **)&((UnkStruct_ov64_0222E21C *)param_2)->unk_20C);
    CopyToBgTilemapRect(*(BgConfig **)param_2, 2, 0, 0x14, 0x20, 4, puVar1 + 6, 0, 0x1c, (u8)((u32)puVar1[0] >> 3), (u8)((u32)puVar1[1] >> 3));
    ScheduleBgTilemapBufferTransfer(*(BgConfig **)param_2, 2);
}
