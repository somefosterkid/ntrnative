#include "global.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sys_task.h"
#include "transform_data.h"
#include "pokemon_anim.h"

#include "constants/pokemon_anim.h"

void sub_020172C8(PokemonAnim *monAnim, u8 *outDestIndex, int *outOperand1, int *outOperand2) {
    u8 index1, index2, readType;

    sub_02017294(monAnim, outDestIndex);
    sub_02017280(monAnim, &readType);

    if (readType == ANIM_READ_TYPE_VALUE) {
        sub_02017294(monAnim, &index1);
        *outOperand1 = monAnim->vars[index1];
        sub_0201726C(monAnim, outOperand2);
    } else if (readType == ANIM_READ_TYPE_VAR) {
        sub_020172B4(monAnim, &index1, &index2);
        *outOperand1 = monAnim->vars[index1];
        *outOperand2 = monAnim->vars[index2];
    } else {
        ((0) ? (void)0 : GF_AssertFail());
    }
}

void sub_0201733C(PokemonAnim *monAnim, u8 *outDestIndex, int *outOperand1, int *outOperand2) {
    u8 index1, index2, readType1, readType2;

    sub_02017294(monAnim, outDestIndex);
    sub_02017280(monAnim, &readType1);
    sub_02017280(monAnim, &readType2);

    if (readType1 == ANIM_READ_TYPE_VALUE) {
        sub_0201726C(monAnim, outOperand1);
    } else if (readType1 == ANIM_READ_TYPE_VAR) {
        sub_02017294(monAnim, &index1);
        *outOperand1 = monAnim->vars[index1];
    } else {
        ((0) ? (void)0 : GF_AssertFail());
    }

    if (readType2 == ANIM_READ_TYPE_VALUE) {
        sub_0201726C(monAnim, outOperand2);
    } else if (readType2 == ANIM_READ_TYPE_VAR) {
        sub_02017294(monAnim, &index2);
        *outOperand2 = monAnim->vars[index2];
    } else {
        ((0) ? (void)0 : GF_AssertFail());
    }
}

void sub_020173C8(PokemonAnim *monAnim, u8 *outDestIndex, int *outRadians, int *outAmplitude) {
    u8 radiansIndex, amplitudeIndex, offsetIndex, readType;
    int radians, offset;

    sub_020172B4(monAnim, outDestIndex, &radiansIndex);
    radians = monAnim->vars[radiansIndex];
    sub_02017280(monAnim, &readType);

    if (readType == ANIM_READ_TYPE_VALUE2) {
        sub_0201726C(monAnim, outAmplitude);
    } else if (readType == ANIM_READ_TYPE_VAR2) {
        sub_02017294(monAnim, &amplitudeIndex);
        (*outAmplitude) = monAnim->vars[amplitudeIndex];
    } else {
        ((0) ? (void)0 : GF_AssertFail());
    }

    sub_02017280(monAnim, &readType);

    if (readType == ANIM_READ_TYPE_VALUE2) {
        sub_0201726C(monAnim, &offset);
    } else if (readType == ANIM_READ_TYPE_VAR2) {
        sub_02017294(monAnim, &offsetIndex);
        offset = monAnim->vars[offsetIndex];
    } else {
        ((0) ? (void)0 : GF_AssertFail());
    }

    *outRadians = radians + offset;
    *outRadians %= 0x10000;
}

u8 sub_02017470(int *value1, int *value2) {
    int diff = *value1 - *value2;

    if (diff < 0) {
        return COMPARISON_RESULT_LESS_THAN;
    }
    if (diff > 0) {
        return COMPARISON_RESULT_GREATER_THAN;
    }
    return COMPARISON_RESULT_EQUAL;
}
