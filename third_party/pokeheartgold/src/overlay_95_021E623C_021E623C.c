#include "global.h"

#include "constants/heap.h"

#include "library/spl_emitter.h"
#include "library/spl_manager.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "naming_screen.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_74_main_menu_util.h"
#include "transform_data.h"
#include "pokemon_anim.h"
#include "overlay_95_egg_hatch_app.h"

void ov95_021E623C(EggHatchCutscene *eggHatch);

void ov95_021E623C(EggHatchCutscene *eggHatch) {
    u8 bVar1;
    u32 uVar2;
    Pokepic *pPVar3;
    Pokemon *mon;
    u32 uStack_50;
    PokepicTemplate PStack_24;
    PokepicAnimScript aPStack_4c[10];

    mon = (eggHatch->app->args).mon;
    uVar2 = GetMonData(mon, 5, (void *)0x0);
    bVar1 = sub_0207083C(mon, 2);
    uStack_50 = 0;
    SetMonData(mon, 0x4c, &uStack_50);
    GetPokemonSpriteCharAndPlttNarcIds(&PStack_24, mon, 2);
    NARC_ReadPokepicAnimScript((eggHatch->graphics).monDataNarc, aPStack_4c, (u16)uVar2, 1);
    pPVar3 = PokepicManager_CreatePokepic(eggHatch->graphics.monSpriteMan, &PStack_24, 0x80, bVar1 + 0x60, 0, 0, aPStack_4c, NULL);
    eggHatch->monSprite = pPVar3;
    return;
}
