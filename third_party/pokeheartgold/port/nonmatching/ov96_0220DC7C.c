/* HDR-OVERLAY {"ManagedSprite_SetPositionXYWithSubscreenOffset": {"file": "include/sprite_system.h", "new": "void ManagedSprite_SetPositionXYWithSubscreenOffset(ManagedSprite *managedSprite, s16 x, u32 y, fx32 yOffset);", "old": "void ManagedSprite_SetPositionXYWithSubscreenOffset(ManagedSprite *managedSprite, s16 x, s16 y, fx32 yOffset);"}} */
#define ManagedSprite_SetPositionXYWithSubscreenOffset ManagedSprite_SetPositionXYWithSubscreenOffset__hdr
/* HDR-OVERLAY defines done */
#include "math_util.h"
#include "pokeathlon/pokeathlon.h"
#include "sprite_system.h"
#include "unk_02005D10.h"
#include "math_util.h"
#include "pokeathlon/pokeathlon.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02005D10.h"
/* HDR-OVERLAY end */
#undef ManagedSprite_SetPositionXYWithSubscreenOffset
void ManagedSprite_SetPositionXYWithSubscreenOffset(ManagedSprite *managedSprite, s16 x, u32 y, fx32 yOffset);
/* HDR-OVERLAY done */
u32 _fadd(u32, u32);
u32 _fdiv(u32, u32);
u32 _fflt(s32);
u32 _s32_div_f(s32, u32);
void ov96_0220D428(s32 * param_1, u8 param_2, u8 param_3);
void ov96_0220DB3C(void * param_1, s32 param_2, s32 param_3);
void ov96_0220DBCC(void * param_1, f32 param_2);
void ov96_0220DBE8(void * param_1, s32 param_2, s32 param_3, s16 param_4);
void ov96_0220DC38(u32 param_1);
s16 ov96_0220E6DC(s16 param_1, u32 param_2);
typedef struct { /* invented */
  u16 a:7;
  u16 b:4;
  u16 kind:3;
  u16 d:2;
} UnkDC7CBits;
typedef struct { /* invented */
  ManagedSprite *sprites[6];
  u32 angle:16;
  u32 state:8;
  u32 unk:8;
} UnkDC7CObj;
void ov96_0220DC7C(s32 param_1, UnkDC7CBits *param_2, PokeathlonCourseData *param_3, s32 arg3) {
  u8 uVar2;
  void *pvVar3;
  UnkDC7CObj *obj;
  s32 iStack_20;

  uVar2 = ov96_021E5F24(param_3);
  pvVar3 = PokeathlonCourse_GetHeapAllocPtr4(param_3);
  iStack_20 = 0;
  obj = (UnkDC7CObj *)(param_1 + 4);
  do {
    s16 xa;
    u32 x;
    u32 uVar12;
    s16 iVar5;
    xa = (param_2->a * 8) / 2;
    if (param_2->kind == 2) {
      xa = (s16)(xa + 0x1e0);
    }
    x = xa;
    uVar12 = xa >= 0x120;
    if (uVar12 != 0) {
      x = (s16)(x - 0x120);
    }
    iVar5 = ov96_0220E6DC(xa, param_2->b);
    if (param_2->kind == 0) {
      ov96_0220DB3C(obj, 2, 0);
      ManagedSprite_SetDrawFlag(obj->sprites[4], 0);
      ManagedSprite_SetDrawFlag(obj->sprites[5], 0);
    } else if (param_2->kind >= 3 && obj->state < 3) {
      ov96_0220DB3C(obj, 2, 0);
      ov96_0220DBE8(obj, param_2->kind, iVar5, (s16)(x - 0x28));
      if (param_2->d == uVar2) {
        ov96_0220D428((s32 *)((u8 *)pvVar3 + 0x6a8), 4, 4);
        ov96_0220DC38(param_2->kind);
      } else {
        ManagedSprite_SetDrawFlag(obj->sprites[4], 0);
      }
      PlaySE(0x8c4);
    } else if (param_2->kind == 1) {
      ov96_0220DB3C(obj, uVar12, 1);
      ManagedSprite_SetDrawFlag(obj->sprites[4], 0);
      ManagedSprite_SetDrawFlag(obj->sprites[5], 0);
    }
    if (obj->state == 0 && param_2->kind == 1) {
      obj->angle = 0;
      ov96_0220DBCC(obj, 1.2f);
      PlaySE(0x8c3);
    }
    if (param_2->kind == 1 || param_2->kind == 2) {
      if (uVar12 != 0 && iVar5 > 0x10) {
        ManagedSprite_SetAnimNoRestart(obj->sprites[uVar12 + 2], 1);
      }
      {
        f32 fVar9 = 1.2f + (f32)(GF_SinDeg((u16)obj->angle) / 18) / 4096.0f;
        obj->angle += 10;
        ov96_0220DBCC(obj, fVar9);
      }
      ManagedSprite_SetPositionXYWithSubscreenOffset(obj->sprites[uVar12], iVar5, (s16)(x - 0x28), FX32_CONST(480));
      ManagedSprite_SetPositionXYWithSubscreenOffset(obj->sprites[uVar12 + 2], iVar5, x, FX32_CONST(480));
    }
    obj->state = param_2->kind;
    param_2++;
    obj++;
    iStack_20++;
  } while (iStack_20 < 2);
}
