#include "global.h"

#include "overlay_manager.h"

#include "bg_window.h"
#include "filesystem.h"
#include "options.h"
#include "palette.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_95_egg_hatch_app.h"
#include "constants/species.h"
#include "pokemon.h"
#include "pokepic.h"
#include "unk_02016EDC.h"

void ov95_021E5B58(void);
BOOL ov95_021E6300(EggHatchCutscene *eggHatch);

BOOL HatchEggApp_Main(OverlayManager *appMan, int *state);

BOOL HatchEggApp_Main(OverlayManager *appMan, int *state) {
    EggHatchCutscene *eggHatch = OverlayManager_GetData(appMan);

    switch (*state) {
    case 0:
        if (IsPaletteFadeFinished() == 1) {
            (*state)++;
        }

        int species = GetMonData(eggHatch->app->args.mon, MON_DATA_SPECIES, 0L);

        sub_020729A4(eggHatch->graphics.monDataNarc, &eggHatch->cryDelay, species, 1);

        if (species == SPECIES_MANAPHY) {
            eggHatch->isManaphy = 1;
        } else {
            eggHatch->isManaphy = 0;
        }
        break;
    case 1: {
        if (!ov95_021E6300(eggHatch)) {
            (*state)++;
            break;
        }

        PokepicManager_DrawAll(eggHatch->graphics.monSpriteMan);
        ov95_021E5B58();
    } break;
    default:
        if (IsPaletteFadeFinished() == 1) {
            return 1;
        }

        break;
    }

    return 0;
}
