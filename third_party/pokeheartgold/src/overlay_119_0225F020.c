#include "global.h"

#include "constants/heap.h"

#include "camera.h"
#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_01_021EFB38.h"
#include "overlay_01_screen_slice_effect.h"
#include "overlay_120.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_02009D48.h"
#include "unk_0200FA24.h"

// Work area of the ov119_0225F020 encounter effect (EncounterEffect.param), 0x1e0 bytes on the DS.
typedef struct Ov119_0225F020Work {
    QuadraticInterpolationTaskFX32 scale;
    LinearInterpolationTaskS32 rotation;
    QuadraticInterpolationTaskFX32 sliceOffset;
    ScreenSliceEffect *sliceEffect;
    UnkStruct_ov5_021DE47C spriteEnv;
    UnkStruct_ov5_021DE5A4 spriteRes;
    Sprite *sprites[2];
    Camera *camera;
    QuadraticInterpolationTaskFX32 cameraDistance;
    int unk_1DC;
} Ov119_0225F020Work;

BOOL ov01_021EFE30(EncounterEffect *encEffect);

void ov119_0225F020(SysTask *task, void *param);

void ov119_0225F020(SysTask *task, void *param) {
    EncounterEffect *encEffect = param;
    s32 done;
    s32 i;
    Ov119_0225F020Work *work;
    fx32 scale;
    s32 iStack_60;
    VecFx32 scaleVec;
    VecFx32 leftVec;
    VecFx32 rightVec;
    VecFx32 leftTmp;
    VecFx32 rightTmp;

    work = encEffect->param;
    switch (encEffect->state) {
    case 0:
        encEffect->param = Heap_Alloc(HEAP_ID_FIELD1, sizeof(Ov119_0225F020Work));
        memset(encEffect->param, 0, sizeof(Ov119_0225F020Work));
        work = encEffect->param;
        work->camera = encEffect->fieldSystem->camera;
        work->sliceEffect = ov01_021EFFEC();
        ov01_021F05C4(&work->spriteEnv, 2, 1);
        ov01_021F0614(encEffect->narc, &work->spriteEnv, &work->spriteRes, 0, 1, 7, 9, 8, 600000);
        for (iStack_60 = 0; iStack_60 < 2; iStack_60++) {
            work->sprites[iStack_60] = ov01_021F0718(&work->spriteEnv, &work->spriteRes, 0x80000, 0x60000, 0, 0);
            Sprite_SetDrawFlag(work->sprites[iStack_60], 0);
            Sprite_SetDrawPriority(work->sprites[iStack_60], iStack_60 * 2);
        }
        GfGfx_EngineATogglePlanes(0x10, 1);
        encEffect->state++;
        break;
    case 1:
        ov01_021EFCF8(1, -16, -16, &encEffect->effectComplete, 2);
        encEffect->state++;
        break;
    case 2:
        if (encEffect->effectComplete != 0) {
            encEffect->state++;
        }
        break;
    case 3:
        ov01_021EFEC8(&work->scale, 0x29, 0x1000, 2, 10);
        scale = work->scale.currentValue;
        scaleVec = ov01_021F074C(scale, scale, scale);
        for (i = 0; i < 2; i++) {
            Sprite_SetDrawFlag(work->sprites[i], 1);
            Sprite_SetAffineOverwriteMode(work->sprites[i], 2);
            Sprite_SetAffineScale(work->sprites[i], &scaleVec);
        }
        ov01_021EFE34(&work->rotation, 0, 0xffff, 10);
        encEffect->state++;
        break;
    case 4:
        done = ov01_021EFF28(&work->scale);
        scale = work->scale.currentValue;
        scaleVec = ov01_021F074C(scale, scale, scale);
        ov01_021EFE44(&work->rotation);
        for (i = 0; i < 2; i++) {
            Sprite_SetAffineScale(work->sprites[i], &scaleVec);
        }
        Sprite_SetAffineZRotation(work->sprites[0], work->rotation.currentValue & 0xffff);
        Sprite_SetAffineZRotation(work->sprites[1], work->rotation.currentValue - 0x100U & 0xffff);
        if (done == 1) {
            encEffect->state++;
        }
        break;
    case 5:
        ov01_021F0050(encEffect, work->sliceEffect, 0x60, 6, 0, 0xff000, 0xa000);
        Sprite_SetAnimCtrlSeq(work->sprites[0], 1);
        Sprite_SetAnimCtrlSeq(work->sprites[1], 2);
        ov01_021EFEC8(&work->sliceOffset, 0, 0xff000, 0xa000, 6);
        scale = Camera_GetDistance(work->camera);
        ov01_021EFEC8(&work->cameraDistance, scale, scale + -0x1f4000, 0xffff6000, 6);
        Sprite_SetAffineZRotation(work->sprites[0], 0);
        Sprite_SetAffineZRotation(work->sprites[1], 0);
        encEffect->state++;
        break;
    case 6:
        ov01_021EFF28(&work->sliceOffset);
        leftTmp = ov01_021F074C(0x80000 - work->sliceOffset.currentValue, 0x60000, 0);
        leftVec = leftTmp;
        rightTmp = ov01_021F074C(work->sliceOffset.currentValue + 0x80000, 0x60000, 0);
        rightVec = rightTmp;
        Sprite_SetMatrix(work->sprites[0], &leftVec);
        Sprite_SetMatrix(work->sprites[1], &rightVec);
        ov01_021EFF28(&work->cameraDistance);
        Camera_SetDistance(work->cameraDistance.currentValue, work->camera);
        done = ov01_021EFE30(encEffect);
        if (done != 0) {
            encEffect->state++;
        }
        break;
    case 7:
        sub_0200FC20(0);
        if (encEffect->done != NULL) {
            *encEffect->done = 1;
        }
        for (i = 0; i < 2; i++) {
            Sprite_Delete(work->sprites[i]);
        }
        ov01_021F06EC(&work->spriteEnv, &work->spriteRes);
        ov01_021F05F4(&work->spriteEnv);
        ov01_021F0028(work->sliceEffect);
        ov01_021EFCDC(encEffect, task);
#if defined(SDK_PORT) || defined(NONMATCHING)
        return; // the effect and its work area were freed above; the DS reads them once more below
#endif
    }
    if (encEffect->state != 7) {
        SpriteList_RenderAndAnimateSprites(work->spriteEnv.unk_00);
    }
}
