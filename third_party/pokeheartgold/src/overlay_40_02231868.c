#include "global.h"

#include "overlay_40.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "heap.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "overlay_40_unk_struct_0208_c06_c.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

s32 ov40_0222BF80(void *, s32);

s32 ov40_0223142C(UnkStruct_0208C06C *param0);

s32 sub_020879E0(void *, s32);

struct FontOAM;
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
void ov40_0222D288(ManagedSprite *param0, s16 param1, s16 param2);
void ov40_0222D294(ManagedSprite *param0, s16 *param1, s16 *param2);
typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    u8 padding_0C[16];
    u8 unk_1C;
    u8 unk_1D;
    u8 padding_1E[2];
    ManagedSprite *unk_20;
    FontOAM *unk_24;
    UnkStruct_0208B878 *unk_28;
    BOOL *unk_2C;
    BOOL *unk_30;
} UnkStruct_ov62_022354A4;
void sub_020136B4(FontOAM *fontOAM, int x, int y);
BOOL sub_020878B8(UnkStruct_0208B878 *param0, s16 param1, s16 param2);

s32 ov40_02231868(void *arg0) {
    if (ov40_0223142C(arg0) != 0) {
        if (((UnkStruct_0208C06C *)arg0)->unk_14.unk_30 == 1) {
            sub_020879E0(*((void **)&((UnkStruct_0208C06C *)arg0)->unk_6F0), 0);
            ov40_0222BF80(arg0, 0);
        } else {
            ov40_0222BF80(arg0, 1);
        }
    }
    return 0;
}

s32 ov40_0223189C(void *arg0) {
    if (*(s32 *)((u8 *)arg0 + 0x44) == 1) {
        sub_020879E0(*(void **)((u8 *)arg0 + 0x6F0), 0);
        ov40_0222BF80(arg0, 0);
    } else {
        ov40_0222BF80(arg0, 1);
    }
    return 0;
}

void ov40_022318C8(SysTask *param0, void *param1) {
    UnkStruct_ov62_022354A4 *v0 = param1;

    switch (v0->unk_1D) {
    case 0:
        v0->unk_08 = (v0->unk_04 - v0->unk_00) / v0->unk_1C;
        v0->unk_0A = (v0->unk_06 - v0->unk_02) / v0->unk_1C;
        v0->unk_1D++;
    case 1: {
        s16 v1, v2;

        ov40_0222D294(v0->unk_20, &v1, &v2);

        v0->unk_1C--;

        if (v0->unk_1C == 0) {
            v1 = v0->unk_04;
            v2 = v0->unk_06;
            v0->unk_1D++;
        } else {
            v1 += v0->unk_08;
            v2 += v0->unk_0A;
        }

        if (v0->unk_28) {
            sub_020878B8(v0->unk_28, v1 + (15 + 1), v2);
            if (v0->unk_1C == 2) {
                *(v0->unk_30) = 1;
            }
        }

        ov40_0222D288(v0->unk_20, v1, v2);
        sub_020136B4(v0->unk_24, 36, -8);

        *(v0->unk_2C) = 1;
    } break;
    default:
        Heap_Free(v0);
        SysTask_Destroy(param0);
        break;
    }
}
