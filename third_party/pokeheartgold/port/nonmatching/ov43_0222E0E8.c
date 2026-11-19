#include "unk_02030A98.h"
#include "save.h"
#include "save_frontier.h"
#include "unk_02030A98.h"
s32 ov43_0222ABFC(void *, u16);
s32 ov43_0222DEF8(s32, s32);
s32 ov43_0222EC58(s32, s32, s32, void *, void *, s32, s32, s32, s32, s32, s32, s32);

void ov43_0222E0E8(s32 arg0, void *arg1, void *arg2)
{
  FrontierSave *temp_r7;
  temp_r7 = Save_Frontier_GetStatic(*((SaveData **) (((u8 *) arg1) + 4)));
  ov43_0222EC58(arg0, 2, 0, arg1, arg2, 0x1D, 0, 0, 0x10200, *((s32 *) (((u8 *) arg2) + 0x7C)), *((s32 *) (((u8 *) arg2) + 0x80)), 0);
  ov43_0222EC58(arg0, 2, 1, arg1, arg2, FrontierSave_GetStat(temp_r7, 0x64, (s32) (*((u8 *) (((u8 *) (arg1 + (*((s8 *) (((u8 *) arg1) + 0xB))))) + 0x18)))) + 0x22, 0, 0, 0x10200, *((s32 *) (((u8 *) arg2) + 0x7C)), *((s32 *) (0x80 + ((u8 *) arg2))), 0);
  ov43_0222ABFC(arg2, FrontierSave_GetStat(temp_r7, 0x71, (s32) (*((u8 *) (((u8 *) (arg1 + (*((s8 *) (0xB + ((u8 *) arg1)))))) + 0x18)))));
  ov43_0222EC58(arg0, 2, 1, arg1, arg2, 0x25, 0x70, 0, 0x10200, *((s32 *) (((u8 *) arg2) + 0x7C)), *((s32 *) (((u8 *) arg2) + 0x80)), 1);
  ov43_0222EC58(arg0, 2, 1, arg1, arg2, 0x24, 0, 0x18, 0x10200, *((s32 *) (((u8 *) arg2) + 0x7C)), *((s32 *) (((u8 *) arg2) + 0x80)), 0);
  ov43_0222ABFC(arg2, FrontierSave_GetStat(temp_r7, 0x70, (s32) (*((u8 *) (((u8 *) (arg1 + (*((s8 *) (((u8 *) arg1) + 0xB))))) + 0x18)))));
  ov43_0222EC58(arg0, 2, 1, arg1, arg2, 0x25, 0x70, 0x18, 0x10200, *((s32 *) (((u8 *) arg2) + 0x7C)), *((s32 *) (((u8 *) arg2) + 0x80)), 1);
  ov43_0222DEF8(arg0, 2);
}


