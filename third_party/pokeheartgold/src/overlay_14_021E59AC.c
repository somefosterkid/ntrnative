#include "global.h"

#include "bg_window.h"
#include "palette.h"
#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"




void ov14_021E5A14(void *arg0);

void ov14_021F29C4(UnkStruct_ov14_boxsys *arg0);

void ov14_021E59AC(SysTask *task, void *param);

void ov14_021E59AC(SysTask *task, void *param) {
    UnkStruct_ov14_boxapp *app = param;
    UnkFuncPtr_ov14_boxtask fn;

    fn = app->sys->unk_4;
    if (fn != NULL && fn(app) == 0) {
        app->sys->unk_4 = NULL;
    }
    ov14_021E5A14(app);
    PaletteData_PushTransparentBuffers(app->sys->unk_18);
    DoScheduledBgGpuUpdates(app->sys->unk_14);
    ov14_021F29C4(app->sys);
    if (app->sys->unk_88D2 == 0) {
        SpriteSystem_DrawSprites(app->sys->unk_2F8);
        SpriteSystem_TransferOam();
    }
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
