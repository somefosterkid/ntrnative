#include "fx_util.h"
#include "pokeathlon/pokeathlon.h"
#include "fx_util.h"
#include "pokeathlon/pokeathlon.h"
void ov96_02213558(void *arg0);
void ov96_02213FB4(void *arg0);
s32 ov96_02213728(VecFx32 *a, VecFx32 *b, u32 c, u32 d, VecFx32 *out);
extern const VecFx32 ov96_0221D25C;

void ov96_022124F8(PokeathlonCourseData *param_1) {
  u8 *heap;
  u8 *elem;
  s32 i;
  fx32 mag;
  fx32 speed;
  u32 angle;
  VecFx32 saved[12];
  VecFx32 zero;
  VecFx32 diff;
  VecFx32 dir;
  VecFx32 pos;
  VecFx32 zero2;
  VecFx32 upCopy;
  VecFx32 flat;
  VecFx32 out;
  VecFx32 up;
  VecFx32 *prev;

  heap = PokeathlonCourse_GetHeapAllocPtr4(param_1);
  zero.x = 0;
  zero.y = 0;
  zero.z = 0;
  up = ov96_0221D25C;
  for (i = 0; i < 12; i++) {
    elem = heap + 0x5c + (i / 3) * 0x174 + (i % 3) * 0x7c;
    *(VecFx32 *)(elem + 0x3c) = zero;
    saved[i] = *(VecFx32 *)(elem + 0x30);
    pos = *(VecFx32 *)(elem + 0x30);
    if (*(s32 *)(elem + 0x48) != 0) {
      if ((u32)(*(s32 *)(elem + 0x48) - 2) <= 1 && *(s16 *)(elem + 0x5a) > 0) {
        (*(s16 *)(elem + 0x5a))--;
      }
      if (*(s16 *)(elem + 0x58) == 0 && *(s16 *)(elem + 0x5a) <= 0) {
        *(s32 *)(elem + 0x48) = 0;
        *(s16 *)(elem + 0x5a) = 0;
      } else {
        VEC_MultAdd(*(s16 *)(elem + 0x58) << 12, (VecFx32 *)(elem + 0x4c), &pos, &pos);
        *(VecFx32 *)(elem + 0x30) = pos;
        *(VecFx32 *)(elem + 0x24) = *(VecFx32 *)(elem + 0x30);
        if (*(s16 *)(elem + 0x58) > 0) {
          (*(s16 *)(elem + 0x58))--;
        }
        if (*(s32 *)(elem + 0x78) != 2 && *(s32 *)(elem + 0x48) == 3) {
          ov96_02213FB4(elem);
        }
      }
    }
    if (*(s32 *)(elem + 0x78) == 2 || elem[0x60] != 0) {
      elem[0x60]--;
      if (elem[0x60] == 0) {
        *(s32 *)(elem + 0x78) = 0;
        elem[0x71] = elem[0x70];
      }
    } else if (*(s32 *)(elem + 0x48) == 0) {
      VEC_Subtract((VecFx32 *)(elem + 0x24), &pos, &diff);
      mag = VEC_Mag(&diff);
      if (mag <= 0) {
        ov96_02213558(elem);
      } else {
        VEC_Normalize(&diff, &dir);
        *(VecFx32 *)(elem + 0x3c) = dir;
        if (*(s32 *)(elem + 0x78) == 1) {
          speed = *(fx32 *)(elem + 0x68);
        } else if (*(s32 *)(elem + 0x78) == 3) {
          speed = 0x2000;
        } else {
          speed = *(fx32 *)(elem + 0x64);
        }
        if (mag <= speed) {
          pos = *(VecFx32 *)(elem + 0x24);
        } else {
          zero2.x = 0;
          zero2.y = 0;
          zero2.z = 0;
          VEC_MultAdd(speed, &dir, &zero2, &diff);
          VEC_Add(&diff, &pos, &pos);
        }
        if (mag != 0) {
          upCopy = up;
          flat.x = dir.y;
          flat.z = dir.x;
          flat.y = 0;
          angle = VEC_AngleBetween(&upCopy, &flat);
          if (angle <= 0x2000 || angle >= 0xE000) {
            *(u16 *)(elem + 0x5c) = 4;
          } else if (angle > 0x2000 && angle < 0x6000) {
            *(u16 *)(elem + 0x5c) = 2;
          } else if (angle >= 0x6000 && angle <= 0xA000) {
            *(u16 *)(elem + 0x5c) = 3;
          } else {
            *(u16 *)(elem + 0x5c) = 1;
          }
        }
        *(VecFx32 *)(elem + 0x30) = pos;
      }
    }
  }
  prev = saved;
  for (i = 0; i < 12; i++, prev++) {
    elem = heap + 0x5c + (i / 3) * 0x174 + (i % 3) * 0x7c;
    if (*(s32 *)(elem + 0x30) != saved[i].x || *(s32 *)(elem + 0x34) != saved[i].y) {
      if (ov96_02213728((VecFx32 *)(elem + 0x30), prev, *(u32 *)(elem + 0x20), 1, &out)) {
        *(VecFx32 *)(elem + 0x30) = out;
        *(VecFx32 *)(elem + 0x24) = *(VecFx32 *)(elem + 0x30);
      }
    }
    if (*(s32 *)(elem + 0x30) == *(s32 *)(elem + 0x24) && *(s32 *)(elem + 0x34) == *(s32 *)(elem + 0x28)
        && *(s32 *)(elem + 0x78) != 3 && *(s32 *)(elem + 0x78) != 2) {
      *(s32 *)(elem + 0x78) = 0;
    }
  }
}
