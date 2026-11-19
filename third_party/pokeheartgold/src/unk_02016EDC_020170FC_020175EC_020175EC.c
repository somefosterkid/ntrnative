#include "global.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sys_task.h"
#include "transform_data.h"
#include "pokemon_anim.h"

#include "constants/pokemon_anim.h"

void sub_020175EC(PokemonAnim *monAnim);

void sub_020175EC(PokemonAnim *monAnim) {
    u8 index1, index2, condition, comparisonResult, readType;
    int value1, value2;

    sub_02017280(monAnim, &readType);

    if (readType == ANIM_READ_TYPE_VALUE2) {
        sub_02017294(monAnim, &index1);
        value1 = monAnim->vars[index1];
        sub_0201726C(monAnim, &value2);
    } else if (readType == ANIM_READ_TYPE_VAR2) {
        sub_020172B4(monAnim, &index1, &index2);
        value1 = monAnim->vars[index1];
        value2 = monAnim->vars[index2];
    } else {
        ((0) ? (void)0 : GF_AssertFail());
    }

    sub_02017280(monAnim, &condition);
    ((condition <= COMPARISON_RESULT_EQUAL) ? (void)0 : GF_AssertFail());

    comparisonResult = sub_02017470(&value1, &value2);

    int newValue;

    sub_02017280(monAnim, &readType);

    if (readType == ANIM_READ_TYPE_VALUE2) {
        sub_02017294(monAnim, &index1);
        sub_0201726C(monAnim, &newValue);
    } else if (readType == ANIM_READ_TYPE_VAR2) {
        sub_020172B4(monAnim, &index1, &index2);
        newValue = monAnim->vars[index2];
    } else {
        ((0) ? (void)0 : GF_AssertFail());
    }

    if (condition == comparisonResult) {
        monAnim->vars[index1] = newValue;
    }
}
