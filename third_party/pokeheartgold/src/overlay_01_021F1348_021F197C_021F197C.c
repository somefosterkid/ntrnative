#include "global.h"

#include "gf_3d_loader.h"
#include "overlay_01_021F1348.h"
#include "sys_task_api.h"

// The field effect texture upload job: the VBlank task loads the texture, the VWait task frees it again.
typedef struct FieldEffectTexJob {
    s32 state;
    s32 id;
    GF_3DGfxRawResMan *resMan;
} FieldEffectTexJob; // sizeof == 0xC

void *ov01_021F141C(const FieldEffectManager *fieldEffMan, u32 size, BOOL allocAtEnd);

static void ov01_021F19B4(SysTask *task, void *data);

static void ov01_021F19D0(SysTask *task, void *data);

void ov01_021F197C(FieldEffectManager *fieldEffMan, u32 id, GF_3DGfxRawResMan *resMan) {
    FieldEffectTexJob *job;

    job = ov01_021F141C(fieldEffMan, sizeof(FieldEffectTexJob), TRUE);
    job->state = 1;
    job->id = id;
    job->resMan = resMan;
    SysTask_CreateOnVBlankQueue(ov01_021F19B4, job, 0xFFU);
    SysTask_CreateOnVWaitQueue(ov01_021F19D0, job, 0xFFU);
}

static void ov01_021F19B4(SysTask *task, void *data) {
    FieldEffectTexJob *job = data;

    GF3dGfxRawResMan_LoadObjTexById(job->resMan, job->id);
    job->state = 1;
    SysTask_Destroy(task);
}

static void ov01_021F19D0(SysTask *task, void *data) {
    FieldEffectTexJob *job = data;

    if (job->state == 1) {
        GF3dGfxRawResMan_FreeObjVramAndSecondaryHeaderById(job->resMan, job->id);
        ov01_021F1448(job);
        SysTask_Destroy(task);
    }
}
