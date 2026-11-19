#include "global.h"

#include "filesystem_files_def.h"
#include "palette.h"
#include "pokemon_icon_idx.h"
#include "sprite_system.h"
#include "overlay_40.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

void ov40_0222FE00(UnkStruct_0208C06C *param_1);

void ov40_0222FE00(UnkStruct_0208C06C *param_1) {
    u32 uVar1;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    PaletteData *plttData;

    spriteSystem = param_1->unk_14.unk_04;
    spriteManager = param_1->unk_14.unk_08;
    plttData = param_1->unk_14.unk_14;
    uVar1 = sub_02074490();
    SpriteSystem_LoadPaletteBuffer(plttData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, uVar1, 0, 3, 1, 100000);
    uVar1 = sub_0207449C();
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, uVar1, 0, 100000);
    uVar1 = sub_020744A8();
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, uVar1, 0, 100000);
    return;
}

void ov40_0222FE68(void *arg0) {
    SpriteManager_UnloadPlttObjById(((SpriteManager *)((UnkStruct_0208C06C *)arg0)->unk_14.unk_08), 0x186A0U);
    SpriteManager_UnloadCellObjById(((SpriteManager *)((UnkStruct_0208C06C *)arg0)->unk_14.unk_08), 0x186A0U);
    SpriteManager_UnloadAnimObjById(((SpriteManager *)((UnkStruct_0208C06C *)arg0)->unk_14.unk_08), 0x186A0U);
}
