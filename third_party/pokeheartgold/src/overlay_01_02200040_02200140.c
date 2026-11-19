#include "global.h"

#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_02200040.h"
#include "overlay_01_021FF6B0.h"

BOOL sub_02023DA4(Billboard *billboard);
int sub_02023F04(Billboard *billboard, fx32 numFrames);
fx32 sub_02023F70(const Billboard *billboard);
Billboard *ov01_021F1740(FieldEffectManager *fieldEffMan, u32 id, const VecFx32 *position);

int ov01_02200140(OverworldAnimManager *param0, UnkStruct_ov5_021F3FB8 *param1);
void ov01_02200184(OverworldAnimManager *param0, UnkStruct_ov5_021F3FB8 *param1);
void ov01_02200190(OverworldAnimManager *param0, UnkStruct_ov5_021F3FB8 *param1);

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the funcs table the DS keeps in asm/overlay_01_02200040.s (same label), with the context size from the type, not the DS literal.
const OverworldAnimManagerFuncs ov01_02209294 = OVERWORLD_ANIM_MANAGER_FUNCS(UnkStruct_ov5_021F3FB8, ov01_02200140, ov01_02200184, ov01_02200190, ov01_022001C0);
#endif

int ov01_02200140(OverworldAnimManager *param0, UnkStruct_ov5_021F3FB8 *param1)
{
    VecFx32 v0;
    UnkStruct_ov5_021F3FB8 *v1;
    const UnkStruct_ov5_021F3F10 *v2;

    v1 = param1;
    v2 = sub_02068D98(param0);

    v1->unk_10 = *v2;
    v1->unk_04 = MapObject_GetID(v1->unk_10.unk_0C);
    v1->unk_08 = MapObject_GetMapID(v1->unk_10.unk_0C);

    sub_02068DB8(param0, &v0);

    v1->unk_20 = ov01_021F1740(v1->unk_10.unk_04, 11, &v0);

    return 1;
}

void ov01_02200184(OverworldAnimManager *param0, UnkStruct_ov5_021F3FB8 *param1)
{
    UnkStruct_ov5_021F3FB8 *v0 = param1;
    sub_02023DA4(v0->unk_20);
}

void ov01_02200190(OverworldAnimManager *param0, UnkStruct_ov5_021F3FB8 *param1)
{
    int v0;
    UnkStruct_ov5_021F3FB8 *v1 = param1;
    LocalMapObject *v2 = v1->unk_10.unk_0C;

    switch (v1->unk_00) {
    case 0:
        sub_02023F04(v1->unk_20, FX32_ONE);
        v0 = sub_02023F70(v1->unk_20) / FX32_ONE;

        if (v0 >= 7) {
            ov01_021F1640(param0);
            return;
        }
        break;
    }
}
