#include "global.h"
struct UnkStruct_ov112_021EA230;

#include "constants/heap.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "sprite_system.h"
#include "overlay_112_unk_struct_ov112_021_ea230.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021EA2C4(Ov112PokewalkerApp *param_1);

void ov112_021EA2C4(Ov112PokewalkerApp *param_1) {
    NARC *narc;

    narc = NARC_New(NARC_a_2_4_7, HEAP_ID_154);
    SpriteSystem_LoadCharResObjFromOpenNarc(param_1->spriteSystem, param_1->spriteManager, narc, 7, 1, 2, 0xc226);
    SpriteSystem_LoadPlttResObjFromOpenNarc(param_1->spriteSystem, param_1->spriteManager, narc, 3, 0, 2, 2, 0xc226);
    SpriteSystem_LoadCellResObjFromOpenNarc(param_1->spriteSystem, param_1->spriteManager, narc, 0x14, 0, 0xc226);
    SpriteSystem_LoadAnimResObjFromOpenNarc(param_1->spriteSystem, param_1->spriteManager, narc, 0x16, 0, 0xc226);
    SpriteSystem_LoadCharResObjFromOpenNarc(param_1->spriteSystem, param_1->spriteManager, narc, 6, 1, 1, 0xc227);
    SpriteSystem_LoadPlttResObjFromOpenNarc(param_1->spriteSystem, param_1->spriteManager, narc, 2, 0, 7, 1, 0xc227);
    SpriteSystem_LoadCellResObjFromOpenNarc(param_1->spriteSystem, param_1->spriteManager, narc, 0x13, 0, 0xc227);
    SpriteSystem_LoadAnimResObjFromOpenNarc(param_1->spriteSystem, param_1->spriteManager, narc, 0x15, 0, 0xc227);
    NARC_Delete(narc);
    return;
}
