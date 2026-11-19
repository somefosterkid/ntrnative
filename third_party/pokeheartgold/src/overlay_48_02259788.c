#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "pm_string.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "overlay_48_unk_struct_ov69_0225_d35_c.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"

struct G3DPipelineBuffers;
struct SpriteList;
struct SpriteResourceCollection;
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
void GF_3DVramMan_Delete(G3DPipelineBuffers *state);
enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};
void sub_02018030(Field3dModel *model, NARC *narc, u32 memberIndex, u32 heapID);
void sub_020181B0(Field3dObject *obj, Field3dModel *model);
typedef struct {
    VecFx32 unk_00;
    VecFx32 unk_0C;
    VecFx32 unk_18;
    Field3dObject unk_24;
    Field3dModel unk_9C;
} UnkStruct_ov69_0225D71C;
void sub_02018068(Field3dModel *model);
void sub_02018288(Field3dObject *obj, const MtxFx33 *rotation);
void sub_020182A8(Field3dObject *obj, fx32 x, fx32 y, fx32 z);
void sub_020182C4(Field3dObject *obj, fx32 x, fx32 y, fx32 z);
void ov48_022598BC(const UnkStruct_ov69_0225D71C *param0, MtxFx33 *param1);

void ov48_02259788(UnkStruct_ov69_0225D35C *param0);
void ov48_02259798(void);
void ov48_02259824(UnkStruct_ov69_0225D71C *param0, UnkStruct_ov69_0225D35C *param1, u32 heapID);
void ov48_02259868(UnkStruct_ov69_0225D71C *param0);
void ov48_02259874(UnkStruct_ov69_0225D71C *param0);

void ov48_02259788(UnkStruct_ov69_0225D35C *param0) {
    GF_3DVramMan_Delete(param0->unk_1A4);
}

void ov48_02259798(void) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, 1);

    G2_SetBG0Priority(1);
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(1);
    G3X_AlphaTest(0, 0);
    G3X_AlphaBlend(1);
    G3X_EdgeMarking(1);
    G3X_SetFog(0, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(((GXRgb)(((26) << (0)) | ((26) << (5)) | ((26) << (10)))), 0, 0x7fff, 63, 0);
    G3_ViewPort(0, 0, 255, 191);
}

void ov48_02259824(UnkStruct_ov69_0225D71C *param0, UnkStruct_ov69_0225D35C *param1, u32 heapID) {
    VEC_Set(&param0->unk_00, 0, 0, 0);
    VEC_Set(&param0->unk_0C, ((fx32)0x0000000000001000L), ((fx32)0x0000000000001000L), ((fx32)0x0000000000001000L));
    VEC_Set(&param0->unk_18, 0x1A40, 0x7C00, 0);
    sub_02018030(&param0->unk_9C, param1->unk_1A8, 3, heapID);
    sub_020181B0(&param0->unk_24, &param0->unk_9C);
}

void ov48_02259868(UnkStruct_ov69_0225D71C *param0) {
    sub_02018068(&param0->unk_9C);
}

void ov48_02259874(UnkStruct_ov69_0225D71C *param0) {
    MtxFx33 v0;

    ov48_022598BC(param0, &v0);

    sub_020182A8(&param0->unk_24, param0->unk_00.x, param0->unk_00.y, param0->unk_00.z);
    sub_020182C4(&param0->unk_24, param0->unk_0C.x, param0->unk_0C.y, param0->unk_0C.z);

    sub_02018288(&param0->unk_24, &v0);
}
