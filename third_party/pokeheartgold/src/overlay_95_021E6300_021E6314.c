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

BOOL ov95_021E67F0(EggHatchCutscene *eggHatch);

BOOL ov95_021E6838(EggHatchCutscene *eggHatch);

BOOL ov95_021E68A8(EggHatchCutscene *eggHatch);

BOOL ov95_021E6900(EggHatchCutscene *eggHatch);

BOOL ov95_021E6964(EggHatchCutscene *eggHatch);

void ov95_021E70BC(EggHatchPrompt *, PaletteData *, u32, u32, u32, u32);

void ov95_021E7208(EggHatchPrompt *, u32, u32);

u8 ov95_021E7258(EggHatchPrompt *);

BOOL ov95_021E6314(EggHatchCutscene *eggHatch);

BOOL ov95_021E6314(EggHatchCutscene *eggHatch) {
    BOOL ret;
    EggHatchParticleSystemArgs args;
    u32 species;
    int state;

    state = eggHatch->state;
    ret = TRUE;

    switch (state) {
    case 0:
        ov95_021E5D44(eggHatch->graphics.bgConfig, eggHatch->graphics.plttData);
        ov95_021E5BBC(eggHatch->graphics.bgConfig, eggHatch->graphics.plttData, eggHatch->graphics.frame);
        ov95_021E5C44(eggHatch->graphics.bgConfig, eggHatch->graphics.windows, 1, 2, 0x13, 0x1b, 4, 0x3b, 0xe);
        ov95_021E5EF8(eggHatch);
        ov95_021E6000(eggHatch);
        ov95_021E70BC(eggHatch->prompt, eggHatch->graphics.plttData, 0x5208, 0x5209, 0x520A, 0x520B);
        ov95_021E623C(eggHatch);
        ov95_021E62F0(eggHatch, 1);
        args.heapID = HEAP_ID_70;
        args.narcIdx = 0;
        eggHatch->graphics.eps = ov95_021E5E90(&args);
        eggHatch->state++;
        break;
    case 1:
        ov95_021E5B7C();
        eggHatch->state++;
    case 2:
        if (IsPaletteFadeFinished() == TRUE) {
            Sound_Stop();
            Sound_SetSceneAndPlayBGM(0xd, 0x3f3, 1);
            eggHatch->state++;
        }
        break;
    case 3:
        state = ov95_021E67F0(eggHatch);
        if (state == 0) {
            eggHatch->state++;
        }
        break;
    case 4:
        state = ov95_021E6838(eggHatch);
        if (state == 0) {
            eggHatch->state++;
        }
        break;
    case 5:
        state = ov95_021E68A8(eggHatch);
        if (state == 0) {
            eggHatch->state++;
        }
        break;
    case 6:
        state = ov95_021E6900(eggHatch);
        if (state == 0) {
            eggHatch->state++;
        }
        break;
    case 7:
        if (ov95_021E6964(eggHatch) == 0) {
            ov95_021E62A4(eggHatch);
            species = GetMonData(eggHatch->app->args.mon, MON_DATA_SPECIES, NULL);
            PlayCry(species, GetMonData(eggHatch->app->args.mon, MON_DATA_FORM, NULL));
            eggHatch->graphics.printerID = ov95_021E5CAC(eggHatch->graphics.windows, 0, eggHatch->app->args.mon, eggHatch->graphics.renderDelay);
            eggHatch->state++;
        }
        break;
    case 8:
        if (IsCryFinished()) {
            break;
        }
        PlayFanfare(0x4a4);
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
        eggHatch->graphics.printerID = ov95_021E5CAC(eggHatch->graphics.windows, 1, eggHatch->app->args.mon, eggHatch->graphics.renderDelay);
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
        state = ov95_021E7258(eggHatch->prompt);
        switch (state) {
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
        eggHatch->state = state + 1;
        break;
    case 14:
        eggHatch->state = state + 1;
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
        ov95_021E5D34(eggHatch->graphics.windows);
        ov95_021E5E40(eggHatch->graphics.eps->ps);
        ov95_021E5EF0(eggHatch->graphics.eps);
        ret = FALSE;
        break;
    }

    SpriteSystem_DrawSprites(eggHatch->graphics.spriteMan);
    return ret;
}
