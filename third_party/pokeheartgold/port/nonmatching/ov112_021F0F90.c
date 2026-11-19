#include "global.h"
#include "error_handling.h"
#include "gf_rtc.h"
#include "player_data.h"
#include "pokewalker.h"


s32 ov112_021F0F70();

void ov112_021F0F90(u8 *work);
void ov112_021F0F90(u8 *work)
{
  u32 list[24][2] = {{0}};
  u32 minTime;
  u8 *e;
  PlayerProfile *profile;
  s32 i;
  u8 n;
  s32 prevT;
  s32 prevS;
  s32 k;
  s32 t;
  s32 count;
  s32 s;
  minTime = 0xFFFFFFFF;
  if (work == 0L)
  {
    GF_AssertFail();
  }
  if ((*((SaveData ***) work)) == 0L)
  {
    GF_AssertFail();
  }
  Save_Pokewalker_Get(*(*((SaveData ***) work)));
  profile = Save_PlayerData_GetProfile(*(*((SaveData ***) work)));
  *((u32 *) (work + 0xC)) = PlayerProfile_GetTrainerGender(profile);
  *((u32 *) (work + 0x8)) = (u32) PlayerProfile_GetNamePtr(profile);
  for (i = 0; i < 0x18; i++)
  {
    e = (*((u8 **) ((*((u8 **) work)) + 4))) + (i * 0x88);
    if ((((*((u16 *) (e + 0x8))) == 7) || ((*((u16 *) (e + 0x8))) == 8)) && (((*((u16 *) (e + 0xA))) > 0x1ED) || ((*((u16 *) (e + 0xC))) > 0x1ED)))
    {
      continue;
    }
    switch (e[0x84])
    {
      case 0xC:
        e[0x84] = 0xB;
        break;

      case 0xE:
        e[0x84] = 0xD;
        break;

      case 0x1C:
        e[0x84] = 0xC;
        break;

      case 0x1D:
        e[0x84] = 0xE;
        break;

    }

    t = e[0x84];
    if (t >= 0x1C)
    {
      t = 0;
    }
    else
      if ((t >= 1) && (t <= 0xA))
    {
      work[0x143]++;
    }
    else
      if (t == 0xC)
    {
      *((u32 *) (work + 0x144)) |= 0x200;
    }
    else
      if (t == 0xE)
    {
      *((u32 *) (work + 0x144)) |= 0x100;
    }
    else
      if (t == 0xB)
    {
      if (work[0x13F] == 0)
      {
        work[0x141]++;
        if (work[0x141] >= 4)
        {
          work[0x13F] = i;
        }
      }
    }
    else
      if (t == 0xD)
    {
      if (work[0x140] == 0)
      {
        work[0x142]++;
        if (work[0x142] >= 4)
        {
          work[0x140] = i;
        }
      }
    }
    if (t != 0)
    {
      if (e[0x84] == 0)
      {
        GF_AssertFail();
      }
      if (minTime > (*((u32 *) e)))
      {
        minTime = *((u32 *) e);
      }
      list[work[0x13E]][0] = (u32) e;
      list[work[0x13E]][1] = i;
      work[0x13E]++;
    }
  }

  n = work[0x13E];
  for (i = 0; i < n; i++)
  {
    if (((u8 *) list[i][0])[0x84] == 0x19)
    {
      if ((*((u32 *) list[i][0])) > minTime)
      {
        if (minTime == 0)
        {
          GF_AssertFail();
        }
        *((u32 *) list[i][0]) = minTime - 1;
      }
      break;
    }
  }

  MATH_QSort(list, work[0x13E], 8, ov112_021F0F70, 0);
  n = work[0x13E];
  if (n < 2)
  {
    ((u8 *) list[n - 1][0])[0x76] = 0;
    *((u32 *) list[work[0x13E] - 1][0]) = (u32) GF_RTC_DateTimeToSec();
  }
  else
  {
    e = (u8 *) list[n - 2][0];
    ((u8 *) list[n - 1][0])[0x76] = e[0x76];
    if (e[0x84] == 0x1B)
    {
      *((u32 *) list[work[0x13E] - 1][0]) = (*((u32 *) e)) + 0xE10;
    }
    else
    {
      *((u32 *) list[work[0x13E] - 1][0]) = (*((u32 *) e)) + 5;
    }
  }
  prevT = 0;
  prevS = 0;
  for (k = 0; k < work[0x13E]; k++)
  {
    e = (u8 *) list[k][0];
    s = *((u16 *) (e + 0x78));
    t = e[0x84];
    if ((((t == 0x1B) && (prevT == 0x1B)) && (s < 0x12C)) && (prevS < 0x12C))
    {
      e[0x84] = 0;
    }
    prevT = t;
    prevS = s;
  }

  count = 0;
  for (k = 0; k < work[0x13E]; k++)
  {
    e = (u8 *) list[k][0];
    if (e[0x84] != 0)
    {
      *((u32 *) ((work + (count * 4)) + 0xC0)) = (u32) e;
      count++;
    }
  }

  work[0x13E] = count;
}

