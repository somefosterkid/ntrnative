#include "global.h"
#include "simple3dmodel.h"

#include "unk_0206979C.h"

void sub_02069964(NNSG3dRenderObj *renderObj, NNSG3dResMdl *resMdl);

void sub_02069978(NNSG3dRenderObj *renderObj, Simple3DModel *model) {
    sub_02069964(renderObj, model->g3DModel);
}
