#include "global.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "field_types_def.h"
#include "map_object.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

void *ov01_021FC5A4(struct ResourceHeap *, u32);

void ov01_02205870(s32 param_1, LocalMapObject *param_2, Billboard *param_3, u8 *param_4);

void ov01_02205870(s32 param_1, LocalMapObject *param_2, Billboard *param_3, u8 *param_4) {
    MapObjectManager *manager;
    void *pvVar1;
    u32 *data;
    u32 uVar2;

    manager = MapObject_GetManager(param_2);
    pvVar1 = sub_0205F1A0(manager);
    if (!param_1) {
        uVar2 = 0x15;
    } else {
        uVar2 = 0x16;
    }
    data = ov01_021FC5A4(((UnkStruct_ov5_021ED0A4_t *)pvVar1)->unk_FC, uVar2);
    BillboardGfxSequence_SetData(data, (BillboardGfxSequence *)(param_4 + 0xc));
    return;
}
