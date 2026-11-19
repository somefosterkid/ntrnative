#include "global.h"

#include "gf_3d_render.h"
#include "overlay_01_02203E40.h"
#include "overworld_anim_manager.h"

extern const MtxFx33 ov01_022095C8;

void ov01_02203F98(OverworldAnimManager *manager, UnkOv01_02203E40_Work *work) {
    VecFx32 translation;
    VecFx32 scale;
    MtxFx33 rotation = ov01_022095C8;

    scale.x = (fx32)(4096.0f * work->data.obj->unk_64);
    scale.y = (fx32)(4096.0f * work->data.obj->unk_64);
    scale.z = (fx32)(4096.0f * work->data.obj->unk_64);
    sub_02068DB8(manager, &translation);
    GF3dRender_DrawModel(&work->data.obj->renderObj, &translation, &rotation, &scale);
}
