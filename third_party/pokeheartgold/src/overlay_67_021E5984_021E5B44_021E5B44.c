#include "global.h"

#include "gf_gfx_planes.h"
#include "overlay_67_app.h"
#include "system.h"

BOOL ov67_021E5B44(Ov67App *app) {
    switch (app->state) {
    case 0:
        if (app->isRankingsScreen) {
            ov67_021E6C04(app);
            ov67_021E6474(app);
        }
        ov67_021E6990(app);
        ov67_021E6080(app);
        ov67_021E60F4(app);
        ov67_021E5E84(app);
        break;
    case 1:
        Main_SetVBlankIntrCB(0L, 0L);
        HBlankInterruptDisable();
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        GX_SetVisiblePlane(0);
        GXS_SetVisiblePlane(0);
        app->state = 0;
        return 1;
    }
    app->state++;
    return 0;
}
