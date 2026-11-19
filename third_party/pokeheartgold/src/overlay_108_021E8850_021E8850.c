#include "global.h"
#include "overlay_108_021E8850.h"

#include "heap.h"
#include "overlay_108.h"
#include "overlay_108_safari_decoration_app.h"
#include "overlay_manager.h"
#include "safari_decoration_args.h"
#include "sound_02004A44.h"

BOOL SafariDecoration_Init(OverlayManager *man, int *state) {
    SafariDecorationApp *app;
    SafariDecorationArgs *args;

    switch (*state) {
    case 0:
        ov108_021E8AD4();
        Heap_Create(HEAP_ID_3, HEAP_ID_95, 0x48000U);
        args = OverlayManager_GetArgs(man);
        app = OverlayManager_CreateAndGetData(man, sizeof(SafariDecorationApp), HEAP_ID_95);
        MI_CpuFill8(app, 0U, sizeof(SafariDecorationApp));
        app->heapID = HEAP_ID_95;
        ov108_021E8968(app, args);
        Sound_SetSceneAndPlayBGM(0x43U, 0U, 0);
        *state += 1;
    default:
        break;
    case 1:
        app = OverlayManager_GetData(man);
        if (ov108_021E929C(app) != 0) {
            return 1;
        }
        break;
    }
    return 0;
}
