#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "options.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "vram_transfer_manager.h"
#include "unk_02014DA0.h"
#include "unk_02026E30.h"
#include "pokemon_types_def.h"
#include "overlay_74_main_menu_util.h"
#include "naming_screen.h"
#include "overlay_95_egg_hatch_app.h"
#include "egg_hatch_particle_system.h"

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);


void ov95_021E5B24(void *arg);

void ov95_021E5B58(void);

void ov95_021E5B24(void *arg) {
    EggHatchCutscene *eggHatch = arg;

    PokepicManager_HandleLoadImgAndOrPltt(eggHatch->graphics.monSpriteMan);
    GF_RunVramTransferTasks();
    SpriteSystem_TransferOam();
    PaletteData_PushTransparentBuffers(eggHatch->graphics.plttData);
    DoScheduledBgGpuUpdates(eggHatch->graphics.bgConfig);

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

void ov95_021E5B58(void) {
    int result;

    Thunk_G3X_Reset();

    result = sub_0201543C();

    if (result > 0) {
        Thunk_G3X_Reset();
        NNS_G2dSetupSoftwareSpriteCamera();
    }

    sub_02015460();
    RequestSwap3DBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
}
