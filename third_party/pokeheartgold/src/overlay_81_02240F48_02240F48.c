#include "global.h"

#include "overlay_81_battle_factory_app.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "wild_encounter.h"

void *ov81_02242C80(void *, u32, Pokemon *, u32, u32, u32);

void ov81_02242CB0(void *, u32);

void ov81_02240F48(BattleFactoryApp *app, s32 param_2, Pokemon *param_3, u32 param_4, u32 param_5, u32 param_6);

void ov81_02240F48(BattleFactoryApp *app, s32 param_2, Pokemon *param_3, u32 param_4, u32 param_5, u32 param_6) {
    u32 species;
    u32 form;
    s32 iVar2;

    app->monSprites[param_2] = ov81_02242C80(app->monSpriteMan, 0, param_3, param_4, param_5, 0);
    Pokepic_SetAttr(app->monSprites[param_2], 6, 0);
    species = GetMonData(param_3, 5, (void *)0x0);
    form = GetMonData(param_3, 0x70, (void *)0x0);
    iVar2 = GetMonBaseStat_HandleAlternateForm(species, form, 0x1c);
    if (iVar2 == 0) {
        ov81_02242CB0(app->monSprites[param_2], param_6);
    }
    return;
}
