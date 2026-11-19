#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "sprite.h"
#include "unk_020932E0.h"

#if !(defined(SDK_PORT) || defined(NONMATCHING))
#define DS_RECT(p, off) (*(UnkStruct_020932E0_Rect8 *)((u8 *)(p)->unk_46A4 + (off)))
#endif

void sub_02093B84(UnkStruct_020932E0 *param_1) {
    s32 cVar1;
    Sprite *pSVar3;
    Sprite *uVar4;
    UnkStruct_020932E0_Rect8 *iVar5;
    s32 iVar7;
    u8 uVar8;
    s32 iVar9;
    s32 iStack_50;
    s32 iStack_54;
    s32 y;
#if !(defined(SDK_PORT) || defined(NONMATCHING))
    u8 *iVar6;
#endif
    SpriteTemplate SStack_48;

    sub_02094150(&SStack_48, param_1, NNS_G2D_VRAM_TYPE_2DMAIN);
    param_1->unk_46A4 = Heap_Alloc(param_1->unk_4, param_1->unk_D * sizeof(UnkStruct_020932E0_Rect8));
#if defined(SDK_PORT) || defined(NONMATCHING)
    iVar9 = 0;
    if (iVar9 < (s32)param_1->unk_D) {
        cVar1 = 0x20;
        iStack_50 = 0;
        iStack_54 = 0x2c;
        do {
            param_1->unk_46A4[iVar9].unk_0 = cVar1;
            param_1->unk_46A4[iVar9].unk_2 = param_1->unk_46A4[iVar9].unk_0 + 0x18;
            param_1->unk_46A4[iVar9].unk_1 = 0xc0;
            iVar5 = &param_1->unk_46A4[iVar9];
            iVar5->unk_3 = iVar5->unk_1 + 0x20;
            param_1->unk_46A4[iVar9].unk_4 = 0;
            SStack_48.position.x = 0xd4000;
            SStack_48.position.y = (iStack_50 + 0x28) * 0x1000;
            if (param_1->unk_7E8[iVar9].unk_0 == NULL) {
                pSVar3 = Sprite_CreateAffine(&SStack_48);
                param_1->unk_7E8[iVar9].unk_0 = pSVar3;
            }
            Sprite_SetAnimActiveFlag(param_1->unk_7E8[iVar9].unk_0, 1);
            Sprite_SetAnimCtrlSeq(param_1->unk_7E8[iVar9].unk_0, iVar9 + 0x27);
            Sprite_SetPriority(param_1->unk_7E8[iVar9].unk_0, 1);
            Sprite_SetDrawFlag(param_1->unk_7E8[iVar9].unk_0, 0);
            Sprite_SetDrawPriority(param_1->unk_7E8[iVar9].unk_0, 6);
            sub_0209428C(&param_1->unk_7E4[iVar9 + 0x21], 0xd4, iStack_54, 0x18, 0x18);
            iVar9 = iVar9 + 1;
            cVar1 = cVar1 + 0x28;
            iStack_50 = iStack_50 + 0x28;
            iStack_54 = iStack_54 + 0x28;
        } while (iVar9 < (s32)param_1->unk_D);
    }
#else
    // DS: the sprite walk is a byte pointer over the whole object, the rect walk its own counter
    iVar9 = 0;
    if (iVar9 < (s32)param_1->unk_D) {
        cVar1 = 0x20;
        iVar7 = 0;
        iStack_50 = 0;
        iVar6 = (u8 *)param_1;
        iStack_54 = 0x2c;
        do {
            DS_RECT(param_1, iVar7).unk_0 = cVar1;
            DS_RECT(param_1, iVar7).unk_2 = DS_RECT(param_1, iVar7).unk_0 + 0x18;
            DS_RECT(param_1, iVar7).unk_1 = 0xc0;
            iVar5 = (&DS_RECT(param_1, iVar7));
            iVar5->unk_3 = iVar5->unk_1 + 0x20;
            DS_RECT(param_1, iVar7).unk_4 = 0;
            SStack_48.position.x = 0xd4000;
            SStack_48.position.y = (iStack_50 + 0x28) * 0x1000;
            if (((UnkStruct_020932E0 *)iVar6)->unk_7E8[0].unk_0 == NULL) {
                pSVar3 = Sprite_CreateAffine(&SStack_48);
                ((UnkStruct_020932E0 *)iVar6)->unk_7E8[0].unk_0 = pSVar3;
            }
            Sprite_SetAnimActiveFlag(((UnkStruct_020932E0 *)iVar6)->unk_7E8[0].unk_0, 1);
            Sprite_SetAnimCtrlSeq(((UnkStruct_020932E0 *)iVar6)->unk_7E8[0].unk_0, iVar9 + 0x27);
            Sprite_SetPriority(((UnkStruct_020932E0 *)iVar6)->unk_7E8[0].unk_0, 1);
            Sprite_SetDrawFlag(((UnkStruct_020932E0 *)iVar6)->unk_7E8[0].unk_0, 0);
            Sprite_SetDrawPriority(((UnkStruct_020932E0 *)iVar6)->unk_7E8[0].unk_0, 6);
            sub_0209428C(&param_1->unk_7E4[iVar9 + 0x21], 0xd4, iStack_54, 0x18, 0x18);
            iVar9 = iVar9 + 1;
            cVar1 = cVar1 + 0x28;
            iVar7 = iVar7 + sizeof(UnkStruct_020932E0_Rect8);
            iStack_50 = iStack_50 + 0x28;
            iVar6 = iVar6 + sizeof(UnkStruct_020932E0_Slot);
            iStack_54 = iStack_54 + 0x28;
        } while (iVar9 < (s32)param_1->unk_D);
    }
#endif
    uVar8 = 0;
    if (iVar9 < 3) {
        y = iVar9 * 0x28 + 0x2c;
        do {
            uVar4 = sub_0209417C(param_1, 0xd4, y, 0x2f, 1);
            iVar7 = uVar8;
            uVar8 = uVar8 + 1 & 0xff;
            y = y + 0x28;
            param_1->unk_8C8[iVar7] = uVar4;
            iVar9 = iVar9 + 1;
        } while (iVar9 < 3);
    }
    param_1->unk_46A8 = 0x20;
    param_1->unk_46AA = 0x97;
    param_1->unk_46A9 = 8;
    param_1->unk_46AB = 0x97;
    param_1->unk_46AC = 0;
    return;
}
