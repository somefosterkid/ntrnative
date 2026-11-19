#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "options.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_74_main_menu_util.h"
#include "overlay_95_egg_hatch_app.h"
#include "pokemon.h"
#include "pokepic.h"
#include "unk_02016EDC.h"

void ov95_021E62A4(EggHatchCutscene *eggHatch);
void ov95_021E62E4(EggHatchCutscene *eggHatch);
void ov95_021E62F0(EggHatchCutscene *eggHatch, BOOL hide);

extern BOOL (*const ov95_021E7810[2])(EggHatchCutscene *);
BOOL ov95_021E6300(EggHatchCutscene *eggHatch);

void ov95_021E62A4(EggHatchCutscene *eggHatch) {
    int species = GetMonData(eggHatch->app->args.mon, MON_DATA_SPECIES, 0L);
    int nature = GetMonNature(eggHatch->app->args.mon);

    Pokepic_StartAnim(eggHatch->monSprite, 1);
    sub_0207294C(eggHatch->graphics.monDataNarc, eggHatch->graphics.animMan, eggHatch->monSprite, species, 2, 0, 0);
}

void ov95_021E62E4(EggHatchCutscene *eggHatch) {
    Pokepic_Delete(eggHatch->monSprite);
}

void ov95_021E62F0(EggHatchCutscene *eggHatch, BOOL hide) {
    Pokepic_SetAttr(eggHatch->monSprite, POKEPIC_VANISHED, hide);
}

BOOL ov95_021E6300(EggHatchCutscene *eggHatch) {
    return ov95_021E7810[eggHatch->isManaphy](eggHatch);
}
