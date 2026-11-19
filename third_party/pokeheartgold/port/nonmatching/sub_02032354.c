void sub_020324F4(s32, s8 *);
extern u32 _020F68DE;

void sub_02032354(s32 param_1) {
  s8 cVar1;
  s8 cVar2;
  u32 uVar4;
  s32 iStack_4c;
  u32 uVar5;
  s32 iVar6;
  s8 *pcVar7;
  s8 *pcVar8;
  s8 *pcVar9;
  s8 *pcVar10;
  u32 uVar11;
  s32 iVar12;
  u32 uStack_38;
  u32 uStack_34;
  u32 uStack_2c;
  u32 uStack_3c;
  BOOL bVar3;
  s32 iStack_28;
  s32 iVar13;
  s8 acStack_20 [5];
  s8 acStack_1b [9];
  
  uStack_3c = (u32)*(u16 *)(param_1 + 10);
  sub_020324F4(param_1 + 0xc,acStack_1b);
  iStack_28 = 0;
  do {
    bVar3 = FALSE;
    uVar4 = (u8)(uStack_3c & 7);
    if (uVar4 == 0) break;
    uStack_2c = 0;
    uStack_38 = 0;
    uStack_34 = 0;
    uVar4 = (u8)(uVar4 - 1);
    pcVar10 = acStack_20;
    iVar12 = 0;
    do {
      *pcVar10 = (s8)iVar12;
      iVar12 = iVar12 + 1;
      pcVar10 = pcVar10 + 1;
    } while (iVar12 < 5);
    iStack_4c = 0;
    pcVar10 = acStack_20;
    do {
      iVar12 = iStack_4c + 1;
      if (iVar12 < 5) {
        pcVar9 = acStack_20 + iVar12;
        do {
          cVar1 = *pcVar9;
          cVar2 = *pcVar10;
          if ((acStack_1b[cVar2] <= acStack_1b[cVar1]) &&
             ((acStack_1b[cVar2] != acStack_1b[cVar1] || ((s32)cVar2 >= (s32)cVar1)))) {
            *pcVar10 = cVar1;
            *pcVar9 = cVar2;
          }
          iVar12 = iVar12 + 1;
          pcVar9 = pcVar9 + 1;
        } while (iVar12 < 5);
      }
      pcVar10 = pcVar10 + 1;
      iStack_4c = iStack_4c + 1;
    } while (iStack_4c < 2);
    pcVar7 = (signed char *)(((u8 *)&_020F68DE) + uVar4 * 5);
    uVar11 = 0;
    pcVar8 = acStack_1b;
    iVar13 = acStack_20[0];
    pcVar9 = pcVar8 + acStack_20[1];
    pcVar10 = pcVar8 + iVar13;
    do {
      uVar5 = ((s32)*pcVar8 + (s32)*pcVar7) * 0x1000000 >> 0x18;
      if ((((0 < *pcVar7) && (*pcVar10 != 0)) && (iVar13 != uVar11)) &&
         ((*pcVar9 != 0 && ((s32)acStack_20[1] != uVar11)))) {
        bVar3 = TRUE;
      }
      if ((s32)uVar5 < 0x3f) {
        if ((s32)uVar5 < 0) {
          uVar5 = 0;
        }
      }
      else {
        uVar5 = 0x3f;
      }
      if (((uVar4 == 6) || ((s32)uVar5 <= (s32)*pcVar8)) && ((s32)uVar5 > (s32)uStack_34)) {
        uStack_34 = uVar5 & 0xff;
        uStack_38 = uVar11 & 0xff;
      }
      *pcVar8 = (s8)uVar5;
      uVar11 = uVar11 + 1;
      uVar5 = uStack_2c + uVar5;
      uStack_2c = uVar5 & 0xff;
      pcVar7 = pcVar7 + 1;
      pcVar8 = pcVar8 + 1;
    } while ((s32)uVar11 < 5);
    uStack_3c = uStack_3c >> 3;
    if (bVar3) {
      if (*(u8 *)(param_1 + 0xe) < 10) {
        *(u8 *)(param_1 + 0xe) = 0;
      }
      else {
        *(u8 *)(param_1 + 0xe) = *(u8 *)(param_1 + 0xe) - 10;
      }
    }
    if ((uVar4 != 5) && (100 < uStack_2c)) {
      acStack_1b[uStack_38] = acStack_1b[uStack_38] - (uVar5 + -100);
    }
    iStack_28 = iStack_28 + 1;
  } while (iStack_28 < 5);
  iVar12 = 0;
  pcVar10 = acStack_1b;
  do {
    iVar6 = param_1 + iVar12;
    *(s8 *)(iVar6 + 0xf) = *pcVar10;
    pcVar10 = pcVar10 + 1;
    iVar12 = iVar12 + 1;
  } while (iVar12 < 5);
  *(u8 *)(param_1 + 9) = 3;
  *(u16 *)(param_1 + 10) = 0;
  return;
}
