#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "overlay_48.h"
#include "pm_string.h"
#include "sys_task.h"
#include "unk_0200A090.h"

void ov48_02258BF4(MtxFx33 *param0, const VecFx32 *param1) {
    MtxFx33 v0;

    MTX_RotY33(param0, FX_SinIdx((u16)param1->y), FX_CosIdx((u16)param1->y));
    MTX_RotX33(&v0, FX_SinIdx((u16)param1->x), FX_CosIdx((u16)param1->x));
    MTX_Concat33(param0, &v0, param0);
    MTX_RotZ33(&v0, FX_SinIdx((u16)param1->z), FX_CosIdx((u16)param1->z));
    MTX_Concat33(param0, &v0, param0);
}

void ov48_02258C6C(MtxFx33 *param0, const VecFx32 *param1) {
    MtxFx33 v0;

    MTX_RotY33(param0, FX_SinIdx((u16)param1->x), FX_CosIdx((u16)param1->x));
    MTX_RotX33(&v0, FX_SinIdx((u16)-param1->y), FX_CosIdx((u16)-param1->y));
    MTX_Concat33(param0, &v0, param0);
    MTX_RotZ33(&v0, FX_CosIdx((u16)param1->z), FX_SinIdx((u16)param1->z));
    MTX_Concat33(param0, &v0, param0);
}
