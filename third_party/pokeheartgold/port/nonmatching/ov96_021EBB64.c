#include "error_handling.h"
#include "heap.h"
#include "message_format.h"
#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_save.h"
#include "sound.h"
#include "sound_02004A44.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"
#include "heap.h"
#include "message_format.h"
#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_save.h"
#include "sound.h"
#include "sound_02004A44.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"
s32 ov96_021EC2E0(void *arg0);


void ov96_021ECBFC(s32 arg0);


s32 ov96_021ECC14(s32 arg0);


int ov96_021ED748(s8 *);


int ov96_021ED74C(s8 *);


int ov96_021ED750(s8 *);


void ov96_021EE944(void *arg0);


s32 ov96_021EE97C(void *arg0);


u32 ov96_021EEA80(void *arg0);


s32 ov96_021EC458(s32, s32);


s32 ov96_021ECA70(PokeathlonCourseData *, u8);


s32 ov96_021ECB38(void *, PokeathlonCourseData *, u32, s32, s32);


s32 ov96_021ECC38(s32, u8, u16);


u32 ov96_021ECC4C(s32, u8);


s32 ov96_021ECC7C(s32, void *, s32 *);


s32 ov96_021ED0C8(s32);


s32 ov96_021ED158(s32, s32);


s32 ov96_021ED578(PokeathlonCourseData *, u8, u16);


s32 ov96_021ED5E0(PokeathlonCourseData *);

u16 ov96_021ED6F8(PokeathlonCourseData *, u8);


int ov96_021ED728(PokeathlonCourseData *, u8, u8);


u32 ov96_021ED754(PokeathlonCourseData *);


u32 ov96_021ED78C(PokeathlonCourseData *);


u32 ov96_021ED7C4(PokeathlonCourseData *);


s32 ov96_021ED7FC(PokeathlonCourseData *, s32 *);


u32 ov96_021EDC38(u8, s32);


s32 ov96_021EDCB4(PokeathlonCourseData *);


s32 ov96_021EDDA4(PokeathlonCourseData *, s32 *, s32);


s32 ov96_021EDE64(PokeathlonCourseData *, s32);
s32 ov96_021EDF3C(s32, s32, u32, u32);


s32 ov96_021EE290(void *);


s32 ov96_021EE440(PokeathlonCourseData *);


s32 ov96_021EE580(PokeathlonCourseData *);


s32 ov96_021EE830(void *);


s32 ov96_021EE8CC(void *, s32);


extern s32 ov96_021ED618;


extern s32 ov96_021ED660;


extern s32 ov96_021ED6A8;



