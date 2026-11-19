#include "global.h"

#include "heap.h"
#include "map_object.h"
#include "overlay_01_021F72DC.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"
#include "overlay_01_021F944C.h"
#include "sys_task_api.h"

void ov01_021FA8F8(UnkStruct_ov5_021ED0A4 *gfx) {
    UnkStruct_ov5_021EDF3C *ptr;

    ptr = gfx->unk_100;
    ptr->unk_06 = 1;
    SysTask_Destroy(ptr->unk_18);
    SysTask_Destroy(ptr->unk_1C);
    SysTask_Destroy(ptr->unk_20);
    SysTask_Destroy(ptr->unk_24);
    Heap_Free(ptr);
    gfx->unk_100 = NULL;
}

s32 ov01_021FA97C(LocalMapObject *, u32);

void ov01_021FA930(LocalMapObject *obj, u32 sprite) {
    MapObjectManager *mgr;
    u32 gfxId;

    if ((ov01_021FA2D4(obj) == 1) || (ov01_021F72DC(obj) == NULL)) {
        gfxId = MapObject_GetSpriteID(obj);
        mgr = MapObject_GetManager(obj);
        sub_0205E420(obj);
        ov01_021FA108(mgr, (s32)gfxId, obj);
        sub_0205E38C(obj, sprite);
        return;
    }
    ov01_021FA97C(obj, sprite);
}
