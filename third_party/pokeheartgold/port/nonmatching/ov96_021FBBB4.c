/* PORTOK hk42 */
#include "error_handling.h"
#include "math_util.h"
#include "pokeathlon/pokeathlon.h"

u8 *ov96_021E60D8(PokeathlonCourseData *data, int participant, int slot);

typedef struct UnkStruct_ov96_021FBBB4_Entry {
    u8 unk00[3];
    u8 unk03;
    u8 unk04;
    u8 unk05[0xB];
    f32 unk10;
    f32 unk14;
    u8 unk18[0x10];
} UnkStruct_ov96_021FBBB4_Entry;

void ov96_021FBBB4(PokeathlonCourseData *data, s32 *stats, int unused2, int unused3)
{
  u8 *work;
  u8 count;
  u8 i;
  u8 j;
  unsigned short base;
  u8 *mon;
  UnkStruct_ov96_021FBBB4_Entry *entry;
  work = PokeathlonCourse_GetHeapAllocPtr4(data);
  count = PokeathlonCourse_GetParticipantCount(data);
  for (i = count; i < 4u; i++)
  {
    base = (i - count) * 3;
    for (j = 0; j < 3u; j++)
    {
      entry = &((UnkStruct_ov96_021FBBB4_Entry *) (work + 0x238))[(u8) (j + base)];
      mon = ov96_021E60D8(data, i, j);
      entry->unk10 = ((f32) stats[mon[1] + 15]) / 10.0f;
      entry->unk14 = ((f32) stats[mon[4] + 10]) / 100.0f;
      switch (LCRandom() % 6)
      {
        case 0:
          entry->unk03 = 10;
          entry->unk04 = 50;
          break;

        case 1:
          entry->unk03 = 20;
          entry->unk04 = 50;
          break;

        case 2:
          entry->unk03 = 30;
          entry->unk04 = 60;
          break;

        case 3:
          entry->unk03 = 20;
          entry->unk04 = 70;
          break;

        case 4:
          entry->unk03 = 30;
          entry->unk04 = 70;
          break;

        case 5:
          entry->unk03 = 40;
          entry->unk04 = 80;
          break;

        default:
          (0) ? ((void) 0) : (GF_AssertFail());
          break;

      }

    }

  }

}

