#include "global.h"

#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_01_unk_struct_ov5_021de47c.h"
#include "overlay_01_unk_struct_ov5_021de5a4.h"
#include "overlay_01_unk_struct_ov5_021e5128.h"
#include "overlay_115.h"

struct G2dRenderer;

struct LinearInterpolationTaskFX32;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

enum AffineOverwriteMode {
    AFFINE_OVERWRITE_MODE_NONE = 0,
    AFFINE_OVERWRITE_MODE_NORMAL,
    AFFINE_OVERWRITE_MODE_DOUBLE,
};


void LinearInterpolationTaskFX32_Init(LinearInterpolationTaskFX32 *task, fx32 startValue, fx32 endValue, int numSteps);

Sprite *ov01_021F0718(UnkStruct_ov5_021DE47C *param0, UnkStruct_ov5_021DE5A4 *param1, fx32 param2, fx32 param3, fx32 param4, int param5);


void ov01_021EFE70(LinearInterpolationTaskFX32 *task, fx32 startValue, fx32 endValue, int numSteps);

void ov115_0225F020(UnkStruct_ov5_021E5128 *param0, UnkStruct_ov5_021DE47C *param1, UnkStruct_ov5_021DE5A4 *param2, fx32 param3, fx32 param4, u32 param5) {
    int v0;
    param0->unk_00 = 0;
    param0->unk_02 = 0;
    for (v0 = 0; v0 < 4; v0++) {
        param0->unk_04[v0] = ov01_021F0718(param1, param2, param3, param4, 0, 0);
        Sprite_SetDrawFlag(param0->unk_04[v0], 0);
        Sprite_SetAnimActiveFlag(param0->unk_04[v0], 1);
        if (v0 != 3) {
            Sprite_SetAffineOverwriteMode(param0->unk_04[v0], 2);
            Sprite_SetAnimCtrlSeq(param0->unk_04[v0], 1);
            ov01_021EFE70(&param0->unk_14[v0], (fx32)((2 > 0) ? ((2 * (1 << 12)) + 0.5f) : ((2 * (1 << 12)) - 0.5f)), (fx32)((1 > 0) ? ((1 * (1 << 12)) + 0.5f) : ((1 * (1 << 12)) - 0.5f)), 6);
        } else {
            ov01_021EFE70(&param0->unk_14[v0], (fx32)((1 > 0) ? ((1 * (1 << 12)) + 0.5f) : ((1 * (1 << 12)) - 0.5f)), (fx32)((1 > 0) ? ((1 * (1 << 12)) + 0.5f) : ((1 * (1 << 12)) - 0.5f)), 6);
        }
    }
}
