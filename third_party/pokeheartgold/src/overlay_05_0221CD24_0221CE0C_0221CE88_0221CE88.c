#include "global.h"

#include "bg_window.h"
#include "overlay_05.h"
#include "overlay_05_unk_struct_ov10_0221_fb28_t.h"
#include "palette.h"
#include "sprite_system.h"
#include "vram_transfer_manager.h"

void ov05_0221CE88(void *param0) {
    UnkStruct_ov10_0221FB28 *v0 = param0;

    DoScheduledBgGpuUpdates(v0->unk_0C);
    PaletteData_PushTransparentBuffers(v0->unk_08);
    GF_RunVramTransferTasks();
    SpriteSystem_TransferOam();

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
