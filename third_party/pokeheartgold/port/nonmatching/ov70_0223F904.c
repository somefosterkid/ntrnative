#include "list_menu_items.h"
u32 ov70_0223F8D0(s32, s32 *, s32, s32);
extern s32 ov70_02245B5C;

u32 ov70_0223F904(ListMenuItem **arg0, MsgData *arg1, s32 arg2, s32 arg3)
{
  int new_var;
  s32 sp4;
  MsgData *sp0;
  s32 temp_r3;
  s32 var_r4;
  u32 temp_r0;
  sp0 = arg1;
  temp_r0 = ov70_0223F8D0(arg2, &sp4, arg2, arg3);
  *arg0 = ListMenuItems_New((unsigned int) temp_r0, HEAP_ID_61);
  var_r4 = 0;
  loop_2:
  if (var_r4 < ((s32) temp_r0))
  {
    temp_r3 = sp4 + var_r4;
    new_var = sp4;
    ListMenuItems_AppendFromMsgData(*arg0, sp0, (s32) (*((u16 *) (((u8 *)&ov70_02245B5C) + ((new_var + var_r4) * 2)))), temp_r3 + 1);
    var_r4 += 1;
    goto loop_2;
  }

  return temp_r0;
}

