#include "global.h"

#include "bg_window.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"
#include "pm_string.h"
#include "options.h"
#include "sprite.h"
#include "sprite_system.h"
#include "mail_message_types.h"
#include "save_frontier.h"
#include "message_format.h"
#include "save_frontier.h"
#include "msgdata.h"
#include "overlay_72_wfc_connect.h"

void TransferSpriteSystemOam(void);

void ov72_022380FC(void *data);

void ov72_022380FC(void *data) {
    NintendoWfcConnect *wfc = data;

    GF_RunVramTransferTasks();
    DoScheduledBgGpuUpdates(wfc->bgConfig);
    OamManager_ApplyAndResetBuffers();
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
