#include "global.h"

#include "filesystem.h"
#include "sys_task_api.h"
#include "unk_02087284.h"
#include "constants/heap.h"
#include "heap.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"

void sub_020873D4(SysTask *task, void *data);

s32 sub_020876B0(void *, s32);

s32 sub_020877B4(void *);

typedef struct {
    int unk_00;
    enum HeapID heapID;
    int unk_08;
    int unk_0C;
    fx32 unk_10;
} UnkStruct_0208B284;
typedef struct {
    NARC *unk_00;
    SpriteSystem *unk_04;
    SpriteManager *unk_08;
    PaletteData *unk_0C;
} UnkStruct_0208B878_sub1;
typedef struct {
    ManagedSprite *unk_00;
    s16 unk_04;
    s16 unk_06;
    int unk_08;
    int unk_0C;
} UnkStruct_0208B878_sub2;
typedef struct {
    UnkStruct_0208B878_sub1 unk_00;
    UnkStruct_0208B284 unk_10;
    UnkStruct_0208B878_sub2 unk_24[12];
    int unk_E4;
    int unk_E8;
    int unk_EC;
    BOOL unk_F0;
    SysTask *unk_F4;
    int unk_F8;
    int unk_FC;
    int unk_100;
    int unk_104;
} UnkStruct_0208B878;
BOOL sub_02087930(UnkStruct_0208B878 *param0, s16 param1, s16 param2);
BOOL sub_020878B0(UnkStruct_0208B878 *param0, BOOL param1);
void sub_02087830(UnkStruct_0208B878 *param0);
BOOL sub_020878B0(UnkStruct_0208B878 *param0, BOOL param1);
BOOL sub_020878B8(UnkStruct_0208B878 *param0, s16 param1, s16 param2);
BOOL sub_020878EC(UnkStruct_0208B878 *param0, s16 param1, s16 param2);
BOOL sub_02087930(UnkStruct_0208B878 *param0, s16 param1, s16 param2);
BOOL sub_02087948(UnkStruct_0208B878 *param0, s16 param1, s16 param2);
BOOL sub_02087988(UnkStruct_0208B878 *param0);
BOOL sub_020879E0(UnkStruct_0208B878 *param0, BOOL param1);
void sub_02087A08(UnkStruct_0208B878 *param0, int param1, int param2);
void sub_02087A30(UnkStruct_0208B878 *param0);
void sub_02087A54(UnkStruct_0208B878 *param0);

s32 sub_02087878(void *arg0, s32 arg1) {
    *(NARC **)arg0 = NARC_New(NARC_a_1_9_1, ((UnkStruct_0208B878 *)arg0)->unk_10.heapID);
    sub_020876B0(arg0, arg1);
    sub_020877B4(arg0);
    ((UnkStruct_0208B878 *)arg0)->unk_F4 = SysTask_CreateOnVBlankQueue(sub_020873D4, arg0, 0x1000U);
    return 1;
}

BOOL sub_020878B0(UnkStruct_0208B878 *param0, BOOL param1) {
    param0->unk_F0 = param1;
    return 1;
}

BOOL sub_020878B8(UnkStruct_0208B878 *param0, s16 param1, s16 param2) {
    param0->unk_24[0].unk_04 = param1;
    param0->unk_24[0].unk_06 = param2;
    param0->unk_E8 = 0xFF;
    param0->unk_EC = 0;

    {
        int v0;

        for (v0 = 0; v0 < param0->unk_10.unk_00; v0++) {
            param0->unk_24[v0].unk_08 = 0;
        }
    }

    return 1;
}

BOOL sub_020878EC(UnkStruct_0208B878 *param0, s16 param1, s16 param2) {
    if (sub_02087930(param0, param1, param2) == 0) {
        return 0;
    }

    param0->unk_24[0].unk_04 = param1;
    param0->unk_24[0].unk_06 = param2;
    param0->unk_E8 = 0xFF;
    param0->unk_EC = 0;

    {
        int v0;

        for (v0 = 0; v0 < param0->unk_10.unk_00; v0++) {
            param0->unk_24[v0].unk_08 = 0;
        }
    }

    return 1;
}

BOOL sub_02087930(UnkStruct_0208B878 *param0, s16 param1, s16 param2) {
    if ((param0->unk_24[0].unk_04 == param1) && (param0->unk_24[0].unk_06 == param2)) {
        return 0;
    }

    return 1;
}

BOOL sub_02087948(UnkStruct_0208B878 *param0, s16 param1, s16 param2) {
    {
        int v0;

        param0->unk_E8 = 0xFF;
        param0->unk_EC = 0;

        for (v0 = 0; v0 < param0->unk_10.unk_00; v0++) {
            param0->unk_24[v0].unk_04 = param1;
            param0->unk_24[v0].unk_06 = param2;
            ManagedSprite_SetPositionXYWithSubscreenOffset(param0->unk_24[v0].unk_00, param1, param2, param0->unk_10.unk_10);
        }
    }

    return 1;
}

BOOL sub_02087988(UnkStruct_0208B878 *param0) {
    switch (param0->unk_E4) {
    case 0:
        sub_020878B0(param0, 0);
        param0->unk_E4++;
        break;
    case 1:
        SysTask_Destroy(param0->unk_F4);
        param0->unk_E4++;
        break;
    default:
        sub_02087830(param0);
        NARC_Delete(param0->unk_00.unk_00);
        Heap_Free(param0);

        return 0;
    }

    return 1;
}

BOOL sub_020879E0(UnkStruct_0208B878 *param0, BOOL param1) {
    {
        int v0;

        for (v0 = 0; v0 < param0->unk_10.unk_00; v0++) {
            ManagedSprite_SetDrawFlag(param0->unk_24[v0].unk_00, param1);
        }
    }

    return 1;
}

void sub_02087A08(UnkStruct_0208B878 *param0, int param1, int param2) {
    if ((param1 != 0) && (param2 != 0)) {
        param0->unk_FC = param1;
        param0->unk_100 = param2;
    } else {
        param0->unk_FC = (15 * 1);
        param0->unk_100 = (15 * 1);
    }
}

void sub_02087A30(UnkStruct_0208B878 *param0) {
    int v0;

    for (v0 = 0; v0 < param0->unk_10.unk_00; v0++) {
        ManagedSprite_SetAnim(param0->unk_24[v0].unk_00, 1);
    }
}

void sub_02087A54(UnkStruct_0208B878 *param0) {
    int v0;

    for (v0 = 0; v0 < param0->unk_10.unk_00; v0++) {
        ManagedSprite_SetAnim(param0->unk_24[v0].unk_00, 0);
    }
}
