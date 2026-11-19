#include "global.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "overlay_01_021F1348.h"
#include "overworld_anim_manager.h"
#include "sys_task.h"
#include "overlay_01_unk_struct_ov5_021f5a34.h"

struct MapObject;
typedef struct MapObject MapObject;
void sub_02023EA4(Billboard *billboard, u8 draw);
typedef struct {
    int unk_00;
    FieldEffectManager *unk_04;
    UnkStruct_ov5_021F5A34 *unk_08;
    MapObject *unk_0C;
} UnkStruct_ov5_021F5D8C;
typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    VecFx32 unk_18;
    VecFx32 unk_24;
    UnkStruct_ov5_021F5D8C unk_30;
    Billboard *unk_40;
} UnkStruct_ov5_021F5C4C;

void ov01_02200400(OverworldAnimManager *param0);

void ov01_02200400(OverworldAnimManager *param0) {
    UnkStruct_ov5_021F5C4C *v0 = sub_02068D74(param0);

    v0->unk_10 = 1;

    if (v0->unk_40 != 0L) {
        sub_02023EA4(v0->unk_40, 0);
    }
}
