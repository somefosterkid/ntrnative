#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "overlay_74_mystery_gift_templates.h"
#include "overlay_manager.h"
#include "unk_02005D10.h"

void ov74_02229E28(u32 *, u32);

void ov74_0222A94C(OverlayManager *, u32, u32);

u32 ov74_02236AE0(OverlayManager *, u32 *);

void ov74_0222AC1C(OverlayManager *param_1, int *param_2) {
    u32 *puVar1;
    u32 uVar2;

    puVar1 = OverlayManager_GetData(param_1);
    uVar2 = ov74_02236AE0(param_1, puVar1 + 0x578);
    switch (uVar2) {
    case 1:
        memcpy(puVar1 + 0x23, puVar1 + 0x596, 0x3a8);
        ov74_02229E28(puVar1, 0);
        puVar1[0x21] = 0x1b;
        *param_2 = 0x13;
        puVar1[0x577] = 0;
        return;
    case 2:
    case 3:
        ov74_02229E28(puVar1, 0);
        puVar1[0x577] = 0;
        *param_2 = 0x11;
        return;
    case 4:
        ClearWindowTilemapAndCopyToVram((Window *)(puVar1 + 0x9a0));
        RemoveWindow((Window *)(puVar1 + 0x9a0));
        BgClearTilemapBufferAndCommit((BgConfig *)*puVar1, 0);
        PlaySE(0x5dc);
        ov74_0222A94C(param_1, 0xc4, 0);
        GfGfx_EngineATogglePlanes(0x10, 1);
        *param_2 = 3;
    }
    return;
}
