#include "global.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "overlay_01_021FC4C4.h"
#include "sys_task.h"
#include "gf_3d_loader.h"

struct MapObject;

struct MapObjectManager;

struct GF_2DGfxRawResMan;

struct ResourceHeap;

struct SysTask;

struct SysTaskQueue;

struct TextureResourceManager;

struct UnkStruct_ov5_021ED0A4_t;

typedef struct MapObject MapObject;

typedef struct MapObjectManager MapObjectManager;


typedef struct TextureResourceManager TextureResourceManager;




enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};




void ov01_021FA370(UnkStruct_ov5_021ED0A4 *param0, u32 param1, BillboardResources *param2);

BillboardResources *ov01_021F9704(UnkStruct_ov5_021ED0A4 *param0, u32 param1);

BillboardResources *ov01_021F9704(UnkStruct_ov5_021ED0A4 *param0, u32 param1) {
    int v0;
    UnkStruct_ov5_021ED110 *v1 = param0->unk_F4;
    v0 = param0->unk_04;

    do {
        if (v1->unk_00 == param1) {
            return v1->unk_04;
        }

        v1++;
        v0--;
    } while (v0);

    v1 = param0->unk_F4;
    v0 = param0->unk_04;

    do {
        if (v1->unk_00 == 0xffff) {
            v1->unk_00 = param1;
            ov01_021FA370(param0, param1, v1->unk_04);
            return v1->unk_04;
        }

        v1++;
        v0--;
    } while (v0);

    return NULL;
}

UnkStruct_ov5_021ED0A4 *sub_0205F1A0(const MapObjectManager *mapObjMan);

int ov01_021F9744(const MapObjectManager *param0, u32 param1, BillboardResources *param2);

int ov01_021F9744(const MapObjectManager *param0, u32 param1, BillboardResources *param2) {
    int v0;
    const UnkStruct_ov5_021ED0A4 *v1 = sub_0205F1A0(param0);
    UnkStruct_ov5_021ED110 *v2 = v1->unk_F4;
    v0 = v1->unk_04;

    do {
        if (v2->unk_00 == param1) {
            *param2 = *(v2->unk_04);
            return 1;
        }

        v2++;
        v0--;
    } while (v0);

    return 0;
}
