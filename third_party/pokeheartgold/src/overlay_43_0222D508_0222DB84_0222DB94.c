#include "global.h"

#include "bg_window.h"
#include "overlay_43_unk_struct_ov64_0222_e21_c.h"

extern s32 ov43_0222EDB8;

extern s32 ov43_0222EDF8;

extern s32 ov43_0222F0C4;

void ov43_0222DB94(s32 *arg0, void *arg1, s16 arg2);

void ov43_0222DB94(s32 *arg0, void *arg1, s16 arg2) {
    u8 *temp;
    u8 x;
    u8 w;

    if (arg0[4] == 1) {
        temp = *((u8 **)&((UnkStruct_ov64_0222E21C *)arg1)->unk_20C);
        CopyToBgTilemapRect(*(BgConfig **)arg1, 2, 0, 0x14, 0x20, 4, temp + 0xC, 0, 0xC, (u8)((u32) * (u16 *)temp >> 3), (u8)((u32) * (u16 *)(temp + 2) >> 3));
    } else {
        temp = *((u8 **)&((UnkStruct_ov64_0222E21C *)arg1)->unk_20C);
        CopyToBgTilemapRect(*(BgConfig **)arg1, 2, 0, 0x14, 0x20, 4, temp + 0xC, 0, 0x14, (u8)((u32) * (u16 *)temp >> 3), (u8)((u32) * (u16 *)(temp + 2) >> 3));
    }
    if (arg2 == 0xFF) {
        ScheduleBgTilemapBufferTransfer(*(BgConfig **)arg1, 2);
        return;
    }
    temp = *((u8 **)&((UnkStruct_ov64_0222E21C *)arg1)->unk_20C);
    x = ((u8 *)&ov43_0222EDB8 + arg0[4] * 8)[arg2];
    w = ((u8 *)&ov43_0222EDF8 + arg0[4] * 8)[arg2];
    CopyToBgTilemapRect(*(BgConfig **)arg1, 2, x, 0x14, w, 4, temp + 0xC, x, *((u8 *)&ov43_0222F0C4 + arg0[4]), (u8)((u32) * (u16 *)temp >> 3), (u8)((u32) * (u16 *)(temp + 2) >> 3));
    ScheduleBgTilemapBufferTransfer(*(BgConfig **)arg1, 2);
}
