#include "system.h"
#include "sprite.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "sprite.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
long sub_02094A70(void *arg0);
void sub_0209569C(void *arg0);
u8 sub_02095DD8(void *arg0);
s32 sub_02094794(void *, u32);
s32 sub_02094860(void *, u32);
void sub_020948C4(void *, u32, u32);
void sub_020949F4(void *);
void sub_02095238(void *);
s32 sub_02095354(void *);
s32 sub_020954CC(void *);
void sub_02095540(void *);
void sub_020955EC(void *);
void sub_020956B8(void *);
void sub_02095D40(u32, u32, u32);
s32 sub_02095D88(u32, s32, s32);

s32 sub_02093630(void *param_1)
{
  BOOL BVar2;
  u8 uVar1;
  s32 key;
  s8 dx;
  s8 dy;
  s32 res;
  s32 ret;
  VecFx32 mat;
  ret = 0;
  key = 0;
  BVar2 = System_GetTouchHeld();
  if (((BVar2 == 0) && ((*((s32 *) (((u8 *) param_1) + 0x46c0))) == 0)) && ((key = gSystem.newKeys) != 0))
  {
    dx = 0;
    dy = 0;
    uVar1 = sub_02095DD8(*((void **) (((u8 *) param_1) + 0x46B8)));
    switch (key)
    {
      case 0x20:
        if (uVar1 == 5)
      {
        sub_02095540(param_1);
      }
      else
      {
        dx--;
      }
        break;

      case 0x10:
        if (uVar1 == 5)
      {
        sub_020955EC(param_1);
      }
      else
      {
        dx++;
      }
        break;

      case 0x40:
        dy--;
        break;

      case 0x80:
        dy++;
        break;

      case 1:
        ret = sub_02095354(param_1);
        if ((ret != 0) && (ret != 3))
      {
        PlaySE(0x5dd);
      }
        break;

      case 2:
        ret = sub_020954CC(param_1);
        if ((ret != 0) && (ret != 3))
      {
        PlaySE(0x5dd);
      }
        break;

    }

    if ((dx != 0) || (dy != 0))
    {
      res = sub_02095D88(*((u32 *) (((u8 *) param_1) + 0x46B8)), dx, dy);
      sub_02095238(param_1);
      if (res != 0)
      {
        PlaySE(0x5dc);
      }
    }
  }
  if ((key == 0) && ((*((s32 *) (((u8 *) param_1) + 0x46bc))) == 0))
  {
    if ((System_GetTouchNew() != 0) && ((*((s32 *) (((u8 *) param_1) + 0x4680))) == 0))
    {
      *((u32 *) (((u8 *) param_1) + 0x46c4)) = 1;
      key = TouchscreenHitbox_FindRectAtTouchNew(*((TouchscreenHitbox **) (((u8 *) param_1) + 0x7e4)));
      if (key != (-1))
      {
        if (key < 0x1e)
        {
          if (((*((u8 *) (((u8 *) param_1) + 0xf))) != 0x12) || (key < 6))
          {
            sub_02095D40(*((u32 *) (((u8 *) param_1) + 0x46B8)), 2, key & 0xff);
            if (sub_02094794(param_1, key) == 1)
            {
              PlaySE(0x5eb);
              sub_020948C4(param_1, 1, key);
              ret = 1;
            }
          }
        }
        else
          if ((key >= 0x21) && (key < ((s32) ((*((u8 *) (((u8 *) param_1) + 0xd))) + 0x21))))
        {
          sub_02095D40(*((u32 *) (((u8 *) param_1) + 0x46B8)), 3, (key - 0x21) & 0xff);
          if (sub_02094860(param_1, key - 0x21) == 1)
          {
            PlaySE(0x5eb);
            key -= 0x21;
            sub_020948C4(param_1, 2, key);
            ret = 1;
          }
        }
        else
        {
          switch (key)
          {
            case 0x1f:
              sub_02095540(param_1);
              break;

            case 0x20:
              sub_020955EC(param_1);
              break;

            case 0x1e:
              if ((*((u8 *) (((u8 *) param_1) + 0x13))) != 0)
            {
              mat.x = 0xe0000;
              mat.y = 0xb0000;
              mat.z = 0;
              Sprite_SetMatrix(*((Sprite **) (((u8 *) param_1) + 0x8c0)), &mat);
              sub_02095D40(*((u32 *) (((u8 *) param_1) + 0x46B8)), 1, 0);
              sub_0209569C(param_1);
              ret = 3;
            }
              break;

          }

        }
        sub_020956B8(param_1);
      }
      *((u32 *) (((u8 *) param_1) + 0x46c0)) = 1;
    }
    else
      if ((System_GetTouchHeld() != 0) && ((*((s32 *) (((u8 *) param_1) + 0x46c4))) != 0))
    {
      sub_020949F4(param_1);
      *((u32 *) (((u8 *) param_1) + 0x46c0)) = 1;
    }
    else
    {
      sub_02094A70(param_1);
      *((u32 *) (((u8 *) param_1) + 0x46c0)) = 0;
      *((u32 *) (((u8 *) param_1) + 0x46c4)) = 0;
    }
  }
  if ((*((u8 *) (((u8 *) param_1) + 0xe))) == (*((u8 *) (((u8 *) param_1) + 0xd))))
  {
    *((u32 *) (((u8 *) param_1) + 0x14)) = 1;
  }
  return ret;
}

