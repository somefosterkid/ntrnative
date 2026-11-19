#include "global.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sys_task.h"
#include "error_handling.h"
#include "transform_data.h"
#include "pokemon_anim.h"

#include "constants/pokemon_anim.h"

void sub_02017D20(TransformData *transform, PokemonAnim *monAnim);
void sub_02017DD8(TransformData *transform, PokemonAnim *monAnim);
void sub_02017E98(TransformData *transform, PokemonAnim *monAnim);
void sub_02017ED4(TransformData *transform, PokemonAnim *monAnim);
void sub_02017F10(TransformData *transform, PokemonAnim *monAnim);

#if defined(SDK_PORT) || defined(NONMATCHING)
// asm/unk_02016EDC.s keeps the DS copy: five { .word SYMBOL, paramCount, transformTypeIndex } records, which the 64-bit port emits as
// an 8-byte pointer + two 4-byte words, cut into three labels (_020F61BC/_020F61C0/_020F61C4). The PC build indexes this C table
// (80 bytes, real function pointers) instead; sub_02017220 and sub_02017C78 read _020F61BC[funcType].
const TransformFuncParameters _020F61BC[5] = {
    { sub_02017D20, 6, 1 },
    { sub_02017DD8, 6, 1 },
    { sub_02017E98, 4, 0 },
    { sub_02017ED4, 3, 0 },
    { sub_02017F10, 4, 0 },
};
#endif

void sub_02017C78(PokemonAnim *monAnim, int funcType) {
    TransformData *transform = sub_02017220(monAnim, funcType);

    sub_02017280(monAnim, &transform->calcType);
    sub_02017280(monAnim, &transform->startDelay);

    for (u8 i = 0; i < _020F61BC[funcType].paramCount; i++) {
        sub_0201726C(monAnim, &transform->vars[i]);
    }

    int index = _020F61BC[funcType].transformTypeIndex;
    sub_02017BF8(transform->vars[index], transform, monAnim);

    if (transform->startDelay == 0) {
        transform->func(transform, monAnim);
    } else {
        transform->startDelay--;
    }
}

void sub_02017D20(TransformData *transform, PokemonAnim *monAnim) {
    int *vars = transform->vars;
    u16 radians = (vars[3] * (vars[6] + 1)) + vars[4];

    switch (vars[0]) {
    case TRANSFORM_CURVE_SIN:
        *transform->transformMemberPtr = FX_Whole(FX_SinIdx(radians) * vars[2]);
        break;
    case TRANSFORM_CURVE_COS:
        *transform->transformMemberPtr = FX_Whole(FX_CosIdx(radians) * vars[2]);
        break;
    case TRANSFORM_CURVE_NEGATIVE_SIN:
        *transform->transformMemberPtr = -FX_Whole(FX_SinIdx(radians) * vars[2]);
        break;
    case TRANSFORM_CURVE_NEGATIVE_COS:
        *transform->transformMemberPtr = -FX_Whole(FX_CosIdx(radians) * vars[2]);
        break;
    default:
        GF_ASSERT(FALSE);
    }

    sub_02017BC8(transform->calcType, &(transform->originalValue), transform->transformMemberPtr, transform->animMemberPtr);

    vars[6]++;

    if (vars[6] >= vars[5]) {
        transform->active = FALSE;
    }
}

void sub_02017DD8(TransformData *transform, PokemonAnim *monAnim) {
    int *vars = transform->vars;
    u16 radians = ((vars[3] * (vars[6] + 1)) / vars[5]) + vars[4];

    switch (vars[0]) {
    case TRANSFORM_CURVE_SIN:
        *transform->transformMemberPtr = FX_Whole(FX_SinIdx(radians) * vars[2]);
        break;
    case TRANSFORM_CURVE_COS:
        *transform->transformMemberPtr = FX_Whole(FX_CosIdx(radians) * vars[2]);
        break;
    case TRANSFORM_CURVE_NEGATIVE_SIN:
        *transform->transformMemberPtr = -FX_Whole(FX_SinIdx(radians) * vars[2]);
        break;
    case TRANSFORM_CURVE_NEGATIVE_COS:
        *transform->transformMemberPtr = -FX_Whole(FX_CosIdx(radians) * vars[2]);
        break;
    default:
        GF_ASSERT(FALSE);
    }

    sub_02017BC8(transform->calcType, &(transform->originalValue), transform->transformMemberPtr, transform->animMemberPtr);

    vars[6]++;

    if (vars[6] >= vars[5]) {
        transform->active = FALSE;
    }
}

void sub_02017E98(TransformData *transform, PokemonAnim *monAnim) {
    int *vars = transform->vars;
    int distance = vars[1] + (vars[2] * vars[4]);

    *transform->transformMemberPtr += distance;

    sub_02017BC8(transform->calcType, &(transform->originalValue), transform->transformMemberPtr, transform->animMemberPtr);

    vars[4]++;

    if (vars[4] >= vars[3]) {
        transform->active = FALSE;
    }
}

void sub_02017ED4(TransformData *transform, PokemonAnim *monAnim) {
    int *vars = transform->vars;
    int distance = ((vars[3] + 1) * vars[1]) / vars[2];

    *transform->transformMemberPtr = distance;

    sub_02017BC8(transform->calcType, &(transform->originalValue), transform->transformMemberPtr, transform->animMemberPtr);

    vars[3]++;

    if (vars[3] >= vars[2]) {
        transform->active = FALSE;
    }
}

void sub_02017F10(TransformData *transform, PokemonAnim *monAnim) {
    int *vars = transform->vars;
    int distance = vars[1] + (vars[2] * vars[4]);

    *transform->transformMemberPtr += distance;

    if (transform->calcType == TRANSFORM_CALC_SET || transform->calcType == TRANSFORM_CALC_INCREMENT) {
        if (distance < 0) {
            if (*transform->transformMemberPtr <= vars[3]) {
                *transform->transformMemberPtr = vars[3];
                transform->active = FALSE;
            }
        } else {
            if (*transform->transformMemberPtr >= vars[3]) {
                *transform->transformMemberPtr = vars[3];
                transform->active = FALSE;
            }
        }
    } else if (transform->calcType == TRANSFORM_CALC_ADD) {
        int v2 = transform->originalValue + *transform->transformMemberPtr;

        if (distance < 0) {
            if (v2 <= vars[3]) {
                *transform->transformMemberPtr += (vars[3] - v2);
                transform->active = FALSE;
            }
        } else {
            if (v2 >= vars[3]) {
                *transform->transformMemberPtr -= (v2 - vars[3]);
                transform->active = FALSE;
            }
        }
    } else {
        GF_ASSERT(FALSE);
    }

    sub_02017BC8(transform->calcType, &(transform->originalValue), transform->transformMemberPtr, transform->animMemberPtr);

    vars[4]++;
}
