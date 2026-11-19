#include "global.h"

#include "constants/sndseq.h"

#include "error_handling.h"
#include "map_object.h"
#include "overworld_anim_manager.h"
#include "unk_02005D10.h"
#include "unk_02023694.h"

struct FieldSystem;

struct LocalMapObject;

struct MapObjectManager;

struct NARC;

struct SysTask;

struct SysTaskQueue;

typedef struct {
    int unk0;
    FieldSystem *unk4;
    void *unk8;
    LocalMapObject *unkC;
} UnkData_022001E4;

typedef struct {
    int unk0;
    int unk4;
    int unk8;
    int unkC;
    int unk10;
    int unk14;
    VecFx32 unk18;
    u8 unk24[4];
    fx32 unk28;
    u8 unk2C[4];
    UnkData_022001E4 unk30;
    void *unk40;
} UnkWork_022001E4;

extern void ov01_021F93AC(LocalMapObject *object, VecFx32 *out);

void ov01_02200614(void *param0, UnkWork_022001E4 *work);

void ov01_02200614(void *param0, UnkWork_022001E4 *work) {
    VecFx32 vec;
    LocalMapObject *mapObject = work->unk30.unkC;
    GF_ASSERT(sub_0205F0A8(mapObject, work->unk8, work->unkC));
    ov01_021F93AC(mapObject, &vec);
    {
        fx32 offset = 2 << 0x10;
        vec.y += offset;
        vec.z += offset >> 5;
    }
    switch (work->unk0) {
    case 0:
        sub_02023EA4(work->unk40, 1);
        work->unk18.y += work->unk28;
        if (work->unk18.y != 0) {
            work->unk28 -= 2 << 0xc;
        } else {
            work->unk28 = 0;
            work->unk0++;
        }
        break;
    case 1:
        work->unk4++;
        if (work->unk4 >= 0x1e) {
            work->unk0++;
            work->unk4 = 0;
            work->unk14 = 1;
        }
        break;
    case 2:
        break;
    }
    sub_02068DA8(param0, &vec);
}
