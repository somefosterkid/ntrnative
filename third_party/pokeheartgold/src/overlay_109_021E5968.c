#include "global.h"

#include "overlay_109_photo_album_app.h"
#include "overlay_manager.h"
#include "sprite_system.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "screen_fade.h"
#include "system.h"

BOOL PhotoAlbum_Main(OverlayManager *man, int *state) {
    PhotoAlbumApp *app;
    s32 iVar1;
    s32 sVar2;

    app = OverlayManager_GetData(man);
    switch (*state) {
    case 0:
        sVar2 = ov109_021E5BAC(app);
        *state = sVar2;
        break;
    case 1:
        iVar1 = ov109_021E5C40(app);
        *state = iVar1;
        break;
    case 2:
        sVar2 = ov109_021E5C48(app);
        *state = sVar2;
        break;
    case 3:
        sVar2 = ov109_021E5C68(app);
        *state = sVar2;
        break;
    case 4:
        iVar1 = ov109_021E5C88(app);
        *state = iVar1;
        break;
    case 5:
        iVar1 = ov109_021E5C90(app);
        *state = iVar1;
        break;
    case 6:
        sVar2 = ov109_021E5C98(app);
        *state = sVar2;
        break;
    case 7:
        iVar1 = ov109_021E5CAC(app);
        *state = iVar1;
        break;
    case 8:
        sVar2 = ov109_021E5BF4(app);
        *state = sVar2;
        break;
    case 9:
        return 1;
    }
    SpriteSystem_DrawSprites(app->spriteManager);
    return 0;
}

BOOL PhotoAlbum_Exit(OverlayManager *man, int *state) {
    PhotoAlbumApp *v0;

    v0 = OverlayManager_GetData(man);
    if (ov109_021E5B98(v0) == 0) {
        return 0;
    }
    ov109_021E5A20();
    ov109_021E5B40(v0);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_96);
    return 1;
}

void ov109_021E5A20(void) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    sub_0200FBF4(PM_LCD_TOP, 0U);
    sub_0200FBF4(PM_LCD_BOTTOM, 0U);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
}
