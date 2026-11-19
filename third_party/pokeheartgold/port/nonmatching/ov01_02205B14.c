#include "follow_mon.h"
#include "heap.h"
#include "map_object.h"
#include "overlay_01.h"
#include "overlay_01_022031C0.h"
#include "task.h"
#include "unk_02062108.h"
#include "field_types_def.h"
#include "follow_mon.h"
#include "heap.h"
#include "map_object.h"
#include "overlay_01.h"
#include "task.h"
#include "unk_02062108.h"
u32 ov01_021F771C(MapObjectManager *);
u32 ov01_02205CF0(FieldSystem * param_1, s32 param_2);
void sub_02023E78(u32, VecFx32 *);
#ifdef __MWERKS__
static u32 ov01_022096E0;  /* judge stand-in (staticfit) */
#else
extern u8 ov01_022096E0[16];  /* the .rodata table of asm/overlay_01_022053EC.s, not a zeroed copy */
#endif
extern u32 ov01_022096F0;
extern u32 ov01_02209704;

typedef struct S8x8 { s8 v[8]; } S8x8;
typedef struct Words2 { u32 w[2]; } Words2;
u32 ov01_02205B14(TaskManager *param_1) {
  u8 bVar1;
  FieldSystem *fieldSystem;
  u8 *ptr;
  BOOL BVar4;
  LocalMapObject *pLVar5;
  u32 uVar6;
  u32 iVar7;
  VecFx32 VStack_1c;
  VecFx32 VStack_18;
  u32 auStack_30 [2];
  s8 acStack_38 [8];
  s8 acStack_40 [8];

  fieldSystem = TaskManager_GetFieldSystem(param_1);
  ptr = TaskManager_GetEnvironment(param_1);
  switch(*ptr) {
  case 0:
    BVar4 = FollowMon_IsActive(fieldSystem);
    if (BVar4 == 0) {
      Heap_Free(ptr);
      return 1;
    }
    BVar4 = ov01_022057C4(fieldSystem);
    if (BVar4 != 0) {
      Heap_Free(ptr);
      return 1;
    }
    pLVar5 = FollowMon_GetMapObject(fieldSystem);
    MapObject_UnpauseMovement(pLVar5);
    *ptr = *ptr + 1;
  case 1:
    pLVar5 = FollowMon_GetMapObject(fieldSystem);
    BVar4 = MapObject_IsMovementPaused(pLVar5);
    if (BVar4 != 0) {
      uVar6 = ov01_02205CF0(fieldSystem,(s32)ptr);
      *ptr = (s8)uVar6;
    }
    break;
  case 2:
    *(Words2 *)((u8 *)auStack_30) = *(Words2 *)((u8 *)&ov01_022096E0 + 0x8);
    pLVar5 = FollowMon_GetMapObject(fieldSystem);
    BVar4 = MapObject_IsMovementPaused(pLVar5);
    if (BVar4 != 0) {
      bVar1 = ptr[3];
      ptr[3] = bVar1 + 1;
      MapObject_SetHeldMovement(pLVar5,auStack_30[bVar1]);
      if ((u8)ptr[3] >= 2) {
        *ptr = *ptr + 1;
      }
    }
    break;
  case 3:
    pLVar5 = FollowMon_GetMapObject(fieldSystem);
    BVar4 = MapObject_IsMovementPaused(pLVar5);
    if (BVar4 != 0) {
      MapObject_SetHeldMovement(pLVar5,0);
      *ptr = *ptr + 1;
    }
    break;
  case 4:
    *(S8x8 *)acStack_38 = *(S8x8 *)&ov01_022096F0;
    *(S8x8 *)acStack_40 = *(S8x8 *)&ov01_022096E0;
    pLVar5 = FollowMon_GetMapObject(fieldSystem);
    iVar7 = 0x2000;
    if (*(s32 *)(ptr + 0x44) == 0) {
      iVar7 = iVar7 * -1;
    }
    MapObject_CopyPositionVector(pLVar5,&VStack_1c);
    VStack_1c.z = VStack_1c.z - acStack_38[(u8)ptr[1]] * 0x1000;
    VStack_1c.x = VStack_1c.x + iVar7;
    VStack_1c.y = VStack_1c.y + acStack_40[(u8)ptr[1]] * 0x1000;
    MapObject_SetPositionVector(pLVar5,&VStack_1c);
    ptr[1] = ptr[1] + 1;
    if ((u8)ptr[1] >= 8) {
      *ptr = *ptr + 1;
    }
    break;
  case 5:
    pLVar5 = FollowMon_GetMapObject(fieldSystem);
    ov01_0220329C(pLVar5,3);
    *ptr = *ptr + 1;
    break;
  case 6:
    ptr[2] = ptr[2] + 1;
    if ((u8)ptr[2] >= 0x14) {
      ov01_02205790(fieldSystem,0);
      VStack_18 = *(VecFx32 *)&ov01_02209704;
      uVar6 = ov01_021F771C(fieldSystem->mapObjectManager);
      sub_02023E78(uVar6, &VStack_18);
      sub_02069E84(fieldSystem->followMon.mapObject,1);
      *ptr = *ptr + 1;
    }
    break;
  case 7:
    Heap_Free(ptr);
    return 1;
  }
  return 0;
}
