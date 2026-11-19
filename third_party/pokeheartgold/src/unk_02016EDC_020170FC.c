#include "global.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "error_handling.h"
#include "pokepic.h"
#include "transform_data.h"
#include "pokemon_anim.h"

typedef void (*PokemonAnimCmd)(PokemonAnim *);

void sub_020179D4(PokemonAnim *monAnim);

void sub_02017A1C(PokemonAnim *monAnim);

extern const PokemonAnimCmd _020F61F8[34];

void sub_020170FC(PokemonAnim *monAnim) {
    monAnim->waitFrame = 0;
    monAnim->commandCount = 0;
    u8 inactiveTransforms = 0;

    for (u8 i = 0; i < 4; i++) {
        TransformData *transform = &(monAnim->transforms[i]);

        if (transform->active) {
            if (transform->startDelay == 0) {
                transform->func(transform, monAnim);
            } else {
                transform->startDelay--;
            }
        } else {
            inactiveTransforms++;
        }
    }

    if (inactiveTransforms == 4) {
        monAnim->waitForTransform = 0;
    }

    if (monAnim->waitForTransform) {
        sub_020179D4(monAnim);
        sub_02017A1C(monAnim);
        return;
    }

    if (monAnim->fadeActive) {
        if (!Pokepic_ResumePaletteFade(monAnim->sprite)) {
            monAnim->fadeActive = 0;
        } else {
            return;
        }
    }

    while (1) {
        monAnim->commandCount++;

        ((*(monAnim->scriptPtr) < 34) ? (void)0 : GF_AssertFail());

        PokemonAnimCmd currAnimCmd = _020F61F8[*(monAnim->scriptPtr)];
        currAnimCmd(monAnim);

        if (monAnim->endAnim) {
            break;
        } else {
            monAnim->scriptPtr++;

            if (monAnim->waitFrame) {
                break;
            } else if (monAnim->waitForTransform) {
                sub_020179D4(monAnim);
                sub_02017A1C(monAnim);
                break;
            }
        }

        if (monAnim->commandCount >= 256) {
            ((0) ? (void)0 : GF_AssertFail());

            monAnim->endAnim = 1;
            break;
        }
    }
}

int sub_020171F4(u32 *scriptPtr, u8 index, u8 one) {
    int ret = scriptPtr[index];

    if (one != 1) {
        GF_ASSERT(FALSE);
    }

    return ret;
}
