#include "global.h"

#include "bg_window.h"
#include "filesystem.h"

void ov01_021F0454(NARC *param0, u32 param1, u32 param2, u32 param3, u32 param4, u32 param5, BgConfig *param6, u32 param7);

void ov01_021F0500(NARC *arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, BgConfig *arg6, u32 arg7, u32 arg8);

void ov01_021F0500(NARC *arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, BgConfig *arg6, u32 arg7, u32 arg8) {
    u16 *buf;
    s32 y;
    s32 x;
    u16 a;
    u16 b;
    u16 tmp;

    ov01_021F0454(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    ov01_021F0454(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg8);
    buf = GetBgTilemapBuffer(arg6, (u8)arg8);
    for (y = 0; y < 24; y++) {
        for (x = 0; x < 8; x++) {
            a = (u16)(y * 16 + x);
            b = (u16)(y * 16 + 15 - x);
            tmp = buf[a];
            buf[a] = buf[b];
            buf[b] = tmp;
            buf[a] = (buf[a] & 0xFBFF) | ((1 ^ (buf[a] >> 10)) << 10);
            buf[b] = (buf[b] & 0xFBFF) | ((1 ^ (buf[b] >> 10)) << 10);
        }
    }
    ScheduleBgTilemapBufferTransfer(arg6, (u8)arg8);
}
