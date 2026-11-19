#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "field_system_def.h"
#include "field_types_def.h"
#include "heap.h"
#include "overlay_02_02245B80.h"
#include "player_avatar.h"
#include "task.h"
#include "unk_02005D10.h"

extern u32 ov02_02253264;

BOOL ov02_022460FC(TaskManager *param_1);

BOOL ov02_022460FC(TaskManager *param_1) {
    LegendEffectTask *ptr;
    VecFx32 VStack_18;

    ptr = TaskManager_GetEnvironment(param_1);
    switch (ptr->state) {
    case 0:
        ptr->effect = ov02_02245E04((const Ov02Field3dEffectDesc *)(&ov02_02253264 + (u32)ptr->effectIdx * 3), ptr->heapID);
        PlayerAvatar_CopyPositionVector(ptr->fieldSystem->playerAvatar, &VStack_18);
        Field3dObject_SetPosEx(&ptr->effect->object, VStack_18.x, VStack_18.y, VStack_18.z);
        PlaySE(*(u16 *)((u8 *)&ov02_02253264 + 8 + (u32)ptr->effectIdx * 0xc));
        ptr->state = ptr->state + 1;
        break;
    case 1:
        if (ov02_02245FD0(ptr->effect) != 0) {
            ptr->state = ptr->state + 1;
        }
        break;
    case 2:
        ov02_02245E68(ptr->effect);
        Heap_Free(ptr);
        return 1;
    }
    return 0;
}
