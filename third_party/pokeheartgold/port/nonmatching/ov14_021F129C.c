/* HDR-OVERLAY {"GridInputHandler_SetNextLastUnk0FInputs": {"file": "include/unk_02019BA4.h", "new": "void GridInputHandler_SetNextLastUnk0FInputs(GridInputHandler *inputHandler, int nextInput, u8 lastInput, u8 unk0F);", "old": "void GridInputHandler_SetNextLastUnk0FInputs(GridInputHandler *inputHandler, int nextInput, int lastInput, int unk0F);"}} */
#define GridInputHandler_SetNextLastUnk0FInputs GridInputHandler_SetNextLastUnk0FInputs__hdr
/* HDR-OVERLAY defines done */
#include "sprite_system.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
/* HDR-OVERLAY end */
#undef GridInputHandler_SetNextLastUnk0FInputs
void GridInputHandler_SetNextLastUnk0FInputs(GridInputHandler *inputHandler, int nextInput, u8 lastInput, u8 unk0F);
/* HDR-OVERLAY done */
s32 ov14_021F29E4(void *, s32, s32);
s32 ov14_021F2A18(void *, s32, s32);
s32 ov14_021F459C(void *, s32);
s32 ov14_021F58B8(void *);

void ov14_021F129C(void *arg0, s32 arg1)
{
  u8 sp0;
  u32 temp_r5;
  s32 val;
  void **new_var;
  void *temp_r0;
  void *temp_r2;
  temp_r2 = *((void **) (((u8 *) arg0) + 0x34));
  temp_r5 = *((u8 *) (((u8 *) temp_r2) + 0x44D));
  temp_r5 = (u32) temp_r5 >> 2;
  temp_r5 = temp_r5 << 2;
  val = arg1 + temp_r5;
  *((u8 *) (((u8 *) temp_r2) + 0x44D)) = val;
  ov14_021F459C(arg0, val);
  ov14_021F58B8(arg0);
  *((s32 *) (((u8 *) (*((void **) (((u8 *) arg0) + 0x34)))) + 0x43C)) = GridInputHandler_GetNextInput(*((GridInputHandler **) (((u8 *) (*((void **) (((u8 *) arg0) + 0x34)))) + 0x2C)));
  temp_r0 = *((void **) (((u8 *) arg0) + 0x34));
  new_var = (void **) (((u8 *) arg0) + 0x34);
  temp_r0 = *new_var;
  temp_r5 = *((u32 *) (((u8 *) temp_r0) + 0x43C));
  GridInputHandler_SetNextLastUnk0FInputs(*((GridInputHandler **) (((u8 *) temp_r0) + 0x2C)), 6, temp_r5, temp_r5);
  DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(*((GridInputHandler **) (((u8 *) (*new_var)) + 0x2C)), 6), (&sp0) + 1, &sp0);
  ManagedSprite_SetPositionXY(*((ManagedSprite **) (((u8 *) (*new_var)) + 0x320)), (*((u8 *) (((u8 *) (&sp0)) + 1))), (*((u8 *) (&sp0))));
  ov14_021F29E4(*((void **) (0x34 + ((u8 *) arg0))), 9, 8);
  ov14_021F2A18(*new_var, 9, 1);
}

