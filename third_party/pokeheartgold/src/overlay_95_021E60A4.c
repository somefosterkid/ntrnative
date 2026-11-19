#include "global.h"

#include "bg_window.h"
#include "options.h"
#include "sprite_system.h"
#include "filesystem.h"
#include "palette.h"
#include "sprite.h"
#include "pokemon_types_def.h"
#include "overlay_74_main_menu_util.h"
#include "naming_screen.h"
#include "overlay_95_egg_hatch_app.h"
#include "egg_hatch_particle_system.h"

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

enum EggHatchShakeAnimID {
    EGG_MINOR_SHAKE = 0,
    EGG_BIG_SHAKE,
    EGG_BIGGER_SHAKE,
    EGG_EXPLOSIVE_SHAKE,
};

extern const s16 ov95_021E75DC[4][10];

extern const s16 ov95_021E762C[4][10];

extern const f32 ov95_021E76D0[4][10];

extern const f32 ov95_021E7770[4][10];

BOOL ov95_021E60A4(EggHatchCutscene *eggHatch, enum EggHatchShakeAnimID idx);

int ov95_021E6150(EggHatchCutscene *eggHatch, enum EggHatchShakeAnimID idx);
void ov95_021E6184(EggHatchCutscene *eggHatch);

BOOL ov95_021E60A4(EggHatchCutscene *eggHatch, enum EggHatchShakeAnimID idx) {
    EggHatchShakeAnimation *anim = &eggHatch->shake;

    switch (anim->isAnimFinished) {
    case FALSE:
        if (anim->shakeProgress >= 10) {
            anim->isAnimFinished++;
            anim->shakeProgress = 0;
        } else {
            int finishedPieces = 0;

            s16 x = ov95_021E75DC[idx][anim->shakeProgress];
            s16 y = ov95_021E762C[idx][anim->shakeProgress];
            f32 xScale = ov95_021E76D0[idx][anim->shakeProgress];
            f32 yScale = ov95_021E7770[idx][anim->shakeProgress];

            if (x == 0xFF) {
                x = 0;
                finishedPieces++;
            }

            if (y == 0xFF) {
                y = 0;
                finishedPieces++;
            }

            ManagedSprite_OffsetPositionXY(eggHatch->eggSprite, x, y);

            if (xScale != 0.0f) {
                ManagedSprite_SetAffineScale(eggHatch->eggSprite, xScale, yScale);
            } else {
                finishedPieces++;
            }

            if (finishedPieces != 3) {
                anim->shakeProgress++;
                break;
            } else {
                anim->isAnimFinished++;
            }
        }
    default:
        anim->isAnimFinished = FALSE;
        anim->shakeProgress = 0;
        return FALSE;
    }

    return TRUE;
}

int ov95_021E6150(EggHatchCutscene *eggHatch, enum EggHatchShakeAnimID idx) {
    if (ov95_021E60A4(eggHatch, idx) == 0) {
        return 2;
    }

    if (idx == 4) {
        if (eggHatch->shake.shakeProgress == 6) {
            return 1;
        }
    } else {
        if (eggHatch->shake.shakeProgress == 6) {
            return 1;
        }
    }

    return 0;
}

void ov95_021E6184(EggHatchCutscene *eggHatch) {
    Sprite_DeleteAndFreeResources(eggHatch->eggSprite);
    Sprite_DeleteAndFreeResources(eggHatch->topBarSprite);
    Sprite_DeleteAndFreeResources(eggHatch->bottomBarSprite);
}
