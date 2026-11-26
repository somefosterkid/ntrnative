#include "global.h"

#include "constants/heap.h"

#include "library/spl_emitter.h"
#include "library/spl_manager.h"

#include "bg_window.h"
#include "camera.h"
#include "error_handling.h"
#include "filesystem.h"
#include "naming_screen.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "screen_fade.h"
#include "sound_02004A44.h"
#include "sound_chatot.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "text.h"
#include "unk_02005D10.h"
#include "unk_02016EDC.h"
#include "overlay_74_main_menu_util.h"
#include "transform_data.h"
#include "pokemon_anim.h"
#include "overlay_95_egg_hatch_app.h"

void ov95_021E5B7C(void);

void ov95_021E5B9C(void);

EggHatchParticleSystem *ov95_021E5E90(EggHatchParticleSystemArgs *args);

void ov95_021E5EF8(EggHatchCutscene *eggHatch);

void ov95_021E623C(EggHatchCutscene *eggHatch);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov95_021E5BBC(BgConfig *, PaletteData *, u8);
#else
void ov95_021E5BBC(BgConfig *, PaletteData *, s32);
#endif

void ov95_021E5C44(BgConfig *, Window *, u32, u32, u32, u32, u32, u32, u32);

u8 ov95_021E5CAC(Window *, u32, Pokemon *, s32);

void ov95_021E5D34(Window *);

void ov95_021E5D44(BgConfig *, PaletteData *);

void ov95_021E5E40(ParticleSystem *);

void ov95_021E5EF0(EggHatchParticleSystem *);

void ov95_021E6000(EggHatchCutscene *);

void ov95_021E6184(EggHatchCutscene *);

void ov95_021E62A4(EggHatchCutscene *);

void ov95_021E62E4(EggHatchCutscene *);

void ov95_021E62F0(EggHatchCutscene *, u32);

BOOL ov95_021E6B74(EggHatchCutscene *);

void ov95_021E70BC(EggHatchPrompt *, PaletteData *, u32, u32, u32, u32);

void ov95_021E7208(EggHatchPrompt *, u32, u32);

u8 ov95_021E7258(EggHatchPrompt *);

BOOL ov95_021E65A0(EggHatchCutscene *eggHatch);

BOOL ov95_021E65A0(EggHatchCutscene *eggHatch) {
    BOOL inProgress = TRUE;

    switch (eggHatch->state) {
    case 0:
        ov95_021E5D44(eggHatch->graphics.bgConfig, eggHatch->graphics.plttData);
        ov95_021E5BBC(eggHatch->graphics.bgConfig, eggHatch->graphics.plttData, eggHatch->graphics.frame);
        ov95_021E5C44(eggHatch->graphics.bgConfig, &eggHatch->graphics.windows[0], 1, 2, 19, 27, 4, 59, 14);
        ov95_021E5EF8(eggHatch);
        ov95_021E6000(eggHatch);
        ManagedSprite_SetDrawFlag(eggHatch->eggSprite, FALSE);
        ov95_021E70BC(eggHatch->prompt, eggHatch->graphics.plttData, 21000, 21001, 21002, 21003);
        ov95_021E623C(eggHatch);
        ov95_021E62F0(eggHatch, TRUE);
        {
            EggHatchParticleSystemArgs epsArgs;
            epsArgs.heapID = HEAP_ID_70;
            epsArgs.narcIdx = 1;
            eggHatch->graphics.eps = ov95_021E5E90(&epsArgs);
        }
        eggHatch->state++;
        break;
    case 1:
        ov95_021E5B7C();
        eggHatch->state++;
    case 2:
        if (IsPaletteFadeFinished() == TRUE) {
            Sound_SetSceneAndPlayBGM(13, 0x3F3, 1);
            eggHatch->state++;
        }
        break;
    case 3:
        if (!ov95_021E6B74(eggHatch)) {
            ov95_021E62A4(eggHatch);
            {
                int species = GetMonData(eggHatch->app->args.mon, MON_DATA_SPECIES, NULL);
                int form = GetMonData(eggHatch->app->args.mon, MON_DATA_FORM, NULL);
                PlayCry(species, form);
            }
            eggHatch->graphics.printerID = ov95_021E5CAC(&eggHatch->graphics.windows[0], 0, eggHatch->app->args.mon, eggHatch->graphics.renderDelay);
            eggHatch->state = 8;
        }
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        GF_ASSERT(FALSE);
        break;
    case 8:
        if (IsCryFinished()) {
            break;
        }
        PlayFanfare(0x4A4);
        eggHatch->state++;
    case 9:
        if (IsFanfarePlaying()) {
            break;
        }
        eggHatch->state++;
    case 10:
        if (TextPrinterCheckActive(eggHatch->graphics.printerID)) {
            break;
        }
        eggHatch->graphics.printerID = ov95_021E5CAC(&eggHatch->graphics.windows[0], 1, eggHatch->app->args.mon, eggHatch->graphics.renderDelay);
        eggHatch->state++;
    case 11:
        if (TextPrinterCheckActive(eggHatch->graphics.printerID)) {
            break;
        }
        if (sub_02017068(eggHatch->graphics.animMan, 0) != TRUE) {
            break;
        }
        if (Pokepic_IsAnimActive(eggHatch->monSprite)) {
            break;
        }
        ov95_021E7208(eggHatch->prompt, 2, 3);
        eggHatch->state++;
        break;
    case 12:
        switch (ov95_021E7258(eggHatch->prompt)) {
        case 1:
            eggHatch->app->nicknameMon = TRUE;
            eggHatch->state++;
            break;
        case 2:
            eggHatch->app->nicknameMon = FALSE;
            eggHatch->state++;
            break;
        }
        break;
    case 13:
        eggHatch->state++;
        break;
    case 14:
        eggHatch->state++;
        break;
    case 15:
        ov95_021E5B9C();
        eggHatch->state++;
    case 16:
        if (IsPaletteFadeFinished() == TRUE) {
            eggHatch->state++;
        }
        break;
    case 17:
    default:
        ov95_021E62E4(eggHatch);
        ov95_021E6184(eggHatch);
        ov95_021E5D34(&eggHatch->graphics.windows[0]);
        ov95_021E5E40(eggHatch->graphics.eps->ps);
        ov95_021E5EF0(eggHatch->graphics.eps);
        inProgress = FALSE;
        break;
    }

    SpriteSystem_DrawSprites(eggHatch->graphics.spriteMan);
    return inProgress;
}
