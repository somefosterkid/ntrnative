// Black's code at ov203 0x021be150-0x021be7a8. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "app/demo3d.h"
#include "app/demo3d/demo3d_engine.h"
#include "gfl/g3d.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/fx/fx_vec.h"
#include "nitro/fx/fx_mtx.h"
#include "struct_decls.h"
#include "gfl/double3Ddisp.h"
#include "gfl/heap.h"
#include "gfl/tcb.h"
#include "system/rtc.h"
#include "app/demo3d/demo3d_cmdsys.h"
#include "field/field_light_status.h"

void func_ov203_021bf810(Demo3DCmdSys *sys);
void func_ov203_021be158(G3DCamera *camera);
void func_ov203_021be63c(Demo3DEngine *engine, Demo3DParam *param, const Demo3DSetup *setup, HeapID heapId);


const VecFx32 data_ov203_021bfce0 = { 0, 0, 0 };
const VecFx32 data_ov203_021bfcd4 = { 0, FX32_ONE, 0 };
const VecFx32 data_ov203_021bfcec = { 0, 0, 0 };

void func_ov203_021be150(void *data) {
    GFL_G3DDual3DExecVBlank();
}

void func_ov203_021be158(G3DCamera *camera) {
    GFL_G3DCameraFree(camera);
}

Demo3DEngine *func_ov203_021be160(Demo3DGraphic *graphic, Demo3DParam *param, HeapID heapId) {
    Demo3DEngine *engine = GFL_HeapAllocate(heapId, sizeof(Demo3DEngine), TRUE, "demo3d_engine.c", 121);
    G3DSceneSetup *sceneSetup;
    fx32 right;
    fx32 left;
    fx32 scale;
    fx32 width;
    fx32 top;
    fx32 height;
    fx16 sin;
    u32 i;
    int j;
    int k;
    fx32 near;
    fx32 far;
    fx32 scaleW;
    fx32 bottom;
    fx32 cos;

    engine->graphic = graphic;
    engine->demoId = param->demoId;
    engine->scene.demoId = engine->demoId;
    engine->scene.param = param->param;
    engine->scene.startFrame = param->unk08;
    engine->scene.dayPeriod = GetDayPeriod(param->season, param->hour);
    engine->scene.playerSex = param->playerSex;
    engine->scene.hour = param->hour;
    engine->scene.minute = param->minute;
    engine->scene.season = param->season;
    engine->setup = func_ov203_021be7a8(engine->demoId);

    cos = FX_CosIdx(FX_Whole(engine->setup->fovyCos) / 2 * 0x10000 / 360);
    scaleW = engine->setup->scaleW;
    far = engine->setup->far;
    near = engine->setup->near;
    sin = FX_SinIdx(FX_Whole(engine->setup->fovySin) / 2 * 0x10000 / 360);
    height = FX_Mul(near, sin);
    top = FX_Div(height, cos);
    bottom = -FX_Div(height, cos);
    width = FX_Mul(near, 0x1555);
    scale = FX32_ONE;
    right = FX_Div(FX_Mul(FX_Div(width, scale), sin), cos);
    left = -FX_Div(FX_Mul(FX_Div(width, scale), sin), cos);
    engine->camera = GFL_G3DCameraCreate(G3DCAM_PROJECTION_FRUSTUM, top, bottom, left, right, near, far, scaleW,
                                         &data_ov203_021bfce0, &data_ov203_021bfcd4, &data_ov203_021bfcec, heapId);
    GFL_G3DCameraOrthoGetTop(engine->camera, &engine->top);
    GFL_G3DCameraOrthoGetBottom(engine->camera, &engine->bottom);

    func_ov203_021be63c(engine, param, engine->setup, heapId);
    engine->frameStep = func_ov203_021be920(engine->demoId);
    engine->dual3D = func_ov203_021be938(engine->demoId);
    if (engine->dual3D) {
        GFL_G3DDual3DInit(heapId);
        GFL_VBlankSetCallback(func_ov203_021be150, NULL);
    }
    VEC_Set(&engine->srt.translation, 0, 0, 0);
    VEC_Set(&engine->srt.scale, FX32_ONE, FX32_ONE, FX32_ONE);
    MTX_Identity33_(&engine->srt.rotation);
    engine->unitCount = func_ov203_021be7d8(engine->demoId);
    engine->scenes = GFL_HeapAllocate(heapId, engine->unitCount * sizeof(u16), TRUE, "demo3d_engine.c", 183);
    engine->cameraAnm = func_ov203_021be7bc(engine->demoId, heapId, 10);
    GFL_G3DCurveFrameSet(engine->cameraAnm, FX32_CONST(engine->scene.startFrame));
    engine->g3dMgr = GFL_G3DMgrCreate(engine->unitCount * 6, engine->unitCount * 6, heapId);
    sceneSetup = func_ov203_021be7f0(heapId);
    for (j = 0; j < engine->unitCount; j++) {
        func_ov203_021be874(sceneSetup, &engine->scene, j);
        engine->scenes[j] = GFL_G3DMgrNewScene(engine->g3dMgr, sceneSetup);
    }
    func_ov203_021be85c(sceneSetup);
    for (i = 0; i < engine->unitCount; i++) {
        G3DActor *actor = GFL_G3DMgrGetActor(engine->g3dMgr, engine->scenes[i]);
        int count = GFL_G3DActorGetAnmCount(actor);

        for (k = 0; k < count; k++) {
            fx32 frame = FX32_CONST(engine->scene.startFrame);

            GFL_G3DActorBindAnm(actor, k);
            GFL_G3DActorSetAnmFrame(actor, k, &frame);
        }
    }
    engine->cmdSys = func_ov203_021bf7c0(engine, heapId);
    return engine;
}

