#include "global.h"

#include "error_handling.h"
#include "transform_data.h"
#include "pokemon_anim.h"

#include "constants/pokemon_anim.h"

void sub_02017BC8(u8 calcType, int *originalValue, int *currentValue, int *nextValue) {
    if (calcType == TRANSFORM_CALC_SET) {
        *nextValue = *currentValue;
    } else if (calcType == TRANSFORM_CALC_ADD) {
        *nextValue = *originalValue + *currentValue;
    } else if (calcType == TRANSFORM_CALC_INCREMENT) {
        *nextValue += *currentValue;
    } else {
        GF_ASSERT(FALSE);
    }
}

void sub_02017BF8(u8 transformType, TransformData *transform, PokemonAnim *monAnim) {
    switch (transformType) {
    case TRANSFORM_TYPE_OFFSET_X:
        transform->transformMemberPtr = &transform->offsetX;
        transform->animMemberPtr = &monAnim->offsetX;
        transform->originalValue = monAnim->offsetX;
        return;
    case TRANSFORM_TYPE_OFFSET_Y:
        transform->transformMemberPtr = &transform->offsetY;
        transform->animMemberPtr = &monAnim->offsetY;
        transform->originalValue = monAnim->offsetY;
        return;
    case TRANSFORM_TYPE_SCALE_X:
        transform->transformMemberPtr = &transform->scaleX;
        transform->animMemberPtr = &monAnim->scaleX;
        transform->originalValue = monAnim->scaleX;
        return;
    case TRANSFORM_TYPE_SCALE_Y:
        transform->transformMemberPtr = &transform->scaleY;
        transform->animMemberPtr = &monAnim->scaleY;
        transform->originalValue = monAnim->scaleY;
        return;
    case TRANSFORM_TYPE_ROTATION_Z:
        transform->transformMemberPtr = &transform->rotationZ;
        transform->animMemberPtr = &monAnim->rotationZ;
        transform->originalValue = monAnim->rotationZ;
        return;
    default:
        GF_AssertFail();
        return;
    }
}
