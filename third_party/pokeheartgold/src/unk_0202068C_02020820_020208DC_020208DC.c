#include "global.h"

#include "ascii_util.h"
#include "unk_0202068C.h"

u32 sub_020208CC(NNSG3dResTex *texture, const NNSG3dResDictTexData *textureData);

u32 sub_020208DC(NNSG3dResTex *texture, const char *name) {
    NNSG3dResName resName;
    sub_02020B3C(&resName, name);
    const NNSG3dResDictTexData *textureData = NNS_G3dGetTexDataByName(texture, &resName);

    if (textureData == NULL) {
        return 0;
    }

    return sub_020208CC(texture, textureData);
}
