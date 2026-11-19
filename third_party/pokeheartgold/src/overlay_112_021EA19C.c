#include "global.h"
struct UnkStruct_ov112_021EA230;

#include "render_window.h"
#include "gf_gfx_planes.h"
#include "sprite_system.h"
#include "unk_02009D48.h"
#include "vram_transfer_manager.h"
#include "overlay_112_unk_struct_ov112_021_ea230.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021EA19C(Ov112PokewalkerApp *arg0);

s32 ov112_021EA230(void *);
s32 ov112_021EA2C4(Ov112PokewalkerApp *);
s32 ov112_021EA3C0(void *);
s32 ov112_021EAB58(void);
void ov112_021EA1AC(Ov112PokewalkerApp *arg0);
void ov112_021EA1F8(Ov112PokewalkerApp *arg0);

void ov112_021EA19C(Ov112PokewalkerApp *arg0) {
    sub_0200F450(arg0->waitingIcon);
}

void ov112_021EA1AC(Ov112PokewalkerApp *arg0) {
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
    GF_CreateVramTransferManager(0x40U, HEAP_ID_154);
    ov112_021EA230(arg0);
    ov112_021EA2C4(arg0);
    ov112_021EA3C0(arg0);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(arg0->spriteSystem), 0, 0x100000);
    ov112_021EAB58();
}

void ov112_021EA1F8(Ov112PokewalkerApp *arg0) {
    u32 v0;

    v0 = 0;
    do {
        Sprite_DeleteAndFreeResources(arg0->sprites[v0]);
        v0 += 1;
    } while (v0 < 0xD9U);
    SpriteSystem_FreeResourcesAndManager(arg0->spriteSystem, arg0->spriteManager);
    SpriteSystem_Free(arg0->spriteSystem);
}
