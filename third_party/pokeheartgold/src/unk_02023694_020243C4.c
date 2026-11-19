#include "global.h"

#include "billboard_gfx_sequence.h"
#include "billboard.h"

void sub_02023B4C(NNSG3dResTex *texture, NNSG3dResMdlSet *modelSet, NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *plttKey);
void sub_02023B38(NNSG3dResTex *texture, NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *outPlttKey);

int sub_020243C4(const BillboardAnim *anim, fx32 *currentFrame, fx32 numFrames);
void sub_020243FC(Billboard *billboard);
void sub_0202441C(Billboard *billboard);
void *sub_0202443C(const BillboardResources *resources, int value);
const BillboardAnim *sub_02024454(const BillboardAnim *anims, int index);

int sub_020243C4(const BillboardAnim *anim, fx32 *currentFrame, fx32 numFrames) {
    int status = BILLBOARD_ANIM_STATUS_RUNNING;

    if (anim->startFrame * ((fx32)0x0000000000001000L) > *currentFrame || anim->endFrame * ((fx32)0x0000000000001000L) < *currentFrame) {
        *currentFrame = anim->startFrame * ((fx32)0x0000000000001000L);
    } else if (anim->endFrame * ((fx32)0x0000000000001000L) < *currentFrame + numFrames) {
        if (anim->animType == BILLBOARD_ANIM_TYPE_LOOP) {
            *currentFrame = anim->startFrame * ((fx32)0x0000000000001000L);
        } else {
            status = BILLBOARD_ANIM_STATUS_FINISHED;
            *currentFrame = anim->endFrame * ((fx32)0x0000000000001000L);
        }
    } else {
        *currentFrame += numFrames;
    }

    return status;
}

void sub_020243FC(Billboard *billboard) {
    sub_02023B4C(billboard->texture, billboard->modelSet, &billboard->texKey, &billboard->tex4x4Key, &billboard->plttKey);
}

void sub_0202441C(Billboard *billboard) {
    NNSG3dTexKey tex4x4Key, texKey;
    NNSG3dPlttKey dummyPlttKey;

    NNS_G3dReleaseMdlSet(billboard->modelSet);
    sub_02023B38(billboard->texture, &texKey, &tex4x4Key, &dummyPlttKey);
}

void *sub_0202443C(const BillboardResources *resources, int value) {
    void *ret;

    switch (value) {
    case BILLBOARD_RESOURCE_MODEL_RES:
        ret = resources->modelRes;
        break;
    case BILLBOARD_RESOURCE_TEXTURE:
        ret = (void *)resources->texture;
        break;
    default:
        ret = 0L;
        break;
    }

    return ret;
}

const BillboardAnim *sub_02024454(const BillboardAnim *anims, int index) {
    return anims + index;
}
