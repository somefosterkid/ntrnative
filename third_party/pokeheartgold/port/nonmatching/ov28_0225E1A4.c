void _u32_div_f(u32, u32);

void ov28_0225DD58(u8 *);

void ov28_0225DEB0(u8 *param_1);

void ov28_0225DF14(u8 *param_1);

void ov28_0225DFBC(u8 *param_1);

void ov28_0225E060(u8 *param_1);

void ov28_0225E0F4(u8 *param_1);

extern u32 ov28_0225EAAC;


void ov28_0225E1A4(u8 *param_1) {
  u8 *iVar5;
  u32 uVar3;
  s32 uVar6;
  s32 iVar4;
  s16 sVar1;
  s16 sVar2;
  u32 uStack_20;
  
  ov28_0225DD58(param_1);
  if (*(s32 *)(param_1 + 0x20c) == 0) {
    ov28_0225DEB0(param_1);
    *(u32 *)(param_1 + 0x210) = 0;
  }
  else {
    sVar1 = (s16)abs(*(s16 *)(param_1 + 0x208));
    sVar2 = (s16)abs(*(s16 *)(param_1 + 0x20a));
    if ((sVar1 <= 8) && (sVar2 <= 8)) {
      ov28_0225E0F4(param_1);
      *(u32 *)(param_1 + 0x210) = 4;
    }
    else if ((sVar1 <= 0xC) && (sVar2 <= 0xC)) {
      ov28_0225E060(param_1);
      *(u32 *)(param_1 + 0x210) = 3;
    }
    else if ((sVar1 <= 0x11) && (sVar2 <= 0x11)) {
      ov28_0225DFBC(param_1);
      *(u32 *)(param_1 + 0x210) = 2;
    }
    else {
      ov28_0225DF14(param_1);
      *(u32 *)(param_1 + 0x210) = 1;
    }
  }
  if (((u32)((u32)*(u16 *)(param_1 + 0x24c) << 0x10) >> 31) == 0) {
    for (uVar3 = 0; uVar3 < 4; uVar3++) {
      *(u32 *)(param_1 + uVar3 * 0xc + 0x21c) = *(u32 *)(param_1 + uVar3 * 0xc + 0x220);
      *(u32 *)(param_1 + uVar3 * 0xc + 0x224) = *(u32 *)(param_1 + uVar3 * 0xc + 0x220);
    }
    *(u16 *)(param_1 + 0x24c) = *(u16 *)(param_1 + 0x24c) | 0x8000;
    *(u16 *)(param_1 + 0x24c) = *(u16 *)(param_1 + 0x24c) & 0xffff8000 | 1;
    return;
  }
  for (uStack_20 = 0; uStack_20 < 4; uStack_20++) {
    iVar5 = param_1 + uStack_20 * 0xc;
    *(u32 *)(iVar5 + 0x220) = *(u32 *)(iVar5 + 0x220) % 0x168;
    *(u32 *)(iVar5 + 0x21c) = *(u32 *)(iVar5 + 0x21c) % 0x168;
    uVar3 = *(u32 *)(iVar5 + 0x21c);
    uVar6 = *(u32 *)(iVar5 + 0x220);
    iVar4 = abs(uVar6 - uVar3);
    if (iVar4 >= 0xB4) {
      if (uVar6 > uVar3) {
        *(s32 *)(iVar5 + 0x21c) = *(s32 *)(iVar5 + 0x21c) + 0x168;
      }
      else if (uVar6 < uVar3) {
        *(s32 *)(iVar5 + 0x220) = *(s32 *)(iVar5 + 0x220) + 0x168;
      }
    }
    ((void (**)(u8 *))&ov28_0225EAAC)[*(s32 *)(param_1 + 0x210)](param_1 + 0x21c + uStack_20 * 0xc);
  }
}
