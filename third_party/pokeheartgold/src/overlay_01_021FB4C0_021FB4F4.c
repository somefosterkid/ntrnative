#include "global.h"

#include "constants/heap.h"

#include "system.h"

#include "field/hblank_system_priv.h"
void ov01_021FB594(void *param);

void HBlankSystem_Start(HBlankSystem *hBlankSystem);

void HBlankSystem_Start(HBlankSystem *hBlankSystem) {
    BOOL result = Main_SetHBlankIntrCB(ov01_021FB594, hBlankSystem);
    ((result == 1) ? (void)0 : GF_AssertFail());

    hBlankSystem->active = 1;
}
