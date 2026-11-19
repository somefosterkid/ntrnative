#include "global.h"

#include "filesystem.h"
#include "pokemon_icon_idx.h"
#include "sprite_system.h"
#include "overlay_111.h"

void ov111_021E6180(BugContestSwapMon *arg0);

void ov111_021E6180(BugContestSwapMon *arg0) {
    NARC *narc;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    s32 v0;

    spriteSystem = arg0->spriteSystem;
    spriteManager = arg0->spriteManager;
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 5, 0, 2, 1, 0);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 6, 1, 1, 0);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 7, 1, 0);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 8, 1, 0);
    narc = NARC_New(NARC_poketool_icongra_poke_icon, arg0->heapID);
    v0 = 0;
    do {
        SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 9, 0, 1, v0 + 1);
        v0 += 1;
    } while (v0 < 2);
    arg0->iconPalette = SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, narc, (s32)sub_02074490(), 0, 3, 1, 1);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, (s32)sub_0207449C(), 0, 1);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, (s32)sub_020744A8(), 0, 1);
    NARC_Delete(narc);
}
