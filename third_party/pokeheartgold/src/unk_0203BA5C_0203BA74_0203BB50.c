#include "global.h"

extern u32 sSpawnMaps;

s32 sub_0203BB50(u32 param_1);

s32 sub_0203BB50(u32 param_1) {
  u32 uVar1;
  u8 *puVar2;
  
  puVar2 = (unsigned char *)(&sSpawnMaps);
  uVar1 = 0;
  do {
    if (param_1 == *(u16 *)(puVar2 + 6)) {
      return uVar1 + 1;
    }
    uVar1 = uVar1 + 1;
    puVar2 = puVar2 + 0x12;
  } while (uVar1 < 0x1e);
  return 0;
}
