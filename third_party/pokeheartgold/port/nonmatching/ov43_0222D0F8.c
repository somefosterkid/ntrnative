#include "global.h"
#include "bg_window.h"
extern u8 _0222ED5C[];

void ov43_0222D0F8(u32 param_1, BgConfig **param_2, s32 param_3)
{
  u8 buf[2] = {2, 9};
  BgTilemapRectChangePalette(*param_2, 2, 0x11, 0x14, 0xe, 4, buf[param_3]);
  ScheduleBgTilemapBufferTransfer(*param_2, 2);
}
