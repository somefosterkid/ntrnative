#include "global.h"
#include "sprite_resources_header.h"

#include "bg_window.h"
#include "save.h"
#include "screen_fade.h"
#include "unk_0200A090.h"
#include "font.h"
#include "overlay_74_main_menu_util.h"
#include "struct_defs/resdat_narc_structs.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"

void G2dRenderer_SetSubSurfaceCoords(G2dRenderer *renderer, fx32 x, fx32 y);

void ov74_0223589C(s32 arg0, s32 arg1);

extern void ov74_022358C8(OSArenaId id);
void ov74_022358BC(void);

void ov74_0223589C(s32 x, s32 y) {
    G2dRenderer_SetSubSurfaceCoords(&ov74_0223D454.spriteManager.renderer, x, y);
    ov74_0223D454.bottomScreenTopY = y;
}

void ov74_022358BC(void) {
    ov74_022358C8(OS_ARENA_MAIN);
}
