#include "global.h"
struct UnkStruct_ov70_0225F350;

#include "font.h"
#include "gf_gfx_loader.h"
#include "sys_task_api.h"
#include "unk_02077678.h"
#include "overlay_49_unk_struct_ov70_0225f350.h"

s32 GetMoveTypeAndCategoryIconPlttFileId(void);

void ov49_0225BA5C(SysTask *task, void *param);

void ov49_0225BA5C(SysTask *task, void *param) {
    UnkStruct_ov70_0225F350 *arg1 = param;
    enum NarcId narcId;

    GfGfxLoader_GXLoadPalFromOpenNarc(arg1->unk_14C, (s32) arg1->unk_154, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, (enum HeapID) arg1->unk_156);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_5_OFFSET, (enum HeapID) arg1->unk_156);
    narcId = TypeIconGfx_GetNarcId();
    GfGfxLoader_GXLoadPal(narcId, GetMoveTypeAndCategoryIconPlttFileId(), GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_11_OFFSET, 0x60U, (enum HeapID) arg1->unk_156);
    SysTask_Destroy(task);
}
