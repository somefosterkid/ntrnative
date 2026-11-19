#include "global.h"

#include "app_launch_warp_task.h"

#include "constants/heap.h"

#include "field_bgm.h"
#include "field_types_def.h"
#include "field_warp_tasks.h"
#include "heap.h"
#include "screen_fade.h"
#include "sound.h"
#include "sound_02004A44.h"
#include "task.h"
#include "unk_02005D10.h"
#include "unk_02055244.h"

void sub_02097810(FieldSystem *param_1, u8 param_2, void *param_3);

BOOL sub_020978D0(TaskManager *param_1);

BOOL sub_020978D0(TaskManager *param_1) {
    u16 sndseq;
    FieldSystem *fieldSystem;
    AppLaunchWarpTask *env;
    BOOL BVar1;

    fieldSystem = TaskManager_GetFieldSystem(param_1);
    env = TaskManager_GetEnvironment(param_1);
    switch (env->state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, HEAP_ID_FIELD2);
        GF_SndStartFadeOutBGM(0, 6);
        env->state = 1;
        break;
    case 1:
        BVar1 = IsPaletteFadeFinished();
        if ((BVar1 == 0) || (BVar1 = GF_SndGetFadeTimer(), BVar1 != 0)) {
            return 0;
        }
        sndseq = GF_GetCurrentPlayingBGM();
        StopBGM(sndseq, 0);
        CallTask_LeaveOverworld(param_1);
        env->state = 2;
        break;
    case 2:
        sub_02097810(fieldSystem, env->mode, &env->lights);
        env->state = 3;
        break;
    case 3:
        sub_020537F0(param_1, env->mapId, -1, env->x, env->y, (u32)env->direction);
        env->state = 4;
        break;
    case 4:
        FieldBGM_PlayForMapHeader(fieldSystem, env->mapId, 0);
        sub_02055408(param_1);
        env->state = 5;
        break;
    case 5:
        Heap_Free(env);
        return 1;
    }
    return 0;
}
