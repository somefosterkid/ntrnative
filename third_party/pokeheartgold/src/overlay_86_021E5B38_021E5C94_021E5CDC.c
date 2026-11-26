#include "global.h"

#include "bg_window.h"
#include "overlay_86_frontier_records_app.h"
#include "palette.h"
#include "sprite_system.h"

void ov86_021E5CDC(void *data) {
    FrontierRecordsApp *app = data;

    if (app->plttData != NULL) {
        PaletteData_PushTransparentBuffers(app->plttData);
    }

    DoScheduledBgGpuUpdates(app->bgConfig);
    SpriteSystem_TransferOam();

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
