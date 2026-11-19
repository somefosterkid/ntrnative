#include "global.h"
#include "error_handling.h"
#include "math_util.h"
#include "pokeathlon/pokeathlon.h"

typedef struct Ov96Pos16 {
    s16 x;
    s16 y;
} Ov96Pos16;

typedef struct Ov96Best {
    u8 idx;
    u8 cnt;
    u8 unk2;
    u8 unk3;
} Ov96Best;

typedef struct Ov96A314Obj {
    PokeathlonCourseData *data;
    u8 state;
    u8 unk5;
    u8 unk6;
    s8 target;
    u32 idx : 2;
    u32 unk8_2 : 2;
    u32 unk8_4 : 1;
    u32 unk8_5 : 1;
    u32 unk8_6 : 26;
} Ov96A314Obj;

VecFx32 *ov96_02215DD4(PokeathlonCourseData *data, u8 idx);
u32 ov96_02215E2C(PokeathlonCourseData *data, u8 idx);
void ov96_02215EE8(PokeathlonCourseData *data, u8 idx, Ov96Pos16 *pos);
void ov96_02215F2C(PokeathlonCourseData *data, u8 idx);
s32 ov96_02215E48(PokeathlonCourseData *data, u8 idx);
s32 ov96_02215DBC(PokeathlonCourseData *data, u8 idx);
BOOL ov96_02215E94(PokeathlonCourseData *data, u8 idx);
void ov96_02215F80(PokeathlonCourseData *data, u8 idx, Ov96Pos16 *pos);
BOOL ov96_0221A034(VecFx32 *a, VecFx32 *b, fx32 dist);
BOOL ov96_0221A05C(VecFx32 *a, VecFx32 *b, u8 c);
extern const u8 ov96_0221D97C[];
extern const u8 ov96_0221D978[];

void ov96_0221A08C(Ov96A314Obj *obj)
{
  u8 hits[4];
  VecFx32 *otherPos;
  u32 kind;
  u8 farCnt;
  u8 nearCnt;
  s8 second;
  s32 minDist;
  s32 r2;
  VecFx32 *myPos;
  s32 i;
  u8 chance;
  s32 r;
  s32 dist;
  s32 st;
  s8 first;
  farCnt = 0;
  nearCnt = 0;
  hits[0] = farCnt;
  hits[3] = farCnt;
  hits[1] = farCnt;
  hits[2] = farCnt;
  myPos = ov96_02215DD4(obj->data, obj->idx);
  kind = ov96_02215E2C(obj->data, obj->idx);
  for (i = 0; i < 4; i++)
  {
    if (i == obj->idx)
    {
      continue;
    }
    otherPos = ov96_02215DD4(obj->data, i);
    if (ov96_0221A034(myPos, otherPos, 0x20000))
    {
      if (ov96_0221A05C(myPos, otherPos, kind))
      {
        hits[i] = 3;
      }
      else
      {
        hits[i] = 1;
      }
      nearCnt++;
    }
    else
      if (ov96_0221A034(myPos, otherPos, 0x30000))
    {
      if (ov96_0221A05C(myPos, otherPos, kind))
      {
        hits[i] = 2;
      }
      farCnt++;
    }
  }

  {
    u8 best[4] = {0};
    chance = 0;
    for (i = 0; i < 4; i++)
    {
      if ((i != obj->idx) && (best[1] < hits[i]))
      {
        best[0] = i;
        best[1] = hits[i];
      }
    }

    r = MTRandom() % 100;
    if (r < (chance + ov96_0221D97C[best[1]]))
    {
      Ov96Pos16 targetPos;
      otherPos = ov96_02215DD4(obj->data, best[0]);
      targetPos.x = otherPos->x >> 12;
      targetPos.y = otherPos->y >> 12;
      ov96_02215EE8(obj->data, obj->idx, &targetPos);
      obj->state = 16;
      return;
    }
    r = MTRandom() % 100;
    if (farCnt != 0)
    {
      chance += 5;
    }
    if (nearCnt != 0)
    {
      chance += ov96_0221D978[nearCnt - 1];
    }
    if (r < chance)
    {
      ov96_02215F2C(obj->data, obj->idx);
      obj->state = 20;
      return;
    }
    r = MTRandom() % 100;
    if (r < 60)
    {
      r2 = MTRandom() % 100;
      if (obj->unk8_5)
      {
        return;
      }
      first = -1;
      second = -1;
      minDist = 999;
      for (i = 0; i < 4; i++)
      {
        dist = ov96_02215E48(obj->data, i);
        st = ov96_02215DBC(obj->data, i);
        if (((((i != obj->idx) && (st != 3)) && (st != 4)) && (!ov96_02215E94(obj->data, obj->target))) && (minDist > dist))
        {
          second = first;
          first = i;
          minDist = dist;
        }
      }

      if (r2 < 60)
      {
        obj->target = first;
      }
      else
        if (r2 < 100)
      {
        obj->target = second;
      }
      else
      {
        (0) ? ((void) 0) : (GF_AssertFail());
      }
      if (obj->target != (-1))
      {
        obj->unk8_5 = 1;
        obj->state = 0;
      }
    }
    else
      if (r < 80)
    {
      obj->state = 10;
    }
    else
      if (r < 100)
    {
      Ov96Pos16 pos;
      pos.x = (MTRandom() % 145) + 56;
      pos.y = (MTRandom() % 80) + 72;
      ov96_02215F80(obj->data, obj->idx, &pos);
      obj->state = 10;
    }
  }
}

