#include "global.h"

#include "heap.h"
#include "overlay_108_021E8850.h"
#include "overlay_108_safari_decoration_app.h"
#include "overlay_manager.h"
#include "sprite_system.h"

BOOL SafariDecoration_Exit(OverlayManager *man, int *state) {
    SafariDecorationApp *app;
    enum HeapID heapID;
    app = OverlayManager_GetData(man);
    if (ov108_021E9304(app) == 0) {
        return 0;
    }
    ov108_021E8AD4();
    ov108_021E8A88(app);
    heapID = app->heapID;
    MI_CpuFill8(app, 0U, sizeof(SafariDecorationApp));
    OverlayManager_FreeData(man);
    Heap_Destroy(heapID);
    return 1;
}

BOOL SafariDecoration_Main(OverlayManager *man, int *state) {
    SafariDecorationApp *arg0;
    s32 iVar1;
    s32 sVar2;

    arg0 = OverlayManager_GetData(man);
    switch (*state) {
    case 0:
        sVar2 = ov108_021E8BC0(arg0);
        *state = sVar2;
        break;
    case 1:
        iVar1 = ov108_021E8B24(arg0);
        *state = iVar1;
        break;
    case 2:
        sVar2 = ov108_021E8B68(arg0);
        *state = sVar2;
        break;
    case 3:
        sVar2 = ov108_021E8C18(arg0);
        *state = sVar2;
        break;
    case 4:
        return 1;
    }
    SpriteSystem_DrawSprites(arg0->spriteManager);
    ov108_021E8E10(&arg0->view);
    return 0;
}
