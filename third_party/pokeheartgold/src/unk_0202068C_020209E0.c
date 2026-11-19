#include "global.h"

#include "unk_0202068C.h"

void sub_02020B3C(NNSG3dResName *resource, const char *src);
int sub_0202094C(NNSG3dResTex *texture, const NNSG3dResDictTexData *textureData);

int sub_020209E0(NNSG3dResTex *texture, const char *name) {
    NNSG3dResName resName;
    sub_02020B3C(&resName, name);
    const NNSG3dResDictTexData *textureData = NNS_G3dGetTexDataByName(texture, &resName);

    return sub_0202094C(texture, textureData);
}