void func_ov203_021be4b4(Demo3DEngine *engine) {
    u32 i;
    func_ov203_021bf810(engine->cmdSys);
    func_ov203_021be158(engine->camera);
    if (engine->dual3D != 0) {
        GFL_VBlankResetCallback();
        GFL_G3DDual3DFree();
    }
    GFL_G3DCurveFree(engine->cameraAnm);
    i = 0;
    while (i < (u32)engine->unitCount) {
        GFL_G3DMgrDeleteScene(engine->g3dMgr, engine->scenes[i]);
        i = i + 1;
    }
    GFL_G3DMgrFree(engine->g3dMgr);
    GFL_HeapFree(engine->scenes);
    GFL_HeapFree(engine);
}

void func_ov203_021be508(Demo3DEngine *engine, G3DCamera *camera) {
    s16 offset;

    if (GFL_G3DDual3DGetRenderDisp() == 0) {
        offset = engine->setup->mainOffset;
    } else {
        offset = engine->setup->subOffset;
    }
    GFL_G3DCameraOrthoSetTop(camera, offset + engine->top);
    GFL_G3DCameraOrthoSetBottom(camera, offset + engine->bottom);
}

BOOL func_ov203_021be538(Demo3DEngine *engine, u32 droppedFrames) {
    BOOL ended;
    fx32 frame;
    u32 i;
    int j;
    fx32 step;
    G3DActor *actor;

    step = 0;
    frame = GFL_G3DCurveGetNowFrame(engine->cameraAnm);
    for (i = 0; i < droppedFrames + 1; i++) {
        if (engine->stopped == 0) {
            func_ov203_021bf858(engine->cmdSys, frame + step, i);
        } else {
            func_ov203_021bf858(engine->cmdSys, -FX32_ONE, i);
        }
        step += engine->frameStep;
    }
    ended = GFL_G3DCurveFrameStep(engine->cameraAnm, step);
    GFL_G3DCurveApplyCamera(engine->camera, engine->cameraAnm);
    for (i = 0; i < engine->unitCount; i++) {
        int count;

        actor = GFL_G3DMgrGetActor(engine->g3dMgr, engine->scenes[i]);
        count = GFL_G3DActorGetAnmCount(actor);
        for (j = 0; j < count; j++) {
            GFL_G3DActorStepAnmFrameLoop(actor, j, step);
        }
    }
    if (engine->dual3D) {
        func_ov203_021be508(engine, engine->camera);
    }
    GFL_G3DCameraFlush(engine->camera);
    func_ov203_021bdf18(engine->graphic);
    for (i = 0; i < engine->unitCount; i++) {
        GFL_G3DSysDrawObj(GFL_G3DMgrGetActor(engine->g3dMgr, engine->scenes[i]), &engine->srt);
    }
    func_ov203_021bdf24(engine->graphic);
    if (engine->dual3D) {
        GFL_G3DDual3DRaiseNeedRenderFlag();
    }
    return ended;
}



fx32 func_ov203_021be620(Demo3DEngine *engine) {
    return GFL_G3DCurveGetNowFrame(engine->cameraAnm);
}

u32 func_ov203_021be62c(Demo3DEngine *engine) {
    return GFL_G3DCurveGetFrameCount(engine->cameraAnm);
}

void func_ov203_021be638(Demo3DEngine *engine, u32 stopped) {
    engine->stopped = stopped;
}

void func_ov203_021be63c(Demo3DEngine *engine, Demo3DParam *param, const Demo3DSetup *setup, HeapID heapId) {
    GXRgb colors[10];
    u32 fogTable[8];
    u32 i;

    func_0201493c(setup->zoneId, param->hour, param->minute, 0, param->season, colors, heapId);
    func_ov203_021bdf3c(engine->graphic, colors, 0);
    G3X_AntiAlias(setup->antiAlias);
    G3X_AlphaTest(setup->alphaTest, engine->setup->alphaTestRef);
    G3X_AlphaBlend(setup->alphaBlend);
    G3X_EdgeMarking(setup->edgeMarking);
    if (setup->edgeMarking) {
        G3X_SetEdgeColorTable(setup->edgeColors);
    }
    G3X_SetFog(setup->fog, setup->fogAlphaMode, setup->fogShift, setup->fogOffset);
    if (setup->fog) {
        G3X_SetFogColor(setup->fogColor, setup->fogAlpha);
        for (i = 0; i < NELEMS(fogTable); i++) {
            fogTable[i] = setup->fogTable[i];
        }
        G3X_SetFogTable(fogTable);
    }
    G3X_SetClearColor(setup->clearColor, setup->clearAlpha, setup->clearDepth, setup->clearPolygonId, setup->fog);
    G3X_SetDisp1DotDepth(setup->polygon1DotDepth);
    GFL_G3DSysSetSwapBufferParams(setup->sortMode, setup->bufferMode);
}


u32 func_ov203_021be79c(u32 demoId) {
    if (demoId == 0 || demoId >= 19) {
        return 0;
    }
    return demoId;
}

