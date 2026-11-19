#include "global.h"

#include "constants/heap.h"

#include "library/spl_emitter.h"
#include "library/spl_manager.h"

#include "bg_window.h"
#include "camera.h"
#include "frontier_map.h"
#include "gf_3d_vramman.h"
#include "overlay_42.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02014DA0.h"
#include "vram_transfer_manager.h"
#include "overlay_80_unk_struct_ov104_0223_c634.h"
#include "particle_system.h"
#include "frontier_graphics.h"
#include "msgdata.h"
#include "overlay_45_unk_struct_ov66_02234798.h"

typedef struct BattleFrontier BattleFrontier;

typedef struct UnkStruct_ov63_0222BCE8_t UnkStruct_ov63_0222BCE8;

typedef struct UnkStruct_ov63_0222BE18_t UnkStruct_ov63_0222BE18;

typedef struct UnkStruct_ov63_0222D1C0_t UnkStruct_ov63_0222D1C0;

typedef struct UnkStruct_ov63_0222D77C_t UnkStruct_ov63_0222D77C;

typedef struct UnkStruct_ov63_0222D848_t UnkStruct_ov63_0222D848;

typedef struct FrontierParticleSystem FrontierParticleSystem;

struct FrontierParticleSystem {
    ParticleSystem *particleSys[8];
    u16 heapID;
};

typedef struct {
    SysTask *unk_00;
    s16 unk_04;
    s16 unk_06;
    s8 unk_08;
    s8 unk_09;
    s8 unk_0A;
    u8 unk_0B;
} UnkStruct_ov104_0223319C;

typedef struct {
    SysTask *unk_00;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06;
    u8 unk_07;
    s16 unk_08;
    s8 unk_0A;
    u8 unk_0B;
} UnkStruct_ov104_022331E8;

typedef struct {
    ManagedSprite *unk_00[8];
    u16 unk_20[8];
    u32 unk_30;
    u16 unk_34[8];
} UnkStruct_ov104_0223D3B0_1;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    s16 unk_02[7];
} UnkStruct_ov104_0223D8F0;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    s16 unk_06;
    s16 unk_08;
    u8 unk_0A;
    u8 unk_0B;
    u8 unk_0C;
    UnkStruct_ov104_0223D8F0 unk_0E;
} UnkStruct_ov104_0223D570;

UnkStruct_ov63_0222BEC0 *ov80_02239510(FrontierGraphics *param0, const UnkStruct_ov104_0223D570 *param1, int param2);

void ov80_02239828(FrontierGraphics *param0);

s32 ov80_02239900(void *, UnkStruct_ov104_0223D570 *);

void *sub_02096864(BattleFrontier *);

void *sub_0209686C(BattleFrontier *, s32);

void ov80_02238A18(FrontierGraphics *param0);

void ov80_02238A18(FrontierGraphics *param0) {
    UnkStruct_ov104_0223D570 v0;
    s32 v4;
    s32 v5;
    u16 v2;
    void *v1;
    void *v3;

    v3 = sub_02096864(param0->frontier);
    v5 = 0;
    do {
        v2 = *(u16 *)v3;
        if (v2 != 0xFFFF) {
            ov42_02228FE0((UnkStruct_ov42_02228EDC *)param0->unk_20, (s32)v2, (s32) * (u8 *)((u8 *)v3 + 2), HEAP_ID_101);
        }
        v5 += 1;
        v3 += 4;
    } while (v5 < 0x18);
    v4 = 0;
    do {
        v1 = sub_0209686C(param0->frontier, v4);
        if (((UnkStruct_ov104_0223C634 *)v1)->unk_08.unk_04 != 0xFFFF) {
            ov80_02239900(v1, &v0);
            ov80_02239510(param0, &v0, v4);
        }
        v4 += 1;
    } while (v4 < 0x20);
    ov80_02239828(param0);
}

struct MsgData;

struct ScreenCapture;

struct SpearPillarCutsceneArgs;

typedef struct SpearPillarCutsceneArgs SpearPillarCutsceneArgs;

typedef struct BlendColor {
    u8 r;
    u8 g;
    u8 b;
    s8 brightness;
} BlendColor;

typedef struct CameraPan {
    Camera *camera;
    VecFx32 *target;
    int durationFrames;
    s32 angleDeltaX;
    s32 angleDeltaY;
    s32 angleDeltaZ;
    fx32 positionDeltaX;
    fx32 positionDeltaY;
    fx32 positionDeltaZ;
    struct {
        s32 framesRemaining;
        s32 x;
        s32 y;
        s32 z;
        s32 unused_10;
    } angleStep;
    struct {
        fx32 unused_00;
        fx32 x;
        fx32 y;
        fx32 z;
        fx32 unused_10;
    } positionStep;
    CameraAngle cameraAngle;
} CameraPan;

typedef struct CameraShakeTask {
    int intensity;
    int step;
    int direction;
    int unused_0C;
    CameraPan *cameraPan;
} CameraShakeTask;

typedef struct ScreenCapture ScreenCapture;

typedef struct CutsceneGraphics {
    BgConfig *bgConfig;
    PaletteData *paletteData;
    GF3DVramMan *g3dPipeline;
    Camera *camera;
    NNSFndAllocator allocator;
    MsgData *messageLoader;
    Window messageWindow;
    int messagePrinter;
    VecFx32 cameraTarget;
    BlendColor tint;
    ScreenCapture *screenCapture;
    CameraPan cameraPan;
    CameraShakeTask cameraShake;
    u8 flashState;
    u8 flashIntensity;
    u8 flashTimer;
} CutsceneGraphics;

typedef struct SpearPillarCutsceneData {
    CutsceneGraphics scene;
    SpearPillarCutsceneArgs *args;
} SpearPillarCutsceneData;

void FrontierMap_VBlank(void *param) {
    SpearPillarCutsceneData *cutscene = param;

    GF_RunVramTransferTasks();
    SpriteSystem_TransferOam();
    PaletteData_PushTransparentBuffers(cutscene->scene.paletteData);
    DoScheduledBgGpuUpdates(cutscene->scene.bgConfig);

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

void ov80_02238AAC(SysTask *task, void *data) {
}
