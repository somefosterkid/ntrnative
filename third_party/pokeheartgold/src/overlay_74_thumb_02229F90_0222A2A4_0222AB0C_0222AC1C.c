#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "overlay_74_mystery_gift_templates.h"
#include "overlay_manager.h"
#include "overlay_74_mystery_gift_app_data.h"
#include "unk_02005D10.h"

void ov74_02229E28(MysteryGiftAppData *, BOOL);

void ov74_0222A94C(OverlayManager *, u32, u32);

u32 ov74_02236AE0(OverlayManager *, int *);

void ov74_0222AC1C(OverlayManager *param_1, int *param_2) {
    MysteryGiftAppData *puVar1;
    u32 uVar2;

    puVar1 = OverlayManager_GetData(param_1);
    uVar2 = ov74_02236AE0(param_1, &puVar1->nasAuthState);
    switch (uVar2) {
    case 1:
        memcpy(&puVar1->eventData, puVar1->wifiDistributionBuffer, sizeof(MysteryGiftEventData));
        ov74_02229E28(puVar1, 0);
        puVar1->foundGiftType = 0x1b;
        *param_2 = 0x13;
        puVar1->giftSearchResultDelay = 0;
        return;
    case 2:
    case 3:
        ov74_02229E28(puVar1, 0);
        puVar1->giftSearchResultDelay = 0;
        *param_2 = 0x11;
        return;
    case 4:
        ClearWindowTilemapAndCopyToVram(&puVar1->wifiCommErrorWindow);
        RemoveWindow(&puVar1->wifiCommErrorWindow);
        BgClearTilemapBufferAndCommit(puVar1->bgConfig, 0);
        PlaySE(0x5dc);
        ov74_0222A94C(param_1, 0xc4, 0);
        GfGfx_EngineATogglePlanes(0x10, 1);
        *param_2 = 3;
    }
    return;
}
