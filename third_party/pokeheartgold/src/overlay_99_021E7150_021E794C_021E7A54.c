#include "global.h"

#include "overlay_99_pokeathlon_medals.h"
#include "bg_window.h"
#include "sprite_system.h"

struct BgConfig;

void OamManager_ApplyAndResetBuffers(void);


void ov99_021E7A54(void *param0);

void ov99_021E7A54(void *param0) {
    SpriteSystem_TransferOam();
    DoScheduledBgGpuUpdates(((PokeathlonMedals *)param0)->bgConfig);

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
