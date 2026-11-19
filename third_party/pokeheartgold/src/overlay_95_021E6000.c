#include "global.h"

#include "filesystem.h"
#include "options.h"
#include "palette.h"
#include "bg_window.h"
#include "pokemon_types_def.h"
#include "overlay_74_main_menu_util.h"
#include "sprite.h"
#include "sprite_system.h"
#include "naming_screen.h"
#include "overlay_95_egg_hatch_app.h"
#include "struct_defs/resdat_narc_structs.h"
#include "egg_hatch_particle_system.h"

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);
enum AffineOverwriteMode {
    AFFINE_OVERWRITE_MODE_NONE = 0,
    AFFINE_OVERWRITE_MODE_NORMAL,
    AFFINE_OVERWRITE_MODE_DOUBLE,
};
ManagedSprite *SpriteSystem_NewSprite(SpriteSystem *spriteSys, SpriteManager *spriteMan, const ManagedSpriteTemplate *template);
void ManagedSprite_TickFrame(ManagedSprite *managedSprite);
void ov95_021E6000(EggHatchCutscene *eggHatch);

void ov95_021E6000(EggHatchCutscene *eggHatch) {
    ManagedSpriteTemplate spriteTemplate;

    spriteTemplate.x = 128;
    spriteTemplate.y = 120;
    spriteTemplate.z = 0;
    spriteTemplate.animation = 0;
    spriteTemplate.drawPriority = 0;
    spriteTemplate.pal = 0;
    spriteTemplate.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    spriteTemplate.bgPriority = 2;
    spriteTemplate.vramTransfer = 0;

    spriteTemplate.resIdList[GF_GFX_RES_TYPE_CHAR] = 20000;
    spriteTemplate.resIdList[GF_GFX_RES_TYPE_PLTT] = 20001;
    spriteTemplate.resIdList[GF_GFX_RES_TYPE_CELL] = 20002;
    spriteTemplate.resIdList[GF_GFX_RES_TYPE_ANIM] = 20003;
    spriteTemplate.resIdList[GF_GFX_RES_TYPE_MCEL] = -1;
    spriteTemplate.resIdList[GF_GFX_RES_TYPE_MANM] = -1;

    eggHatch->eggSprite = SpriteSystem_NewSprite(eggHatch->graphics.spriteSys, eggHatch->graphics.spriteMan, &spriteTemplate);

    ManagedSprite_TickFrame(eggHatch->eggSprite);
    ManagedSprite_SetAffineOverwriteMode(eggHatch->eggSprite, AFFINE_OVERWRITE_MODE_DOUBLE);

    spriteTemplate.x = 0;
    spriteTemplate.y = -16;
    spriteTemplate.bgPriority = 0;
    spriteTemplate.resIdList[GF_GFX_RES_TYPE_CHAR] = 25000;
    spriteTemplate.resIdList[GF_GFX_RES_TYPE_PLTT] = 25001;
    spriteTemplate.resIdList[GF_GFX_RES_TYPE_CELL] = 25002;
    spriteTemplate.resIdList[GF_GFX_RES_TYPE_ANIM] = 25003;

    eggHatch->topBarSprite = SpriteSystem_NewSprite(eggHatch->graphics.spriteSys, eggHatch->graphics.spriteMan, &spriteTemplate);
    ManagedSprite_TickFrame(eggHatch->topBarSprite);

    spriteTemplate.y = 192 - 48;

    eggHatch->bottomBarSprite = SpriteSystem_NewSprite(eggHatch->graphics.spriteSys, eggHatch->graphics.spriteMan, &spriteTemplate);
    ManagedSprite_TickFrame(eggHatch->bottomBarSprite);
}
