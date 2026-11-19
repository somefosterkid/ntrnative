#include "global.h"

#include "gf_3d_vramman.h"
#include "gf_gfx_planes.h"
#include "overlay_71_trade_ball_flyby_phase.h"
#include "camera.h"
#include "overlay_71_trade3dmodel.h"
#include "bg_window.h"
#include "sprite.h"
#include "overlay_71_trade_sequence_data.h"
#include "overlay_71_sprite_anim_resources.h"
#include "filesystem_files_def.h"

struct BallOrbitState;

struct CameraAngle;

struct SysTask;

struct SysTaskQueue;

struct Trade3DModel;

struct Trade3DScene;

struct TradeBallFlybyPhase;

typedef struct Trade3DScene Trade3DScene;

Trade3DScene *ov71_0224744C(u32 modelCount, fx32 targetX, fx32 targetY, fx32 targetZ);

Trade3DModel *ov71_02247610(Trade3DScene *tradeScene, int modelIndex, enum NarcId narcID, u32 memberIndex, fx32 x, fx32 y, fx32 z, BOOL enabled);

void ov71_022477EC(Trade3DScene *tradeScene, VecFx32 *outPosition);

void ov71_0224780C(Trade3DScene *tradeScene, const CameraAngle *cameraAngle);

void ov71_0224783C(Trade3DScene *tradeScene, u16 fov);

typedef struct BallOrbitState {
    void *phase;
    SysTask *taskHandle;
    Trade3DScene *scene;
    Trade3DModel *model;
    VecFx32 center;
    VecFx32 orbitOffset;
    VecFx32 currentPos;
    VecFx16 rotation;
    fx32 centerYStep;
    fx32 centerZStep;
    fx32 orbitRadiusX;
    fx32 orbitRadiusY;
    fx32 orbitRadiusXStep;
    fx32 orbitRadiusYStep;
    fx32 scale;
    fx32 scaleStep;
    int transitionFrames;
    int state;
    int timer;
    int frameCount;
    fx16 angle;
    fx16 angleSpeed;
} BallOrbitState;

void ov71_022497E0(TradeBallFlybyPhase *tbfPhase);

void ov71_022497E0(TradeBallFlybyPhase *tbfPhase) {
    static const GXRgb sEdgeColors[8] = {
        ((GXRgb)(((4) << (0)) | ((4) << (5)) | ((4) << (10)))),
        ((GXRgb)(((4) << (0)) | ((4) << (5)) | ((4) << (10)))),
        ((GXRgb)(((4) << (0)) | ((4) << (5)) | ((4) << (10)))),
        ((GXRgb)(((4) << (0)) | ((4) << (5)) | ((4) << (10)))),
        ((GXRgb)(((4) << (0)) | ((4) << (5)) | ((4) << (10)))),
        ((GXRgb)(((4) << (0)) | ((4) << (5)) | ((4) << (10)))),
        ((GXRgb)(((4) << (0)) | ((4) << (5)) | ((4) << (10)))),
        ((GXRgb)(((4) << (0)) | ((4) << (5)) | ((4) << (10))))
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

    tbfPhase->scene = ov71_0224744C(1, 0, 0, 0);
    tbfPhase->model = ov71_02247610(tbfPhase->scene, 0, (enum NarcId)89, 27, 0, 0, 0, 0);

    CameraAngle cameraAngle;

    cameraAngle.x = 6371;
    cameraAngle.y = 0;
    cameraAngle.z = 0;

    ov71_0224780C(tbfPhase->scene, &cameraAngle);
    ov71_0224783C(tbfPhase->scene, ((22 * 0xffff) / 360) / 2);

    VecFx32 vecFx;
    ov71_022477EC(tbfPhase->scene, &vecFx);
}
