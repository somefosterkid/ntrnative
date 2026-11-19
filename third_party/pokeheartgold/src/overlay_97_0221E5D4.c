#include "global.h"
#include "overlay_manager.h"
#include "screen_fade.h"
#include "constants/heap.h"

void ov97_0221E98C(void *, void *);

void ov97_0221F010(void *);

s32 sub_020935E0(void *);

void sub_02093A40(void *);

u32 ov97_0221E5D4(OverlayManager *param_1);

u32 ov97_0221E5D4(OverlayManager *param_1) {
  void *pvVar1;
  s32 iVar2;
  BOOL BVar3;
  void *pvVar4;
  
  pvVar1 = OverlayManager_GetData(param_1);
  switch(*(u32 *)((u8 *)pvVar1 + 4)) {
  case 0:
    *(u32 *)((u8 *)pvVar1 + 4) = 1;
    break;
  case 1:
    BeginNormalPaletteFade
              (FADE_BOTH_SCREENS,FADE_TYPE_BRIGHTNESS_IN,FADE_TYPE_BRIGHTNESS_IN,0,6,1,
               HEAP_ID_POKEATHLON);
    *(u32 *)((u8 *)pvVar1 + 4) = 2;
    break;
  case 2:
    BVar3 = IsPaletteFadeFinished();
    if (BVar3 != 0) {
      *(u32 *)((u8 *)pvVar1 + 4) = 3;
    }
    break;
  case 3:
    iVar2 = sub_020935E0(*(void **)((u8 *)pvVar1 + 8));
    if (iVar2 == 2) {
      *(u32 *)((u8 *)pvVar1 + 0x28) = 0;
      *(u32 *)((u8 *)pvVar1 + 4) = 4;
    }
    else if (iVar2 == 3) {
      *(u32 *)((u8 *)pvVar1 + 0x28) = 1;
      *(u32 *)((u8 *)pvVar1 + 4) = 4;
    }
    break;
  case 4:
    BeginNormalPaletteFade
              (FADE_BOTH_SCREENS,FADE_TYPE_BRIGHTNESS_OUT,FADE_TYPE_BRIGHTNESS_OUT,0,6,1,
               HEAP_ID_POKEATHLON);
    *(u32 *)((u8 *)pvVar1 + 4) = 5;
    break;
  case 5:
    BVar3 = IsPaletteFadeFinished();
    if (BVar3 != 0) {
      pvVar4 = OverlayManager_GetArgs(param_1);
      ov97_0221E98C(pvVar1,pvVar4);
      return 1;
    }
  }
  sub_02093A40(*(void **)((u8 *)pvVar1 + 8));
  ov97_0221F010(*(void **)((u8 *)pvVar1 + 0xc));
  return 0;
}
