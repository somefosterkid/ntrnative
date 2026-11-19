#include "global.h"

#include "gf_gfx_planes.h"
#include "sys_task.h"
#include "frontier_map.h"
#include "sprite_system.h"
#include "unk_02009D48.h"
#include "frontier_map_internal.h"
#include "gf_3d_vramman.h"

void GF_3DVramMan_Delete(GF3DVramMan *vramMan);

void ov80_022392F8(void);
void ov80_0223937C(GF3DVramMan *vramMan);

extern OamCharTransferParam ov80_0223D570;
extern SpriteResourceCountsListUnion ov80_0223D584;
extern OamManagerParam ov80_0223D5B8;

void ov80_022392F8(void) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, 1);

    G2_SetBG0Priority(1);
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(1);
    G3X_AlphaTest(0, 0);
    G3X_AlphaBlend(1);
    G3X_EdgeMarking(0);
    G3X_SetFog(0, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(((GXRgb)(((0) << (0)) | ((0) << (5)) | ((0) << (10)))), 0, 0x7fff, 63, 0);
    G3_ViewPort(0, 0, 255, 191);
}

void ov80_0223937C(GF3DVramMan *vramMan) {
    GF_3DVramMan_Delete(vramMan);
}

void ov80_02239384(void *arg0) {
    SpriteSystem *spriteSystem;

    spriteSystem = SpriteSystem_Alloc(HEAP_ID_101);
    ((FrontierMapInternal *)arg0)->spriteSystem = spriteSystem;
    SpriteSystem_Init(spriteSystem, &ov80_0223D5B8, &ov80_0223D570, 0x20);
    G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_128K);
    G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
    ((FrontierMapInternal *)arg0)->spriteManager = SpriteManager_New(((FrontierMapInternal *)arg0)->spriteSystem);
    SpriteSystem_InitSprites(((FrontierMapInternal *)arg0)->spriteSystem, ((FrontierMapInternal *)arg0)->spriteManager, 0x80);
    SpriteSystem_InitManagerWithCapacities(((FrontierMapInternal *)arg0)->spriteSystem, ((FrontierMapInternal *)arg0)->spriteManager, &ov80_0223D584);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(((FrontierMapInternal *)arg0)->spriteSystem), 0, 0x200000);
}
