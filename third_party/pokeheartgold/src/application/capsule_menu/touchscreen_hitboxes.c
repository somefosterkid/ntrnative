#include "application/capsule_menu/defs.h"

#include "pokemon.h"

static TouchscreenHitbox CapsuleUIRects[] __attribute__((section(BWPORT_DATA_SECTION))) = {
    { .rect = { .top = 0, .bottom = 0, .left = 0, .right = 0 } },
    { .rect = { .top = 0, .bottom = 0, .left = 0, .right = 0 } },
    { .rect = { .top = 0, .bottom = 0, .left = 0, .right = 0 } },
    { .rect = { .top = 0, .bottom = 0, .left = 0, .right = 0 } },
    { .rect = { .top = 0, .bottom = 0, .left = 0, .right = 0 } },
    { .rect = { .top = 0, .bottom = 0, .left = 0, .right = 0 } },
    { .rect = { .top = 0, .bottom = 0, .left = 0, .right = 0 } },
    { .rect = { .top = 0, .bottom = 0, .left = 0, .right = 0 } },
    { .rect = { .top = 0, .bottom = 0, .left = 0, .right = 0 } },
    { .rect = { .top = 0, .bottom = 0, .left = 0, .right = 0 } },
    { .rect = { .top = 0, .bottom = 0, .left = 0, .right = 0 } },
    { .rect = { .top = 0, .bottom = 0, .left = 0, .right = 0 } },
    { .rect = { .top = 0, .bottom = 0, .left = 0, .right = 0 } },
};

void ov57_0223BB84(CapsuleAppManager *appMan) {
    int i;
    const TouchscreenHitbox emptyRect = { 0 };

    for (i = 0; i < 13; ++i) {
        appMan->graphicsMan.touchscreenHitboxes[i] = CapsuleUIRects[i];
    }
    for (; i < 21; ++i) {
        appMan->graphicsMan.touchscreenHitboxes[i] = emptyRect;
        appMan->sealRenderInfo[i - 13].touchscreenHitbox = &appMan->graphicsMan.touchscreenHitboxes[i];
    }
    appMan->graphicsMan.touchHitboxController = TouchHitboxController_Create(appMan->graphicsMan.touchscreenHitboxes, 21, CapsuleManager_HandleSpritePress, appMan, HEAP_ID_CAPSULE_MENU);
}

void CapsuleManager_AssignCapsuleMon(CapsuleAppManager *appMan, int capsuleIndex) {
    u32 sp0 = 0;

    if (appMan->capsules[capsuleIndex].partyIdx != 0xFF) {
        SetMonData(appMan->appData->mons[appMan->capsules[capsuleIndex].partyIdx], MON_DATA_BALL_CAPSULE_ID, &sp0);
    }
    appMan->capsules[capsuleIndex].partyIdx = 0xFF;
    CapsuleGraphics_SetCapsuleSpriteAnim(appMan);
}

// tools/hg_assets.py: the tables above are blanks, port/src/asset_load.c fills them from the ROM at start
void PORT_AssetAdd(const char *game, const char *name, void *p, unsigned long size, const char *layout);
__attribute__((constructor)) static void hgasset_register_src_application_capsule_menu_touchscreen_hitboxes_c_0(void) {
    PORT_AssetAdd("IPKE", "src/application/capsule_menu/touchscreen_hitboxes.c:CapsuleUIRects", (void *)&CapsuleUIRects, sizeof(CapsuleUIRects), "u8");
    PORT_AssetAdd("IPGE", "src/application/capsule_menu/touchscreen_hitboxes.c:CapsuleUIRects", (void *)&CapsuleUIRects, sizeof(CapsuleUIRects), "u8");
}
