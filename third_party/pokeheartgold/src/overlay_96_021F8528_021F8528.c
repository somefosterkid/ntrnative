#include "global.h"

#include "filesystem_files_def.h"
#include "overlay_96_unk_struct_ov96_021_f7934.h"
#include "sprite_system.h"

void ov96_021F8528(UnkStruct_ov96_021F7934 *param_1);

void ov96_021F8528(UnkStruct_ov96_021F7934 *param_1) {
    SpriteManager *new_var2;
    int new_var;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    spriteSystem = param_1->spriteSystem;
    spriteManager = param_1->spriteManager;
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 9, 1, 2, 1000);
    new_var = 1000;
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 8, 0, 1, 2, new_var);
    new_var2 = spriteManager;
    SpriteSystem_LoadCellResObj(spriteSystem, new_var2, NARC_a_1_5_4, 10, 1, 1000);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_1_5_4, 0xb, 1, new_var);
    return;
}
