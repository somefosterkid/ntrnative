#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "sprite.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_90_unk_struct_ov114_0225cdb4.h"
#include "overlay_90_unk_struct_ov114_0225ccd0.h"
#include "overlay_90_unk_struct_ov114_0225fab8.h"

struct SpriteResourceCollection;

struct UnkStruct_ov114_02025CAD4_t;

struct UnkStruct_ov114_02025CAD4_t {
    u32 unk_00;
    u16 unk_04;
    s16 unk_06;
    SpriteList *unk_08;
    NARC *unk_0C;
    UnkStruct_ov114_0225CCD0 unk_10;
    UnkStruct_ov114_0225CDB4 *unk_28;
    SysTask *unk_2C;
    Sprite *unk_30;
    UnkStruct_ov114_0225FAB8 unk_34;
};

Sprite *ov90_02258EB4(UnkStruct_ov114_0225CDB4 *param0, SpriteList *param1, s32 param2, s32 param3, u16 param4, u32 param5);

void ov90_0225BAD0(UnkStruct_ov114_0225FAB8 *param0, UnkStruct_ov114_0225CDB4 *param1, SpriteList *param2, u32 param3);

BOOL ov90_0225BBD0(UnkStruct_ov114_0225FAB8 *param0);

BOOL ov90_0225BBF0(UnkStruct_ov114_0225FAB8 *param0);

void ov90_0225BD08(UnkStruct_ov114_0225FAB8 *param0);

void ov90_0225C06C(SysTask *param0, void *param1);

extern s32 ov90_0225C4FC;
s32 *ov90_0225C150(void);

s32 ov90_0225C158(void);

void ov90_0225C06C(SysTask *param0, void *param1) {
    UnkStruct_ov114_0225CAD4 *v0 = param1;
    BOOL v1;

    switch (v0->unk_04) {
    case 0:
        break;
    case 1:
        ov90_0225BAD0(&v0->unk_34, v0->unk_28, v0->unk_08, v0->unk_00);
        v0->unk_30 = ov90_02258EB4(v0->unk_28, v0->unk_08, 128, 97, 64, v0->unk_00);
        Sprite_SetAnimCtrlSeq(v0->unk_30, 1);
        Sprite_SetPriority(v0->unk_30, 0);
        v0->unk_04++;
        break;
    case 2:
        v1 = ov90_0225BBD0(&v0->unk_34);

        if (v1 == 1) {
            Sprite_SetAnimActiveFlag(v0->unk_30, 1);
            v0->unk_04++;
            v0->unk_06 = 0;
        }
        break;
    case 3:
        v0->unk_06++;

        if (v0->unk_06 >= 50) {
            v0->unk_04++;
        }
        break;
    case 4:
        v1 = ov90_0225BBF0(&v0->unk_34);

        if (v1 == 1) {
            Sprite_Delete(v0->unk_30);
            v0->unk_30 = 0L;
            v0->unk_06 = 0;
            v0->unk_04++;
        }
        break;
    case 5:
        v0->unk_06++;

        if (v0->unk_06 < 2) {
            break;
        }

        v0->unk_04 = 0;

        ov90_0225BD08(&v0->unk_34);
        SysTask_Destroy(v0->unk_2C);
        v0->unk_2C = 0L;
        break;
    default:
        break;
    }
}

s32 *ov90_0225C150(void) {
    return &ov90_0225C4FC;
}

s32 ov90_0225C158(void) {
    return 5;
}
