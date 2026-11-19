#include "global.h"

#include "bg_window.h"
#include "palette.h"
#include "sprite_system.h"
#include "vram_transfer_manager.h"

typedef struct UnkStruct_sub_020855CC {
    u8 unk_0[0x2F8];
    BgConfig *unk_2F8;
    PaletteData *unk_2FC;
} UnkStruct_sub_020855CC;

void sub_020855CC(UnkStruct_sub_020855CC *arg0);

void sub_020855CC(UnkStruct_sub_020855CC *arg0) {
    GF_RunVramTransferTasks();
    SpriteSystem_TransferOam();
    PaletteData_PushTransparentBuffers(arg0->unk_2FC);
    DoScheduledBgGpuUpdates(arg0->unk_2F8);
#if defined(SDK_PORT) || defined(NONMATCHING)
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
#else
    *(s32 *)((u8 *)OS_IRQTable + 0x3FF8) = (s32)(1 | *(s32 *)((u8 *)OS_IRQTable + 0x3FF8));
#endif
}
