#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sys_task.h"

typedef struct UnkStruct_ov07_02225810_ptr {
    u8 unk_0[0x4];
    void *unk_4;
    u8 unk_8[0x28];
    void *unk_30;
} UnkStruct_ov07_02225810_ptr;

s32 ov07_02222EE8(s32 *);

s32 ov07_02222EF8(void *);

void ov07_02225810(SysTask *arg0, void *param);

void ov07_02225810(SysTask *arg0, void *param) {
    UnkStruct_ov07_02225810_ptr *ptr = param;
    if (ov07_02222EE8(ptr->unk_30) == 0) {
        ov07_02222EF8(ptr->unk_30);
        ov07_0221C448(ptr->unk_4, arg0);
        Heap_Free(ptr);
    }
}
