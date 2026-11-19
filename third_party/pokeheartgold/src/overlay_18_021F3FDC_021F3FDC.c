#include "global.h"

#include "sprite_system.h"
#include "application/pokedex/pokedex_internal.h"

void ov18_021F3FDC(PokedexAppData *arg0);

void ov18_021F3FDC(PokedexAppData *arg0) {
    SpriteManager_UnloadCharObjById(arg0->unk_066C, 0xC550U);
    SpriteManager_UnloadPlttObjById(arg0->unk_066C, 0xC551U);
    SpriteManager_UnloadCellObjById(arg0->unk_066C, 0xC550U);
    SpriteManager_UnloadAnimObjById(arg0->unk_066C, 0xC550U);
    SpriteManager_UnloadCharObjById(arg0->unk_066C, 0xC59CU);
    SpriteManager_UnloadPlttObjById(arg0->unk_066C, 0xC55EU);
    SpriteManager_UnloadCellObjById(arg0->unk_066C, 0xC55AU);
    SpriteManager_UnloadAnimObjById(arg0->unk_066C, 0xC55AU);
    SpriteManager_UnloadCharObjById(arg0->unk_066C, 0xC59AU);
    SpriteManager_UnloadPlttObjById(arg0->unk_066C, 0xC55CU);
    SpriteManager_UnloadCellObjById(arg0->unk_066C, 0xC559U);
    SpriteManager_UnloadAnimObjById(arg0->unk_066C, 0xC559U);
}
