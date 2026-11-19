#include "global.h"

s32 sub_02069958();

void sub_02069964(NNSG3dRenderObj *arg0, NNSG3dResMdl *arg1);

void sub_02069964(NNSG3dRenderObj *pRenderObj, NNSG3dResMdl *pResMdl) {
    sub_02069958();
    NNS_G3dRenderObjInit(pRenderObj, pResMdl);
}
