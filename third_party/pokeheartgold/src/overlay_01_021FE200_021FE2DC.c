#include "global.h"
#include "simple3dmodel.h"

#include "error_handling.h"
#include "overlay_01_021F1348.h"
#include "field_types_def.h"
#include "overlay_01_021FE200.h"
#include "overlay_01_simple_renderers.h"

extern const int ov01_02209044[4][4];

Simple3DRenderObj *ov01_021FE2DC(UnkStruct_021F1CF8 *param0, int param1, int param2, int param3);

Simple3DRenderObj *ov01_021FE2DC(UnkStruct_021F1CF8 *param0, int param1, int param2, int param3) {
    int v0;
    Simple3DRenderObj *v1 = NULL;

    switch (param1) {
    case 0:
        v0 = 0 + param2;
        v1 = &param0->unk_194[v0];
        break;
    case 1:
        v0 = ov01_02209044[param3][param2];
        v1 = &param0->unk_194[v0];
        break;
    case 2:
    case 4:
        v0 = 0 + param2;
        v1 = &param0->unk_4DC[v0];
        break;
    case 3:
        v0 = ov01_02209044[param3][param2];
        v1 = &param0->unk_4DC[v0];
        break;
    }

    GF_ASSERT(v1 != NULL);
    return v1;
}

Simple3DModel *ov01_021FE35C(UnkStruct_021F1CF8 *param0, int param1, int param2, int param3);

Simple3DModel *ov01_021FE35C(UnkStruct_021F1CF8 *param0, int param1, int param2, int param3) {
    int v0 = 0;
    Simple3DModel *v1 = NULL;

    switch (param1) {
    case 0:
        v0 = 0 + param2;
        v1 = &param0->unk_04[v0];
        break;
    case 1:
        v0 = ov01_02209044[param3][param2];
        v1 = &param0->unk_04[v0];
        break;
    case 2:
    case 4:
        v0 = 0 + param2;
        v1 = &param0->unk_CC[v0];
        break;
    case 3:
        v0 = ov01_02209044[param3][param2];
        v1 = &param0->unk_CC[v0];
        break;
    }

    GF_ASSERT(v1 != NULL);
    return v1;
}

OverworldAnimManager *ov01_021FE3C4(LocalMapObject *mapObj) {
    return ov01_021FE3F8(mapObj, 0);
}
