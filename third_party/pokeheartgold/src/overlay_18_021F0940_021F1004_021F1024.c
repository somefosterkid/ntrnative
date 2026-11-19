#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite_system.h"
#include "unk_02009D48.h"

extern const OamManagerParam ov18_021FA3C8;

extern const OamCharTransferParam ov18_021FA36C;

extern const SpriteResourceCountsListUnion ov18_021FA380;

void ov18_021F1024(PokedexAppData *pokedexApp);

void ov18_021F1024(PokedexAppData *pokedexApp) {
    SpriteSystem **pSpriteSys = &pokedexApp->unk_0668; /* struct-layout: SpriteSystem * at 0x668 missing */
    *pSpriteSys = SpriteSystem_Alloc((enum HeapID)37);
    pokedexApp->unk_066C = SpriteManager_New(*pSpriteSys);
    {
        OamManagerParam v0 = ov18_021FA3C8;
        OamCharTransferParam v1 = ov18_021FA36C;

        SpriteSystem_Init(*pSpriteSys, &v0, &v1, 32);
    }
    {
        SpriteResourceCountsListUnion v3 = ov18_021FA380;

        SpriteSystem_InitSprites(*pSpriteSys, pokedexApp->unk_066C, 120);
        SpriteSystem_InitManagerWithCapacities(*pSpriteSys, pokedexApp->unk_066C, &v3);
    }
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(*pSpriteSys), 0, 0x200000);
}
