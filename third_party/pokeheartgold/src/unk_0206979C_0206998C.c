#include "global.h"
#include "simple3dmodel.h"
#include "simple3danimation.h"

void sub_02069984(Simple3DRenderObj *renderObj, NNSG3dAnmObj *anim);

void sub_0206998C(Simple3DRenderObj *renderObj, Simple3DAnimation *anim);

void sub_0206998C(Simple3DRenderObj *renderObj, Simple3DAnimation *anim) {
    sub_02069984(renderObj, anim->animObj);
}
