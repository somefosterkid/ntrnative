#include "global.h"

#include "constants/heap.h"
#include "constants/sndseq.h"

#include "field/area_light.h"
#include "field_system.h"
#include "heap.h"
#include "overlay_02_02245B80.h"
#include "screen_fade.h"
#include "task.h"
#include "unk_02005D10.h"

BOOL ov02_02245BC8(TaskManager *taskManager) {
    LegendDemoScene *scene = TaskManager_GetEnvironment(taskManager);

    switch (scene->state) {
    case 0:
        ov02_02245D18(scene);
        scene->timer = 0;
        scene->soundFrame = 0;
        scene->state += 1;
        break;
    case 1:
        if (scene->soundFrame != 0xff) {
            if (scene->soundFrame == 0 || scene->soundFrame == 0x1e || scene->soundFrame == 0x32 || scene->soundFrame == 0x3c || scene->soundFrame == 0x46 || scene->soundFrame == 0x50) {
                PlaySE(SEQ_SE_DP_FW411);
                if (scene->soundFrame == 0x32) {
                    PlaySE(SEQ_SE_DP_F207);
                }
                if (scene->soundFrame >= 0x3c) {
                    PlaySE_SetPitch(SEQ_SE_DP_FW411, (s16)((scene->soundFrame / 10 - 5) << 6));
                }
            }
            scene->soundFrame = scene->soundFrame + 1;
        }
        ov02_02245DE0(scene);
        {
            u16 timer = scene->timer;
            scene->timer = timer + 1;
            if (timer >= 0x5a) {
                BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0x7fff, 0x18, 2, scene->heapID);
                scene->state += 1;
            }
        }
        break;
    case 2:
        ov02_02245DE0(scene);
        if (IsPaletteFadeFinished()) {
            scene->timer = 0;
            scene->state += 1;
        }
        break;
    case 3: {
        u16 timer = scene->timer;
        scene->timer = timer + 1;
        if (timer >= 0x3c) {
            ov02_02245DB0(scene);
            AreaLightManager_Free(&scene->fieldSystem->areaLightManager);
            scene->fieldSystem->areaLightManager = AreaLightManager_New(scene->fieldSystem->modelAttributes, 4);
            BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7fff, 0x12, 1, scene->heapID);
            scene->state += 1;
        }
    } break;
    case 4:
        if (IsPaletteFadeFinished()) {
            Heap_Free(scene);
            return TRUE;
        }
        break;
    }
    return FALSE;
}
