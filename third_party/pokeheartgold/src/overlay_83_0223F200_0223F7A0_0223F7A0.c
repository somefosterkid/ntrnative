#include "global.h"

#include "bg_window.h"
#include "palette.h"
#include "sprite.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"
#include "overlay_83_battle_castle_self_app.h"

void ov83_0223F7A0(BattleCastleSelfApp *app);

void ov83_0224780C(s32 *);

void ov83_0223F7A0(BattleCastleSelfApp *app) {
    PaletteData *plttData = app->plttData;
    if (plttData != NULL) {
        PaletteData_PushTransparentBuffers(plttData);
    }
    DoScheduledBgGpuUpdates(app->bgConfig);
    ov83_0224780C(&app->unk_868);
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    OS_SetIrqCheckFlag(1UL << 0);
}
