#include "global.h"

#include "sprite.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 sub_0208B85C(u8 *param_1, s32 param_2, s32 param_3, s8 param_4, s32 param_5);
#else
s32 sub_0208B85C(u8 *param_1, s32 param_2, s32 param_3, s32 param_4, s32 param_5);
#endif

void sub_0208B780(Sprite **sprites, s32 param_2, s32 param_3, s32 param_4, s32 param_5, s32 param_6, s32 param_7);

void sub_0208B780(Sprite **sprites, s32 param_2, s32 param_3, s32 param_4, s32 param_5, s32 param_6, s32 param_7) {
    u8 cVar1;
    s32 iVar2;
    s8 *pcVar3;
    Sprite **puVar4;
    s32 iVar5;
    s8 acStack_20[8];

    iVar2 = sub_0208B85C((u8 *)acStack_20, param_3, param_4, param_5, param_6);
    iVar5 = 0;
    pcVar3 = acStack_20;
    puVar4 = sprites + param_2;
    do {
        cVar1 = *pcVar3;
        if (cVar1 == 0) {
            Sprite_SetDrawFlag(*puVar4, 0);
        } else if (cVar1 == 1) {
            Sprite_SetDrawFlag(*puVar4, 1);
            Sprite_SetAnimCtrlSeq(*puVar4, 3);
        } else if (cVar1 == 2) {
            Sprite_SetDrawFlag(*puVar4, 1);
            Sprite_SetAnimCtrlSeq(*puVar4, 0);
        } else if (cVar1 == 3) {
            Sprite_SetDrawFlag(*puVar4, 1);
            Sprite_SetAnimCtrlSeq(*puVar4, 1);
        } else if (cVar1 == 4) {
            Sprite_SetDrawFlag(*puVar4, 1);
            Sprite_SetAnimCtrlSeq(*puVar4, 2);
        }
        pcVar3 = pcVar3 + 1;
        puVar4 = puVar4 + 1;
        iVar5 = iVar5 + 1;
    } while (iVar5 < 5);
    if (iVar2 == 0) {
        Sprite_SetDrawFlag(sprites[param_7], 0);
        return;
    }
    if (0 < iVar2) {
        Sprite_SetDrawFlag(sprites[param_7], 1);
        Sprite_SetAnimCtrlSeq(sprites[param_7], 4);
        return;
    }
    if (iVar2 < 0) {
        Sprite_SetDrawFlag(sprites[param_7], 1);
        Sprite_SetAnimCtrlSeq(sprites[param_7], 5);
    }
    return;
}
