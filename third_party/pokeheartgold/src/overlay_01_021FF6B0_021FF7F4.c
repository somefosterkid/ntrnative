#include "global.h"

#include "overlay_01_021F1348.h"
#include "overlay_01_021FF6B0.h"

BOOL sub_02023DA4(Billboard *billboard);
int sub_02023F04(Billboard *billboard, fx32 numFrames);
fx32 sub_02023F70(const Billboard *billboard);

void ov01_021FF7F4(OverworldAnimManager *param0, UnkStruct_ov5_021F3FB8 *param1)
{
    UnkStruct_ov5_021F3FB8 *v0 = param1;
    sub_02023DA4(v0->unk_20);
}

void ov01_021FF800(OverworldAnimManager *param0, UnkStruct_ov5_021F3FB8 *param1)
{
    int v0;
    UnkStruct_ov5_021F3FB8 *v1 = param1;
    LocalMapObject *v2 = v1->unk_10.unk_0C;

    switch (v1->unk_00) {
    case 0:
        sub_02023F04(v1->unk_20, FX32_ONE);
        v0 = sub_02023F70(v1->unk_20) / FX32_ONE;

        if (v0 >= 9) {
            ov01_021F1640(param0);
            return;
        }
        break;
    }
}
