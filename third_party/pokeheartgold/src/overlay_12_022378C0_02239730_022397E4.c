#include "global.h"

#include "bg_window.h"
#include "overlay_12_022378C0.h"
#include "palette.h"
#include "vram_transfer_manager.h"
#include "overlay_12_field_battle_dto.h"
#include "save_palpad.h"

struct FieldBattleDTO;

struct SavePalPad;

typedef struct LinkBattleCommState {
    FieldBattleDTO *dto;
    BgConfig *bgConfig;
    Window *window;
    PaletteData *paletteData;
    SavePalPad *palPad[4];
    u8 sendBuffer[0x1000];
    u8 recvCount;
    u8 handshakeStep;
    u16 syncTimer;
    void *waitDial;
} LinkBattleCommState;

void ov12_022397E4(void *commState) {
    LinkBattleCommState *linkBattleCommState = commState;

    PaletteData_PushTransparentBuffers(linkBattleCommState->paletteData);
    GF_RunVramTransferTasks();
    DoScheduledBgGpuUpdates(linkBattleCommState->bgConfig);

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
