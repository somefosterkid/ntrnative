#include "global.h"

#include "gf_3d_vramman.h"
#include "gf_gfx_planes.h"
#include "camera.h"
#include "overlay_71_trade3dmodel.h"
#include "bg_window.h"
#include "sprite.h"
#include "overlay_71_trade_sequence_data.h"
#include "overlay_71_sprite_anim_resources.h"
#include "overlay_71_trade_ball_arrival_phase.h"
#include "filesystem_files_def.h"

struct BallArrivalPathState;

struct CameraAngle;

struct SysTask;

struct SysTaskQueue;

struct Trade3DModel;

struct Trade3DScene;

struct TradeBallArrivalPhase;

typedef struct Trade3DScene Trade3DScene;

Trade3DScene *ov71_0224744C(u32 modelCount, fx32 targetX, fx32 targetY, fx32 targetZ);

Trade3DModel *ov71_02247610(Trade3DScene *tradeScene, int modelIndex, enum NarcId narcID, u32 memberIndex, fx32 x, fx32 y, fx32 z, BOOL enabled);

void ov71_022477EC(Trade3DScene *tradeScene, VecFx32 *outPosition);

void ov71_0224780C(Trade3DScene *tradeScene, const CameraAngle *cameraAngle);

void ov71_0224781C(Trade3DScene *tradeScene, const CameraAngle *cameraAngle);

void ov71_0224783C(Trade3DScene *tradeScene, u16 fov);

typedef struct BallArrivalPathState {
    void *arrivalPhase;
    SysTask *task;
    Trade3DScene *scene;
    Trade3DModel *model;
    VecFx32 orbitCenter;
    VecFx32 orbitOffset;
    VecFx32 modelPos;
    VecFx16 rotation;
    fx32 orbitCenterStepY;
    fx32 orbitCenterStepZ;
    fx32 orbitRadiusX;
    fx32 orbitRadiusY;
    fx32 orbitRadiusXStep;
    fx32 orbitRadiusYStep;
    fx32 targetOrbitRadiusX;
    fx32 targetOrbitRadiusY;
    fx32 scale;
    fx32 scaleStep;
    fx32 targetScale;
    int transitionFrames;
    int subState;
    int exitTimer;
    int frameCount;
    fx16 rotationAngle;
    fx16 rotationSpeed;
} BallArrivalPathState;

void ov71_0224A3F0(TradeBallArrivalPhase *arrivalPhase);

void ov71_0224A3F0(TradeBallArrivalPhase *arrivalPhase) {
    static const GXRgb sEdgeColors[8] = {
        ((GXRgb)(((4) << (0)) | ((4) << (5)) | ((4) << (10)))),
        ((GXRgb)(((4) << (0)) | ((4) << (5)) | ((4) << (10)))),
        ((GXRgb)(((4) << (0)) | ((4) << (5)) | ((4) << (10)))),
        ((GXRgb)(((4) << (0)) | ((4) << (5)) | ((4) << (10)))),
        ((GXRgb)(((4) << (0)) | ((4) << (5)) | ((4) << (10)))),
        ((GXRgb)(((4) << (0)) | ((4) << (5)) | ((4) << (10)))),
        ((GXRgb)(((4) << (0)) | ((4) << (5)) | ((4) << (10)))),
        ((GXRgb)(((4) << (0)) | ((4) << (5)) | ((4) << (10)))),
    };

    NNS_G3dInit();

    G3X_InitMtxStack();
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(1);
    G3X_AlphaTest(0, 0);
    G3X_AlphaBlend(1);
    G3X_EdgeMarking(1);
    G3X_SetEdgeColorTable(sEdgeColors);
    G3X_SetClearColor(((GXRgb)(((31) << (0)) | ((31) << (5)) | ((16) << (10)))), 0, 0x7fff, 63, 0);
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
    G3_ViewPort(0, 0, 255, 191);

    GF_3DVramMan_InitFrameTexVramManager(1, 1);
    GF_3DVramMan_InitFramePlttVramManager(0x4000, 1);

    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, 1);
    G2_SetBG0Priority(0);

    arrivalPhase->scene = ov71_0224744C(1, 0, 0, 0);
    arrivalPhase->model = ov71_02247610(arrivalPhase->scene, 0, (enum NarcId)89, 27, 0, 0, 0, 0);

    CameraAngle cameraAngle;

    cameraAngle.x = 0x18E3;
    cameraAngle.y = 0;
    cameraAngle.z = 0;

    ov71_0224780C(arrivalPhase->scene, &cameraAngle);
    cameraAngle.x = 0x1150;

    ov71_0224781C(arrivalPhase->scene, &cameraAngle);
    ov71_0224783C(arrivalPhase->scene, ((22 * 0xffff) / 360) / 2);

    VecFx32 vecFx;
    ov71_022477EC(arrivalPhase->scene, &vecFx);
}