s32 ov96_021EBB64(PokeathlonCourseData *arg0)
{
  s32 sp14;
  PokeathlonSave *sp10;
  u32 temp_r0_4;
  u32 sp8;
  u32 spC;
  PokeathlonCourseData *var_r6;
  s32 temp_r0_2;
  s32 var_r5;
  s32 var_r7;
  s8 *temp_r0;
  s8 *var_r4;
  u32 sp4;
  u8 temp_r0_3;
  u8 temp_r1;
  u8 temp_r1_2;
  u8 var_r0;
  var_r6 = arg0;
  temp_r0 = PokeathlonCourse_GetHeapAllocPtr4(arg0);
  var_r4 = temp_r0;
  var_r5 = 0;
  ov96_021ECBFC(*((s32 *) (((u8 *) temp_r0) + 0x8C)));
  ov96_021ED0C8(*((s32 *) (((u8 *) var_r4) + 0x90)));
  if (PokeathlonCourse_GetMode(var_r6) == 1)
  {
    var_r5 = ov96_021EE440(var_r6);
  }
  ov96_021EE830(*((void **) (((u8 *) var_r4) + 0xC)));
  if ((ov96_021EEA80(*((void **) (((u8 *) var_r4) + 0xC))) != 0) || (var_r5 != 0))
  {
    goto end;
  }
  temp_r0_2 = ov96_021EE97C(*((void **) (((u8 *) var_r4) + 0xC)));
  var_r7 = 0;
  temp_r1 = *((u8 *) (((u8 *) var_r4) + 0xB5));
  var_r5 = temp_r0_2;
  var_r7 = -1;
  switch ((u32) temp_r1)
  {
    case 0:
      var_r7 = 3;
      *((u8 *) (((u8 *) var_r4) + 0xB5)) = 1U;
      goto block_81;

    case 1:
      var_r7 = 4;
      *((u8 *) (((u8 *) var_r4) + 0xB5)) = 2U;
      goto block_81;

    case 2:
      var_r5 = 0;
      do
    {
      ov96_021ECC38(*((s32 *) (((u8 *) var_r4) + 0x8C)), (u8) var_r5, ov96_021ED6F8(var_r6, (u8) var_r5));
      var_r5 += 1;
    }
    while (var_r5 < 4);
      *((u8 *) (((u8 *) var_r4) + 0xB5)) = 3U;

    case 3:
      if (((u32) (*((u8 *) (((u8 *) var_r4) + 0xB2)))) >= 4U)
    {
      *((u8 *) (((u8 *) var_r4) + 0xB2)) = 0U;
      var_r7 = 6;
      *((u8 *) (((u8 *) var_r4) + 0xB5)) = 4U;
    }
    else
    {
      var_r7 = 5;
      ov96_021ED578(var_r6, *((u8 *) (((u8 *) var_r4) + 0xB2)), ov96_021ED6F8(var_r6, *((u8 *) (((u8 *) var_r4) + 0xB2))));
      *((u8 *) (((u8 *) var_r4) + 0xB2)) = (u8) ((*((u8 *) (((u8 *) var_r4) + 0xB2))) + 1);
    }
      goto block_81;

    case 4:
      if (ov96_021ED5E0(var_r6) != 0)
    {
      var_r7 = 7;
      *((u8 *) (((u8 *) var_r4) + 0xB5)) = 5U;
    }
    else
    {
      var_r7 = 9;
      *((u8 *) (((u8 *) var_r4) + 0xB5)) = 6U;
    }
      goto block_81;

    case 5:
      if (ov96_021EC2E0(var_r4) == 0)
    {
      if (((u32) (*((u8 *) (((u8 *) var_r4) + 0xB4)))) >= ov96_021ED754(var_r6))
      {
        *((u8 *) (((u8 *) var_r4) + 0xB2)) = 0U;
        *((s8 *) (((u8 *) var_r4) + 0xB3)) = 0;
        *((u8 *) (((u8 *) var_r4) + 0xB4)) = 0U;
        var_r7 = 0xA;
        *((u8 *) (((u8 *) var_r4) + 0xB5)) = 6U;
      }
      else
        if (ov96_021EDDA4(var_r6, &ov96_021ED618, ov96_021ED748(var_r4)) != 0)
      {
        var_r7 = 8;
      }
    }
      goto block_81;

    case 6:
      var_r7 = 0xB;
      *((u8 *) (((u8 *) var_r4) + 0xB5)) = 7U;
      goto block_81;

    case 7:
      if (ov96_021EC2E0(var_r4) == 0)
    {
      if (((u32) (*((u8 *) (((u8 *) var_r4) + 0xB4)))) >= ov96_021ED78C(var_r6))
      {
        *((u8 *) (((u8 *) var_r4) + 0xB2)) = 0U;
        *((s8 *) (((u8 *) var_r4) + 0xB3)) = 0;
        *((u8 *) (((u8 *) var_r4) + 0xB4)) = 0U;
        var_r7 = 0xA;
        *((u8 *) (((u8 *) var_r4) + 0xB5)) = 8U;
      }
      else
        if (ov96_021EDDA4(var_r6, &ov96_021ED660, ov96_021ED74C(var_r4)) != 0)
      {
        var_r7 = 0xC;
      }
    }
      goto block_81;

    case 8:
      if (ov96_021EDCB4(var_r6) != 4)
    {
      var_r7 = 0xE;
      *((u8 *) (((u8 *) var_r4) + 0xB5)) = 9U;
    }
    else
    {
      *((u8 *) (((u8 *) var_r4) + 0xB5)) = 0xBU;
    }
      goto block_81;

    case 9:
      var_r7 = ov96_021EDCB4(var_r6);
      var_r7 += 0xF;
      *((u8 *) (((u8 *) var_r4) + 0xB5)) = 0xAU;
      goto block_81;

    case 10:
      if (ov96_021EC2E0(var_r4) == 0)
    {
      if (((u32) (*((u8 *) (((u8 *) var_r4) + 0xB4)))) >= ov96_021ED7C4(var_r6))
      {
        *((u8 *) (((u8 *) var_r4) + 0xB2)) = 0U;
        *((s8 *) (((u8 *) var_r4) + 0xB3)) = 0;
        *((u8 *) (((u8 *) var_r4) + 0xB4)) = 0U;
        var_r7 = 0x15;
        *((u8 *) (((u8 *) var_r4) + 0xB5)) = 0xBU;
      }
      else
        if (ov96_021EDDA4(var_r6, &ov96_021ED6A8, ov96_021ED750(var_r4)) != 0)
      {
        var_r7 = 0x14;
      }
    }
      goto block_81;

    case 11:
      var_r7 = 0x16;
      *((u8 *) (((u8 *) var_r4) + 0xB5)) = 0xCU;
      StopBGM(GF_GetCurrentPlayingBGM(), 0x10);
      PlaySE(0x6EEU);
      goto block_81;

    case 12:
      if (ov96_021ECC14(*((s32 *) (((u8 *) var_r4) + 0x8C))) != 0)
    {
      if (((*((s32 *) (((u8 *) var_r4) + 0x94)))++) > 0x1E)
      {
        var_r0 = (PokeathlonCourse_GetMode(var_r6) == 0) ? (3) : (4);
        *((s32 *) (((u8 *) var_r4) + 0x94)) = 0;
        temp_r1_2 = *((u8 *) (((u8 *) var_r4) + 0xB4));
        if (((u32) temp_r1_2) >= ((u32) var_r0))
        {
          *((u8 *) (((u8 *) var_r4) + 0xB4)) = 0U;
          *((u8 *) (((u8 *) var_r4) + 0xB5)) = 0xDU;
        }
        else
        {
          var_r7 = 0x17;
          ov96_021EDF3C(var_r5, temp_r1_2 + 1, 1, 0);
          var_r5 = 0;
          do
          {
            ov96_021ECC38(*((s32 *) (((u8 *) var_r4) + 0x8C)), (u8) var_r5, ov96_021ED728(var_r6, (u8) var_r5, *((u8 *) (((u8 *) var_r4) + 0xB4))));
            var_r5 += 1;
          }
          while (var_r5 < 4);
          *((u8 *) (((u8 *) var_r4) + 0xB4)) = (u8) ((*((u8 *) (((u8 *) var_r4) + 0xB4))) + 1);
        }
      }
    }
      goto block_81;

    case 13:
      ov96_021ED7FC(var_r6, &sp14);
      ov96_021ECC7C(*((s32 *) (((u8 *) var_r4) + 0x8C)), var_r4 + 0x9C, &sp14);
      if ((((u32) ((*((s32 *) (((u8 *) var_r4) + 0x9C))) << 8)) >> 0x18) == (((u32) ((*((s32 *) (((u8 *) var_r4) + 0xA0))) << 8)) >> 0x18))
    {
      *((u8 *) (((u8 *) var_r4) + 0xB5)) = 0xEU;
    }
    else
    {
      *((u8 *) (((u8 *) var_r4) + 0xB5)) = 0xFU;
    }
      goto block_81;
      var_r7 = 0x18;

    case 14:
      ov96_021EE290(var_r4 + 0x9C);
      *((u8 *) (((u8 *) var_r4) + 0xB5)) = 0xFU;
      goto block_81;

    case 15:
      var_r7 = 0x19;
      ov96_021EDF3C(var_r5, ov96_021ECC4C(*((s32 *) (((u8 *) var_r4) + 0x8C)), (u8) (((u32) ((*((s32 *) (((u8 *) var_r4) + 0x9C))) * 0x10)) >> 0x1C)), 3, 0);
      ov96_021EDE64(var_r6, *((s32 *) (((u8 *) var_r4) + 0x8C)));
      *((u8 *) (((u8 *) var_r4) + 0xB5)) = 0x10U;
      goto block_81;

    case 16:
      ov96_021EC458(*((s32 *) (((u8 *) var_r4) + 4)), *((s32 *) var_r4));
      *((u8 *) (((u8 *) var_r4) + 0xB5)) = 0x11U;

    case 17:
      var_r7 = 0x1A;
      temp_r0_4 = ((u32) ((*((s32 *) (((u8 *) var_r4) + 0x9C))) * 0x10)) >> 0x1C;
      spC = temp_r0_4;
      BufferPlayersName((MessageFormat *) var_r5, 0U, PokeathlonCourse_GetPlayerProfileFromData(var_r6, (s32) spC));
      ov96_021ECA70(var_r6, (u8) spC);
      ov96_021ED158(*((s32 *) (((u8 *) var_r4) + 0x90)), 1);
      ov96_021ECB38(var_r4 + 0x20, var_r6, spC, 2, *((s32 *) var_r4));
      ManagedSprite_SetAnim(*((ManagedSprite **) (((u8 *) (var_r4 + (temp_r0_4 * 4))) + 0x20)), 1);
      StopSE(0x6EEU, 0);
      PlaySE(0x8E2U);
      PlaySE(0x8E0U);
      PlayBGM(0x476U);
      if ((*((u8 *) (((u8 *) var_r4) + 0xB1))) == 0xA)
    {
      *((u8 *) (((u8 *) var_r4) + 0xB5)) = 0x16U;
    }
    else
    {
      *((u8 *) (((u8 *) var_r4) + 0xB5)) = 0x12U;
    }
      goto block_81;

    case 22:
      var_r7 = 0x1E;
      BufferPokeathlonCourseName((MessageFormat *) temp_r0_2, 0U, (u32) (*((u8 *) (((u8 *) var_r4) + 0xB1))));
      *((u8 *) (((u8 *) var_r4) + 0xB5)) = 0x14U;
      goto block_81;

    case 18:
      var_r7 = 0x1B;
      BufferPlayersName((MessageFormat *) var_r5, 0U, PokeathlonCourse_GetPlayerProfileFromData(var_r6, (s32) (((u32) ((*((s32 *) (((u8 *) var_r4) + 0x9C))) * 0x10)) >> 0x1C)));
      ManagedSprite_SetDrawFlag(*((ManagedSprite **) (((u8 *) var_r4) + 0x80)), 1);
      BufferPokeathlonMedalName((MessageFormat *) var_r5, 1U, ov96_021EDC38(*((u8 *) (((u8 *) var_r4) + 0xB1)), 0));
      if (((u32) (*((u8 *) (((u8 *) var_r4) + 0xB1)))) >= 5U)
    {
      var_r7 = 0x1C;
      ManagedSprite_SetDrawFlag(*((ManagedSprite **) (((u8 *) var_r4) + 0x84)), 1);
      BufferPokeathlonMedalName((MessageFormat *) var_r5, 2U, ov96_021EDC38(*((u8 *) (((u8 *) var_r4) + 0xB1)), 1));
    }
      *((u8 *) (((u8 *) var_r4) + 0xB5)) = 0x13U;
      goto block_81;

    case 19:
      sp8 = (u32) (*((u8 *) (((u8 *) var_r4) + 0xB1)));
      if (PokeathlonCourse_GetMode(var_r6) == 0)
    {
      sp4 = ((u32) ((*((s32 *) (((u8 *) var_r4) + 0x9C))) * 0x10)) >> 0x1C;
      if (sp4 == ov96_021E5F24(var_r6))
      {
        if (sp4 != 0)
        {
          GF_AssertFail();
        }
        sp10 = PokeathlonSave_dummy2(Save_Pokeathlon_Get(PokeathlonCourse_GetSaveData(var_r6)));
        if (((u32) (*((u16 *) (((u8 *) (&sp10->unk_0[0x2C * sp8])) + 6)))) < ov96_021ECC4C(*((s32 *) (((u8 *) var_r4) + 0x8C)), (u8) (((u32) ((*((s32 *) (((u8 *) var_r4) + 0x9C))) * 0x10)) >> 0x1C)))
        {
          var_r7 = 0x1D;
          BufferPlayersName((MessageFormat *) var_r5, 0U, PokeathlonCourse_GetPlayerProfileFromData(var_r6, (s32) (((u32) ((*((s32 *) (((u8 *) var_r4) + 0x9C))) * 0x10)) >> 0x1C)));
          BufferPokeathlonCourseName((MessageFormat *) var_r5, 1U, sp8);
        }
        goto case19_end;
      }
    }
      var_r7 = 0x1E;
      BufferPokeathlonCourseName((MessageFormat *) var_r5, 0U, sp8);
      case19_end:
    *((u8 *) (((u8 *) var_r4) + 0xB5)) = 0x14U;

      goto block_81;

    case 20:
      var_r7 = 0x1F;
      StopSE(0x8E2U, 0x78);
      *((s32 *) (((u8 *) var_r4) + 0xB8)) = 1;
      *((u8 *) (((u8 *) var_r4) + 0xB5)) = 0x15U;
      goto block_81;

    case 21:
      if ((PokeathlonCourse_GetMode(var_r6) == 0) || (ov96_021EE580(var_r6) != 0))
    {
      if (ov96_021EC2E0(var_r4) != 0)
      {
        Heap_Free(*((void **) (((u8 *) var_r4) + 0x88)));
        SysTask_Destroy(*((SysTask **) (((u8 *) var_r4) + 8)));
      }
      ov96_021EE944(*((void **) (((u8 *) var_r4) + 0xC)));
      PokeathlonCourse_SetStateField07(var_r6, 2U);
      return 0;
    }
      goto block_81;

    default:
      GF_AssertFail();
      goto block_81;

  }

  block_81:
  if (var_r7 != (-1))
  {
    ov96_021EE8CC(*((void **) (((u8 *) var_r4) + 0xC)), var_r7);
    temp_r0_3 = *((u8 *) (((u8 *) var_r4) + 0xB7));
    var_r4 += 0xB7;
    *var_r4 = temp_r0_3 + 1;
  }

  end:
  return 0;

}
