#include "global.h"

#include "constants/heap.h"

#include "bag.h"
#include "bag_types_def.h"
#include "bug_contest_internal.h"
#include "error_handling.h"
#include "field_types_def.h"
#include "filesystem_files_def.h"
#include "item.h"
#include "pokemon_icon_idx.h"
#include "sprite_transfer.h"
#include "unk_0200A090.h"
#include "unk_0206D494.h"

s32 ov27_0225AE8C(s32 arg0);

extern void *ov27_0225CF94;

extern u8 ov27_0225CFC8;

void ov27_0225AEA8(FieldSystem *param_1, void **param_2, void **param_3, s32 param_4, int param_5, s32 param_6, Bag *param_7, s32 param_8);

void ov27_0225AEA8(FieldSystem *param_1, void **param_2, void **param_3, s32 param_4, int param_5, s32 param_6, Bag *param_7, s32 param_8) {
    u16 uVar1;
    u32 uStack_18;
    s32 sVar3;
    SpriteResource *pSVar4;
    u32 uVar5;
    u32 fileId;
    NarcId narcId;
    BOOL BStack_1c;
    s32 iStack_20;
    BugContest *pBVar2;

    fileId = 0x12;
    narcId = NARC_a_0_1_4;
    uVar5 = (u32)(u8)(&ov27_0225CFC8 + param_8 * 8)[param_4];
    uStack_18 = 0xe;
    BStack_1c = 1;
    iStack_20 = 1;
    if (uVar5 != 0xd) {
        if ((uVar5 == 2) && (param_6 == 1)) {
            fileId = 0x1b;
        } else {
            fileId = (u32) * (u16 *)(((u8 *)&ov27_0225CF94) + uVar5 * 4);
            if (fileId == 0xffff) {
                pBVar2 = FieldSystem_BugContest_Get(param_1);
                if (pBVar2 == NULL) {
                    GF_AssertFail();
                }
                if (pBVar2->caught_poke) {
                    BStack_1c = 0;
                    narcId = NARC_poketool_icongra_poke_icon;
                    iStack_20 = 3;
                    uStack_18 = sub_02074490();
                    fileId = Pokemon_GetIconNaix(pBVar2->mon);
                } else {
                    fileId = 0x12;
                }
            }
        }
    }
    if (param_4 == 7) {
        BStack_1c = 0;
        narcId = NARC_itemtool_itemdata_item_icon;
        uVar1 = Bag_GetRegisteredItem1(param_7);
        uStack_18 = GetItemIndexMapping(uVar1, 2);
        uVar1 = Bag_GetRegisteredItem1(param_7);
        fileId = GetItemIndexMapping(uVar1, 1);
    } else if (param_4 == 8) {
        BStack_1c = 0;
        narcId = NARC_itemtool_itemdata_item_icon;
        uVar1 = Bag_GetRegisteredItem2(param_7);
        uStack_18 = GetItemIndexMapping(uVar1, 2);
        uVar1 = Bag_GetRegisteredItem2(param_7);
        fileId = GetItemIndexMapping(uVar1, 1);
    } else if (param_4 == 9) {
        uStack_18 = 7;
        fileId = 0x46;
        iStack_20 = 4;
    } else if (param_4 == 10) {
        sVar3 = ov27_0225AE8C(param_8);
        BStack_1c = 0;
        narcId = NARC_itemtool_itemdata_item_icon;
        uStack_18 = GetItemIndexMapping((u16)sVar3, 2);
        fileId = GetItemIndexMapping((u16)sVar3, 1);
    }
    pSVar4 = AddCharResObjFromNarc((GF_2DGfxResMan *)*param_2, narcId, fileId, BStack_1c, param_5, 2, HEAP_ID_8);
    *param_3 = (u8 *)(pSVar4);
    pSVar4 = AddPlttResObjFromNarc((GF_2DGfxResMan *)param_2[1], narcId, uStack_18, 0, param_5, 2, iStack_20, HEAP_ID_8);
    param_3[1] = (u8 *)(pSVar4);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd((SpriteResource *)*param_3);
    sub_0200A740((SpriteResource *)*param_3);
    SpriteTransfer_CreatePlttTransferTask((SpriteResource *)param_3[1]);
    sub_0200A740((SpriteResource *)param_3[1]);
    return;
}
